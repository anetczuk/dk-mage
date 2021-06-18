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
        const utils::Point start = mapRect.center() - utils::Point( xDimm / 2, yDimm / 2 );

        for ( std::size_t y=0; y<yDimm; ++y ) {
            for ( std::size_t x=0; x<xDimm; ++x ) {
                const bool val = maze.state( x, y );
                if ( val == false ) {
                    /// closed
                    level.setSlab( start + utils::Point( x, y ), adiktedpp::SlabType::ST_ROCK );
                }
                else {
                    /// open
                    level.setSlab( start + utils::Point( x, y ), adiktedpp::SlabType::ST_EARTH );
                }
            }
        }
    }

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces ) {
        const Point veinCenter = veinRect.center();

        int veinDir = 1;
        const utils::Rect mapSize = adiktedpp::Level::mapSize();
        if ( veinCenter.y > mapSize.center().y / 2 ) {
            /// on south
            veinDir = 1;
        } else {
            /// on north
            veinDir = -1;
        }

        const std::size_t veinSize = veinRect.area() * 0.7;
        switch( gemFaces ) {
        case 0: {
            level.setVein( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            break;
        }
        case 1: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, adiktedpp::SlabType::ST_ROCK );
            level.setVein( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );
            level.setSlab( gemCenter.x, gemCenter.y - veinDir * 1, adiktedpp::SlabType::ST_GOLD );          /// ensure available face
            break;
        }
        case 3: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );                              /// 3 faces
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, adiktedpp::SlabType::ST_GOLD );
            level.setVein( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );
            level.setSlab( gemCenter.x - 1, gemCenter.y + veinDir * 1,
                           gemCenter.x + 1, gemCenter.y + veinDir * 1, adiktedpp::SlabType::ST_ROCK );
            break;
        }
        default: {
            LOG() << "unhandled gem faces size: " << gemFaces;
        }
        }
    }

} /* namespace dkmage */