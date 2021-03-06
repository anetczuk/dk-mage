##
## various functions and macros
##

function(generate_to_file sourceFile targetFile)
    set( GENERATOR_FILE "${sourceFile}" )
    configure_file( ${sourceFile} ${targetFile} @ONLY )
endfunction()


function(generate_file script_template)
    set( GENERATOR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${script_template}" )
    set( DATA_TYPE ${data_type} )
    string(REGEX REPLACE "\\.[^.]*$" "" output_file ${script_template})
    configure_file( ${GENERATOR_FILE} "${output_file}" @ONLY )
endfunction()


macro(current_dir_name output_var)
    get_filename_component( ${output_var} ${CMAKE_CURRENT_SOURCE_DIR} NAME)
endmacro()


##
## concept of function "add_subdirectories_recursive" is bad by design (causes CMake errors)
##
function( add_subdirectories )
    ## returns files in alphabetic order
    set( subdirs_list )
    file(GLOB cmake_files */CMakeLists.txt )
    foreach(item ${cmake_files})
        get_filename_component( dir_path ${item} DIRECTORY)
        
        set( is_found -1 )
        foreach(subdir ${subdirs_list})
            string(FIND "${dir_path}" "${subdir}" "is_found" )
            if ( ${is_found} GREATER "-1" )
                ## found
                break()
            endif()
        endforeach()
        
        if ( ${is_found} LESS "0" )
            ## new path
            list(APPEND subdirs_list ${dir_path} )
        endif()
    endforeach()
    
    ## message("found dirs: ${subdirs_list}")
    foreach(item ${subdirs_list})
        add_subdirectory( ${item} )
    endforeach()
endfunction()


function(build_cpps project_name)
    file(GLOB cpp_files *.cpp)
    
    foreach(single_cpp ${cpp_files})
        get_filename_component( cpp_name ${single_cpp} NAME_WE)
        if("${cpp_name}" MATCHES "^test*")
            continue()
        endif()
        set( TARGET_NAME ${project_name}-${cpp_name} )
        add_executable( ${TARGET_NAME} ${single_cpp} )
        target_link_libraries( ${TARGET_NAME} ${EXT_LIBS} )
        set_target_properties( ${TARGET_NAME} PROPERTIES OUTPUT_NAME ${cpp_name})
    endforeach()
endfunction()


function(get_library_path output_var package_name)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set( varname "${package_name}_LIBRARY_DEBUG" )
    else()
        set( varname "${package_name}_LIBRARY_RELEASE" )
    endif()
    set( ${output_var} "${${varname}}" PARENT_SCOPE )
endfunction()


function(copy_package_library package_name output_dir)
    get_library_path( package_lib "${package_name}" )
    file(COPY ${package_lib} DESTINATION ${output_dir} )
endfunction()

function(copy_package_libraries package_name output_dir)
    set( varname "${package_name}_LIBRARY_DEBUG" )
    if ( ${varname} )
        file(COPY ${${varname}} DESTINATION ${output_dir} )
    endif()
    set( varname "${package_name}_LIBRARY_RELEASE" )
    if ( ${varname} )
        file(COPY ${${varname}} DESTINATION ${output_dir} )
    endif()
endfunction()


add_custom_target( purge COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/purge.cmake )
