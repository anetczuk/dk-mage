/*
 * Rand.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_RAND_H_
#define UTILS_INCLUDE_UTILS_RAND_H_

#include <random>
#include <string>
#include <set>


namespace utils {

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

    template <typename T>
    T getSetItem( const std::set<T>& container, const std::size_t index ) {
        auto iter = container.begin();
        std::advance( iter, index );
        if ( iter == container.end() ) {
            return T();
        }
        return *iter;
    }

    template <typename T>
    T getSetItem( std::set<T>& container, const std::size_t index, const bool popItem ) {
        auto iter = container.begin();
        std::advance( iter, index );
        if ( iter == container.end() ) {
            return T();
        }
        if ( popItem == false ) {
            return *iter;
        }
        T value = *iter;
        container.erase( iter );
        return value;
    }

}


#endif /* UTILS_INCLUDE_UTILS_RAND_H_ */
