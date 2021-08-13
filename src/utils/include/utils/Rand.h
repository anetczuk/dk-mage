/*
 * Rand.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RAND_H_
#define UTILS_INCLUDE_UTILS_RAND_H_

/// inclusion: #include "utils/Rand.h"

#include <random>
#include <string>


namespace utils {

    void srand_ng( unsigned int seed );

    bool randb();

    bool randb( const double probability );

    /// returns value from range [ 0, MAX )
    unsigned int randi();

    /// returns value from range [ 0, maxValue )
    unsigned int randi( const unsigned int maxValue );

    /// returns value from range [ minValue, maxValue )
    unsigned int randi( const unsigned int minValue, const unsigned int maxValue );

    /**
     * Return value in range [0..1].
     */
    double randd();

    std::string genSeed( const std::size_t length = 8 );

}


#endif /* UTILS_INCLUDE_UTILS_RAND_H_ */
