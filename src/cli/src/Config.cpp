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

    std::string Config::readLevelsPath() const {
        if ( configPath.empty() ) {
            return "";
        }

        INIReader reader( configPath );
        if (reader.ParseError() != 0) {
            std::string message = std::string( "Can't load '" ) + configPath + "'";
            throw std::runtime_error( message );
            return "";
        }

        const std::string levelPath = reader.Get("data", "levels_path", "");
        if ( levelPath.empty() ) {
            throw std::runtime_error( "Can't read 'levels_path' field in 'data' section" );
        }
        return levelPath;
    }

} /* namespace cli */
