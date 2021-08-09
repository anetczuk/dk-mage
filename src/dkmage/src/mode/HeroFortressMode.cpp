/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/LakeGenerator.h"
#include "adiktedpp/AreaDetector.h"
#include "adiktedpp/script/Script.h"

#include "utils/Set.h"

#include <cmath>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        void HeroFortressMode::generateLevel() {
            LOG() << "preparing map";
            level.generateRandomMap( 9 );

//            generateCaves( 28 );

            if ( prepareEnemyDungeon() == false ) {
                LOG() << "could not generate fortress -- restarting";
//                    storePreview( "level.bmp" );
                generateLevel();
                return ;
            };

            preparePlayerDungeon();

            LOG() << "preparing script";
            prepareScript();

            const bool valid = level.verifyMap();
            if ( valid == false ) {
                LOG() << "detected invalid map -- restarting generation";
//                    storePreview( "level.bmp" );
                generateLevel();
                return ;
            }
//            if ( level.countClaimAreas() > 0 ) {
//                LOG() << "map problem found: unclaimable areas";
//                generateLevel();
//                return ;
//            }
        }

        void HeroFortressMode::generateCaves( const std::size_t cavesNum ) {
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
                const int itemIndex = rand() % indexSet.size();
                const int regionIndex = getSetItem( indexSet, itemIndex, true );
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
            spatial::Dungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                dungeon.generate( 4, 5 );
            } else {
                dungeon.generate( 1, 5 );
            }

            dungeon.moveToBottomEdge( 4 );

//                LOG() << "dungeon:\n" << dungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, dungeon );

            const spatial::DungeonRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();
            const Rect bbox = dungeon.boundingBox();

            /// add neutral portal
            const Point portalCenter( bbox.max.x + 16, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.setRoom( portalRect, Room::R_PORTAL );

            {
                /// add gold vein
                std::size_t goldSlabsNum = parameters.getSizeT( ParameterName::PN_GOLD_SLABS_NUMBER, 40 );
                LOG() << "gold slabs number: " << goldSlabsNum;

                std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 2 );
                gemsNum = std::min( gemsNum, (std::size_t)4 );
                LOG() << "gems number: " << gemsNum;

                const std::size_t leftVeinGold = goldSlabsNum;
                const std::size_t leftVeinDimm = (std::size_t) sqrt( leftVeinGold ) * 1.5;
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect leftVeinRect( leftVeinDimm, leftVeinDimm );
                leftVeinRect.move( 10, 0 );
                leftVeinRect.moveBottomTo( mapRect.max.y );
                drawGoldVein( level, leftVeinRect, leftVeinGold, gemsNum );
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
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 9 );
            }

            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, Creature::C_IMP, 8 );

//                    /// fill treasure with gold
//                    spatial::DungeonRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        static std::vector< const spatial::DungeonRoom* > prepareBranches( spatial::Dungeon& enemyDungeon, const std::vector< const spatial::DungeonRoom* >& startRooms ) {
            static const std::size_t ROOM_MAX_SIZE = 4;

            std::vector< const spatial::DungeonRoom* > roomQueue = startRooms;
            const std::size_t qSize = roomQueue.size();

            /// create branches
            const std::size_t branchLength = rand() % 3 + 6;
            for ( std::size_t i=0; i<branchLength; ++i ) {
                for ( std::size_t x=0; x<qSize; ++x ) {
//                    if ( i > 4 && (rand() % 4 == 0) ) {
//                        /// skip iteration with certain probability -- will make branch shorter
//                        continue;
//                    }
                    const spatial::DungeonRoom* item = roomQueue[ x ];
                    const spatial::DungeonRoom* next = nullptr;
                    const std::size_t rSize = ( rand() % ROOM_MAX_SIZE ) * 2 + 1;
                    const std::size_t corridor = rand() % 5 + 1;
                    if ( i == 0 ) {
                        /// first item
                        const std::vector< spatial::Direction > directions = enemyDungeon.availableDirections( *item );
                        const spatial::Direction corridorDirection = directions[ 0 ];
                        const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
                        next = enemyDungeon.addRoom( Room::R_CLAIMED, rSize, *item, entranceDirection, true, corridor );
                    } else {
                        next = enemyDungeon.addRandomRoom( Room::R_CLAIMED, rSize, *item, true, corridor );
                    }
                    if ( next == nullptr ) {
                        LOG() << "unable to generate chamber level " << i << " room " << rSize << " corridor " << corridor;
////                        LOG() << "dungeon:\n" << enemyDungeon.print();
                        return std::vector< const spatial::DungeonRoom* >();
//                        branchesQueue.push_back( item );
//                        continue ;
                    }
                    roomQueue[x] = next;
                }
            }

            /// create branch exit
            for ( std::size_t x=0; x<qSize; ++x ) {
                const spatial::DungeonRoom* next = roomQueue[ x ];
                next = enemyDungeon.addRandomRoom( Room::R_CLAIMED, 3, *next, true, 3 );
                if ( next == nullptr ) {
                    LOG() << "unable to generate branch exit";
                    return std::vector< const spatial::DungeonRoom* >();
                }
                roomQueue[ x ] = next;
            }

            return roomQueue;
        }

        static bool prepareEntrance( adiktedpp::Level& level, spatial::Dungeon& enemyDungeon, const spatial::DungeonRoom& entranceRoom ) {
            /// create branch exit
            {
                /// set entrance traps
                const std::vector< spatial::Direction > directions = enemyDungeon.availableDirections( entranceRoom );
                const spatial::Direction corridorDirection = directions[ 0 ];
                /// LOG() << "corridor direction: " << corridorDirection << " " << directions.size();

                const Point roomCenter = entranceRoom.position().center();
                const Point corridorStart = entranceRoom.edgePoint( corridorDirection, 1 );
                const Point boulderPosition = movePoint( corridorStart, corridorDirection, 1 );
                const Point corridorEnd = movePoint( boulderPosition, corridorDirection, 1 );

                drawTrap3x3Diamond( level, roomCenter, Trap::T_BOULDER );
                drawTrap3x3Corners( level, roomCenter, Trap::T_LIGHTNING );
                level.setDoor( corridorStart, Door::D_IRON, true );
                level.setTrap( boulderPosition, Trap::T_BOULDER );
                level.setDoor( corridorEnd, Door::D_IRON, true );
            }
            {
                /// make a bridge
                const std::vector< spatial::Direction > directions = enemyDungeon.availableDirections( entranceRoom );
                const spatial::Direction corridorDirection = directions[ 0 ];
                const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
                const Point entrancePoint = entranceRoom.edgePoint( entranceDirection, 1 );

                const Point bridgeDirection = movePoint( Point(0, 0), entranceDirection, 1 );
                Point bridgePoint = entrancePoint + bridgeDirection;

                std::set< Point > heighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
                heighbourDirections.erase( -bridgeDirection );

                while( true ) {
                    if ( level.isSlab( bridgePoint, Slab::S_EARTH ) ) {
                        /// shore found
                        break;
                    }
                    if ( level.isSlab( bridgePoint, Slab::S_LAVA ) == false ) {
                        /// found other slab -- bridge didn't reach lake's shore
                        LOG() << "unable find bridge shore";
                        return false;
                    }

                    level.setSlab( bridgePoint, Slab::S_PATH );

                    bool shoreFound = false;
                    for ( const Point item: heighbourDirections ) {
                        if ( level.isSlab( bridgePoint + item, Slab::S_EARTH ) ) {
                            /// lake's shore found
                            shoreFound = true;
                            break;
                        }
                    }
                    if ( shoreFound ) {
                        break;
                    }

                    bridgePoint += bridgeDirection;
                }

//                while( level.isSlab( bridgePoint, Slab::S_LAVA ) ) {
//                    level.setSlab( bridgePoint, Slab::S_EARTH );
//                    bridgePoint += bridgeDirection;
//                }
//                if ( level.isSlab( bridgePoint, Slab::S_EARTH ) == false ) {
//                    /// bridge didn't reach lake's shore -- dungeon invalid
//                    LOG() << "unable find bridge shore";
//                    return false;
//                }

                level.setDoor( entrancePoint, Door::D_IRON, true );
            }
            return true;
        }

        bool HeroFortressMode::prepareEnemyDungeon() {
            spatial::Dungeon enemyDungeon( Player::P_GOOD );
            enemyDungeon.limitNorth = 3;
            enemyDungeon.limitSouth = 3;
            enemyDungeon.fortify( true );

            const spatial::DungeonRoom* heart = enemyDungeon.addRandomRoom( Room::R_DUNGEON_HEART, 5 );

            std::vector< const spatial::DungeonRoom* > treasures;
            for ( std::size_t i=0; i<3; ++i ) {
                const spatial::DungeonRoom* treasure = enemyDungeon.addRandomRoom( Room::R_TREASURE, 5, *heart, true, 1 );
                if ( treasure == nullptr ) {
                    LOG() << "unable to create treasure room";
                    return false;
                }
                treasures.push_back( treasure );
            }

            const std::vector< const spatial::DungeonRoom* > entrances = prepareBranches( enemyDungeon, treasures );
            if ( entrances.empty() ) {
                return false;
            }

            enemyDungeon.moveToTopEdge( 8 );

            {
                /// generate lake
                Rect lakeLimit = raw::RawLevel::mapRect( 4 );
                lakeLimit.max.y -= 18;                                      /// make space for evil dungeon
                const Rect dungeonBBox = enemyDungeon.boundingBox();
                if ( lakeLimit.hasInside( dungeonBBox ) == false ) {
                    LOG() << "fortress too big";
                    return false;
                }
                const std::set< Point > outline = enemyDungeon.outline();
                if ( lakeLimit.hasInside( outline ) == false ) {
                    LOG() << "fortress too big";
                    return false;
                }

                LakeGenerator lavaLake;
                lavaLake.available = outline;
                const std::size_t lakeSize = outline.size() * 1.6;
                lavaLake.generate( lakeLimit, lakeSize );
                LakeGenerator::grow( lavaLake.added, 1, 0 );

                /// draw lake
                level.setSlab( lavaLake.added, Slab::S_LAVA );
                level.setSlab( outline, Slab::S_LAVA );

                /// draw 'EARTH' shore
                std::set< Point > lakeEdge = LakeGenerator::edge( lavaLake.added );
                level.setSlab( lakeEdge, Slab::S_EARTH );

                /// fill islands
                AreaDetector areaDetector;
                areaDetector.set( lavaLake.added, 1 );                   /// mark lava
                areaDetector.fill( 0, 0, 1 );                            /// fill outside as lava

                const std::vector< AreaDetector::AreaInfo > islands = areaDetector.fill( 0 );
                LOG() << "islands found: " << islands.size();

                for ( const AreaDetector::AreaInfo& item: islands ) {
                    const std::vector< utils::Point > points = item.cellsAsPoints();
                    level.setSlab( points, Slab::S_LAVA );
                }
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

            /// prepare entrances
            for ( const spatial::DungeonRoom* entrance: entrances ) {
                if ( prepareEntrance( level, enemyDungeon, *entrance ) == false ) {
                    return false;
                }
            }

            const Point firstCenter = enemyDungeon.roomCenter( 0 );

            /// add guards
            const Player owner = enemyDungeon.owner();
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-2, Creature::C_SAMURAI, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x-1, firstCenter.y-2, Creature::C_THIEF, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x,   firstCenter.y-2, Creature::C_FAIRY, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+1, firstCenter.y-2, Creature::C_GIANT, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-2, Creature::C_BARBARIAN, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-1, Creature::C_KNIGHT, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-1, Creature::C_MONK, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y, Creature::C_WIZARD, 9, 9, owner );

            /// fill treasure with gold
            for ( const spatial::DungeonRoom* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }

            return true;
        }

        void void HeroFortressMode::prepareScript() {
            script::Script script( level );
            {
                const std::string type = parameters.getString( ParameterName::PN_TYPE, "" );
                const std::string seed = parameters.getString( ParameterName::PN_SEED, "" );
                script.storeParameters( type, seed );
            }

            script.addLine( "" );
            script.addLine( "SET_GENERATE_SPEED( 500 )" );
            script.addLine( "MAX_CREATURES( PLAYER0, 10 )" );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, 20000 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                initialGold += 200000;
            }
            script.setStartMoney( Player::P_P0, initialGold );                /// does not show in treasure

            script.addLine( "" );
            script.addLine( "" );
            const std::set< Player > availablePlayers = { Player::P_P0 };

            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setEvilAvailable( Player::P_P0 );
            script.set( availableCreatures );

            script.addLine( "" );
            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::AvailableMode::AM_DISABLED );
            script.set( availableRooms );

            script.addLine( "" );
            script.setDoorsAvailable( Player::P_ALL, 0 );

            script.addLine( "" );
//            script.setTrapsAvailable( Player::P_ALL, 0 );
            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );
            script.set( availableTraps );

            script.addLine( "" );
//            script.setMagicStandard( Player::P_ALL );
            script::MagicAvailableState availableMagic( availablePlayers );
            availableMagic.setStandard( Player::P_ALL );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_DESTROY_WALLS, script::AvailableMode::AM_DISABLED );
            availableMagic.setStateMode( Player::P_ALL, Spell::S_POWER_ARMAGEDDON, script::AvailableMode::AM_DISABLED );
            script.set( availableMagic );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( Player::P_P0 );

            script.rebuild();
        }

    } /* namespace mode */
} /* namespace dkmage */
