/*
 * Dungeon.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_

#include "dkmage/spatial/Spatialtem.h"
#include "dkmage/spatial/DungeonGraph.h"

#include "adiktedpp/Type.h"
#include "adiktedpp/PlayerType.h"

#include "utils/Log.h"
#include "utils/Rect.h"

#include <map>


using utils::Point;
using utils::Rect;


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        class DungeonRoom {

            Rect roomPosition;

            bool validType;
            adiktedpp::Room roomType;
            adiktedpp::PlayerType roomOwner;


        public:

            int northCoord;
            int eastCoord;


            DungeonRoom(): roomPosition(), validType(false),
                roomType( adiktedpp::Room::R_TREASURE ),
                roomOwner( adiktedpp::PlayerType::PT_0 ),
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

            adiktedpp::PlayerType owner() const {
                return roomOwner;
            }

            void owner( const adiktedpp::PlayerType newType ) {
                roomOwner = newType;
            }

            std::string print() const;

        };

        Point movePoint( const Point& base, const Direction direction, const std::size_t space );

        void moveRect( Rect& rect, const Rect& base, const Direction direction, const std::size_t space );


        /// ==================================================================


        /**
         *
         */
        class Dungeon: public Spatialtem {

            DungeonGraph< DungeonRoom, NoEdgeData > graph;
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

            std::vector< const DungeonRoom* > rooms() const {
                return graph.itemsList();
            }

            std::vector< DungeonRoom* > rooms() {
                return graph.itemsList();
            }

            DungeonRoom* room( const std::size_t index ) {
                return graph.itemByindex( index );
            }

            utils::Point roomCenter( const std::size_t index ) {
                DungeonRoom* aRoom = room( index );
                if ( aRoom == nullptr ) {
                    return utils::Point();
                }
                return aRoom->position().center();
            }

            std::vector< const DungeonRoom* > connectedRooms( const DungeonRoom& room ) const {
                return graph.connectedItems( room );
            }

            std::vector< DungeonRoom* > connectedRooms( const DungeonRoom& room ) {
                return graph.connectedItems( room );
            }

            std::vector< Direction > availableDirections( const DungeonRoom& room ) const {
                return graph.availableDirections( room );
            }

            std::vector< DungeonRoom* > findRoom( const adiktedpp::Room roomType );

            DungeonRoom* findRoomFirst( const adiktedpp::Room roomType );

            DungeonRoom* addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const bool addLink = true, const std::size_t distance = 1 );

            DungeonRoom* addRandomRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const DungeonRoom& from, const bool addLink = true, const std::size_t corridorLength = 1 );

            DungeonRoom* addRoom( const adiktedpp::Room roomType, const std::size_t roomSize );

            DungeonRoom* addRoom( const adiktedpp::Room roomType, const std::size_t roomSize, const DungeonRoom& from, const Direction direction, const bool addLink = true, const std::size_t corridorLength = 1 );

            void generate( const std::size_t roomsNum, const std::size_t roomSize );

            bool isCollision( const Rect& rect );

            /// is collision with corridor
            bool isCollision( const Point& start, const Point& end );

            /// size of dungeon
            Rect boundingBox() const override;

            std::set< Point > outline() const;

            void move( const int offsetX, const int offsetY ) override {
                std::vector< DungeonRoom* > roomsList = graph.itemsList();
                for ( DungeonRoom* item: roomsList ) {
                    Rect& pos = item->position();
                    pos.move( offsetX, offsetY );
                }
            }

            std::string print();

        };


    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_DUNGEON_H_ */
