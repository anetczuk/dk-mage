##
##
##


add_subdirectory( adiktedpp )

add_subdirectory( testadiktedpp )


## copy examples

file(GLOB_RECURSE py_files example/*.py )
foreach( item IN ITEMS ${py_files} )
    configure_file( ${item} ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
endforeach()
