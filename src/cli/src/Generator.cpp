/*
 * Generator.cpp
 *
 */

#include "cli/Generator.h"

#include "dkmage/mode/Cave.h"

#include "utils/Log.h"

#include <functional>                               /// std::hash
#include <time.h>                                   /// time


namespace cli {

    std::vector< std::string > Generator::generatorsList() const {
        std::vector< std::string > list;
        for ( const auto& kv: genMap ) {
            list.push_back( kv.first );
        }
        return list;
    }

    dkmage::LevelGenerator* Generator::get( const std::string& type ) {
        LevelGeneratorPtr& item = genMap[ type ];
        return item.get();
    }

    Generator& Generator::instance() {
        static Generator generator;
        return generator;
    }

    Generator::Generator() {
        add< dkmage::mode::Cave >( "cave" );
    }

} /* namespace cli */
