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

        std::string readLevelsPath() const;

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_CONFIG_H_ */
