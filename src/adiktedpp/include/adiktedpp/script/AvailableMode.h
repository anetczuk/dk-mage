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


        /**
         *
         */
        enum class RoomAvailableMode {
            RAM_DISABLED,
            RAM_RESEARCHABLE_FOUND,         /// researchable when claimed/found
            RAM_RESEARCHABLE,               /// available after research
            RAM_AVAILABLE_FOUND,            /// enable when claimed/found
            RAM_AVAILABLE
        };


        /**
         *
         */
        enum class ResearchableMode {
            AM_DISABLED,
            AM_RESEARCHABLE,          /// available after research
            AM_AVAILABLE
        };


        using MagicAvailableMode = ResearchableMode;


        /// ==========================================================================


        /**
         *
         */
        enum class AvailableCommand {
            AC_ROOM_AVAILABLE,
            AC_CREATURE_AVAILABLE,
            AC_DOOR_AVAILABLE,
            AC_TRAP_AVAILABLE,
            AC_MAGIC_AVAILABLE
        };

        inline std::ostream& operator<<( std::ostream& os, const AvailableCommand data ) {
            switch( data ) {
            case AvailableCommand::AC_ROOM_AVAILABLE:        { os << "ROOM_AVAILABLE"; return os; }
            case AvailableCommand::AC_CREATURE_AVAILABLE:    { os << "CREATURE_AVAILABLE"; return os; }
            case AvailableCommand::AC_DOOR_AVAILABLE:        { os << "DOOR_AVAILABLE"; return os; }
            case AvailableCommand::AC_TRAP_AVAILABLE:        { os << "TRAP_AVAILABLE"; return os; }
            case AvailableCommand::AC_MAGIC_AVAILABLE:       { os << "MAGIC_AVAILABLE"; return os; }
            }
            os << "UNKNOWN_COMMAND[" << (int) data << "]";
            return os;
        }

    }
}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLEMODE_H_ */
