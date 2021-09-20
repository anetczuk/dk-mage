/*
 * SlabType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_

/// inclusion: #include "adiktedpp/raw/SlabType.h"

#include <set>
#include <vector>
#include <ostream>


namespace adiktedpp {
    namespace raw {

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

        std::string SlabTypeName( const SlabType data );

        inline std::ostream& operator<<( std::ostream& os, const SlabType data ) {
            os << SlabTypeName( data );
            return os;
        }

        const std::set< SlabType >& SlabRooms();

        const std::set< SlabType >& SlabDoors();


        inline bool isImpassable( const SlabType type ) {
            switch( type ) {
            case SlabType::ST_ROCK:  return true;
            case SlabType::ST_GEMS:  return true;
            default: return false;
            }
        }

        inline bool isEarth( const SlabType type ) {
            switch( type ) {
            case SlabType::ST_EARTH:     return true;
            case SlabType::ST_TORCHDIRT: return true;
            default: return false;
            }
        }

        inline bool isPassage( const SlabType type ) {
            switch( type ) {
            case SlabType::ST_PATH:     return true;
            case SlabType::ST_CLAIMED:  return true;
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

        inline bool isLiquid( const SlabType type ) {
            switch( type ) {
            case SlabType::ST_WATER:    return true;
            case SlabType::ST_LAVA:     return true;
            default: return false;
            }
        }

        inline bool isSolid( const SlabType type ) {
            if ( isWall(type) ) {
                return true;
            }
            if ( isEarth(type) ) {
                return true;
            }
            if ( isImpassable(type) ) {
                return true;
            }
            return false;
        }

    }
}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_ */
