#
#
#


set( TARGET_NAME dkmage )


set( PUBLIC_HEADERS "../include" )

include_directories( ${PUBLIC_HEADERS} )

set( EXT_LIBS ${LEMON_LIBRARY} )

file(GLOB_RECURSE cpp_files *.cpp )
file(GLOB_RECURSE h_files ${PUBLIC_HEADERS}/*.h )


add_library( ${TARGET_NAME} SHARED ${cpp_files} )
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )

set_target_properties( ${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${h_files}" )

install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
