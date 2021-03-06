/*
 * Parameter.cpp
 *
 */

#include "dkmage/Parameter.h"

#include "adiktedpp/Rand.h"

#include "utils/Log.h"

#include <vector>
#include <string.h>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {

    template<>
    std::size_t NumberSet<std::size_t>::size() const {
        std::size_t iSize = data.size();
        for ( const NumberRange<std::size_t>& item: data ) {
            iSize += item.size();
        }
        return iSize;
    }

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::valueByIndex( const std::size_t index ) const {
        std::size_t currIndex = index;
        for ( const NumberRange<std::size_t>& item: data ) {
            const std::size_t currSize = item.size() + 1;
            if ( currIndex >= currSize ) {
                currIndex -= currSize;
                continue ;
            }
            return item.item( currIndex );
        }
        return {};
    }

    template<>
    std::size_t NumberSet<std::size_t>::valueByFactor( const double factor ) const {
        const std::size_t iSize = size();
        if ( iSize < 1 ) {
            return std::size_t();
        }
        if ( iSize == 1 ) {
            Optional<std::size_t> item = valueByIndex( 0 );
            if ( item.has_value() == false ) {
                return std::size_t();
            }
            return item.value();
        }

        const int index = factor * iSize;

        if ( index < 0 ) {
            Optional<std::size_t> item = valueByIndex( 0 );
            if ( item.has_value() == false ) {
                return std::size_t();
            }
            return item.value();
        }

        const std::size_t uIndex = (std::size_t) index;
        if ( uIndex >= iSize ) {
            Optional<std::size_t> item = valueByIndex( iSize - 1 );
            if ( item.has_value() == false ) {
                return std::size_t();
            }
            return item.value();
        }

        Optional<std::size_t> item = valueByIndex( uIndex );
        if ( item.has_value() == false ) {
            return std::size_t();
        }
        return item.value();
    }

    template<>
    std::size_t NumberSet<std::size_t>::maxValue() const {
        const std::size_t setSize = size();
        if ( setSize < 1 ) {
            return std::size_t();
        }
        const NumberRange<std::size_t>& item = data[0];
        std::size_t ret = item.item(0);
        if ( setSize == 1 ) {
            return ret;
        }
        for ( const NumberRange<std::size_t>& item: data ) {
            const std::size_t itemMax = item.maxValue();
            ret = std::max( ret, itemMax );
        }
        return ret;
    }

    template<>
    std::size_t NumberSet<std::size_t>::randomized() const {
        const std::size_t setSize = size();
        if ( setSize < 1 ) {
            return std::size_t();
        }
        if ( setSize == 1 ) {
            const NumberRange<std::size_t>& item = data[0];
            return item.item(0);
        }
        std::size_t index = rng_randi( setSize );
        for ( const NumberRange<std::size_t>& item: data ) {
            const std::size_t iSize = item.size() + 1;
            if ( index < iSize ) {
                return item.item( index );
            }
            index -= iSize;
        }
        return std::size_t();
    }

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::getRandom() const {
        const std::size_t setSize = size();
        if ( setSize < 1 ) {
            return {};
        }
        if ( setSize == 1 ) {
            const NumberRange<std::size_t>& item = data[0];
            return item.item(0);
        }
        std::size_t index = rng_randi( setSize );
        for ( const NumberRange<std::size_t>& item: data ) {
            const std::size_t iSize = item.size() + 1;
            if ( index < iSize ) {
                return item.item( index );
            }
            index -= iSize;
        }
        return {};
    }

    template<>
    NumberSet<std::size_t> NumberSet<std::size_t>::filter( const std::size_t lower, const std::size_t upper ) const {
        NumberSet<std::size_t> ret;
        for ( const NumberRange<std::size_t>& item: data ) {
            if ( item.upperBound < lower ) {
                continue ;
            }
            if ( item.lowerBound > upper ) {
                continue ;
            }
            const std::size_t rLower = std::max( lower, item.lowerBound );
            const std::size_t rUpper = std::min( upper, item.upperBound );
            ret.add( rLower, rUpper );
        }
        return ret;
    }

    template<>
    Optional<std::size_t> NumberSet<std::size_t>::getRandom( const std::size_t lower, const std::size_t upper ) const {
        NumberSet<std::size_t> filtered = filter( lower, upper );
        return filtered.getRandom();
    }


    /// ==========================================================================================


    static std::vector< std::string > parseList( const std::string& rawData, const char* delimiter ) {
        std::vector< std::string > list;

        std::size_t startPos = 0;
        std::size_t delimPos = 0;
        while ( ( delimPos = rawData.find_first_of( delimiter, startPos ) ) != std::string::npos ) {
            delimPos = rawData.find_first_of( delimiter, startPos );
            const std::size_t length = delimPos - startPos;
            if ( length > 0 ) {
                list.push_back( rawData.substr( startPos, length ) );
            }
            startPos = delimPos + 1;
        }
        /// no more delimiters -- add last item
        list.push_back( rawData.substr( startPos ) );

        /// old implementation
//        char* dup   = strdup( rawData.c_str() );
//        char* token = strtok( dup, delimiter );             /// note: function is not thread safe nor reentrant, so running in parallel unsupported
//        while ( token != NULL ) {
//            list.push_back( token );
//            token = strtok( NULL, delimiter );
//        }
//        free(dup);

        return list;
    }

    static std::string randomFromList( const std::vector< std::string >& stringList ) {
        const std::size_t listSize = stringList.size();
        switch( listSize ) {
        case 0: return "";
        case 1: return stringList[0];
        }
        const std::size_t listIndex = rng_randi( listSize );
        const std::string& listItem = stringList[ listIndex ];
        return listItem;
    }

//    /// 'rawData' is single number or range in format '{num}:{num}'
//    static std::vector< int > handleRangeInt( const std::string& rawData ) {
//        const std::vector< std::string > stringList = parseList( rawData, ":" );
//
//        const std::size_t listSize = stringList.size();
//        switch( listSize ) {
//        case 0: return {};
//        case 1: {
//            const int number = stoi( stringList[0] );
//            return { number };
//        }
//        case 2: {
//            const int numberA = stoi( stringList[0] );
//            const int numberB = stoi( stringList[1] );
//            const int diff = numberB - numberA;
//            if ( diff < 0 ) {
//                std::stringstream stream;
//                stream << FILE_NAME << ": invalid range -- lower range have to be not greater than upper range, passed: " << rawData;
//                LOG_INFO() << stream.str();
//                throw std::invalid_argument( stream.str() );
//            }
//            return { numberA, numberB };
//        }
//        }
//
//        return {};
//    }

    /// 'rawData' is single number or range in format '{num}:{num}'
    static std::vector< std::size_t > handleRangeUnsigned( const std::string& rawData ) {
        const std::vector< std::string > stringList = parseList( rawData, ":" );

        const std::size_t listSize = stringList.size();
        switch( listSize ) {
        case 0: return {};
        case 1: {
            const int number = stoi( stringList[0] );
            if ( number < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- number have not be negative, passed: " << rawData;
                LOG_INFO() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            return { (std::size_t)number };
        }
        case 2: {
            const int numberA = stoi( stringList[0] );
            if ( numberA < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- lower range have not be negative, passed: " << rawData;
                LOG_INFO() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            const int numberB = stoi( stringList[1] );
            if ( numberB < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- upper range have not be negative, passed: " << rawData;
                LOG_INFO() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            const int diff = numberB - numberA;
            if ( diff < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- lower range have to be not greater than upper range, passed: " << rawData;
                LOG_INFO() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            return { (std::size_t)numberA, (std::size_t)numberB };
        }
        }

        return {};
    }

    /// parse unsigned number or unsigned range
    static int parseUnsigned( const std::string& parameter, const std::string& rawData ) {
        const std::vector< std::size_t > range = handleRangeUnsigned( rawData );
        const std::size_t rSize = range.size();
        if ( rSize == 1 ) {
            return range[ 0 ];
        }
        if ( range.size() != 2 ) {
            std::stringstream stream;
            stream << FILE_NAME << ": parameter " << parameter << " invalid range -- format not match {number}:{number}, passed: " << rawData;
            LOG_INFO() << stream.str();
            throw std::invalid_argument( stream.str() );
        }
        const int rangeFrom = range[ 0 ];
        const int rangeTo   = range[ 1 ];
        const int diff = rangeTo - rangeFrom;
        if ( diff == 0 ) {
            return (std::size_t) rangeFrom;
        }
        const int number = rangeFrom + rng_randi( diff + 1 );
        return (std::size_t) number;
    }

    void ParametersMap::appendData( const Data& parameters ) {
        auto iter  = parameters.begin();
        auto eiter = parameters.end();
        for ( ; iter != eiter; ++iter ) {
            data[ iter->first ] = iter->second;
        }
    }

    void ParametersMap::add( const std::string& name, const std::string& value ) {
        data[ name ] = value;
    }

    void ParametersMap::add( const std::string& name, const std::size_t value ) {
        data[ name ] = std::to_string( value );
    }

    void ParametersMap::add( const ParameterName parameter, const std::string& value ) {
        const std::string parameterName = getParameterName( parameter );
        data[ parameterName ] = value;
    }

    void ParametersMap::add( const ParameterName parameter, const std::size_t value ) {
        const std::string parameterName = getParameterName( parameter );
        data[ parameterName ] = std::to_string( value );
    }

    bool ParametersMap::isSet( const std::string& parameter ) const {
        auto iter = data.find( parameter );
        if ( iter == data.end() ) {
            return false;
        }
        return true;
    }

    bool ParametersMap::isSet( const ParameterName parameter ) const {
        const std::string parameterName = getParameterName( parameter );
        return isSet( parameterName );
    }

    Optional< std::string > ParametersMap::getRawString( const std::string& parameter ) const {
        auto iter = data.find( parameter );
        if ( iter == data.end() ) {
            return {};
        }
        return iter->second;
    }

    std::string ParametersMap::getRawString( const ParameterName parameter, const std::string& defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        auto iter = data.find( parameterName );
        if ( iter == data.end() ) {
            return defaultValue;
        }
        if ( iter->second.empty() == false ) {
            return iter->second;
        }
        return defaultValue;
    }

    Optional< std::string > ParametersMap::getString( const std::string& parameter ) const {
        Optional< std::string > rawData = getRawString( parameter );
        if ( rawData.has_value() == false ) {
            return {};
        }
        const std::string& value = rawData.value();
        const std::vector< std::string > stringList = parseList( value, "," );
        const std::string listItem = randomFromList( stringList );
        if ( listItem.empty() ) {
            return {};
        }
        return listItem;
    }

    Optional< std::string > ParametersMap::getString( const ParameterName parameter ) const {
        const std::string parameterName = getParameterName( parameter );
        return getString( parameterName );
    }

    std::string ParametersMap::getString( const std::string& parameter, const std::string& defaultValue ) const {
        auto iter = data.find( parameter );
        if ( iter == data.end() ) {
            return defaultValue;
        }
        const std::vector< std::string > stringList = parseList( iter->second, "," );
        const std::string listItem = randomFromList( stringList );
        if ( listItem.empty() == false ) {
            return listItem;
        }
        return defaultValue;
    }

    std::string ParametersMap::getString( const ParameterName parameter, const std::string& defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        return getString( parameterName, defaultValue );
    }

    Optional< std::size_t > ParametersMap::getSizeT( const std::string& parameter ) const {
        Optional< std::string > rawData = getString( parameter );
        if ( rawData.has_value() == false ) {
            return {};
        }
        const std::string& value = rawData.value();
        const int num = parseUnsigned( parameter, value );
        return (std::size_t) num;
    }

    Optional< std::size_t > ParametersMap::getSizeT( const ParameterName parameter ) const {
        const std::string parameterName = getParameterName( parameter );
        return getSizeT( parameterName );
    }

    SizeTSet convert_SizeTSet( const std::string& value ) {
        const std::vector< std::string > stringList = parseList( value, "," );

        SizeTSet ret;
        for ( const std::string& item: stringList ) {
            const std::vector< std::size_t > range = handleRangeUnsigned( item );
            const std::size_t rSize = range.size();
            if ( rSize == 1 ) {
                ret.add( range[0] );
                continue ;
            }
            if ( rSize == 2 ) {
                ret.add( range[0], range[1] );
                continue ;
            }
        }

        return { ret };
    }

    Optional< SizeTSet > ParametersMap::getSizeTSet( const std::string& parameter ) const {
        Optional< std::string > rawData = getRawString( parameter );
        if ( rawData.has_value() == false ) {
            return {};
        }
        const std::string& value = rawData.value();
        return convert_SizeTSet( value );
    }

    SizeTSet ParametersMap::getSizeTSet( const ParameterName parameter, const std::string& defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        Optional< SizeTSet > retSet = getSizeTSet( parameterName );
        if ( retSet.has_value() ) {
            return retSet.value();
        }
        return convert_SizeTSet( defaultValue );
    }

    std::size_t ParametersMap::getSizeT( const std::string& parameter, const std::string& defaultValue ) const {
        Optional< std::string > rawData = getString( parameter );
        if ( rawData.has_value() == false ) {
            const int num = parseUnsigned( parameter, defaultValue );
            return (std::size_t) num;
        }

        const std::string& value = rawData.value();
        const int num = parseUnsigned( parameter, value );
        return (std::size_t) num;
    }

    std::size_t ParametersMap::getSizeT( const std::string& parameter, const std::size_t defaultValue ) const {
        Optional< std::string > rawData = getString( parameter );
        if ( rawData.has_value() == false ) {
            return defaultValue;
        }

        const std::string& value = rawData.value();
        const int num = parseUnsigned( parameter, value );
        return (std::size_t) num;
    }

    std::size_t ParametersMap::getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        return getSizeT( parameterName, defaultValue );
    }

    std::size_t ParametersMap::getSizeT( const ParameterName parameter, const std::string& defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        return getSizeT( parameterName, defaultValue );
    }

    std::string ParametersMap::printData() const {
        std::stringstream stream;
        auto iter  = data.begin();
        auto eiter = data.end();
        for (; iter != eiter; ++iter ) {
            stream << "\n    " << iter->first << " = " << iter->second;
        }
        return stream.str();
    }

} /* namespace dkmage */
