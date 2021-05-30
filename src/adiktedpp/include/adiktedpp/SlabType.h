/*
 * SlabType.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_

/// inclusion: #include "adiktedpp/SlabType.h"


namespace adiktedpp {

    /// data taken from header 'obj_slabs.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SlabType {
        ST_ROCK         = 0x00,
        ST_GOLD         = 0x01,
        ST_EARTH        = 0x02,
        ST_TORCHDIRT    = 0x03,
        ST_WALLDRAPE    = 0x04,
        ST_WALLTORCH    = 0x05,
        ST_WALLWTWINS   = 0x06,
        ST_WALLWWOMAN   = 0x07,
        ST_WALLPAIRSHR  = 0x08,
        ST_PATH         = 0x0a,
        ST_CLAIMED      = 0x0b,
        ST_LAVA         = 0x0c,
        ST_WATER        = 0x0d,
        ST_PORTAL       = 0x0e,
        ST_TREASURE     = 0x10,
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

}

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SLABTYPE_H_ */
