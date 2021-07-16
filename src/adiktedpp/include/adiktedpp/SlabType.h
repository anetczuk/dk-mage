/*
 * SlabType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_

/// inclusion: #include "adiktedpp/SlabType.h"

#include <set>
#include <ostream>


namespace adiktedpp {

    /// data taken from header 'obj_slabs.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SlabType {
        ST_ROCK         = 0x00,             /// impassable
        ST_GOLD         = 0x01,             /// typically contains 1024 of gold (can be changed in rules.cfg of DK Fx)
        ST_EARTH        = 0x02,
        ST_TORCHDIRT    = 0x03,
        ST_WALLDRAPE    = 0x04,             /// fortified wall
        ST_WALLTORCH    = 0x05,             /// fortified wall
        ST_WALLWTWINS   = 0x06,             /// fortified wall
        ST_WALLWWOMAN   = 0x07,             /// fortified wall
        ST_WALLPAIRSHR  = 0x08,             /// fortified wall
        ST_PATH         = 0x0a,             /// unclaimed passage
        ST_CLAIMED      = 0x0b,             /// claimed passage
        ST_LAVA         = 0x0c,
        ST_WATER        = 0x0d,
        ST_PORTAL       = 0x0e,
        ST_TREASURE     = 0x10,             /// can store up to 2250 gold
        ST_LIBRARY      = 0x12,
        ST_PRISONCASE   = 0x14,
        ST_TORTURE      = 0x16,
        ST_TRAINING     = 0x18,
        ST_DUNGHEART    = 0x1a,
        ST_WORKSHOP     = 0x1c,
        ST_SCAVENGER    = 0x1e,
        ST_TEMPLE       = 0x20,
        ST_GRAVEYARD    = 0x22,
        ST_HATCHERY     = 0x24,
        ST_LAIR         = 0x26,
        ST_BARRACKS     = 0x28,
        ST_DOORWOOD1    = 0x2a,
        ST_DOORWOOD2    = 0x2b,
        ST_DOORBRACE1   = 0x2c,
        ST_DOORBRACE2   = 0x2d,
        ST_DOORIRON1    = 0x2e,
        ST_DOORIRON2    = 0x2f,
        ST_DOORMAGIC1   = 0x30,
        ST_DOORMAGIC2   = 0x31,
        ST_BRIDGE       = 0x33,
        ST_GEMS         = 0x34,
        ST_GUARDPOST    = 0x35
    };

    inline std::ostream& operator<<( std::ostream& os, const SlabType data ) {
        switch( data ) {
        case SlabType::ST_ROCK:       { os << "ST_ROCK"; return os; }
        case SlabType::ST_GOLD:       { os << "ST_GOLD"; return os; }
        case SlabType::ST_EARTH:      { os << "ST_EARTH"; return os; }
        case SlabType::ST_TORCHDIRT:  { os << "ST_TORCHDIRT"; return os; }
        case SlabType::ST_WALLDRAPE:  { os << "ST_WALLDRAPE"; return os; }
        case SlabType::ST_WALLTORCH:  { os << "ST_WALLTORCH"; return os; }
        case SlabType::ST_WALLWTWINS: { os << "ST_WALLWTWINS"; return os; }
        case SlabType::ST_WALLWWOMAN: { os << "ST_WALLWWOMAN"; return os; }
        case SlabType::ST_WALLPAIRSHR:{ os << "ST_WALLPAIRSHR"; return os; }
        case SlabType::ST_PATH:       { os << "ST_PATH"; return os; }
        case SlabType::ST_CLAIMED:    { os << "ST_CLAIMED"; return os; }
        case SlabType::ST_LAVA:       { os << "ST_LAVA"; return os; }
        case SlabType::ST_WATER:      { os << "ST_WATER"; return os; }
        case SlabType::ST_PORTAL:     { os << "ST_PORTAL"; return os; }
        case SlabType::ST_TREASURE:   { os << "ST_TREASURE"; return os; }
        case SlabType::ST_LIBRARY:    { os << "ST_LIBRARY"; return os; }
        case SlabType::ST_PRISONCASE: { os << "ST_PRISONCASE"; return os; }
        case SlabType::ST_TORTURE:    { os << "ST_TORTURE"; return os; }
        case SlabType::ST_TRAINING:   { os << "ST_TRAINING"; return os; }
        case SlabType::ST_DUNGHEART:  { os << "ST_DUNGHEART"; return os; }
        case SlabType::ST_WORKSHOP:   { os << "ST_WORKSHOP"; return os; }
        case SlabType::ST_SCAVENGER:  { os << "ST_SCAVENGER"; return os; }
        case SlabType::ST_TEMPLE:     { os << "ST_TEMPLE"; return os; }
        case SlabType::ST_GRAVEYARD:  { os << "ST_GRAVEYARD"; return os; }
        case SlabType::ST_HATCHERY:   { os << "ST_HATCHERY"; return os; }
        case SlabType::ST_LAIR:       { os << "ST_LAIR"; return os; }
        case SlabType::ST_BARRACKS:   { os << "ST_BARRACKS"; return os; }
        case SlabType::ST_DOORWOOD1:  { os << "ST_DOORWOOD1"; return os; }
        case SlabType::ST_DOORWOOD2:  { os << "ST_DOORWOOD2"; return os; }
        case SlabType::ST_DOORBRACE1: { os << "ST_DOORBRACE1"; return os; }
        case SlabType::ST_DOORBRACE2: { os << "ST_DOORBRACE2"; return os; }
        case SlabType::ST_DOORIRON1:  { os << "ST_DOORIRON1"; return os; }
        case SlabType::ST_DOORIRON2:  { os << "ST_DOORIRON2"; return os; }
        case SlabType::ST_DOORMAGIC1: { os << "ST_DOORMAGIC1"; return os; }
        case SlabType::ST_DOORMAGIC2: { os << "ST_DOORMAGIC2"; return os; }
        case SlabType::ST_BRIDGE:     { os << "ST_BRIDGE"; return os; }
        case SlabType::ST_GEMS:       { os << "ST_GEMS"; return os; }
        case SlabType::ST_GUARDPOST:  { os << "ST_GUARDPOST"; return os; }
        }
        os << "UNKNOWN[" << (int) data << "]";
        return os;
    }

    const std::set< SlabType >& SlabRooms();

    const std::set< SlabType >& SlabDoors();

    inline bool isEarth( const SlabType type ) {
        switch( type ) {
        case SlabType::ST_EARTH:     return true;
        case SlabType::ST_TORCHDIRT: return true;
        default: return false;
        }
    }

    inline bool isWall( const SlabType type ) {
        switch( type ) {
        case SlabType::ST_WALLDRAPE:     return true;
        case SlabType::ST_WALLTORCH:     return true;
        case SlabType::ST_WALLWTWINS:    return true;
        case SlabType::ST_WALLWWOMAN:    return true;
        case SlabType::ST_WALLPAIRSHR:   return true;
        default: return false;
        }
    }

}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_ */
