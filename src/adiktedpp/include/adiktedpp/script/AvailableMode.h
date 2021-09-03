/*
 * AvailableMode.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLEMODE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLEMODE_H_

/// inclusion: #include "adiktedpp/script/AvailableMode.h"

#include <ostream>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        enum class AvailableMode {
            AM_DISABLED,
            AM_POSSIBLE,            /// available after meet certain criteria (e.g. research or build proper room)
            AM_ENABLED
        };


        enum class AvailableRoomMode {
            ARM_DISABLED,
            ARM_POSSIBLE_FOUND,      /// available to research when claimed/found
            ARM_ENABLED_FOUND,       /// enable when claimed/found
            ARM_POSSIBLE,            /// available after meet certain criteria (e.g. research or build proper room)
            ARM_ENABLED
        };


        /**
         *
         */
        enum class AvailableCommand {
            SAC_ROOM_AVAILABLE,
            SAC_CREATURE_AVAILABLE,
            SAC_DOOR_AVAILABLE,
            SAC_TRAP_AVAILABLE,
            SAC_MAGIC_AVAILABLE
        };

        inline std::ostream& operator<<( std::ostream& os, const AvailableCommand data ) {
            switch( data ) {
            case AvailableCommand::SAC_ROOM_AVAILABLE:        { os << "ROOM_AVAILABLE"; return os; }
            case AvailableCommand::SAC_CREATURE_AVAILABLE:    { os << "CREATURE_AVAILABLE"; return os; }
            case AvailableCommand::SAC_DOOR_AVAILABLE:        { os << "DOOR_AVAILABLE"; return os; }
            case AvailableCommand::SAC_TRAP_AVAILABLE:        { os << "TRAP_AVAILABLE"; return os; }
            case AvailableCommand::SAC_MAGIC_AVAILABLE:       { os << "MAGIC_AVAILABLE"; return os; }
            }
            os << "UNKNOWN_COMMAND[" << (int) data << "]";
            return os;
        }

    }
}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLEMODE_H_ */
