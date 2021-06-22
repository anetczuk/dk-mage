/*
 * Spatialtem.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_GENERATOR_SPATIALTEM_H_
#define DKMAGE_INCLUDE_DKMAGE_GENERATOR_SPATIALTEM_H_

#include "utils/Rect.h"


namespace dkmage {
    namespace generator {

        /**
         *
         */
        class Spatialtem {
        public:

            Spatialtem();

            virtual ~Spatialtem() = default;

            virtual utils::Rect boundingBox() const = 0;

            virtual void move( const int offsetX, const int offsetY ) = 0;

            void move( const utils::Point offset ) {
                move( offset.x, offset.y );
            }

            void centerize();

            void centerizeOnMap();

            void centerizeOn( const int x, const int y );

            void centerizeOn( const utils::Point& point );

            /// method prevents moving to world edge
            void moveToTopEdge( const std::size_t distanceFromEdge );

            /// method prevents moving to world edge
            void moveToBottomEdge( const std::size_t distanceFromEdge );

        };

    } /* namespace generator */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_GENERATOR_SPATIALTEM_H_ */
