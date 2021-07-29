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


## ================= building dependencies =================

include( FetchContent )


### build filesystem project (headers only)
#FetchContent_Declare( filesystem
#                      SOURCE_DIR ${EXTERNAL_DIR}/filesystem/filesystem
#)
#FetchContent_MakeAvailable( filesystem )
set( filesystem_SOURCE_DIR "${EXTERNAL_DIR}/filesystem/filesystem" )


### build inih project (headers only)
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
set( lemon_UNPACK_DIR ${FETCHCONTENT_BASE_DIR}/lemon-src )
set( lemon_patch_command ${CMAKE_COMMAND} -DDESTINATION_DIR=${lemon_UNPACK_DIR}/ -P ${EXTERNAL_DIR}/lemon/patch.cmake )
FetchContent_Declare( lemon
                      URL ${EXTERNAL_DIR}/lemon/lemon-1.3.1.zip
                      SOURCE_DIR ${lemon_UNPACK_DIR}
                      PATCH_COMMAND ${lemon_patch_command}
)
set( LEMON_THREADING "None" )
FetchContent_MakeAvailable( lemon )


### build tclap project
set( tclap_UNPACK_DIR ${FETCHCONTENT_BASE_DIR}/tclap-src )
set( tclap_patch_command ${CMAKE_COMMAND} -DDESTINATION_DIR=${tclap_UNPACK_DIR}/ -P ${EXTERNAL_DIR}/tclap/patch.cmake )
set( BUILD_TESTING OFF CACHE BOOL "" FORCE )
FetchContent_Declare( tclap
                      URL ${EXTERNAL_DIR}/tclap/tclap-1.4.0-rc1.tar.bz2
                      SOURCE_DIR ${tclap_UNPACK_DIR}
                      PATCH_COMMAND ${tclap_patch_command}
)
FetchContent_MakeAvailable( tclap )
set( tclap_INCLUDE_DIR ${tclap_SOURCE_DIR}/include )


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
