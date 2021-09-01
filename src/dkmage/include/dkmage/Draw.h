/*
 * Draw.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_DRAW_H_
#define DKMAGE_INCLUDE_DKMAGE_DRAW_H_

#include "adiktedpp/Level.h"


namespace dkmage {

    namespace spatial {
        class Maze;
    }


    void drawMaze( adiktedpp::Level& level, const dkmage::spatial::Maze& maze );

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t gemFaces );

    void drawGoldVein( adiktedpp::Level& level, const utils::Rect& veinRect, const std::size_t goldSlabs, const std::size_t gemFaces );

    void drawGemFaces( adiktedpp::Level& level, const utils::Point gemCenter, const std::size_t gemFaces, const adiktedpp::Slab fillType, const utils::Point corridorDir );

    void drawGoldChests( adiktedpp::Level& level, const utils::Rect& treasureRect );

    /// draw 5 traps in 3x3 rectangle (X shape)
    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap );
    void drawTrap3x3X( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap, const adiktedpp::Slab fillType );

    /// draw 4 traps in 3x3 rectangle (diamond shape)
    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap );
    void drawTrap3x3Diamond( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap, const adiktedpp::Slab fillType );

    /// draw 4 traps in 3x3 rectangle (corners)
    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap );
    void drawTrap3x3Corners( adiktedpp::Level& level, const utils::Point& trapCenter, const adiktedpp::Trap trap, const adiktedpp::Slab fillType );

    void drawSpecial3x3( adiktedpp::Level& level, const utils::Point& specialCenter, const adiktedpp::Item specialItem );

    void drawSpecial( adiktedpp::Level& level, const utils::Rect& chamber, const adiktedpp::Item specialItem );

    /// 'creatureLevel' in range [1, 10] (10 is maximum level possible)
    void drawHeroTrap( adiktedpp::Level& level, const utils::Point& point, const std::size_t creatureLevel, const std::size_t creaturesNum = 5 );

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_DRAW_H_ */
