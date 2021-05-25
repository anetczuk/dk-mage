##
## option for clang sanitizers
##

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    ## other sanitizers (memory, leak) does not work properly on clang 3.5
    option( ADDRESS_SANITIZER "Address sanitizer (for clang)" OFF )
    if( ADDRESS_SANITIZER )
        set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ## other sanitizers (memory, leak) does not work properly on clang 3.5
    option( ADDRESS_SANITIZER "Address sanitizer (for clang)" OFF )
    if( ADDRESS_SANITIZER )
        set( CMAKE_CXX_COMPILER_ID "${CMAKE_CXX_COMPILER_ID} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()
