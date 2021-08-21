/*
 * Fortress.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/DungeonGraph.h"
#include "dkmage/spatial/FortressRoom.h"

#include "adiktedpp/Level.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        void FortressDungeon::draw( adiktedpp::Level& level ) const {
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const FortressRoom* > roomsList = rooms();
            for ( const FortressRoom* item: roomsList ) {
                /// draw corridors
                const Point& itemCenter = item->joinPoint();
                std::vector< const FortressRoom* > connectedList = connectedRooms( *item );
                for ( const FortressRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->joinPoint();
                    level.digLine( itemCenter, connectedCenter, owner, fortify );
                }

                /// draw content
                item->draw( *this, level );
            }
        }

        FortressRoom* FortressDungeon::setFirstRoom( const FortressRoomType roomType, const std::size_t roomSize ) {
            std::vector< FortressRoom* > roomsList = graph.itemsList();
            if ( roomsList.empty() == false ) {
                LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

            /// no rooms in dungeon
            std::unique_ptr< FortressRoom > roomPtr = spawn_object( roomType );
            FortressRoom& newRoom = *roomPtr;
            roomPtr.release();                              //// release ownership (prevent deleting the object)

            graph.addItem( newRoom );

            newRoom.owner( player );
            Rect newRect( roomSize, roomSize );
            newRect.centerize();
            newRoom.setPosition( newRect );
            return &newRoom;
        }

        FortressRoom* FortressDungeon::addRandomRoom( const FortressRoomType roomType, const FortressRoom& from ) {
            std::unique_ptr< FortressRoom > roomPtr = spawn_object( roomType );
            FortressRoom& room = *roomPtr;
            room.prepare( *this, from );
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
            Rect newRect = newRoom.position();
            const Rect& basePos = from.position();
            moveRect( newRect, basePos, direction, corridorLength );

            const Point newOffset  = newRoom.joinCenterOffset();
            const Point fromOffset = from.joinCenterOffset();
            newRect.move(  fromOffset );
            newRect.move( -newOffset );

            /// check collision
            if ( canAdd( newRect, from, direction ) == false ) {
                return false;
            }

            const Point targetOffset = newRect.center() - newRoom.position().center();
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
                const Rect& itemRect = item->position();
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
