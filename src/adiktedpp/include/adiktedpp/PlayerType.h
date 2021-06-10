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
        PT_GOOD             = 0x04,     /// hero (white)
        PT_UNSET            = 0x05,     /// neutral (multicolor)
        PT_ALL              = 0x06      /// used in scripts only
    };

    inline std::ostream& operator<<( std::ostream& os, const PlayerType data ) {
        switch( data ) {
        case PlayerType::PT_0:       { os << "PLAYER0"; return os; }
        case PlayerType::PT_1:       { os << "PLAYER1"; return os; }
        case PlayerType::PT_2:       { os << "PLAYER2"; return os; }
        case PlayerType::PT_3:       { os << "PLAYER3"; return os; }
        case PlayerType::PT_GOOD:    { os << "PLAYER_GOOD"; return os; }
        case PlayerType::PT_UNSET:   { os << "PLAYER_UNSET"; return os; }
        case PlayerType::PT_ALL:     { os << "PLAYER_ALL"; return os; }
        }
        os << "UNKNOWN[" << (int) data << "]";
        return os;
    }

}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_PLAYERTYPE_H_ */
