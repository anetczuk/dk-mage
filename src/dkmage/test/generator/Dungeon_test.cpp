/*
 * Dungeon_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/generator/Dungeon.h"


using namespace dkmage;
using namespace generator;


TEST_CASE( "Rect_dimm_zero" ) {
    {
        Rect rect{ Point(0,0), Point(0,0) };
        CHECK( rect.width()  == 1 );
        CHECK( rect.height() == 1 );
    }
    {
        Rect rect( 1, 1 );
        CHECK( rect.width()  == 1 );
        CHECK( rect.height() == 1 );
    }
}

TEST_CASE( "Rect_dimm" ) {
    {
        Rect rect{ Point(0,0), Point(4,4) };
        CHECK( rect.width()  == 5 );
        CHECK( rect.height() == 5 );
    }
    {
        Rect rect( 5, 5 );
        CHECK( rect.width()  == 5 );
        CHECK( rect.height() == 5 );
    }
}

TEST_CASE( "Rect_centerize_odd" ) {
    Rect rect( 5, 5 );
    rect.centerize();
    CHECK( rect.min.x == -2 );
    CHECK( rect.min.y == -2 );
    CHECK( rect.max.x ==  2 );
    CHECK( rect.max.y ==  2 );
}

TEST_CASE( "Rect_centerize_even" ) {
    Rect rect( 4, 4 );
    rect.centerize();
    CHECK( rect.min.x == -2 );
    CHECK( rect.min.y == -2 );
    CHECK( rect.max.x ==  1 );
    CHECK( rect.max.y ==  1 );
}

TEST_CASE( "Rect_center" ) {
    {
        Rect rect( 5, 5 );
        const Point center = rect.center();
        CHECK( center.x == 2 );
        CHECK( center.y == 2 );
    }
    {
        Rect rect( 5, 5 );
        rect.move( 10, 10 );
        const Point center = rect.center();
        CHECK( center.x == 12 );
        CHECK( center.y == 12 );
    }
    {
        Rect rect( 5, 5 );
        rect.centerize();
        const Point center = rect.center();
        CHECK( center.x == 0 );
        CHECK( center.y == 0 );
    }
}


/// =============================================


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
