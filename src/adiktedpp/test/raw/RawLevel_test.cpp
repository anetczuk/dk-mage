/*
 *
 */

#include "catch2/catch.hpp"
#include "Path.h"

#include "adiktedpp/raw/RawLevel.h"

#include "adiktedpp/Messages.h"
#include "Path.h"
#include "utils/Rand.h"
#include "utils/Log.h"
#include "utils/Separator.h"


using namespace utils;
using namespace adiktedpp;


inline std::string get_test_LOG_INFO() {
    path outputPath = getTestOutputPath();
    const std::string testName = Catch::getResultCapture().getCurrentTestName();
    outputPath /= testName;
    outputPath += std::string(".log");
    return outputPath.u8string();
}

inline ScopeMessages initialize_messages() {
    const std::string outputFile = get_test_LOG_INFO();
    return ScopeMessages( outputFile );
}


//using RawLevelMock = RawLevel;

class RawLevelMock: public raw::RawLevel {
public:

    ScopeMessages scoped;


    RawLevelMock(): raw::RawLevel(), scoped( initialize_messages() ) {
    }

//    void generateTestBmp( const std::size_t rescale = 3 ) {
//        setRescale( rescale );
//        const std::string outputPath = Catch::getResultCapture().getCurrentTestName() + ".bmp";
//        generateBmp( outputPath );
//    }

};


TEST_CASE("RawLevel_constructor", "[classic]") {
    RawLevelMock level;
    REQUIRE( true );
}

TEST_CASE("RawLevel_levelPath_unload", "[classic]") {
    RawLevelMock level;
    const std::string path = level.levelsPath();
    REQUIRE( path == "" );
}

TEST_CASE("RawLevel_setLevelsPath", "[classic]") {
    RawLevelMock level;

    level.setLevelsPath( "abc/def" );
    const std::string path = level.levelsPath();
    REQUIRE( path == "abc/def" );
}

TEST_CASE("RawLevel_dataPath_unload", "[classic]") {
    ScopeMessages messages = initialize_messages();

    raw::RawLevel level;
    const std::string path = level.dataPath();
    REQUIRE( path == "" );
}

TEST_CASE("RawLevel_startNewMap_succeed", "[classic]") {
    RawLevelMock level;

    const std::string fName = level.inputFileName();
    CHECK( fName == "" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("RawLevel_setSlab_rng", "[classic]") {
    /**
     * Ensure that placing items on map does not consume random numbers.
     */

    utils::rng_srand( 1 );

    REQUIRE( utils::rng_randi() == 3796174982 );

    RawLevelMock level;
    level.setSlab( 10, 10, raw::SlabType::ST_PATH );

    REQUIRE( utils::rng_randi() == 4182529786 );
}

//TEST_CASE("RawLevel_generateRandomMap", "[classic]") {
//    RawLevelMock level;
//
////    srand_ng( 0 );
//    level.generateRandomMap();
//
//    level.generateTestBmp();
//}

TEST_CASE("RawLevel_verifyMap_empty", "[classic]") {
    RawLevelMock level;

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == false );
}

TEST_CASE("RawLevel_verifyMap_random", "[classic]") {
    RawLevelMock level;

    level.generateRandomMap();

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == false );
}

TEST_CASE("RawLevel_countAllCreatures", "[classic]") {
    RawLevelMock level;

//    level.generateTestBmp();

    const Point point( 20, 20 );
    level.setSlab( point, raw::SlabType::ST_EARTH );
    level.setCreature( point, 4, raw::SubTypeCreature::STC_FAIRY, 3 );
    const std::size_t count = level.countAllCreatures();
    REQUIRE( count == 3 );
}

TEST_CASE("RawLevel_countSeparatedAreas_new", "[classic]") {
    RawLevelMock level;

//    level.generateTestBmp();

    const std::size_t areas = level.countSeparatedAreas();
    REQUIRE( areas == 0 );
}

TEST_CASE("RawLevel_loadMapByPath_string_name_failed", "[classic]") {
    RawLevelMock level;

    const bool loaded = level.loadMapByPath( "map_invalid_name" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( separatorsToLinux( fName ) == "./map_invalid_name" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("RawLevel_loadMapByPath_string_subdir_failed", "[classic]") {
    RawLevelMock level;

    const bool loaded = level.loadMapByPath( "aaa/map_invalid_name" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( fName == "aaa/map_invalid_name" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("RawLevel_loadMapByPath_string_number_failed", "[classic]") {
    RawLevelMock level;

    const bool loaded = level.loadMapByPath( "123" );
    REQUIRE_FALSE( loaded );

    const std::string fName = level.inputFileName();
    CHECK( separatorsToLinux( fName ) == "./map00123" );

    const std::string outFName = level.outputFileName();
    CHECK( outFName == "" );

    const std::string levelPath = level.levelsPath();
    CHECK( levelPath == "" );
}

TEST_CASE("RawLevel_loadMapByPath_string_subdir_loaded", "[classic]") {
    RawLevelMock level;

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

TEST_CASE("RawLevel_setSlab_rect", "[classic]") {
    RawLevelMock level;

    const raw::SlabType newSlab = raw::SlabType::ST_LAVA;
    level.setSlab( 10, 10, 20, 20, newSlab );

    const raw::SlabType gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == newSlab );

//    level.generateTestBmp();
}

TEST_CASE("RawLevel_setSlab_owner_path", "[classic]") {
    RawLevelMock level;

    level.setSlab( 30, 30, raw::SlabType::ST_PATH );

    const raw::PlayerType gotOwner = level.getOwner( 30, 30 );
    CHECK( gotOwner == raw::PlayerType::PT_UNSET );

//    level.generateTestBmp();
}

TEST_CASE("RawLevel_setSlab_owner_room", "[classic]") {
    RawLevelMock level;

    const raw::SlabType newSlab    = raw::SlabType::ST_TRAINING;
    const raw::PlayerType newOwner = raw::PlayerType::PT_0;
    level.setSlab( 10, 10, 20, 20, newSlab, newOwner );

    /// check training
    const raw::SlabType gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == newSlab );

    const raw::PlayerType gotOwner = level.getOwner( 15, 15 );
    CHECK( gotOwner == newOwner );

//    level.generateTestBmp();
}

TEST_CASE("RawLevel_getActionPointPosition", "[classic]") {
    RawLevelMock level;

    const std::size_t acn = level.addActionPoint( 10, 20, 4, 0 );
    CHECK( acn == 1 );

    const Point acp = level.getActionPointPosition( acn );
    CHECK( acp == Point( 10, 20 ) );
}

TEST_CASE("RawLevel_getHeroGatePosition", "[classic]") {
    RawLevelMock level;

    level.setItem( 10, 20, 4, raw::SubTypeItem::STI_HEROGATE );
    const Point acp = level.getHeroGatePosition( 1 );
    CHECK( acp == Point( 10, 20 ) );
}
