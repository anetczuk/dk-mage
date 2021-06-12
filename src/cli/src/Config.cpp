/*
 * Config.cpp
 *
 */

#include "cli/Config.h"

#include "utils/Log.h"

#include "inih/INIReader.h"


namespace cli {

    Config::Config( const std::string& configPath ): configPath( configPath ) {
    }

    std::string Config::readFieldString( const std::string& section, const std::string& field ) const {
        if ( configPath.empty() ) {
            return "";
        }

        INIReader reader( configPath );
        if (reader.ParseError() != 0) {
            const std::string message = std::string( "Can't load '" ) + configPath + "'";
            throw std::runtime_error( message );
        }

        const std::string fieldData = reader.Get(section, field, "");
        if ( fieldData.empty() ) {
            const std::string message = std::string( "Can't read '" ) + field + "' field in '" + section + "' section";
            throw std::runtime_error( message );
        }
        return fieldData;
    }

} /* namespace cli */
