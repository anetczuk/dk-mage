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
        enum class MagicAvailableMode {
            AM_DISABLED,
            AM_RESEARCHABLE,          /// available after research
            AM_AVAILABLE
        };


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

        /// translate enum value to script command
        std::string AvailableCommandName( const AvailableCommand data );

        inline std::ostream& operator<<( std::ostream& os, const AvailableCommand data ) {
            os << AvailableCommandName( data );
            return os;
        }

    }
}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_AVAILABLEMODE_H_ */
