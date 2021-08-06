/*
 * Config.h
 *
 */

#ifndef CLI_INCLUDE_CLI_CONFIG_H_
#define CLI_INCLUDE_CLI_CONFIG_H_

#include <string>
#include <map>


namespace cli {

    /**
     *
     */
    class Config {
    public:

        using RawData = std::map< std::string, std::string >;


    private:

        std::string configPath;


    public:

        Config( const std::string& configPath = "" );

        bool isValid() const;

        std::string readDataPath() const {
            return readFieldString( "general", "data_path" );
        }

        std::string readLevelsPath() const {
            return readFieldString( "general", "levels_path", true );
        }

        std::string readSeed() const {
            return readFieldString( "general", "seed", true );
        }

        RawData readSection( const std::string& section ) const;


    private:

        std::string readFieldString( const std::string& section, const std::string& field, const bool allowEmpty = false ) const;

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_CONFIG_H_ */
