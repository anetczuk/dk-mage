/*
 * Dungeon.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_

#include "dkmage/generator/DungeonGraph.h"

#include "utils/Log.h"

#include <map>


namespace dkmage {
    namespace generator {

        struct Point {
            int x;
            int y;
            Point(): x(0), y(0) {
            }
            Point( const int ax, const int ay ): x(ax), y(ay) {
            }
        };

        struct Rect {
            Point min;
            Point max;

            /// empty rect
            Rect(): min(0,0), max(-1,-1) {
            }

            Rect( const std::size_t aWidth, const std::size_t aHeight ): min(), max(aWidth - 1, aHeight - 1) {
            }

            Rect( const Point& aMin, const Point& aMax ): min(aMin), max(aMax) {
            }

            int width() const {
                return ( max.x - min.x + 1);
            }
            int height() const {
                return ( max.y - min.y + 1);
            }
            int area() const {
                return width() * height();
            }
            Point vector() const {
                return Point( width() -1, height() -1 );
            }

            Point center() const {
                const int cx = min.x + width() / 2;
                const int cy = min.y + height() / 2;
                return Point( cx, cy );
            }

            void move( const int offsetX, const int offsetY ) {
                min.x += offsetX;
                min.y += offsetY;
                max.x += offsetX;
                max.y += offsetY;
            }

            void move( const Point offset ) {
                move( offset.x, offset.y );
            }

            void centerize() {
                const int diffX = width()  / 2;
                const int diffY = height() / 2;
                move( -diffX, -diffY );
            }

            void normalize() {
                const int w = width();
                const int h = height();
                *this = Rect( w, h );
            }

            void expand( const Rect& rect ) {
                min.x = std::min( min.x, rect.min.x );
                min.y = std::min( min.y, rect.min.y );
                max.x = std::max( max.x, rect.max.x );
                max.y = std::max( max.y, rect.max.y );
            }

            /// ================================================

            static Rect empty() {
                Rect rect;
                rect.max.x = -1;
                rect.max.y = -1;
                return rect;
            }

        };

        inline std::ostream& operator<<( std::ostream& os, const Rect& data ) {
            os << "{[" << data.min.x << " " << data.min.y << "] [" << data.max.x << " " << data.max.y << "]}";
            return os;
        }


        /// =================================================


        /**
         *
         */
        class Room {

            Rect roomPosition;


        public:

            Room(): roomPosition() {
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

            void resize( const std::size_t newSize ) {
                roomPosition = Rect( newSize, newSize );
            }

            std::string print() const;

        };


        /// ==================================================================


        class Dungeon {

            DungeonGraph<Room> graph;


        public:

            Dungeon() {
            }

            void generate( const std::size_t roomsNum, const std::size_t roomSize );

            /// size of dungeon
            Rect boundingBox();

            void move( const int offsetX, const int offsetY ) {
                std::vector< Room* > roomsList = graph.itemsList();
                for ( Room* item: roomsList ) {
                    Rect& pos = item->position();
                    pos.move( offsetX, offsetY );
                }
            }

            void move( const Point offset ) {
                move( offset.x, offset.y );
            }

            void centerize();

            std::string print();

        };


    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_ */
