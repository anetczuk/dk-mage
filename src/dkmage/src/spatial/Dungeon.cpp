/*
 * Dungeon.cpp
 *
 */

#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/Level.h"
#include "adiktedpp/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace spatial {

        Point movePoint( const Direction direction, const int space ) {
            return movePoint( Point(0,0), direction, space );
        }

        Point movePoint( const Point& base, const Direction direction, const int space ) {
            switch( direction ) {
            case Direction::D_NORTH: {
                return base + Point( 0, -space );
            }
            case Direction::D_SOUTH: {
                return base + Point( 0,  space );
            }
            case Direction::D_EAST: {
                return base + Point(  space, 0 );
            }
            case Direction::D_WEST: {
                return base + Point( -space, 0 );
            }
            }

            LOG_INFO() << "unhandled case: " << direction;
            return base;
        }

        void moveRect( Rect& rect, const Rect& base, const Direction direction, const int space ) {
            rect.centerize();
            const Point baseCenter = base.center();
            rect.move( baseCenter );

            switch( direction ) {
            case Direction::D_NORTH: {
                const int sum = rect.height() + base.height();
                const int yOffset = sum / 2 + space + sum % 2;
                rect.move( 0, -yOffset );
                break;
            }
            case Direction::D_SOUTH: {
                const int sum = rect.height() + base.height();
                const int yOffset = sum / 2 + space + sum % 2;
                rect.move( 0, yOffset );
                break;
            }
            case Direction::D_EAST: {
                const int sum = rect.width() + base.width();
                const int xOffset = sum / 2 + space + sum % 2;
                rect.move( xOffset, 0 );
                break;
            }
            case Direction::D_WEST: {
                const int sum = rect.width() + base.width();
                const int xOffset = sum / 2 + space + sum % 2;
                rect.move( -xOffset, 0 );
                break;
            }
            }
        }


        /// ============================================================


        Point EvilRoom::edgePoint( const Direction direction, const std::size_t delta ) const {
            switch( direction ) {
            case Direction::D_NORTH: return roomPosition.centerTop( delta );
            case Direction::D_SOUTH: return roomPosition.centerBottom( delta );
            case Direction::D_WEST:  return roomPosition.leftCenter( delta );
            case Direction::D_EAST:  return roomPosition.rightCenter( delta );
            }
            LOG_INFO() << "invalid case";
            return roomPosition.center();
        }

        std::string EvilRoom::print() const {
            std::stringstream stream;
            stream << "position: " << position() << " type: " << roomType;
            return stream.str();
        }


        /// ============================================================


        void EvilDungeon::draw( adiktedpp::Level& level ) const {
            const Player owner = this->owner();
            const bool fortify = this->fortify();
            std::vector< const spatial::EvilRoom* > roomsList = rooms();
            for ( const spatial::EvilRoom* item: roomsList ) {
                /// set room
                const Rect& position = item->position();
                const Room itemType = item->type();
                level.setRoom( position, itemType, owner, fortify );

                /// draw corridors
                const Point& itemCenter = item->position().center();
                std::vector< const spatial::EvilRoom* > connectedList = connectedRooms( *item );
                for ( const spatial::EvilRoom* connected: connectedList ) {
                    const Point& connectedCenter = connected->position().center();
                    level.digLine( itemCenter, connectedCenter, owner, fortify );
                }
            }
        }

    } /* namespace spatial */
} /* namespace dkmage */
