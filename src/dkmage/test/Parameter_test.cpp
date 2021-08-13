/*
 * Parameter_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/Parameter.h"

#include "utils/Rand.h"


using namespace utils;
using namespace dkmage;


TEST_CASE( "ParametersMap_getRawString" ) {
    {
        ParametersMap parameters;
        parameters.add( "param", "1,2,3" );
        const Optional< std::string > param = parameters.getRawString( "param" );
        REQUIRE( param == true );
        CHECK( param.value() == "1,2,3" );
    }
    {
        ParametersMap parameters;
        parameters.add( "param", "1:10" );
        const Optional< std::string > param = parameters.getRawString( "param" );
        REQUIRE( param == true );
        CHECK( param.value() == "1:10" );
    }
}

TEST_CASE( "ParametersMap_getString" ) {
    ParametersMap parameters;
    parameters.add( "param", "value1" );
    const Optional< std::string > param = parameters.getString( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == "value1" );
}

TEST_CASE( "ParametersMap_getString_list" ) {
    rng_srand( 3 );

    ParametersMap parameters;
    parameters.add( "param", "aaa,bbb,ccc,ddd" );
    const Optional< std::string > param = parameters.getString( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == "ccc" );
}

TEST_CASE( "ParametersMap_getString_list_emptyListElement" ) {
    rng_srand( 2 );

    ParametersMap parameters;
    parameters.add( "param", ",,," );
    const Optional< std::string > param = parameters.getString( "param" );
    REQUIRE( param == false );
}

TEST_CASE( "ParametersMap_getSizeT_item" ) {
    ParametersMap parameters;
    parameters.add( "param", 11 );
    const Optional< std::size_t > param = parameters.getSizeT( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == 11 );
}

TEST_CASE( "ParametersMap_getSizeT_list" ) {
    rng_srand( 3 );

    ParametersMap parameters;
    parameters.add( "param", "1,3,5,7" );
    const Optional< std::size_t > param = parameters.getSizeT( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == 5 );
}

TEST_CASE( "ParametersMap_getSizeT_range" ) {
    rng_srand( 21 );

    ParametersMap parameters;
    parameters.add( "param", "10:20" );
    const Optional< std::size_t > param = parameters.getSizeT( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == 18 );
}
