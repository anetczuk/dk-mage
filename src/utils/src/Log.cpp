/*
 * Log.cpp
 *
 */

#include "utils/Log.h"

#include <fstream>


namespace utils {

    static std::ofstream& get_file_stream() {
        static std::ofstream fileStream;
        return fileStream;
    }

    Logger::~Logger() {
        buffer << "\n";
        std::cerr << buffer.str();
        std::ofstream& fileStream = get_file_stream();
        if ( fileStream.is_open() ) {
            fileStream << buffer.str();
        }
    }

    void Logger::setLogFile( const std::string& logFile ) {
        std::ofstream& fileStream = get_file_stream();
        fileStream.open( logFile );
    }

} /* namespace utils */
