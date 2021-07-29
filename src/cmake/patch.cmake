##
## Copy files with subdirectories without deleting target directory before the operation.
##

if( NOT SOURCE_DIR )
    message( FATAL_ERROR "Argument SOURCE_DIR not set. Pass -DSOURCE_DIR=<path> to script." )
endif()

if( NOT DESTINATION_DIR )
    message( FATAL_ERROR "Argument DESTINATION_DIR not set. Pass -DDESTINATION_DIR=<path> to script." )
endif()


message( STATUS "copying ${SOURCE_DIR} to ${DESTINATION_DIR}" )
file( COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR} )
