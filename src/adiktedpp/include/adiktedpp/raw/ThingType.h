/*
 * ThingType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_

/// inclusion: #include "adiktedpp/raw/ThingType.h"

#include <ostream>


namespace adiktedpp {
    namespace raw {

        /// data taken from header 'obj_things.h'
        //TODO: make auto converter of '#define' list to enum
        enum class ThingType {
            /*Things - most of the game objects is stored as thing */
            TT_NONE        = 0x00,
            TT_ITEM        = 0x01, /* called "object" in the game */
            TT_SHOT        = 0x02,
            TT_EFFCTELEM   = 0x03, /* "effect element" (?) */
            TT_DEADCRTR    = 0x04, /* dead creature */
            TT_CREATURE    = 0x05,
            TT_EFFECTGEN   = 0x07, /* called "effect generator" in the game */
            TT_TRAP        = 0x08,
            TT_DOOR        = 0x09,
            TT_AMBNITSND   = 0x0c, /* ambient sound */
            TT_CAVEIN      = 0x0d
        };

        std::string ThingTypeName( const ThingType data );

        inline std::ostream& operator<<( std::ostream& os, const ThingType data ) {
            os << ThingTypeName( data );
            return os;
        }

    }
}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_ */
