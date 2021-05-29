/*
 * Messages.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_

#include <string>


namespace adiktedpp {

    /**
     * Singleton
     */
    class Messages {
    private:

        bool initialized;


        Messages();

        ~Messages();

        void initializeData( const std::string& fileName = "messages.log" );

        void releaseData();

        static Messages& getRaw();


    public:

        Messages( const Messages& )       = delete;
        void operator=( const Messages& ) = delete;

        bool isInitialized() const {
            return initialized;
        }

        /// only log to file
        void log( const std::string& message );

        /// log to file and to buffer (available by 'readRecent()')
        void info( const std::string& message );

        void readRecent();


        /// get instance
        static Messages& get();

        static Messages& get( const std::string& fileName );

        static Messages& get( const std::string& fileName, const std::string& fileSuffix );

        static void release();

    };


    /**
     * Warning: does not work for nested scopes yet.
     */
    class ScopeMessages {
    public:

        ScopeMessages() {
            Messages::get();            /// initialize if needed
        }

        ScopeMessages( const std::string& fileName ) {
            Messages::get( fileName );
        }

        ScopeMessages( const std::string& fileName, const std::string& fileSuffix ) {
            Messages::get( fileName, fileSuffix );
        }

        ~ScopeMessages() {
            Messages::release();
        }

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_ */
