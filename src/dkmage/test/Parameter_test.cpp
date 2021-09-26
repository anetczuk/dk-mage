/*
 * Parameter_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/Parameter.h"

#include "dkmage/Generator.h"

#include "adiktedpp/Rand.h"


using namespace utils;
using namespace dkmage;


TEST_CASE( "NumberSet_sizet_valueByIndex" ) {
    SizeTSet container;
    container.add( 5 );
    container.add( 10, 12 );
    {
        Optional<std::size_t> item = container.valueByIndex( 0 );
        REQUIRE( item == true );
        CHECK( item.value() == 5 );
    }
    {
        Optional<std::size_t> item = container.valueByIndex( 1);
        REQUIRE( item == true );
        CHECK( item.value() == 10 );
    }
    {
        Optional<std::size_t> item = container.valueByIndex( 2 );
        REQUIRE( item == true );
        CHECK( item.value() == 11 );
    }
    {
        Optional<std::size_t> item = container.valueByIndex( 3 );
        REQUIRE( item == true );
        CHECK( item.value() == 12 );
    }
}

TEST_CASE( "NumberSet_sizet_valueByFactor_empty" ) {
    SizeTSet container;
    const std::size_t item = container.valueByFactor( 0.5 );
    REQUIRE( item == 0 );
}

TEST_CASE( "NumberSet_sizet_valueByFactor_1" ) {
    SizeTSet container;
    container.add( 5 );
    const std::size_t item = container.valueByFactor( 0.5 );
    CHECK( item == 5 );
}

TEST_CASE( "NumberSet_sizet_valueByFactor" ) {
    SizeTSet container;
    container.add( 5 );
    container.add( 10, 12 );
    {
        const std::size_t item = container.valueByFactor( 0.2 );
        CHECK( item == 5 );
    }
    {
        const std::size_t item = container.valueByFactor( 0.4 );
        CHECK( item == 10 );
    }
    {
        const std::size_t item = container.valueByFactor( 0.6 );
        CHECK( item == 11 );
    }
    {
        const std::size_t item = container.valueByFactor( 0.8 );
        CHECK( item == 12 );
    }
}

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
    adiktedpp::rng_srand( 2 );

    SizeTSet container;
    container.add( 5 );
    container.add( 10, 20 );

    Optional<std::size_t> item = container.getRandom();
    REQUIRE( item == true );
    CHECK( item.value() == 16 );
}

TEST_CASE( "NumberSet_sizet_getRandom_range_01" ) {
    adiktedpp::rng_srand( 2 );

    SizeTSet container;
    container.add( 5 );
    container.add( 10, 20 );

    Optional<std::size_t> item = container.getRandom( 2, 7 );
    REQUIRE( item == true );
    CHECK( item.value() == 5 );
}

TEST_CASE( "NumberSet_sizet_getRandom_range_02" ) {
    adiktedpp::rng_srand( 5 );

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
    adiktedpp::rng_srand( 3 );

    ParametersMap parameters;
    parameters.add( "param", "aaa,bbb,ccc,ddd" );
    const Optional< std::string > param = parameters.getString( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == "ccc" );
}

TEST_CASE( "ParametersMap_getString_list_emptyListElement" ) {
    adiktedpp::rng_srand( 2 );

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
    adiktedpp::rng_srand( 3 );

    ParametersMap parameters;
    parameters.add( "param", "1,3,5,7" );
    const Optional< std::size_t > param = parameters.getSizeT( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == 5 );
}

TEST_CASE( "ParametersMap_getSizeT_range" ) {
    adiktedpp::rng_srand( 21 );

    ParametersMap parameters;
    parameters.add( "param", "10:20" );
    const Optional< std::size_t > param = parameters.getSizeT( "param" );
    REQUIRE( param == true );
    CHECK( param.value() == 18 );
}

TEST_CASE( "ParametersMap_getSizeTSet_range" ) {
    adiktedpp::rng_srand( 21 );

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
