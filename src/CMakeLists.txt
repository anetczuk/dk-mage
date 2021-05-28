##
##
##

cmake_minimum_required(VERSION 3.1.1)


if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}" )
    message( FATAL_ERROR "Building in source is forbidden. Change output directory.")
endif()


project( dk-mage )


set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

SET( EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" )

list( APPEND CMAKE_PREFIX_PATH "${EXTERNAL_DIR}/adikted/install" )
#list( APPEND CMAKE_INCLUDE_PATH "${EXTERNAL_DIR}/adikted/install" )
#list( APPEND CMAKE_LIBRARY_PATH "${EXTERNAL_DIR}/adikted/install" )

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



## ================= build =================


if( CMAKE_TESTING_ENABLED )   
    find_package( Catch2 REQUIRED )
endif()


add_subdirectory( adiktedpp )
add_subdirectory( dkmage )
add_subdirectory( cli )
