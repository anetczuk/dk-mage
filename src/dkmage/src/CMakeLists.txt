#
#
#


set( TARGET_NAME dkmage )


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
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options( lemon PRIVATE "-w" )
endif()


file(GLOB_RECURSE cpp_files *.cpp )


if (WIN32)
    ## for Windows target
    add_library( ${TARGET_NAME} ${cpp_files} )                  ## static linking
else()
    add_library( ${TARGET_NAME} SHARED ${cpp_files} )
endif()

target_include_directories( ${TARGET_NAME} PUBLIC "../include" )
target_link_libraries( ${TARGET_NAME} lemon utils adiktedpp )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
)
