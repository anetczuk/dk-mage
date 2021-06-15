/*
 * Config.h
 *
 */

#ifndef CLI_INCLUDE_CLI_CONFIG_H_
#define CLI_INCLUDE_CLI_CONFIG_H_

#include <string>


namespace cli {

    /**
     *
     */
    class Config {

        std::string configPath;


    public:

        Config( const std::string& configPath = "" );

        std::string readDataPath() const {
            return readFieldString( "data", "data_path" );
        }

        std::string readLevelsPath() const {
            return readFieldString( "data", "levels_path" );
        }


    private:

        std::string readFieldString( const std::string& section, const std::string& field ) const;

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_CONFIG_H_ */