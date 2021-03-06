##
##
##


if( NOT CMAKE_BUILD_TYPE )
    ##set( CMAKE_BUILD_TYPE "Debug")
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set( CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix" FORCE )
endif()

set( CMAKE_DEBUG_POSTFIX _d )

## set( CMAKE_INSTALL_DEFAULT_COMPONENT_NAME binaries )


##
## set default compilation standard
##
if (CMAKE_VERSION VERSION_LESS "3.1")
    ## handled by gcc and clang
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )
else()
    ## supported by CMake 3.1 and above
    set( CMAKE_C_STANDARD 99 )
    set( CMAKE_C_STANDARD_REQUIRED ON )
    set( CMAKE_CXX_STANDARD 14 )
    set( CMAKE_CXX_STANDARD_REQUIRED ON )
endif()


##
## compiler flags, gcc does not handle "appended" values
##
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra" )
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra" )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long" )

## static linking of libraries
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc" )
#set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )
#set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )
