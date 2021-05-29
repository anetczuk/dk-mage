/*
 * Messages_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "adiktedpp/Messages.h"


using namespace adiktedpp;


TEST_CASE("Messages_log", "[classic]") {
    Messages& messages = Messages::get( Catch::getResultCapture().getCurrentTestName(), ".log" );
    messages.log( "test message 1" );
    REQUIRE( true );
}
