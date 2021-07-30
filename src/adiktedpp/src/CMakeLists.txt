#
#
#


set( TARGET_NAME adiktedpp )


### build adikted project
FetchContent_Declare( adikted
                      URL ${EXTERNAL_DIR}/adikted/ADiKtEd-master.zip
## It has been found that 'SOURCE_SUBDIR' does not work under WLS (Ubuntu 20 & cmake 3.16). 
## Workaround is to maunally execute 'FetchContent_Populate()' instead of 'FetchContent_MakeAvailable()'.
##                      SOURCE_SUBDIR libadikted
)
IF(WIN32)
    set( adikted_TYPE STATIC )
ENDIF()
FetchContent_GetProperties( adikted )
if( NOT adikted_POPULATED )   
    FetchContent_Populate( adikted )
    add_subdirectory( ${adikted_SOURCE_DIR}/libadikted ${adikted_BINARY_DIR} )
endif()
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options( adikted PRIVATE "-w" )
endif()
## install( TARGETS adikted LIBRARY DESTINATION "." )


file(GLOB_RECURSE cpp_files *.cpp )


if (WIN32)
    ## for Windows target
    add_library( ${TARGET_NAME} ${cpp_files} )                  ## static linking
else()
    add_library( ${TARGET_NAME} SHARED ${cpp_files} )
endif()

target_include_directories( ${TARGET_NAME} PUBLIC "../include" )
target_link_libraries( ${TARGET_NAME} adikted utils )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
)
