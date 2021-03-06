/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/spatial/Fortress.h"
#include "dkmage/Draw.h"
#include "dkmage/ParameterDefaults.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        bool HeroFortressMode::generate() {
            Level& level = map.level;

            level.generateRandomMap( 9 );

//            generateCaves( 28 );

            spatial::Fortress enemyFortress( map, parameters );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                if ( enemyFortress.generateTest() == false ) {
                    return false;
                }
            } else {
                if ( enemyFortress.generate() == false ) {
                    return false;
                }
            }

            preparePlayerDungeon();

            LOG_INFO() << "preparing script";
            prepareScript();

            return true;
        }

        bool HeroFortressMode::check() {
//            if ( level.countClaimAreas() > 0 ) {
//                LOG_INFO() << "map problem found: unclaimable areas";
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
                LOG_INFO() << "invalid region area: " << regionArea;
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
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
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
            generateGoldSlabs( PN_DEFAULT_GOLD_SLABS_NUMBER_HEROFORTRESS, PN_DEFAULT_GEM_SLABS_NUMBER_HEROFORTRESS, PN_DEFAULT_GEM_FACES_NUMBER_HEROFORTRESS, PN_DEFAULT_GEM_TRAPS_NUMBER_HEROFORTRESS, 1, 77 );

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
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
                level.setItem( revPos, Item::I_SPECIAL_REVMAP );

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
            adiktedpp::script::Script mainScript;
            script::ScriptCommand& initSection = mainScript.initSection();

            initSection.addEmptyLine();
            mainScript.setFXLevel();

            initSection.addEmptyLine();
            initSection.SET_GENERATE_SPEED( 500 );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, PN_DEFAULT_INIT_GOLD_AMOUNT );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                initialGold += 200000;
            }
            initSection.START_MONEY( Player::P_P0, initialGold );                /// does not show in treasure

            const std::size_t maxCreatures = parameters.getSizeT( ParameterName::PN_CREATURES_LIMIT, PN_DEFAULT_CREATURES_LIMIT_HEROFORTRESS );
            initSection.MAX_CREATURES( Player::P_P0, maxCreatures );

            initSection.addEmptyLine();
            mainScript.setEvilCreaturesPool( 10 );

            initSection.addEmptyLine();
            const std::set< Player > availablePlayers = { Player::P_P0 };
            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( Player::P_P0 );
            mainScript.set( availableCreatures );

            initSection.addEmptyLine();
            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::RoomAvailableMode::RAM_DISABLED );

            availableRooms.setStateMode( Player::P_ALL, Room::R_PRISON, script::RoomAvailableMode::RAM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_TORTURE, script::RoomAvailableMode::RAM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, script::RoomAvailableMode::RAM_DISABLED );

            availableRooms.setStateMode( Player::P_P0, Room::R_PRISON, script::RoomAvailableMode::RAM_AVAILABLE_FOUND );
            availableRooms.setStateMode( Player::P_P0, Room::R_TORTURE, script::RoomAvailableMode::RAM_AVAILABLE_FOUND );
            availableRooms.setStateMode( Player::P_P0, Room::R_GRAVEYARD, script::RoomAvailableMode::RAM_AVAILABLE_FOUND );

            mainScript.set( availableRooms );

            initSection.addEmptyLine();
            mainScript.setDoorsAvailable( Player::P_ALL, 0 );

            initSection.addEmptyLine();
//            script.setTrapsAvailable( Player::P_ALL, 0 );
            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );
            mainScript.set( availableTraps );

            initSection.addEmptyLine();
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( Player::P_ALL );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_DESTROY_WALLS, script::MagicAvailableMode::AM_DISABLED );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_ARMAGEDDON, script::MagicAvailableMode::AM_DISABLED );
            if ( parameters.isSet( ParameterName::PN_TEST_ADDONS ) ) {
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_CALL_TO_ARMS, script::MagicAvailableMode::AM_AVAILABLE );
                availableMagic.setStateMode( Player::P_P0, Spell::S_POWER_DESTROY_WALLS, script::MagicAvailableMode::AM_AVAILABLE );
//                magicAvailableState.setAllAvailable( Player::P_P0, script::AvailableMagicMode::AM_ENABLED );
            }
            mainScript.set( availableMagic );

            initSection.addEmptyLine();
            mainScript.setImpRotting( false );

//            script.addLineInit( "" );
//            script.concealWholeMap( Player::P_P0 );

            initSection.addEmptyLine();
            const std::size_t infoIndex = mainScript.nextObjectiveIndex();
            initSection.QUICK_OBJECTIVE( infoIndex, "Coward landlord hid in fortress. Plunder surroundings and burn his pitty cottage." );

            /// flags part
            script::ScriptCommand& flagsSection = mainScript.flagsSection();
            flagsSection.addEmptyLine();
            flagsSection.REM( "- meaning of flags and timers -" );
            flagsSection.REM( "     P0 FLAG7  -- modify stun chance after transformation of prisoners to creatures (skeletons, ghosts or conversion)" );
            flagsSection.REM( "     PGOOD FLAG5  -- activate counter strike teams timer" );
            flagsSection.REM( "     PGOOD FLAG6  -- scout team" );
            flagsSection.REM( "     PGOOD FLAG7  -- random counter strike team selector" );
            flagsSection.REM( "     PGOOD TIMER7 -- counting time to next main counter attack" );

            /// anti snow-balling
            script::ScriptCommand& mainSection = mainScript.mainSection();
            mainSection.addEmptyLine();
            mainScript.setPrisonConvertLimits();
            mainSection.addEmptyLine();
            mainScript.setTortureConvertLimits();
            mainSection.addEmptyLine();
            mainScript.setStunChance();
            mainSection.addEmptyLine();
            mainScript.setSacrificeLimits();
            mainSection.addEmptyLine();
            mainScript.setGraveyardLimits();
            mainSection.addEmptyLine();

            /// main part
            const GateDistanceData closestHeroGate = closestHeroGates( map.level, evilHeartPosition );
            const std::size_t gatesSize = closestHeroGate.size();
            if ( gatesSize > 0 ) {
                script::ScriptCommand& parties = mainScript.partiesSection();
                parties.addEmptyLine();
                parties.REM( "- scout team -" );
                parties.CREATE_PARTY( "scout_1" );
                parties.ADD_TO_PARTY( "scout_1", Creature::C_ARCHER, 2, 5, 500, script::PartyObjective::PO_DEFEND_PARTY );
                parties.ADD_TO_PARTY( "scout_1", Creature::C_THIEF, 2, 5, 500, script::PartyObjective::PO_DEFEND_PARTY );

                mainSection.addEmptyLine();
                mainSection.REM( "- send scout team -" );
                mainSection.IF_flag( Player::P_GOOD, script::Flag::F_FLAG_6, "<", 100 );
                mainSection.IF_option( Player::P_P0, script::IfOption::IO_TOTAL_GOLD_MINED, ">", 16000 );
                {
                    for ( std::size_t i=0; i<gatesSize; ++i ) {
                        const GateDistance& item = closestHeroGate[ i ];
                        mainSection.SET_FLAG( Player::P_GOOD, script::Flag::F_FLAG_6, item.id );
                        mainSection.IF_SLAB_OWNER( item.position, Player::P_P0 );
                        mainSection.REM( "hero gate " + std::to_string( item.id ) + " captured" );
                    }
                    {
                        /// deepest if-clause
                        mainSection.REM( "no more hero gates" );
                        mainSection.SET_FLAG( Player::P_GOOD, script::Flag::F_FLAG_6, 100 );
                    }
                    for ( std::size_t i=gatesSize-1; i<gatesSize; --i ) {
                        mainSection.ENDIF();
                        const GateDistance& item = closestHeroGate[ i ];
                        mainSection.IF_flag( Player::P_GOOD, script::Flag::F_FLAG_6, "==", item.id );
                        {
                            mainSection.REM( "hero gate " + std::to_string( item.id ) + " not captured" );
                            const std::size_t infoIndex = mainScript.nextInfoIndex();
                            mainSection.QUICK_INFORMATION( infoIndex, "Your activity was noticed by defenders. Prepare for incoming confrontation." );
                            mainSection.ADD_TUNNELLER_PARTY_TO_LEVEL( Player::P_GOOD, "scout_1", (int)-item.id, 0, 5, 1000 );
                            mainSection.SET_FLAG( Player::P_GOOD, script::Flag::F_FLAG_6, 100 );
                        }
                        mainSection.ENDIF();
                    }
                }
                mainSection.ENDIF();
                mainSection.ENDIF();
            }

            /// end game conditions
            mainScript[ script::ScriptSection::SS_ENDCOND ].addEmptyLine();
            mainScript.setWinConditionKillGood();
            mainScript[ script::ScriptSection::SS_ENDCOND ].addEmptyLine();
            mainScript.setLoseConditionStandard( Player::P_P0 );

            /// merge scripts
            adiktedpp::script::Script& script = map.script;
            script.pushFrontBySections( mainScript );
        }

    } /* namespace mode */
} /* namespace dkmage */
