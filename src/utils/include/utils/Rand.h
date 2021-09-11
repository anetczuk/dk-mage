/*
 * Rand.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RAND_H_
#define UTILS_INCLUDE_UTILS_RAND_H_

/// inclusion: #include "utils/Rand.h"

#include "utils/Container.h"

#include <string>


namespace utils {

    void rng_srand( unsigned int seed );

    /// initialize RNG with random seed
    void rng_srand();

    bool rng_randb();

    bool rng_randb( const double probability );

    /// returns value from range [ 0, MAX )
    unsigned int rng_randi();

    /// returns value from range [ 0, maxValue )
    unsigned int rng_randi( const unsigned int maxValue );

    /// returns value from range [ minValue, maxValue )
    unsigned int rng_randi( const unsigned int minValue, const unsigned int maxValue );

    /**
     * Return value in range [0..1].
     */
    double rng_randd();

    void rng_consume( const std::size_t num );

    std::string genSeed( const std::size_t length = 8 );

    template< typename T>
    T rng_rand( const std::set<T>& data ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            return get_item( data, 0 );
        }
        const std::size_t index = rng_randi( data.size() );
        return get_item( data, index );
    }

    template< typename T>
    T rng_rand( std::set<T>& data, const bool popItem ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            return get_item( data, 0, popItem );
        }
        const std::size_t index = rng_randi( data.size() );
        return get_item( data, index, popItem );
    }

    template< typename T>
    T rng_rand( const std::vector<T>& data ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            return data[ 0 ];
        }
        const std::size_t index = rng_randi( data.size() );
        return data[ index ];
    }

    template< typename T>
    T rng_rand( std::vector<T>& data, const bool popItem ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            if ( popItem == false ) {
                return data[ 0 ];
            }
            const T ret = data[ 0 ];
            data.clear();
            return ret;
        }
        const std::size_t index = rng_randi( data.size() );
        if ( popItem == false ) {
            return data[ index ];
        }
        const T ret = data[ index ];
        remove_at( data, index );
        return ret;
    }

}


#endif /* UTILS_INCLUDE_UTILS_RAND_H_ */
