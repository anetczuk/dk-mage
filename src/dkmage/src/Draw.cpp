/*
 * Draw.cpp
 *
 */

#include "dkmage/Draw.h"

#include "dkmage/spatial/Maze.h"

#include "adiktedpp/script/Creator.h"
#include "adiktedpp/Rand.h"

#include "utils/Container.h"


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
            veinDir = -1;
            gemCenter = veinRect.centerBottom();
        } else {
            /// on north
            veinDir =  1;
            gemCenter = veinRect.centerTop();
        }

        if ( gemFaces > 0 && gemFaces < 5 ) {
            level.digLine( gemCenter, veinCenter, Slab::S_GOLD );                       /// ensure access to gem
            level.setCave( veinRect, Slab::S_GOLD, goldSlabs );
        }
        const Point corridorDir( 0, veinDir );
        drawGemFaces( level, gemCenter, gemFaces, Slab::S_GOLD, corridorDir );
    }

    void drawGemFaces( adiktedpp::Level& level, const utils::Point gemCenter, const std::size_t gemFaces, const adiktedpp::Slab fillType, const utils::Point corridorDir ) {
        switch( gemFaces ) {
        case 1: {
            const Point ortho = corridorDir.swapped();
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter - ortho, Slab::S_ROCK );                           /// ensure available face
            level.setSlab( gemCenter + ortho, Slab::S_ROCK );                           /// ensure available face
            level.setSlab( gemCenter - corridorDir, Slab::S_ROCK );                     /// ensure available face
            level.setSlab( gemCenter + corridorDir, fillType );                         /// ensure available face
            break;
        }
        case 2: {
            const Point ortho = corridorDir.swapped();
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter - ortho, Slab::S_ROCK );                           /// ensure available face
            level.setSlab( gemCenter + ortho, fillType );                               /// ensure available face
            level.setSlab( gemCenter + corridorDir + ortho, fillType );                 /// ensure available face
            level.setSlab( gemCenter - corridorDir, Slab::S_ROCK );                     /// ensure available face
            level.setSlab( gemCenter + corridorDir, fillType );                         /// ensure available face
            break;
        }
        case 3: {
            const Point ortho = corridorDir.swapped();
            level.setSlab( gemCenter, Slab::S_GEMS );
            level.setSlab( gemCenter - ortho, fillType );                               /// ensure available face
            level.setSlab( gemCenter + ortho, fillType );                               /// ensure available face
            level.setSlab( gemCenter + corridorDir + ortho, fillType );                 /// ensure available face
            level.setSlab( gemCenter + corridorDir - ortho, fillType );                 /// ensure available face
            level.setSlab( gemCenter - corridorDir, Slab::S_ROCK );                     /// ensure available face
            level.setSlab( gemCenter + corridorDir, fillType );                         /// ensure available face
            break;
        }
        case 4: {
            const Rect gemRect( gemCenter, 3, 3 );
            level.setSlab( gemRect, fillType );
            level.setSlab( gemCenter, Slab::S_GEMS );
            break;
        }
        default: {
            LOG_INFO() << "unhandled gem faces size: " << gemFaces;
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
            LOG_INFO() << "invalid rect";
            return ;
        }
        level.setSlab( chamber, Slab::S_PATH );
        const Point specialCenter = chamber.center();
        level.setItem( specialCenter, specialItem );
        if ( chamber.width() == 3 && chamber.height() == 3 ) {
            drawTrap3x3Diamond( level, specialCenter, Trap::T_BOULDER );
            return ;
        }
        LOG_INFO() << "unsupported rect dimensions";
    }

    void drawHeroTrap( Level& level, const Point& point, const std::size_t creatureLevel, const std::size_t creaturesNum ) {
        const int cLevel = std::min( std::max( creatureLevel, (std::size_t)2 ), (std::size_t)10 );

        std::set< Creature > list = HeroCreatures();                /// yes, copy
        list.erase( Creature::C_TUNNELLER );
        list.erase( Creature::C_KNIGHT );
        list.erase( Creature::C_AVATAR );
        const std::size_t index1 = rng_randi( list.size() );
        const Creature creature1 = get_item( list, index1 );
        const std::size_t index2 = rng_randi( list.size() );
        const Creature creature2 = get_item( list, index2 );

        const std::size_t num1 = creaturesNum / 2 + creaturesNum % 2;
        const std::size_t num2 = creaturesNum / 2;
        level.setCreature( point, 3, creature1, num1, cLevel, Player::P_GOOD );
        level.setCreature( point, 5, creature2, num2, cLevel, Player::P_GOOD );
    }

    void drawHeroTrap( script::Script& script, const adiktedpp::Player player, const std::size_t ambushAP,
                       const std::size_t creatureLevel, const std::size_t creaturesNum, const std::string& partyDescription )
    {
        const int cLevel = std::min( std::max( creatureLevel, (std::size_t)2 ), (std::size_t)10 );

        std::set< Creature > list = HeroCreatures();                /// yes, copy
        list.erase( Creature::C_TUNNELLER );
        list.erase( Creature::C_KNIGHT );
        list.erase( Creature::C_AVATAR );
        const std::size_t index1 = rng_randi( list.size() );
        const Creature creature1 = get_item( list, index1 );
        const std::size_t index2 = rng_randi( list.size() );
        const Creature creature2 = get_item( list, index2 );

        const std::size_t num1 = creaturesNum / 2 + creaturesNum % 2;
        const std::size_t num2 = creaturesNum / 2;

        const std::string ambushName =  "hero_trap_" + std::to_string( ambushAP );

        script::AmbushCreator ambush( script );
        ambush.partyId = ambushName;
        if ( partyDescription.empty() == false )
            ambush.description = "- " + partyDescription + " -";
        ambush.actionPoint = ambushAP;
        ambush.partyCopies = 2;
        ambush.owner = player;
        ambush.prepare();
        ambush.addToParty( creature1, num1, cLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );
        ambush.addToParty( creature2, num2, cLevel, 500, script::PartyObjective::PO_DEFEND_LOCATION );
    }

} /* namespace dkmage */
