#
#
#


set(adikted_FOUND FALSE)
if ( NOT adikted_INCLUDE_DIR OR (NOT adikted_LIBRARY_RELEASE AND NOT adikted_LIBRARY_DEBUG) )

    ## Put here path to custom location
    find_path( adikted_INCLUDE_DIR libadikted/adikted.h
        HINTS $ENV{adikted_DIR} ${adikted_DIR}
    )
    
    ## library filename have to have "lib" prefix
    FIND_LIBRARY(adikted_LIBRARY_RELEASE
      NAMES adikted adikted${CMAKE_SHARED_LIBRARY_SUFFIX}
      HINTS "$ENV{adikted_DIR} ${adikted_DIR}"
    )
    
    ## library filename have to have "lib" prefix
    FIND_LIBRARY(adikted_LIBRARY_DEBUG
      NAMES adikted${CMAKE_DEBUG_POSTFIX} adikted{CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}
      HINTS "$ENV{adikted_DIR} ${adikted_DIR}"
    )
     
    if (adikted_INCLUDE_DIR)
        if (NOT adikted_FIND_QUIETLY)
            message(STATUS "Found adikted include-dir path: ${adikted_INCLUDE_DIR}")
        endif (NOT adikted_FIND_QUIETLY)
    endif()

endif()


if (adikted_INCLUDE_DIR)
    if (adikted_LIBRARY_RELEASE)
        set(adikted_FOUND TRUE)
        if (adikted_LIBRARY_DEBUG)
            set(adikted_LIBRARY optimized ${adikted_LIBRARY_RELEASE} debug ${adikted_LIBRARY_DEBUG})
        else()
            set(adikted_LIBRARY ${adikted_LIBRARY_RELEASE})
            # set(adikted_LIBRARY optimized ${adikted_LIBRARY_RELEASE})
        endif()
    else()
        if (adikted_LIBRARY_DEBUG)
            set(adikted_FOUND TRUE)
            set(adikted_LIBRARY ${adikted_LIBRARY_DEBUG})
            # set(adikted_LIBRARY debug ${adikted_LIBRARY_DEBUG})
        endif()
    endif()
endif()



if (NOT adikted_FOUND)
    if (adikted_FIND_REQUIRED)
        message( STATUS "adikted variables: ${adikted_INCLUDE_DIR} ${adikted_LIBRARY_RELEASE} ${adikted_LIBRARY_DEBUG} ${adikted_LIBRARY}" )
        message( FATAL_ERROR "could not find adikted" )
    else()
        message( STATUS "adikted NOT REQUIRED" )
    endif (adikted_FIND_REQUIRED)
endif()
