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


## ================= building global dependencies =================

include( FetchContent )


### build catch2 project
if( CMAKE_TESTING_ENABLED )
    FetchContent_Declare( catch2
                          URL ${EXTERNAL_DIR}/catch2/Catch2-2.13.6.zip
    )
    FetchContent_MakeAvailable( catch2 )
endif()


## ================= building modules =================

add_subdirectory( utils )
add_subdirectory( adiktedpp )
add_subdirectory( dkmage )
add_subdirectory( cli )


if( NOT EXISTS "bin/config.ini" )
    configure_file( config.ini.in bin/config.ini @ONLY )
endif()
