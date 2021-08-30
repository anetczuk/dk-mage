/*
 * ProbabilityMass.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_
#define UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_

/// inclusion: #include "utils/ProbabilityMass.h"

#include "utils/Rand.h"

#include <sstream>
#include <map>
#include <set>


namespace utils {

    /**
     * Implementation of concept: https://en.wikipedia.org/wiki/Probability_mass_function
     */
    template <typename T>
    class ProbabilityMass {

        struct ValueSize {
            double weight;
            std::size_t allowed = (std::size_t) -1;             /// max possible value

            ValueSize& operator= (const double value) {
                weight  = value;
                allowed = (std::size_t) -1;
                return *this;
            }
            operator double() const {
                return weight;
            }
        };

        std::map< T, ValueSize > weights;


    public:

        ProbabilityMass() = default;

        bool empty() const {
            return weights.empty();
        }

        std::size_t size() const {
            return weights.size();
        }

        const std::map< T, ValueSize >& data() const {
            return weights;
        }

        void set( const std::set<T>& values, const double weight ) {
            for ( const T& item: values ) {
                weights[ item ] = weight;
            }
        }

        void set( const T value, const double weight ) {
            weights[ value ] = weight;
        }

        void set( const T value, const double weight, const std::size_t limit ) {
            weights[ value ] = { weight, limit };
        }

        void multiply( const T value, const double factor ) {
            auto iter = weights.find( value );
            if ( iter == weights.end() ) {
                /// no item to multiply
                return ;
            }
            ValueSize& data = iter->second;
            data.weight *= factor;
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
                ValueSize& item = iter->second;
                item.weight = item.weight / weightSum;
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

        void popItem( const T value ) {
            auto iter = weights.find( value );
            if ( iter == weights.end() ) {
                /// no item
                return ;
            }
            ValueSize& data = iter->second;
            if ( data.allowed <= 1 ) {
                weights.erase( value );
                normalize();
                return ;
            }
            --data.allowed;
        }

        /// pop map item by value
        T pop( const double value ) {
            const T ret = get( value );
            popItem( ret );
            return ret;
        }

        T getRandom() const {
            const double num = rng_randd();
            return get( num );
        }

        T popRandom() {
            const double num = rng_randd();
            return pop( num );
        }

        std::string print() const {
            std::stringstream stream;
            stream << "fortress rooms probability map:\n";
            auto iter  = weights.begin();
            auto eiter = weights.end();
            for ( ; iter != eiter; ++iter ) {
                if ( iter->second.allowed != (std::size_t) -1 ) {
                    stream << "     " << iter->first << " = " << iter->second.weight << " " << iter->second.allowed << "\n";
                } else {
                    stream << "     " << iter->first << " = " << iter->second.weight << " inf\n";
                }
            }
            const std::string dataString = stream.str();
            return dataString.substr( 0, dataString.length() - 1 );
        }

    };

} /* namespace utils */

#endif /* UTILS_INCLUDE_UTILS_PROBABILITYMASS_H_ */
