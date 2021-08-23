/*
 * FortressRoom.cpp
 *
 */

#include "dkmage/spatial/FortressRoom.h"

#include "dkmage/spatial/Fortress.h"
#include "dkmage/Draw.h"

#include "adiktedpp/GameMap.h"

#include "utils/ProbabilityMass.h"
#include "utils/Log.h"
#include "utils/Macro.h"

//#include <algorithm>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        std::ostream& operator<<( std::ostream& os, const FortressRoomType data ) {
            switch( data ) {
            ENUM_STREAM_CASE( FortressRoomType, FR_DUNGEON_HEART, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_EMPTY, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_TREASURE, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_CORRIDOR, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_BRANCH, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_BOULDER_CORRIDOR, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_PRISON, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_TORTURE, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_GRAVEYARD, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_LAVA_POST, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_SECRET_INCLVL, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_SECRET_RESURRECT, os );
            ENUM_STREAM_CASE( FortressRoomType, FR_EXIT, os );
            }
            os << "UNKNOWN_ROOM[" << (int) data << "]";
            return os;
        }


        /// =======================================================================================


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
                roomSize357.set( 7, 0.1 );
                roomSize357.normalize();
            }
            return roomSize357;
        }

        static PointList drawBoulderCorridor( adiktedpp::Level& level, const Player owner, const Point start, const int length, const Point along, const Point ortho ) {
            PointList ret;
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
                if ( orthoLen > 1 ) {
                    const Point doorPoint = center + orthoDir * multiplier;
                    level.setDoor( doorPoint, Door::D_WOOD, true );

                    const Point slotPoint = center + orthoDir * multiplier * orthoLen;
                    ret.push_back( slotPoint );
                } else {
                    const Point trapPoint = center + orthoDir * multiplier;
                    level.setTrap( trapPoint, 4, Trap::T_BOULDER );
                }
            }
            return ret;
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

                setPosition( 5, 5 );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_DUNGEON_HEART, roomOwner, true );
            }

        };


        /**
         *
         */
        class FortressRoomEmpty: public FortressRoom {
        public:

            std::size_t roomSize = 0;

            FortressRoomType type() const override {
                return FortressRoomType::FR_EMPTY;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                const ProbabilityMass<std::size_t>& roomSize357 = FortressRoomSizeProbability();

                roomSize = roomSize357.getRandom();
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                setPosition( roomSize, roomSize );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                std::size_t radius = 0;
                switch( roomSize ) {
                case 5: {
                    radius = 1;
                    break;
                }
                case 7: {
                    radius = 2;
                    break;
                }
                }

                if ( radius == 0) {
                    level.setFortified( roomRect.center(), roomOwner );
                } else {
                    level.setFortified( roomRect.center() + Point( 1, 1) * radius, roomOwner );
                    level.setFortified( roomRect.center() + Point(-1, 1) * radius, roomOwner );
                    level.setFortified( roomRect.center() + Point(-1,-1) * radius, roomOwner );
                    level.setFortified( roomRect.center() + Point( 1,-1) * radius, roomOwner );
                }
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

                setPosition( rSize, rSize );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_TREASURE, roomOwner, true );

                /// fill treasure with gold
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
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

                const ProbabilityMass<std::size_t>& roomSize357 = FortressRoomSizeProbability();

                const std::size_t roomLength = roomSize357.getRandom() - 1;
                const std::size_t corridorLength = 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    switch( newDir ) {
                    case Direction::D_NORTH:
                    case Direction::D_SOUTH: {
                        setPosition( 1, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Direction::D_EAST:
                    case Direction::D_WEST: {
                        setPosition( roomLength, 1 );
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

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );
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

                setPosition( 1, 1 );
                dungeon.addRandomRoom( *this, from, corridorLength );
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );
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
                        setPosition( 5, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Direction::D_EAST:
                    case Direction::D_WEST: {
                        setPosition( roomLength, 5 );
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

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                const int width  = roomRect.width();
                const int height = roomRect.height();
                PointList slots;
                if ( width > height ) {
                    /// horizontal
                    const int step = height / 2;
                    const Point start = roomRect.leftCenter();
                    if ( rng_randb() ) {
                        slots = drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0,  step) );
                    } else {
                        slots = drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0, -step) );
                    }
                } else if ( width < height ) {
                    /// vertical
                    const int step = width / 2;
                    const Point start = roomRect.centerTop();
                    if ( rng_randb() ) {
                        slots = drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point( step, 0) );
                    } else {
                        slots = drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point(-step, 0) );
                    }
                } else {
                    LOG() << "unable to draw " << type();
                }

                /// set traps
                for ( const Point item: slots ) {
                    level.setTrap( item, 4, Trap::T_BOULDER );
                }
            }

        };


        /**
         *
         */
        class FortressRoomPrison: public FortressRoom {
        public:

            Axis orientation = Axis::A_HORIZONTAL;


            FortressRoomType type() const override {
                return FortressRoomType::FR_PRISON;
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
                    orientation = get_axis( newDir );
                    switch( orientation ) {
                    case Axis::A_VERTICAL: {
                        setPosition( 5, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Axis::A_HORIZONTAL: {
                        setPosition( roomLength, 5 );
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

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                const int width  = roomRect.width();
                const int height = roomRect.height();
                PointList slots;
                switch( orientation ) {
                case Axis::A_VERTICAL: {
                    /// vertical
                    const int step = width / 2;
                    const Point start = roomRect.centerTop();
                    if ( rng_randb() ) {
                        slots = drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point( step, 0) );
                    } else {
                        slots = drawBoulderCorridor( level, roomOwner, start, height, Point(0, 1), Point(-step, 0) );
                    }
                    break ;
                }
                case Axis::A_HORIZONTAL: {
                    /// horizontal
                    const int step = height / 2;
                    const Point start = roomRect.leftCenter();
                    if ( rng_randb() ) {
                        slots = drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0,  step) );
                    } else {
                        slots = drawBoulderCorridor( level, roomOwner, start, width, Point(1, 0), Point(0, -step) );
                    }
                    break ;
                }
                }

                /// set traps
                {
                    const std::size_t sSize = slots.size();
                    const std::size_t items = std::min( (std::size_t)2, sSize );
                    for ( std::size_t i =0; i<items; ++i ) {
                        const std::size_t pIndex = rng_randi( slots.size() );
                        const Point point = remove_at( slots, pIndex );
                        level.setRoom( point, Room::R_PRISON, roomOwner, true );
                        level.setCreature( point, 1, Creature::C_SKELETON, 1, 9, Player::P_UNSET );
                    }
                }
                for ( const Point item: slots ) {
                    level.setTrap( item, 4, Trap::T_BOULDER );
                }
            }

        };


        /**
         *
         */
        class FortressRoomTorture: public FortressRoom {
        public:

            Axis orientation = Axis::A_HORIZONTAL;


            FortressRoomType type() const override {
                return FortressRoomType::FR_TORTURE;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    orientation = get_axis( newDir );
                    switch( orientation ) {
                    case Axis::A_VERTICAL: {
                        setPosition( 9, 9 );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Axis::A_HORIZONTAL: {
                        setPosition( 9, 9 );
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

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                const Rect chamber( roomRect.center(), 3, 3 );

                static std::set< Player > PlayerSet = { Player::P_UNSET, Player::P_GOOD };
                Player cOwner = Player::P_UNSET;

                const Rect r1 = chamber.moved( -3, -3 );
                level.setRoom( r1, Room::R_TORTURE, roomOwner, true );
                level.setFortified( r1, roomOwner );
                cOwner = rng_rand( PlayerSet );
                level.setCreature( r1.center(), 1, Creature::C_MISTRESS, 2, 9, cOwner );

                const Rect r2 = chamber.moved(  3, -3 );
                level.setRoom( r2, Room::R_TORTURE, roomOwner, true );
                level.setFortified( r2, roomOwner );
                cOwner = rng_rand( PlayerSet );
                level.setCreature( r2.center(), 1, Creature::C_MISTRESS, 2, 9, cOwner );

                const Rect r3 = chamber.moved(  3,  3 );
                level.setRoom( r3, Room::R_TORTURE, roomOwner, true );
                level.setFortified( r3, roomOwner );
                cOwner = rng_rand( PlayerSet );
                level.setCreature( r3.center(), 1, Creature::C_MISTRESS, 2, 9, cOwner );

                const Rect r4 = chamber.moved( -3,  3 );
                level.setRoom( r4, Room::R_TORTURE, roomOwner, true );
                level.setFortified( r4, roomOwner );
                cOwner = rng_rand( PlayerSet );
                level.setCreature( r4.center(), 1, Creature::C_MISTRESS, 2, 9, cOwner );

                switch( orientation ) {
                case Axis::A_VERTICAL: {
                    /// vertical
                    const Point door1 = roomRect.center() + Point( -1, 0 );
                    level.setDoor( door1, Door::D_IRON, true );
                    const Point door1a1 = door1 + Point( -2,  1 );
                    level.setDoor( door1a1, Door::D_IRON, true );
                    const Point door1a2 = door1 + Point( -2, -1 );
                    level.setDoor( door1a2, Door::D_IRON, true );


                    const Point door2 = roomRect.center() + Point(  1, 0 );
                    level.setDoor( door2, Door::D_IRON, true );
                    const Point door2a1 = door2 + Point(  2,  1 );
                    level.setDoor( door2a1, Door::D_IRON, true );
                    const Point door2a2 = door2 + Point(  2, -1 );
                    level.setDoor( door2a2, Door::D_IRON, true );
                    break ;
                }
                case Axis::A_HORIZONTAL: {
                    /// horizontal
                    const Point door1 = roomRect.center() + Point( 0, -1 );
                    level.setDoor( door1, Door::D_IRON, true );
                    const Point door1a1 = door1 + Point( -1, -2 );
                    level.setDoor( door1a1, Door::D_IRON, true );
                    const Point door1a2 = door1 + Point(  1, -2 );
                    level.setDoor( door1a2, Door::D_IRON, true );


                    const Point door2 = roomRect.center() + Point(  0, 1 );
                    level.setDoor( door2, Door::D_IRON, true );
                    const Point door2a1 = door2 + Point( -1, 2 );
                    level.setDoor( door2a1, Door::D_IRON, true );
                    const Point door2a2 = door2 + Point(  1, 2 );
                    level.setDoor( door2a2, Door::D_IRON, true );
                }
                }
            }

        };


        /**
         *
         */
        class FortressRoomGraveyard: public FortressRoom {
        public:

            Axis orientation    = Axis::A_HORIZONTAL;
            bool alternativePos = false;


            FortressRoomType type() const override {
                return FortressRoomType::FR_GRAVEYARD;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                const Rect baseRect( 3, 3 );
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    orientation = get_axis( newDir );
                    alternativePos = rng_randb();
                    switch( orientation ) {
                    case Axis::A_VERTICAL: {
                        Rect newRect = baseRect;
                        newRect.growWidth( 1 );
                        setPosition( newRect );
                        if ( alternativePos ) {
                            setJointPoint( -2, 0 );
                        } else {
                            setJointPoint(  2, 0 );
                        }
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added == false ) {
                            break ;
                        }
                        restrictedDirs.push_back( newDir );
                        if ( alternativePos ) {
                            restrictedDirs.push_back( Direction::D_WEST );
                        } else {
                            restrictedDirs.push_back( Direction::D_EAST );
                        }
                        return;
                    }
                    case Axis::A_HORIZONTAL: {
                        Rect newRect = baseRect;
                        newRect.growHeight( 1 );
                        setPosition( newRect );
                        if ( alternativePos ) {
                            setJointPoint( 0, -2 );
                        } else {
                            setJointPoint( 0,  2 );
                        }
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added == false ) {
                            break ;
                        }
                        restrictedDirs.push_back( newDir );
                        if ( alternativePos ) {
                            restrictedDirs.push_back( Direction::D_NORTH );
                        } else {
                            restrictedDirs.push_back( Direction::D_SOUTH );
                        }
                        return;
                    }
                    }
                }
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();

                Rect room( roomRect.center(), 3, 3 );
                Point doorPoint = room.center();

                switch( orientation ) {
                case Axis::A_VERTICAL: {
                    if ( alternativePos ) {
                        room.move( 1, 0 );
                        doorPoint += Point( -1, 0 );
                    } else {
                        room.move( -1, 0 );
                        doorPoint += Point( 1, 0 );
                    }
                    break;
                }
                case Axis::A_HORIZONTAL: {
                    if ( alternativePos ) {
                        room.move( 0, 1 );
                        doorPoint += Point( 0, -1 );
                    } else {
                        room.move( 0, -1 );
                        doorPoint += Point( 0, 1 );
                    }
                    break;
                }
                }

                level.setRoom( room, Room::R_GRAVEYARD, roomOwner, true );
                level.setDoor( doorPoint, Door::D_WOOD, true );
            }

        };


        /**
         *
         */
        class FortressRoomLavaPost: public FortressRoom {
        public:

            Axis orientation = Axis::A_HORIZONTAL;


            FortressRoomType type() const override {
                return FortressRoomType::FR_LAVA_POST;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                const std::size_t roomLength     = rng_randi( 3 ) * 2 + 5;
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    orientation = get_axis( newDir );
                    switch( orientation ) {
                    case Axis::A_VERTICAL: {
                        setPosition( 7, roomLength );
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added ) {
                            setRestrictedDirection( newDir );
                            return;
                        }
                        break ;
                    }
                    case Axis::A_HORIZONTAL: {
                        setPosition( roomLength, 7 );
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

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                Point start;
                Point along;
                int length = 0;

                switch( orientation ) {
                case Axis::A_VERTICAL: {
                    /// vertical
                    start = roomRect.centerTop();
                    along = Point( 0, 1 );
                    length = roomRect.height();
                    break ;
                }
                case Axis::A_HORIZONTAL: {
                    /// horizontal
                    start = roomRect.leftCenter();
                    along = Point( 1, 0 );
                    length = roomRect.width();
                    break ;
                }
                }

                const Point ortho = along.swapped();
                const Point orthoDir = ortho.dir();
                for ( int coord=0; coord<length; ++coord ) {
                    const Point center = start + along * coord;
                    level.setSlab( center + orthoDir, Slab::S_LAVA );
                    level.setSlab( center - orthoDir, Slab::S_LAVA );
                    if ( coord % 2 == 1 ) {
                        for ( int i=2; i<=3; ++i ) {
                            level.setFortified( center - orthoDir * i, roomOwner );
                            level.setFortified( center + orthoDir * i, roomOwner );
                        }
                        continue ;
                    }

                    level.setDoor( center - orthoDir * 2, Door::D_WOOD );
                    level.setDoor( center + orthoDir * 2, Door::D_WOOD );

                    static std::set< Creature > CreatureSet = { Creature::C_ARCHER, Creature::C_MONK, Creature::C_WIZARD };
                    const Creature cOwner1 = rng_rand( CreatureSet );
                    level.setCreature( center - orthoDir * 3, 1, cOwner1, 1, 9, roomOwner );
                    const Creature cOwner2 = rng_rand( CreatureSet );
                    level.setCreature( center + orthoDir * 3, 1, cOwner2, 1, 9, roomOwner );
                }
            }
        };


        /**
         *
         */
        class FortressRoomSecret: public FortressRoom {
        public:

            Axis orientation    = Axis::A_HORIZONTAL;
            bool alternativePos = false;


            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = dungeon.freeDirections( from );
                    if ( availableDirs.empty() ) {
                        return ;
                    }
                }

                const Rect baseRect( 3, 3 );
                const std::size_t corridorLength = rng_randi( 5 ) + 1;

                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    orientation = get_axis( newDir );
                    alternativePos = rng_randb();
                    switch( orientation ) {
                    case Axis::A_VERTICAL: {
                        Rect newRect = baseRect;
                        newRect.growWidth( 1 );
                        setPosition( newRect );
                        if ( alternativePos ) {
                            setJointPoint( -2, 0 );
                        } else {
                            setJointPoint(  2, 0 );
                        }
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added == false ) {
                            break ;
                        }
                        restrictedDirs.push_back( newDir );
                        if ( alternativePos ) {
                            restrictedDirs.push_back( Direction::D_WEST );
                        } else {
                            restrictedDirs.push_back( Direction::D_EAST );
                        }
                        return;
                    }
                    case Axis::A_HORIZONTAL: {
                        Rect newRect = baseRect;
                        newRect.growHeight( 1 );
                        setPosition( newRect );
                        if ( alternativePos ) {
                            setJointPoint( 0, -2 );
                        } else {
                            setJointPoint( 0,  2 );
                        }
                        const bool added = dungeon.createRoom( *this, from, newDir, corridorLength );
                        if ( added == false ) {
                            break ;
                        }
                        restrictedDirs.push_back( newDir );
                        if ( alternativePos ) {
                            restrictedDirs.push_back( Direction::D_NORTH );
                        } else {
                            restrictedDirs.push_back( Direction::D_SOUTH );
                        }
                        return;
                    }
                    }
                }
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                const Rect& roomRect = position();
                Rect room( roomRect.center(), 3, 3 );

                switch( orientation ) {
                case Axis::A_VERTICAL: {
                    if ( alternativePos ) {
                        room.move( 1, 0 );
                    } else {
                        room.move( -1, 0 );
                    }
                    break;
                }
                case Axis::A_HORIZONTAL: {
                    if ( alternativePos ) {
                        room.move( 0, 1 );
                    } else {
                        room.move( 0, -1 );
                    }
                    break;
                }
                }

                level.setRoom( room, Room::R_CLAIMED, roomOwner, true );
                level.setSlab( room, Slab::S_EARTH );
                drawSpecial( level, room );
            }

            virtual void drawSpecial( adiktedpp::Level& level, const Rect& room ) const = 0;

        };


        /**
         *
         */
        class FortressRoomSecretIncLvl: public FortressRoomSecret {
        public:
            FortressRoomType type() const override {
                return FortressRoomType::FR_SECRET_INCLVL;
            }
            virtual void drawSpecial( adiktedpp::Level& level, const Rect& room ) const override {
                const Point center = room.center();
                level.setSlab( center, Slab::S_PATH );
                level.setItem( center, 4, Item::I_SPECIAL_INCLEV );
            }
        };


        /**
         *
         */
        class FortressRoomSecretResurrect: public FortressRoomSecret {
        public:
            FortressRoomType type() const override {
                return FortressRoomType::FR_SECRET_RESURRECT;
            }
            virtual void drawSpecial( adiktedpp::Level& level, const Rect& room ) const override {
                const Point center = room.center();
                level.setSlab( center, Slab::S_PATH );
                level.setItem( center, 4, Item::I_SPECIAL_RESURCT );
            }
        };


        /**
         *
         */
        class FortressRoomExit: public FortressRoom {
        public:

            Direction exitDirection = Direction::D_EAST;


            FortressRoomType type() const override {
                return FortressRoomType::FR_EXIT;
            }

            void prepare( FortressDungeon& dungeon, const FortressRoom& from ) override {
                setPosition( 3, 3 );
                const bool added = dungeon.addRandomRoom( *this, from, 3 );
                if ( added == false ) {
                    return ;
                }
                const std::vector< Direction > directions = dungeon.linkDirections( *this );
                exitDirection = directions[ 0 ];
                /// LOG() << "exitDirection: " << exitDirection << " " << directions.size();
            }

            void draw( adiktedpp::GameMap& gameMap ) const override {
                adiktedpp::Level& level = gameMap.level;
                /// create branch exit
                const Rect& roomRect = position();
                level.setRoom( roomRect, Room::R_CLAIMED, roomOwner, true );

                /// set entrance traps

                const Point roomCenter = position().center();
                const Point corridorStart = edgePoint( exitDirection, 1 );
                const Point boulderPosition = movePoint( corridorStart, exitDirection, 1 );
                const Point corridorEnd = movePoint( boulderPosition, exitDirection, 1 );

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
        std::unique_ptr< FortressRoom > spawn_object( const FortressRoomType roomType ) {
            switch( roomType ) {
            case FortressRoomType::FR_DUNGEON_HEART:        return std::make_unique< FortressRoomDungeonHeart >();
            case FortressRoomType::FR_EMPTY:                return std::make_unique< FortressRoomEmpty >();
            case FortressRoomType::FR_TREASURE:             return std::make_unique< FortressRoomTreasure >();
            case FortressRoomType::FR_CORRIDOR:             return std::make_unique< FortressRoomCorridor >();
            case FortressRoomType::FR_BRANCH:               return std::make_unique< FortressRoomBranch >();
            case FortressRoomType::FR_BOULDER_CORRIDOR:     return std::make_unique< FortressRoomBoulderCorridor >();
            case FortressRoomType::FR_PRISON:               return std::make_unique< FortressRoomPrison >();
            case FortressRoomType::FR_TORTURE:              return std::make_unique< FortressRoomTorture >();
            case FortressRoomType::FR_GRAVEYARD:            return std::make_unique< FortressRoomGraveyard >();
            case FortressRoomType::FR_LAVA_POST:            return std::make_unique< FortressRoomLavaPost >();
            case FortressRoomType::FR_SECRET_INCLVL:        return std::make_unique< FortressRoomSecretIncLvl >();
            case FortressRoomType::FR_SECRET_RESURRECT:     return std::make_unique< FortressRoomSecretResurrect >();
            case FortressRoomType::FR_EXIT:                 return std::make_unique< FortressRoomExit >();
            }

            LOG() << "unhandled case: " << roomType;
            return nullptr;
        }


    } /* namespace spatial */
} /* namespace dkmage */
