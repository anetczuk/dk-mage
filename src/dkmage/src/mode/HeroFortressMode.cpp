/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/Map.h"
#include "adiktedpp/LakeGenerator.h"
#include "adiktedpp/AreaDetector.h"

#include "utils/Container.h"

#include <cmath>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        static const ProbabilityMass< spatial::FortressRoomType >& FortressRoomsProbability() {
            static ProbabilityMass< spatial::FortressRoomType > roomProbability;
            if ( roomProbability.empty() ) {
                roomProbability.set( spatial::FortressRoomType::FR_EMPTY, 0.3 );
                roomProbability.set( spatial::FortressRoomType::FR_TREASURE, 0.3, 4 );
                roomProbability.set( spatial::FortressRoomType::FR_CORRIDOR, 0.5 );
                roomProbability.set( spatial::FortressRoomType::FR_BRANCH, 0.3 );
                roomProbability.set( spatial::FortressRoomType::FR_BOULDER_CORRIDOR, 0.8, 3 );      ///
                roomProbability.set( spatial::FortressRoomType::FR_PRISON, 0.4, 1 );                ///
                roomProbability.set( spatial::FortressRoomType::FR_TORTURE, 0.8, 1 );               ///
                roomProbability.set( spatial::FortressRoomType::FR_GRAVEYARD, 1.2, 1 );             ///
                roomProbability.set( spatial::FortressRoomType::FR_LAVA_POST, 0.4, 3 );             ///
                roomProbability.set( spatial::FortressRoomType::FR_SECRET_INCLVL, 0.8, 2 );         ///
                roomProbability.set( spatial::FortressRoomType::FR_SECRET_RESURRECT, 0.8, 1 );      ///
                roomProbability.normalize();

                LOG() << "fortress rooms probability map:\n" << roomProbability.print();
            }
            return roomProbability;
        }

        ///===================================================================================================

        Fortress::Fortress( adiktedpp::Map& map, ParametersMap& parameters ): map(map), level(map.level), parameters(parameters), fortress( Player::P_GOOD ) {
//                fortress.limitNorth = 3;
//                fortress.limitSouth = 3;
            fortress.fortify( true );
            roomProbability = FortressRoomsProbability();            /// yes, copy
        }

        bool Fortress::generate() {
            Rect lakeLimit = raw::RawLevel::mapRect( 4 );
            lakeLimit.max.y -= 18;                                      /// make space for evil dungeon

            fortress.limitWidth  = lakeLimit.width()  - 6;
            fortress.limitHeight = lakeLimit.height() - 6;

            const spatial::FortressRoom* heart = fortress.setFirstRoom( spatial::FortressRoomType::FR_DUNGEON_HEART, 5 );
            std::vector< const spatial::FortressRoom* > branchStart;
            branchStart.push_back( heart );

            const std::size_t roomsNum = rng_randi( 4 ) + 4;
            std::vector< const spatial::FortressRoom* > mainCorridor = prepareCorridors( branchStart, roomsNum, false );
            if ( mainCorridor.size() == 1 ) {
                const spatial::FortressRoom* lastRoom = mainCorridor.back();
                if ( lastRoom->restrictedDirections().empty() == false ) {
                    /// restricted room -- add unrestricted one
                    const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_EMPTY, *lastRoom );
                    if ( next == nullptr ) {
                        LOG() << "unable to create main junction room";
                        fortress.moveToTopEdge( 8 );
                        fortress.draw( map );
                        return false;
                    }
                    mainCorridor = { next };
                }
                const spatial::FortressRoom* corridorExit = mainCorridor.back();
                for ( std::size_t i=0; i<2; ++i ) {
                    mainCorridor.push_back( corridorExit );
                }
            }

            LOG() << "generating branch corridors";
            const std::size_t branchLength = rng_randi( 4 ) + 5;
            const std::vector< const spatial::FortressRoom* > branches = prepareCorridors( mainCorridor, branchLength, true );
            if ( branches.size() < 2 ) {
                LOG() << "unable create at least two branch corridors";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                }
                return false;
            }
            const std::vector< const spatial::FortressRoom* > exitRooms = prepareExitRooms( branches );
            if ( exitRooms.size() < 2 ) {
                LOG() << "unable create at least two exits needed";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                }
                return false;
            }

            /// secondary pass
            prepareSecondaryPass();

            /// check required rooms
            {
                if ( fortress.findRoom( spatial::FortressRoomType::FR_PRISON ).empty() ) {
                    LOG() << "missing required prison";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_TORTURE ).empty() ) {
                    LOG() << "missing required torture room";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_GRAVEYARD ).empty() ) {
                    LOG() << "missing required graveyard";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_LAVA_POST ).empty() ) {
                    LOG() << "missing required lava posts";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( map );
                    return false;
                }
            }

            cutBlindCorridors();

            fortress.moveToTopEdge( 8 );

            /// generate lake
            if ( generateLake( lakeLimit ) == false ) {
                fortress.draw( map );
                return false;
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            fortress.draw( map );

            /// prepare entrances
            if ( prepareBridges( exitRooms ) == false ) {
                return false;
            }

            const Point firstCenter = heart->position().center();

            /// add guards
            const Player owner = fortress.owner();
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-2, Creature::C_SAMURAI, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x-1, firstCenter.y-2, Creature::C_THIEF, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x,   firstCenter.y-2, Creature::C_FAIRY, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+1, firstCenter.y-2, Creature::C_GIANT, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-2, Creature::C_BARBARIAN, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y-1, Creature::C_KNIGHT, 7, 9, owner );
            level.setCreatureAuto( firstCenter.x+2, firstCenter.y-1, Creature::C_MONK, 9, 9, owner );
            level.setCreatureAuto( firstCenter.x-2, firstCenter.y, Creature::C_WIZARD, 9, 9, owner );

            return true;
        }

        std::vector< const spatial::FortressRoom* > Fortress::prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches ) {
            std::vector< const spatial::FortressRoom* > roomQueue = startRooms;

            /// create branches
            for ( std::size_t i=0; i<roomsNum; ++i ) {
                std::vector< const spatial::FortressRoom* > nextRooms;
                const std::size_t qSize = roomQueue.size();
                for ( std::size_t x=0; x<qSize; ++x ) {
                    const spatial::FortressRoomType roomType = roomProbability.popRandom();
                    const spatial::FortressRoom* item = roomQueue[ x ];
                    const spatial::FortressRoom* next = fortress.addRandomRoom( roomType, *item );
                    if ( next == nullptr ) {
                        LOG() << "unable to generate chamber level " << i;
//                        LOG() << "unable to generate chamber level " << i << " " << roomType;
                        continue ;
                    }

                    nextRooms.push_back( next );
                    if ( allowBranches == false ) {
                        /// no additional branches
                        continue ;
                    }

                    const std::vector< spatial::Direction > freeDirs = fortress.freeDirections( *next );
                    const std::size_t fSize = freeDirs.size();
                    for ( std::size_t i=1; i<fSize; ++i ) {
                        if ( rng_randb( 0.6 ) ) {
                            /// add once again to make a branch
                            nextRooms.push_back( next );
                        }
                    }

                    nextRooms.push_back( next );
                }
                roomQueue = nextRooms;
            }

            return roomQueue;
        }

        std::vector< const spatial::FortressRoom* > Fortress::prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms ) {
            /// create branch exit
            std::vector< const spatial::FortressRoom* > exitRooms;
            const std::set< const spatial::FortressRoom* > uniqueRooms( startRooms.begin(), startRooms.end() );
            for ( const spatial::FortressRoom* item: uniqueRooms ) {
                const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *item );
                if ( next == nullptr ) {
                    LOG() << "unable to generate branch exit";
                    continue ;
                }
                exitRooms.push_back( next );
            }
            return exitRooms;
        }

        void Fortress::prepareSecondaryPass() {
            LOG() << "performing secondary pass";
            std::vector< const spatial::FortressRoom* > roomQueue = const_cast< const spatial::FortressDungeon& >( fortress ).rooms();

            /// create branches
            std::size_t stepsCounter = 0;
            while ( roomQueue.empty() == false ) {
                std::vector< const spatial::FortressRoom* > nextRooms;
                const std::size_t qSize = roomQueue.size();
                for ( std::size_t x=0; x<qSize; ++x ) {
                    const spatial::FortressRoom* item = roomQueue[ x ];
                    const spatial::FortressRoomType itemType = item->type();
                    if ( itemType == spatial::FortressRoomType::FR_DUNGEON_HEART ) {
                        continue ;
                    }
                    if ( itemType == spatial::FortressRoomType::FR_EXIT ) {
                        continue ;
                    }

                    const spatial::FortressRoomType roomType = roomProbability.popRandom();
                    const spatial::FortressRoom* next = fortress.addRandomRoom( roomType, *item );
                    if ( next == nullptr ) {
                        continue ;
                    }
                    nextRooms.push_back( next );

                    const std::vector< spatial::Direction > freeDirs = fortress.freeDirections( *next );
                    const std::size_t fSize = freeDirs.size();
                    for ( std::size_t i=1; i<fSize; ++i ) {
                        if ( rng_randb( 0.6 ) ) {
                            /// add once again to make a branch
                            nextRooms.push_back( next );
                        }
                    }
                }
                roomQueue = nextRooms;
                ++stepsCounter;
                if ( stepsCounter > 6 ) {
                    /// do not generate very long branches in secondary step
                    break;
                }
            }
        }

        void Fortress::cutBlindCorridors() {
            bool removed = false;
            do {
                removed = false;
                std::vector< spatial::FortressRoom* > rooms = fortress.rooms();
                for ( spatial::FortressRoom* item: rooms ) {
                    const std::vector< spatial::FortressRoom* > neighbours = fortress.connectedRooms( *item );
                    if ( neighbours.size() > 1 ) {
                        continue ;
                    }
                    const Rect itemRect = item->position();
                    if ( itemRect.width() > 1 && itemRect.height() > 1 ) {
                        continue ;
                    }
                    /// dead-end corridor
                    fortress.removeRoom( *item );
                    removed = true;
                }
            } while( removed == true );
        }

        bool Fortress::generateLake( const Rect& lakeLimit ) {
            const Rect dungeonBBox = fortress.boundingBox();
            if ( lakeLimit.hasInside( dungeonBBox ) == false ) {
                LOG() << "fortress too big";
                return false;
            }
            const std::set< Point > outline = fortress.outline();
            if ( lakeLimit.hasInside( outline ) == false ) {
                LOG() << "fortress too big";
                return false;
            }

            LakeGenerator lavaLake;
            lavaLake.available = outline;
            const std::size_t lakeSize = outline.size() * 1.4;
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

            return true;
        }

        bool Fortress::prepareBridges( const std::vector< const spatial::FortressRoom* >& entranceRooms ) {
            SizeTSet entrancesAllowed;
            if ( parameters.isSet( ParameterName::PN_ENTRANCES_NUMBER ) ) {
                Optional< SizeTSet > allowed = parameters.getSizeTSet( ParameterName::PN_ENTRANCES_NUMBER );
                entrancesAllowed = allowed.value();
            } else {
                entrancesAllowed.add( 2, 5 );
            }

            const std::size_t qSize = entranceRooms.size();
            LOG() << "found exit rooms: " << qSize;

            const SizeTSet entrancesRange = entrancesAllowed.filter( 1, qSize );
            if ( entrancesRange.size() < 1 ) {
                LOG() << "unable to create requested number of bridges";
                return false;
            }

//            LOG() << "requested number of fortress entrances: " << entrancesAllowed;

            std::vector< Point > entrances;
            std::vector< PointList > bridges;
            for ( const spatial::FortressRoom* entrance: entranceRooms ) {
                const spatial::FortressRoom& entranceRoom = *entrance;

                const std::vector< spatial::Direction > directions = fortress.linkDirections( entranceRoom );
                const spatial::Direction corridorDirection = directions[ 0 ];
                const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
                const Point entrancePoint = entranceRoom.edgePoint( entranceDirection, 1 );

                const Point initialDirection = movePoint( Point(0, 0), entranceDirection, 1 );
                const Point bridgeStart = entrancePoint + initialDirection;
                const PointList bridge = findBridge( bridgeStart );
                if ( bridge.size() < 2 ) {
                    LOG() << "unable find bridge shore";
                    continue;
                }

                bool collision = false;
                for ( const PointList& item: bridges ) {
                    if ( is_collision( item, bridge ) ) {
                        /// collision -- skip bridge
                        collision = true;
                        break;
                    }
                }
                if ( collision ) {
                    continue ;
                }

                entrances.push_back( entrancePoint );
                bridges.push_back( bridge );
            }

            const std::size_t foundBridges = bridges.size();
            LOG() << "found bridges: " << foundBridges;

            /// taking maximum possible
            std::size_t bSize = 0;
            for ( std::size_t i = foundBridges; i > 0; --i ) {
                if ( entrancesRange.contains( i ) ) {
                    bSize = i;
                    break ;
                }
            }
            if ( bSize < 1 ) {
                LOG() << "unable create required number of bridges";
                return false;
            }

            /// take random value
//            const SizeTSet bridgesRange = entrancesAllowed.filter( 1, foundBridges );
//            const Optional<std::size_t> createBridges = bridgesRange.getRandom();
//            if ( createBridges == false ) {
//                LOG() << "unable to create requested number of bridges";
//                return false;
//            }
//            const std::size_t bSize = createBridges.value();

            LOG() << "creating bridges: " << bSize;
            std::set< std::size_t > indexSet;
            for ( std::size_t bIndex=0; bIndex<foundBridges; ++bIndex ) {
                indexSet.insert( bIndex );
            }

            for ( std::size_t bIndex=0; bIndex<bSize; ++bIndex ) {
                const std::size_t setIndex = rng_randi( indexSet.size() );
                const int bridgeIndex = get_item( indexSet, setIndex, true );

                const Point entrance    = entrances[ bridgeIndex ];
                const PointList& bridge = bridges[ bridgeIndex ];
                level.setDoor( entrance, Door::D_IRON, true );

                const Point bridgeDirection = bridge[1] - bridge[0];
                const Point bridgeOrtho = bridgeDirection.swapped();
                std::set< Point > neighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
                neighbourDirections.erase( -bridgeDirection );

                std::size_t i = 0;
                for ( const Point bridgePoint: bridge ) {
                    ++i;

                    /// add bridge keepers
                    level.setSlab( bridgePoint, Slab::S_PATH );
                    const int fairyLevel = rng_randi( 4 ) + 3;
                    level.setCreature( bridgePoint, 0, Creature::C_FAIRY, 1, fairyLevel, Player::P_GOOD );
                    level.setCreature( bridgePoint, 2, Creature::C_FAIRY, 1, fairyLevel, Player::P_GOOD );
                    if ( i == 1 ) {
                        level.setCreature( bridgePoint, 1, Creature::C_KNIGHT, 1, 7, Player::P_GOOD );
                    }

                    /// add turrets
                    if ( i % 2 == 1 ) {
                        bool added = false;
                        const int turrentLevel = rng_randi( 4 ) + 4;
                        const Point rightGuardPosition = bridgePoint + bridgeOrtho * 2;
                        if ( level.isSlab( rightGuardPosition, Slab::S_LAVA ) ) {
    //                            const Rect guardPost( rightGuardPosition, 3, 3 );
    //                            level.setSlab( guardPost, Slab::S_LAVA );
                            level.setSlab( rightGuardPosition, Slab::S_PATH );
                            level.setCreature( rightGuardPosition, 0, Creature::C_MONK, 1, turrentLevel, Player::P_GOOD );
                            level.setCreature( rightGuardPosition, 1, Creature::C_WIZARD, 1, turrentLevel, Player::P_GOOD );
                            level.setCreature( rightGuardPosition, 2, Creature::C_ARCHER, 1, turrentLevel, Player::P_GOOD );
                            added = true;
                        }
                        const Point leftGuardPosition = bridgePoint - bridgeOrtho * 2;
                        if ( level.isSlab( leftGuardPosition, Slab::S_LAVA ) ) {
    //                            const Rect guardPost( leftGuardPosition, 3, 3 );
    //                            level.setSlab( guardPost, Slab::S_LAVA );
                            level.setSlab( leftGuardPosition, Slab::S_PATH );
                            level.setCreature( leftGuardPosition, 0, Creature::C_MONK, 1, turrentLevel, Player::P_GOOD );
                            level.setCreature( leftGuardPosition, 1, Creature::C_WIZARD, 1, turrentLevel, Player::P_GOOD );
                            level.setCreature( leftGuardPosition, 2, Creature::C_ARCHER, 1, turrentLevel, Player::P_GOOD );
                            added = true;
                        }
                        if ( added == false ) {
                            ++i;
                        }
                    }
                }
            }

            return true;
        }

        PointList Fortress::findBridge( const Point startPoint ) {
            static const PointList directions = { Point( 1, 0 ), Point( -1, 0 ), Point( 0, 1 ), Point( 0, -1 ) };

            PointList ret;

            for ( const Point dir: directions ) {
                Point nextPoint = startPoint + dir;
                if ( level.isSlab( nextPoint, Slab::S_LAVA ) == false ) {
                    continue ;
                }
                PointList bridge = findBridge( startPoint, dir );
                if ( bridge.empty() ) {
                    continue ;
                }

                if ( ret.empty() ) {
                    ret = bridge;
                    continue ;
                }
                if ( ret.size() > bridge.size() ) {
                    ret = bridge;
                }
            }

            return ret;
        }

        PointList Fortress::findBridge( const Point startPoint, const Point bridgeDirection ) {
            PointList ret;

            std::set< Point > heighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
            heighbourDirections.erase( -bridgeDirection );

            Point bridgePoint = startPoint;
            while( true ) {
                if ( level.isSlab( bridgePoint, Slab::S_EARTH ) ) {
                    /// shore found
                    return ret;
                }
                if ( level.isSlab( bridgePoint, Slab::S_LAVA ) == false ) {
                    /// found other slab -- bridge didn't reach lake's shore
                    return PointList();
                }

                ret.push_back( bridgePoint );

                /// search for shore
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

            return ret;
        }


        /// ===========================================================


        bool HeroFortressMode::generate() {
            level.generateRandomMap( 9 );

//            generateCaves( 28 );

            Fortress enemyFortress( map, parameters );
            if ( enemyFortress.generate() == false ) {
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
            adiktedpp::script::Script& script = map.script;

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
                dungeon.addRoom( Room::R_LAIR, 5, *hatchery, spatial::Direction::D_EAST );
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

            {
                /// add gold vein
                std::size_t goldSlabsNum = parameters.getSizeT( ParameterName::PN_GOLD_SLABS_NUMBER, 40 );
                LOG() << "gold slabs number: " << goldSlabsNum;

                std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 2 );
                gemsNum = std::min( gemsNum, (std::size_t)4 );
                LOG() << "gems number: " << gemsNum;

                generateLeftGoldVein( goldSlabsNum, gemsNum );
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            dungeon.draw( level );

            const spatial::EvilRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();

            {
                /// add neutral portal
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect randRect( 21, 11 );
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
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 9 );
            }

            level.setCreatureAuto( firstCenter.x, firstCenter.y-2, Creature::C_IMP, 8 );

//                    /// fill treasure with gold
//                    spatial::FortressRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void HeroFortressMode::prepareScript() {
            adiktedpp::script::Script& script = map.script;

//            script.setFXLevel();

            script.addLineInit( "SET_GENERATE_SPEED( 500 )" );
            script.addLineInit( "MAX_CREATURES( PLAYER0, 25 )" );

            std::size_t initialGold = parameters.getSizeT( ParameterName::PN_INIT_GOLD_AMOUNT, 20000 );
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                initialGold += 200000;
            }
            script.setStartMoney( Player::P_P0, initialGold );                /// does not show in treasure

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
            script.set( availableMagic );

//            script.addLineInit( "" );
//            script.setImpRotting( false );

            /// end game conditions
            script.setWinConditionStandard( Player::P_P0 );
            script.setLoseConditionStandard( Player::P_P0 );
        }

    } /* namespace mode */
} /* namespace dkmage */
