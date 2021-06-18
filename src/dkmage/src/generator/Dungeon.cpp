/*
 * Dungeon.cpp
 *
 */

#include "dkmage/generator/Dungeon.h"

#include <random>


namespace dkmage {
    namespace generator {

        template <typename T>
        T remove_at( std::vector<T>& v, typename std::vector<T>::size_type n ) {
            std::swap(v[n], v.back());
            T last = v.back();
            v.pop_back();
            return last;
        }

        void moveRect( Rect& rect, const Rect& base, const Direction direction, const std::size_t space ) {
            rect.centerize();
            const Point baseCenter = base.center();
            rect.move( baseCenter );

            switch( direction ) {
            case Direction::D_NORTH: {
                const int yOffset = ( rect.height() + base.height() ) / 2 + space;
                rect.move( 0, -yOffset );
                break;
            }
            case Direction::D_SOUTH: {
                const int yOffset = ( rect.height() + base.height() ) / 2 + space;
                rect.move( 0, yOffset );
                break;
            }
            case Direction::D_EAST: {
                const int xOffset = ( rect.width() + base.width() ) / 2 + space;
                rect.move( xOffset, 0 );
                break;
            }
            case Direction::D_WEST: {
                const int xOffset = ( rect.width() + base.width() ) / 2 + space;
                rect.move( -xOffset, 0 );
                break;
            }
            }

        }

        template <typename T>
        void remove( std::vector<T>& vec, const T data ) {
            vec.erase( std::remove( vec.begin(), vec.end(), data ), vec.end() );
        }


        /// ============================================================


        std::string Room::print() const {
            std::stringstream stream;
            stream << "position: " << position() << " type: " << roomType;
            return stream.str();
        }


        /// ============================================================


        std::vector< Room* > Dungeon::findRoom( const adiktedpp::SlabType roomType ) {
            std::vector< Room* > ret;
            std::vector< Room* > allRooms = graph.itemsList();
            for ( Room* item: allRooms ) {
                if ( item == nullptr ) {
                    continue ;
                }
                if ( item->type() == roomType ) {
                    ret.push_back( item );
                }
            }
            return ret;
        }

        Room* Dungeon::findRoomFirst( const adiktedpp::SlabType roomType ) {
            std::vector< Room* > rooms = findRoom( roomType );
            if ( rooms.empty() ) {
                return nullptr;
            }
            return rooms[0];
        }

        void Dungeon::addRandomRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize, const bool addLink, const std::size_t distance ) {
            std::vector< Room* > roomsList = graph.itemsList();
            while ( roomsList.empty() == false ) {
                const std::size_t rRoom = rand() % roomsList.size();
                Room* selected = remove_at( roomsList, rRoom );
                if ( selected == nullptr ) {
                    continue;
                }
                std::vector< Direction > availableDirs = graph.freeDirections( *selected );

                const int northDiff = limitNorth - selected->northCoord;
                if ( northDiff <= 0) {
                    remove( availableDirs, Direction::D_NORTH );
                }
                const int southDiff = limitSouth + selected->northCoord;
                if ( southDiff <= 0) {
                    remove( availableDirs, Direction::D_SOUTH );
                }
                const int eastDiff = limitEast - selected->eastCoord;
                if ( eastDiff <= 0) {
                    remove( availableDirs, Direction::D_EAST );
                }
                const int westDiff = limitWest + selected->eastCoord;
                if ( westDiff == 0) {
                    remove( availableDirs, Direction::D_WEST );
                }

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = rand() % availableDirs.size();
                    const Direction newDir = remove_at( availableDirs, rDir );
                    const Room* added = addRoom( roomType, roomSize, *selected, newDir, addLink, distance );
                    if ( added != nullptr ) {
                        return ;
                    }
                }
            }
            LOG() << "unable to add room: " << roomType;
        }

        Room* Dungeon::addRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize ) {
            const Rect newRect( roomSize, roomSize );
            if ( isCollision( newRect ) ) {
                /// collision detected
                return nullptr;
            }

            Room* newRoom = graph.addItem();
            if ( newRoom == nullptr ) {
                return newRoom;
            }

            /// new node added
            newRoom->position() = newRect;
            newRoom->type( roomType );
            newRoom->owner( player );
            return newRoom;
        }

        Room* Dungeon::addRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize, const Room& from, const Direction direction, const bool addLink, const std::size_t distance ) {
            Rect newRect( roomSize, roomSize );
            const Rect& basePos = from.position();
            moveRect( newRect, basePos, direction, distance );
            if ( isCollision( newRect ) ) {
                /// collision detected
                return nullptr;
            }

            Room* newRoom = graph.addItem( from, direction, addLink );
            if ( newRoom == nullptr ) {
                return newRoom;
            }

            switch( direction ) {
            case Direction::D_NORTH: {
                newRoom->northCoord = from.northCoord + 1;
                newRoom->eastCoord  = from.eastCoord;
                break;
            }
            case Direction::D_SOUTH: {
                newRoom->northCoord = from.northCoord - 1;
                newRoom->eastCoord  = from.eastCoord;
                break;
            }
            case Direction::D_EAST: {
                newRoom->northCoord = from.northCoord;
                newRoom->eastCoord  = from.eastCoord + 1;
                break;
            }
            case Direction::D_WEST: {
                newRoom->northCoord = from.northCoord;
                newRoom->eastCoord  = from.eastCoord - 1;
                break;
            }
            }

            /// new node added
            newRoom->position() = newRect;
            newRoom->type( roomType );
            newRoom->owner( player );
            return newRoom;
        }

        void Dungeon::generate( const std::size_t roomsNum, const std::size_t roomSize ) {
            std::vector< adiktedpp::SlabType > roomsType;
            {
                roomsType.push_back( adiktedpp::SlabType::ST_TREASURE );
                roomsType.push_back( adiktedpp::SlabType::ST_LAIR );
                roomsType.push_back( adiktedpp::SlabType::ST_HATCHERY );
                roomsType.push_back( adiktedpp::SlabType::ST_TRAINING );
                roomsType.push_back( adiktedpp::SlabType::ST_LIBRARY );
                roomsType.push_back( adiktedpp::SlabType::ST_WORKSHOP );
                roomsType.push_back( adiktedpp::SlabType::ST_PRISONCASE );
                roomsType.push_back( adiktedpp::SlabType::ST_TORTURE );
                roomsType.push_back( adiktedpp::SlabType::ST_GRAVEYARD );
                roomsType.push_back( adiktedpp::SlabType::ST_TEMPLE );
                roomsType.push_back( adiktedpp::SlabType::ST_SCAVENGER );
            }
            {
                Room* root = graph.addItem();
                root->resize( roomSize );
                Rect& basePos = root->position();
                basePos.centerize();
                root->type( adiktedpp::SlabType::ST_DUNGHEART );
                root->owner( player );
            }
            for (std::size_t i=1; i<roomsNum; ++i) {
                const std::size_t roomIndex = ( i - 1 ) % roomsType.size();
                const adiktedpp::SlabType newRoomType = roomsType[roomIndex];
                /// randomize next room
                addRandomRoom( newRoomType, roomSize );
            }
        }

        Rect Dungeon::boundingBox() {
            std::vector< Room* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            if ( rSize < 1 ) {
                return Rect();
            }
            Rect minMax = roomsList[0]->position();
            for (std::size_t i=1; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                const Rect& roomPos = room->position();
                minMax.expand( roomPos );
            }
            return minMax;
        }

        bool Dungeon::isCollision( const Rect& rect ) {
            std::vector< Room* > roomsList = graph.itemsList();
            for ( const Room* item: roomsList ) {
                const Rect& pos = item->position();
                if ( rect.isCollision( pos ) ) {
                    return true;
                }
            }
            return false;
        }

        void Dungeon::centerize() {
            const Rect bbox = boundingBox();
            const Point center = bbox.center();
            move( -center.x, -center.y );
        }

        void Dungeon::centerize( const int x, const int y ) {
            centerize();
            move( x, y );
        }

        void Dungeon::moveToTopEdge( const std::size_t distanceFromEdge ) {
            centerize( 42, 42 );
            const Rect bbox = boundingBox();
            const int yoffset = 0 - bbox.min.y + distanceFromEdge + 1;          /// +1 to prevent going outside map
            move( 0, yoffset );
        }

        void Dungeon::moveToBottomEdge( const std::size_t distanceFromEdge ) {
            centerize( 42, 42 );
            const Rect bbox = boundingBox();
            const int yoffset = 84 - bbox.max.y - distanceFromEdge - 1;         /// -1 to prevent going outside map
            move( 0, yoffset );
        }

        std::string Dungeon::print() {
            std::stringstream stream;
            stream << "bbox: " << boundingBox() << "\n";
            std::vector< Room* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            for (std::size_t i=0; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                const std::size_t currId = graph.itemId( *room );
                stream << currId << ": " << room->print() << "\n";
            }
            const std::vector<Direction>& directions = DirectionValues();
            for (std::size_t i=0; i<rSize; ++i) {
                const Room* room = roomsList[ i ];
                if ( room == nullptr ) {
                    continue ;
                }
                const std::size_t currId = graph.itemId( *room );
                for ( const Direction dir: directions ) {
                    Room* next = graph.getItem( *room, dir );
                    if ( next == nullptr ) {
                        continue ;
                    }
                    const std::size_t nextId = graph.itemId( *next );
                    stream << currId << " -> " << dir << " " << nextId << "\n";
                }
            }
            return stream.str();
        }

    } /* namespace generator */
} /* namespace dkmage */
