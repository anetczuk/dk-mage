/*
 * Fortress.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/DungeonGraph.h"
#include "dkmage/spatial/FortressRoom.h"

#include "adiktedpp/GameMap.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        void FortressDungeon::draw( adiktedpp::GameMap& gameMap ) const {
            adiktedpp::Level& level = gameMap.level;
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const FortressRoom* > roomsList = rooms();

            /// draw corridors
            for ( const FortressRoom* item: roomsList ) {
                const Point& itemCenter = item->joinPoint();
                std::vector< const FortressRoom* > connectedList = connectedRooms( *item );
                for ( const FortressRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->joinPoint();
                    level.digLine( itemCenter, connectedCenter, owner, fortify );
                }

            }

            /// draw room content
            for ( const FortressRoom* item: roomsList ) {
                item->draw( gameMap );
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
            if ( isCollision( start, end ) ) {
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

        bool FortressDungeon::isCollision( const Point& start, const Point& end ) const {
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

    } /* namespace spatial */
} /* namespace dkmage */
