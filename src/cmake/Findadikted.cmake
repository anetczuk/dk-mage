#
#
#


set(adikted_FOUND FALSE)
if (NOT adikted_INCLUDE_DIR)

    ## Put here path to custom location
    
    find_path(adikted_INCLUDE_DIR libadikted/adikted.h
        HINTS "$ENV{adikted_DIR} ${adikted_DIR}"
        PATHS ${CMAKE_MODULE_PATH}
        PATH_SUFFIXES include ../../include
    )
    
    ## library filename have to have "lib" prefix
    FIND_LIBRARY(adikted_LIBRARY_RELEASE
      NAMES adikted
      HINTS "$ENV{adikted_DIR} ${adikted_DIR}"
      PATHS ${CMAKE_MODULE_PATH}
      PATH_SUFFIXES lib ../../lib
    )
    
    
    ## library filename have to have "lib" prefix
    FIND_LIBRARY(adikted_LIBRARY_DEBUG
      NAMES adikted_d
      HINTS "$ENV{adikted_DIR} ${adikted_DIR}"
      PATHS ${CMAKE_MODULE_PATH}
      PATH_SUFFIXES lib ../../lib
    )
        
        
    if (adikted_INCLUDE_DIR)
        if (NOT adikted_FIND_QUIETLY)
            message(STATUS "Found adikted include-dir path: ${adikted_INCLUDE_DIR}")
        endif (NOT adikted_FIND_QUIETLY)
    endif()

endif()


if (adikted_INCLUDE_DIR)
    set(adikted_FOUND TRUE)
    if (adikted_LIBRARY_RELEASE)
        set(adikted_FOUND TRUE)
        if (adikted_LIBRARY_DEBUG)
            set(adikted_LIBRARY optimized ${adikted_LIBRARY_RELEASE} debug ${adikted_LIBRARY_DEBUG})
        else()
            set(adikted_LIBRARY optimized ${adikted_LIBRARY_RELEASE})
        endif()
    else()
        if (adikted_LIBRARY_DEBUG)
            set(adikted_FOUND TRUE)
            set(adikted_LIBRARY debug ${adikted_LIBRARY_DEBUG})
        endif()
    endif()
endif()


if (NOT adikted_FOUND)
    if (adikted_FIND_REQUIRED)
        message(FATAL_ERROR "could not find adikted")
    else()
        message( STATUS "adikted NOT REQUIRED" )
    endif (adikted_FIND_REQUIRED)
endif()
