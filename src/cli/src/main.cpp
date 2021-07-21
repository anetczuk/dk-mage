/*
 * main.cpp
 *
 */

#include "cli/Config.h"

#include "dkmage/Generator.h"

#include "adiktedpp/Level.h"

#include "utils/Rand.h"
#include "utils/Log.h"

#include <tclap/CmdLine.h>

#include <filesystem>
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

std::string findFreeMapName( const std::string& levelsPath ) {
    for (std::size_t i=3333; i<100000; ++i ) {
        const std::string mapName = adiktedpp::Level::prepareMapName( i );
        const std::filesystem::path levelPath = levelsPath + "/" + mapName + ".dat";
        if ( std::filesystem::exists( levelPath ) ) {
            continue ;
        }
        return mapName;
    }
    return "";
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

        TCLAP::ValueArg<std::string> outputPathArg( "", "outputPath", "Path to map's output file (relative or absolute)", false, "", "path string" );
        TCLAP::ValueArg<std::size_t> outputIdArg( "", "outputId", "Id of output map (will be placed in game's level directory)", false, 3333, "int" );
        TCLAP::SwitchArg outputAutoArg( "", "outputAuto", "Finds unused map id and use it to store map", true );
        TCLAP::EitherOf input;
//        TCLAP::OneOf input;
        input.add( outputPathArg ).add( outputIdArg ).add( outputAutoArg );
        cmd.add( input );

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

        LOG() << "generation completed";

        if ( outputPathArg.isSet() ) {
            /// store generated level
            const std::string levelsPath = config.readLevelsPath();
            const std::string levelFile  = outputPathArg.getValue();         /// yes, copy
            const std::filesystem::path levelPath = levelFile;
            if ( levelPath.is_absolute() ) {
                /// absolute path
                typeGenerator->storeLevel( levelFile );
            } else {
                /// relative path
                const std::string outPath = levelsPath + "/" + levelFile;
                typeGenerator->storeLevel( outPath );
            }
        } else if ( outputIdArg.isSet() ) {
            const std::string levelsPath  = config.readLevelsPath();
            const std::size_t levelNumber = outputIdArg.getValue();
            const std::string mapName     = adiktedpp::Level::prepareMapName( levelNumber );
            const std::string levelFile   = levelsPath + "/" + mapName;
            typeGenerator->storeLevel( levelFile );
        } else {
            const std::string levelsPath = config.readLevelsPath();
            const std::string mapName    = findFreeMapName( levelsPath );
            const std::string levelFile  = levelsPath + "/" + mapName;
            typeGenerator->storeLevel( levelFile );
        }

        /// store preview image
        const std::string& bmpFile = outbmpArg.getValue();
        if ( bmpFile.empty() == false ) {
            typeGenerator->storePreview( bmpFile );
        }

    } catch ( TCLAP::ArgException& e ) {
        LOG() << "error: " << e.error() << " for arg " << e.argId();
        return 1;

    } catch ( std::exception& e ) {
        LOG() << "error: " << e.what();
        return 1;
    }

    return 0;
}
