#
#
#


set( TARGET_NAME dkmagecli )


list( APPEND PUBLIC_HEADERS "../include" )

include_directories( "${EXTERNAL_DIR}/tclap/install/include" )          ## header-only library
include_directories( ${inih_INCLUDE_DIR} )
include_directories( "${EXTERNAL_DIR}/filesystem/filesystem" )
include_directories( ${PUBLIC_HEADERS} )


file(GLOB_RECURSE cpp_files *.cpp )
file(GLOB_RECURSE h_files ${PUBLIC_HEADERS}/*.h )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} utils dkmage )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
#         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
