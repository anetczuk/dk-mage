/*
 * SubTypeDoor.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEDOOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEDOOR_H_

/// inclusion: #include "adiktedpp/SubTypeDoor.h"

#include <set>


namespace adiktedpp {

    /// data taken from header 'obj_things.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SubTypeDoor {
        STD_WOOD      = 0x01,
        STD_BRACED    = 0x02,
        STD_IRON      = 0x03,
        STD_MAGIC     = 0x04
    };

    const std::set< SubTypeDoor >& Doors();

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPETRAP_H_ */
