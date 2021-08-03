/*
 * Spatialtem.cpp
 *
 */

#include "dkmage/spatial/Spatialtem.h"

#include "adiktedpp/Level.h"


namespace dkmage {
    namespace spatial {

        Spatialtem::Spatialtem() {
        }

        void Spatialtem::centerize() {
            const utils::Rect bbox = boundingBox();
            const utils::Point center = bbox.center();
            move( -center.x, -center.y );
        }

        void Spatialtem::centerizeOnMap() {
            const utils::Rect mapRect = adiktedpp::raw::RawLevel::mapRect();
            const utils::Point mapCenter = mapRect.center();
            centerizeOn( mapCenter );
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
            centerizeOnMap();
            const utils::Rect bbox = boundingBox();
            const int yoffset = 0 - bbox.min.y + distanceFromEdge + 1;          /// +1 to prevent going outside map
            move( 0, yoffset );
        }

        void Spatialtem::moveToBottomEdge( const std::size_t distanceFromEdge ) {
            centerizeOnMap();
            const utils::Rect mapRect = adiktedpp::raw::RawLevel::mapRect();
            const utils::Rect bbox = boundingBox();
            const int yoffset = mapRect.max.y + 1 - bbox.max.y - distanceFromEdge - 1;         /// -1 to prevent going outside map
            move( 0, yoffset );
        }

    } /* namespace spatial */
} /* namespace dkmage */
