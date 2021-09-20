##
##
##


file(GLOB_RECURSE py_files *.py )
foreach( item IN ITEMS ${py_files} )
    configure_file( ${item} ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
endforeach()


add_test( NAME pyutils_tests COMMAND ${Python3_EXECUTABLE} -m unittest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/.. )
