/*
 * SubTypeItem.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_

/// inclusion: #include "adiktedpp/SubTypeItem.h"

#include <set>


namespace adiktedpp {

    /// data taken from header 'obj_things.h'
    //TODO: make auto converter of '#define' list to enum
    enum class SubTypeItem {
        /* sub-types for items */
        STI_NULL      = 0x00,
        STI_BARREL    = 0x01,
        STI_TORCH     = 0x02,
        STI_GOLDCHEST = 0x03,
        STI_TEMPLESTA = 0x04,
        STI_DNHEART   = 0x05,
        STI_GOLD      = 0x06,
        STI_TORCHUN   = 0x07,
        STI_STATUEWO  = 0x08,
        STI_CHICKNGRW = 0x09,
        STI_CHICKN    = 0x0a,
        STI_SPELLHOE  = 0x0b,
        STI_SPELLIMP  = 0x0c,
        STI_SPELLMUST = 0x0d,
        STI_SPELLSLAP = 0x0e,
        STI_SPELLSOE  = 0x0f,
        STI_SPELLCTA  = 0x10,
        STI_SPELLCAVI = 0x11,
        STI_SPELLHEAL = 0x12,
        STI_SPELLHLDA = 0x13,
        STI_SPELLLIGH = 0x14,
        STI_SPELLSPDC = 0x15,
        STI_SPELLPROT = 0x16,
        STI_SPELLCONC = 0x17,

        STI_NULL1     = 0x18,
        STI_NULL2     = 0x19,

        STI_ANVIL     = 0x1a,
        STI_PRISONBAR = 0x1b,
        STI_CANDLSTCK = 0x1c, /*candle with candlestick */
        STI_GRAVSTONE = 0x1d,
        STI_STATUHORN = 0x1e,
        STI_TRAINPOST = 0x1f,
        STI_TORTSPIKE = 0x20,
        STI_TEMPLESPN = 0x21, /*Temple Spangle */

        STI_POTION1   = 0x22,
        STI_POTION2   = 0x23,
        STI_POTION3   = 0x24,
        STI_PWHAND    = 0x25,
        STI_PWHANDGRB = 0x26,
        STI_PWHANDWHP = 0x27,
        STI_CHICKNSTB = 0x28,
        STI_CHICKNWOB = 0x29,
        STI_CHICKNCRK = 0x2a,
        STI_GOLDL     = 0x2b,
        STI_SPINNKEY  = 0x2c,
        STI_SPELLDISE = 0x2d,
        STI_SPELLCHKN = 0x2e,
        STI_SPELLDWAL = 0x2f,
        STI_SPELLTBMB = 0x30,
        STI_HEROGATE  = 0x31,
        STI_SPINNKEY2 = 0x32,

        /*UNTESTED - indices may differ by one */
        STI_ARMOUR    = 0x33,
        STI_GLDHOARD1 = 0x34,       /// small bag (800)
        STI_GLDHOARD2 = 0x35,       /// medium bag (1200)
        STI_GLDHOARD3 = 0x36,       /// big bag (1600)
        STI_GLDHOARD4 = 0x37,       /// big bag (1600)
        STI_GLDHOARD5 = 0x38,       /// big bag (1600)

        /*Creature lairs */
        STI_LAIRWIZRD = 0x39,
        STI_LAIRBARBR = 0x3a,
        STI_LAIRARCHR = 0x3b,
        STI_LAIRMONK  = 0x3c,
        STI_LAIRDWRFA = 0x3d,
        STI_LAIRKNGHT = 0x3e,
        STI_LAIRAVATR = 0x3f,
        STI_LAIRTUNLR = 0x40,
        STI_LAIRWITCH = 0x41,
        STI_LAIRGIANT = 0x42,
        STI_LAIRFAIRY = 0x43,
        STI_LAIRTHEFT = 0x44,
        STI_LAIRSAMUR = 0x45,
        STI_LAIRHORNY = 0x46,
        STI_LAIRSKELT = 0x47,
        STI_LAIRGOBLN = 0x48,
        STI_LAIRDRAGN = 0x49,
        STI_LAIRDEMSP = 0x4a,
        STI_LAIRFLY   = 0x4b,
        STI_LAIRDKMIS = 0x4c,
        STI_LAIRSORCR = 0x4d,
        STI_LAIRBILDM = 0x4e,
        STI_LAIRIMP   = 0x4f,
        STI_LAIRBUG   = 0x50,
        STI_LAIRVAMP  = 0x51,
        STI_LAIRSPIDR = 0x52,
        STI_LAIRHLHND = 0x53,
        STI_LAIRGHOST = 0x54,
        STI_LAIRTENTC = 0x55,

        STI_SPREVMAP  = 0x56,
        STI_SPRESURCT = 0x57,
        STI_SPTRANSFR = 0x58,
        STI_SPSTEALHR = 0x59,
        STI_SPMULTPLY = 0x5a,
        STI_SPINCLEV  = 0x5b,
        STI_SPMKSAFE  = 0x5c,
        STI_SPHIDNWRL = 0x5d,
        STI_TBBOULDER = 0x5e,
        STI_TBALARM   = 0x5f,
        STI_TBPOISONG = 0x60,
        STI_TBLIGHTNG = 0x61,
        STI_TBWRDOFPW = 0x62,
        STI_TBLAVA    = 0x63,
        STI_TBDUMMY2  = 0x64,
        STI_TBDUMMY3  = 0x65,
        STI_TBDUMMY4  = 0x66,
        STI_TBDUMMY5  = 0x67,
        STI_TBDUMMY6  = 0x68,
        STI_TBDUMMY7  = 0x69,
        STI_DBWOOD    = 0x6a,
        STI_DBBRACE   = 0x6b,
        STI_DBSTEEL   = 0x6c,
        STI_DBMAGIC   = 0x6d,
        STI_WBITEM    = 0x6e,
        STI_HEARTFLMR = 0x6f,

        STI_DISEASE   = 0x70,
        STI_SCAVNGEYE = 0x71,
        STI_WRKSHPMCH = 0x72,
        STI_GURDFLAGR = 0x73,
        STI_GURDFLAGB = 0x74,
        STI_GURDFLAGG = 0x75,
        STI_GURDFLAGY = 0x76,
        STI_FLAGPOST  = 0x77,
        STI_HEARTFLMB = 0x78,
        STI_HEARTFLMG = 0x79,
        STI_HEARTFLMY = 0x7a,
        STI_PWSIGHT   = 0x7b,
        STI_PWLIGHTNG = 0x7c,
        STI_TORTURER  = 0x7d,
        STI_LAIRORC   = 0x7e,
        STI_PWHANDGLD = 0x7f,
        STI_SPINNCOIN = 0x80,
        STI_STATUE2   = 0x81,
        STI_STATUE3   = 0x82,
        STI_STATUE4   = 0x83,
        STI_STATUE5   = 0x84,
        STI_SPCUSTOM  = 0x85,
        STI_SPELLARMG = 0x86
    };

    const std::set< SubTypeItem >& Traps();

    const std::set< SubTypeItem >& Spells();

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_ */
