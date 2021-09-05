/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/spatial/Fortress.h"
#include "dkmage/Draw.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        bool HeroFortressMode::generate() {
            Level& level = map.level;

            level.generateRandomMap( 9 );

//            generateCaves( 28 );

            spatial::Fortress enemyFortress( map, parameters );
            if ( enemyFortress.generate() == false ) {
//            if ( enemyFortress.generateTest() == false ) {
                return false;
            };

            preparePlayerDungeon();

            LOG() << "preparing script";
            prepareScript();

            return true;
        }

        bool HeroFortressMode::check() {
//            if ( level.countClaimAreas() > 0 ) {
//                LOG() << "map problem found: unclaimable areas";
//                return false;
//            }

            return true;
        }

        void HeroFortressMode::generateCaves( const std::size_t cavesNum ) {
            Level& level = map.level;

            const Rect mapRect = raw::RawLevel::mapRect();
            const Point mapCenter = mapRect.center();
            const Rect region( mapCenter + Point(0, 8), 70, 33 );

            const int regionArea = region.area();
            if ( regionArea < 0 ) {
                LOG() << "invalid region area: " << regionArea;
                return ;
            }

            std::set< std::size_t > indexSet;
            for ( int i=0; i<regionArea; ++i ) {
                indexSet.insert( i );
            }

            const std::size_t trapsNum = std::min( (std::size_t)regionArea, cavesNum );

            for ( std::size_t i=0; i<trapsNum; ++i ) {
                const int itemIndex = rng_randi( indexSet.size() );
                const int regionIndex = get_item( indexSet, itemIndex, true );
                const int rX = regionIndex % region.width();
                const int rY = regionIndex / region.width();

                const Point caveCenter = region.min + Point( rX, rY );
                const Slab centerStab = level.getSlab( caveCenter );
                if ( centerStab == Slab::S_ROCK ) {
                    continue ;
                }

                const Rect chamber( caveCenter, 5, 5 );
                level.setCave( chamber, Slab::S_PATH, 12 );

                const int centerDistance = std::abs(mapCenter.y - caveCenter.y);
                const double centerFactor = ( 1.0 - 2.0 * centerDistance / region.height() );        /// in range [0.5, 1.0]
                const int creatureLevel = centerFactor * 7 + 3;
                drawHeroTrap( level, caveCenter, creatureLevel );
            }
        }

        void HeroFortressMode::preparePlayerDungeon() {
//            adiktedpp::script::Script& script = map.script;

            spatial::EvilDungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            dungeon.generate( 1, 5 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                ///dungeon.limitNorth = 0;
                ///dungeon.limitSouth = 0;
                ///dungeon.generate( 4, 5 );

                spatial::EvilRoom* heart = dungeon.room( 0 );
                dungeon.addRoom( Room::R_TREASURE, 5, *heart, spatial::Direction::D_WEST );
                spatial::EvilRoom* hatchery = dungeon.addRoom( Room::R_HATCHERY, 5, *heart, spatial::Direction::D_EAST );
                dungeon.addRoom( Room::R_LAIR, 7, *hatchery, spatial::Direction::D_EAST );
            }

            dungeon.moveToBottomEdge( 4 );

            {
                /// randomize dungeon position
                Rect randRect( 21, 9 );
                randRect.centerize();
                randRect.moveBottomTo( 0 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point center = randRect.pointByIndex( randomPosIndex );
                dungeon.move( center );
            }

            Level& level = map.level;

            /// add gold vein
            generateGoldSlabs( 40, 1, 1, 77 );

            /// dungeon have to be drawn before placing items inside it's rooms
            dungeon.draw( level );

            const spatial::EvilRoom* heart = dungeon.room( 0 );
            evilHeartPosition = heart->position().center();

            {
                /// add neutral portal
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect randRect( 18, 11 );
                randRect.moveRightTo( mapRect.max.x - 7 );
                randRect.moveBottomTo( mapRect.max.y - 3 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point portalCenter = randRect.pointByIndex( randomPosIndex );
                const Rect portalRect( portalCenter, 3, 3 );
                level.setRoom( portalRect, Room::R_PORTAL );
            }

            /// add other
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
                level.setItem( revPos, 4, Item::I_SPECIAL_REVMAP );

                const adiktedpp::Player player = dungeon.owner();
                level.setFortified( revPos + Point(0, 1), player );
                level.setSlab( revPos + Point(0, 2), Slab::S_EARTH );
                const Point monstersPos = revPos + Point(0, 3);
                level.setSlab( monstersPos, Slab::S_PATH );
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 10 );
            }

            const adiktedpp::Player player = dungeon.owner();
            level.setCreatureAuto( evilHeartPosition.x, evilHeartPosition.y-2, Creature::C_IMP, 8, 1, player );

//                    /// fill treasure with gold
//                    spatial::FortressRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        struct GateDistance {
            std::size_t id;
            std::size_t distance;
            Point position;

            static bool sortbysec(const GateDistance& a, const GateDistance& b) {
                return (a.distance < b.distance);
            }
        };

        using GateDistanceData = std::vector< GateDistance >;


        static GateDistanceData closestHeroGates( Level& level, const utils::Point referencePoint ) {
            GateDistanceData distances;

            std::size_t gateId = 1;
            while (true) {
                const utils::Point position = level.getHeroGatePosition( gateId );
                if ( position.x < 0 || position.y < 0 ) {
                    if ( gateId == 1 ) {
                        /// no hero gates
                        return GateDistanceData();
                    }
                    break;
                }

                const Point vector = position - referencePoint;
                const std::size_t distance = vector.length();
                distances.push_back( { gateId, distance, position } );

                ++gateId;
            }

            std::sort( distances.begin(), distances.end(), GateDistance::sortbysec );
            return distances;
        }

        void HeroFortressMode::prepareScript() {
            adiktedpp::script::Script& script = map.script;

            script::ScriptCommand& initSection = script.initSection();
            initSection.addEmptyLine();
            initSection.REM( "- flags meaning -" );
            initSection.REM( "     FLAG6 -- scout_1 team" );
            initSection.REM( "     FLAG7 -- modify stun chance after transformation prisoners to creatures (skeletons, ghosts or conversion)" );

            initSection.addEmptyLine( 2 );
            script.setFXLevel();

            script.addLineInit( "" );
            script.addLineInit( "SET_GENERATE_SPEED( 500 )" );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, 20000 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                initialGold += 200000;
            }
            initSection.START_MONEY( Player::P_P0, initialGold );                /// does not show in treasure

            const std::size_t maxCreatures = parameters.getSizeT( ParameterName::PN_CREATURES_LIMIT, 25 );
            initSection.MAX_CREATURES( Player::P_P0, maxCreatures );

            script.addLineInit( "" );
            script.setEvilCreaturesPool( 10 );

            script.addLineInit( "" );
            const std::set< Player > availablePlayers = { Player::P_P0 };
            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( Player::P_P0 );
            script.set( availableCreatures );

            script.addLineInit( "" );
            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::AvailableRoomMode::ARM_DISABLED );

            availableRooms.setStateMode( Player::P_ALL, Room::R_PRISON, script::AvailableRoomMode::ARM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_TORTURE, script::AvailableRoomMode::ARM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, script::AvailableRoomMode::ARM_DISABLED );

            availableRooms.setStateMode( Player::P_P0, Room::R_PRISON, script::AvailableRoomMode::ARM_ENABLED_FOUND );
            availableRooms.setStateMode( Player::P_P0, Room::R_TORTURE, script::AvailableRoomMode::ARM_ENABLED_FOUND );
            availableRooms.setStateMode( Player::P_P0, Room::R_GRAVEYARD, script::AvailableRoomMode::ARM_ENABLED_FOUND );

            script.set( availableRooms );

            script.addLineInit( "" );
            script.setDoorsAvailable( Player::P_ALL, 0 );

            script.addLineInit( "" );
//            script.setTrapsAvailable( Player::P_ALL, 0 );
            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );
            script.set( availableTraps );

            script.addLineInit( "" );
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( Player::P_ALL );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_DESTROY_WALLS, script::AvailableMode::AM_DISABLED );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_ARMAGEDDON, script::AvailableMode::AM_DISABLED );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_CALL_TO_ARMS, script::AvailableMode::AM_ENABLED );
//                magicAvailableState.setAllAvailable( Player::P_P0, script::AvailableMode::AM_ENABLED );
            }
            script.set( availableMagic );

//            script.addLineInit( "" );
//            script.concealWholeMap( Player::P_P0 );

            /// anti snow-balling
            script.addLineInit( "" );
            script.setImpRotting( false );
            script.addLineInit( "" );
            script.setPrisonConvertLimits();
            script.addLineInit( "" );
            script.setTortureConvertLimits();
            script.addLineInit( "" );
            script.setStunChance();
            script.addLineInit( "" );
            script.setSacrificeLimits();
            script.addLineInit( "" );
            script.setGraveyardLimits();

            /// main part
            script::ScriptCommand& mainSection = script.mainSection();
            const GateDistanceData closestHeroGate = closestHeroGates( map.level, evilHeartPosition );
            const std::size_t gatesSize = closestHeroGate.size();
            if ( gatesSize > 0 ) {
                script::ScriptCommand& parties = script.partiesSection();
                parties.addEmptyLine();
                parties.REM( "- first recon team -" );
                parties.CREATE_PARTY( "scout_1" );
                parties.ADD_TO_PARTY( "scout_1", Creature::C_ARCHER, 2, 5, 500, script::PartyObjective::PO_DEFEND_PARTY );
                parties.ADD_TO_PARTY( "scout_1", Creature::C_THIEF, 2, 5, 500, script::PartyObjective::PO_DEFEND_PARTY );

                mainSection.addEmptyLine();
                mainSection.REM( "- send scout team -" );
                mainSection.IF( Player::P_P0, script::Flag::F_FLAG_6, "<", 255 );
                mainSection.IF( Player::P_P0, script::IfOption::IO_TOTAL_GOLD_MINED, ">", 16000 );
                {
                    for ( std::size_t i=0; i<gatesSize; ++i ) {
                        const GateDistance& item = closestHeroGate[ i ];
                        mainSection.SET_FLAG( Player::P_P0, script::Flag::F_FLAG_6, item.id );
                        mainSection.IF_SLAB_OWNER( item.position, Player::P_P0 );
                        mainSection.REM( "hero gate " + std::to_string( item.id ) + " captured" );
                    }
                    {
                        /// deepest if-clause
                        mainSection.REM( "no more hero gates" );
                        mainSection.SET_FLAG( Player::P_P0, script::Flag::F_FLAG_6, 255 );
                    }
                    for ( std::size_t i=gatesSize-1; i<gatesSize; --i ) {
                        mainSection.ENDIF();
                        const GateDistance& item = closestHeroGate[ i ];
                        mainSection.IF( Player::P_P0, script::Flag::F_FLAG_6, "==", item.id );
                        {
                            mainSection.REM( "hero gate " + std::to_string( item.id ) + " not captured" );
                            mainSection.ADD_TUNNELLER_PARTY_TO_LEVEL( Player::P_GOOD, "scout_1", (int)-item.id, 0, 5, 1000 );
                            mainSection.SET_FLAG( Player::P_P0, script::Flag::F_FLAG_6, 255 );
                        }
                        mainSection.ENDIF();
                    }
                }
                mainSection.ENDIF();
                mainSection.ENDIF();
            }

            /// end game conditions
            script[ script::ScriptSection::SS_ENDCOND ].addEmptyLine();
            script.setWinConditionKillGood();
            script[ script::ScriptSection::SS_ENDCOND ].addEmptyLine();
            script.setLoseConditionStandard( Player::P_P0 );
        }

    } /* namespace mode */
} /* namespace dkmage */
