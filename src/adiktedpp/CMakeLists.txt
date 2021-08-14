##
##
##


generate_to_file( Version.h.in "${CMAKE_CURRENT_SOURCE_DIR}/include/adiktedpp/Version.h" )


add_subdirectory( src )

add_subdirectory( test )
