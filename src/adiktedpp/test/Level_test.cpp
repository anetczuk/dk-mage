/*
 *
 */

#include "catch2/catch.hpp"
#include "Path.h"

#include "adiktedpp/Level.h"

#include "adiktedpp/Messages.h"
#include "Path.h"
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


//using LevelMock = Level;

class LevelMock: public Level {
public:

    ScopeMessages scoped;


    LevelMock(): Level(), scoped( initialize_messages() ) {
    }

//    void generateTestBmp( const std::size_t rescale = 3 ) {
//        setRescale( rescale );
//        const std::string outputPath = Catch::getResultCapture().getCurrentTestName() + ".bmp";
//        generateBmp( outputPath );
//    }

};


TEST_CASE("Level_constructor", "[classic]") {
    LevelMock level;
    REQUIRE( true );

    raw::RawLevel& rawLevel = level.getRawLevel();

    const std::size_t earthSlabs = rawLevel.countSlabs( raw::SlabType::ST_EARTH );
    REQUIRE( earthSlabs == 0 );

    const std::size_t rockSlabs = rawLevel.countSlabs( raw::SlabType::ST_ROCK );
    REQUIRE( rockSlabs == 7225 );
}

TEST_CASE("Level_startNewMap", "[classic]") {
    LevelMock level;
    level.startNewMap();

    raw::RawLevel& rawLevel = level.getRawLevel();

    const std::size_t rockSlabs = rawLevel.countSlabs( raw::SlabType::ST_ROCK );
    REQUIRE( rockSlabs == 336 );

    const std::size_t earthSlabs = rawLevel.countSlabs( raw::SlabType::ST_EARTH );
    REQUIRE( earthSlabs == 6889 );
}

//TEST_CASE("Level_generateRandomMap", "[classic]") {
//    LevelMock level;
//
////    srand_ng( 0 );
//    level.generateRandomMap();
//
//    level.generateTestBmp();
//}

TEST_CASE("Level_verifyMap_empty", "[classic]") {
    LevelMock level;

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == false );
}

TEST_CASE("Level_verifyMap_random", "[classic]") {
    LevelMock level;

    level.generateRandomMap();

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == false );
}

TEST_CASE("Level_verifyMap_ok", "[classic]") {
    LevelMock level;

    const Rect room( 20, 20, 30, 30 );
    level.setRoom( room, Room::R_DUNGEON_HEART, Player::P_P0, true );

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == true );
}

TEST_CASE("Level_verifyMap_slab50_fail", "[classic]") {
    LevelMock level;

    const Rect room( 20, 20, 30, 30 );
    level.setRoom( room, Room::R_DUNGEON_HEART, Player::P_P0, true );

    const Rect corner = room.leftBottom();
    level.setSlab( corner, Slab::S_HOLY );

    const std::size_t holySlabs = level.getRawLevel().countSlabs( raw::SlabType::ST_HOLYGROUND );
    REQUIRE( holySlabs == 1 );

    const bool ok = level.verifyMap( true );
    REQUIRE( ok == false );
}

TEST_CASE("Level_verifyMap_slab50_skip", "[classic]") {
    LevelMock level;

    const Rect room( 20, 20, 30, 30 );
    level.setRoom( room, Room::R_DUNGEON_HEART, Player::P_P0, true );

    const Rect corner = room.leftBottom();
    level.setSlab( corner, Slab::S_HOLY );

    const std::size_t holySlabs = level.getRawLevel().countSlabs( raw::SlabType::ST_HOLYGROUND );
    REQUIRE( holySlabs == 1 );

    const bool ok = level.verifyMap( true, false, true );
    REQUIRE( ok == true );
}

TEST_CASE("Level_countSeparatedAreas_new", "[classic]") {
    LevelMock level;
    level.startNewMap();

    const std::size_t areas = level.countSeparatedAreas();
    REQUIRE( areas == 0 );
}

TEST_CASE("Level_countSeparatedAreas_rock", "[classic]") {
    LevelMock level;

    raw::RawLevel& rawLevel = level.getRawLevel();

    const std::size_t rockSlabs = rawLevel.countSlabs( raw::SlabType::ST_ROCK );
    REQUIRE( rockSlabs == 7225 );

    const std::size_t areas = level.countSeparatedAreas();
    REQUIRE( areas == 0 );
}

TEST_CASE("Level_countSeparatedAreas_earth", "[classic]") {
    LevelMock level;

    level.setSlab( Rect( 0, 0, 84, 84 ), Slab::S_EARTH );

    const std::size_t areas = level.countSeparatedAreas();
    REQUIRE( areas == 0 );
}

TEST_CASE("Level_countSeparatedAreas_1", "[classic]") {
    LevelMock level;

    level.setSlab( Rect( Point(20, 20), 9, 9 ), Slab::S_EARTH );
    level.setSlab( Rect( Point(50, 50), 9, 9 ), Slab::S_EARTH );

    const std::size_t areas = level.countSeparatedAreas();
    REQUIRE( areas == 1 );
}

TEST_CASE("Level_fillSeparatedAreas", "[classic]") {
    LevelMock level;

    level.setSlab( Rect( Point(20, 20), 5, 5 ), Slab::S_EARTH );
    level.setSlab( Rect( Point(50, 50), 3, 3 ), Slab::S_EARTH );

    REQUIRE( level.countSeparatedAreas() == 1 );

    level.fillSeparatedAreas( 10 );

    REQUIRE( level.countSeparatedAreas() == 0 );
}

TEST_CASE("Level_loadMapByPath_string_name_failed", "[classic]") {
    LevelMock level;

    const bool loaded = level.loadMapByPath( "map_invalid_name" );
    REQUIRE_FALSE( loaded );
}

TEST_CASE("Level_loadMapByPath_string_subdir_loaded", "[classic]") {
    LevelMock level;

    const std::string inputData = getTestDataPath( "map_empty/map_empty" );
    const bool loaded = level.loadMapByPath( inputData );
    REQUIRE( loaded );
}

TEST_CASE("Level_setSlab_rect", "[classic]") {
    LevelMock level;

    const Rect lavaPool( 10, 10, 20, 20 );
    const Slab newSlab = Slab::S_LAVA;
    level.setSlab( lavaPool, newSlab );

    const Slab gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == newSlab );
}

TEST_CASE("Level_setRoom", "[classic]") {
    LevelMock level;

    const Rect room( 10, 10, 20, 20 );
    const Room newRoom    = Room::R_TRAINING;
    const Player newOwner = Player::P_P0;
    level.setRoom( room, newRoom, newOwner, true );

    const Slab gotSlab = level.getSlab( 15, 15 );
    CHECK( gotSlab == Slab::S_PATH );

    const bool owned = level.isOwner( 15, 15, newOwner );
    CHECK( owned );

    const raw::SlabType rawSlab = level.getRawLevel().getSlab( 15, 15 );
    CHECK( rawSlab == raw::SlabType::ST_TRAINING );
}
