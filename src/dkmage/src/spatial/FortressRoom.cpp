/*
 * FortressRoom.cpp
 *
 */

#include "dkmage/spatial/FortressRoom.h"

#include "dkmage/spatial/Fortress.h"
#include "dkmage/Draw.h"

#include "adiktedpp/Level.h"

#include "utils/ProbabilityMass.h"
#include "utils/Log.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        Point FortressRoom::edgePoint( const Direction direction, const std::size_t delta ) const {
            switch( direction ) {
            case Direction::D_NORTH: return roomPosition.centerTop( delta );
            case Direction::D_SOUTH: return roomPosition.centerBottom( delta );
            case Direction::D_WEST:  return roomPosition.leftCenter( delta );
            case Direction::D_EAST:  return roomPosition.rightCenter( delta );
            }
            LOG() << "invalid case";
            return roomPosition.center();
        }

        Room FortressRoom::roomType() const {
            const FortressRoomType fortressType = type();

            switch( fortressType ) {
            case FortressRoomType::FR_DUNGEON_HEART:    return Room::R_DUNGEON_HEART;
            case FortressRoomType::FR_TREASURE:         return Room::R_TREASURE;
            case FortressRoomType::FR_CORRIDOR:         return Room::R_CLAIMED;
            case FortressRoomType::FR_BRANCH:           return Room::R_CLAIMED;
            case FortressRoomType::FR_BOULDER_CORRIDOR: return Room::R_CLAIMED;
            case FortressRoomType::FR_EXIT:             return Room::R_CLAIMED;
            case FortressRoomType::FR_EMPTY:            return Room::R_CLAIMED;
            }

            LOG() << "unhandled fortress room: " << (int)fortressType;
            return Room::R_CLAIMED;
        }

        std::string FortressRoom::print() const {
            std::stringstream stream;
            stream << "position: " << position() << " type: " << type();
            return stream.str();
        }


        /// ============================================================


        static const ProbabilityMass<std::size_t>& FortressRoomSizeProbability() {
            static ProbabilityMass<std::size_t> roomSize357;
            if ( roomSize357.empty() ) {
                roomSize357.set( 3, 1.0 );
                roomSize357.set( 5, 1.0 );
                roomSize357.set( 7, 0.25 );
                roomSize357.normalize();
            }
            return roomSize357;
        }

        static void drawBoulderCorridor( adiktedpp::Level& level, const Player owner, const Point start, const int length, const Point along, const Point ortho ) {
            const std::size_t orthoLen = ortho.length();
            const Point orthoDir = ortho.dir();
            for ( int coord=0; coord<length; ++coord ) {
                const Point center = start + along * coord;
                int multiplier = 1;
                if ( coord % 2 == 1 ) {
                    multiplier = -1;
                }
                for ( std::size_t i=1; i<=orthoLen; ++i ) {
                    level.setFortified( center - orthoDir * i * multiplier, owner );
                }
                level.setTrap( center + orthoDir * multiplier, 4, Trap::T_BOULDER );
            }
        }


        /// ============================================================


        /**
         *
         */
        class FortressRoomDungeonHeart: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_DUNGEON_HEART;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                roomPosition = Rect( 5, 5 );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );
            }

        };


        /**
         *
         */
        class FortressRoomTreasure: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_TREASURE;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                const ProbabilityMass<std::size_t>& roomSize357 = FortressRoomSizeProbability();

                const std::size_t rSize = roomSize357.getRandom();
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                roomPosition = Rect( rSize, rSize );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );
            }

        };


        /**
         *
         */
        class FortressRoomCorridor: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_CORRIDOR;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                allowedBranches = true;

                const ProbabilityMass<std::size_t>& roomSize357 = FortressRoomSizeProbability();

                const std::size_t roomLength = roomSize357.getRandom() - 1;
                const std::size_t corridorLength = 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    switch( newDir ) {
                    case Direction::D_NORTH:
                    case Direction::D_SOUTH: {
                        roomPosition = Rect( 1, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Direction::D_EAST:
                    case Direction::D_WEST: {
                        roomPosition = Rect( roomLength, 1 );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    }
                }
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );
            }

        };


        /**
         *
         */
        class FortressRoomBranch: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_BRANCH;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                allowedBranches = true;
                roomPosition = Rect( 1, 1 );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );
            }

        };


        /**
         *
         */
        class FortressRoomBoulderCorridor: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_BOULDER_CORRIDOR;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                const std::size_t roomLength     = rng_randi( 3 ) + 6;
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    switch( newDir ) {
                    case Direction::D_NORTH:
                    case Direction::D_SOUTH: {
                        roomPosition = Rect( 5, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Direction::D_EAST:
                    case Direction::D_WEST: {
                        roomPosition = Rect( roomLength, 5 );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    }
                }
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );

                const int width  = roomRect.width();
                const int height = roomRect.height();
                if ( width > height ) {
                    /// horizontal
                    const int step = height / 2;
                    const Point start = roomRect.leftCenter();
                    if ( rng_randb() ) {
                        drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0,  step) );
                    } else {
                        drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0, -step) );
                    }
                } else if ( width < height ) {
                    /// vertical
                    const int step = width / 2;
                    const Point start = roomRect.centerTop();
                    if ( rng_randb() ) {
                        drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point( step, 0) );
                    } else {
                        drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point(-step, 0) );
                    }
                } else {
                    LOG() << "unable to draw " << type();
                }
            }

        };


        /**
         *
         */
        class FortressRoomExit: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_EXIT;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                roomPosition = Rect( 3, 3 );
                dungeon.addRandomRoom( *this, from, 3 );
            }

            void draw( const FortressDungeon& dungeon, adiktedpp::Level& level ) const override {
                /// create branch exit
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );

                /// set entrance traps
                const std::vector< Direction > directions = dungeon.linkDirections( *this );
                const Direction corridorDirection = directions[ 0 ];
                /// LOG() << "corridor direction: " << corridorDirection << " " << directions.size();

                const Point roomCenter = position().center();
                const Point corridorStart = edgePoint( corridorDirection, 1 );
                const Point boulderPosition = movePoint( corridorStart, corridorDirection, 1 );
                const Point corridorEnd = movePoint( boulderPosition, corridorDirection, 1 );

                drawTrap3x3Diamond( level, roomCenter, Trap::T_BOULDER );
                drawTrap3x3Corners( level, roomCenter, Trap::T_LIGHTNING );
                level.setDoor( corridorStart, Door::D_IRON, true );
                level.setTrap( boulderPosition, Trap::T_BOULDER );
                level.setDoor( corridorEnd, Door::D_IRON, true );
            }

        };


        /**
         *
         */
        class FortressRoomEmpty: public FortressRoom {
        public:

            FortressRoomType type() const override {
                return FortressRoomType::FR_EMPTY;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                const ProbabilityMass<std::size_t>& roomSize357 = FortressRoomSizeProbability();

                const std::size_t rSize = roomSize357.getRandom();
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                roomPosition = Rect( rSize, rSize );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( const FortressDungeon& /*dungeon*/, adiktedpp::Level& level ) const override {
                const Rect& roomRect = position();
                const Room itemType  = roomType();
                level.setRoom( roomRect, itemType, roomOwner, true );
            }

        };


        /**
         *
         */
        std::unique_ptr< FortressRoom > spawn_object( const FortressRoomType roomType ) {
            switch( roomType ) {
            case FortressRoomType::FR_DUNGEON_HEART:        return std::make_unique< FortressRoomDungeonHeart >();
            case FortressRoomType::FR_TREASURE:             return std::make_unique< FortressRoomTreasure >();
            case FortressRoomType::FR_CORRIDOR:             return std::make_unique< FortressRoomCorridor >();
            case FortressRoomType::FR_BRANCH:               return std::make_unique< FortressRoomBranch >();
            case FortressRoomType::FR_BOULDER_CORRIDOR:     return std::make_unique< FortressRoomBoulderCorridor >();
            case FortressRoomType::FR_EXIT:                 return std::make_unique< FortressRoomExit >();
            case FortressRoomType::FR_EMPTY:                return std::make_unique< FortressRoomEmpty >();
            }

            LOG() << "unhandled case: " << roomType;
            return nullptr;
        }


    } /* namespace spatial */
} /* namespace dkmage */
