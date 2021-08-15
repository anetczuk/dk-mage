/*
 * Fortress.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

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
            case Direction::D_WEST:  return roomPosition.centerLeft( delta );
            case Direction::D_EAST:  return roomPosition.centerRight( delta );
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


        void FortressDungeon::draw( adiktedpp::Level& level ) const {
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const spatial::FortressRoom* > roomsList = rooms();
            for ( const spatial::FortressRoom* item: roomsList ) {
                /// set room
                const Rect& position = item->position();
                const Room itemType  = item->roomType();
                level.setRoom( position, itemType, owner, fortify );

                /// draw corridors
                const Point& itemCenter = item->position().center();
                std::vector< const spatial::FortressRoom* > connectedList = connectedRooms( *item );
                for ( const spatial::FortressRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->position().center();
                    level.digLine( itemCenter, connectedCenter, owner, fortify );
                }
            }
        }

        std::vector< const spatial::FortressRoom* > FortressDungeon::addRandomRoom( const FortressRoomType roomType, const FortressRoom& from, const bool allowBranches ) {
            static ProbabilityMass<std::size_t> roomSize357;
            if ( roomSize357.empty() ) {
                roomSize357.set( 3, 1.0 );
                roomSize357.set( 5, 1.0 );
                roomSize357.set( 7, 0.25 );
                roomSize357.normalize();
            }

            const std::size_t corridorLength = rng_randi( 5 ) + 1;

            switch( roomType ) {
            case spatial::FortressRoomType::FR_DUNGEON_HEART: {
//                const std::size_t rSize = roomSize357.getRandom();
//                const spatial::FortressRoom* next = dungeon.addRandomRoom( roomType, rSize, *startItem, true, corridorLength );
//                if ( next == nullptr ) {
//                    return {};
//                }
//                return {next};
                return {nullptr};
            }
            case spatial::FortressRoomType::FR_TREASURE: {
                const std::size_t rSize = roomSize357.getRandom();
                spatial::FortressRoom* next = addRandomRoom( roomType, rSize, from, true, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                return {next};
            }
            case spatial::FortressRoomType::FR_CORRIDOR: {
                std::size_t rSizeX = roomSize357.getRandom();
                std::size_t rSizeY = 1;
                if ( rng_randb() ) {
                    std::swap( rSizeX, rSizeY );
                }
                spatial::FortressRoom* next = addRandomRoom( roomType, rSizeX, rSizeY, from, true, corridorLength );
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
            case spatial::FortressRoomType::FR_BRANCH: {
                const spatial::FortressRoom* next = addRandomRoom( roomType, 1, from, true, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                if ( allowBranches ) {
                    return {next, next};            /// yes, added twice to make a branch
                }
                return {next};
            }
            case spatial::FortressRoomType::FR_BOULDER_CORRIDOR: {
                const std::vector< spatial::Direction > availableDirs = freeDirections( from );

                const spatial::FortressRoom* next = addRandomRoom( roomType, 1, from, true, corridorLength );
                if ( next == nullptr ) {
                    return {};
                }
                if ( allowBranches ) {
                    return {next, next};            /// yes, added twice to make a branch
                }
                return {next};
            }
            case spatial::FortressRoomType::FR_EMPTY: {
                const std::size_t rSize = roomSize357.getRandom();
                const spatial::FortressRoom* next = addRandomRoom( roomType, rSize, from, true, corridorLength );
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
