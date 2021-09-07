/*
 * Parameter.cpp
 *
 */

#include "dkmage/Parameter.h"

#include "utils/Rand.h"
#include "utils/Log.h"

#include <vector>
#include <string.h>


using namespace utils;


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

            data.insert( ParameterName::PN_ACTION_POINTS_LIMIT );

            data.insert( ParameterName::PN_SCRIPT_TUNNELLERS_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_PARTIES_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_VALUES_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_IF_CONDS_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_PARTY_DEFS_LIMIT );

            data.insert( ParameterName::PN_GOLD_SLABS_NUMBER );
            data.insert( ParameterName::PN_GEM_SLABS_NUMBER );
            data.insert( ParameterName::PN_GEM_FACES_NUMBER );
            data.insert( ParameterName::PN_GEM_TRAPS_NUMBER );
            data.insert( ParameterName::PN_GEM_GUARD_NUMBER );
            data.insert( ParameterName::PN_GEM_GUARD_LEVEL );
            data.insert( ParameterName::PN_INIT_GOLD_AMOUNT );
            data.insert( ParameterName::PN_CREATURES_LIMIT );

            data.insert( ParameterName::PN_ENEMY_KEEPER_ATTITUDE );
            data.insert( ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER );
            data.insert( ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT );

            data.insert( ParameterName::PN_CENTAL_CAVERNS_NUMBER );
            data.insert( ParameterName::PN_CAVERN_CREATURES_NUMBER );
            data.insert( ParameterName::PN_CAVERN_CREATURES_LEVEL );

            data.insert( ParameterName::PN_ENTRANCES_NUMBER );
            data.insert( ParameterName::PN_BRIDGE_GUARD_LEVEL );
            data.insert( ParameterName::PN_CORRIDOR_GUARD_LEVEL );
            data.insert( ParameterName::PN_TORTURE_GUARD_LEVEL );
            data.insert( ParameterName::PN_GRAVEYARD_GUARD_LEVEL );
            data.insert( ParameterName::PN_DUNGEON_HEADER_GUARD_LEVEL );

            data.insert( ParameterName::PN_TEST_MODE );
            data.insert( ParameterName::PN_STOP_ON_FAIL );
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

            data.insert( ParameterName::PN_STOP_ON_FAIL );
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

        case ParameterName::PN_ACTION_POINTS_LIMIT:   return "action_points_limit";

        case ParameterName::PN_SCRIPT_TUNNELLERS_LIMIT:   return "script_tunnellers_limit";
        case ParameterName::PN_SCRIPT_PARTIES_LIMIT:      return "script_parties_limit";
        case ParameterName::PN_SCRIPT_VALUES_LIMIT:       return "script_values_limit";
        case ParameterName::PN_SCRIPT_IF_CONDS_LIMIT:     return "script_if_conds_limit";
        case ParameterName::PN_SCRIPT_PARTY_DEFS_LIMIT:   return "script_party_defs_limit";

        case ParameterName::PN_GOLD_SLABS_NUMBER:   return "gold_slabs_number";
        case ParameterName::PN_GEM_SLABS_NUMBER:    return "gem_slabs_number";
        case ParameterName::PN_GEM_FACES_NUMBER:    return "gem_faces_number";
        case ParameterName::PN_GEM_TRAPS_NUMBER:    return "gem_traps_number";
        case ParameterName::PN_GEM_GUARD_NUMBER:    return "gem_guard_number";
        case ParameterName::PN_GEM_GUARD_LEVEL:     return "gem_guard_level";
        case ParameterName::PN_INIT_GOLD_AMOUNT:    return "init_gold_amount";
        case ParameterName::PN_CREATURES_LIMIT:     return "creatures_limit";

        case ParameterName::PN_ENEMY_KEEPER_ATTITUDE:           return "enemy_keeper_attitude";
        case ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER:       return "enemy_keeper_rooms_number";
        case ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT:    return "enemy_keeper_creatures_limit";

        case ParameterName::PN_CENTAL_CAVERNS_NUMBER:    return "cental_caverns_number";
        case ParameterName::PN_CAVERN_CREATURES_NUMBER:  return "cavern_creatures_number";
        case ParameterName::PN_CAVERN_CREATURES_LEVEL:   return "cavern_creatures_level";

        case ParameterName::PN_ENTRANCES_NUMBER:            return "entrances_number";
        case ParameterName::PN_BRIDGE_GUARD_LEVEL:          return "bridge_guard_level";
        case ParameterName::PN_CORRIDOR_GUARD_LEVEL:        return "corridor_guard_level";
        case ParameterName::PN_TORTURE_GUARD_LEVEL:         return "torture_guard_level";
        case ParameterName::PN_GRAVEYARD_GUARD_LEVEL:       return "graveyard_guard_level";
        case ParameterName::PN_DUNGEON_HEADER_GUARD_LEVEL:  return "dungeon_header_guard_level";

        case ParameterName::PN_TEST_MODE:       return "test_mode";
        case ParameterName::PN_TEST_ADDONS:     return "test_addons";
        case ParameterName::PN_STOP_ON_FAIL:    return "stop_on_fail";
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


    /// ==========================================================================================


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
//                LOG() << stream.str();
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
                LOG() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            return { (std::size_t)number };
        }
        case 2: {
            const int numberA = stoi( stringList[0] );
            if ( numberA < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- lower range have not be negative, passed: " << rawData;
                LOG() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            const int numberB = stoi( stringList[1] );
            if ( numberB < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- upper range have not be negative, passed: " << rawData;
                LOG() << stream.str();
                throw std::invalid_argument( stream.str() );
            }
            const int diff = numberB - numberA;
            if ( diff < 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid range -- lower range have to be not greater than upper range, passed: " << rawData;
                LOG() << stream.str();
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
            LOG() << stream.str();
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

    Optional< SizeTSet > ParametersMap::getSizeTSet( const std::string& parameter ) const {
        Optional< std::string > rawData = getRawString( parameter );
        if ( rawData.has_value() == false ) {
            return {};
        }
        const std::string& value = rawData.value();
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

    std::string ParametersMap::print() const {
        std::stringstream stream;
        auto iter  = data.begin();
        auto eiter = data.end();
        for (; iter != eiter; ++iter ) {
            stream << "\n    " << iter->first << " = " << iter->second;
        }
        return stream.str();
    }

} /* namespace dkmage */
