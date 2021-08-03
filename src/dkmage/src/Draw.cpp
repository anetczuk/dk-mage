/*
 * Draw.cpp
 *
 */

#include "dkmage/Draw.h"

#include "dkmage/spatial/Dungeon.h"
#include "dkmage/spatial/Maze.h"

#include "utils/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {

    void drawDungeon( Level& level, const dkmage::spatial::Dungeon& dungeon ) {
        const PlayerType owner = dungeon.owner();
        const bool fortify = dungeon.fortify();
        std::vector< const dkmage::spatial::Room* > roomsList = dungeon.rooms();
        for ( const dkmage::spatial::Room* item: roomsList ) {
            /// set room
            const Rect& position = item->position();
            const Room itemType = item->type();
            level.setRoom( position, itemType, owner, fortify );

            /// draw corridors
            const Point& itemCenter = item->position().center();
            std::vector< const dkmage::spatial::Room* > connectedList = dungeon.connectedRooms( *item );
            for ( const dkmage::spatial::Room* connected: connectedList ) {
                const Point& connectedCenter = connected->position().center();
                level.digLine( itemCenter, connectedCenter, owner, fortify );
            }
        }
    }

    void drawMaze( Level& level, const dkmage::spatial::Maze& maze ) {
        const std::size_t xDimm = maze.dimmensionX();
        const std::size_t yDimm = maze.dimmensionY();

        const Rect bbox = maze.boundingBox();
        const Point& minPoint = bbox.min;

        for ( std::size_t y=0; y<yDimm; ++y ) {
            for ( std::size_t x=0; x<xDimm; ++x ) {
                const dkmage::spatial::Maze::PathType val = maze.state( x, y );
                switch ( val ) {
                case dkmage::spatial::Maze::PathType::PT_ROCK: {
                    /// closed
                    level.setSlab( minPoint + Point( x, y ), Slab::S_ROCK );
                    break ;
                }
                case dkmage::spatial::Maze::PathType::PT_EARTH: {
                    /// open
                    level.setSlab( minPoint + Point( x, y ), Slab::S_EARTH );
                    break ;
                }
                case dkmage::spatial::Maze::PathType::PT_LAVA: {
                    /// closed
                    level.setSlab( minPoint + Point( x, y ), Slab::S_LAVA );
                    break ;
                }
                }
            }
        }
    }

    void drawGoldVein( Level& level, const Rect& veinRect, const std::size_t gemFaces ) {
        const Point veinCenter = veinRect.center();

        int veinDir = 1;
        const Rect mapSize = raw::RawLevel::mapSize();
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
            level.setCave( veinRect, Slab::S_GOLD, veinSize );
            break;
        }
        case 1: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, Slab::S_ROCK );
            level.setCave( veinRect, Slab::S_GOLD, veinSize );
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter.x, gemCenter.y - veinDir * 1, Slab::S_GOLD );          /// ensure available face
            break;
        }
        case 2: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );                              /// 2 faces
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, Slab::S_GOLD );
            level.setCave( veinRect, Slab::S_GOLD, veinSize );
            level.setSlab( gemCenter, Slab::S_GEMS );
            const Rect rect( gemCenter.x - 1, gemCenter.y + veinDir * 1, gemCenter.x + 1, gemCenter.y + veinDir * 1 );
            level.setSlab( rect, Slab::S_ROCK );
            level.setSlab( gemCenter.x - 1, gemCenter.y, Slab::S_ROCK );
            break;
        }
        case 3: {
            const Point gemCenter( veinCenter.x, veinCenter.y + veinDir * 3 );                              /// 3 faces
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, Slab::S_GOLD );
            level.setCave( veinRect, Slab::S_GOLD, veinSize );
            level.setSlab( gemCenter, Slab::S_GEMS );
            const Rect rect( gemCenter.x - 1, gemCenter.y + veinDir * 1, gemCenter.x + 1, gemCenter.y + veinDir * 1 );
            level.setSlab( rect, Slab::S_ROCK );
            break;
        }
        default: {
            LOG() << "unhandled gem faces size: " << gemFaces;
        }
        }
    }

    void drawGoldChests( Level& level, const Rect& treasureRect ) {
        /// prevents picking up gold from unclaimed path
        level.setClaimed( treasureRect, PlayerType::PT_GOOD );
        level.setItem( treasureRect, 4, Item::I_GOLDCHEST );
    }

    void drawTrap3x3X( Level& level, const Point& trapCenter, const Trap trap, const Slab fillType ) {
        const Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3X( level, trapCenter, trap );
    }

    void drawTrap3x3X( Level& level, const Point& trapCenter, const Trap trap ) {
        level.setTrap( trapCenter, 4, trap );

        level.setTrap( trapCenter + Point(-1,-1), 4, trap );
        level.setTrap( trapCenter + Point( 1,-1), 4, trap );
        level.setTrap( trapCenter + Point(-1, 1), 4, trap );
        level.setTrap( trapCenter + Point( 1, 1), 4, trap );
    }

    void drawTrap3x3Diamond( Level& level, const Point& trapCenter, const Trap trap, const Slab fillType ) {
        const Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3Diamond( level, trapCenter, trap );
    }

    void drawTrap3x3Diamond( Level& level, const Point& trapCenter, const Trap trap ) {
        level.setTrap( trapCenter + Point( 1, 0), 4, trap );
        level.setTrap( trapCenter + Point(-1, 0), 4, trap );
        level.setTrap( trapCenter + Point( 0,-1), 4, trap );
        level.setTrap( trapCenter + Point( 0, 1), 4, trap );
    }

    void drawTrap3x3Corners( Level& level, const Point& trapCenter, const Trap trap, const Slab fillType ) {
        const Rect rect( trapCenter, 3, 3 );
        level.setSlab( rect, fillType );
        drawTrap3x3Corners( level, trapCenter, trap );
    }

    void drawTrap3x3Corners( Level& level, const Point& trapCenter, const Trap trap ) {
        level.setTrap( trapCenter + Point( 1, 1), 4, trap );
        level.setTrap( trapCenter + Point(-1, 1), 4, trap );
        level.setTrap( trapCenter + Point( 1,-1), 4, trap );
        level.setTrap( trapCenter + Point(-1,-1), 4, trap );
    }

    void drawSpecial3x3( Level& level, const Point& specialCenter, const Item specialItem ) {
        const Rect rect( specialCenter, 3, 3 );
        drawSpecial( level, rect, specialItem );
    }

    void drawSpecial( Level& level, const Rect& chamber, const Item specialItem ) {
        if ( chamber.valid() == false ) {
            LOG() << "invalid rect";
            return ;
        }
        level.setSlab( chamber, Slab::S_PATH );
        const Point specialCenter = chamber.center();
        level.setItem( specialCenter, 4, specialItem );
        if ( chamber.width() == 3 && chamber.height() == 3 ) {
            drawTrap3x3Diamond( level, specialCenter, Trap::T_BOULDER );
            return ;
        }
        LOG() << "unsupported rect dimensions";
    }

    void drawHeroTrap( Level& level, const Point& point, const int creatureLevel ) {
        const int cLevel = std::min( std::max( creatureLevel, 1 ), 9 );

        std::set< Creature > list = HeroCreatures();                /// yes, copy
        list.erase( Creature::C_TUNNELLER );
        list.erase( Creature::C_KNIGHT );
        list.erase( Creature::C_AVATAR );
        const std::size_t index1 = rand() % list.size();
        const Creature creature1 = getSetItem( list, index1 );
        const std::size_t index2 = rand() % list.size();
        const Creature creature2 = getSetItem( list, index2 );

        level.setCreature( point, 3, creature1, 3, cLevel - 1, PlayerType::PT_GOOD );
        level.setCreature( point, 5, creature2, 2, cLevel, PlayerType::PT_GOOD );
    }

} /* namespace dkmage */
