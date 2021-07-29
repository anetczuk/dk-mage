#
#
#


set( TARGET_NAME dkmagecli )


### build tclap project
set( tclap_UNPACK_DIR ${FETCHCONTENT_BASE_DIR}/tclap-src )
set( tclap_patch_command ${CMAKE_COMMAND} -DDESTINATION_DIR=${tclap_UNPACK_DIR}/ -P ${EXTERNAL_DIR}/tclap/patch.cmake )
set( BUILD_TESTING OFF CACHE BOOL "" FORCE )
FetchContent_Declare( tclap
                      URL ${EXTERNAL_DIR}/tclap/tclap-1.4.0-rc1.tar.bz2
                      SOURCE_DIR ${tclap_UNPACK_DIR}
                      PATCH_COMMAND ${tclap_patch_command}
)
FetchContent_MakeAvailable( tclap )
set( tclap_INCLUDE_DIR ${tclap_SOURCE_DIR}/include )


### build inih project (headers only)
FetchContent_Declare( inih
                      URL ${EXTERNAL_DIR}/inih/inih-master.zip
                      SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/inih-src/inih
)
FetchContent_Populate( inih )
set( inih_INCLUDE_DIR "${inih_SOURCE_DIR}/.." )


file(GLOB_RECURSE cpp_files *.cpp )


add_executable( ${TARGET_NAME} ${cpp_files} )
target_include_directories( ${TARGET_NAME} PRIVATE ../include ${inih_INCLUDE_DIR} ${tclap_INCLUDE_DIR} )
target_link_libraries( ${TARGET_NAME} utils dkmage )


install( TARGETS ${TARGET_NAME}
         RUNTIME DESTINATION "."
         LIBRARY DESTINATION "."
         ARCHIVE DESTINATION "."
#         PUBLIC_HEADER DESTINATION include/${TARGET_NAME}
)
