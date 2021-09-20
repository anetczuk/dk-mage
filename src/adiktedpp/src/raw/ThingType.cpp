/*
 * ThingType.cpp
 *
 */

#include "adiktedpp/raw/ThingType.h"

#include "utils/Log.h"


namespace adiktedpp {
    namespace raw {

        std::string ThingTypeName( const ThingType data ) {
            switch( data ) {
            case ThingType::TT_NONE:      { return "TT_NONE"; }
            case ThingType::TT_ITEM:      { return "TT_ITEM"; }
            case ThingType::TT_SHOT:      { return "TT_SHOT"; }
            case ThingType::TT_EFFCTELEM: { return "TT_EFFCTELEM"; }
            case ThingType::TT_DEADCRTR:  { return "TT_DEADCRTR"; }
            case ThingType::TT_CREATURE:  { return "TT_CREATURE"; }
            case ThingType::TT_EFFECTGEN: { return "TT_EFFECTGEN"; }
            case ThingType::TT_TRAP:      { return "TT_TRAP"; }
            case ThingType::TT_DOOR:      { return "TT_DOOR"; }
            case ThingType::TT_AMBNITSND: { return "TT_AMBNITSND"; }
            case ThingType::TT_CAVEIN:    { return "TT_CAVEIN"; }
            }
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument -- unknown value: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }

    }
}
