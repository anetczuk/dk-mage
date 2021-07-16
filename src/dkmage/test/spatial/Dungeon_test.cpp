/*
 * Dungeon_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/spatial/Dungeon.h"


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
    srand( 2 );

    Dungeon dungeon;
    dungeon.generate(2, 5);
    const Rect rect = dungeon.boundingBox();
    REQUIRE( rect.area() == 55 );               /// 2x25 + 5
    CHECK( rect.min.x == -8 );
    CHECK( rect.min.y == -2 );
    CHECK( rect.max.x ==  2 );
    CHECK( rect.max.y ==  2 );
}

TEST_CASE( "Dungeon_addRoom_collision" ) {
    Dungeon dungeon;
    Room* first = dungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 5 );
    REQUIRE( first != nullptr );

    Room* next = first;
    next = dungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 5, *next, Direction::D_WEST );
    REQUIRE( next != nullptr );

    next = dungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 5, *next, Direction::D_NORTH );
    REQUIRE( next != nullptr );

    next = dungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 5, *next, Direction::D_EAST );
    REQUIRE( next != nullptr );

    Room* last = dungeon.addRoom( adiktedpp::SlabType::ST_TREASURE, 5, *next, Direction::D_SOUTH );
    REQUIRE( last == nullptr );

    REQUIRE( dungeon.roomsNum() == 4 );
}
