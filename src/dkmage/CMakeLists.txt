##
##
##


find_package( LEMON REQUIRED )


include_directories( ${LEMON_INCLUDE_DIR} )


add_subdirectory( src )

add_subdirectory( test )
