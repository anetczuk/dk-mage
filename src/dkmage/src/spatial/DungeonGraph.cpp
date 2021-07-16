/*
 * DungeonGraph.cpp
 *
 */

#include "dkmage/spatial/DungeonGraph.h"


namespace dkmage {
    namespace spatial {

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

    } /* namespace spatial */
} /* namespace dkmage */
