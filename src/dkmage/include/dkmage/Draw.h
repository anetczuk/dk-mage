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

    void drawMaze( adiktedpp::Level& level, dkmage::generator::Maze& maze );

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces );

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_DRAW_H_ */