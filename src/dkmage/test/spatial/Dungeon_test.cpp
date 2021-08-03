/*
 * Dungeon_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/spatial/Dungeon.h"


using namespace adiktedpp;
using namespace dkmage;
using namespace spatial;


TEST_CASE( "Dungeon_boundingBox" ) {
    Dungeon dungeon;
    dungeon.generate(1, 3);
    const Rect rect = dungeon.boundingBox();
    REQUIRE( rect.area() == 9 );
    CHECK( rect.min.x == -1 );
    CHECK( rect.min.y == -1 );
    CHECK( rect.max.x ==  1 );
    CHECK( rect.max.y ==  1 );
}

TEST_CASE( "Dungeon_boundingBox_02" ) {
    Dungeon dungeon;
    DungeonRoom* room1 = dungeon.addRoom( Room::R_DUNGEON_HEART, 5 );
    REQUIRE( room1 != nullptr );
    DungeonRoom* room2 = dungeon.addRoom( Room::R_TREASURE, 5, *room1, Direction::D_WEST );
    REQUIRE( room2 != nullptr );

    const Rect rect = dungeon.boundingBox();
    REQUIRE( rect.area() == 55 );               /// 2x25 + 5
    CHECK( rect.min.x == -8 );
    CHECK( rect.min.y == -2 );
    CHECK( rect.max.x ==  2 );
    CHECK( rect.max.y ==  2 );
}

TEST_CASE( "Dungeon_addRoom_collision" ) {
    Dungeon dungeon;
    DungeonRoom* first = dungeon.addRoom( Room::R_TREASURE, 5 );
    REQUIRE( first != nullptr );

    DungeonRoom* next = first;
    next = dungeon.addRoom( Room::R_TREASURE, 5, *next, Direction::D_WEST );
    REQUIRE( next != nullptr );

    next = dungeon.addRoom( Room::R_TREASURE, 5, *next, Direction::D_NORTH );
    REQUIRE( next != nullptr );

    next = dungeon.addRoom( Room::R_TREASURE, 5, *next, Direction::D_EAST );
    REQUIRE( next != nullptr );

    DungeonRoom* last = dungeon.addRoom( Room::R_TREASURE, 5, *next, Direction::D_SOUTH );
    REQUIRE( last == nullptr );

    REQUIRE( dungeon.roomsNum() == 4 );
}
