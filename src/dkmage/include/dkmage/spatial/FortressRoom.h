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
    class Level;
}


namespace dkmage {
    namespace spatial {


        class FortressDungeon;


        /**
         *
         */
        enum class FortressRoomType {
            FR_DUNGEON_HEART,
            FR_EMPTY,
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

            adiktedpp::Player roomOwner;

            std::vector< Direction > restrictedDirs;                /// allowed directions


        public:

            FortressRoom(): roomPosition(), corridorJoinPoint(),
                roomOwner( adiktedpp::Player::P_P0 ),
                restrictedDirs()
            {
            }

            virtual ~FortressRoom() = default;

            virtual FortressRoomType type() const = 0;

            /// prepare room position and internal state
            virtual void prepare( FortressDungeon& dungeon, const FortressRoom& from ) = 0;

            /// put room to 'level'
            virtual void draw( adiktedpp::Level& level ) const = 0;

            std::size_t roomArea() const {
                return roomPosition.area();
            }

            utils::Point roomSize() const {
                return roomPosition.vector();
            }

            const utils::Rect& position() const {
                return roomPosition;
            }

            void setPosition( const utils::Rect& rect ) {
                roomPosition = rect;
                corridorJoinPoint = roomPosition.center();
            }

            void setPosition( const std::size_t width, const std::size_t height ) {
                roomPosition = utils::Rect( width, height );
                corridorJoinPoint = roomPosition.center();
            }

            const utils::Point joinPoint() const {
                return corridorJoinPoint;
            }

            void setJointPoint( const int offsetX, const int offsetY ) {
                corridorJoinPoint = roomPosition.center() + utils::Point( offsetX, offsetY );
            }
            void setJointPoint( const utils::Point newPoint ) {
                corridorJoinPoint = roomPosition.center() + newPoint;
            }

            utils::Point joinCenterOffset() const {
                return corridorJoinPoint - roomPosition.center();

            }

            utils::Point edgePoint( const Direction direction, const std::size_t delta = 0 ) const;

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

            void move( const utils::Point offset ) {
                move( offset.x, offset.y );
            }

            void move( const int offsetX, const int offsetY ) {
                roomPosition.move( offsetX, offsetY );
                corridorJoinPoint += utils::Point( offsetX, offsetY );
            }

            std::string print() const;

        };


        /**
         *
         */
        std::unique_ptr< FortressRoom > spawn_object( const FortressRoomType roomType );

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESSROOM_H_ */
