/*
 * Set.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_SET_H_
#define UTILS_INCLUDE_UTILS_SET_H_

#include <set>
#include <algorithm>


namespace utils {

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

    template <typename T>
    std::set< T > disjoint_sets( const std::set<T>& fromSet, const std::set<T>& substractSet ) {
        std::vector< Point > output;
        std::set_difference( fromSet.begin(), fromSet.end(), substractSet.begin(), substractSet.end(), std::inserter(output, output.begin()) );
        return std::set< T >( output.begin(), output.end() );
    }

}

#endif /* UTILS_INCLUDE_UTILS_SET_H_ */
