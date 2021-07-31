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

    bool Config::isValid() const {
        if ( configPath.empty() ) {
            return false;
        }

        INIReader reader( configPath );
        const int lineNum = reader.ParseError();
        if ( lineNum == 0) {
            return true;
        }
        if ( lineNum < 0 ) {
            LOG() << "can't open ini file";
            return false;
        }

        LOG() << "parse error at line " << lineNum;
        return false;
    }

    std::string Config::readFieldString( const std::string& section, const std::string& field, const bool allowEmpty ) const {
        if ( configPath.empty() ) {
            return "";
        }

        INIReader reader( configPath );
        if (reader.ParseError() != 0) {
            const std::string message = std::string( "Can't load '" ) + configPath + "'";
            throw std::runtime_error( message );
        }

        const std::string fieldData = reader.Get(section, field, "");
        if ( allowEmpty ) {
            return fieldData;
        }
        if ( fieldData.empty() ) {
            const std::string message = std::string( "Can't read '" ) + field + "' field in '" + section + "' section";
            throw std::runtime_error( message );
        }
        return fieldData;
    }

} /* namespace cli */
