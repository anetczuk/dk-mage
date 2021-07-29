##
##
##

if( NOT DESTINATION_DIR )
    message( FATAL_ERROR "Argument DESTINATION_DIR not set. Pass -DDESTINATION_DIR=<path> to script." )
endif()

set( patch_files ${CMAKE_CURRENT_LIST_DIR}/fix/ )
message( STATUS "copying ${patch_files} to ${DESTINATION_DIR}" )

file( COPY ${patch_files} DESTINATION ${DESTINATION_DIR} )



#file( GLOB_RECURSE patch_files ${CMAKE_CURRENT_LIST_DIR}/fix/* )
#message( STATUS "copying ${patch_files} to ${DESTINATION_DIR}" )

#file( COPY ${patch_files} DESTINATION ${DESTINATION_DIR} )
