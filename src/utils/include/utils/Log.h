/*
 * Log.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_LOG_H_
#define UTILS_INCLUDE_UTILS_LOG_H_

/// inclusion: #include "utils/Log.h"

#include <sstream>
#include <iostream>


namespace utils {

    class Logger {
        std::stringstream buffer;
        std::string file;
        int lane;


    public:

        Logger( const char* file, const int lane );

        ~Logger();

        std::ostream& stream() {
            return buffer;
        }

        static void setLogFile( const std::string& logFile );

    };

}


#define LOG()   utils::Logger( __FILE__, __LINE__ ).stream()


#endif /* UTILS_INCLUDE_UTILS_LOG_H_ */
