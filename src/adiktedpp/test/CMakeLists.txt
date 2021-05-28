#
#
#

if( NOT CMAKE_TESTING_ENABLED )  
    return()
endif()


set( TARGET_NAME adiktedpp_tests )


include_directories( "../include" )


file(GLOB_RECURSE cpp_files *.cpp )


add_definitions( -DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data" )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} PUBLIC adiktedpp PRIVATE Catch2::Catch2 )
#target_link_libraries( ${TARGET_NAME} PUBLIC ${adikted_LIBRARY} adiktedpp PRIVATE Catch2::Catch2 )


if (WIN32)
    ## for Windows target
    set( EXECUTABLE "wine ./${TARGET_NAME}" )
else()
    set( EXECUTABLE "./${TARGET_NAME}" )
endif()

generate_to_file( ../../run_tests.sh.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_${TARGET_NAME}.sh )
#generate_to_file( ../../run_tests.sh.in run_tests.sh )


add_test( NAME ${TARGET_NAME} COMMAND ${EXE_EMULATOR} ${TARGET_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
#add_test( NAME ${TARGET_NAME} COMMAND ${EXE_EMULATOR} ${TARGET_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
