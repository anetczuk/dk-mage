##
##
##


include_directories( ${PROJECT_SOURCE_DIR}/dkmage/include )
include_directories( ${PROJECT_SOURCE_DIR}/adiktedpp/include )
include_directories( ${PROJECT_SOURCE_DIR}/utils/include )

include_directories( ${PROJECT_SOURCE_DIR}/pyadiktedpp )
include_directories( ${PROJECT_SOURCE_DIR}/pyutils )


## hide swig related warnings
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers" )


file(GLOB swg_files *.swg )
foreach( item IN ITEMS ${swg_files} )
    build_swig_module( ${item} dkmage )
endforeach()

#build_swig_module( raw/raw.swg dkmage )
#build_swig_module( script/script.swg dkmage )


configure_file( __init__.py ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
