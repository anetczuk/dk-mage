/*
 * Rect_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "utils/Rect.h"


using namespace utils;


TEST_CASE( "Point_less" ) {
    {
        Point p1(0,0);
        Point p2(1,0);
        const bool less = p1 < p2;
        CHECK( less == true );
    }
    {
        Point p1(0,0);
        Point p2(0,1);
        const bool less = p1 < p2;
        CHECK( less == true );
    }

    {
        Point p1(0,0);
        Point p2(0,0);
        const bool less = p1 < p2;
        CHECK( less == false );
    }
    {
        Point p1(1,0);
        Point p2(0,1);
        const bool less = p1 < p2;
        CHECK( less == false );
    }
}

TEST_CASE( "Point_distance" ) {
    {
        Point p1(0,0);
        Point p2(0,0);
        const std::size_t dist = p1.distanceManhattan( p2 );
        CHECK( dist == 0 );
    }
    {
        Point p1(0,0);
        Point p2(1,0);
        const std::size_t dist = p1.distanceManhattan( p2 );
        CHECK( dist == 1 );
    }
    {
        Point p1(0,0);
        Point p2(1,1);
        const std::size_t dist = p1.distanceManhattan( p2 );
        CHECK( dist == 2 );
    }
}

/// ================================================

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
    CHECK( rect.min.x == -1 );
    CHECK( rect.min.y == -1 );
    CHECK( rect.max.x ==  2 );
    CHECK( rect.max.y ==  2 );
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
    {
        const Rect rect( 3, 3, 3, 3 );
        const Point center = rect.center();
        CHECK( center.x == 3 );
        CHECK( center.y == 3 );
    }
    {
        const Rect rect( 3, 3, 5, 5 );
        const Point center = rect.center();
        CHECK( center.x == 4 );
        CHECK( center.y == 4 );
    }
}

TEST_CASE( "Rect_distance" ) {
    {
        Rect rect1( 5, 5 );
        Rect rect2( 5, 5 );
        const Point dist = rect1.distance( rect2 );
        CHECK( dist.x == 0 );
        CHECK( dist.y == 0 );
    }
    {
        Rect rect1( Point(0,0), Point(2,2) );
        Rect rect2( Point(3,0), Point(5,2) );
        const Point dist = rect1.distance( rect2 );
        CHECK( dist.x == 1 );
        CHECK( dist.y == 0 );
    }
    {
        Rect rect1( 5, 5 );
        Rect rect2( 5, 5 );
        rect2.move( 20, 20 );
        const Point dist = rect1.distance( rect2 );
        CHECK( dist.x == 16 );
        CHECK( dist.y == 16 );
    }
    {
        /// common
        Rect rect1( 0, 0, 5, 5 );
        Rect rect2( 3, 3, 8, 8 );
        const Point dist = rect1.distance( rect2 );
        CHECK( dist.x == 0 );
        CHECK( dist.y == 0 );
    }
    {
        /// on side
        Rect rect1( 0, 0, 5, 5 );
        Rect rect2( 6, 0, 10, 5 );
        const Point dist = rect1.distance( rect2 );
        CHECK( dist.x == 1 );
        CHECK( dist.y == 0 );
    }
}

TEST_CASE( "Rect_distanceChebyshev" ) {
    {
        const Rect rect( 1, 1 );
        const Point point(0, 0);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 0 );
    }
    {
        const Rect rect( 1, 1 );
        const Point point(1, 0);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 1 );
    }
    {
        const Rect rect( 1, 1 );
        const Point point(0, 1);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 1 );
    }

    {
        const Rect rect( 10, 10 );
        const Point point(-1, -2);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 2 );
    }
    {
        const Rect rect( 10, 10 );
        const Point point(10, 10);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 1 );
    }
    {
        const Rect rect( 10, 10 );
        const Point point(11, 12);
        const std::size_t dist = rect.distanceChebyshev( point );
        CHECK( dist == 3 );
    }
}

TEST_CASE( "Rect_isCollision" ) {
    {
        Rect rect1( 5, 5 );
        Rect rect2( 5, 5 );
        const bool collision = rect1.isCollision( rect2 );
        CHECK( collision == true );
    }
    {
        Rect rect1( Point(0,0), Point(2,2) );
        Rect rect2( Point(3,0), Point(5,2) );
        const bool collision = rect1.isCollision( rect2 );
        CHECK( collision == false );
    }
    {
        Rect rect1( 5, 5 );
        Rect rect2( 5, 5 );
        rect2.move( 20, 20 );
        const bool collision = rect1.isCollision( rect2 );
        CHECK( collision == false );
    }
    {
        /// common
        Rect rect1( 0, 0, 5, 5 );
        Rect rect2( 3, 3, 8, 8 );
        const bool collision = rect1.isCollision( rect2 );
        CHECK( collision == true );
    }
    {
        /// on side
        Rect rect1( 0, 0, 5, 5 );
        Rect rect2( 6, 0, 10, 5 );
        const bool collision = rect1.isCollision( rect2 );
        CHECK( collision == false );
    }
}

/// ===============================================================

TEST_CASE( "line_horizontal" ) {
    {
        const Point p1( 0, 0 );
        const Point p2( 3, 0 );
        const std::vector<Point> aLine = line( p1, p2 );
        REQUIRE( aLine.size() == 4 );
        CHECK( aLine[0] == Point(0,0) );
        CHECK( aLine[1] == Point(1,0) );
        CHECK( aLine[2] == Point(2,0) );
        CHECK( aLine[3] == Point(3,0) );
    }
}

TEST_CASE( "line_angled" ) {
    {
        const Point p1( 0, 0 );
        const Point p2( 3, 1 );
        const std::vector<Point> aLine = line( p1, p2 );
        REQUIRE( aLine.size() == 5 );
        CHECK( aLine[0] == Point(0,0) );
        CHECK( aLine[1] == Point(1,0) );
        CHECK( aLine[2] == Point(2,0) );
        CHECK( aLine[3] == Point(2,1) );
        CHECK( aLine[4] == Point(3,1) );
    }
}

TEST_CASE( "line_diagonal" ) {
    {
        const Point p1( 0, 0 );
        const Point p2( 3, 3 );
        const std::vector<Point> aLine = line( p1, p2 );
        REQUIRE( aLine.size() == 7 );
        CHECK( aLine[0] == Point(0,0) );
        CHECK( aLine[1] == Point(0,1) );
        CHECK( aLine[2] == Point(1,1) );
        CHECK( aLine[3] == Point(1,2) );
        CHECK( aLine[4] == Point(2,2) );
        CHECK( aLine[5] == Point(2,3) );
        CHECK( aLine[6] == Point(3,3) );
    }
}
