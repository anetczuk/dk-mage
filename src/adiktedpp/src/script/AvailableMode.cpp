/*
 * AvailableMode.cpp
 *
 */

#include "adiktedpp/script/AvailableMode.h"

#include "utils/Log.h"


namespace adiktedpp {
    namespace script {

        std::string AvailableCommandName( const AvailableCommand data ) {
            switch( data ) {
            case AvailableCommand::AC_ROOM_AVAILABLE:        { return "ROOM_AVAILABLE"; }
            case AvailableCommand::AC_CREATURE_AVAILABLE:    { return "CREATURE_AVAILABLE"; }
            case AvailableCommand::AC_DOOR_AVAILABLE:        { return "DOOR_AVAILABLE"; }
            case AvailableCommand::AC_TRAP_AVAILABLE:        { return "TRAP_AVAILABLE"; }
            case AvailableCommand::AC_MAGIC_AVAILABLE:       { return "MAGIC_AVAILABLE"; }
            }
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument -- unknown value: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }

    }
}
