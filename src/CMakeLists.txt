##
##
##


## 3.8  -- force C++ 17 standard
## 3.11 -- FetchContent
## 3.14 -- FetchContent_MakeAvailable
cmake_minimum_required( VERSION 3.14 )


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( dk-mage )


SET( EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" )

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )

list( APPEND CMAKE_PREFIX_PATH "${EXTERNAL_DIR}/adikted/install" )
list( APPEND CMAKE_PREFIX_PATH "${EXTERNAL_DIR}/catch2/install/lib/cmake" )


include( CMakeCompileFlags.txt )

include( CMakeCoverage.txt )

include( CMakeSanitizer.txt )

include( CMakeUtils.txt )


##
## enable CTest utility
##
enable_testing()


set( EXE_EMULATOR "" )
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    ## Linux host
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        ## Windows target
        set( EXE_EMULATOR "wine" )
    endif()
endif()


## ================= building dependencies =================

include( FetchContent )

### build inih project (header only)
FetchContent_Declare( inih
                      URL ${EXTERNAL_DIR}/inih/inih-master.zip
                      SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/inih-src/inih
)
FetchContent_Populate( inih )
set( inih_INCLUDE_DIR "${inih_SOURCE_DIR}/.." )


### build adikted project
FetchContent_Declare( adikted
                      URL ${EXTERNAL_DIR}/adikted/ADiKtEd-master.zip
                      SOURCE_SUBDIR libadikted
)
IF(WIN32)
    set( adikted_TYPE STATIC )
ENDIF()
FetchContent_MakeAvailable( adikted )
## install( TARGETS adikted LIBRARY DESTINATION "." )


### build lemon project
FetchContent_Declare( lemon
                      SOURCE_DIR ${EXTERNAL_DIR}/lemon/src/lemon-1.3.1
)
set( LEMON_THREADING "None" )
FetchContent_MakeAvailable( lemon )


## ================= building modules =================

if( CMAKE_TESTING_ENABLED )   
    find_package( Catch2 REQUIRED )
endif()


add_subdirectory( utils )
add_subdirectory( adiktedpp )
add_subdirectory( dkmage )
add_subdirectory( cli )
