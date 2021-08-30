/*
 * Log.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_LOG_H_
#define UTILS_INCLUDE_UTILS_LOG_H_

/// inclusion: #include "utils/Log.h"

#include <sstream>
#include <iostream>



#define FILE_NAME       utils::Logger::extractFileName( __FILE__ )


namespace utils {

    class Logger {
        std::stringstream buffer;
        std::string file;
        int lane;


    public:

        Logger( const std::string& prefix, const char* file, const int lane );

        ~Logger();

        std::ostream& stream() {
            return buffer;
        }

        static void setLogFile( const std::string& logFile );

        static std::string extractFileName( const char* filePath );

    };


    template <typename T>
    std::string printIterable( const T& container ) {
        std::stringstream buffer;
        for ( const auto& item: container ) {
            buffer << item << " ";
        }
        return buffer.str();
    }

}


#define LOG()        utils::Logger( "INFO ", __FILE__, __LINE__ ).stream()
#define LOG_INFO()   utils::Logger( "INFO ", __FILE__, __LINE__ ).stream()
#define LOG_WARN()   utils::Logger( "WARN ", __FILE__, __LINE__ ).stream()
#define LOG_ERR()    utils::Logger( "ERR  ", __FILE__, __LINE__ ).stream()


#endif /* UTILS_INCLUDE_UTILS_LOG_H_ */
