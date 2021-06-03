/*
 * Log.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_

/// inclusion: #include "utils/Log.h"

#include <sstream>
#include <iostream>


class Logger {
    std::stringstream buffer;
    std::string file;
    int lane;


public:

    Logger( const char* file, const int lane ): file(file), lane(lane) {
        buffer << file << "(" << lane << "): ";
    }
    ~Logger() {
        buffer << "\n";
        std::cerr << buffer.str();
    }

    std::ostream& stream() {
        return buffer;
    }
};


#define LOG()   Logger( __FILE__, __LINE__ ).stream()


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_ */
