/*
 * SubTypeCreature.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPECREATURE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPECREATURE_H_

/// inclusion: #include "adiktedpp/SubTypeCreature.h"

#include <set>


namespace adiktedpp {

    /// data taken from header 'obj_things.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SubTypeCreature {
        /*Creature types (all listed) */
        STC_WIZRD     = 0x01,
        STC_BARBARIN  = 0x02,
        STC_ARCHER    = 0x03,
        STC_MONK      = 0x04,
        STC_DWAFT     = 0x05,
        STC_KNIGHT    = 0x06,
        STC_AVATAR    = 0x07,
        STC_TUNELER   = 0x08,
        STC_WITCH     = 0x09,
        STC_GIANT     = 0x0A,
        STC_FAIRY     = 0x0B,
        STC_THEFT     = 0x0C,
        STC_SMURI     = 0x0D,

        STC_HORNY     = 0x0E,
        STC_SKELETON  = 0x0F,
        STC_TROLL     = 0x10,
        STC_DRAGON    = 0x11,
        STC_SPAWN     = 0x12,
        STC_FLY       = 0x13,
        STC_MISTRESS  = 0x14,
        STC_WARLOCK   = 0x15,
        STC_BILEDEMN  = 0x16,
        STC_IMP       = 0x17,
        STC_BEETLE    = 0x18,
        STC_VAMPIRE   = 0x19,
        STC_SPIDER    = 0x1A,
        STC_HOUND     = 0x1B,
        STC_GHOST     = 0x1C,
        STC_TENTCL    = 0x1D,
        STC_ORC       = 0x1E,
        STC_FLOAT     = 0x1F
    };


    /// all evil creatures
    const std::set< SubTypeCreature >& EvilCreatures();

    const std::set< SubTypeCreature >& HeroCreatures();

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPECREATURE_H_ */
