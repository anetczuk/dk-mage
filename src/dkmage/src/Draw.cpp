/*
 * Draw.cpp
 *
 */

#include "dkmage/Draw.h"

#include "dkmage/generator/Dungeon.h"
#include "dkmage/generator/Maze.h"

#include "utils/Rand.h"


namespace dkmage {

    void drawDungeon( adiktedpp::Level& level, const dkmage::generator::Dungeon& dungeon ) {
        const adiktedpp::PlayerType owner = dungeon.owner();
        const bool fortify = dungeon.fortify();
        std::vector< const dkmage::generator::Room* > roomsList = dungeon.rooms();
        for ( const dkmage::generator::Room* item: roomsList ) {
            /// set room
            const Rect& position = item->position();
            const adiktedpp::SlabType itemType = item->type();
            level.setRoom( position, itemType, owner, fortify );

            /// draw corridors
            const Point& itemCenter = item->position().center();
            std::vector< const dkmage::generator::Room* > connectedList = dungeon.connectedRooms( *item );
            for ( const dkmage::generator::Room* connected: connectedList ) {
                const Point& connectedCenter = connected->position().center();
                level.digLine( itemCenter, connectedCenter, owner, fortify );
            }
        }
    }

    void drawMaze( adiktedpp::Level& level, const dkmage::generator::Maze& maze ) {
        const std::size_t xDimm = maze.dimmensionX();
        const std::size_t yDimm = maze.dimmensionY();

        const utils::Rect bbox = maze.boundingBox();
        const utils::Point& minPoint = bbox.min;

        for ( std::size_t y=0; y<yDimm; ++y ) {
            for ( std::size_t x=0; x<xDimm; ++x ) {
                const dkmage::generator::Maze::PathType val = maze.state( x, y );
                switch ( val ) {
                case dkmage::generator::Maze::PathType::PT_ROCK: {
                    /// closed
                    level.setSlab( minPoint + utils::Point( x, y ), adiktedpp::SlabType::ST_ROCK );
                    break ;
                }
                case dkmage::generator::Maze::PathType::PT_EARTH: {
                    /// open
                    level.setSlab( minPoint + utils::Point( x, y ), adiktedpp::SlabType::ST_EARTH );
                    break ;
                }
                case dkmage::generator::Maze::PathType::PT_LAVA: {
                    /// closed
                    level.setSlab( minPoint + utils::Point( x, y ), adiktedpp::SlabType::ST_LAVA );
                    break ;
                }
                }
            }
        }
    }

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces ) {
        const Point veinCenter = veinRect.center();

        int veinDir = 1;
        const utils::Rect mapSize = level.mapSize();
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
            level.setChamber( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            break;
        }
        case 1: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, adiktedpp::SlabType::ST_ROCK );
            level.setChamber( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );
            level.setSlab( gemCenter.x, gemCenter.y - veinDir * 1, adiktedpp::SlabType::ST_GOLD );          /// ensure available face
            break;
        }
        case 2: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );                              /// 2 faces
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, adiktedpp::SlabType::ST_GOLD );
            level.setChamber( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
            level.setSlab( gemCenter, adiktedpp::SlabType::ST_GEMS );
            level.setSlab( gemCenter.x - 1, gemCenter.y + veinDir * 1,
                           gemCenter.x + 1, gemCenter.y + veinDir * 1, adiktedpp::SlabType::ST_ROCK );
            level.setSlab( gemCenter.x - 1, gemCenter.y, adiktedpp::SlabType::ST_ROCK );
            break;
        }
        case 3: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );                              /// 3 faces
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, adiktedpp::SlabType::ST_GOLD );
            level.setChamber( veinRect, adiktedpp::SlabType::ST_GOLD, veinSize );
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

    void drawGoldChests( adiktedpp::Level& level, const utils::Rect& treasureRect ) {
        /// prevents picking up gold from unclaimed path
        level.setSlab( treasureRect, adiktedpp::SlabType::ST_CLAIMED, adiktedpp::PlayerType::PT_GOOD );
        level.setItem( treasureRect, 4, adiktedpp::SubTypeItem::STI_GOLDCHEST );
    }

    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType ) {
        const utils::Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3X( level, trapCenter, trap );
    }

    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap ) {
        level.setSlab( trapCenter, adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter, 4, trap );

        level.setSlab( trapCenter + utils::Point(-1,-1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point(-1,-1), 4, trap );
        level.setSlab( trapCenter + utils::Point( 1,-1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 1,-1), 4, trap );
        level.setSlab( trapCenter + utils::Point(-1, 1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point(-1, 1), 4, trap );
        level.setSlab( trapCenter + utils::Point( 1, 1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 1, 1), 4, trap );
    }

    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType ) {
        const utils::Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3Diamond( level, trapCenter, trap );
    }

    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap ) {
        level.setSlab( trapCenter + utils::Point( 1, 0), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 1, 0), 4, trap );
        level.setSlab( trapCenter + utils::Point(-1, 0), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point(-1, 0), 4, trap );
        level.setSlab( trapCenter + utils::Point( 0,-1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 0,-1), 4, trap );
        level.setSlab( trapCenter + utils::Point( 0, 1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 0, 1), 4, trap );
    }

    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType ) {
        const utils::Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3Corners( level, trapCenter, trap );
    }

    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap ) {
        level.setSlab( trapCenter + utils::Point( 1, 1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 1, 1), 4, trap );
        level.setSlab( trapCenter + utils::Point(-1, 1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point(-1, 1), 4, trap );
        level.setSlab( trapCenter + utils::Point( 1,-1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point( 1,-1), 4, trap );
        level.setSlab( trapCenter + utils::Point(-1,-1), adiktedpp::SlabType::ST_PATH );
        level.setTrap( trapCenter + utils::Point(-1,-1), 4, trap );
    }

    void drawSpecial3x3( adiktedpp::Level& level, const utils::Point& specialCenter, const adiktedpp::SubTypeItem specialItem ) {
        const utils::Rect rect( specialCenter, 3, 3 );
        drawSpecial( level, rect, specialItem );
    }

    void drawSpecial( adiktedpp::Level& level, const utils::Rect& chamber, const adiktedpp::SubTypeItem specialItem ) {
        if ( chamber.valid() == false ) {
            LOG() << "invalid rect";
            return ;
        }
        level.setSlab( chamber, adiktedpp::SlabType::ST_PATH );
        const utils::Point specialCenter = chamber.center();
        level.setItem( specialCenter, 4, specialItem );
        if ( chamber.width() == 3 && chamber.height() == 3 ) {
            drawTrap3x3Diamond( level, specialCenter, adiktedpp::SubTypeTrap::STT_BOULDER );
            return ;
        }
        LOG() << "unsupported rect dimensions";
    }

    void drawHeroTrap( adiktedpp::Level& level, const utils::Point& point, const int creatureLevel ) {
        const int cLevel = std::min( std::max( creatureLevel, 1 ), 9 );

        std::set< adiktedpp::SubTypeCreature > list = adiktedpp::SubTypeCreatureHero();
        list.erase( adiktedpp::SubTypeCreature::STC_TUNELER );
        list.erase( adiktedpp::SubTypeCreature::STC_KNIGHT );
        list.erase( adiktedpp::SubTypeCreature::STC_AVATAR );
        const std::size_t index1 = rand() % list.size();
        const adiktedpp::SubTypeCreature creature1 = utils::getSetItem( list, index1 );
        const std::size_t index2 = rand() % list.size();
        const adiktedpp::SubTypeCreature creature2 = utils::getSetItem( list, index2 );

        level.setCreature( point, 3, creature1, 3, cLevel - 1, adiktedpp::PlayerType::PT_GOOD );
        level.setCreature( point, 5, creature2, 2, cLevel, adiktedpp::PlayerType::PT_GOOD );
    }

} /* namespace dkmage */
