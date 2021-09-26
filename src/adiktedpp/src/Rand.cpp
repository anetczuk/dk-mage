/*
 * Rand.cpp
 *
 */

#include "adiktedpp/Rand.h"

#include "adiktedpp/raw/RNG.h"


namespace adiktedpp {

    raw::RNG& rng_instance() {
        static raw::RNG rng;
        return rng;
    }


    void rng_srand( unsigned int seed ) {
        raw::adikted_srand( seed );

        raw::RNG& engine = rng_instance();
        engine.srand( seed );
    }

    void rng_srand() {
        raw::adikted_srand();

        raw::RNG& engine = rng_instance();
        engine.srand();
    }

    bool rng_randb() {
        raw::RNG& engine = rng_instance();
        return engine.randb();
    }

    bool rng_randb( const double probability ) {
        raw::RNG& engine = rng_instance();
        return engine.randb( probability );
    }

    /// returns value from range [ 0, MAX )
    unsigned int rng_randi() {
        raw::RNG& engine = rng_instance();
        return engine.randi();
    }

    /// returns value from range [ 0, maxValue )
    unsigned int rng_randi( const unsigned int maxValue ) {
        raw::RNG& engine = rng_instance();
        return engine.randi( maxValue );
    }

    /// returns value from range [ minValue, maxValue )
    unsigned int rng_randi( const unsigned int minValue, const unsigned int maxValue ) {
        raw::RNG& engine = rng_instance();
        return engine.randi( minValue, maxValue );
    }

    /**
     * Return value in range [0..1].
     */
    double rng_randd() {
        raw::RNG& engine = rng_instance();
        return engine.randd();
    }

//    void rng_consume( const std::size_t num ) {
//        raw::RNG& engine = rng_instance();
//        return engine.consume( num );
//    }

    std::string gen_seed( const std::size_t length ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = rng_randi() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }

}
