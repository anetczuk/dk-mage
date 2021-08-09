/*
 * Messages.cpp
 *
 */

#include "adiktedpp/Messages.h"

#include "utils/Log.h"


extern "C" {
    #include "libadikted/msg_log.h"
}


namespace adiktedpp {

    Messages& Messages::getRaw() {
        static Messages messages;
        return messages;
    }

    Messages& Messages::get() {
        Messages& messages = getRaw();
        if ( messages.isInitialized() == false ) {
            messages.initializeData();
        }
        return messages;
    }

    Messages& Messages::get( const std::string& fileName ) {
        Messages& messages = getRaw();
        messages.initializeData( fileName );
        return messages;
    }

    Messages& Messages::get( const std::string& fileName, const std::string& fileSuffix ) {
        const std::string fName = fileName + fileSuffix;
        return get( fName );
    }

    void Messages::release() {
        Messages& messages = getRaw();
        messages.releaseData();
    }

    /// ==================================================================

    Messages::Messages(): initialized(false) {
    }

    Messages::~Messages() {
        releaseData();
    }

    void Messages::initializeData( const std::string& fileName ) {
        if ( initialized ) {
            free_messages();
        }
        initialized = true;
        init_messages();
        const char* data = fileName.c_str();
        set_msglog_fname( (char*)data );
    }

    void Messages::releaseData() {
        if ( initialized == false ) {
            return ;
        }
        initialized = false;
        free_messages();
        init_messages();                /// reset state to NULL
    }

    void Messages::log( const std::string& message ) {
        const char* data = message.c_str();
        message_log( data );
    }

    void Messages::info( const std::string& message ) {
        const char* data = message.c_str();
        message_info( data );
    }

    const char* Messages::getRecent() {
        return message_get();
    }

    void Messages::readRecent() {
        LOG() << message_get() << "\n";
    }


    /// ==============================================


    bool& ScopeMessages::getActivatedFlag() {
        static bool activated = false;
        return activated;
    }

    void ScopeMessages::checkActivated() {
        bool& activated = getActivatedFlag();
        if ( activated ) {
            /// guard
            std::stringstream stream;
            stream << FILE_NAME << ": scope message already created";
            throw std::runtime_error( stream.str() );
        }
        activated = true;
    }

} /* namespace adiktedpp */
