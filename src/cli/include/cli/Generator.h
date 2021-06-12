/*
 * Generator.h
 *
 */

#ifndef CLI_INCLUDE_CLI_GENERATOR_H_
#define CLI_INCLUDE_CLI_GENERATOR_H_

#include "dkmage/LevelGenerator.h"

#include <set>
#include <vector>
#include <map>
#include <memory>
#include <string>


namespace cli {

    /**
     *
     */
    class Generator {
    private:

        typedef std::unique_ptr<dkmage::LevelGenerator> LevelGeneratorPtr;

        std::map< std::string, LevelGeneratorPtr > genMap;


    public:

        std::vector< std::string > generatorsList() const;

        dkmage::LevelGenerator* get( const std::string& type );

        static Generator& instance();


    private:

        Generator();

        template <typename T>
        void add( const std::string& code ) {
            genMap[ code ].reset( new T() );
        }

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_GENERATOR_H_ */
