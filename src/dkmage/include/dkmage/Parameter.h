/*
 * Parameter.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_
#define DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_

/// inclusion: #include "dkmage/Parameter.h"

#include "utils/Rand.h"

#include <map>
#include <set>
#include <vector>


namespace dkmage {

    /**
     *
     */
    enum class ParameterName {
        PN_CONFIG,
        PN_DATA_PATH,
        PN_LEVELS_PATH,
        PN_OUTPUT_PATH,
        PN_OUTPUT_SUBPATH,
        PN_OUTPUT_ID,
        PN_OUTPUT_BMP,

        PN_SEED,
        PN_TYPE,
        PN_GOLD_SLABS_NUMBER,
        PN_GEM_FACES_NUMBER,
        PN_INIT_GOLD_AMOUNT,

        PN_ENTRANCES_NUMBER,

        PN_TEST_MODE,
        PN_STOP_ON_FAIL                         /// do not retry generation if attempt failed
    };

    const std::set< ParameterName >& getAllParameterNames();

    /**
     * User specific parameters (e.g. paths) that does not affect map content itself.
     */
    const std::set< ParameterName >& getLocalParameterNames();

    std::string getParameterName( const ParameterName parameter );

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters );


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

        NumberSet<TNumber> filter( const TNumber lower, const TNumber upper ) const;

        Optional<TNumber> getRandom() const;

        Optional<TNumber> getRandom( const TNumber lower, const TNumber upper ) const;

    };

    template<>
    std::size_t NumberSet<std::size_t>::size() const;

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

        Optional< SizeTSet > getSizeTSet( const std::string& parameter ) const;

        Optional< SizeTSet > getSizeTSet( const ParameterName parameter ) const {
            const std::string parameterName = getParameterName( parameter );
            return getSizeTSet( parameterName );
        }

        std::size_t getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const;

        std::string print() const;

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_PARAMETER_H_ */
