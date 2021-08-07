/*
 * AreaDetector.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_

/// inclusion: #include "adiktedpp/AreaDetector.h"

#include "adiktedpp/raw/RawLevel.h"

#include <queue>
//#include <vector>


namespace adiktedpp {

    /**
     *
     */
    struct AreaInfo {
        std::size_t posx;
        std::size_t posy;
        std::vector< std::size_t > cells;
    };


    /**
     *
     */
    template <std::size_t SX, std::size_t SY>
    class AreaDetector {
    public:

        int data[ SX * SY ] = { 0 };


        void set( const std::size_t x, const std::size_t y, const int val ) {
            data[ y * SX + x ] = val;
//            data[y][x] = val;
        }

        int get( const std::size_t x, const std::size_t y ) const {
            return data[ y * SX + x ];
//            return data[y][x];
        }

        /// assign numbers to separated areas indicated by 'value'
        std::vector< AreaInfo > fill( const int value ) {
            std::vector< AreaInfo > areas;
            for ( std::size_t y=0; y<SY; ++y) {
                for ( std::size_t x=0; x<SX; ++x) {
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

        std::vector< std::size_t > fillValue( const std::size_t startX, const std::size_t startY, const int oldValue, const int newValue ) {
            std::vector< std::size_t > ret;
            std::queue< std::pair<std::size_t, std::size_t> > obj;
            obj.push( { startX, startY } );
            while ( obj.empty() == false ) {
                const std::pair<std::size_t, std::size_t> coord = obj.front();
                const std::size_t x = coord.first;
                const std::size_t y = coord.second;

                /// poping front pair of queue
                obj.pop();

                if ( x >= SX ) {
                    continue ;
                }
                if ( y >= SY ) {
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
                ret.push_back( y * SX + x );

                obj.push( { x + 1, y } );
                obj.push( { x - 1, y } );
                obj.push( { x, y + 1 } );
                obj.push( { x, y - 1 } );
            }

            return ret;
        }

//        /// recursive solution
//        std::size_t fillValue( const std::size_t x, const std::size_t y, const int oldValue, const int newValue ) {
//            if ( x >= SX ) {
//                return 0;
//            }
//            if ( y >= SY ) {
//                return 0;
//            }
//            if ( get( x, y ) != oldValue ) {
//                return 0;
//            }
//            if ( get( x, y ) == newValue ) {
//                return 0;
//            }
//            set( x, y, newValue );
//            std::size_t ret = 1;
//            ret += fillValue( x+1, y, oldValue, newValue );
//            ret += fillValue( x-1, y, oldValue, newValue );
//            ret += fillValue( x, y+1, oldValue, newValue );
//            ret += fillValue( x, y-1, oldValue, newValue );
//            return ret;
//        }

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_AREADETECTOR_H_ */
