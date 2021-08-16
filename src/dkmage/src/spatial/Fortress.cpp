/*
 * Fortress.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/DungeonGraph.h"

#include "adiktedpp/Level.h"

#include "utils/ProbabilityMass.h"


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
            stream << "position: " << position() << " type: " << fortressType;
            return stream.str();
        }


        /// ============================================================


        static void drawBoulderCorridor( adiktedpp::Level& level, const Player owner, const Point start, const int length, const Point along, const Point ortho ) {
            for ( int coord=0; coord<length; ++coord ) {
                if ( coord % 2 == 1 ) {
                    const Point center = start + along * coord;
                    level.setTrap( center + ortho, 4, Trap::T_BOULDER );
                    level.setFortified( center - ortho, owner );
                } else {
                    const Point center = start + along * coord;
                    level.setTrap( center - ortho, 4, Trap::T_BOULDER );
                    level.setFortified( center + ortho, owner );
                }
            }
        }

        void FortressDungeon::draw( adiktedpp::Level& level ) const {
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const FortressRoom* > roomsList = rooms();
            for ( const FortressRoom* item: roomsList ) {
                /// set room
                const Rect& position = item->position();
                const Room itemType  = item->roomType();
                level.setRoom( position, itemType, owner, fortify );

                /// draw corridors
                const Point& itemCenter = item->position().center();
                std::vector< const FortressRoom* > connectedList = connectedRooms( *item );
                for ( const FortressRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->position().center();
                    level.digLine( itemCenter, connectedCenter, owner, fortify );
                }

                /// draw content
                if ( item->type() == FortressRoomType::FR_BOULDER_CORRIDOR ) {
                    const Rect& roomRect = item->position();
                    const int width  = roomRect.width();
                    const int height = roomRect.height();
                    if ( width > height && height == 3 ) {
                        /// horizontal
                        const Point start = roomRect.leftCenter();
                        if ( rng_randb() ) {
                            drawBoulderCorridor( level, owner, start, width, Point(1, 0), Point(0, 1) );
                        } else {
                            drawBoulderCorridor( level, owner, start, width, Point(1, 0), Point(0, -1) );
                        }
                    } else if ( width < height && width == 3) {
                        /// vertical
                        const Point start = roomRect.centerTop();
                        if ( rng_randb() ) {
                            drawBoulderCorridor( level, owner, start, height, Point(0, 1), Point(1, 0) );
                        } else {
                            drawBoulderCorridor( level, owner, start, height, Point(0, 1), Point(-1, 0) );
                        }
                    } else {
                        LOG() << "unable to draw " << item->type();
                    }
                }

                if ( item->type() == FortressRoomType::FR_EXIT ) {
                    /// create branch exit
                    /// set entrance traps
                    const std::vector< Direction > directions = linkDirections( *item );
                    const Direction corridorDirection = directions[ 0 ];
                    /// LOG() << "corridor direction: " << corridorDirection << " " << directions.size();

                    const Point roomCenter = item->position().center();
                    const Point corridorStart = item->edgePoint( corridorDirection, 1 );
                    const Point boulderPosition = movePoint( corridorStart, corridorDirection, 1 );
                    const Point corridorEnd = movePoint( boulderPosition, corridorDirection, 1 );

                    drawTrap3x3Diamond( level, roomCenter, Trap::T_BOULDER );
                    drawTrap3x3Corners( level, roomCenter, Trap::T_LIGHTNING );
                    level.setDoor( corridorStart, Door::D_IRON, true );
                    level.setTrap( boulderPosition, Trap::T_BOULDER );
                    level.setDoor( corridorEnd, Door::D_IRON, true );
                }
            }
        }

        std::vector< const FortressRoom* > FortressDungeon::addRandomRoom( const FortressRoomType roomType, const FortressRoom& from, const bool allowBranches ) {
            static ProbabilityMass<std::size_t> roomSize357;
            if ( roomSize357.empty() ) {
                roomSize357.set( 3, 1.0 );
                roomSize357.set( 5, 1.0 );
                roomSize357.set( 7, 0.25 );
                roomSize357.normalize();
            }

            switch( roomType ) {
            case FortressRoomType::FR_DUNGEON_HEART: {
//                const std::size_t rSize = roomSize357.getRandom();
//                const FortressRoom* next = dungeon.addRandomRoom( roomType, rSize, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                return {next};
                return {nullptr};
            }
            case FortressRoomType::FR_TREASURE: {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;
                const std::size_t rSize = roomSize357.getRandom();
                FortressRoom* next = addRandomRoom( roomType, rSize, from, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                return {next};
            }
            case FortressRoomType::FR_CORRIDOR: {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;
                std::size_t rSizeX = roomSize357.getRandom();
                std::size_t rSizeY = 1;
                if ( rng_randb() ) {
                    std::swap( rSizeX, rSizeY );
                }
                FortressRoom* next = addRandomRoom( roomType, rSizeX, rSizeY, from, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                if ( allowBranches ) {
                    if ( rng_randb( 0.3 ) ) {
                        /// give a chance to generate branch
                        return {next, next};
                    }
                }
                return {next};
            }
            case FortressRoomType::FR_BRANCH: {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;
                const FortressRoom* next = addRandomRoom( roomType, 1, from, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                if ( allowBranches ) {
                    return {next, next};            /// yes, added twice to make a branch
                }
                return {next};
            }
            case FortressRoomType::FR_BOULDER_CORRIDOR: {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;
                const std::size_t roomLength     = rng_randi( 4 ) + 4;

                std::vector< Direction > availableDirs = from.restrictedDirections();
                if ( availableDirs.empty() ) {
                    availableDirs = freeDirections( from );
                }

                FortressRoom* next = nullptr;
                while ( availableDirs.empty() == false ) {
                    const std::size_t rDir = utils::rng_randi( availableDirs.size() );
                    const Direction newDir = remove_at( availableDirs, rDir );
                    switch( newDir ) {
                    case Direction::D_NORTH:
                    case Direction::D_SOUTH: {
                        next = addRoom( roomType, 3, roomLength, from, newDir, corridorLength );
                        break;
                    }
                    case Direction::D_EAST:
                    case Direction::D_WEST: {
                        next = addRoom( roomType, roomLength, 3, from, newDir, corridorLength );
                        break;
                    }
                    }
                    if ( next != nullptr ) {
                        next->setRestrictedDirection( newDir );
                        break;
                    }
                }

                if ( next == nullptr ) {
                    return {};
                }
                return {next};
            }
            case FortressRoomType::FR_EXIT: {
                const FortressRoom* next = addRandomRoom( roomType, 3, from, 4 );
                if ( next == nullptr ) {
                    return {};
                }
                return {next};
            }
            case FortressRoomType::FR_EMPTY: {
                const std::size_t corridorLength = rng_randi( 5 ) + 1;
                const std::size_t rSize = roomSize357.getRandom();
                const FortressRoom* next = addRandomRoom( roomType, rSize, from, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                return {next};
            }
            }

            LOG() << "unhandled fortress room: " << (int)roomType;
            return {};
        }

    } /* namespace spatial */
} /* namespace dkmage */
