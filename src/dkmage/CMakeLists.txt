##
##
##


#find_package( LEMON REQUIRED )                     ## currently configured as external project


include_directories( ${LEMON_INCLUDE_DIR} )
include_directories( "../adiktedpp/include" )


add_subdirectory( src )

add_subdirectory( test )
