#
#
#


set( TARGET_NAME dkmagecli )


set( PUBLIC_HEADERS "../dkmage/include" )


set( EXT_LIBS  )

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
