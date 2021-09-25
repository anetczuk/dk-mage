/*
 * Config.cpp
 *
 */

#include "cli/Config.h"

#include "utils/Log.h"

#include "inih/INIReader.h"

#include <vector>


namespace cli {

    class ExtendedINIReader: public INIReader {
    public:

        using Data = std::map< std::string, std::string >;


        using INIReader::INIReader;

        Data getSectionData( const std::string& section ) {
            Data ret;
            auto iter  = _values.begin();
            auto eiter = _values.end();
            for ( ; iter != eiter; ++iter ) {
                const std::string& key = iter->first;
                std::vector< std::string > data = explodeKey( key );
                if ( data.size() != 2 ) {
                    throw std::runtime_error( "invalid number of fields" );
                }
                if ( section.compare( data[0] ) != 0 ) {
                    /// bad section
                    continue;
                }
                /// section found
                const std::string& field = data[1];
                ret[ field ] = iter->second;
            }
            return ret;
        }

        //_values

        /// implementation taken from: http://www.cplusplus.com/articles/2wA0RXSz/
        static std::vector< std::string > explodeKey( const std::string& key) {
            std::string buff{""};
            std::vector< std::string > v;

            static const char c = '=';              /// separator

            for ( auto n: key) {
                if(n != c)
                    buff += n;
                else if(n == c && buff != "") {
                    v.push_back(buff);
                    buff = "";
                }
            }
            if(buff != "")
                v.push_back(buff);
            return v;
        }

    };

    /// ======================================================================

    Config::Config( const std::string& configPath ): configPath( configPath ) {
    }

    bool Config::isValid() const {
        if ( configPath.empty() ) {
            return false;
        }

        ExtendedINIReader reader( configPath );
        const int lineNum = reader.ParseError();
        if ( lineNum == 0) {
            return true;
        }
        if ( lineNum < 0 ) {
            LOG_INFO() << "can't open ini file";
            return false;
        }

        LOG_INFO() << "parse error at line " << lineNum;
        return false;
    }

    Config::RawData Config::readSection( const std::string& section ) const {
        Config::RawData data;

        ExtendedINIReader reader( configPath );
        if (reader.ParseError() != 0) {
            std::stringstream stream;
            stream << FILE_NAME << ": can't load '" << configPath << "'";
            throw std::runtime_error( stream.str() );
        }

        return reader.getSectionData( section );
    }

    std::string Config::readFieldString( const std::string& section, const std::string& field, const bool allowEmpty ) const {
        if ( configPath.empty() ) {
            return "";
        }

        ExtendedINIReader reader( configPath );
        if (reader.ParseError() != 0) {
            std::stringstream stream;
            stream << FILE_NAME << ": can't load '" << configPath << "'";
            throw std::runtime_error( stream.str() );
        }

        const std::string fieldData = reader.Get(section, field, "");
        if ( allowEmpty ) {
            return fieldData;
        }
        if ( fieldData.empty() ) {
            std::stringstream stream;
            stream << FILE_NAME << ": can't read '" << field << "' field in '" << section << "' section";
            throw std::runtime_error( stream.str() );
        }
        return fieldData;
    }

} /* namespace cli */
