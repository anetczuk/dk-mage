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
        protected:

            utils::Rect roomPosition;

            adiktedpp::Player roomOwner;

            std::vector< Direction > restrictedDirs;                /// allowed directions


        public:

            bool allowedBranches;


            FortressRoom(): roomPosition(),
                roomOwner( adiktedpp::Player::P_P0 ),
                restrictedDirs(),
                allowedBranches( false )
            {
            }

            virtual ~FortressRoom() = default;

            virtual FortressRoomType type() const = 0;

            virtual void prepare( FortressDungeon& dungeon, const FortressRoom& from ) = 0;

            virtual void draw( const FortressDungeon& dungeon, adiktedpp::Level& level ) const = 0;

            std::size_t roomArea() const {
                return roomPosition.area();
            }

            utils::Point roomSize() const {
                return roomPosition.vector();
            }

            const utils::Rect& position() const {
                return roomPosition;
            }

            utils::Rect& position() {
                return roomPosition;
            }

            utils::Point edgePoint( const Direction direction, const std::size_t delta = 0 ) const;

            void resize( const std::size_t newSize ) {
                roomPosition = utils::Rect( newSize, newSize );
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
        std::unique_ptr< FortressRoom > spawn_object( const FortressRoomType roomType );

    } /* namespace spatial */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_SPATIAL_FORTRESSROOM_H_ */
