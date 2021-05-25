#
#
#

if( NOT CMAKE_TESTING_ENABLED )  
    return()
endif()


set( TARGET_NAME dkmage_tests )


include_directories( "../include" )


set( EXT_LIBS ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} dkmage )

file(GLOB_RECURSE cpp_files *.cpp )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )

set( EXECUTABLE "./${TARGET_NAME}" )
generate_file( run_tests.sh.in )

add_test( NAME ${TARGET_NAME} COMMAND ${TARGET_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        

#TODO: copying data and scripts can be optimised by creating script from template with defined data directry

## copy run script
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runTests.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

## copy data directory
#file(COPY refimg DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
