/*
 * Draw.cpp
 *
 */

#include "dkmage/Draw.h"

#include "dkmage/generator/Dungeon.h"
#include "dkmage/generator/Maze.h"


namespace dkmage {

    void drawDungeon( adiktedpp::Level& level, dkmage::generator::Dungeon& dungeon ) {
        const adiktedpp::PlayerType owner = dungeon.owner();
        const bool fortify = dungeon.fortify();
        std::vector< dkmage::generator::Room* > roomsList = dungeon.rooms();
        for ( const dkmage::generator::Room* item: roomsList ) {
            /// set room
            const Rect& position = item->position();
            const adiktedpp::SlabType itemType = item->type();
            level.setRoom( position, itemType, owner, fortify );

            /// draw corridors
            const Point& itemCenter = item->position().center();
            std::vector< dkmage::generator::Room* > connectedList = dungeon.connectedRooms( *item );
            for ( const dkmage::generator::Room* connected: connectedList ) {
                const Point& connectedCenter = connected->position().center();
                level.digLine( itemCenter, connectedCenter, owner, fortify );
            }
        }
    }

    void drawMaze( adiktedpp::Level& level, dkmage::generator::Maze& maze ) {
        const std::size_t xDimm = maze.dimmensionX();
        const std::size_t yDimm = maze.dimmensionY();

        const utils::Rect mapRect = level.mapRect();
        const utils::Point start = mapRect.center() - utils::Point( xDimm / 2 + 1, yDimm / 2 + 1 );

        const utils::Point end = start + utils::Point( xDimm + 1, yDimm + 1 );
        level.setSlabOutline( utils::Rect( start, end ), adiktedpp::SlabType::ST_ROCK );

        const utils::Point corner = start + utils::Point( 1, 1 );
        for ( std::size_t y=0; y<yDimm; ++y ) {
            for ( std::size_t x=0; x<xDimm; ++x ) {
                const bool val = maze.state( x, y );
                if ( val == false ) {
                    /// closed
                    level.setSlab( corner + utils::Point( x, y ), adiktedpp::SlabType::ST_ROCK );
                }
    //            else {
    //                /// open
    //                level.setSlab( corner + utils::Point( x, y ), adiktedpp::SlabType::ST_EARTH );
    //            }
            }
        }
    }

} /* namespace dkmage */
