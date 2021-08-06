/*
 * Generator.cpp
 *
 */

#include "dkmage/Generator.h"


namespace dkmage {

    std::vector< std::string > Generator::generatorsList() const {
        std::vector< std::string > list;
        for ( const auto& kv: genMap ) {
            list.push_back( kv.first );
        }
        return list;
    }

    LevelGenerator* Generator::get( const std::string& type ) {
        LevelGeneratorPtr& itemPtr = genMap[ type ];
        AbstractItem* item = itemPtr.get();
        if ( item == nullptr ) {
            return nullptr;
        }
        return item->create();
//        return itemPtr.get();
    }

    Generator& Generator::instance() {
        static Generator generator;
        return generator;
    }

} /* namespace cli */
