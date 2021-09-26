/*
 * ProbabilityMass_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "utils/ProbabilityMass.h"

#include <random>


using namespace utils;


TEST_CASE( "ProbabilityMass_get" ) {
    const unsigned int timeSeed = time(NULL);
    srand( timeSeed );

    ProbabilityMass<int> probability;
    probability.set( 1, 10.0 );
    probability.set( 2,  0.0 );
    probability.set( 3, 10.0 );
    probability.normalize();

    for ( std::size_t i=0; i<100; ++i ) {
        const double num = ((double) rand()) / RAND_MAX;
        const int value = probability.get( num );
        REQUIRE( value != 2 );
    }
}

TEST_CASE( "ProbabilityMass_pop" ) {
    ProbabilityMass<int> probability;
    probability.set( 1, 10.0, 1 );
    probability.set( 3, 10.0, 1 );
    probability.normalize();
    REQUIRE( probability.size() == 2 );

    const int value = probability.pop( 0.5 );
    REQUIRE( value == 3 );
    REQUIRE( probability.size() == 1 );
}

