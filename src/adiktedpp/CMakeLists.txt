##
##
##


find_package( adikted REQUIRED )


include_directories( ${adikted_INCLUDE_DIR} )

copy_package_library( "adikted" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )


add_subdirectory( src )

add_subdirectory( test )
