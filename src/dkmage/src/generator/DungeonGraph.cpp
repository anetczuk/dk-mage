/*
 * DungeonGraph.cpp
 *
 */

#include "dkmage/generator/DungeonGraph.h"


namespace dkmage {
    namespace generator {

        const std::vector<Direction>& DirectionValues() {
            static std::vector<Direction> ret;
            if (ret.size() < 1) {
                ret.push_back( Direction::D_NORTH );
                ret.push_back( Direction::D_SOUTH );
                ret.push_back( Direction::D_EAST );
                ret.push_back( Direction::D_WEST );
            }
            return ret;
        }

    } /* namespace generator */
} /* namespace dkmage */
