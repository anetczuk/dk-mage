/*
 * Log.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_

/// inclusion: #include "utils/Log.h"

#include <iostream>


class Logger {
    std::string file;
    int lane;


public:

    Logger( const char* file, const int lane ): file(file), lane(lane) {
        stream() << file << "(" << lane << "): ";
    }
    ~Logger() {
        stream() << "\n";
    }

    std::ostream& stream() {
        return std::cerr;
    }
};


#define LOG()   Logger( __FILE__, __LINE__ ).stream()


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LOG_H_ */
