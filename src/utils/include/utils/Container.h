/*
 * Container.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_CONTAINER_H_
#define UTILS_INCLUDE_UTILS_CONTAINER_H_

/// inclusion: #include "utils/Container.h"

#include <vector>
#include <set>
#include <algorithm>


namespace utils {

    template <typename T>
    T get_item( const std::set<T>& container, const std::size_t index ) {
        auto iter = container.begin();
        std::advance( iter, index );
        if ( iter == container.end() ) {
            return T();
        }
        return *iter;
    }

    template <typename T>
    T get_item( std::set<T>& container, const std::size_t index, const bool popItem ) {
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

    template <typename T>
    std::set< T > disjoint_sets( const std::set<T>& fromSet, const std::set<T>& substractSet ) {
        std::vector< T > output;
        std::set_difference( fromSet.begin(), fromSet.end(), substractSet.begin(), substractSet.end(), std::inserter(output, output.begin()) );
        return std::set< T >( output.begin(), output.end() );
    }


    /// =========================================================================================


    template <typename T>
    void remove( std::vector<T>& vec, const T data ) {
        vec.erase( std::remove( vec.begin(), vec.end(), data ), vec.end() );
    }

    template <typename T>
    T remove_at( std::vector<T>& v, typename std::vector<T>::size_type n ) {
        std::swap(v[n], v.back());
        T last = v.back();
        v.pop_back();
        return last;
    }

}

#endif /* UTILS_INCLUDE_UTILS_CONTAINER_H_ */
