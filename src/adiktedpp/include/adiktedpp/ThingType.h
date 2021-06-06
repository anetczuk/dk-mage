/*
 * ThingType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_

/// inclusion: #include "adiktedpp/ThingType.h"


namespace adiktedpp {

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

    inline std::ostream& operator<<( std::ostream& os, const ThingType data ) {
        switch( data ) {
        case ThingType::TT_NONE:      { os << "TT_NONE"; return os; }
        case ThingType::TT_ITEM:      { os << "TT_ITEM"; return os; }
        case ThingType::TT_SHOT:      { os << "TT_SHOT"; return os; }
        case ThingType::TT_EFFCTELEM: { os << "TT_EFFCTELEM"; return os; }
        case ThingType::TT_DEADCRTR:  { os << "TT_DEADCRTR"; return os; }
        case ThingType::TT_CREATURE:  { os << "TT_CREATURE"; return os; }
        case ThingType::TT_EFFECTGEN: { os << "TT_EFFECTGEN"; return os; }
        case ThingType::TT_TRAP:      { os << "TT_TRAP"; return os; }
        case ThingType::TT_DOOR:      { os << "TT_DOOR"; return os; }
        case ThingType::TT_AMBNITSND: { os << "TT_AMBNITSND"; return os; }
        case ThingType::TT_CAVEIN:    { os << "TT_CAVEIN"; return os; }
        }
        os << "UNKNOWN[" << (int) data << "]";
        return os;
    }

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_THINGTYPE_H_ */
