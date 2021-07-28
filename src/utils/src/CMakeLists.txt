#
#
#


set( TARGET_NAME utils )


include_directories( "${EXTERNAL_DIR}/filesystem/filesystem" )


file(GLOB_RECURSE cpp_files *.cpp )


if (WIN32)
    ## for Windows target
    add_library( ${TARGET_NAME} ${cpp_files} )                  ## static linking
else()
    add_library( ${TARGET_NAME} SHARED ${cpp_files} )
endif()

target_include_directories( ${TARGET_NAME} PUBLIC "../include" )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
)
