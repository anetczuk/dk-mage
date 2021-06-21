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


    void drawDungeon( adiktedpp::Level& level, dkmage::generator::Dungeon& dungeon );

    void drawMaze( adiktedpp::Level& level, dkmage::generator::Maze& maze, const utils::Point& minPoint );

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces );

    /// draw 5 traps in 3x3 rectangle
    void drawTrap3x3A( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap );

    /// draw 4 traps in 3x3 rectangle
    void drawTrap3x3B( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::SubTypeTrap trap );

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_DRAW_H_ */
