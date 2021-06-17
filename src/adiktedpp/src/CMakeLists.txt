#
#
#


set( TARGET_NAME adiktedpp )


set( PUBLIC_HEADERS "../include" )

include_directories( ${PUBLIC_HEADERS} )


file(GLOB_RECURSE cpp_files *.cpp )
file(GLOB_RECURSE h_files ${PUBLIC_HEADERS}/*.h )


if (WIN32)
    ## for Windows target
    add_library( ${TARGET_NAME} ${cpp_files} )                  ## static linking
else()
    add_library( ${TARGET_NAME} SHARED ${cpp_files} )
endif()

target_link_libraries( ${TARGET_NAME} ${adikted_LIBRARY} utils )

set_target_properties( ${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${h_files}" )

install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
