/*
 * Spatialtem.cpp
 *
 */

#include "dkmage/spatial/Spatialtem.h"

#include "adiktedpp/Level.h"


using namespace utils;


namespace dkmage {
    namespace spatial {

        Spatialtem::Spatialtem() {
        }

        void Spatialtem::centerize() {
            const Rect bbox = boundingBox();
            const Point center = bbox.center();
            move( -center.x, -center.y );
        }

        void Spatialtem::centerizeOnMap() {
            const Rect mapRect = adiktedpp::raw::RawLevel::mapRect();
            const Point mapCenter = mapRect.center();
            centerizeOn( mapCenter );
        }

        void Spatialtem::centerizeOn( const int x, const int y ) {
            centerize();
            move( x, y );
        }

        void Spatialtem::centerizeOn( const Point& point ) {
            centerize();
            move( point.x, point.y );
        }

        void Spatialtem::moveToTopEdge( const std::size_t distanceFromEdge ) {
            centerizeOnMap();
            const Rect bbox = boundingBox();
            const int yoffset = 0 - bbox.min.y + distanceFromEdge + 1;          /// +1 to prevent going outside map
            move( 0, yoffset );
        }

        void Spatialtem::moveToBottomEdge( const std::size_t distanceFromEdge ) {
            centerizeOnMap();
            const Rect mapRect = adiktedpp::raw::RawLevel::mapRect();
            const Rect bbox = boundingBox();
            const int yoffset = mapRect.max.y + 1 - bbox.max.y - distanceFromEdge - 1;         /// -1 to prevent going outside map
            move( 0, yoffset );
        }

    } /* namespace spatial */
} /* namespace dkmage */
