/*
 * DungeonGraph.cpp
 *
 */

#include "dkmage/spatial/DungeonGraph.h"


using namespace utils;


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

        Point edgePoint( const Rect& rect, const Direction direction, const std::size_t delta ) {
            switch( direction ) {
            case Direction::D_NORTH: return rect.centerTop( delta );
            case Direction::D_SOUTH: return rect.centerBottom( delta );
            case Direction::D_WEST:  return rect.leftCenter( delta );
            case Direction::D_EAST:  return rect.rightCenter( delta );
            }
            LOG() << "invalid case";
            return rect.center();
        }

    } /* namespace spatial */
} /* namespace dkmage */
