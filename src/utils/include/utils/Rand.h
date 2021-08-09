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

    /**
     * Return value in range [0..1].
     */
    inline double randd() {
        const int rint   = rand();
        const double val = ((double) rint / (RAND_MAX));
        return val;
    }

    inline std::string genSeed( const std::size_t length = 8 ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = rand() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }

}


#endif /* UTILS_INCLUDE_UTILS_RAND_H_ */
