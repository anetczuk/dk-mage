/*
 * SubTypeTrap.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPETRAP_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPETRAP_H_

/// inclusion: #include "adiktedpp/SubTypeTrap.h"

#include <set>


namespace adiktedpp {

    /// data taken from header 'obj_things.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SubTypeTrap {
        STT_BOULDER   = 0x01,
        STT_ALARM     = 0x02,
        STT_GAS       = 0x03,
        STT_LIGHTNG   = 0x04,
        STT_WORDPWR   = 0x05,
        STT_LAVA      = 0x06,
        STT_DUMMY2    = 0x07,
        STT_DUMMY3    = 0x08,
        STT_DUMMY4    = 0x09,
        STT_DUMMY5    = 0x0A,
        STT_DUMMY6    = 0x0B,
        STT_DUMMY7    = 0x0C
    };

    const std::set< SubTypeTrap >& DamageTraps();

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPETRAP_H_ */
