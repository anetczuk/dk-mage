#
#
#


set( TARGET_NAME utils )


### build filesystem project (headers only)
#FetchContent_Declare( filesystem
#                      SOURCE_DIR ${EXTERNAL_DIR}/filesystem/filesystem
#)
#FetchContent_MakeAvailable( filesystem )
set( filesystem_SOURCE_DIR "${EXTERNAL_DIR}/filesystem/filesystem" )


file(GLOB_RECURSE cpp_files *.cpp )


if (WIN32)
    ## for Windows target
    add_library( ${TARGET_NAME} ${cpp_files} )                  ## static linking
else()
    add_library( ${TARGET_NAME} SHARED ${cpp_files} )
endif()

target_include_directories( ${TARGET_NAME} PUBLIC ../include ${filesystem_SOURCE_DIR} )
target_link_libraries( ${TARGET_NAME} adikted )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
)
