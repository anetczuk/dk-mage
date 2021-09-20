##
##
##


set( TARGET_NAME pyadiktedpp_tests )


file(GLOB_RECURSE py_files *.py )
foreach( item IN ITEMS ${py_files} )
    configure_file( ${item} ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
endforeach()


set( EXECUTABLE_BASE "${Python3_EXECUTABLE} -m unittest" )
if (WIN32)
    ## for Windows target
    set( EXECUTABLE_PATH "wine ${EXECUTABLE_BASE}" )
else()
    set( EXECUTABLE_PATH "${EXECUTABLE_BASE}" )
endif()

set( ADD_PYTHON_PATH ${PROJECT_BINARY_DIR}/pyutils )


set( TESTS_SCRIPT run_${TARGET_NAME}.sh )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )
generate_to_file( run_tests.sh.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TESTS_SCRIPT} )

add_test( NAME ${TARGET_NAME} COMMAND ${EXE_EMULATOR} ${TESTS_SCRIPT} )
