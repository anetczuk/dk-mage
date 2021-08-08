/*
 * AreaDetector.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_

/// inclusion: #include "adiktedpp/AreaDetector.h"

#include "adiktedpp/raw/RawLevel.h"

#include <queue>


namespace adiktedpp {

    struct AreaData;


    /**
     *
     */
    class AreaDetector {
    public:

        struct AreaInfo {
            std::size_t posx;
            std::size_t posy;
            std::vector< std::size_t > cells;

            std::vector< utils::Point > cellsAsPoints() const;

        };


    private:

        std::unique_ptr<AreaData> data;


    public:

        AreaDetector();

        ~AreaDetector();

        void set( const std::size_t x, const std::size_t y, const int val );

        void set( const utils::Point point, const int val );

        void set( const std::set< utils::Point >& points, const int val );

        int get( const std::size_t x, const std::size_t y ) const;

        /// assign numbers to separated areas indicated by 'value'
        std::vector< AreaInfo > fill( const int value );

        void fill( const std::size_t x, const std::size_t y, const int value );

        std::vector< std::size_t > fillValue( const std::size_t startX, const std::size_t startY, const int oldValue, const int newValue );


        static std::size_t AreaSizeX();

        static std::size_t AreaSizeY();

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_ */
