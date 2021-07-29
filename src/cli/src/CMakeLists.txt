#
#
#


set( TARGET_NAME dkmagecli )


file(GLOB_RECURSE cpp_files *.cpp )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_include_directories( ${TARGET_NAME} PRIVATE ../include ${inih_INCLUDE_DIR} ${tclap_INCLUDE_DIR} )
target_link_libraries( ${TARGET_NAME} utils dkmage )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
#         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
