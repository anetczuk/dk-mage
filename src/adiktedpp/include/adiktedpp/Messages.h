/*
 * Messages.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_

#include <string>
#include <stdexcept>


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

        Messages( const Messages& )            = delete;
        Messages& operator=( const Messages& ) = delete;

        bool isInitialized() const {
            return initialized;
        }

        /// only log to file
        void log( const std::string& message );

        /// log to file and to buffer (available by 'readRecent()')
        void info( const std::string& message );

        const char* getRecent();

        void readRecentInfo();

        void readRecentErr();


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

        static bool& getActivatedFlag();

        void checkActivated();


    public:

        ScopeMessages() {
        }

        ScopeMessages( const std::string& fileName ) {
            setFile( fileName );
        }

        ScopeMessages( const std::string& fileName, const std::string& fileSuffix ) {
            setFile( fileName, fileSuffix );
        }

        ~ScopeMessages() {
            Messages::release();
            bool& activated = getActivatedFlag();
            activated = false;
        }

        void setFile( const std::string& fileName ) {
            checkActivated();
            Messages::get( fileName );
        }

        void setFile( const std::string& fileName, const std::string& fileSuffix ) {
            checkActivated();
            Messages::get( fileName, fileSuffix );
        }

        void readRecentInfo() {
            Messages& messages = Messages::get();
            messages.readRecentInfo();
        }

        void readRecentErr() {
            Messages& messages = Messages::get();
            messages.readRecentErr();
        }

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_MESSAGES_H_ */
