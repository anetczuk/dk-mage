/*
 * Generator.cpp
 *
 */

#include "cli/Generator.h"

#include "dkmage/mode/Cave.h"

#include "utils/Log.h"

#include <functional>                               /// std::hash
#include <random>
#include <time.h>                                   /// time


namespace cli {

    /**
     *
     */
    class CaveMode: public LevelGenerator {
    public:

        dkmage::mode::Cave generator;


        CaveMode( const std::string& code ): LevelGenerator( code ) {
        }

        void generate( const std::size_t seed ) override {
            generator.generate( seed );
        }

        void storeLevel( const std::string& dirPath ) override {
            generator.saveLevel( dirPath );
        }

        void storePreview( const std::string& filePath ) override {
            generator.saveBmp( filePath );
        }

    };


    /// ===============================================================


    static std::string genSeed( const std::size_t length = 8 ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = rand() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }


    /// ===============================================================


    void LevelGenerator::generate( const std::string& seed ) {
        if ( seed.empty() ) {
            const unsigned int timeSeed = time(NULL);
            srand( timeSeed );
            const std::string newSeed = genSeed();
            if ( newSeed.empty() ) {
                LOG() << "unable to generate seed";
                return ;
            }
            generate( newSeed );
            return ;
        }

        LOG() << "generating map '" << genCode << "' with seed '" << seed << "'";
        const std::size_t seedValue = std::hash< std::string >{}( seed );
        generate( seedValue );
    }


    /// ===============================================================


    std::vector< std::string > Generator::generatorsList() const {
        std::vector< std::string > list;
        for ( const auto& item: genMap ) {
            const std::string& code = item->code();
            list.push_back( code );
        }
        return list;
    }

    LevelGenerator* Generator::get( const std::string& type ) {
        for ( const auto& item: genMap ) {
            const std::string& itemCode = item->code();
            if ( itemCode == type ) {
                return item.get();
            }
        }
        return nullptr;
    }

    Generator& Generator::instance() {
        static Generator generator;
        return generator;
    }

    Generator::Generator() {
        add<CaveMode>( "cave" );
    }

} /* namespace cli */
