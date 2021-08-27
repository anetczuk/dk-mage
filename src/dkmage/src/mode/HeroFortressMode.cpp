/*
 * HeroFortressMode.cpp
 *
 */

#include "dkmage/mode/HeroFortressMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/GameMap.h"
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
                roomProbability.set( spatial::FortressRoomType::FR_PRISON, 0.3, 1 );                ///
                roomProbability.set( spatial::FortressRoomType::FR_TORTURE, 0.4, 1 );               ///
                roomProbability.set( spatial::FortressRoomType::FR_GRAVEYARD, 0.8, 1 );             ///
                roomProbability.set( spatial::FortressRoomType::FR_LAVA_POST, 0.4, 3 );             ///
                roomProbability.set( spatial::FortressRoomType::FR_SECRET_INCLVL, 0.8, 2 );         ///
                roomProbability.set( spatial::FortressRoomType::FR_SECRET_RESURRECT, 0.8, 1 );      ///
                roomProbability.normalize();

                LOG() << "fortress rooms probability map:\n" << roomProbability.print();
            }
            return roomProbability;
        }

        struct Bridge {
            Point entrance;
            PointList bridge;
        };

        ///===================================================================================================

        Fortress::Fortress( adiktedpp::GameMap& gameMap, ParametersMap& parameters ): gameMap(gameMap), level(gameMap.level), parameters(parameters), fortress( Player::P_GOOD ) {
//                fortress.limitNorth = 3;
//                fortress.limitSouth = 3;
            fortress.fortify( true );
            roomProbability = FortressRoomsProbability();            /// yes, copy
        }

        bool Fortress::generate() {
            Rect lakeLimit = raw::RawLevel::mapRect( 6 );
            lakeLimit.max.y -= 24;                                      /// make space for evil dungeon

            fortress.limitWidth  = lakeLimit.width()  - 6;
            fortress.limitHeight = lakeLimit.height() - 6;

            spatial::FortressData fortressData = { gameMap, parameters };

            const spatial::FortressRoom* heart = fortress.setFirstRoom( spatial::FortressRoomType::FR_DUNGEON_HEART );
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
                        fortress.draw( fortressData );
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
                    fortress.draw( fortressData );
                }
                return false;
            }
            prepareExitRooms( branches );

            /// secondary pass
            prepareSecondaryPass();

            cutBlindCorridors();

            cutInvalidExits();

            /// once again, in case of removed exit rooms
            cutBlindCorridors();

            std::vector< const spatial::FortressRoom* > exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );
            if ( exitRooms.size() < 2 ) {
                LOG() << "unable create at least two exits";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                }
                return false;
            }

            /// check required rooms
            {
                if ( fortress.findRoom( spatial::FortressRoomType::FR_PRISON ).empty() ) {
                    LOG() << "missing required prison";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_TORTURE ).empty() ) {
                    LOG() << "missing required torture room";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_GRAVEYARD ).empty() ) {
                    LOG() << "missing required graveyard";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.findRoom( spatial::FortressRoomType::FR_LAVA_POST ).empty() ) {
                    LOG() << "missing required lava posts";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
            }

            fortress.moveToTopEdge( 8 );

            /// generate lake
            if ( generateLake( lakeLimit ) == false ) {
                fortress.draw( fortressData );
                return false;
            }

            /// dungeon have to be drawn before placing items inside it's rooms
            fortress.draw( fortressData );

            /// prepare entrances
            if ( prepareBridges( exitRooms ) == false ) {
                return false;
            }

            prepareCorridorTraps();

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

        void Fortress::prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms ) {
            /// create branch exit
            std::vector< const spatial::FortressRoom* > uniqueRooms( startRooms.begin(), startRooms.end() );
            remove_dupes( uniqueRooms );

            for ( const spatial::FortressRoom* item: startRooms ) {
                const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *item );
                if ( next == nullptr ) {
                    LOG() << "unable to generate branch exit";
                    continue ;
                }
            }
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
                    const Rect itemRect = item->bbox();
                    if ( itemRect.width() > 1 && itemRect.height() > 1 ) {
                        continue ;
                    }
                    /// dead-end corridor
                    fortress.removeRoom( *item );
                    removed = true;
                }
            } while( removed == true );
        }

        void Fortress::cutInvalidExits() {
            std::vector< const spatial::FortressRoom* > exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );

            /// remove inner exits (cannot make bridge)
            for ( const spatial::FortressRoom* entrance: exitRooms ) {
                const spatial::FortressRoom& entranceRoom = *entrance;

                const std::vector< spatial::Direction > directions = fortress.linkDirections( entranceRoom );
                const spatial::Direction corridorDirection = directions[ 0 ];
                const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
                const Point entrancePoint = entranceRoom.edgePoint( entranceDirection, 1 );

                const Point initialDirection = movePoint( Point(0, 0), entranceDirection, 1 );
                const Point bridgeStart = entrancePoint + initialDirection;
                const bool bridgePossible = isBridgePossible( bridgeStart );
                if ( bridgePossible == false ) {
                    fortress.removeRoom( *entrance );
                    continue;
                }
            }

            /// remove close exits
            static const std::size_t MIN_DISTANCE = 20;
            exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );

            while( true ) {
                const std::size_t eSize = exitRooms.size();
                std::vector< std::size_t > neighbourCounter( eSize );
                std::size_t maxNeighbour = 0;
                std::size_t maxIndex     = 0;

                for ( std::size_t i=0; i<eSize; ++i ) {
                    const spatial::FortressRoom* from = exitRooms[ i ];
                    const Point fromCenter = from->bbox().center();
                    for ( std::size_t j=i+1; j<eSize; ++j ) {
                        const spatial::FortressRoom* to = exitRooms[ j ];
                        const Point toCenter = to->bbox().center();
                        const std::size_t distance = fromCenter.distanceChebyshev( toCenter );
                        if ( distance < MIN_DISTANCE ) {
                            neighbourCounter[ i ] += 1;
                            neighbourCounter[ j ] += 1;
                        }
                    }
                    const std::size_t currNeigh = neighbourCounter[ i ];
                    if ( currNeigh > maxNeighbour ) {
                        maxNeighbour = currNeigh;
                        maxIndex     = i;
                    }
                }
                if ( eSize > 0 ) {
                    const std::size_t currIndex = eSize - 1;
                    const std::size_t currNeigh = neighbourCounter[ currIndex ];
                    if ( currNeigh > maxNeighbour ) {
                        maxNeighbour = currNeigh;
                        maxIndex     = currIndex;
                    }
                }
                if ( maxNeighbour < 1 ) {
                    break ;
                }
                /// remove first most popular exit
                const spatial::FortressRoom* room = exitRooms[ maxIndex ];
                fortress.removeRoom( *room );
                exitRooms.erase( exitRooms.begin() + maxIndex );
            }
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

        bool Fortress::prepareBridges( const std::vector< const spatial::FortressRoom* >& exitRooms ) {
            const SizeTSet entrancesAllowed = parameters.getSizeTSet( ParameterName::PN_ENTRANCES_NUMBER, 2, 5 );

            const std::size_t qSize = exitRooms.size();
            LOG() << "found exit rooms: " << qSize;

            const SizeTSet entrancesRange = entrancesAllowed.filter( 1, qSize );
            if ( entrancesRange.size() < 1 ) {
                LOG() << "unable to create requested number of bridges";
                return false;
            }

//            LOG() << "requested number of fortress entrances: " << entrancesAllowed;

            std::vector< Bridge > bridges;
            for ( const spatial::FortressRoom* entrance: exitRooms ) {
                const spatial::FortressRoom& entranceRoom = *entrance;

                const std::vector< spatial::Direction > directions = fortress.linkDirections( entranceRoom );
                const spatial::Direction corridorDirection = directions[ 0 ];
                const spatial::Direction entranceDirection = spatial::opposite( corridorDirection );
                const Point entrancePoint = entranceRoom.edgePoint( entranceDirection, 1 );

                const Point initialDirection = movePoint( Point(0, 0), entranceDirection, 1 );
                const Point bridgeStart = entrancePoint + initialDirection;
                const PointList bridge = findBridge( bridgeStart );
                const std::size_t bSize = bridge.size();
                if ( bridge.size() < 2 ) {
                    LOG() << "unable find bridge shore";
                    continue;
                }
                if ( bSize > 10 ) {
                    LOG() << "bridge too long";
                    continue;
                }

                bool collision = false;
                for ( const Bridge& item: bridges ) {
                    const PointList& currBridge = item.bridge;
                    if ( is_collision( currBridge, bridge ) ) {
                        /// collision -- skip bridge
                        collision = true;
                        break;
                    }
                }
                if ( collision ) {
                    continue ;
                }

                bridges.push_back( { entrancePoint, bridge } );
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

            const SizeTSet guardLevel = parameters.getSizeTSet( ParameterName::PN_BRIDGE_GUARD_LEVEL, 2, 5 );

            for ( std::size_t bIndex=0; bIndex<bSize; ++bIndex ) {
                const std::size_t setIndex = rng_randi( indexSet.size() );
                const int bridgeIndex = get_item( indexSet, setIndex, true );

                const Bridge& bridge = bridges[ bridgeIndex ];
                const Point entrance = bridge.entrance;
                const PointList& bridgePoints = bridge.bridge;

                level.setDoor( entrance, Door::D_IRON, true );

                const Point bridgeDirection = bridgePoints[1] - bridgePoints[0];
                const Point bridgeOrtho = bridgeDirection.swapped();
                std::set< Point > neighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
                neighbourDirections.erase( -bridgeDirection );

                std::size_t i = 0;
                for ( const Point bridgePoint: bridgePoints ) {
                    ++i;

                    /// add bridge keepers
                    level.setSlab( bridgePoint, Slab::S_PATH );
                    if ( i == 1 ) {
                        level.setCreature( bridgePoint, 0, Creature::C_FAIRY, 1, guardLevel.randomized(), Player::P_GOOD );
                        level.setCreature( bridgePoint, 2, Creature::C_FAIRY, 1, guardLevel.randomized(), Player::P_GOOD );
                        level.setCreature( bridgePoint, 1, Creature::C_KNIGHT, 1, 8, Player::P_GOOD );
                    }

                    /// add turrets
                    if ( i % 2 == 1 ) {
                        bool added = false;
                        const int turretLevel = guardLevel.randomized();
                        const Point rightGuardPosition = bridgePoint + bridgeOrtho * 2;
                        if ( level.isSlab( rightGuardPosition, Slab::S_LAVA ) ) {
    //                            const Rect guardPost( rightGuardPosition, 3, 3 );
    //                            level.setSlab( guardPost, Slab::S_LAVA );
                            level.setSlab( rightGuardPosition, Slab::S_PATH );
                            level.setCreature( rightGuardPosition, 0, Creature::C_MONK, 1, turretLevel, Player::P_GOOD );
                            level.setCreature( rightGuardPosition, 1, Creature::C_WIZARD, 1, turretLevel, Player::P_GOOD );
                            level.setCreature( rightGuardPosition, 2, Creature::C_ARCHER, 1, turretLevel, Player::P_GOOD );
                            added = true;
                        }
                        const Point leftGuardPosition = bridgePoint - bridgeOrtho * 2;
                        if ( level.isSlab( leftGuardPosition, Slab::S_LAVA ) ) {
    //                            const Rect guardPost( leftGuardPosition, 3, 3 );
    //                            level.setSlab( guardPost, Slab::S_LAVA );
                            level.setSlab( leftGuardPosition, Slab::S_PATH );
                            level.setCreature( leftGuardPosition, 0, Creature::C_MONK, 1, turretLevel, Player::P_GOOD );
                            level.setCreature( leftGuardPosition, 1, Creature::C_WIZARD, 1, turretLevel, Player::P_GOOD );
                            level.setCreature( leftGuardPosition, 2, Creature::C_ARCHER, 1, turretLevel, Player::P_GOOD );
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

        enum class CorridorFurniture {
            CF_EMPTY,
            CF_DOOR_OPEN,
            CF_DOOR_LOCKED,
            CF_GAS,
            CF_LIGHTNING,
            CF_BOULDER,
            CF_HERO1,
            CF_HERO2,
        };

        void Fortress::prepareCorridorTraps() {
            static ProbabilityMass< CorridorFurniture > corridorItem;
            if ( corridorItem.empty() ) {
                corridorItem.set( CorridorFurniture::CF_EMPTY, 0.6 );
                corridorItem.set( CorridorFurniture::CF_DOOR_OPEN, 0.12 );
                corridorItem.set( CorridorFurniture::CF_DOOR_LOCKED, 0.08 );
                corridorItem.set( CorridorFurniture::CF_GAS, 0.1 );
                corridorItem.set( CorridorFurniture::CF_LIGHTNING, 0.1 );
                corridorItem.set( CorridorFurniture::CF_BOULDER, 0.1 );
                corridorItem.set( CorridorFurniture::CF_HERO1, 0.04 );
                corridorItem.set( CorridorFurniture::CF_HERO2, 0.04 );
                corridorItem.normalize();
            }

            const Player dungeonOwner = fortress.owner();

            /// draw corridors
            const SizeTSet guardLevel = parameters.getSizeTSet( ParameterName::PN_CORRIDOR_GUARD_LEVEL, 4, 7 );
            const auto connectedList = fortress.connectedRooms();
            for ( const std::pair< const spatial::FortressRoom*, const spatial::FortressRoom* >& pair: connectedList ) {
                const spatial::FortressRoom* room      = pair.first;
                const spatial::FortressRoom* connected = pair.second;

                const PointList corridor = fortress.getCorridor( *room, *connected );
                for ( const Point pt: corridor ) {
                    if ( level.countItems( pt ) > 0 ) {
                        continue ;
                    }

                    const CorridorFurniture furniture = corridorItem.getRandom();
                    switch( furniture ) {
                    case CorridorFurniture::CF_EMPTY: {
                        /// do nothing
                        break ;
                    }
                    case CorridorFurniture::CF_DOOR_OPEN: {
                        if ( level.canPlaceDoor( pt ) == false ) {
                            break ;
                        }
                        level.setDoor( pt, Door::D_IRON, false );
                        break ;
                    }
                    case CorridorFurniture::CF_DOOR_LOCKED: {
                        if ( level.canPlaceDoor( pt ) == false ) {
                            break ;
                        }
                        level.setDoor( pt, Door::D_IRON, true );
                        break ;
                    }
                    case CorridorFurniture::CF_GAS: {
                        level.setTrap( pt, Trap::T_POISON_GAS );
                        break ;
                    }
                    case CorridorFurniture::CF_LIGHTNING: {
                        level.setTrap( pt, Trap::T_LIGHTNING );
                        break ;
                    }
                    case CorridorFurniture::CF_BOULDER: {
                        level.setTrap( pt, Trap::T_BOULDER );
                        break ;
                    }
                    case CorridorFurniture::CF_HERO1: {
                        level.setCreature( pt, 1, Creature::C_ARCHER, 2, guardLevel.randomized(), dungeonOwner );
                        level.setCreature( pt, 1, Creature::C_DWARF,  2, guardLevel.randomized(), dungeonOwner );
                        break ;
                    }
                    case CorridorFurniture::CF_HERO2: {
                        level.setCreature( pt, 1, Creature::C_SAMURAI, 2, guardLevel.randomized(), dungeonOwner );
                        level.setCreature( pt, 1, Creature::C_WIZARD,  2, guardLevel.randomized(), dungeonOwner );
                        break ;
                    }
                    }
                }
            }
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

        bool Fortress::isBridgePossible( const Point startPoint ) const {
            static const PointList directions = { Point( 1, 0 ), Point( -1, 0 ), Point( 0, 1 ), Point( 0, -1 ) };
            for ( const Point dir: directions ) {
                const bool found = findFortified( startPoint, dir );
                if ( found == false ) {
                    return true;
                }
            }
            return false;
        }

        bool Fortress::findFortified( const Point startPoint, const Point bridgeDirection ) const {
            const Rect dungeonRect = fortress.boundingBox();

            std::set< Point > heighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
            heighbourDirections.erase( -bridgeDirection );
            Point bridgePoint = startPoint;
            while( true ) {
                if ( dungeonRect.isInside( bridgePoint ) == false ) {
                    break;
                }
                const Rect bridgeCellRect( bridgePoint, 3, 3 );
                if ( fortress.isCollision( bridgeCellRect ) ) {
                    /// fortified found
                    return true;
                }
                bridgePoint += bridgeDirection;
            }
            return false;
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
                Rect randRect( 18, 11 );
                randRect.moveRightTo( mapRect.max.x - 7 );
                randRect.moveBottomTo( mapRect.max.y - 3 );
                const std::size_t randomPosIndex = rng_randi( randRect.area() );
                const Point portalCenter = randRect.pointByIndex( randomPosIndex );
                const Rect portalRect( portalCenter, 3, 3 );
                level.setRoom( portalRect, Room::R_PORTAL );
            }

            /// add other
//            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
//                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
//                level.setItem( revPos, 4, Item::I_SPECIAL_REVMAP );
//
//                const adiktedpp::Player player = dungeon.owner();
//                level.setFortified( revPos + Point(0, 1), player );
//                level.setSlab( revPos + Point(0, 2), Slab::S_EARTH );
//                const Point monstersPos = revPos + Point(0, 3);
//                level.setSlab( monstersPos, Slab::S_PATH );
//                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 9 );
//            }

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

            availableRooms.setStateMode( Player::P_ALL, Room::R_PRISON, script::AvailableRoomMode::ARM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_TORTURE, script::AvailableRoomMode::ARM_DISABLED );
            availableRooms.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, script::AvailableRoomMode::ARM_DISABLED );
            /// 'ARM_ENABLED_FOUND' not available in current adikted lib version
//            availableRooms.setStateMode( Player::P_ALL, Room::R_PRISON, script::AvailableRoomMode::ARM_ENABLED_FOUND );
//            availableRooms.setStateMode( Player::P_ALL, Room::R_TORTURE, script::AvailableRoomMode::ARM_ENABLED_FOUND );
//            availableRooms.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, script::AvailableRoomMode::ARM_ENABLED_FOUND );

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
