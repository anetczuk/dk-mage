##
## option for code coverage
##

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    option(CODE_COVERAGE "Code coverage (for GCC)" OFF )
    if ( CODE_COVERAGE )
        ## to check compilation parameters run "make VERBOSE=1"
        ## to test is program is compiled properly call "strings test.exe | grep gcda"
        
        set( COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage" )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
    endif()
    
    option(GPROF_ANALYSIS "GProf analysis (for GCC)" OFF )
    if ( GPROF_ANALYSIS )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg" )
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    option(CODE_COVERAGE "Code coverage (for GCC)" OFF )
    if ( CODE_COVERAGE )
        ## to check compilation parameters run "make VERBOSE=1"
        ## to test is program is compiled properly call "strings test.exe | grep gcda"
        
        set( COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" )
    endif()
    
    option(GPROF_ANALYSIS "GProf analysis (for GCC)" OFF )
    if ( GPROF_ANALYSIS )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg" )
    endif()
endif()
