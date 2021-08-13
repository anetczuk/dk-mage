/*
 * Rand.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RAND_H_
#define UTILS_INCLUDE_UTILS_RAND_H_

/// inclusion: #include "utils/Rand.h"

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

    std::string genSeed( const std::size_t length = 8 );

}


#endif /* UTILS_INCLUDE_UTILS_RAND_H_ */
