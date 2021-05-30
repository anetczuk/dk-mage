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


//using LevelMock = Level;

class LevelMock: public Level {
public:

    ScopeMessages scoped;


    LevelMock(): Level(), scoped( initialize_messages() ) {
        const std::string& keeperData = getKeeperDataPath();
        setDataPath( keeperData );
    }

    void generateTestBmp( const std::size_t rescale = 3 ) {
        setRescale( rescale );
        const std::string outputPath = Catch::getResultCapture().getCurrentTestName() + ".bmp";
        generateBmp( outputPath );
    }

};


TEST_CASE("Level_constructor", "[classic]") {
    LevelMock level;
    REQUIRE( true );
}

TEST_CASE("Level_levelPath_unload", "[classic]") {
    LevelMock level;
    const std::string path = level.levelsPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_setLevelsPath", "[classic]") {
    LevelMock level;

    level.setLevelsPath( "abc/def" );
    const std::string path = level.levelsPath();
    REQUIRE( path == "abc/def" );
}

TEST_CASE("Level_dataPath_unload", "[classic]") {
    ScopeMessages  messages = initialize_messages();

    Level level;
    const std::string path = level.dataPath();
    REQUIRE( path == "" );
}

TEST_CASE("Level_startNewMap_succeed", "[classic]") {
    LevelMock level;

    level.startNewMap();

    const std::string fName = level.inputFileName();
    CHECK( fName == "" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_loadMapByPath_string_name_failed", "[classic]") {
    LevelMock level;

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
    LevelMock level;

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
    LevelMock level;

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
    LevelMock level;

    const std::string inputData = getTestDataPath( "map_empty/map_empty" );
    const bool loaded = level.loadMapByPath( inputData );
    REQUIRE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName != "" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName != "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("Level_setSlab_rect", "[classic]") {
    LevelMock level;

    level.startNewMap();

    const SlabType newSlab = SlabType::ST_LAVA;
    level.setSlab( 10, 10, 20, 20, newSlab );

    const SlabType gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == newSlab );

    level.generateTestBmp();
}

TEST_CASE("Level_setSlabRoom", "[classic]") {
    LevelMock level;

    level.startNewMap();

    const SlabType newSlab    = SlabType::ST_TRAINING;
    const PlayerType newOwner = PlayerType::PT_0;
    level.setSlabRoom( 10, 10, 20, 20, newSlab, newOwner );

    const SlabType gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == newSlab );

    const PlayerType gotOwner = level.getOwner( 15, 15 );
    CHECK( gotOwner == newOwner );

    level.generateTestBmp();
}
