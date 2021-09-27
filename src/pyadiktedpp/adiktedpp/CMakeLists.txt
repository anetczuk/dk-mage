##
##
##


include_directories( ${PROJECT_SOURCE_DIR}/adiktedpp/include )
include_directories( ${PROJECT_SOURCE_DIR}/utils/include )
include_directories( ${PROJECT_SOURCE_DIR}/pyutils )


file(GLOB swg_files *.swg )
foreach( item IN ITEMS ${swg_files} )
    build_swig_module( ${item} adiktedpp )
endforeach()

build_swig_module( raw/raw.swg adiktedpp )
build_swig_module( script/script.swg adiktedpp )


configure_file( __init__.py ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
