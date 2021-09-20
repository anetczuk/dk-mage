##
## various functions and macros
##


macro( init_swig )
    find_package(Python3 COMPONENTS Interpreter Development )       ## Development required for Python3_INCLUDE_DIRS
    
    # message("Python3_VERSION: ${Python3_VERSION}")
    # message("Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
    # message("Python3_LIBRARIES: ${Python3_LIBRARIES}")
    # message("Python3_EXECUTABLE: ${Python3_EXECUTABLE}")
    
    find_package( SWIG REQUIRED )
    #include( ${SWIG_USE_FILE} )
    include( UseSWIG )
    
    #message("SWIG_VERSION: ${SWIG_VERSION}")
    
    set( SWIG_SOURCE_FILE_EXTENSIONS ".i" ".swg" )
    
    if( Python3_VERSION MATCHES "^2.*$" )
        list( APPEND CMAKE_SWIG_FLAGS -classic )
        ##set(CMAKE_SWIG_FLAGS -classic)
    else()
        list( APPEND CMAKE_SWIG_FLAGS -py3 )
        ##set(CMAKE_SWIG_FLAGS -py3)
    endif()
    
    # message("CMAKE_SWIG_FLAGS: ${CMAKE_SWIG_FLAGS}")
    
    include_directories( ${Python3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
endmacro()


function( build_swig_module swig_file link_libs )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )

    get_filename_component( file_name ${swig_file} NAME_WE )
    set( lib_name "py${file_name}" )
    ##message( "xxxx: ${file_name}" )
    set_source_files_properties( ${swig_file} PROPERTIES CPLUSPLUS ON )
    swig_add_library( ${lib_name} TYPE SHARED LANGUAGE python SOURCES ${swig_file} )
    swig_link_libraries( ${lib_name} ${Python3_LIBRARIES} ${link_libs} )
endfunction()
