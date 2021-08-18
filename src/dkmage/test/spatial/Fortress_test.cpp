/*
 * Fortress_test.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "catch2/catch.hpp"

#include "dkmage/spatial/Fortress.h"

//#include "dkmage/spatial/Fortress.h"


using namespace adiktedpp;
using namespace dkmage;
using namespace spatial;


TEST_CASE( "FortressDungeon_addBoulderCorridor" ) {
    utils::rng_srand( 5 );

    FortressDungeon dungeon;
    const FortressRoom* next = nullptr;
    next = dungeon.addRandomRoom( FortressRoomType::FR_DUNGEON_HEART, 5 );
    next = dungeon.createRoom( FortressRoomType::FR_EMPTY, 3, 3, *next, Direction::D_SOUTH, 3 );
    next = dungeon.createRoom( FortressRoomType::FR_BOULDER_CORRIDOR, 9, 3, *next, Direction::D_WEST, 1 );
    next = dungeon.createRoom( FortressRoomType::FR_BOULDER_CORRIDOR, 9, 3, *next, Direction::D_WEST, 1 );
    REQUIRE( next != nullptr );
    const std::size_t dSize = dungeon.size();
    REQUIRE( dSize == 4 );
}
