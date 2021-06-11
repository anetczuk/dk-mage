/*
 * Generator.cpp
 *
 */

#include "cli/Generator.h"

#include "utils/Log.h"


namespace cli {

    class CaveMode: public GeneratorInstance {
    public:

        CaveMode( const std::string& code ): GeneratorInstance( code ) {
        }

        void generate( const int seed ) override {
            LOG() << "generating cave with seed " << seed;
        }

    };


    /// ===============================================================


    std::vector< std::string > Generator::generatorsList() const {
        std::vector< std::string > list;
        for ( const auto& item: genMap ) {
            const std::string& code = item->code();
            list.push_back( code );
        }
        return list;
    }

    void Generator::generate( const std::string& type, const std::string& seed ) {
        GeneratorInstance* generator = find( type );
        if ( generator == nullptr ) {
            LOG() << "unable to find generator " << type;
            return ;
        }
        LOG() << "generating map '" << type << "' with seed '" << seed << "'";
        generator->generate( 0 );
    }

    Generator& Generator::instance() {
        static Generator generator;
        return generator;
    }

    Generator::Generator() {
        add<CaveMode>( "cave" );
    }

    GeneratorInstance* Generator::find( const std::string& code ) {
        for ( const auto& item: genMap ) {
            const std::string& itemCode = item->code();
            if ( itemCode == code ) {
                return item.get();
            }
        }
        return nullptr;
    }

} /* namespace cli */
