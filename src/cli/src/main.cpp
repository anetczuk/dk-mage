/*
 * main.cpp
 *
 */

#include "cli/Generator.h"

#include "utils/Log.h"

#include <tclap/CmdLine.h>


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', "1.0.1" );

        cli::Generator& generator = cli::Generator::instance();

        std::vector<std::string> typeAllowed = generator.generatorsList();                                          /// yes, copy
        TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
        TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "cave", &typeAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        cmd.parse( argc, argv );

        const std::string& mapType = typeArg.getValue();
        const std::string& mapSeed = seedArg.getValue();

        generator.generate( mapType, mapSeed );

    } catch ( TCLAP::ArgException &e ) {
        LOG() << "error: " << e.error() << " for arg " << e.argId();
    }

    return 0;
}
