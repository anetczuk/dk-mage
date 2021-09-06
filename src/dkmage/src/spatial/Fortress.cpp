/*
 * Fortress.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/DungeonGraph.h"
#include "dkmage/spatial/FortressRoom.h"

#include "adiktedpp/GameMap.h"
#include "adiktedpp/LakeGenerator.h"
#include "adiktedpp/AreaDetector.h"
#include "adiktedpp/script/Creator.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        void FortressDungeon::draw( FortressData& data ) const {
            adiktedpp::Level& level = data.level();
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const FortressRoom* > roomsList = rooms();

            /// draw corridors
            const auto connectedList = connectedRooms();
            for ( const std::pair< const FortressRoom*, const FortressRoom* >& pair: connectedList ) {
                const FortressRoom* room      = pair.first;
                const FortressRoom* connected = pair.second;

                const PointList corridor = getCorridor( *room, *connected );
                level.digCorridor( corridor, owner, fortify );
            }

            /// draw room content
            for ( const FortressRoom* item: roomsList ) {
                item->draw( data );
            }
        }

        FortressRoom* FortressDungeon::setFirstRoom( const FortressRoomType roomType ) {
            std::vector< FortressRoom* > roomsList = graph.itemsList();
            if ( roomsList.empty() == false ) {
                LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

            /// no rooms in dungeon -- add empty room
            std::unique_ptr< FortressRoom > emptyPtr = spawn_empty( *this );
            FortressRoom& emptyRoom = *emptyPtr;
            emptyPtr.release();                              //// release ownership (prevent deleting the object)
            emptyRoom.owner( player );
            graph.addItem( emptyRoom );

            /// add proper room
            FortressRoom* firstRoom = addRandomRoom( roomType, emptyRoom );
            graph.removeItem( emptyRoom );
            return firstRoom;
        }

        FortressRoom* FortressDungeon::addRandomRoom( const FortressRoomType roomType, const FortressRoom& from ) {
            std::unique_ptr< FortressRoom > roomPtr = spawn_object( *this, roomType );
            FortressRoom& room = *roomPtr;
            room.prepare( from );
            if ( exists( room ) == false ) {
                return nullptr;
            }
             /// room prepared/added properly
            roomPtr.release();                              //// release ownership (prevent deleting the object)
            room.owner( player );
            return &room;
        }

        /// randomize direction
        bool FortressDungeon::addRandomRoom( FortressRoom& newRoom, const FortressRoom& from, const std::size_t corridorLength ) {
            std::vector< Direction > availableDirs = from.restrictedDirections();
            if ( availableDirs.empty() ) {
                availableDirs = graph.freeDirections( from );
            }

            std::vector< Direction > available = availableDirs;
            while ( available.empty() == false ) {
                const std::size_t rDir = utils::rng_randi( available.size() );
                const Direction newDir = utils::remove_at( available, rDir );
                const bool added = createRoom( newRoom, from, newDir, corridorLength );
                if ( added == false ) {
                    continue;
                }
                /// new node added
                return true;
            }

//            LOG() << "unable to add room: " << (int)roomType;
            return false;
        }

        bool FortressDungeon::createRoom( FortressRoom& newRoom, const FortressRoom& from, const Direction direction, const std::size_t corridorLength ) {
            /// move room to fit direction and corridor length
            Rect newRect = newRoom.bbox();
            const Rect& basePos = from.bbox();
            moveRect( newRect, basePos, direction, corridorLength );

            const Point newOffset  = newRoom.joinCenterOffset();
            const Point fromOffset = from.joinCenterOffset();
            newRect.move(  fromOffset );
            newRect.move( -newOffset );

            /// check collision
            if ( canAdd( newRect, from, direction ) == false ) {
                return false;
            }

            const Point targetOffset = newRect.center() - newRoom.bbox().center();
            newRoom.move( targetOffset );

            return graph.setItem( newRoom, from, direction, true );
        }

        bool FortressDungeon::canAdd( const Rect& roomRect, const FortressRoom& from, const Direction direction ) const {
            if ( isRectInLimit( roomRect ) == false ) {
                return false;
            }

            // check room collision
            Rect collisionRect = roomRect;
            collisionRect.grow( 1 );
            if ( isCollision( collisionRect ) ) {
                /// collision detected
//                LOG() << "unable to add room -- rooms collision detected: " << newRect << " " << direction;
                return false;
            }

            /// check corridor collision
            const Point start = from.edgePoint( direction, 2 );
            const Point end   = roomRect.center();
            if ( isCollisionLine( start, end ) ) {
                /// collision detected
//                LOG() << "unable to add room -- corridor collision detected: " << newRect << " " << direction;
                return false;
            }

            return true;
        }

        bool FortressDungeon::isCollision( const Rect& rect ) const {
            std::vector< const FortressRoom* > roomsList = graph.itemsList();
            for ( const FortressRoom* item: roomsList ) {
                const Rect& itemRect = item->bbox();
                if ( itemRect.isCollision( rect ) ) {
//                    LOG() << "collision detected, rectangles: " << itemRect << " " << rect;;
                    return true;
                }

                /// check existing corridors
                const Point itemCenter = item->joinPoint();
                std::vector< const FortressRoom* > connected = connectedRooms( *item );
                for ( const FortressRoom* next: connected ) {
                    const Point nextCenter = next->joinPoint();
                    const std::vector<Point> points = line( itemCenter, nextCenter );
                    if ( is_collision( points, rect ) ) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool FortressDungeon::isCollisionLine( const Point& start, const Point& end ) const {
            const std::vector<Point> points = line( start, end );
            const std::size_t pSize = points.size();
            for ( std::size_t i=0; i<pSize; i += 3 ) {
                const Rect rect( points[i], 3, 3 );
                if ( isCollision(rect) ) {
                    return true;
                }
            }
            if ( pSize > 1 ) {
                const Rect pointRect( points[pSize-1], 3, 3 );
                if ( isCollision( pointRect ) ) {
                    return true;
                }
            }
            return false;
        }

        const PointList FortressDungeon::getCorridor( const FortressRoom& from, const FortressRoom& to ) const {
            const Point& fromPoint = from.joinPoint();
            const Point& toPoint   = to.joinPoint();
//            return line( fromPoint, toPoint );

            const PointList points = line( fromPoint, toPoint );
            PointList ret;
            ret.reserve( points.size() );
            for ( const Point pt: points ) {
                if ( from.isCorridor( pt ) == false ) {
                    continue ;
                }
                if ( to.isCorridor( pt ) == false ) {
                    continue ;
                }
                ret.push_back( pt );
            }
            return ret;
        }

        void FortressDungeon::calculateDistances() {
            /// clear distances (important when recalculating distances)
            std::vector< spatial::FortressRoom* > allRooms = rooms();
            for ( spatial::FortressRoom* item: allRooms ) {
                item->distanceToHeart = 0;
            }

            /// calculate distances
            std::vector< FortressRoom* > dungeonHearts = findRoom( FortressRoomType::FR_DUNGEON_HEART );
            for ( FortressRoom* heart: dungeonHearts ) {
                heart->distanceToHeart = 0;
                std::vector< FortressRoom* > queue;
                queue.push_back( heart );
                std::set< FortressRoom* > visited;
                visited.insert( heart );
                while( queue.empty() == false ) {
                    FortressRoom* start = queue.back();                                     /// yes, copy
                    queue.pop_back();
                    std::vector< FortressRoom* > connected = connectedRooms( *start );
                    if ( connected.empty() ) {
                        continue ;
                    }
                    const utils::Point joint = start->joinPoint();
                    for ( FortressRoom* next: connected ) {
                        if ( visited.count( next ) > 0 ) {
                            continue ;
                        }
                        visited.insert( next );

                        const utils::Point nextJoint = next->joinPoint();
                        const std::size_t jointDistance = nextJoint.distanceManhattan( joint );
                        next->distanceToHeart = start->distanceToHeart + jointDistance;
                        queue.push_back( next );
                    }
                }
            }
        }

        std::size_t FortressDungeon::maxDistance() const {
            int maxDist = 0;
            std::vector< const FortressRoom* > roomsList = graph.itemsList();
            for ( const FortressRoom* item: roomsList ) {
                const int currDistance = item->distanceToHeart;
                maxDist = std::max( maxDist, currDistance );
            }
            return (std::size_t) maxDist;
        }

        std::size_t FortressDungeon::closestDistance( const FortressRoomType roomType ) const {
            std::vector< const FortressRoom* > foundRooms = findRoom( roomType );
            int closest = -1;
            for ( const FortressRoom* item: foundRooms ) {
                const int currDistance = item->distanceToHeart;
                if ( currDistance < 0 ) {
                    continue ;
                }
                if ( closest < 0 ) {
                    closest = currDistance;
                    continue ;
                }
                closest = std::min( closest, currDistance );
            }
            if ( closest < 0 ) {
                return (std::size_t) -1;
            }
            return (std::size_t) closest;
        }

        std::string FortressDungeon::print() const {
            std::stringstream stream;
            stream << "bbox: " << boundingBox() << "\n";
            std::vector< const FortressRoom* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            for (std::size_t i=0; i<rSize; ++i) {
                const FortressRoom* room = roomsList[ i ];
                const std::size_t currId = graph.itemId( *room );
                stream << currId << ": " << room << " " << room->print() << "\n";
            }
            const std::vector<Direction>& directions = DirectionValues();
            for (std::size_t i=0; i<rSize; ++i) {
                const FortressRoom* room = roomsList[ i ];
                if ( room == nullptr ) {
                    stream << " null -> \n";
                    continue ;
                }
                const std::size_t currId = graph.itemId( *room );
                for ( const Direction dir: directions ) {
                    const FortressRoom* next = graph.getItem( *room, dir );
                    if ( next == nullptr ) {
//                        stream << currId << " -> " << dir << " null\n";
                        continue ;
                    }
                    const std::size_t nextId = graph.itemId( *next );
                    stream << currId << " -> " << dir << " " << nextId << "\n";
                }
            }
            return stream.str();
        }


        /// ===========================================================================================


        static const ProbabilityMass< spatial::FortressRoomType >& FortressRoomsProbability() {
            static ProbabilityMass< spatial::FortressRoomType > roomProbability;
            if ( roomProbability.empty() ) {
                roomProbability.set( spatial::FortressRoomType::FR_EMPTY, 0.05 );
                roomProbability.set( spatial::FortressRoomType::FR_TREASURE, 0.3, 4 );
                roomProbability.set( spatial::FortressRoomType::FR_TRAP, 0.3 );
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


        Fortress::Fortress( adiktedpp::GameMap& gameMap, ParametersMap& parameters ): gameMap(gameMap), parameters(parameters), fortress( Player::P_GOOD ) {
//                fortress.limitNorth = 3;
//                fortress.limitSouth = 3;
            fortress.fortify( true );
            roomProbability = FortressRoomsProbability();            /// yes, copy
        }

        bool Fortress::generate() {
            Rect lakeLimit = raw::RawLevel::mapRect( 6 );
            lakeLimit.growWidth( -1 );
            lakeLimit.max.y -= 24;                                      /// make space for evil dungeon

            fortress.limitWidth  = lakeLimit.width()  - 6;
            fortress.limitHeight = lakeLimit.height() - 6;

            spatial::FortressData fortressData = { gameMap, parameters };

            const spatial::FortressRoom* heart = fortress.setFirstRoom( spatial::FortressRoomType::FR_DUNGEON_HEART );
            std::vector< const spatial::FortressRoom* > branchStart;
            branchStart.push_back( heart );

            const std::size_t roomsNum = rng_randi( 4 ) + 3;
            std::vector< const spatial::FortressRoom* > mainCorridor = prepareCorridors( branchStart, roomsNum, false );
            {
                /// add junction room
                const spatial::FortressRoom* lastRoom = mainCorridor.back();
                const spatial::FortressRoom* next = addMainJunction( *lastRoom );
                if ( next == nullptr ) {
                    /// unable to create next room
                    LOG_WARN() << "unable to create main junction room";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                mainCorridor = { next };
                const spatial::FortressRoom* corridorExit = mainCorridor.back();
                for ( std::size_t i=0; i<2; ++i ) {
                    mainCorridor.push_back( corridorExit );
                }
            }

            LOG() << "generating branch corridors";
            const std::size_t branchLength = rng_randi( 4 ) + 5;
            const std::vector< const spatial::FortressRoom* > branches = prepareCorridors( mainCorridor, branchLength, true );
            if ( branches.size() < 2 ) {
                LOG_WARN() << "unable create at least two branch corridors";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                }
                return false;
            }
            prepareExitRooms( branches );

            /// secondary pass
//            if ( fortress.countRooms( spatial::FortressRoomType::FR_EXIT ) < 3 ) {
//                roomProbability.multiply( spatial::FortressRoomType::FR_EXIT, 3.0 );
//            }
            prepareSecondaryPass();

            /// check required rooms
            {
                if ( fortress.countRooms( spatial::FortressRoomType::FR_PRISON ) < 1 ) {
                    LOG_WARN() << "missing required prison";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.countRooms( spatial::FortressRoomType::FR_TORTURE ) < 1 ) {
                    LOG_WARN() << "missing required torture room";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.countRooms( spatial::FortressRoomType::FR_GRAVEYARD ) < 1 ) {
                    LOG_WARN() << "missing required graveyard";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
                if ( fortress.countRooms( spatial::FortressRoomType::FR_LAVA_POST ) < 1 ) {
                    LOG_WARN() << "missing required lava posts";
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                    return false;
                }
            }

            cutBlindCorridors();
            cutInvalidExits();
            /// once again, in case of removed exit rooms
            cutBlindCorridors();

            ensureExits();

            cutInvalidExits();
            cutBlindCorridors();

            std::vector< spatial::FortressRoom* > exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );
            if ( exitRooms.size() < 2 ) {
                LOG_WARN() << "unable create at least two exits";
                if ( parameters.isSet( ParameterName::PN_STOP_ON_FAIL ) ) {
                    /// draw for debug purpose
                    fortress.moveToTopEdge( 8 );
                    fortress.draw( fortressData );
                }
                return false;
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

            fortress.calculateDistances();

//            LOG() << "fortress:\n" << fortress.print();

            prepareCorridorTraps();

            return true;
        }

        bool Fortress::generateTest() {
            spatial::FortressData fortressData = { gameMap, parameters };
            const spatial::FortressRoom* heart  = fortress.setFirstRoom( spatial::FortressRoomType::FR_DUNGEON_HEART );
            const spatial::FortressRoom* post   = fortress.addRandomRoom( spatial::FortressRoomType::FR_LAVA_POST, *heart );
            const spatial::FortressRoom* prison = fortress.addRandomRoom( spatial::FortressRoomType::FR_PRISON, *post );
            fortress.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *prison );
            fortress.moveToTopEdge( 8 );
            fortress.draw( fortressData );
            return true;
        }

        std::vector< const spatial::FortressRoom* > Fortress::prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches ) {
            std::vector< const spatial::FortressRoom* > roomQueue = startRooms;

            std::size_t failedAttempts = 0;

            /// create branches
            for ( std::size_t i=0; i<roomsNum; ++i ) {
                std::vector< const spatial::FortressRoom* > nextRooms;
                const std::size_t qSize = roomQueue.size();
                for ( std::size_t x=0; x<qSize; ++x ) {
                    const spatial::FortressRoomType roomType = roomProbability.getRandom();
                    const spatial::FortressRoom* item = roomQueue[ x ];
                    const spatial::FortressRoom* next = fortress.addRandomRoom( roomType, *item );
                    if ( next == nullptr ) {
                        ++failedAttempts;
//                        LOG() << "unable to generate chamber level " << i;
//                        LOG() << "unable to generate chamber level " << i << " " << roomType;
                        continue ;
                    }

                    roomProbability.popItem( roomType );
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
                if ( nextRooms.empty() ) {
                    break ;
                }
                roomQueue = nextRooms;

                if ( allowBranches ) {
                    /// increase probability to fulfill requirements
                    roomProbability.multiply( FortressRoomType::FR_PRISON, 2.0 );
                    roomProbability.multiply( FortressRoomType::FR_TORTURE, 2.0 );
                    roomProbability.multiply( FortressRoomType::FR_GRAVEYARD, 2.0 );
                    roomProbability.multiply( FortressRoomType::FR_LAVA_POST, 2.0 );
                    roomProbability.normalize();
                }
            }

            LOG() << "chamber generation failed attempts " << failedAttempts;

            return roomQueue;
        }

        void Fortress::prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms ) {
            /// create branch exit
            std::vector< const spatial::FortressRoom* > uniqueRooms( startRooms.begin(), startRooms.end() );
            remove_dupes( uniqueRooms );

            std::size_t failedAttempts = 0;

            for ( const spatial::FortressRoom* item: uniqueRooms ) {
                const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *item );
                if ( next == nullptr ) {
//                    LOG() << "unable to generate branch exit";
                    ++failedAttempts;
                    continue ;
                }
            }

            LOG() << "exit room failed attempts " << failedAttempts;
        }

        void Fortress::prepareSecondaryPass() {
            LOG() << "performing secondary pass";
            std::vector< const spatial::FortressRoom* > roomQueue = const_cast< const spatial::FortressDungeon& >( fortress ).rooms();

//            LOG() << "fortress:\n" << fortress.print();

            /// create branches
            std::size_t stepsCounter = 0;
            while ( roomQueue.empty() == false ) {
                std::set< const spatial::FortressRoom* > blindCandidates;
                std::vector< const spatial::FortressRoom* > nextRooms;

                /// add new rooms
                for ( const spatial::FortressRoom* item: roomQueue ) {
                    const spatial::FortressRoomType itemType = item->type();
                    if ( itemType == spatial::FortressRoomType::FR_DUNGEON_HEART ) {
                        continue ;
                    }
                    if ( itemType == spatial::FortressRoomType::FR_EXIT ) {
                        continue ;
                    }

                    const spatial::FortressRoomType roomType = roomProbability.getRandom();
                    const spatial::FortressRoom* next = fortress.addRandomRoom( roomType, *item );
                    if ( next == nullptr ) {
                        /// unable to add next room from 'item' room
                        blindCandidates.insert( item );
                        continue ;
                    }
                    roomProbability.popItem( roomType );
                    nextRooms.push_back( next );
                }

                /// remove blind corridors
                for ( const spatial::FortressRoom* item: blindCandidates ) {
                    remove( nextRooms, item );

                    const spatial::FortressRoom* prev = cutBlindCorridor( *item );
                    if ( prev == nullptr ) {
                        continue ;
                    }
                    nextRooms.push_back( prev );
                }

                /// make additional branches
                const std::size_t nSize = nextRooms.size();
                for ( std::size_t i=0; i<nSize; ++i ) {
                    const spatial::FortressRoom* next = nextRooms[i];
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
                    const spatial::FortressRoom* next = item;
                    while ( next != nullptr ) {
                        next = cutBlindCorridor( *next );
                        if ( next != nullptr ) {
                            removed = true;
                        }
                    }
                }
            } while( removed == true );
        }

        const spatial::FortressRoom* Fortress::addMainJunction( const spatial::FortressRoom& room ) {
            if ( room.restrictedDirections().empty() ) {
                return &room;
            }
            /// restricted room -- add unrestricted one
            const FortressRoomType lastType = room.type();
            const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_TRAP, room );
            if ( next != nullptr ) {
                return next;
            }
            const std::vector< spatial::FortressRoom* > neighbours = fortress.connectedRooms( room );
            if ( neighbours.size() > 1 ) {
                LOG_WARN() << "unable to add main junction room";
                return nullptr;
            }
            const bool removed = fortress.removeRoom( room );
            if ( removed == false ) {
                return nullptr;
            }
            const spatial::FortressRoom* prev = neighbours[0];
            next = fortress.addRandomRoom( spatial::FortressRoomType::FR_TRAP, *prev );
            if ( next == nullptr ) {
                LOG_WARN() << "unable to add main junction room";
                return nullptr;
            }
            /// restore removed room
            return fortress.addRandomRoom( lastType, *next );
        }

        const spatial::FortressRoom* Fortress::cutBlindCorridor( const spatial::FortressRoom& room ) {
            const std::vector< spatial::FortressRoom* > neighbours = fortress.connectedRooms( room );
            if ( neighbours.size() > 1 ) {
                return nullptr;
            }
            const Rect itemRect = room.bbox();
            if ( itemRect.width() > 1 && itemRect.height() > 1 ) {
                return nullptr;
            }
            /// dead-end corridor
            fortress.removeRoom( room );
            return neighbours[0];
        }

        void Fortress::cutInvalidExits() {
            std::vector< spatial::FortressRoom* > exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );

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

        void Fortress::ensureExits() {
            std::vector< spatial::FortressRoom* > exitRooms = fortress.findRoom( spatial::FortressRoomType::FR_EXIT );
            if ( exitRooms.size() > 2 ) {
                return ;
            }
            /// try to insert exits
            LOG() << "adding exits";

            fortress.calculateDistances();
            const std::size_t closestExitDistance = fortress.closestDistance( spatial::FortressRoomType::FR_EXIT ) * 0.8;
            const std::size_t halfDistance = fortress.maxDistance() / 1.5;
//            const std::size_t minExitDistance = halfDistance;
            const std::size_t minExitDistance = std::max( closestExitDistance, halfDistance );

            std::vector< const spatial::FortressRoom* > roomQueue = const_cast< const spatial::FortressDungeon& >( fortress ).rooms();

//            LOG() << "fortress:\n" << fortress.print();

            /// create branches
            std::size_t exitsCounter = exitRooms.size();
            for ( std::size_t i=0; i<3; ++i ) {
                std::vector< const spatial::FortressRoom* > nextRooms;

                /// add new rooms
                for ( const spatial::FortressRoom* item: roomQueue ) {
                    const spatial::FortressRoomType itemType = item->type();
                    if ( itemType == spatial::FortressRoomType::FR_DUNGEON_HEART ) {
                        continue ;
                    }
                    if ( itemType == spatial::FortressRoomType::FR_EXIT ) {
                        continue ;
                    }
                    if ( (std::size_t) item->distanceToHeart < minExitDistance ) {
                        continue ;
                    }

                    const spatial::FortressRoom* next = fortress.addRandomRoom( spatial::FortressRoomType::FR_EXIT, *item );
                    if ( next != nullptr ) {
                        ++exitsCounter;
                    }
                }

                if ( exitsCounter > 3) {
                    break;
                }
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

            Level& level = gameMap.level;

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

        bool Fortress::prepareBridges( const std::vector< spatial::FortressRoom* >& exitRooms ) {
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
                if ( bSize < 1 ) {
                    LOG() << "unable find bridge shore";
                    continue;
                }
//                if ( bSize > 10 ) {
//                    LOG() << "bridge too long";
//                    continue;
//                }

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

            Level& level = gameMap.level;

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
                const PointList& bridgePoints = bridge.bridge;

                const std::size_t bridgeSize = bridgePoints.size();
                Point bridgeDirection( 0, 0 );
                if ( bridgePoints.size() > 1 ) {
                    bridgeDirection = bridgePoints[1] - bridgePoints[0];
                }
                const Point bridgeOrtho = bridgeDirection.swapped();
                std::set< Point > neighbourDirections = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };
                neighbourDirections.erase( -bridgeDirection );

//                adiktedpp::script::Script& script = gameMap.script;
//                script::ScriptCommand& parties = script.partiesSection();
//                parties.addEmptyLine();
//                parties.REM( "- bridge turret parties -" );
//
//                const Point bridgeCenter = bridgePoints[ bridgeSize / 2 ];
//                const std::size_t bridgeAP  = level.addActionPoint( bridgeCenter, bridgeSize / 2 + 3 );
//                script::ScriptCommand& action = script.actionSection();
//                action.addEmptyLine();
//                action.REM( "- bridge turrets -" );
//                action.REM( std::to_string( bridgeAP ) + " -- bridge center" );
//                action.IF_ACTION_POINT( bridgeAP, Player::P_P0 );

                std::size_t i = 0;
                for ( const Point bridgePoint: bridgePoints ) {
                    ++i;

                    /// set bridge stub
                    level.setSlab( bridgePoint, Slab::S_PATH );

                    if ( i > 11 ) {
                        /// do not add more guards along long bridges
                        continue ;
                    }

                    /// add bridge keepers
                    if ( i == 1 ) {
                        /// add hero gate
                        level.setItem( bridgePoint, 4, Item::I_HEROGATE );
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

//                            const std::size_t turrentAP = level.addActionPoint( rightGuardPosition );
//                            const std::string turretPartyName =  "bridge_turret_" + std::to_string( turrentAP );
//                            parties.CREATE_PARTY( turretPartyName );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_MONK, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_WIZARD, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_ARCHER, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//
//                            action.ADD_PARTY_TO_LEVEL( Player::P_GOOD, turretPartyName, turrentAP );

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

//                            const std::size_t turrentAP = level.addActionPoint( leftGuardPosition );
//                            const std::string turretPartyName =  "bridge_turret_" + std::to_string( turrentAP );
//                            parties.CREATE_PARTY( turretPartyName );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_MONK, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_WIZARD, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//                            parties.ADD_TO_PARTY( turretPartyName, Creature::C_ARCHER, 1, turretLevel, 0, script::PartyObjective::PO_DEFEND_LOCATION  );
//
//                            action.ADD_PARTY_TO_LEVEL( Player::P_GOOD, turretPartyName, turrentAP );

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

//                action.ENDIF();
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
                corridorItem.set( CorridorFurniture::CF_DOOR_OPEN, 0.06 );
                corridorItem.set( CorridorFurniture::CF_DOOR_LOCKED, 0.14 );
                corridorItem.set( CorridorFurniture::CF_GAS, 0.1 );
                corridorItem.set( CorridorFurniture::CF_LIGHTNING, 0.1 );
                corridorItem.set( CorridorFurniture::CF_BOULDER, 0.1 );
                corridorItem.set( CorridorFurniture::CF_HERO1, 0.04 );
                corridorItem.set( CorridorFurniture::CF_HERO2, 0.04 );
                corridorItem.normalize();
            }

            Level& level = gameMap.level;
//            adiktedpp::script::Script& script = gameMap.script;
            const Player dungeonOwner = fortress.owner();

            const SizeTSet guardLevel = parameters.getSizeTSet( ParameterName::PN_CORRIDOR_GUARD_LEVEL, 4, 7 );
            const int maxDistance = fortress.maxDistance();
            double distanceMinFactor = 1.0;
            double distanceMaxFactor = 0.0;

            const auto connectedList = fortress.connectedRooms();
            for ( const std::pair< const spatial::FortressRoom*, const spatial::FortressRoom* >& pair: connectedList ) {
                const spatial::FortressRoom* room      = pair.first;
                const spatial::FortressRoom* connected = pair.second;

                const int distanceToHeart = std::min( room->distanceToHeart, connected->distanceToHeart );
                const double distanceFactor = (double)distanceToHeart / maxDistance;

                distanceMinFactor = std::min( distanceMinFactor, distanceFactor );
                distanceMaxFactor = std::max( distanceMaxFactor, distanceFactor );
            }
            const double distanceDiff = distanceMaxFactor - distanceMinFactor;

            /// draw corridors
            for ( const std::pair< const spatial::FortressRoom*, const spatial::FortressRoom* >& pair: connectedList ) {
                const spatial::FortressRoom* room      = pair.first;
                const spatial::FortressRoom* connected = pair.second;

                const int distanceToHeart = std::min( room->distanceToHeart, connected->distanceToHeart );
                const double distanceFactor = (double)distanceToHeart / maxDistance;
                const double corridorFactor = 1.0 - (distanceFactor - distanceMinFactor) / distanceDiff;
                const std::size_t creatureLevel = guardLevel.valueByFactor( corridorFactor );

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
//                        const std::size_t corridorAP = level.addActionPoint( pt, 0 );
//                        const std::string corridorPartyName  = "corridor_guard_" + std::to_string( corridorAP );
//
//                        script::AmbushCreator ambush( script );
//                        ambush.partyId = corridorPartyName;
//                        ambush.description = "- corridor ambush -";
//                        ambush.actionPoint = corridorAP;
//                        ambush.partyCopies = 2;
//                        ambush.owner = dungeonOwner;
//                        ambush.prepare();
//                        ambush.addToParty( Creature::C_ARCHER, 1, creatureLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );
//                        ambush.addToParty( Creature::C_DWARF,  1, creatureLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );

                        level.setCreature( pt, 1, Creature::C_ARCHER, 2, creatureLevel, dungeonOwner );
                        level.setCreature( pt, 1, Creature::C_DWARF,  2, creatureLevel, dungeonOwner );
                        break ;
                    }
                    case CorridorFurniture::CF_HERO2: {
//                        const std::size_t corridorAP = level.addActionPoint( pt, 0 );
//                        const std::string corridorPartyName  = "corridor_guard_" + std::to_string( corridorAP );
//
//                        script::AmbushCreator ambush( script );
//                        ambush.partyId = corridorPartyName;
//                        ambush.description = "- corridor ambush -";
//                        ambush.actionPoint = corridorAP;
//                        ambush.partyCopies = 2;
//                        ambush.owner = dungeonOwner;
//                        ambush.prepare();
//                        ambush.addToParty( Creature::C_SAMURAI, 1, creatureLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );
//                        ambush.addToParty( Creature::C_WIZARD,  1, creatureLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );

                        level.setCreature( pt, 1, Creature::C_SAMURAI, 2, creatureLevel, dungeonOwner );
                        level.setCreature( pt, 1, Creature::C_WIZARD,  2, creatureLevel, dungeonOwner );
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
//                Point nextPoint = startPoint + dir;
//                if ( level.isSlab( nextPoint, Slab::S_LAVA ) == false ) {
//                    continue ;
//                }
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
            Level& level = gameMap.level;

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
//                    LOG() << "found other slab -- unable to find shore";
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

    } /* namespace spatial */
} /* namespace dkmage */
