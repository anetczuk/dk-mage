/*
 * Generator.cpp
 *
 */

#include "cli/Generator.h"

#include "dkmage/mode/Cave.h"
#include "dkmage/mode/Maze.h"


namespace cli {

    std::vector< std::string > Generator::generatorsList() const {
        std::vector< std::string > list;
        for ( const auto& kv: genMap ) {
            list.push_back( kv.first );
        }
        return list;
    }

    dkmage::LevelGenerator* Generator::get( const std::string& type ) {
        LevelGeneratorPtr& itemPtr = genMap[ type ];
        AbstractItem* item = itemPtr.get();
        return item->create();
//        return itemPtr.get();
    }

    Generator& Generator::instance() {
        static Generator generator;
        return generator;
    }

    Generator::Generator() {
        add< dkmage::mode::Cave >( "cave" );
        add< dkmage::mode::Maze >( "maze" );
    }

} /* namespace cli */
