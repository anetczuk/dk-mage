/*
 * Dungeon.cpp
 *
 */

#include "dkmage/spatial/Dungeon.h"

#include "utils/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        template <typename T>
        T remove_at( std::vector<T>& v, typename std::vector<T>::size_type n ) {
            std::swap(v[n], v.back());
            T last = v.back();
            v.pop_back();
            return last;
        }

        Point movePoint( const Point& base, const Direction direction, const std::size_t space ) {
            switch( direction ) {
            case Direction::D_NORTH: {
                return base + Point( 0, -space );
            }
            case Direction::D_SOUTH: {
                return base + Point( 0,  space );
            }
            case Direction::D_EAST: {
                return base + Point(  space, 0 );
            }
            case Direction::D_WEST: {
                return base + Point( -space, 0 );
            }
            }

            LOG() << "unhandled case: " << direction;
            return base;
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


        Point DungeonRoom::edgePoint( const Direction direction, const std::size_t delta ) const {
            switch( direction ) {
            case Direction::D_NORTH: return roomPosition.centerTop( delta );
            case Direction::D_SOUTH: return roomPosition.centerBottom( delta );
            case Direction::D_WEST:  return roomPosition.centerLeft( delta );
            case Direction::D_EAST:  return roomPosition.centerRight( delta );
            }
            LOG() << "invalid case";
            return roomPosition.center();
        }

        std::string DungeonRoom::print() const {
            std::stringstream stream;
            stream << "position: " << position() << " type: " << roomType;
            return stream.str();
        }


        /// ============================================================


        std::vector< DungeonRoom* > Dungeon::findRoom( const Room roomType ) {
            std::vector< DungeonRoom* > ret;
            std::vector< DungeonRoom* > allRooms = graph.itemsList();
            for (  DungeonRoom* item: allRooms ) {
                if ( item == nullptr ) {
                    continue ;
                }
                if ( item->type() == roomType ) {
                    ret.push_back( item );
                }
            }
            return ret;
        }

        DungeonRoom* Dungeon::findRoomFirst( const Room roomType ) {
            std::vector< DungeonRoom* > rooms = findRoom( roomType );
            if ( rooms.empty() ) {
                return nullptr;
            }
            return rooms[0];
        }

        DungeonRoom* Dungeon::addRandomRoom( const Room roomType, const std::size_t roomSize, const bool addLink, const std::size_t distance ) {
            std::vector< DungeonRoom* > roomsList = graph.itemsList();
            if ( roomsList.empty()) {
                /// no rooms in dungeon
                DungeonRoom* root = graph.addItem();
                root->resize( roomSize );
                Rect& basePos = root->position();
                basePos.centerize();
                root->type( roomType );
                root->owner( player );
                return root;
            }

            while ( roomsList.empty() == false ) {
                const std::size_t rRoom = rng_randi( roomsList.size() );
                DungeonRoom* selected = remove_at( roomsList, rRoom );
                if ( selected == nullptr ) {
                    continue;
                }

                DungeonRoom* added = addRandomRoom( roomType, roomSize, *selected, addLink, distance );
                if ( added != nullptr ) {
                    return added;
                }
            }

            LOG() << "unable to add room: " << (int)roomType;
            return nullptr;
        }

        DungeonRoom* Dungeon::addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const DungeonRoom& from, const bool addLink, const std::size_t corridorLength ) {
            return addRandomRoom( roomType, roomSize, roomSize, from, addLink, corridorLength );
        }

        DungeonRoom* Dungeon::addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const DungeonRoom& from, const bool addLink, const std::size_t corridorLength ) {
            std::vector< Direction > availableDirs = graph.freeDirections( from );

            const int northDiff = limitNorth - from.northCoord;
            if ( northDiff <= 0) {
                remove( availableDirs, Direction::D_NORTH );
            }
            const int southDiff = limitSouth + from.northCoord;
            if ( southDiff <= 0) {
                remove( availableDirs, Direction::D_SOUTH );
            }
            const int eastDiff = limitEast - from.eastCoord;
            if ( eastDiff <= 0) {
                remove( availableDirs, Direction::D_EAST );
            }
            const int westDiff = limitWest + from.eastCoord;
            if ( westDiff == 0) {
                remove( availableDirs, Direction::D_WEST );
            }

            while ( availableDirs.empty() == false ) {
                const std::size_t rDir = rng_randi( availableDirs.size() );
                const Direction newDir = remove_at( availableDirs, rDir );
                DungeonRoom* added = addRoom( roomType, roomSizeX, roomSizeY, from, newDir, addLink, corridorLength );
                if ( added != nullptr ) {
                    return added;
                }
            }

//            LOG() << "unable to add room: " << (int)roomType;
            return nullptr;
        }

        DungeonRoom* Dungeon::addRoom( const Room roomType, const std::size_t roomSize ) {
            Rect newRect( roomSize, roomSize );

            Rect collisionRect = newRect;
            collisionRect.grow( 1 );
            if ( isCollision( collisionRect ) ) {
                /// collision detected
                return nullptr;
            }

            DungeonRoom* newRoom = graph.addItem();
            if ( newRoom == nullptr ) {
                return nullptr;
            }

            /// new node added
            if ( graph.size() == 1 ) {
                /// only room in dungeon -- centerize root
                newRect.centerize();
            }
            newRoom->position() = newRect;
            newRoom->type( roomType );
            newRoom->owner( player );
            return newRoom;
        }

        DungeonRoom* Dungeon::addRoom( const Room roomType, const std::size_t roomSize, const DungeonRoom& from, const Direction direction, const bool addLink, const std::size_t corridorLength ) {
            return addRoom( roomType, roomSize, roomSize, from, direction, addLink, corridorLength );
        }

        DungeonRoom* Dungeon::addRoom( const Room roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const DungeonRoom& from, const Direction direction, const bool addLink, const std::size_t corridorLength ) {
            Rect newRect( roomSizeX, roomSizeY );
            const Rect& basePos = from.position();
            moveRect( newRect, basePos, direction, corridorLength );

            // check room collision
            Rect collisionRect = newRect;
            collisionRect.grow( 1 );
            if ( isCollision( collisionRect ) ) {
                /// collision detected
//                LOG() << "unable to add room -- rooms collision detected: " << newRect << " " << direction;
                return nullptr;
            }

            /// check corridor collision
            const Point start = from.edgePoint( direction, 2 );
            const Point end = newRect.center();
            if ( isCollision( start, end ) ) {
                /// collision detected
//                LOG() << "unable to add room -- corridor collision detected: " << newRect << " " << direction;
                return nullptr;
            }

            DungeonRoom* newRoom = graph.addItem( from, direction, addLink );
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
            std::vector< Room > roomsType;
            {
                roomsType.push_back( Room::R_TREASURE );
                roomsType.push_back( Room::R_LAIR );
                roomsType.push_back( Room::R_HATCHERY );
                roomsType.push_back( Room::R_TRAINING );
                roomsType.push_back( Room::R_LIBRARY );
                /// bridge
                roomsType.push_back( Room::R_WORKSHOP );
                /// guard post
                roomsType.push_back( Room::R_PRISON );
                roomsType.push_back( Room::R_TORTURE );
                roomsType.push_back( Room::R_TEMPLE );
                roomsType.push_back( Room::R_GRAVEYARD );
                roomsType.push_back( Room::R_SCAVENGER );
            }
            if ( graph.size() < 1 ) {
                /// create dungeon heart
                addRandomRoom( Room::R_DUNGEON_HEART, roomSize );
            }
            for (std::size_t i=1; i<roomsNum; ++i) {
                const std::size_t roomIndex = ( i - 1 ) % roomsType.size();
                const Room newRoomType = roomsType[roomIndex];
                /// randomize next room
                addRandomRoom( newRoomType, roomSize );
            }
        }

        Rect Dungeon::boundingBox() const {
            std::vector< const DungeonRoom* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            if ( rSize < 1 ) {
                return Rect();
            }
            Rect minMax = roomsList[0]->position();
            for (std::size_t i=1; i<rSize; ++i) {
                const DungeonRoom* room = roomsList[ i ];
                const Rect& roomPos = room->position();
                minMax.expand( roomPos );
            }
            return minMax;
        }

        static void addOutline( std::set< Point >& container, const Rect& rect ) {
            for ( int y = rect.min.y; y<= rect.max.y; ++y ) {
                container.insert( Point(rect.min.x, y) );
                container.insert( Point(rect.max.x, y) );
            }
            for ( int x = rect.min.x; x<= rect.max.x; ++x ) {
                container.insert( Point(x, rect.min.y) );
                container.insert( Point(x, rect.max.y) );
            }
        }

        static void addLine( std::set< Point >& container, const Point& from, const Point& to ) {
            const std::vector<Point> points = line( from, to );
            if ( points.empty() ) {
                return ;
            }
            for ( const Point& item: points ) {
                container.insert( item + Point( 2, 0) );
                container.insert( item + Point(-2, 0) );
                container.insert( item + Point( 0,  2) );
                container.insert( item + Point( 0, -2) );
            }
            {
                const Rect first( points.front(), 5, 5 );
                addOutline( container, first );
                const Rect last( points.back(), 5, 5 );
                addOutline( container, last );
            }
        }

        std::set< Point > Dungeon::outline() const {
            std::set< Point > ret;

            std::vector< const spatial::DungeonRoom* > roomsList = rooms();
            for ( const spatial::DungeonRoom* item: roomsList ) {
                /// room
                Rect position = item->position();
                position.grow( 2 );
                addOutline( ret, position );

                /// corridors
                const Point& itemCenter = item->position().center();
                std::vector< const spatial::DungeonRoom* > connectedList = connectedRooms( *item );
                for ( const spatial::DungeonRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->position().center();
                    addLine( ret, itemCenter, connectedCenter );
                }
            }

            return ret;
        }

        bool Dungeon::isCollision( const Rect& rect ) {
            std::vector< DungeonRoom* > roomsList = graph.itemsList();
            for ( const DungeonRoom* item: roomsList ) {
                const Rect& itemRect = item->position();
                if ( itemRect.isCollision( rect ) ) {
//                    LOG() << "collision detected, rectangles: " << itemRect << " " << rect;;
                    return true;
                }

                /// check existing corridors
                const Point itemCenter = itemRect.center();
                std::vector< DungeonRoom* > connected = connectedRooms( *item );
                for ( const DungeonRoom* next: connected ) {
                    const Point nextCenter = next->position().center();
                    const std::vector<Point> points = line( itemCenter, nextCenter );
                    const std::size_t pSize = points.size();
                    for ( std::size_t i=0; i<pSize; ++i ) {
                        const Rect pointRect( points[i], 1, 1 );
                        if ( pointRect.isCollision( rect ) ) {
//                            LOG() << "collision detected, rectangles: " << pointRect << " " << rect;;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool Dungeon::isCollision( const Point& start, const Point& end ) {
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

        std::string Dungeon::print() {
            std::stringstream stream;
            stream << "bbox: " << boundingBox() << "\n";
            std::vector< DungeonRoom* > roomsList = graph.itemsList();
            const std::size_t rSize = roomsList.size();
            for (std::size_t i=0; i<rSize; ++i) {
                const DungeonRoom* room = roomsList[ i ];
                const std::size_t currId = graph.itemId( *room );
                stream << currId << ": " << room->print() << "\n";
            }
            const std::vector<Direction>& directions = DirectionValues();
            for (std::size_t i=0; i<rSize; ++i) {
                const DungeonRoom* room = roomsList[ i ];
                if ( room == nullptr ) {
                    continue ;
                }
                const std::size_t currId = graph.itemId( *room );
                for ( const Direction dir: directions ) {
                    DungeonRoom* next = graph.getItem( *room, dir );
                    if ( next == nullptr ) {
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
