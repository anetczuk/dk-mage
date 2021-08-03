/*
 *
 */

#include "catch2/catch.hpp"

#include "adiktedpp/LakeGenerator.h"


using namespace utils;
using namespace adiktedpp;


//inline std::string get_test_log() {
//    path outputPath = getTestOutputPath();
//    const std::string testName = Catch::getResultCapture().getCurrentTestName();
//    outputPath /= testName;
//    outputPath += std::string(".log");
//    return outputPath.u8string();
//}
//
//inline ScopeMessages initialize_messages() {
//    const std::string outputFile = get_test_log();
//    return ScopeMessages( outputFile );
//}


TEST_CASE("LakeGenerator_3x3", "[classic]") {
    LakeGenerator generator;
    const Rect rect( 3, 3 );
    generator.generateLake( rect, 1.0 );
    const std::size_t lakeSize = generator.added.size();
    CHECK( lakeSize == 9 );
}

TEST_CASE("LakeGenerator_4x4", "[classic]") {
    LakeGenerator generator;
    const Rect rect( 4, 4 );
    generator.generateLake( rect, 1.0 );
    const std::size_t lakeSize = generator.added.size();
    CHECK( lakeSize == 16 );
}

TEST_CASE("LakeGenerator_10x10", "[classic]") {
    LakeGenerator generator;
    const Rect rect( 20, 20 );
    generator.generateLake( rect, 1.0 );
    const std::size_t lakeSize = generator.added.size();
    CHECK( lakeSize == 400 );
}
