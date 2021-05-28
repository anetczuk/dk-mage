/*
 *
 */

#include "catch2/catch.hpp"

#include "adiktedpp/Level.h"


using namespace adiktedpp;


TEST_CASE("Level_constructor", "[classic]") {
    Level level;
    REQUIRE( true );
}

TEST_CASE("Level_levelPath_unload", "[classic]") {
    Level level;
    const std::string path = level.levelsPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_setLevelsPath", "[classic]") {
    Level level;

    level.setLevelsPath( "abc/def" );
    const std::string path = level.levelsPath();
    REQUIRE( path == "abc/def" );
}

TEST_CASE("Level_dataPath_unload", "[classic]") {
    Level level;
    const std::string path = level.dataPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_startNewMap_succeed", "[classic]") {
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
    Level level;

    const bool loaded = level.loadMapByPath( "map_invalid_name" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName == ".\\map_invalid_name" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_subdir_failed", "[classic]") {
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
    Level level;

    const bool loaded = level.loadMapByPath( "123" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName == ".\\map00123" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_subdir_loaded", "[classic]") {
    Level level;

    const bool loaded = level.loadMapByPath( "xxx/map00001" );
    REQUIRE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName == "xxx/map00001" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "xxx/map00001" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );

//    level.generateBmp();
}

//TEST_CASE("Level_loadMapById_number", "[classic]") {
//    Level level;
//    const bool loaded = level.loadMapById( 23 );
//    CHECK( loaded );
//
//    const std::string path = level.levelsPath();
//    CHECK( path == "" );
//}
