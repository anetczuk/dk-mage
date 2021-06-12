/*
 * main.cpp
 *
 */

#include "cli/Generator.h"
#include "cli/Config.h"

#include "utils/Log.h"

#include <tclap/CmdLine.h>


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', "1.0.1" );

        cli::Generator& generator = cli::Generator::instance();

        TCLAP::ValueArg<std::string> configArg( "", "config", "Path to configuration INI file", false, "config.ini", "path string", cmd );

        std::vector<std::string> typeAllowed = generator.generatorsList();                                          /// yes, copy
        TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
        TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "cave", &typeAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        TCLAP::ValueArg<std::string> outdirArg( "", "output", "Path to map's output directory", true, ".", "path string", cmd );

        TCLAP::ValueArg<std::string> outbmpArg( "", "outbmp", "Path to map's output BMP file", false, "", "path string", cmd );

        cmd.parse( argc, argv );

        /// ---------------------------------------------------------

        const std::string& configPath = configArg.getValue();
        cli::Config config( configPath );

        const std::string& mapType = typeArg.getValue();
        dkmage::LevelGenerator* typeGenerator = generator.get( mapType );
        if ( typeGenerator == nullptr ) {
            LOG() << "unable to find generator '" << mapType << "'";
            return 1;
        }

        const std::string& mapSeed = seedArg.getValue();
        typeGenerator->generate( mapSeed );

        const std::string levelsPath = config.readLevelsPath();
        const std::string& levelDir = outdirArg.getValue();
        const std::string outPath = levelsPath + "/" + levelDir;
        typeGenerator->storeLevel( outPath );

        const std::string& bmpFile = outbmpArg.getValue();
        if ( bmpFile.empty() == false ) {
            typeGenerator->storePreview( bmpFile );
        }

    } catch ( TCLAP::ArgException& e ) {
        LOG() << "error: " << e.error() << " for arg " << e.argId();
        return 1;

    } catch ( std::runtime_error& e ) {
        LOG() << "error: " << e.what();
        return 1;
    }

    return 0;
}
