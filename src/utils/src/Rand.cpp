/*
 * Rand.cpp
 *
 */

#include "utils/Rand.h"


extern "C" {
    #include "libadikted/rng.h"
}


namespace utils {

    void srand_ng( unsigned int seed ) {
        rng_srand( seed );
    }

    bool randb() {
        return ( rng_rand() % 2 == 1 );
    }

    bool randb( const double probability ) {
        return ( randd() < probability );
    }

    /// returns value from range [ 0, MAX )
    unsigned int randi() {
        return rng_rand();
    }

    /// returns value from range [ 0, maxValue )
    unsigned int randi( const unsigned int maxValue ) {
        const unsigned int rint = rng_rand();
        return ( rint % maxValue );
    }

    /// returns value from range [ minValue, maxValue )
    unsigned int randi( const int minValue, const int maxValue ) {
        const int rint = rng_rand();
        return ( rint % (maxValue - minValue) ) + minValue;
    }

    /**
     * Return value in range [0..1].
     */
    double randd() {
        const int rint   = rng_rand();
        const double val = ((double) rint / (RAND_MAX));
        return val;
    }

    std::string genSeed( const std::size_t length ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = rng_rand() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }

}
