/*
 * Log.cpp
 *
 */

#include "utils/Log.h"

#include <ghc/filesystem.hpp>


using ghc::filesystem::path;
namespace utils {

    static std::unique_ptr<LogSink>& get_log_sink() {
        static std::unique_ptr<LogSink> log_sink( new CerrLogSink() );
        return log_sink;
    }


    /// ============================================================================================================


    std::string LogSink::logPrefix( const LogLevel level, const std::string& file, const std::size_t lineNo ) {
        switch( level ) {
        case LogLevel::LL_INFO: {
            std::stringstream stream;
            stream << "INFO " << extractFileName( file ) << "(" << lineNo << ")";
            return stream.str();
        }
        case LogLevel::LL_WARN: {
            std::stringstream stream;
            stream << "WARN " << extractFileName( file ) << "(" << lineNo << ")";
            return stream.str();
        }
        case LogLevel::LL_ERR: {
            std::stringstream stream;
            stream << "ERR  " << extractFileName( file ) << "(" << lineNo << ")";
            return stream.str();
        }
        }
        std::stringstream stream;
        stream << "UNKN " << extractFileName( file ) << "(" << lineNo << ")";
        return stream.str();
    }

    std::string LogSink::logPrefix( const std::string& file, const std::size_t lineNo ) {
        std::stringstream stream;
        stream << extractFileName( file ) << "(" << lineNo << ")";
        return stream.str();
    }

    std::string LogSink::extractFileName( const std::string& filePath ) {
        path file( filePath );
        return file.filename().u8string();
    }

    void CerrLogSink::printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) {
        std::cerr << logPrefix( level, file, lineNo ) << ": " << message << "\n";
    }

    CerrFileLogSink::CerrFileLogSink( const std::string& filePath ): fileStream() {
        fileStream.open( filePath );
    }

    void CerrFileLogSink::printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) {
        const std::string prefix = logPrefix( level, file, lineNo );
        std::cerr  << prefix << ": " << message << "\n";
        fileStream << prefix << ": " << message << "\n";
    }

    PrintLogSink::PrintLogSink( PrintFunction* function ): function( function ) {
    }

    void PrintLogSink::printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) {
        if ( function == nullptr ) {
            return ;
        }
        (*function)( level, file, lineNo, message );
    }


    /// ============================================================================================================


    void LogConfig::setSink( std::unique_ptr<LogSink>& newSink ) {
        std::unique_ptr<LogSink>& sink = get_log_sink();
        sink.swap( newSink );
    }

    void LogConfig::setLogSink( LogSink& newSink ) {
        std::unique_ptr<LogSink> sink( new WrapperLogSink( &newSink ) );
        setSink( sink );
    }

    void LogConfig::setPrintSink( PrintLogSink::PrintFunction* function ) {
        std::unique_ptr<LogSink> sink( new PrintLogSink( function ) );
        setSink( sink );
    }

    void LogConfig::setCerrSink() {
        std::unique_ptr<LogSink> sink( new CerrLogSink() );
        setSink( sink );
    }

    void LogConfig::setCerrFileSink( const std::string& logFile ) {
        std::unique_ptr<LogSink> sink( new CerrFileLogSink( logFile ) );
        setSink( sink );
    }


    /// ============================================================================================================


    Logger::Logger( const LogLevel level, const char* file, const int lane ): file(file), lane(lane), level(level) {
    }

    Logger::~Logger() {
        std::unique_ptr<LogSink>& sink = get_log_sink();
        LogSink* sinkPtr = sink.get();
        if ( sinkPtr == nullptr ) {
            return ;
        }

        const std::string message = str();
        sinkPtr->printMessage( level, file, lane, message );
    }

} /* namespace utils */


