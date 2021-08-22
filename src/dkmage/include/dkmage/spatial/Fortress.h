/*
 * Fortress.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_

#include "dkmage/spatial/Dungeon.h"
#include "dkmage/spatial/FortressRoom.h"


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        class FortressDungeon: public BaseDungeon< FortressRoom > {
        public:

            using BaseDungeon::BaseDungeon;

            void draw( adiktedpp::Level& level ) const;

            std::vector< Direction > freeDirections( const FortressRoom& room ) const {
                std::vector< Direction > availableDirs = room.restrictedDirections();
                if ( availableDirs.empty() == false ) {
                    /// return restricted directions
                    return availableDirs;
                }
                return graph.freeDirections( room );
            }

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
                    item->move( offsetX, offsetY );
                }
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

            FortressRoom* setFirstRoom( const FortressRoomType roomType, const std::size_t roomSize );

            FortressRoom* addRandomRoom( const FortressRoomType roomType, const FortressRoom& from );

            bool addRandomRoom( FortressRoom& newRoom, const FortressRoom& from, const std::size_t corridorLength = 1 );

            bool createRoom( FortressRoom& newRoom, const FortressRoom& from, const Direction direction, const std::size_t corridorLength = 1 );

            std::string print() const;


        protected:

            bool canAdd( const Rect& roomRect, const FortressRoom& from, const Direction direction ) const;

            bool isCollision( const Rect& rect ) const;

            /// is collision with corridor
            bool isCollision( const Point& start, const Point& end ) const {
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
