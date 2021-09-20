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


project( dk-mage VERSION 1.5.2 )


SET( EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" )
SET( DOC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../doc" )

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )


include( CMakeCompileFlags )

include( CMakeCoverage )

include( CMakeSanitizer )

include( CMakeUtils )


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


### build adikted project
FetchContent_Declare( adikted
                      URL ${EXTERNAL_DIR}/adikted/ADiKtEd-master.zip
                      
## Hash is only checked once during first configuration (extract of zip).
##                      URL_HASH MD5=0a5c6b4259775a80c6c1f54bc4811234

## It has been found that 'SOURCE_SUBDIR' does not work under WLS (Ubuntu 20 & cmake 3.16). 
## Workaround is to maunally execute 'FetchContent_Populate()' instead of 'FetchContent_MakeAvailable()'.
##                      SOURCE_SUBDIR libadikted
)
IF(WIN32)
    set( adikted_TYPE STATIC )
ENDIF()

SET( USE_RNG_MT ON CACHE BOOL "Force Mersenne Twister RNG" FORCE )

FetchContent_GetProperties( adikted )
if( NOT adikted_POPULATED )   
    FetchContent_Populate( adikted )
    add_subdirectory( ${adikted_SOURCE_DIR}/libadikted ${adikted_BINARY_DIR} )
endif()
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options( adikted PRIVATE "-w" )
endif()
## install( TARGETS adikted LIBRARY DESTINATION "." )


## ================= building modules =================


add_subdirectory( utils )
add_subdirectory( adiktedpp )
add_subdirectory( dkmage )
add_subdirectory( cli )

option( BUILD_PYTHON_BINDINGS "Generate Python bindings (SWIG)" OFF )
if( BUILD_PYTHON_BINDINGS )
    add_subdirectory( pyutils )
    add_subdirectory( pyadiktedpp )
endif()


set( CUSTOM_CONFIG_PATH "" CACHE FILEPATH "Path to custom config.ini" )
include( ParameterDefaults.cmake )
configure_file( config.ini.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config-fresh.ini @ONLY )
if( EXISTS "${CUSTOM_CONFIG_PATH}" )
    ## custom config -- copy
    configure_file( ${CUSTOM_CONFIG_PATH} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini COPYONLY )
else()
    if( NOT EXISTS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini" )
        configure_file( config.ini.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini @ONLY )
    endif()
endif()


if (WIN32)
    ## for Windows target
    set( EXECUTABLE_PATH "wine ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dkmagecli.exe" )
else()
    set( EXECUTABLE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dkmagecli" )
endif()

generate_to_file( rebuild.sh.in rebuild.sh )

generate_to_file( generate_doc.sh.in generate_doc.sh )

generate_to_file( deploy_zip.sh.in deploy_zip.sh )
