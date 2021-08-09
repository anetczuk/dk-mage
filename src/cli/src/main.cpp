/*
 * main.cpp
 *
 */

#include "cli/Config.h"

#include "dkmage/Generator.h"
#include "dkmage/Parameter.h"

#include "adiktedpp/Level.h"
#include "adiktedpp/Version.h"

#include "utils/Set.h"
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

std::string getProperSeed( const std::string& seed ) {
    if ( seed.empty() == false ) {
        return seed;
    }

    const unsigned int timeSeed = time(NULL);
    srand( timeSeed );
    const std::string newSeed = genSeed();
    if ( newSeed.empty() ) {
        LOG() << "unable to generate seed";
        return "";
    }
    return newSeed;
}

void initializeRand( const std::string& seed ) {
    LOG() << "using seed '" << seed << "'";
    const std::size_t seedValue = std::hash< std::string >{}( seed );
    srand( seedValue );
}

std::string getProperType( const std::string& mapType ) {
    if ( mapType.compare("random") != 0 ) {
        return mapType;
    }

    /// random
    LOG() << "getting random map generator";
    const Generator& generator = Generator::instance();
    const std::vector<std::string> typeAllowed = generator.generatorsList();
    std::set<std::string> typeSet( typeAllowed.begin(), typeAllowed.end() );
    typeSet.erase( "random" );
    if ( typeSet.empty() ) {
        LOG() << "could not get proper map type: no generators found";
        return "";
    }
    const std::size_t rIndex = rand() % typeSet.size();
    return getSetItem( typeSet, rIndex );
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

int readParameters( int argc, char** argv, ParametersMap& retParameters ) {
    TCLAP::CmdLine cmd( "Map and scenario generator for Dungeon Keeper 1 PC game", ' ', VERSION_FULL_STRING );
    cmd.ignoreUnmatched( true );                /// ignore unmatched/unknown arguments

    Generator& generator = Generator::instance();

    TCLAP::ValueArg<std::string> configArg( "", "config", "Path to configuration INI file", false, "config.ini", "path string", cmd );

    std::vector<std::string> typeAllowed = generator.generatorsList();
    typeAllowed.push_back( "random" );

    TCLAP::ValuesConstraint<std::string> typeAllowedVals( typeAllowed );
    TCLAP::ValueArg<std::string> typeArg( "", "type", "Map type", false, "random", &typeAllowedVals, cmd );

    TCLAP::ValueArg<std::string> seedArg( "", "seed", "Generation seed", false, "", "any string", cmd );

    TCLAP::SwitchArg testingModeArg( "", "test_mode", "Additional items on map, e.g. reveal map or neutral monsters next to player's dungeon heart", cmd );

    TCLAP::ValueArg<std::string> outputPathArg( "", "output_path", "Path to map's output file (absolute or relative to work dir)", false, "", "path string" );
    TCLAP::ValueArg<std::string> outputSubPathArg( "", "output_subpath", "Path to map's output file relative to 'level_path' config field", false, "", "path string" );
    TCLAP::ValueArg<std::size_t> outputIdArg( "", "output_id", "Id of output map (will be placed in game's level directory)", false, 3333, "int" );
    TCLAP::SwitchArg outputAutoArg( "", "output_auto", "Finds unused map id and use it to store map", true );
    TCLAP::EitherOf input;
//        TCLAP::OneOf input;
    input.add( outputPathArg ).add( outputSubPathArg ).add( outputIdArg ).add( outputAutoArg );
    cmd.add( input );

    TCLAP::ValueArg<std::string> outputbmpArg( "", "output_bmp", "Path to map's output BMP file", false, "", "path string", cmd );

    cmd.parse( argc, argv );

    /// ===========================================================================

    /// loading config file
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

    /**
     * Parameters needed before construction of parameters map (because already in use):
     *      config      -- config already loaded
     *      seed        -- needed to get random type
     *      type        -- required to read proper section from config
     */

    const cli::Config::RawData generalData = config.readSection( "general" );
    retParameters.appendData( generalData );

    /// handle seed
    std::string mapSeed = retParameters.getString( ParameterName::PN_SEED, "" );
    if ( seedArg.isSet() ) {
        mapSeed = seedArg.getValue();                                   /// yes, copy
    }
    mapSeed = getProperSeed( mapSeed );
    if ( mapSeed.empty() ) {
        return 1;
    }
    initializeRand( mapSeed );

    /// try to load map type section based on value 'general::type' from config
    {
        std::string configMapType = retParameters.getString( ParameterName::PN_TYPE, "random" );
        configMapType = getProperType( configMapType );
        if ( configMapType.empty() == false ) {
            const cli::Config::RawData mapData = config.readSection( configMapType );
            retParameters.appendData( mapData );
            retParameters.add( typeArg.getName(), configMapType );                        /// override section field
        }
    }

    /// try to load map type section based on cmd line argument
    if ( typeArg.isSet() ) {
        std::string cmdMapType = typeArg.getValue();
        cmdMapType = getProperType( cmdMapType );
        if ( cmdMapType.empty() == false ) {
            const cli::Config::RawData mapData = config.readSection( cmdMapType );
            retParameters.appendData( mapData );
            retParameters.add( typeArg.getName(), cmdMapType );                           /// override section field
        }
    }

    /// append cmd args
    retParameters.add( configArg.getName(), configPath );
    /// 'type' field already set
    retParameters.add( seedArg.getName(), mapSeed );                                      /// override section field

    if ( testingModeArg.isSet() )    retParameters.add( testingModeArg.getName(), testingModeArg.getValue() );

    if ( outputPathArg.isSet() )    retParameters.add( outputPathArg.getName(), outputPathArg.getValue() );
    if ( outputSubPathArg.isSet() ) retParameters.add( outputSubPathArg.getName(), outputSubPathArg.getValue() );
    if ( outputIdArg.isSet() )      retParameters.add( outputIdArg.getName(), outputIdArg.getValue() );
    if ( outputAutoArg.isSet() )    retParameters.add( outputAutoArg.getName(), outputAutoArg.getValue() );
    if ( outputbmpArg.isSet() )     retParameters.add( outputbmpArg.getName(), outputbmpArg.getValue() );

    return 0;
}


/// =========================================================================================================


int main( int argc, char** argv ) {
    try {
        ParametersMap parameters;
        const int invalid = readParameters( argc, argv, parameters );
        if ( invalid != 0 ) {
            return invalid;
        }

        const Optional<std::string> mapTypeParam = parameters.getString( "type" );
        const std::string mapType = mapTypeParam.value();

        LOG() << "using map generator: '" << mapType << "'";
        Generator& generator = Generator::instance();
        LevelGenerator* typeGenerator = generator.get( mapType );
        if ( typeGenerator == nullptr ) {
            LOG() << "unable to handle generator '" << mapType << "'";
            return 1;
        }

        typeGenerator->setParameters( parameters );

        /// generate level
        LOG() << "generating level";
        typeGenerator->generateLevel();

        LOG() << "generation completed";

        /// store generated level
        path outputLevelFile = "";
        const Optional<std::string> outputPathParam    = parameters.getString( ParameterName::PN_OUTPUT_PATH );
        const Optional<std::string> outputSubPathParam = parameters.getString( ParameterName::PN_OUTPUT_SUBPATH );
        const Optional<std::size_t> outputIdParam      = parameters.getSizeT( ParameterName::PN_OUTPUT_ID );
        const Optional<std::string> levelsPathParam    = parameters.getString( ParameterName::PN_LEVELS_PATH );
        if ( outputPathParam.isSet() ) {
            /// store in absolute path or path relative to work dir
            const path outputPath = outputPathParam.getValue();
            if ( outputPath.is_absolute() ) {
                /// absolute command-line argument path
                outputLevelFile = outputPath;
            } else {
                /// relative path to work dir
                const path output = current_path() / outputPath;
                outputLevelFile = output.string();
            }
        } else if ( outputSubPathParam.isSet() ) {
            /// store in path relative to 'levels_path'
            const std::string levelsPath = levelsPathParam.value_or( "" );                     /// yes, copy
            const path outputSubPath     = outputSubPathParam.getValue();
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
        } else if ( outputIdParam.isSet() ) {
            const std::string levelsPath  = levelsPathParam.value_or( "" );                     /// yes, copy
            const std::size_t levelNumber = outputIdParam.getValue();
            const std::string mapName     = raw::RawLevel::prepareMapName( levelNumber );
            if ( levelsPath.empty() == false ) {
                const path outputPath     = path(levelsPath) / mapName;
                outputLevelFile           = outputPath.string();
            } else {
                outputLevelFile           = mapName;
            }
        } else {
            /// auto mode
            const std::string levelsPath = levelsPathParam.value_or( "" );                     /// yes, copy
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

            {
                const std::size_t mapId = raw::RawLevel::extractMapIdFromPath( output );
                std::stringstream stream;
                stream << "dk-mage map " << mapId;
                const std::string name = stream.str();
                typeGenerator->setLevelName( name );
            }

            typeGenerator->storeLevel( output );
            const std::string outputBmp = output + ".bmp";
            typeGenerator->storePreview( outputBmp );
        } else {
            LOG() << "unable to store level: empty path";
        }

        /// store preview image
        const Optional<std::string> outputBmpParam = parameters.getString( ParameterName::PN_OUTPUT_BMP );
        const std::string& bmpFile = outputBmpParam.getValue( "" );
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
