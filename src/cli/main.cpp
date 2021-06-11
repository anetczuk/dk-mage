/*
 * main.cpp
 *
 */

#include "utils/Log.h"

#include <tclap/CmdLine.h>


namespace {

    void generate( const std::string& mapTemplate, const std::string& seed ) {
        LOG() << "generating map: " << mapTemplate << " seed: " << seed;
    }

}


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', "1.0.1" );

        std::vector<std::string> templateAllowed;
        templateAllowed.push_back("cave");
        TCLAP::ValuesConstraint<std::string> templateAllowedVals( templateAllowed );
        TCLAP::ValueArg<std::string> templateArg( "", "template", "Map template", true, "", &templateAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        cmd.parse( argc, argv );

        const std::string& mapTemplate = templateArg.getValue();
        const std::string& mapSeed     = seedArg.getValue();

        generate( mapTemplate, mapSeed );

    } catch ( TCLAP::ArgException &e ) {
        LOG() << "error: " << e.error() << " for arg " << e.argId();
    }

    return 0;
}
