/*
 * Generator.h
 *
 */

#ifndef CLI_INCLUDE_CLI_GENERATOR_H_
#define CLI_INCLUDE_CLI_GENERATOR_H_

//#include <map>
#include <set>
#include <vector>
#include <memory>
#include <string>


namespace cli {

    /**
     *
     */
    class LevelGenerator {

        std::string genCode;


    public:

        LevelGenerator( const std::string& code ): genCode( code ) {
        }

        virtual ~LevelGenerator() {
        }

        const std::string& code() const {
            return genCode;
        }

        void generate( const std::string& seed );

        virtual void generate( const std::size_t seed ) = 0;

        /**
         * "levelPath" is path without extension, e.g. "map00333" or "path/to/map/file"
         * in both cases various files will be created by adding extension to "levelPath"
         */
        virtual void storeLevel( const std::string& levelPath ) = 0;

        virtual void storePreview( const std::string& filePath ) = 0;

    };


    /**
     *
     */
    class Generator {
    private:

        typedef std::unique_ptr<LevelGenerator> LevelGeneratorPtr;

        std::vector< LevelGeneratorPtr > genMap;


    public:

        std::vector< std::string > generatorsList() const;

        LevelGenerator* get( const std::string& type );

        static Generator& instance();


    private:

        Generator();

        template <typename T>
        void add( const std::string& code ) {
            genMap.push_back( LevelGeneratorPtr( new T(code) ) );
        }

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_GENERATOR_H_ */
