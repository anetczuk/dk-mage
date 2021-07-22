##
##
##


## 3.8 -- force C++ 17 standard
cmake_minimum_required(VERSION 3.8)


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


set( KEEPER_DATA "" CACHE PATH "Path to DK data directory" )

if ( NOT KEEPER_DATA )
    message( WARNING "path to game's data directory not set (KEEPER_DATA)" )
endif()

add_definitions( -DKEEPER_DATA_PATH="${KEEPER_DATA}" )


set( KEEPER_LEVELS "" CACHE PATH "Path to DK levels directory" )

if ( NOT KEEPER_LEVELS )
    message( WARNING "path to game's levels directory not set (KEEPER_LEVELS)" )
endif()

add_definitions( -DKEEPER_LEVELS_PATH="${KEEPER_LEVELS}" )


## ================= build =================

include(ExternalProject)


ExternalProject_Add( lemon_project
                     SOURCE_DIR ${EXTERNAL_DIR}/lemon/src/lemon-1.3.1
                     PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lemon_project
                     CMAKE_ARGS -DLEMON_THREADING="None" -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/lemon_project
                    )
                     
ExternalProject_Get_Property( lemon_project install_dir )
set( LEMON_INCLUDE_DIR ${install_dir}/include )
set( LEMON_BINARY_DIR ${install_dir}/lib )

## create additional library forcing other libraries to wait until external project is installed
add_library( lemon UNKNOWN IMPORTED )
set_target_properties( lemon PROPERTIES IMPORTED_LOCATION ${LEMON_BINARY_DIR}/libemon.a )
add_dependencies( lemon lemon_project )

set( LEMON_LIBRARY lemon )


if( CMAKE_TESTING_ENABLED )   
    find_package( Catch2 REQUIRED )
endif()


include_directories( "utils/include" )

add_subdirectory( utils )
add_subdirectory( adiktedpp )
add_subdirectory( dkmage )
add_subdirectory( cli )
