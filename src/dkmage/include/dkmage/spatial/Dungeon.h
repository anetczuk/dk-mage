/*
 * Dungeon.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_

#include "dkmage/spatial/Spatialtem.h"
#include "dkmage/spatial/DungeonGraph.h"

#include "adiktedpp/Type.h"

#include "utils/Container.h"
#include "utils/Log.h"
#include "utils/Rect.h"
#include "utils/Rand.h"

#include <map>


using utils::Point;
using utils::Rect;


namespace adiktedpp {
    class Level;
}


namespace dkmage {
    namespace spatial {

        inline void addOutline( std::set< Point >& container, const Rect& rect ) {
            utils::PointSet points = utils::outline( rect );
            container.insert( points.begin(), points.end() );
        }

        inline void addLine( std::set< Point >& container, const Point& from, const Point& to ) {
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

        Point movePoint( const Point& base, const Direction direction, const std::size_t space );

        /// move 'rect' to side of 'base' rect in given 'direction' with 'space' between them
        void moveRect( Rect& rect, const Rect& base, const Direction direction, const int space );


        /// ====================================================================


        /**
         *
         */
        template< class TRoom >
        class BaseDungeon: public Spatialtem {
        protected:

            DungeonGraph< TRoom, NoEdgeData > graph;
            adiktedpp::Player player;
            bool fortifyWalls;


        public:

            std::size_t limitNorth;
            std::size_t limitSouth;
            std::size_t limitEast;
            std::size_t limitWest;

            std::size_t limitWidth;
            std::size_t limitHeight;


            BaseDungeon( const adiktedpp::Player player=adiktedpp::Player::P_P0, const bool fortify=true ): player(player), fortifyWalls( fortify ),
                    limitNorth( 999 ), limitSouth( 999 ), limitEast( 999 ), limitWest( 999 ),
                    limitWidth( 255 ), limitHeight( 255 )
            {
            }

            std::size_t roomsNum() const {
                return graph.nodesNum();
            }

            std::size_t connectionsNum() const {
                /// unidirected graph has two arcs for each connection
                return graph.edgesNum() / 2;
            }

            adiktedpp::Player owner() const {
                return player;
            }
            void owner( const adiktedpp::Player owner ) {
                player = owner;
            }

            bool fortify() const {
                return fortifyWalls;
            }
            void fortify( const bool value ) {
                fortifyWalls = value;
            }

            std::vector< const TRoom* > rooms() const {
                return graph.itemsList();
            }

            std::vector< TRoom* > rooms() {
                return graph.itemsList();
            }

            TRoom* room( const std::size_t index ) {
                return graph.itemByindex( index );
            }

            bool exists( const TRoom& room ) const {
                std::vector< const TRoom* > items = graph.itemsList();
                auto pos = std::find( items.begin(), items.end(), &room );
                return ( pos != items.end() );
            }

            bool removeRoom( const TRoom& room ) {
                return graph.removeItem( room );
            }

            std::vector< std::pair<const TRoom*, const TRoom*> > connectedRooms() const {
                return graph.connectedItems();
            }

            std::vector< const TRoom* > connectedRooms( const TRoom& room ) const {
                return graph.connectedItems( room );
            }

            std::vector< TRoom* > connectedRooms( const TRoom& room ) {
                return graph.connectedItems( room );
            }

            std::vector< Direction > linkDirections( const TRoom& room ) const {
                return graph.linkDirections( room );
            }

            using Spatialtem::move;

            bool isRectInLimit( const Rect& rect ) const {
                Rect bbox = boundingBox();
                bbox.expand( rect );

                const int bw = bbox.width();
                if ( bw < 0 ) {
                    return false;
                }
                if ( (std::size_t)bw > limitWidth ) {
                    return false;
                }

                const int bh = bbox.height();
                if ( bh < 0 ) {
                    return false;
                }
                if ( (std::size_t)bh > limitHeight ) {
                    return false;
                }
                return true;
            }

        };


        /// ==================================================================


        /**
         *
         */
        class EvilRoom {

            Rect roomPosition;

            bool validType;
            adiktedpp::Room roomType;
            adiktedpp::Player roomOwner;


        public:

            int northCoord;
            int eastCoord;


            EvilRoom(): roomPosition(), validType(false),
                roomType( adiktedpp::Room::R_TREASURE ),
                roomOwner( adiktedpp::Player::P_P0 ),
                northCoord(0), eastCoord(0)
            {
            }

            std::size_t roomArea() const {
                return roomPosition.area();
            }

            Point roomSize() const {
                return roomPosition.vector();
            }

            const Rect& position() const {
                return roomPosition;
            }

            Rect& position() {
                return roomPosition;
            }

            Point edgePoint( const Direction direction, const std::size_t delta = 0 ) const;

            void resize( const std::size_t newSize ) {
                roomPosition = Rect( newSize, newSize );
            }

            adiktedpp::Room type() const {
                return roomType;
            }

            void type( const adiktedpp::Room newType ) {
                roomType = newType;
            }

            adiktedpp::Player owner() const {
                return roomOwner;
            }

            void owner( const adiktedpp::Player newType ) {
                roomOwner = newType;
            }

            std::string print() const;

        };


        /// ==================================================================


        /**
         *
         */
        class EvilDungeon: public BaseDungeon< EvilRoom > {
        public:

            using BaseDungeon::BaseDungeon;

            void draw( adiktedpp::Level& level ) const;

            utils::Point roomCenter( const std::size_t index ) {
                EvilRoom* aRoom = room( index );
                if ( aRoom == nullptr ) {
                    return utils::Point();
                }
                return aRoom->position().center();
            }

            std::vector< EvilRoom* > findRoom( const adiktedpp::Room roomType ) {
                std::vector< EvilRoom* > ret;
                std::vector< EvilRoom* > allRooms = graph.itemsList();
                for (  EvilRoom* item: allRooms ) {
                    if ( item == nullptr ) {
                        continue ;
                    }
                    if ( item->type() == roomType ) {
                        ret.push_back( item );
                    }
                }
                return ret;
            }

            EvilRoom* findRoomFirst( const adiktedpp::Room roomType ) {
                std::vector< EvilRoom* > rooms = findRoom( roomType );
                if ( rooms.empty() ) {
                    return nullptr;
                }
                return rooms[0];
            }

            EvilRoom* addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const bool addLink = true, const std::size_t distance = 1 ) {
                std::vector< EvilRoom* > roomsList = graph.itemsList();
                if ( roomsList.empty()) {
                    /// no rooms in dungeon
                    EvilRoom* root = graph.addItem();
                    root->resize( roomSize );
                    Rect& basePos = root->position();
                    basePos.centerize();
                    root->type( roomType );
                    root->owner( player );
                    return root;
                }

                while ( roomsList.empty() == false ) {
                    const std::size_t rRoom = utils::rng_randi( roomsList.size() );
                    EvilRoom* selected = utils::remove_at( roomsList, rRoom );
                    if ( selected == nullptr ) {
                        continue;
                    }

                    EvilRoom* added = addRandomRoom( roomType, roomSize, *selected, addLink, distance );
                    if ( added != nullptr ) {
                        return added;
                    }
                }

                LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

            EvilRoom* addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const EvilRoom& from, const bool addLink = true, const std::size_t corridorLength = 1 ) {
                return addRandomRoom( roomType, roomSize, roomSize, from, addLink, corridorLength );
            }

            EvilRoom* addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const EvilRoom& from, const bool addLink = true, const std::size_t corridorLength = 1 ) {
                std::vector< Direction > availableDirs = freeDirections( from );

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = utils::remove_at( availableDirs, rDir );
                    EvilRoom* added = addRoom( roomType, roomSizeX, roomSizeY, from, newDir, addLink, corridorLength );
                    if ( added != nullptr ) {
                        return added;
                    }
                }

    //            LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

            EvilRoom* addRoom( const adiktedpp::Room roomType, const std::size_t roomSize ) {
                Rect newRect( roomSize, roomSize );

                Rect collisionRect = newRect;
                collisionRect.grow( 1 );
                if ( isCollision( collisionRect ) ) {
                    /// collision detected
                    return nullptr;
                }

                EvilRoom* newRoom = graph.addItem();
                if ( newRoom == nullptr ) {
                    return nullptr;
                }

                /// new node added
                if ( graph.nodesNum() == 1 ) {
                    /// only room in dungeon -- centerize root
                    newRect.centerize();
                }
                newRoom->position() = newRect;
                newRoom->type( roomType );
                newRoom->owner( player );
                return newRoom;
            }

            EvilRoom* addRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const EvilRoom& from, const Direction direction, const bool addLink = true, const std::size_t corridorLength = 1 ) {
                return addRoom( roomType, roomSize, roomSize, from, direction, addLink, corridorLength );
            }

            EvilRoom* addRoom( const adiktedpp::Room roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const EvilRoom& from, const Direction direction, const bool addLink = true, const std::size_t corridorLength = 1 ) {
                Rect newRect( roomSizeX, roomSizeY );
                const Rect& basePos = from.position();
                moveRect( newRect, basePos, direction, corridorLength );

                if ( isRectInLimit( newRect ) == false ) {
                    return nullptr;
                }

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

                EvilRoom* newRoom = graph.addItem( from, direction, addLink );
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

            void generate( const std::size_t roomsNum, const std::size_t roomSize ) {
                std::vector< adiktedpp::Room > roomsType;
                {
                    roomsType.push_back( adiktedpp::Room::R_TREASURE );
                    roomsType.push_back( adiktedpp::Room::R_LAIR );
                    roomsType.push_back( adiktedpp::Room::R_HATCHERY );
                    roomsType.push_back( adiktedpp::Room::R_TRAINING );
                    roomsType.push_back( adiktedpp::Room::R_LIBRARY );
                    /// bridge
                    roomsType.push_back( adiktedpp::Room::R_WORKSHOP );
                    /// guard post
                    roomsType.push_back( adiktedpp::Room::R_PRISON );
                    roomsType.push_back( adiktedpp::Room::R_TORTURE );
                    roomsType.push_back( adiktedpp::Room::R_TEMPLE );
                    roomsType.push_back( adiktedpp::Room::R_GRAVEYARD );
                    roomsType.push_back( adiktedpp::Room::R_SCAVENGER );
                }
                if ( graph.nodesNum() < 1 ) {
                    /// create dungeon heart
                    addRandomRoom( adiktedpp::Room::R_DUNGEON_HEART, roomSize );
                }
                for (std::size_t i=1; i<roomsNum; ++i) {
                    const std::size_t roomIndex = ( i - 1 ) % roomsType.size();
                    const adiktedpp::Room newRoomType = roomsType[roomIndex];
                    /// randomize next room
                    addRandomRoom( newRoomType, roomSize );
                }
            }

            std::vector< Direction > freeDirections( const EvilRoom& room ) const {
                std::vector< Direction > availableDirs = graph.freeDirections( room );

                const int northDiff = limitNorth - room.northCoord;
                if ( northDiff <= 0) {
                    utils::remove( availableDirs, Direction::D_NORTH );
                }
                const int southDiff = limitSouth + room.northCoord;
                if ( southDiff <= 0) {
                    utils::remove( availableDirs, Direction::D_SOUTH );
                }
                const int eastDiff = limitEast - room.eastCoord;
                if ( eastDiff <= 0) {
                    utils::remove( availableDirs, Direction::D_EAST );
                }
                const int westDiff = limitWest + room.eastCoord;
                if ( westDiff == 0) {
                    utils::remove( availableDirs, Direction::D_WEST );
                }

                return availableDirs;
            }

            bool isCollision( const Rect& rect ) const {
                std::vector< const EvilRoom* > roomsList = graph.itemsList();
                for ( const EvilRoom* item: roomsList ) {
                    const Rect& itemRect = item->position();
                    if ( itemRect.isCollision( rect ) ) {
    //                    LOG() << "collision detected, rectangles: " << itemRect << " " << rect;;
                        return true;
                    }

                    /// check existing corridors
                    const Point itemCenter = itemRect.center();
                    std::vector< const EvilRoom* > connected = connectedRooms( *item );
                    for ( const EvilRoom* next: connected ) {
                        const Point nextCenter = next->position().center();
                        const std::vector<Point> points = line( itemCenter, nextCenter );
                        if ( is_collision( points, rect ) ) {
                            return true;
                        }
                    }
                }
                return false;
            }

            /// is collision with corridor
            bool isCollision( const Point& start, const Point& end ) {
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

            std::set< Point > outline() const {
                std::set< Point > ret;

                std::vector< const EvilRoom* > roomsList = rooms();
                for ( const EvilRoom* item: roomsList ) {
                    /// room
                    Rect position = item->position();
                    position.grow( 2 );
                    addOutline( ret, position );

                    /// corridors
                    const Point& itemCenter = item->position().center();
                    std::vector< const EvilRoom* > connectedList = connectedRooms( *item );
                    for ( const EvilRoom* connected: connectedList ) {
                        const Point& connectedCenter = connected->position().center();
                        addLine( ret, itemCenter, connectedCenter );
                    }
                }

                return ret;
            }

            /// size of dungeon
            Rect boundingBox() const override {
                std::vector< const EvilRoom* > roomsList = graph.itemsList();
                const std::size_t rSize = roomsList.size();
                if ( rSize < 1 ) {
                    return Rect();
                }
                const EvilRoom* firstRoom = roomsList[0];
                Rect minMax = firstRoom->position();
                for (std::size_t i=1; i<rSize; ++i) {
                    const EvilRoom* room = roomsList[ i ];
                    const Rect& roomPos = room->position();
                    minMax.expand( roomPos );
                }
                return minMax;
            }

            using BaseDungeon::move;

            void move( const int offsetX, const int offsetY ) override {
                std::vector< EvilRoom* > roomsList = graph.itemsList();
                for ( EvilRoom* item: roomsList ) {
                    Rect& pos = item->position();
                    pos.move( offsetX, offsetY );
                }
            }

            std::string print() {
                std::stringstream stream;
                stream << "bbox: " << boundingBox() << "\n";
                std::vector< EvilRoom* > roomsList = graph.itemsList();
                const std::size_t rSize = roomsList.size();
                for (std::size_t i=0; i<rSize; ++i) {
                    const EvilRoom* room = roomsList[ i ];
                    const std::size_t currId = graph.itemId( *room );
                    stream << currId << ": " << room->print() << "\n";
                }
                const std::vector<Direction>& directions = DirectionValues();
                for (std::size_t i=0; i<rSize; ++i) {
                    const EvilRoom* room = roomsList[ i ];
                    if ( room == nullptr ) {
                        continue ;
                    }
                    const std::size_t currId = graph.itemId( *room );
                    for ( const Direction dir: directions ) {
                        EvilRoom* next = graph.getItem( *room, dir );
                        if ( next == nullptr ) {
                            continue ;
                        }
                        const std::size_t nextId = graph.itemId( *next );
                        stream << currId << " -> " << dir << " " << nextId << "\n";
                    }
                }
                return stream.str();
            }

        };


    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_ */
