#
#
#


set( TARGET_NAME dkmagecli )


list( APPEND PUBLIC_HEADERS "../include" )

#include_directories( ${LEMON_INCLUDE_DIR} )
include_directories( "${EXTERNAL_DIR}/tclap/install/include" )          ## header-only library
include_directories( "${EXTERNAL_DIR}/inih/install/include" )
include_directories( "../../utils/include" )
include_directories( "../../adiktedpp/include" )
include_directories( "../../dkmage/include" )
include_directories( ${PUBLIC_HEADERS} )


set( EXT_LIBS utils dkmage )
#set( EXT_LIBS utils adiktedpp dkmage )

file(GLOB_RECURSE cpp_files *.cpp )
file(GLOB_RECURSE h_files ${PUBLIC_HEADERS}/*.h )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
