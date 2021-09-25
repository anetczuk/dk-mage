/*
 * AreaDetector.cpp
 *
 */

#include "adiktedpp/AreaDetector.h"

#include "utils/Log.h"

extern "C" {
    #include "libadikted/lev_data.h"
}


namespace adiktedpp {

    struct AreaData {

        int data[ MAP_SIZE_DKSTD_X * MAP_SIZE_DKSTD_Y ] = { 0 };


        void set( const std::size_t x, const std::size_t y, const int val ) {
            data[ y * MAP_SIZE_DKSTD_X + x ] = val;
        }

        void set( const utils::Point point, const int val ) {
            data[ point.y * MAP_SIZE_DKSTD_X + point.x ] = val;
        }

        void set( const std::set< utils::Point >& points, const int val ) {
            for ( const utils::Point item: points ) {
                data[ item.y * MAP_SIZE_DKSTD_X + item.x ] = val;
            }
        }

        int get( const std::size_t x, const std::size_t y ) const {
            return data[ y * MAP_SIZE_DKSTD_X + x ];
        }

    };


    /// ======================================================================


    std::vector< utils::Point > AreaDetector::AreaInfo::cellsAsPoints() const {
        std::vector< utils::Point > ret;
        for ( const auto cell: cells ) {
            const std::size_t cellX = cell % MAP_SIZE_DKSTD_X;
            const std::size_t cellY = cell / MAP_SIZE_DKSTD_X;
            ret.push_back( utils::Point(cellX, cellY) );
        }
        return ret;
    }

    AreaDetector::AreaDetector(): data( new AreaData() ) {
    }

    AreaDetector::~AreaDetector() {
    }

    void AreaDetector::set( const std::size_t x, const std::size_t y, const int val ) {
        data->set( x, y, val );
    }

    void AreaDetector::set( const utils::Point point, const int val ) {
        data->set( point, val );
    }

    void AreaDetector::set( const std::set< utils::Point >& points, const int val ) {
        data->set( points, val );
    }

    int AreaDetector::get( const std::size_t x, const std::size_t y ) const {
        return data->get( x, y );
    }

    std::vector< AreaDetector::AreaInfo > AreaDetector::fill( const int value ) {
        std::vector< AreaInfo > areas;
        for ( std::size_t y=0; y<MAP_SIZE_DKSTD_Y; ++y) {
            for ( std::size_t x=0; x<MAP_SIZE_DKSTD_X; ++x) {
                if ( get( x, y ) == value ) {
                     /// cell not filled
                    const std::size_t num = areas.size() + 1;
                    AreaInfo info;
                    info.posx = x;
                    info.posy = y;
                    info.cells = fillValue( x, y, value, num );
                    areas.push_back( info );
                }
            }
        }
        return areas;
    }

    void AreaDetector::fill( const std::size_t x, const std::size_t y, const int value ) {
        const int oldValue = get( x, y );
        if ( oldValue == value ) {
            LOG_INFO() << "area already filled";
            return ;
        }
        fillValue( x, y, oldValue, value );
    }

    std::vector< std::size_t > AreaDetector::fillValue( const std::size_t startX, const std::size_t startY, const int oldValue, const int newValue ) {
        std::vector< std::size_t > ret;
        std::queue< std::pair<std::size_t, std::size_t> > obj;
        obj.push( { startX, startY } );
        while ( obj.empty() == false ) {
            const std::pair<std::size_t, std::size_t> coord = obj.front();
            const std::size_t x = coord.first;
            const std::size_t y = coord.second;

            /// poping front pair of queue
            obj.pop();

            if ( x >= MAP_SIZE_DKSTD_X ) {
                continue ;
            }
            if ( y >= MAP_SIZE_DKSTD_Y ) {
                continue ;
            }

            const int preColor = get( x, y );
            if ( preColor != oldValue ) {
                continue ;
            }
            if ( preColor == newValue ) {
                continue ;
            }

            set( x, y, newValue );
            ret.push_back( y * MAP_SIZE_DKSTD_X + x );

            obj.push( { x + 1, y } );
            obj.push( { x - 1, y } );
            obj.push( { x, y + 1 } );
            obj.push( { x, y - 1 } );
        }

        return ret;
    }


    std::size_t AreaDetector::AreaSizeX() {
        return MAP_SIZE_DKSTD_X;
    }

    std::size_t AreaDetector::AreaSizeY() {
        return MAP_SIZE_DKSTD_Y;
    }

} /* namespace adiktedpp */
