/*
 * Separator.h
 *
 */

#ifndef UTILS_INCLUDE_SEPARATOR_H_
#define UTILS_INCLUDE_SEPARATOR_H_

/// inclusion: #include "utils/Separator.h"


#if defined(unix) && !defined (GO32)
    #define SEPARATOR "/"
#else
    #define SEPARATOR "\\"
#endif


inline std::string separatorsToWindows( const std::string& path ) {
//    std::string ret = path;
    return path;
}

/// replace '\\' to '/'
inline std::string separatorsToLinux( const std::string& path ) {
    std::string ret = path;

    size_t index = 0;
    while (true) {
         /* Locate the substring to replace. */
         index = ret.find("\\", index);
         if (index == std::string::npos) break;

         /* Make the replacement. */
         ret.replace(index, 1, "/");

         index += 1;
    }

    return ret;
}


#endif /* UTILS_INCLUDE_SEPARATOR_H_ */
