/*
 * Dungeon.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_

#include "dkmage/generator/DungeonGraph.h"

#include "adiktedpp/SlabType.h"
#include "adiktedpp/PlayerType.h"

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

            bool validType;
            adiktedpp::SlabType roomType;
            adiktedpp::PlayerType roomOwner;


        public:

            Room(): roomPosition(), validType(false),
                roomType( adiktedpp::SlabType::ST_ROCK ),
                roomOwner( adiktedpp::PlayerType::PT_0 )
            {
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

            adiktedpp::SlabType type() const {
                return roomType;
            }

            void type( const adiktedpp::SlabType newType ) {
                roomType = newType;
            }

            adiktedpp::PlayerType owner() const {
                return roomOwner;
            }

            void owner( const adiktedpp::PlayerType newType ) {
                roomOwner = newType;
            }

            std::string print() const;

        };


        /// ==================================================================


        /**
         *
         */
        class Dungeon {

            DungeonGraph<Room> graph;
            adiktedpp::PlayerType player;
            bool fortifyWalls;


        public:

            Dungeon( const adiktedpp::PlayerType player=adiktedpp::PlayerType::PT_0, const bool fortify=true ): player(player), fortifyWalls( fortify ) {
            }

            adiktedpp::PlayerType owner() const {
                return player;
            }

            bool fortify() const {
                return fortifyWalls;
            }
            void fortify( const bool value ) {
                fortifyWalls = value;
            }

            std::vector< Room* > rooms() {
                return graph.itemsList();
            }

            Room* room( const std::size_t index ) {
                return graph.itemByindex( index );
            }

            std::vector< Room* > connectedRooms( const Room& room ) {
                return graph.connectedItems( room );
            }

            std::vector< Room* > findRoom( const adiktedpp::SlabType roomType );

            void addRandomRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize );

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

            void centerize( const int x, const int y );

            std::string print();

        };


    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_ */
