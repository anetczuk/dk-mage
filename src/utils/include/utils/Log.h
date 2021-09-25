/*
 * Log.h
 *
 */

#ifndef UTILS_INCLUDE_UTILS_LOG_H_
#define UTILS_INCLUDE_UTILS_LOG_H_

/// inclusion: #include "utils/Log.h"

#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>


#define FILE_NAME       utils::LogSink::extractFileName( __FILE__ )


namespace utils {

    template <typename T>
    std::string printIterable( const T& container ) {
        std::stringstream buffer;
        for ( const auto& item: container ) {
            buffer << item << " ";
        }
        return buffer.str();
    }


    /// ===========================================================


    /**
     *
     */
    enum class LogLevel {
        LL_INFO,
        LL_WARN,
        LL_ERR
    };


    /**
     *
     */
    class LogSink {
    public:

        LogSink() {
        }

        virtual ~LogSink() {
        }

        virtual void printMessage( const LogLevel /*level*/, const std::string& /*file*/, const std::size_t /*lineNo*/, const std::string& /*message*/ ) = 0;


        static std::string logPrefix( const LogLevel level, const std::string& file, const std::size_t lineNo );

        static std::string logPrefix( const std::string& file, const std::size_t lineNo );

        static std::string extractFileName( const std::string& filePath );

    };


    /**
     *
     */
    class WrapperLogSink: public LogSink {
        LogSink* sink;


    public:

        WrapperLogSink( LogSink* sink = nullptr ): sink(sink) {
        }

        void printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) override {
            if ( sink == nullptr ) {
                return;
            }
            sink->printMessage( level, file, lineNo, message );
        }

    };


    /**
     *
     */
    class CerrLogSink: public LogSink {
    public:

        void printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) override;

    };


    /**
     *
     */
    class CerrFileLogSink: public LogSink {

        std::ofstream fileStream;


    public:

        CerrFileLogSink( const std::string& filePath );

        void printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) override;

    };


    /**
     *
     */
    class PrintLogSink: public LogSink {
    public:

        typedef void (*PrintFunction)( const LogLevel /*level*/, const std::string& /*file*/, const std::size_t /*lineNo*/, const std::string& /*message*/ );


    private:

        PrintFunction* function;


    public:

        PrintLogSink( PrintFunction* function );

        void printMessage( const LogLevel level, const std::string& file, const std::size_t lineNo, const std::string& message ) override;

    };


    /// ==========================================================================


    /**
     *
     */
    class LogConfig {
    public:

        /// pass log sink with ownership (release object's memory)
        static void setSink( std::unique_ptr<LogSink>& newSink );

        /// pass log sink without granting ownership (do not release object's memory)
        static void setLogSink( LogSink& newSink );

        static void setPrintSink( PrintLogSink::PrintFunction* function );

        /// sets file log sink
        static void setCerrSink();

        /// sets cerr and file log sink
        static void setCerrFileSink( const std::string& logFile );

    };


    /// ==========================================================================


    /**
     *
     */
    class Logger: public std::stringstream {
        std::string file;
        int lane;
        LogLevel level;


    public:

        Logger( const LogLevel level, const char* file, const int lane );

        ~Logger();

        std::ostream& stream() {
            return *this;
        }

        void printMessage( const std::string& message ) {
            *this << message;
        }

    };


    inline void print_message( const LogLevel level, const std::string& file, const int lane, const std::string& message ) {
        Logger logger( level, file.c_str(), lane );
        logger.printMessage( message );
    }

}


#define LOG_INFO()   utils::Logger( utils::LogLevel::LL_INFO, __FILE__, __LINE__ )
#define LOG_WARN()   utils::Logger( utils::LogLevel::LL_WARN, __FILE__, __LINE__ )
#define LOG_ERR()    utils::Logger( utils::LogLevel::LL_ERR,  __FILE__, __LINE__ )


#endif /* UTILS_INCLUDE_UTILS_LOG_H_ */
