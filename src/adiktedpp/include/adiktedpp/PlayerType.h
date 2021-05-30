/*
 * PlayerType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_PLAYERTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_PLAYERTYPE_H_

/// inclusion: #include "adiktedpp/PlayerType.h"


namespace adiktedpp {

    /// data taken from header 'obj_slabs.h'
    //TODO: make auto converter of '#define' list to enum
    enum class PlayerType {
        PT_0                = 0x00,
        PT_1                = 0x01,
        PT_2                = 0x02,
        PT_3                = 0x03,
        PT_GOOD             = 0x04,
        PT_UNSET            = 0x05,
        PT_ALL              = 0x06  /*used in scripts only */
    };

}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_PLAYERTYPE_H_ */
