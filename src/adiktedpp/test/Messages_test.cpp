/*
 * Messages_test.cpp
 *
 */

#include "catch2/catch.hpp"
#include "Path.h"

#include "adiktedpp/Messages.h"


using namespace adiktedpp;


inline std::string get_test_log() {
    path outputPath = getTestOutputPath();
    const std::string testName = Catch::getResultCapture().getCurrentTestName();
    outputPath /= testName;
    outputPath += std::string(".log");
    return outputPath.u8string();
}


TEST_CASE("Messages_log", "[classic]") {
    const std::string outputFile = get_test_log();
    Messages& messages = Messages::get( outputFile );
    messages.log( "test message 1" );
    REQUIRE( true );
}
