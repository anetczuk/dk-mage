/*
 * Parameter_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/Parameter.h"

#include "utils/Rand.h"


using namespace utils;
using namespace dkmage;


TEST_CASE( "NumberSet_sizet_getRandom_empty" ) {
    SizeTSet container;
    Optional<std::size_t> item = container.getRandom();
    REQUIRE( item == false );
}

TEST_CASE( "NumberSet_sizet_getRandom_1" ) {
    SizeTSet container;
    container.add( 5 );
    Optional<std::size_t> item = container.getRandom();
    REQUIRE( item == true );
    CHECK( item.value() == 5 );
}

TEST_CASE( "NumberSet_sizet_getRandom" ) {
    rng_srand( 2 );

    SizeTSet container;
    container.add( 5 );
    container.add( 10, 20 );

    Optional<std::size_t> item = container.getRandom();
    REQUIRE( item == true );
    CHECK( item.value() == 16 );
}

TEST_CASE( "NumberSet_sizet_getRandom_range_01" ) {
    rng_srand( 2 );

    SizeTSet container;
    container.add( 5 );
    container.add( 10, 20 );

    Optional<std::size_t> item = container.getRandom( 2, 7 );
    REQUIRE( item == true );
    CHECK( item.value() == 5 );
}

TEST_CASE( "NumberSet_sizet_getRandom_range_02" ) {
    rng_srand( 5 );

    SizeTSet container;
    container.add( 5 );
    container.add( 10, 20 );

    Optional<std::size_t> item = container.getRandom( 12, 17 );
    REQUIRE( item == true );
    CHECK( item.value() == 15 );
}

/// =====================================================

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

TEST_CASE( "ParametersMap_getSizeTSet_range" ) {
    rng_srand( 21 );

    ParametersMap parameters;
    parameters.add( "param", "5,10:20" );
    const Optional< SizeTSet > param = parameters.getSizeTSet( "param" );
    REQUIRE( param == true );
    const SizeTSet& numSet = param.value();
    CHECK( numSet.contains( 5 ) );
    CHECK( numSet.contains( 8 ) == false );
    CHECK( numSet.contains( 15 ) );
    CHECK( numSet.contains( 21 ) == false );
}
