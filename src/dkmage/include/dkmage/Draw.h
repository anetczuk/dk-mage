/*
 * Draw.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_DRAW_H_
#define DKMAGE_INCLUDE_DKMAGE_DRAW_H_

#include "adiktedpp/Level.h"


namespace dkmage {

    namespace generator {
        class Dungeon;
        class Maze;
    }


    void drawDungeon( adiktedpp::Level& level, const dkmage::generator::Dungeon& dungeon );

    void drawMaze( adiktedpp::Level& level, const dkmage::generator::Maze& maze );

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces );

    void drawGoldChests( adiktedpp::Level& level, const utils::Rect& treasureRect );

    /// draw 5 traps in 3x3 rectangle (X shape)
    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap );
    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType );

    /// draw 4 traps in 3x3 rectangle (diamond shape)
    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap );
    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType );

    /// draw 4 traps in 3x3 rectangle (corners)
    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap );
    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap, const adiktedpp::SlabType fillType );

    void drawSpecial3x3( adiktedpp::Level& level, const utils::Point& specialCenter, const adiktedpp::SubTypeItem specialItem );

    void drawSpecial( adiktedpp::Level& level, const utils::Rect& chamber, const adiktedpp::SubTypeItem specialItem );

    /// 'creatureLevel' in range [0, 9] (9 is maximum level possible)
    void drawHeroTrap( adiktedpp::Level& level, const utils::Point& point, const int creatureLevel );

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_DRAW_H_ */
