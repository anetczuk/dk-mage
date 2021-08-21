/*
 * ProbabilityMass_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "utils/ProbabilityMass.h"

#include "utils/Rand.h"


using namespace utils;


TEST_CASE( "ProbabilityMass_get" ) {
    {
        const unsigned int timeSeed = time(NULL);
        rng_srand( timeSeed );

        ProbabilityMass<int> probability;
        probability.set( 1, 10.0 );
        probability.set( 2,  0.0 );
        probability.set( 3, 10.0 );
        probability.normalize();

        for ( std::size_t i=0; i<100; ++i ) {
            const double num = rng_randd();
            const int value = probability.get( num );
            REQUIRE( value != 2 );
        }
    }
}

TEST_CASE( "ProbabilityMass_pop" ) {
    {
        rng_srand( 0 );

        ProbabilityMass<int> probability;
        probability.set( 1, 10.0, 1 );
        probability.set( 3, 10.0, 1 );
        probability.normalize();
        REQUIRE( probability.size() == 2 );


        const int value = probability.popRandom();
        REQUIRE( value == 3 );
        REQUIRE( probability.size() == 1 );
    }
}

