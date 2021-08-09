/*
 * Parameter.cpp
 *
 */

#include "dkmage/Parameter.h"

#include "utils/Log.h"


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

    Optional< std::string > ParametersMap::getString( const std::string& parameter ) const {
        auto iter = data.find( parameter );
        if ( iter == data.end() ) {
            return {};
        }
        return iter->second;
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
        const std::string& value = iter->second;
        if ( value.empty() == false ) {
            return value;
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
        const int number = stoi( value );
        return (std::size_t) number;
    }

    Optional< std::size_t > ParametersMap::getSizeT( const ParameterName parameter ) const {
        const std::string parameterName = getParameterName( parameter );
        return getSizeT( parameterName );
    }

    std::size_t ParametersMap::getSizeT( const std::string& parameter, const std::size_t defaultValue ) const {
        Optional< std::string > rawData = getString( parameter );
        if ( rawData.has_value() == false ) {
            return defaultValue;
        }
        const std::string& value = rawData.value();
        const int number = stoi( value );
        return (std::size_t) number;
    }

    std::size_t ParametersMap::getSizeT( const ParameterName parameter, const std::size_t defaultValue ) const {
        const std::string parameterName = getParameterName( parameter );
        return getSizeT( parameterName, defaultValue );
    }

} /* namespace dkmage */
