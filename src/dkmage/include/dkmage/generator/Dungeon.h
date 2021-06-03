/*
 * Dungeon.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_

#include "dkmage/generator/DungeonGraph.h"

#include "utils/Log.h"
#include "utils/Rect.h"

#include <map>


using utils::Point;
using utils::Rect;


namespace dkmage {
    namespace generator {

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
