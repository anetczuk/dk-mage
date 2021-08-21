/*
 * Rand.cpp
 *
 */

#include "utils/Rand.h"


namespace libadikted {
    extern "C" {
        #include "libadikted/rng.h"
    }
}


namespace utils {

    void rng_srand( unsigned int seed ) {
        libadikted::rng_srand( seed );
    }

    void rng_srand() {
        libadikted::rng_srand_random();
    }

    bool rng_randb() {
        return ( libadikted::rng_rand() % 2 == 1 );
    }

    bool rng_randb( const double probability ) {
        return ( rng_randd() < probability );
    }

    /// returns value from range [ 0, MAX )
    unsigned int rng_randi() {
        return libadikted::rng_rand();
    }

    /// returns value from range [ 0, maxValue )
    unsigned int rng_randi( const unsigned int maxValue ) {
        const unsigned int rint = libadikted::rng_rand();
        return ( rint % maxValue );
    }

    /// returns value from range [ minValue, maxValue )
    unsigned int rng_randi( const unsigned int minValue, const unsigned int maxValue ) {
        const unsigned int rint = libadikted::rng_rand();
        return ( rint % (maxValue - minValue) ) + minValue;
    }

    /**
     * Return value in range [0..1].
     */
    double rng_randd() {
        const unsigned int rint = libadikted::rng_rand();
        const double val = ((double) rint / libadikted::rng_rand_max() );
        return val;
    }

    void rng_consume( const std::size_t num ) {
        for ( std::size_t i=0; i<num; ++i ) {
            rng_randi();
        }
    }

    std::string genSeed( const std::size_t length ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = libadikted::rng_rand() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }

}
