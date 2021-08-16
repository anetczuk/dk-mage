/*
 * Fortress.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_

#include "dkmage/spatial/Dungeon.h"


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        enum class FortressRoomType {
            FR_DUNGEON_HEART,
            FR_TREASURE,
            FR_CORRIDOR,
            FR_BRANCH,
            FR_BOULDER_CORRIDOR,
            FR_EXIT,
            FR_EMPTY
        };

        inline std::ostream& operator<<( std::ostream& os, const FortressRoomType data ) {
            switch( data ) {
            case FortressRoomType::FR_DUNGEON_HEART:        { os << "FR_DUNGEON_HEART"; return os; }
            case FortressRoomType::FR_TREASURE:             { os << "FR_TREASURE"; return os; }
            case FortressRoomType::FR_CORRIDOR:             { os << "FR_CORRIDOR"; return os; }
            case FortressRoomType::FR_BRANCH:               { os << "FR_BRANCH"; return os; }
            case FortressRoomType::FR_BOULDER_CORRIDOR:     { os << "FR_BOULDER_CORRIDOR"; return os; }
            case FortressRoomType::FR_EXIT:                 { os << "FR_EXIT"; return os; }
            case FortressRoomType::FR_EMPTY:                { os << "FR_EMPTY"; return os; }
            }
            os << "UNKNOWN_ROOM[" << (int) data << "]";
            return os;
        }


        /// ======================================================================


        /**
         *
         */
        class FortressRoom {

            Rect roomPosition;

            FortressRoomType fortressType;
            adiktedpp::Player roomOwner;

            std::vector< Direction > restrictedDirs;


        public:

            int northCoord;
            int eastCoord;


            FortressRoom(): roomPosition(),
                fortressType( FortressRoomType::FR_EMPTY ),
                roomOwner( adiktedpp::Player::P_P0 ),
                restrictedDirs(),
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

            FortressRoomType type() const {
                return fortressType;
            }

            void type( FortressRoomType newType ) {
                fortressType = newType;
            }

            adiktedpp::Room roomType() const;

            adiktedpp::Player owner() const {
                return roomOwner;
            }

            void owner( const adiktedpp::Player newType ) {
                roomOwner = newType;
            }

            const std::vector< Direction >& restrictedDirections() const {
                return restrictedDirs;
            }

            void setRestrictedDirection( const Direction direction ) {
                restrictedDirs.clear();
                restrictedDirs.push_back( direction );
            }

            std::string print() const;

        };


        /**
         *
         */
        class FortressDungeon: public BaseDungeon< FortressRoom > {
        public:

            using BaseDungeon::BaseDungeon;

            void draw( adiktedpp::Level& level ) const;

            /// size of dungeon
            Rect boundingBox() const override {
                std::vector< const FortressRoom* > roomsList = graph.itemsList();
                const std::size_t rSize = roomsList.size();
                if ( rSize < 1 ) {
                    return Rect();
                }
                const FortressRoom* firstRoom = roomsList[0];
                Rect minMax = firstRoom->position();
                for (std::size_t i=1; i<rSize; ++i) {
                    const FortressRoom* room = roomsList[ i ];
                    const Rect& roomPos = room->position();
                    minMax.expand( roomPos );
                }
                return minMax;
            }

            void move( const int offsetX, const int offsetY ) override {
                std::vector< FortressRoom* > roomsList = graph.itemsList();
                for ( FortressRoom* item: roomsList ) {
                    Rect& pos = item->position();
                    pos.move( offsetX, offsetY );
                }
            }

            utils::Point roomCenter( const std::size_t index ) {
                FortressRoom* aRoom = room( index );
                if ( aRoom == nullptr ) {
                    return utils::Point();
                }
                return aRoom->position().center();
            }

            std::set< Point > outline() const {
                std::set< Point > ret;

                std::vector< const FortressRoom* > roomsList = rooms();
                for ( const FortressRoom* item: roomsList ) {
                    /// room
                    Rect position = item->position();
                    position.grow( 2 );
                    addOutline( ret, position );

                    /// corridors
                    const Point& itemCenter = item->position().center();
                    std::vector< const FortressRoom* > connectedList = connectedRooms( *item );
                    for ( const FortressRoom* connected: connectedList ) {
                        const Point& connectedCenter = connected->position().center();
                        addLine( ret, itemCenter, connectedCenter );
                    }
                }

                return ret;
            }

            std::vector< FortressRoom* > findRoom( const FortressRoomType roomType ) {
                std::vector< FortressRoom* > ret;
                std::vector< FortressRoom* > allRooms = graph.itemsList();
                for (  FortressRoom* item: allRooms ) {
                    if ( item == nullptr ) {
                        continue ;
                    }
                    if ( item->type() == roomType ) {
                        ret.push_back( item );
                    }
                }
                return ret;
            }

            std::vector< const spatial::FortressRoom* > addRandomRoom( const FortressRoomType roomType, const FortressRoom& from, const bool allowBranches = true );

            FortressRoom* addRandomRoom( const FortressRoomType roomType, const std::size_t roomSize, const std::size_t distance = 1 ) {
                std::vector< FortressRoom* > roomsList = graph.itemsList();
                if ( roomsList.empty()) {
                    /// no rooms in dungeon
                    FortressRoom* root = graph.addItem();
                    root->resize( roomSize );
                    Rect& basePos = root->position();
                    basePos.centerize();
                    root->type( roomType );
                    root->owner( player );
                    return root;
                }

                while ( roomsList.empty() == false ) {
                    const std::size_t rRoom = utils::rng_randi( roomsList.size() );
                    FortressRoom* selected = utils::remove_at( roomsList, rRoom );
                    if ( selected == nullptr ) {
                        continue;
                    }

                    FortressRoom* added = addRandomRoom( roomType, roomSize, *selected, distance );
                    if ( added != nullptr ) {
                        return added;
                    }
                }

                LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

            FortressRoom* addRoom( const FortressRoomType roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const FortressRoom& from, const Direction direction, const std::size_t corridorLength = 1 ) {                Rect newRect( roomSizeX, roomSizeY );
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

                FortressRoom* newRoom = graph.addItem( from, direction, true );
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


        protected:

            FortressRoom* addRandomRoom( const FortressRoomType roomType, const std::size_t roomSize, const FortressRoom& from, const std::size_t corridorLength = 1 ) {
                return addRandomRoom( roomType, roomSize, roomSize, from, corridorLength );
            }

            FortressRoom* addRandomRoom( FortressRoomType roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const FortressRoom& from, const std::size_t corridorLength = 1 ) {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = freeDirections( from );
                }
//                else {
//                    LOG() << "yyyyyyyyyyyyy: " << from.type() << " " << roomType << " " << availableDirs.size() << " " << availableDirs[0];
//                }
                return addRandomRoom( roomType, roomSizeX, roomSizeY, from, availableDirs, corridorLength );
            }

            FortressRoom* addRandomRoom( FortressRoomType roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const FortressRoom& from, const std::vector< Direction >& availableDirs, const std::size_t corridorLength = 1 ) {
                std::vector< Direction > available = availableDirs;
                while ( available.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( available.size() );
                    const Direction newDir = utils::remove_at( available, rDir );
                    FortressRoom* added = addRoom( roomType, roomSizeX, roomSizeY, from, newDir, corridorLength );
                    if ( added != nullptr ) {
                        return added;
                    }
                }

    //            LOG() << "unable to add room: " << (int)roomType;
                return nullptr;
            }

//            FortressRoom* addRoom( FortressRoomType roomType, const std::size_t roomSize ) {
//                Rect newRect( roomSize, roomSize );
//
//                Rect collisionRect = newRect;
//                collisionRect.grow( 1 );
//                if ( isCollision( collisionRect ) ) {
//                    /// collision detected
//                    return nullptr;
//                }
//
//                FortressRoom* newRoom = graph.addItem();
//                if ( newRoom == nullptr ) {
//                    return nullptr;
//                }
//
//                /// new node added
//                if ( graph.size() == 1 ) {
//                    /// only room in dungeon -- centerize root
//                    newRect.centerize();
//                }
//                newRoom->position() = newRect;
//                newRoom->type( roomType );
//                newRoom->owner( player );
//                return newRoom;
//            }

//            FortressRoom* addRoom( const FortressRoomType roomType, const std::size_t roomSize, const FortressRoom& from, const Direction direction, const bool addLink = true, const std::size_t corridorLength = 1 ) {
//                return addRoom( roomType, roomSize, roomSize, from, direction, addLink, corridorLength );
//            }

            bool isCollision( const Rect& rect ) const;

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

        };

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_ */
