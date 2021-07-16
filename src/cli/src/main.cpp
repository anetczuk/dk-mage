/*
 * main.cpp
 *
 */

#include "cli/Config.h"

#include "dkmage/Generator.h"

#include "utils/Rand.h"
#include "utils/Log.h"

#include <tclap/CmdLine.h>
#include <random>


bool initializeRand( const std::string& seed ) {
    if ( seed.empty() ) {
        const unsigned int timeSeed = time(NULL);
        srand( timeSeed );
        const std::string newSeed = utils::genSeed();
        if ( newSeed.empty() ) {
            LOG() << "unable to generate seed";
            return false;
        }
        return true;
    }

    LOG() << "using seed '" << seed << "'";
    const std::size_t seedValue = std::hash< std::string >{}( seed );
    srand( seedValue );

    return true;
}

dkmage::LevelGenerator* getGenerator( dkmage::Generator& generator, const std::string& mapType ) {
    if ( mapType.compare("random") != 0 ) {
        LOG() << "using map generator: '" << mapType << "'";
        return generator.get( mapType );
    }

    /// random
    const std::vector<std::string> typeAllowed = generator.generatorsList();
    std::set<std::string> typeSet( typeAllowed.begin(), typeAllowed.end() );
    typeSet.erase( "random" );
    const std::size_t rIndex = rand() % typeSet.size();
    const std::string newType = utils::getSetItem( typeSet, rIndex );
    return getGenerator( generator, newType );
}


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', "1.0.1" );

        dkmage::Generator& generator = dkmage::Generator::instance();

        TCLAP::ValueArg<std::string> configArg( "", "config", "Path to configuration INI file", false, "config.ini", "path string", cmd );

        std::vector<std::string> typeAllowed = generator.generatorsList();
        typeAllowed.push_back( "random" );

        TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
        TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "random", &typeAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        TCLAP::ValueArg<std::string> outdirArg( "", "output", "Path to map's output directory", true, ".", "path string", cmd );

        TCLAP::ValueArg<std::string> outbmpArg( "", "outbmp", "Path to map's output BMP file", false, "", "path string", cmd );

        cmd.parse( argc, argv );

        /// ---------------------------------------------------------

        const std::string& configPath = configArg.getValue();
        cli::Config config( configPath );

        const std::string& mapSeed = seedArg.getValue();
        if ( initializeRand( mapSeed ) == false ) {
            LOG() << "unable to initialize random number generator with seed '" << mapSeed << "'";
            return 1;
        }

        const std::string& mapType = typeArg.getValue();
        dkmage::LevelGenerator* typeGenerator = getGenerator( generator, mapType );
        if ( typeGenerator == nullptr ) {
            LOG() << "unable to find generator '" << mapType << "'";
            return 1;
        }

        const std::string dataPath = config.readDataPath();
        typeGenerator->setDataPath( dataPath );

        /// generate level
        LOG() << "generating level";
        typeGenerator->generateLevel();

        {
            /// store generated level
            const std::string levelsPath = config.readLevelsPath();
            const std::string& levelDir = outdirArg.getValue();
            const std::string outPath = levelsPath + "/" + levelDir;
            typeGenerator->storeLevel( outPath );
        }

        /// store preview image
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
