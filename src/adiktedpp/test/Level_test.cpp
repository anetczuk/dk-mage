/*
 *
 */

#include "catch2/catch.hpp"

#include "adiktedpp/Level.h"

#include "adiktedpp/Messages.h"
#include "Path.h"
#include "utils/Log.h"
#include "utils/Separator.h"


using namespace adiktedpp;


inline ScopeMessages initialize_messages() {
    const std::string testName = Catch::getResultCapture().getCurrentTestName();
    return ScopeMessages( testName, ".log" );
}


TEST_CASE("Level_constructor", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;
    REQUIRE( true );
}

TEST_CASE("Level_levelPath_unload", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;
    const std::string path = level.levelsPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_setLevelsPath", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    level.setLevelsPath( "abc/def" );
    const std::string path = level.levelsPath();
    REQUIRE( path == "abc/def" );
}

TEST_CASE("Level_dataPath_unload", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;
    const std::string path = level.dataPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_startNewMap_succeed", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    level.startNewMap();

    const std::string fName = level.inputFileName();
    CHECK( fName == "" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_name_failed", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    const bool loaded = level.loadMapByPath( "map_invalid_name" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( separatorsToLinux( fName ) == "./map_invalid_name" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_subdir_failed", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    const bool loaded = level.loadMapByPath( "aaa/map_invalid_name" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName == "aaa/map_invalid_name" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_number_failed", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    const bool loaded = level.loadMapByPath( "123" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( separatorsToLinux( fName ) == "./map00123" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_subdir_loaded", "[classic]") {
    ScopeMessages scoped = initialize_messages();

    Level level;

    const std::string inputData = getTestDataPath( "map_empty/map_empty" );
    const bool loaded = level.loadMapByPath( inputData );
    REQUIRE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName != "" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName != "" );

    const std::string dataPath = level.dataPath();
    CHECK( dataPath == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

//TEST_CASE("Level_generateBmp", "[classic]") {
//    ScopeMessages scoped = initialize_messages();
//
//    Level level;
//
//    level.startNewMap();
//
//    const std::string& keeperData = getKeeperDataPath();
//    level.setDataPath( keeperData );
//
//    const std::string outputPath = Catch::getResultCapture().getCurrentTestName() + ".bmp";
//    level.generateBmp( outputPath );
//}
