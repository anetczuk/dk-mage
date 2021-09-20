##
##
##


include_directories( ../../utils/include )


build_swig_module( Rect.swg utils )
build_swig_module( Rand.swg utils )


configure_file( __init__.py ${CMAKE_CURRENT_BINARY_DIR} @ONLY )
