##
##
##


find_package( adikted REQUIRED )


include_directories( ${adikted_INCLUDE_DIR} )


add_subdirectory( src )

add_subdirectory( test )
