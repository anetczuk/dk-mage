##
##
##


include_directories( ${PROJECT_SOURCE_DIR}/utils/include )


## hide swig related warnings
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers" )


file(GLOB swg_files *.swg )
foreach( item IN ITEMS ${swg_files} )
    build_swig_module( ${item} utils )
endforeach()


configure_file( __init__.py ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
