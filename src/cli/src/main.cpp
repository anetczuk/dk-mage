/*
 * main.cpp
 *
 */

#include "cli/Config.h"

#include "dkmage/Generator.h"

#include "adiktedpp/Level.h"
#include "adiktedpp/Version.h"

#include "utils/Rand.h"
#include "utils/Log.h"
#include "utils/GlobalScope.h"

#include <tclap/CmdLine.h>
#include <ghc/filesystem.hpp>

//#include <filesystem>
#include <random>


using namespace utils;
using namespace adiktedpp;
using namespace dkmage;

using ghc::filesystem::path;
using ghc::filesystem::exists;
using ghc::filesystem::current_path;
using ghc::filesystem::create_directories;

//using std::filesystem::path;
//using std::filesystem::exists;


GLOBAL_SCOPE() {
    Logger::setLogFile( "dkmage.log.txt" );
}


bool initializeRand( const std::string& seed ) {
    if ( seed.empty() ) {
        const unsigned int timeSeed = time(NULL);
        srand( timeSeed );
        const std::string newSeed = genSeed();
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

LevelGenerator* getGenerator( Generator& generator, const std::string& mapType ) {
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
    const std::string newType = getSetItem( typeSet, rIndex );
    return getGenerator( generator, newType );
}

std::string findFreeMapName( const std::string& levelsPath ) {
    for (std::size_t i=3333; i<100000; ++i ) {
        const std::string mapName = raw::RawLevel::prepareMapName( i );
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
        TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', VERSION_FULL_STRING );

        Generator& generator = Generator::instance();

        TCLAP::ValueArg<std::string> configArg( "", "config", "Path to configuration INI file", false, "config.ini", "path string", cmd );

        std::vector<std::string> typeAllowed = generator.generatorsList();
        typeAllowed.push_back( "random" );

        TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
        TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "random", &typeAllowedVals, cmd );

        TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

        TCLAP::ValueArg<std::string> outputPathArg( "", "output_path", "Path to map's output file (absolute or relative to work dir)", false, "", "path string" );
        TCLAP::ValueArg<std::string> outputSubPathArg( "", "output_subpath", "Path to map's output file relative to 'level_path' config field", false, "", "path string" );
        TCLAP::ValueArg<std::size_t> outputIdArg( "", "output_id", "Id of output map (will be placed in game's level directory)", false, 3333, "int" );
        TCLAP::SwitchArg outputAutoArg( "", "output_auto", "Finds unused map id and use it to store map", true );
        TCLAP::EitherOf input;
//        TCLAP::OneOf input;
        input.add( outputPathArg ).add( outputSubPathArg ).add( outputIdArg ).add( outputAutoArg );
        cmd.add( input );

        TCLAP::ValueArg<std::string> outbmpArg( "", "output_bmp", "Path to map's output BMP file", false, "", "path string", cmd );

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
        LevelGenerator* typeGenerator = getGenerator( generator, mapType );
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

        /// store generated level
        path outputLevelFile = "";
        if ( outputPathArg.isSet() ) {
            /// store in absolute path or path relative to work dir
            const path outputPath = outputPathArg.getValue();
            if ( outputPath.is_absolute() ) {
                /// absolute command-line argument path
                outputLevelFile = outputPath;
            } else {
                /// relative path to work dir
                const path output = current_path() / outputPath;
                outputLevelFile = output.string();
            }
        } else if ( outputSubPathArg.isSet() ) {
            /// store in path relative to 'levels_path'
            const std::string levelsPath = config.readLevelsPath();
            const path outputSubPath        = outputSubPathArg.getValue();
            if ( outputSubPath.is_absolute() ) {
                /// absolute command-line argument path
                outputLevelFile = outputSubPath;
            } else if ( levelsPath.empty() == false ) {
                /// level_path not empty -- append relative path
                const path output = path(levelsPath) / outputSubPath;
                outputLevelFile = output.string();
            } else {
                outputLevelFile = outputSubPath;
            }
        } else if ( outputIdArg.isSet() ) {
            const std::string levelsPath  = config.readLevelsPath();
            const std::size_t levelNumber = outputIdArg.getValue();
            const std::string mapName     = raw::RawLevel::prepareMapName( levelNumber );
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
            const std::string output = outputLevelFile.string();
            typeGenerator->storeLevel( output );
            const std::string outputBmp = output + ".bmp";
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
