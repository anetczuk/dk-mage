/*
 * Draw.cpp
 *
 */

#include "dkmage/Draw.h"

#include "dkmage/spatial/Maze.h"

#include "utils/Container.h"
#include "utils/Rand.h"


using namespace utils;
using namespace adiktedpp;


namespace dkmage {

    void drawMaze( Level& level, const spatial::Maze& maze ) {
        const std::size_t xDimm = maze.dimmensionX();
        const std::size_t yDimm = maze.dimmensionY();

        const Rect bbox = maze.boundingBox();
        const Point& minPoint = bbox.min;

        for ( std::size_t y=0; y<yDimm; ++y ) {
            for ( std::size_t x=0; x<xDimm; ++x ) {
                const spatial::Maze::PathType val = maze.state( x, y );
                switch ( val ) {
                case spatial::Maze::PathType::PT_ROCK: {
                    /// closed
                    level.setSlab( minPoint + Point( x, y ), Slab::S_ROCK );
                    break ;
                }
                case spatial::Maze::PathType::PT_EARTH: {
                    /// open
                    level.setSlab( minPoint + Point( x, y ), Slab::S_EARTH );
                    break ;
                }
                case spatial::Maze::PathType::PT_LAVA: {
                    /// closed
                    level.setSlab( minPoint + Point( x, y ), Slab::S_LAVA );
                    break ;
                }
                }
            }
        }
    }

    void drawGoldVein( Level& level, const Rect& veinRect, const std::size_t gemFaces ) {
        const std::size_t veinSize = veinRect.area() * 0.7;
        drawGoldVein( level, veinRect, veinSize, gemFaces );
    }

    void drawGoldVein( Level& level, const Rect& veinRect, const std::size_t goldSlabs, const std::size_t gemFaces ) {
        if ( gemFaces == 0 ) {
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
            return ;
        }

        const Point veinCenter = veinRect.center();

        Point gemCenter;
        int veinDir = 1;
        const Rect mapSize = raw::RawLevel::mapSize();
        if ( veinCenter.y > mapSize.center().y / 2 ) {
            /// on south
            veinDir = 1;
            gemCenter = veinRect.centerBottom();
        } else {
            /// on north
            veinDir = -1;
            gemCenter = veinRect.centerTop();
        }

        switch( gemFaces ) {
        case 1: {
            level.digLine( gemCenter, veinCenter, Slab::S_GOLD );                       /// ensure access to gem
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter.x + 1, gemCenter.y, Slab::S_ROCK );                /// ensure available face
            level.setSlab( gemCenter.x - 1, gemCenter.y, Slab::S_ROCK );                /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y + veinDir, Slab::S_ROCK );          /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y - veinDir, Slab::S_GOLD );          /// ensure available face
            break;
        }
        case 2: {
            level.digLine( gemCenter, veinCenter, Slab::S_GOLD );                       /// ensure access to gem
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter.x - 1, gemCenter.y, Slab::S_ROCK );                /// ensure available face
            level.setSlab( gemCenter.x + 1, gemCenter.y, Slab::S_GOLD );                /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y + veinDir, Slab::S_ROCK );          /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y - veinDir, Slab::S_GOLD );          /// ensure available face
            break;
        }
        case 3: {
            level.digLine( gemCenter, veinCenter, Slab::S_GOLD );                       /// ensure access to gem
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter.x - 1, gemCenter.y, Slab::S_GOLD );                /// ensure available face
            level.setSlab( gemCenter.x + 1, gemCenter.y, Slab::S_GOLD );                /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y + veinDir, Slab::S_ROCK );          /// ensure available face
            level.setSlab( gemCenter.x, gemCenter.y - veinDir, Slab::S_GOLD );          /// ensure available face
            break;
        }
        case 4: {
            level.digLine( gemCenter, veinCenter, Slab::S_GOLD );                       /// ensure access to gem
            gemCenter -= Point(0, veinDir);
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, Slab::S_EARTH );
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
            level.setSlab( gemCenter, Slab::S_GEMS );
            break;
        }
        default: {
            LOG() << "unhandled gem faces size: " << gemFaces;
        }
        }
    }

    void drawGoldChests( Level& level, const Rect& treasureRect ) {
        /// prevents picking up gold from unclaimed path
        level.setClaimed( treasureRect, Player::P_GOOD );
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
        const std::size_t index1 = rng_randi( list.size() );
        const Creature creature1 = get_item( list, index1 );
        const std::size_t index2 = rng_randi( list.size() );
        const Creature creature2 = get_item( list, index2 );

        level.setCreature( point, 3, creature1, 3, cLevel - 1, Player::P_GOOD );
        level.setCreature( point, 5, creature2, 2, cLevel, Player::P_GOOD );
    }

} /* namespace dkmage */
