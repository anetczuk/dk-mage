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

            int northCoord;
            int eastCoord;


            Room(): roomPosition(), validType(false),
                roomType( adiktedpp::SlabType::ST_ROCK ),
                roomOwner( adiktedpp::PlayerType::PT_0 ),
                northCoord(0), eastCoord(0)
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

            DungeonGraph< Room, NoEdgeData > graph;
            adiktedpp::PlayerType player;
            bool fortifyWalls;


        public:

            std::size_t limitNorth;
            std::size_t limitSouth;
            std::size_t limitEast;
            std::size_t limitWest;


            Dungeon( const adiktedpp::PlayerType player=adiktedpp::PlayerType::PT_0, const bool fortify=true ): player(player), fortifyWalls( fortify ),
                    limitNorth( 999 ), limitSouth( 999 ), limitEast( 999 ), limitWest( 999 )
            {
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

            std::size_t roomsNum() const {
                return graph.size();
            }

            std::vector< const Room* > rooms() const {
                return graph.itemsList();
            }

            std::vector< Room* > rooms() {
                return graph.itemsList();
            }

            Room* room( const std::size_t index ) {
                return graph.itemByindex( index );
            }

            utils::Point roomCenter( const std::size_t index ) {
                Room* aRoom = room( index );
                if ( aRoom == nullptr ) {
                    return utils::Point();
                }
                return aRoom->position().center();
            }

            std::vector< const Room* > connectedRooms( const Room& room ) const {
                return graph.connectedItems( room );
            }

            std::vector< Room* > connectedRooms( const Room& room ) {
                return graph.connectedItems( room );
            }

            std::vector< Room* > findRoom( const adiktedpp::SlabType roomType );

            Room* findRoomFirst( const adiktedpp::SlabType roomType );

            void addRandomRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize, const bool addLink = true, const std::size_t distance = 1 );

            Room* addRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize );

            Room* addRoom( const adiktedpp::SlabType roomType, const std::size_t roomSize, const Room& from, const Direction direction, const bool addLink = true, const std::size_t distance = 1 );

            void generate( const std::size_t roomsNum, const std::size_t roomSize );

            /// size of dungeon
            Rect boundingBox();

            bool isCollision( const Rect& rect );

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

            void centerizeOn( const int x, const int y );

            /// method prevents moving to world edge
            void moveToTopEdge( const std::size_t distanceFromEdge );

            /// method prevents moving to world edge
            void moveToBottomEdge( const std::size_t distanceFromEdge );

            std::string print();

        };


    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_DUNGEON_H_ */
