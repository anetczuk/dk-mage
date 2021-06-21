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
        STI_TORCH     = 0x02,                   /// torch with fire (high)
        STI_GOLDCHEST = 0x03,                   /// gold bag (500)
        STI_TEMPLESTA = 0x04,                   /// statue sitting idol without number with flame
        STI_DNHEART   = 0x05,
        STI_GOLD      = 0x06,                   /// gold bag (250)
        STI_TORCHUN   = 0x07,                   /// torch without fire (high)
        STI_STATUEWO  = 0x08,                   /// statue sitting idol without number (missing flame)
        STI_CHICKNGRW = 0x09,                   /// chicken
        STI_CHICKN    = 0x0a,                   /// not working?

        /// spells
        STI_SPELLHOE  = 0x0b,                   /// time bomb (unfinished)
        STI_SPELLIMP  = 0x0c,                   /// create imp
        STI_SPELLMUST = 0x0d,                   /// must obey
        STI_SPELLSLAP = 0x0e,                   /// slap
        STI_SPELLSOE  = 0x0f,                   /// sight of evil
        STI_SPELLCTA  = 0x10,                   /// call to arms
        STI_SPELLCAVI = 0x11,                   /// cave-in
        STI_SPELLHEAL = 0x12,                   /// heal
        STI_SPELLHLDA = 0x13,                   /// hold audience
        STI_SPELLLIGH = 0x14,                   /// lightning
        STI_SPELLSPDC = 0x15,                   /// speed monster
        STI_SPELLPROT = 0x16,                   /// protection
        STI_SPELLCONC = 0x17,                   /// conceal monster
        STI_SPELLDISE = 0x2d,                   /// disease
        STI_SPELLCHKN = 0x2e,                   /// chicken
        STI_SPELLDWAL = 0x2f,                   /// destroy wall
        STI_SPELLTBMB = 0x30,                   /// time bomb (unfinished)
        STI_SPELLARMG = 0x86,

        STI_NULL1     = 0x18,
        STI_NULL2     = 0x19,

        STI_ANVIL     = 0x1a,               /// standard anvil
        STI_PRISONBAR = 0x1b,
        STI_CANDLSTCK = 0x1c,               /*candle with candlestick */
        STI_GRAVSTONE = 0x1d,
        STI_STATUHORN = 0x1e,               /// statue of idol
        STI_TRAINPOST = 0x1f,
        STI_TORTSPIKE = 0x20,
        STI_TEMPLESPN = 0x21,               /*Temple Spangle */

        STI_POTION1   = 0x22,
        STI_POTION2   = 0x23,
        STI_POTION3   = 0x24,
        STI_PWHAND    = 0x25,               /// keeper hand
        STI_PWHANDGRB = 0x26,               /// keeper hand
        STI_PWHANDWHP = 0x27,               /// keeper hand
        STI_CHICKNSTB = 0x28,
        STI_CHICKNWOB = 0x29,
        STI_CHICKNCRK = 0x2a,
        STI_GOLDL     = 0x2b,           /// gold (200)
        STI_SPINNKEY  = 0x2c,                   /// spinning key low
        STI_SPINNKEY2 = 0x32,                   /// spinning key high

        STI_HEROGATE  = 0x31,

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
        STI_LAIRORC   = 0x7e,

        /// specials
        STI_SPREVMAP  = 0x56,                      /// special: reveal map
        STI_SPRESURCT = 0x57,                      /// special: resurrect
        STI_SPTRANSFR = 0x58,                      /// special: transfer
        STI_SPSTEALHR = 0x59,                      /// special: steal hero
        STI_SPMULTPLY = 0x5a,                      /// special: multiply monsters
        STI_SPINCLEV  = 0x5b,                      /// special: increase level
        STI_SPMKSAFE  = 0x5c,                      /// special: make safe
        STI_SPHIDNWRL = 0x5d,                      /// special: hidden world

        /// traps boxes
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

        /// doors
        STI_DBWOOD    = 0x6a,
        STI_DBBRACE   = 0x6b,
        STI_DBSTEEL   = 0x6c,
        STI_DBMAGIC   = 0x6d,

        STI_WBITEM    = 0x6e,                       /// standard anvil
        STI_HEARTFLMR = 0x6f,
        STI_HEARTFLMB = 0x78,
        STI_HEARTFLMG = 0x79,
        STI_HEARTFLMY = 0x7a,

        STI_DISEASE   = 0x70,
        STI_SCAVNGEYE = 0x71,
        STI_WRKSHPMCH = 0x72,
        STI_GURDFLAGR = 0x73,
        STI_GURDFLAGB = 0x74,
        STI_GURDFLAGG = 0x75,
        STI_GURDFLAGY = 0x76,
        STI_FLAGPOST  = 0x77,
        STI_PWSIGHT   = 0x7b,                       /// spinning circle (spell effect)
        STI_PWLIGHTNG = 0x7c,
        STI_TORTURER  = 0x7d,
        STI_PWHANDGLD = 0x7f,                       /// keeper hand holding gold
        STI_SPINNCOIN = 0x80,
        STI_STATUE2   = 0x81,                       /// statue sitting idol (dark)
        STI_STATUE3   = 0x82,                       /// statue sitting idol with number 3
        STI_STATUE4   = 0x83,                       /// statue sitting idol with number 4
        STI_STATUE5   = 0x84,                       /// statue sitting idol with number 5
        STI_SPCUSTOM  = 0x85                        /// statue sitting idol with number 6
    };

    const std::set< SubTypeItem >& TrapBoxes();

    const std::set< SubTypeItem >& Spells();

}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_ */
