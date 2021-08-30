/*
 * Log.cpp
 *
 */

#include "utils/Log.h"

#include <ghc/filesystem.hpp>

#include <fstream>


using ghc::filesystem::path;


namespace utils {

    static std::ofstream& get_file_stream() {
        static std::ofstream fileStream;
        return fileStream;
    }

    Logger::Logger( const std::string& prefix, const char* file, const int lane ): file(file), lane(lane) {
        buffer << prefix << extractFileName( file ) << "(" << lane << "): ";
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

    std::string Logger::extractFileName( const char* filePath ) {
        path file( filePath );
        return file.filename().u8string();
    }

} /* namespace utils */
