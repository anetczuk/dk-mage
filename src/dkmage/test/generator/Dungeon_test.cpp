/*
 * Dungeon_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/generator/Dungeon.h"


using namespace dkmage;
using namespace generator;


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
