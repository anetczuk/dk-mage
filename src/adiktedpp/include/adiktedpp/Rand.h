/*
 * Rand.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_RAND_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_RAND_H_

/// inclusion: #include "adiktedpp/Rand.h"

#include "adiktedpp/raw/RNG.h"

#include "utils/Container.h"

#include <string>


namespace adiktedpp {

    /// pass RNG with ownership (release object's memory)
    void rng_set( std::unique_ptr<raw::AbstractRNG>& newRNG );

    /// pass RNG without granting ownership (do not release object's memory)
    void rng_set_generator( raw::AbstractRNG& newRNG );

    /// set internal RNG with given seed
    void rng_srand( unsigned int seed );

    /// set internal RNG with random seed
    void rng_srand();

    /// =============================================================================

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

//    void rng_consume( const std::size_t num );

    std::string gen_seed( const std::size_t length = 8 );

    template< typename T>
    T rng_rand( const std::set<T>& data ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            return utils::get_item( data, 0 );
        }
        const std::size_t index = rng_randi( data.size() );
        return utils::get_item( data, index );
    }

    template< typename T>
    T rng_rand( std::set<T>& data, const bool popItem ) {
        const std::size_t dSize = data.size();
        if ( dSize < 1 ) {
            return T();
        }
        if ( dSize < 2 ) {
            return utils::get_item( data, 0, popItem );
        }
        const std::size_t index = rng_randi( data.size() );
        return utils::get_item( data, index, popItem );
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
        utils::remove_at( data, index );
        return ret;
    }

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_RAND_H_ */
