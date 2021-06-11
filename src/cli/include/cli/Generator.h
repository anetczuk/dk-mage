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
    class GeneratorInstance {

        std::string genCode;


    public:

        GeneratorInstance( const std::string& code ): genCode( code ) {
        }

        virtual ~GeneratorInstance() {
        }

        const std::string& code() const {
            return genCode;
        }

        virtual void generate( const int seed ) = 0;

    };


    /**
     *
     */
    class Generator {
    private:

        typedef std::unique_ptr<GeneratorInstance> GeneratorInstancePtr;

        std::vector< GeneratorInstancePtr > genMap;


    public:

        std::vector< std::string > generatorsList() const;

        void generate( const std::string& type, const std::string& seed );

        static Generator& instance();


    private:

        Generator();

        template <typename T>
        void add( const std::string& code ) {
            genMap.push_back( GeneratorInstancePtr( new T(code) ) );
        }

        GeneratorInstance* find( const std::string& code );

    };

} /* namespace cli */

#endif /* CLI_INCLUDE_CLI_GENERATOR_H_ */
