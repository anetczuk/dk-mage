/*
 * Log_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "utils/Log.h"


using namespace utils;


TEST_CASE( "PrintLogSink_null" ) {
    PrintLogSink sink( nullptr );
    sink.printMessage( LogLevel::LL_INFO, "file.cpp", 123, "xxx" );
    CHECK( true );
}


static int LOG_COUNTER = 0;

void print_log( const LogLevel /*level*/, const std::string& /*file*/, const std::size_t /*lineNo*/, const std::string& /*message*/ ) {
    LOG_COUNTER += 1;
}

TEST_CASE( "PrintLogSink_function" ) {
    PrintLogSink::PrintFunction func = print_log;
    PrintLogSink sink( &func );
    CHECK( LOG_COUNTER == 0 );
    sink.printMessage( LogLevel::LL_INFO, "file.cpp", 123, "xxx" );
    CHECK( LOG_COUNTER == 1 );
}
