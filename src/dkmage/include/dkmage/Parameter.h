/*
 * Parameter.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_
#define DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_

/// inclusion: #include "dkmage/Parameter.h"

#include "dkmage/ParameterName.h"

#include "adiktedpp/Rand.h"

#include <map>
#include <vector>


namespace dkmage {

    /**
     * Pre C++17 implementation of std::optional.
     */
    template <typename T>
    class Optional {
    public:

        bool initialized;
        T data;

        Optional(): initialized(false) {
        }

        Optional( const T& value ): initialized(true), data(value) {
        }

        void operator =( const T& value ) {
            initialized = true;
            data = value;
        }

        operator bool() const {
            return initialized;
        }

        bool has_value() const {
            return initialized;
        }
        /// alias
        bool isSet() const {
            return initialized;
        }

        const T& value() const {
            return data;
        }
        /// alias
        const T& getValue() const {
            return data;
        }

        const T& value_or( const T& defaultValue ) const {
            if ( initialized )
                return data;
            return defaultValue;
        }
        /// alias
        const T& getValue( const T& defaultValue ) const {
            if ( initialized )
                return data;
            return defaultValue;
        }

    };


    /// ================================================


    template<typename TNumber>
    class NumberRange {
    public:

        TNumber lowerBound;
        TNumber upperBound;


        NumberRange( const TNumber value ): lowerBound(value), upperBound(value) {
        }

        NumberRange( const TNumber lower, const TNumber upper ): lowerBound(lower), upperBound(upper) {
        }

        TNumber size() const {
            return upperBound - lowerBound;
        }

        TNumber item( const TNumber index ) const {
            return lowerBound + index;
        }

        TNumber maxValue() const {
            return upperBound;
        }

        bool contains( const TNumber value ) const {
            if ( value < lowerBound )
                return false;
            if ( value > upperBound )
                return false;
            return true;
        }

    };


    template<typename TNumber>
    class NumberSet {
    public:

        std::vector< NumberRange<TNumber> > data;

        TNumber size() const {
            TNumber iSize = 0;
            for ( const NumberRange<TNumber>& item: data ) {
                iSize += item.size();
            }
            return iSize;
        }

        void add( const TNumber value ) {
            data.push_back( NumberRange<TNumber>(value) );
        }

        void add( const TNumber lower, const TNumber upper ) {
            data.push_back( NumberRange<TNumber>(lower, upper) );
        }

        bool contains( const TNumber value ) const {
            for ( const auto& item: data ) {
                if ( item.contains( value ) ) {
                    return true;
                }
            }
            return false;
        }

        Optional<TNumber> valueByIndex( const std::size_t index ) const;

        /// factor in range [0..1]
        TNumber valueByFactor( const double factor ) const;

        NumberSet<TNumber> filter( const TNumber lower, const TNumber upper ) const;

        TNumber maxValue() const;

        TNumber randomized() const;

        Optional<TNumber> getRandom() const;

        Optional<TNumber> getRandom( const TNumber lower, const TNumber upper ) const;

    };

    template<>
    std::size_t NumberSet<std::size_t>::size() const;

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::valueByIndex( const std::size_t index ) const;

    template<>
    std::size_t NumberSet<std::size_t>::valueByFactor( const double factor ) const;

    template<>
    std::size_t NumberSet<std::size_t>::maxValue() const;

    template<>
    std::size_t NumberSet<std::size_t>::randomized() const;

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::getRandom() const;

    template<>
    NumberSet<std::size_t> NumberSet<std::size_t>::filter( const std::size_t lower, const std::size_t upper ) const;

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::getRandom( const std::size_t lower, const std::size_t upper ) const;

    using SizeTSet = NumberSet<std::size_t>;


    /// ================================================


    class Parameter {
    public:

        Parameter() {
        }

    };


    /// ================================================


    class ParametersMap {
    public:

        using Data = std::map< std::string, std::string >;


    protected:

        Data data;


    public:

        ParametersMap() {
        }

        const Data& rawData() const {
            return data;
        }

        void appendData( const Data& parameters );

        void add( const std::string& name, const std::string& value );

        void add( const std::string& name, const std::size_t value );

        void add( const ParameterName parameter, const std::string& value );

        void add( const ParameterName parameter, const std::size_t value );

        bool isSet( const std::string& parameter ) const;

        bool isSet( const ParameterName parameter ) const;

        /// returns raw parameter string
        Optional< std::string > getRawString( const std::string& parameter ) const;

        std::string getRawString( const ParameterName parameter, const std::string& defaultValue ) const;

        /// handles lists of strings separated by comma
        Optional< std::string > getString( const std::string& parameter ) const;

        /// handles lists of strings separated by comma
        Optional< std::string > getString( const ParameterName parameter ) const;

        /// handles lists of strings separated by comma
        std::string getString( const std::string& parameter, const std::string& defaultValue ) const;

        /// handles lists of strings separated by comma
        std::string getString( const ParameterName parameter, const std::string& defaultValue ) const;

        Optional< std::size_t > getSizeT( const std::string& parameter ) const;

        Optional< std::size_t > getSizeT( const ParameterName parameter ) const;

        /// handles list of values and ranges
        std::size_t getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const;

        std::size_t getSizeT( const ParameterName parameter, const std::string& defaultValue ) const;

        Optional< SizeTSet > getSizeTSet( const std::string& parameter ) const;

        Optional< SizeTSet > getSizeTSet( const ParameterName parameter ) const {
            const std::string parameterName = getParameterName( parameter );
            return getSizeTSet( parameterName );
        }

        SizeTSet getSizeTSet( const ParameterName parameter, const std::size_t defaultMinValue, const std::size_t defaultMaxValue ) const {
            const std::string parameterName = getParameterName( parameter );
            Optional< SizeTSet > retSet = getSizeTSet( parameterName );
            if ( retSet.has_value() ) {
                return retSet.value();
            }
            SizeTSet defaultSet;
            defaultSet.add( defaultMinValue, defaultMaxValue );
            return defaultSet;
        }

        SizeTSet getSizeTSet( const ParameterName parameter, const std::string& defaultValue ) const;

        std::string printData() const;

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_ */
