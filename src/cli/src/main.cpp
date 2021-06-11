/*
 * main.cpp
 *
 */

#include "utils/Log.h"

#include <tclap/CmdLine.h>


namespace {

    void generate( const std::string& mapType, const std::string& seed ) {
        LOG() << "generating map: " << mapType << " seed: " << seed;
    }

}


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', "1.0.1" );

        std::vector<std::string> typeAllowed;
        typeAllowed.push_back("cave");
        TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
        TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "cave", &typeAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        cmd.parse( argc, argv );

        const std::string& mapType = typeArg.getValue();
        const std::string& mapSeed = seedArg.getValue();

        generate( mapType, mapSeed );

    } catch ( TCLAP::ArgException &e ) {
        LOG() << "error: " << e.error() << " for arg " << e.argId();
    }

    return 0;
}
