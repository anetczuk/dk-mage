/*
 * main.cpp
 *
 */

#include "cli/Config.h"

#include "dkmage/Generator.h"
#include "dkmage/mode/CaveMode.h"
#include "dkmage/mode/MazeMode.h"

#include "adiktedpp/Level.h"
#include "adiktedpp/Version.h"

#include "utils/Rand.h"
#include "utils/Log.h"
#include "utils/GlobalScope.h"

#include <tclap/CmdLine.h>
#include <ghc/filesystem.hpp>

//#include <filesystem>
#include <random>


using ghc::filesystem::path;
using ghc::filesystem::exists;
using ghc::filesystem::current_path;
using ghc::filesystem::create_directories;

//using std::filesystem::path;
//using std::filesystem::exists;


GLOBAL_SCOPE() {
    utils::Logger::setLogFile( "dkmage.log.txt" );

    dkmage::Generator::registerMode<dkmage::mode::CaveMode>( "cave" );
    dkmage::Generator::registerMode<dkmage::mode::MazeMode>( "maze" );
}


bool initializeRand( const std::string& seed ) {
    if ( seed.empty() ) {
        const unsigned int timeSeed = time(NULL);
        srand( timeSeed );
        const std::string newSeed = utils::genSeed();
        if ( newSeed.empty() ) {
            LOG() << "unable to generate seed";
            return false;
        }
        return initializeRand( newSeed );
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
    LOG() << "getting random map generator";
    const std::vector<std::string> typeAllowed = generator.generatorsList();
    std::set<std::string> typeSet( typeAllowed.begin(), typeAllowed.end() );
    typeSet.erase( "random" );
    if ( typeSet.empty() ) {
        LOG() << "could not get generator: no generator candidate";
        return nullptr;
    }
    const std::size_t rIndex = rand() % typeSet.size();
    const std::string newType = utils::getSetItem( typeSet, rIndex );
    return getGenerator( generator, newType );
}

std::string findFreeMapName( const std::string& levelsPath ) {
    for (std::size_t i=3333; i<100000; ++i ) {
        const std::string mapName = adiktedpp::Level::prepareMapName( i );
        path levelPath( levelsPath );
        levelPath /= mapName;
        levelPath += ".dat";
        if ( exists( levelPath ) ) {
            continue ;
        }
        return mapName;
    }
    return "";
}


int main( int argc, char** argv ) {
    try {
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', adiktedpp::VERSION_FULL_STRING );

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

        std::string configPath = configArg.getValue();
        if ( configArg.isSet() == false ) {
            /// config parameter not given -- read default file
            path currPath = current_path() / path( argv[0] ).parent_path() / configPath;
            configPath = currPath.string();
        }
        cli::Config config( configPath );
        if ( config.isValid() == false ) {
            LOG() << "unable to parse config file '" << configPath << "'";
            return 1;
        }
        LOG() << "loaded config file: " << configPath;

        const std::string& mapSeed = seedArg.getValue();
        if ( initializeRand( mapSeed ) == false ) {
            LOG() << "unable to initialize random number generator with seed '" << mapSeed << "'";
            return 1;
        }

        const std::string& mapType = typeArg.getValue();
        dkmage::LevelGenerator* typeGenerator = getGenerator( generator, mapType );
        if ( typeGenerator == nullptr ) {
            LOG() << "unable to handle generator '" << mapType << "'";
            return 1;
        }

        const std::string dataPath = config.readDataPath();
        typeGenerator->setDataPath( dataPath );

        /// generate level
        LOG() << "generating level";
        typeGenerator->generateLevel();

        LOG() << "generation completed";

        path outputLevelFile = "";
        if ( outputPathArg.isSet() ) {
            /// store generated level
            const std::string levelsPath = config.readLevelsPath();
            const std::string levelFile  = outputPathArg.getValue();         /// yes, copy
            const path outputFile = levelFile;
            if ( outputFile.is_absolute() ) {
                /// absolute command-line argument path
                outputLevelFile = levelFile;
            } else if ( levelsPath.empty() == false ) {
                /// level_path not empty -- append relative path
                const path outputPath = path(levelsPath) / levelFile;
                outputLevelFile = outputPath.string();
            } else {
                outputLevelFile = levelFile;
            }
        } else if ( outputIdArg.isSet() ) {
            const std::string levelsPath  = config.readLevelsPath();
            const std::size_t levelNumber = outputIdArg.getValue();
            const std::string mapName     = adiktedpp::Level::prepareMapName( levelNumber );
            if ( levelsPath.empty() == false ) {
                const path outputPath     = path(levelsPath) / mapName;
                outputLevelFile           = outputPath.string();
            } else {
                outputLevelFile           = mapName;
            }
        } else {
            const std::string levelsPath = config.readLevelsPath();
            const std::string mapName    = findFreeMapName( levelsPath );
            if ( levelsPath.empty() == false ) {
                const path outputPath     = path(levelsPath) / mapName;
                outputLevelFile           = outputPath.string();
            } else {
                outputLevelFile           = mapName;
            }
        }

        if ( outputLevelFile.empty() == false ) {
            create_directories( outputLevelFile.parent_path() );                      /// create parent directories
            typeGenerator->storeLevel( outputLevelFile );
            const std::string outputBmp = outputLevelFile.string() + ".bmp";
            typeGenerator->storePreview( outputBmp );
        } else {
            LOG() << "unable to store level: empty path";
        }

        /// store preview image
        const std::string& bmpFile = outbmpArg.getValue();
        if ( bmpFile.empty() == false ) {
            path outputBmp = bmpFile;
            create_directories( outputBmp.parent_path() );                      /// create parent directories
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
