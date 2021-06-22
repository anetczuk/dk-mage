/*
 * Spatialtem.cpp
 *
 */

#include "dkmage/generator/Spatialtem.h"


namespace dkmage {
    namespace generator {

        Spatialtem::Spatialtem() {
        }

        void Spatialtem::centerize() {
            const utils::Rect bbox = boundingBox();
            const utils::Point center = bbox.center();
            move( -center.x, -center.y );
        }

        void Spatialtem::centerizeOn( const int x, const int y ) {
            centerize();
            move( x, y );
        }

        void Spatialtem::centerizeOn( const utils::Point& point ) {
            centerize();
            move( point.x, point.y );
        }

        void Spatialtem::moveToTopEdge( const std::size_t distanceFromEdge ) {
            centerizeOn( 42, 42 );
            const utils::Rect bbox = boundingBox();
            const int yoffset = 0 - bbox.min.y + distanceFromEdge + 1;          /// +1 to prevent going outside map
            move( 0, yoffset );
        }

        void Spatialtem::moveToBottomEdge( const std::size_t distanceFromEdge ) {
            centerizeOn( 42, 42 );
            const utils::Rect bbox = boundingBox();
            const int yoffset = 84 - bbox.max.y - distanceFromEdge - 1;         /// -1 to prevent going outside map
            move( 0, yoffset );
        }

    } /* namespace generator */
} /* namespace dkmage */
