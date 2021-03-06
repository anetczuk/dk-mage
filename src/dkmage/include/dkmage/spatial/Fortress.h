/*
 * Fortress.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_

#include "dkmage/spatial/Dungeon.h"
#include "dkmage/spatial/FortressRoom.h"
#include "dkmage/Parameter.h"

#include "utils/ProbabilityMass.h"


namespace dkmage {
    namespace spatial {

        /**
         *
         */
        class FortressDungeon: public BaseDungeon< FortressRoom > {
        public:

            using BaseDungeon::BaseDungeon;

            void draw( FortressData& data ) const;

            std::size_t countRooms( const FortressRoomType roomType ) const {
                std::vector< const FortressRoom* > rooms = findRoom( roomType );
                return rooms.size();
            }

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
                Rect minMax = firstRoom->bbox();
                for (std::size_t i=1; i<rSize; ++i) {
                    const FortressRoom* room = roomsList[ i ];
                    const Rect& roomPos = room->bbox();
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

            bool isCollision( const Rect& rect ) const;

            /// is collision with corridor
            bool isCollisionLine( const Point& start, const Point& end ) const;

            std::set< Point > outline() const {
                std::set< Point > ret;

                std::vector< const FortressRoom* > roomsList = rooms();
                for ( const FortressRoom* item: roomsList ) {
                    /// room
                    Rect position = item->bbox();
                    position.grow( 2 );
                    addOutline( ret, position );

                    /// corridors
                    const Point& itemCenter = item->bbox().center();
                    std::vector< const FortressRoom* > connectedList = connectedRooms( *item );
                    for ( const FortressRoom* connected: connectedList ) {
                        const Point& connectedCenter = connected->bbox().center();
                        addLine( ret, itemCenter, connectedCenter );
                    }
                }

                return ret;
            }

            std::vector< const FortressRoom* > findRoom( const FortressRoomType roomType ) const {
                std::vector< const FortressRoom* > ret;
                std::vector< const FortressRoom* > allRooms = graph.itemsList();
                for ( const FortressRoom* item: allRooms ) {
                    if ( item == nullptr ) {
                        continue ;
                    }
                    if ( item->type() == roomType ) {
                        ret.push_back( item );
                    }
                }
                return ret;
            }

            std::vector< FortressRoom* > findRoom( const FortressRoomType roomType ) {
                std::vector< FortressRoom* > ret;
                std::vector< FortressRoom* > allRooms = graph.itemsList();
                for ( FortressRoom* item: allRooms ) {
                    if ( item == nullptr ) {
                        continue ;
                    }
                    if ( item->type() == roomType ) {
                        ret.push_back( item );
                    }
                }
                return ret;
            }

            FortressRoom* setFirstRoom( const FortressRoomType roomType );

            FortressRoom* addRandomRoom( const FortressRoomType roomType, const FortressRoom& from );

            bool addRandomRoom( FortressRoom& newRoom, const FortressRoom& from, const std::size_t corridorLength = 1 );

            bool createRoom( FortressRoom& newRoom, const FortressRoom& from, const Direction direction, const std::size_t corridorLength = 1 );

            const utils::PointList getCorridor( const FortressRoom& from, const FortressRoom& to ) const;

            void calculateDistances();

            std::size_t maxDistance() const;

            std::size_t closestDistance( const FortressRoomType roomType ) const;

            std::string print() const;


        protected:

            bool canAdd( const Rect& roomRect, const FortressRoom& from, const Direction direction ) const;

        };


        /// =========================================================================================


        /**
         *
         */
        class Fortress {
        public:

            adiktedpp::GameMap& gameMap;
            ParametersMap& parameters;
            spatial::FortressDungeon fortress;

            utils::ProbabilityMass< spatial::FortressRoomType > roomProbability;


            Fortress( adiktedpp::GameMap& gameMap, ParametersMap& parameters );

            bool generate();

            bool generateTest();

            std::vector< const spatial::FortressRoom* > prepareCorridors( const std::vector< const spatial::FortressRoom* >& startRooms, const std::size_t roomsNum, const bool allowBranches );

            void prepareExitRooms( const std::vector< const spatial::FortressRoom* >& startRooms );

            void prepareSecondaryPass();

            void cutBlindCorridors();

            void cutInvalidExits();

            void ensureExits();

            bool generateLake( const utils::Rect& lakeLimit );

    //            std::vector< const spatial::FortressRoom* > prepareRoom( const spatial::FortressRoomType roomType, const spatial::FortressRoom* startItem, const bool allowBranches );

            bool prepareBridges( const std::vector< spatial::FortressRoom* >& exitRooms );

            void prepareCorridorTraps();

            /// find shortest bridge in four directions
            utils::PointList findBridge( const utils::Point startPoint );

            utils::PointList findBridge( const utils::Point startPoint, const utils::Point bridgeDirection );

            bool isBridgePossible( const utils::Point startPoint ) const;

            bool findFortified( const utils::Point startPoint, const utils::Point bridgeDirection ) const;


        protected:

            const spatial::FortressRoom* addMainJunction( const spatial::FortressRoom& room );

            const spatial::FortressRoom* cutBlindCorridor( const spatial::FortressRoom& room );

        };

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESS_H_ */
