/*
 * ProbabilityMass.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_
#define UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_

/// inclusion: #include "utils/ProbabilityMass.h"

#include "utils/Rand.h"

#include <map>
#include <set>


namespace utils {

    /**
     * Implementation of concept: https://en.wikipedia.org/wiki/Probability_mass_function
     */
    template <typename T>
    class ProbabilityMass {

        std::map< T, double > weights;


    public:

        ProbabilityMass() = default;

        bool empty() const {
            return weights.empty();
        }

        void set( const std::set<T>& values, const double weight ) {
            for ( const T& item: values ) {
                weights[ item ] = weight;
            }
        }

        void set( const T value, const double weight ) {
            weights[ value ] = weight;
        }

        void normalize() {
            double weightSum = 0.0;
            auto iter = weights.begin();
            auto eiter = weights.end();
            for ( ; iter != eiter; ++iter ) {
                weightSum += iter->second;
            }

            iter = weights.begin();
            for ( ; iter != eiter; ++iter ) {
                iter->second = iter->second / weightSum;
            }
        }

        /// get map item by value
        T get( const double value ) const {
            if ( weights.empty() ) {
                throw std::runtime_error( "empty probability map" );
            }
            double number = value;
            auto iter  = weights.begin();
            auto eiter = weights.end();
            for ( ; iter != eiter; ++iter ) {
                if ( number < iter->second ) {
                    return iter->first;
                }
                number -= iter->second;
            }
            /// return greatest key
            return weights.rbegin()->first;
        }

        T getRandom() const {
            const double num = randd();
            return get( num );
        }

    };

} /* namespace utils */

#endif /* UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_ */
