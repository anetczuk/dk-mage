/*
 * Parameter.cpp
 *
 */

#include "dkmage/Parameter.h"

#include "utils/Log.h"

#include <vector>
#include <string.h>


namespace dkmage {

    const std::set< ParameterName >& getAllParameterNames() {
        static std::set< ParameterName > data;
        if ( data.empty() ) {
            data.insert( ParameterName::PN_CONFIG );
            data.insert( ParameterName::PN_DATA_PATH );
            data.insert( ParameterName::PN_LEVELS_PATH );
            data.insert( ParameterName::PN_OUTPUT_PATH );
            data.insert( ParameterName::PN_OUTPUT_SUBPATH );
            data.insert( ParameterName::PN_OUTPUT_ID );
            data.insert( ParameterName::PN_OUTPUT_BMP );

            data.insert( ParameterName::PN_SEED );
            data.insert( ParameterName::PN_TYPE );
            data.insert( ParameterName::PN_GOLD_SLABS_NUMBER );
            data.insert( ParameterName::PN_GEM_FACES_NUMBER );
            data.insert( ParameterName::PN_INIT_GOLD_AMOUNT );

            data.insert( ParameterName::PN_TEST_MODE );
        }
        return data;
    }

    const std::set< ParameterName >& getLocalParameterNames() {
        static std::set< ParameterName > data;
        if ( data.empty() ) {
            data.insert( ParameterName::PN_CONFIG );
            data.insert( ParameterName::PN_DATA_PATH );
            data.insert( ParameterName::PN_LEVELS_PATH );
            data.insert( ParameterName::PN_OUTPUT_PATH );
            data.insert( ParameterName::PN_OUTPUT_SUBPATH );
            data.insert( ParameterName::PN_OUTPUT_ID );
            data.insert( ParameterName::PN_OUTPUT_BMP );
        }
        return data;
    }

    std::string getParameterName( const ParameterName parameter ) {
        switch( parameter ) {
        case ParameterName::PN_CONFIG:          return "config";
        case ParameterName::PN_DATA_PATH:       return "data_path";
        case ParameterName::PN_LEVELS_PATH:     return "levels_path";
        case ParameterName::PN_OUTPUT_PATH:     return "output_path";
        case ParameterName::PN_OUTPUT_SUBPATH:  return "output_subpath";
        case ParameterName::PN_OUTPUT_ID:       return "output_id";
        case ParameterName::PN_OUTPUT_BMP:      return "output_bmp";

        case ParameterName::PN_SEED:                return "seed";
        case ParameterName::PN_TYPE:                return "type";
        case ParameterName::PN_GOLD_SLABS_NUMBER:   return "gold_slabs_number";
        case ParameterName::PN_GEM_FACES_NUMBER:    return "gem_faces_number";
        case ParameterName::PN_INIT_GOLD_AMOUNT:    return "init_gold_amount";

        case ParameterName::PN_TEST_MODE:   return "test_mode";
        }

        std::stringstream stream;
        stream << FILE_NAME << ": invalid argument -- unavailable player: " << (int)parameter;
        LOG() << stream.str();
        throw std::invalid_argument( stream.str() );
    }

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters ) {
        std::set< std::string > ret;
        for ( const auto item: parameters ) {
            const std::string name = getParameterName( item );
            ret.insert( name );
        }
        return ret;
    }

    /// ==============================

    Parameter::Parameter() {
    }

    /// ==============================

    static std::vector< std::string > parseList( const std::string& rawData, const char* delimiter ) {
        std::vector< std::string > list;
        char* dup   = strdup( rawData.c_str() );
        char* token = strtok( dup, delimiter );             /// note: function is not thread safe nor reentrant, so running in parallel unsupported
        while ( token != NULL ) {
            list.push_back( token );
            token = strtok( NULL, delimiter );
        }
        free(dup);

//        char* rest = str;
//
//        while ((token = strtok_r(rest, " ", &rest)))
//            printf("%s\n", token);

        return list;
    }

    static std::string randomFromList( const std::vector< std::string >& stringList ) {
        const std::size_t listSize = stringList.size();
        switch( listSize ) {
        case 0: return "";
        case 1: return stringList[0];
        }
        const std::size_t listIndex = rand() % listSize;
        const std::string& listItem = stringList[ listIndex ];
        return listItem;
    }

    /// 'rawData' is single number or range in format '{num}:{num}'
    static std::vector< int > handleRange( const std::string& rawData ) {
        const std::vector< std::string > stringList = parseList( rawData, ":" );
        const std::string listItem = randomFromList( stringList );
        if ( listItem.empty() ) {
            return {};
        }
        const int number = stoi( listItem );
        return { number };
    }

    /// parse unsigned number or unsigned range
    static int parseUnsigned( const std::string& parameter, const std::string& rawData ) {
        const std::vector< int > range = handleRange( rawData );
        const std::size_t rSize = range.size();
        if ( rSize == 1 ) {
            return range[ 0 ];
        }
        if ( range.size() != 2 ) {
            std::stringstream stream;
            stream << FILE_NAME << ": parameter " << parameter << " invalid range -- format not match {number}:{number}, passed: " << rawData;
            LOG() << stream.str();
            throw std::invalid_argument( stream.str() );
        }
        const int rangeFrom = range[ 0 ];
        if ( rangeFrom < 0 ) {
            std::stringstream stream;
            stream << FILE_NAME << ": parameter " << parameter << " invalid range -- lower range have not be negative, passed: " << rawData;
            LOG() << stream.str();
            throw std::invalid_argument( stream.str() );
        }
        const int rangeTo   = range[ 1 ];
        if ( rangeTo < 0 ) {
            std::stringstream stream;
            stream << FILE_NAME << ": parameter " << parameter << " invalid range -- upper range have not be negative, passed: " << rawData;
            LOG() << stream.str();
            throw std::invalid_argument( stream.str() );
        }
        const int diff = rangeTo - rangeFrom;
        if ( diff < 0 ) {
            std::stringstream stream;
            stream << FILE_NAME << ": parameter " << parameter << " invalid range -- lower range have to be not greater than upper range, passed: " << rawData;
            LOG() << stream.str();
            throw std::invalid_argument( stream.str() );
        }
        if ( diff == 0 ) {
            return (std::size_t) rangeFrom;
        }
        const int number = rangeFrom + rand() % ( diff + 1 );
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

    std::size_t ParametersMap::getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        Optional< std::string > rawData = getString( parameterName );
        if ( rawData.has_value() == false ) {
            return defaultValue;
        }

        const std::string& value = rawData.value();
        const int num = parseUnsigned( parameterName, value );
        return (std::size_t) num;
    }

} /* namespace dkmage */
