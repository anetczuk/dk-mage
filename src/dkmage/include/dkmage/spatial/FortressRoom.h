/*
 * FortressRoom.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESSROOM_H_
#define DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESSROOM_H_

#include "dkmage/spatial/DungeonGraph.h"

#include "adiktedpp/Type.h"

#include "utils/Rect.h"


namespace adiktedpp {
    class GameMap;
    class Level;

    namespace script {
        class Script;
    }
}


namespace dkmage {

    class ParametersMap;


    namespace spatial {

        struct FortressData {
            adiktedpp::GameMap& gameMap;
            ParametersMap& parameters;

            adiktedpp::Level& level();

            adiktedpp::script::Script& script();
        };


        class FortressDungeon;


        /**
         *
         */
        enum class FortressRoomType {
            FR_DUNGEON_HEART,
            FR_EMPTY,
            FR_TRAP,
            FR_TREASURE,
            FR_CORRIDOR,
            FR_BRANCH,
            FR_BOULDER_CORRIDOR,
            FR_PRISON,
            FR_TORTURE,
            FR_GRAVEYARD,
            FR_LAVA_POST,
            FR_SECRET_INCLVL,
            FR_SECRET_RESURRECT,
            FR_EXIT
        };

        std::ostream& operator<<( std::ostream& os, const FortressRoomType data );


        /// ======================================================================


        /**
         *
         */
        class FortressRoom {

            utils::Rect roomPosition;
            utils::Point corridorJoinPoint;


        protected:

            FortressDungeon& dungeon;

            adiktedpp::Player roomOwner;

            std::vector< Direction > restrictedDirs;                /// allowed directions


        public:

            int distanceToHeart;


            FortressRoom( FortressDungeon& dungeon ): roomPosition(), corridorJoinPoint(),
                dungeon(dungeon), roomOwner( adiktedpp::Player::P_P0 ),
                restrictedDirs(), distanceToHeart( -1 )
            {
            }

            virtual ~FortressRoom() = default;

            virtual FortressRoomType type() const = 0;

            /// is point part of room's corridor?
            virtual bool isCorridor( const utils::Point point ) const = 0;

            /// prepare room position and internal state
            virtual void prepare( const FortressRoom& from ) = 0;

            /// put room to 'level'
            virtual void draw( FortressData& gameData ) const = 0;

            /// bounding box of room
            const utils::Rect& bbox() const {
                return roomPosition;
            }

            utils::Point joinPoint() const {
                return corridorJoinPoint;
            }

            utils::Point joinCenterOffset() const {
                return corridorJoinPoint - roomPosition.center();

            }

            utils::Point edgePoint( const Direction direction, const std::size_t delta = 0 ) const;

            void move( const utils::Point offset ) {
                move( offset.x, offset.y );
            }

            void move( const int offsetX, const int offsetY ) {
                roomPosition.move( offsetX, offsetY );
                corridorJoinPoint += utils::Point( offsetX, offsetY );
            }

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


        protected:

            void setBbox( const utils::Rect& rect ) {
                roomPosition = rect;
                corridorJoinPoint = roomPosition.center();
            }

            void setBbox( const std::size_t width, const std::size_t height ) {
                roomPosition = utils::Rect( width, height );
                corridorJoinPoint = roomPosition.center();
            }

            void setJointPoint( const int offsetX, const int offsetY ) {
                corridorJoinPoint = roomPosition.center() + utils::Point( offsetX, offsetY );
            }
            void setJointPoint( const utils::Point newPoint ) {
                corridorJoinPoint = roomPosition.center() + newPoint;
            }

        };


        /**
         *
         */
        std::unique_ptr< FortressRoom > spawn_object( FortressDungeon& dungeon, const FortressRoomType roomType );

        std::unique_ptr< FortressRoom > spawn_empty( FortressDungeon& dungeon );

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESSROOM_H_ */
