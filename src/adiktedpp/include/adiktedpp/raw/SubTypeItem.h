/*
 * SubTypeItem.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_

/// inclusion: #include "adiktedpp/raw/SubTypeItem.h"

#include <set>
#include <ostream>


namespace adiktedpp {
    namespace raw {

        /**
         * It's enum representation of 'ITEM_SUBTYPE_*' defines in 'obj_things.h' header.
         */
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

            /// gold for treasure room
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

        inline std::ostream& operator<<( std::ostream& os, const SubTypeItem data ) {
            switch( data ) {
            case SubTypeItem::STI_NULL:           { os << "STI_NULL"; return os; }
            case SubTypeItem::STI_BARREL:         { os << "STI_BARREL"; return os; }
            case SubTypeItem::STI_TORCH:          { os << "STI_TORCH"; return os; }
            case SubTypeItem::STI_GOLDCHEST:      { os << "STI_GOLDCHEST"; return os; }
            case SubTypeItem::STI_TEMPLESTA:      { os << "STI_TEMPLESTA"; return os; }
            case SubTypeItem::STI_DNHEART:        { os << "STI_DNHEART"; return os; }
            case SubTypeItem::STI_GOLD:           { os << "STI_GOLD"; return os; }
            case SubTypeItem::STI_TORCHUN:        { os << "STI_TORCHUN"; return os; }
            case SubTypeItem::STI_STATUEWO:       { os << "STI_STATUEWO"; return os; }
            case SubTypeItem::STI_CHICKNGRW:      { os << "STI_CHICKNGRW"; return os; }
            case SubTypeItem::STI_CHICKN:         { os << "STI_CHICKN"; return os; }
            case SubTypeItem::STI_SPELLHOE:       { os << "STI_SPELLHOE"; return os; }
            case SubTypeItem::STI_SPELLIMP:       { os << "STI_SPELLIMP"; return os; }
            case SubTypeItem::STI_SPELLMUST:      { os << "STI_SPELLMUST"; return os; }
            case SubTypeItem::STI_SPELLSLAP:      { os << "STI_SPELLSLAP"; return os; }
            case SubTypeItem::STI_SPELLSOE:       { os << "STI_SPELLSOE"; return os; }
            case SubTypeItem::STI_SPELLCTA:       { os << "STI_SPELLCTA"; return os; }
            case SubTypeItem::STI_SPELLCAVI:      { os << "STI_SPELLCAVI"; return os; }
            case SubTypeItem::STI_SPELLHEAL:      { os << "STI_SPELLHEAL"; return os; }
            case SubTypeItem::STI_SPELLHLDA:      { os << "STI_SPELLHLDA"; return os; }
            case SubTypeItem::STI_SPELLLIGH:      { os << "STI_SPELLLIGH"; return os; }
            case SubTypeItem::STI_SPELLSPDC:      { os << "STI_SPELLSPDC"; return os; }
            case SubTypeItem::STI_SPELLPROT:      { os << "STI_SPELLPROT"; return os; }
            case SubTypeItem::STI_SPELLCONC:      { os << "STI_SPELLCONC"; return os; }

            case SubTypeItem::STI_NULL1:          { os << "STI_NULL1"; return os; }
            case SubTypeItem::STI_NULL2:          { os << "STI_NULL2"; return os; }

            case SubTypeItem::STI_ANVIL:          { os << "STI_ANVIL"; return os; }
            case SubTypeItem::STI_PRISONBAR:      { os << "STI_PRISONBAR"; return os; }
            case SubTypeItem::STI_CANDLSTCK:      { os << "STI_CANDLSTCK"; return os; }
            case SubTypeItem::STI_GRAVSTONE:      { os << "STI_GRAVSTONE"; return os; }
            case SubTypeItem::STI_STATUHORN:      { os << "STI_STATUHORN"; return os; }
            case SubTypeItem::STI_TRAINPOST:      { os << "STI_TRAINPOST"; return os; }
            case SubTypeItem::STI_TORTSPIKE:      { os << "STI_TORTSPIKE"; return os; }
            case SubTypeItem::STI_TEMPLESPN:      { os << "STI_TEMPLESPN"; return os; }

            case SubTypeItem::STI_POTION1:        { os << "STI_POTION1"; return os; }
            case SubTypeItem::STI_POTION2:        { os << "STI_POTION2"; return os; }
            case SubTypeItem::STI_POTION3:        { os << "STI_POTION3"; return os; }
            case SubTypeItem::STI_PWHAND:         { os << "STI_PWHAND"; return os; }
            case SubTypeItem::STI_PWHANDGRB:      { os << "STI_PWHANDGRB"; return os; }
            case SubTypeItem::STI_PWHANDWHP:      { os << "STI_PWHANDWHP"; return os; }
            case SubTypeItem::STI_CHICKNSTB:      { os << "STI_CHICKNSTB"; return os; }
            case SubTypeItem::STI_CHICKNWOB:      { os << "STI_CHICKNWOB"; return os; }
            case SubTypeItem::STI_CHICKNCRK:      { os << "STI_CHICKNCRK"; return os; }
            case SubTypeItem::STI_GOLDL:          { os << "STI_GOLDL"; return os; }
            case SubTypeItem::STI_SPINNKEY:       { os << "STI_SPINNKEY"; return os; }
            case SubTypeItem::STI_SPELLDISE:      { os << "STI_SPELLDISE"; return os; }
            case SubTypeItem::STI_SPELLCHKN:      { os << "STI_SPELLCHKN"; return os; }
            case SubTypeItem::STI_SPELLDWAL:      { os << "STI_SPELLDWAL"; return os; }
            case SubTypeItem::STI_SPELLTBMB:      { os << "STI_SPELLTBMB"; return os; }
            case SubTypeItem::STI_HEROGATE:       { os << "STI_HEROGATE"; return os; }
            case SubTypeItem::STI_SPINNKEY2:      { os << "STI_SPINNKEY2"; return os; }

            case SubTypeItem::STI_ARMOUR:         { os << "STI_ARMOUR"; return os; }
            case SubTypeItem::STI_GLDHOARD1:      { os << "STI_GLDHOARD1"; return os; }
            case SubTypeItem::STI_GLDHOARD2:      { os << "STI_GLDHOARD2"; return os; }
            case SubTypeItem::STI_GLDHOARD3:      { os << "STI_GLDHOARD3"; return os; }
            case SubTypeItem::STI_GLDHOARD4:      { os << "STI_GLDHOARD4"; return os; }
            case SubTypeItem::STI_GLDHOARD5:      { os << "STI_GLDHOARD5"; return os; }

            case SubTypeItem::STI_LAIRWIZRD:      { os << "STI_LAIRWIZRD"; return os; }
            case SubTypeItem::STI_LAIRBARBR:      { os << "STI_LAIRBARBR"; return os; }
            case SubTypeItem::STI_LAIRARCHR:      { os << "STI_LAIRARCHR"; return os; }
            case SubTypeItem::STI_LAIRMONK:       { os << "STI_LAIRMONK"; return os; }
            case SubTypeItem::STI_LAIRDWRFA:      { os << "STI_LAIRDWRFA"; return os; }
            case SubTypeItem::STI_LAIRKNGHT:      { os << "STI_LAIRKNGHT"; return os; }
            case SubTypeItem::STI_LAIRAVATR:      { os << "STI_LAIRAVATR"; return os; }
            case SubTypeItem::STI_LAIRTUNLR:      { os << "STI_LAIRTUNLR"; return os; }
            case SubTypeItem::STI_LAIRWITCH:      { os << "STI_LAIRWITCH"; return os; }
            case SubTypeItem::STI_LAIRGIANT:      { os << "STI_LAIRGIANT"; return os; }
            case SubTypeItem::STI_LAIRFAIRY:      { os << "STI_LAIRFAIRY"; return os; }
            case SubTypeItem::STI_LAIRTHEFT:      { os << "STI_LAIRTHEFT"; return os; }
            case SubTypeItem::STI_LAIRSAMUR:      { os << "STI_LAIRSAMUR"; return os; }
            case SubTypeItem::STI_LAIRHORNY:      { os << "STI_LAIRHORNY"; return os; }
            case SubTypeItem::STI_LAIRSKELT:      { os << "STI_LAIRSKELT"; return os; }
            case SubTypeItem::STI_LAIRGOBLN:      { os << "STI_LAIRGOBLN"; return os; }
            case SubTypeItem::STI_LAIRDRAGN:      { os << "STI_LAIRDRAGN"; return os; }
            case SubTypeItem::STI_LAIRDEMSP:      { os << "STI_LAIRDEMSP"; return os; }
            case SubTypeItem::STI_LAIRFLY:        { os << "STI_LAIRFLY"; return os; }
            case SubTypeItem::STI_LAIRDKMIS:      { os << "STI_LAIRDKMIS"; return os; }
            case SubTypeItem::STI_LAIRSORCR:      { os << "STI_LAIRSORCR"; return os; }
            case SubTypeItem::STI_LAIRBILDM:      { os << "STI_LAIRBILDM"; return os; }
            case SubTypeItem::STI_LAIRIMP:        { os << "STI_LAIRIMP"; return os; }
            case SubTypeItem::STI_LAIRBUG:        { os << "STI_LAIRBUG"; return os; }
            case SubTypeItem::STI_LAIRVAMP:       { os << "STI_LAIRVAMP"; return os; }
            case SubTypeItem::STI_LAIRSPIDR:      { os << "STI_LAIRSPIDR"; return os; }
            case SubTypeItem::STI_LAIRHLHND:      { os << "STI_LAIRHLHND"; return os; }
            case SubTypeItem::STI_LAIRGHOST:      { os << "STI_LAIRGHOST"; return os; }
            case SubTypeItem::STI_LAIRTENTC:      { os << "STI_LAIRTENTC"; return os; }

            case SubTypeItem::STI_SPREVMAP:       { os << "STI_SPREVMAP"; return os; }
            case SubTypeItem::STI_SPRESURCT:      { os << "STI_SPRESURCT"; return os; }
            case SubTypeItem::STI_SPTRANSFR:      { os << "STI_SPTRANSFR"; return os; }
            case SubTypeItem::STI_SPSTEALHR:      { os << "STI_SPSTEALHR"; return os; }
            case SubTypeItem::STI_SPMULTPLY:      { os << "STI_SPMULTPLY"; return os; }
            case SubTypeItem::STI_SPINCLEV:       { os << "STI_SPINCLEV"; return os; }
            case SubTypeItem::STI_SPMKSAFE:       { os << "STI_SPMKSAFE"; return os; }
            case SubTypeItem::STI_SPHIDNWRL:      { os << "STI_SPHIDNWRL"; return os; }
            case SubTypeItem::STI_TBBOULDER:      { os << "STI_TBBOULDER"; return os; }
            case SubTypeItem::STI_TBALARM:        { os << "STI_TBALARM"; return os; }
            case SubTypeItem::STI_TBPOISONG:      { os << "STI_TBPOISONG"; return os; }
            case SubTypeItem::STI_TBLIGHTNG:      { os << "STI_TBLIGHTNG"; return os; }
            case SubTypeItem::STI_TBWRDOFPW:      { os << "STI_TBWRDOFPW"; return os; }
            case SubTypeItem::STI_TBLAVA:         { os << "STI_TBLAVA"; return os; }
            case SubTypeItem::STI_TBDUMMY2:       { os << "STI_TBDUMMY2"; return os; }
            case SubTypeItem::STI_TBDUMMY3:       { os << "STI_TBDUMMY3"; return os; }
            case SubTypeItem::STI_TBDUMMY4:       { os << "STI_TBDUMMY4"; return os; }
            case SubTypeItem::STI_TBDUMMY5:       { os << "STI_TBDUMMY5"; return os; }
            case SubTypeItem::STI_TBDUMMY6:       { os << "STI_TBDUMMY6"; return os; }
            case SubTypeItem::STI_TBDUMMY7:       { os << "STI_TBDUMMY7"; return os; }
            case SubTypeItem::STI_DBWOOD:         { os << "STI_DBWOOD"; return os; }
            case SubTypeItem::STI_DBBRACE:        { os << "STI_DBBRACE"; return os; }
            case SubTypeItem::STI_DBSTEEL:        { os << "STI_DBSTEEL"; return os; }
            case SubTypeItem::STI_DBMAGIC:        { os << "STI_DBMAGIC"; return os; }
            case SubTypeItem::STI_WBITEM:         { os << "STI_WBITEM"; return os; }
            case SubTypeItem::STI_HEARTFLMR:      { os << "STI_HEARTFLMR"; return os; }
            case SubTypeItem::STI_DISEASE:        { os << "STI_DISEASE"; return os; }
            case SubTypeItem::STI_SCAVNGEYE:      { os << "STI_SCAVNGEYE"; return os; }
            case SubTypeItem::STI_WRKSHPMCH:      { os << "STI_WRKSHPMCH"; return os; }
            case SubTypeItem::STI_GURDFLAGR:      { os << "STI_GURDFLAGR"; return os; }
            case SubTypeItem::STI_GURDFLAGB:      { os << "STI_GURDFLAGB"; return os; }
            case SubTypeItem::STI_GURDFLAGG:      { os << "STI_GURDFLAGG"; return os; }
            case SubTypeItem::STI_GURDFLAGY:      { os << "STI_GURDFLAGY"; return os; }
            case SubTypeItem::STI_FLAGPOST:       { os << "STI_FLAGPOST"; return os; }
            case SubTypeItem::STI_HEARTFLMB:      { os << "STI_HEARTFLMB"; return os; }
            case SubTypeItem::STI_HEARTFLMG:      { os << "STI_HEARTFLMG"; return os; }
            case SubTypeItem::STI_HEARTFLMY:      { os << "STI_HEARTFLMY"; return os; }
            case SubTypeItem::STI_PWSIGHT:        { os << "STI_PWSIGHT"; return os; }
            case SubTypeItem::STI_PWLIGHTNG:      { os << "STI_PWLIGHTNG"; return os; }
            case SubTypeItem::STI_TORTURER:       { os << "STI_TORTURER"; return os; }
            case SubTypeItem::STI_LAIRORC:        { os << "STI_LAIRORC"; return os; }
            case SubTypeItem::STI_PWHANDGLD:      { os << "STI_PWHANDGLD"; return os; }
            case SubTypeItem::STI_SPINNCOIN:      { os << "STI_SPINNCOIN"; return os; }
            case SubTypeItem::STI_STATUE2:        { os << "STI_STATUE2"; return os; }
            case SubTypeItem::STI_STATUE3:        { os << "STI_STATUE3"; return os; }
            case SubTypeItem::STI_STATUE4:        { os << "STI_STATUE4"; return os; }
            case SubTypeItem::STI_STATUE5:        { os << "STI_STATUE5"; return os; }
            case SubTypeItem::STI_SPCUSTOM:       { os << "STI_SPCUSTOM"; return os; }
            case SubTypeItem::STI_SPELLARMG:      { os << "STI_SPELLARMG"; return os; }
            }
            os << "UNKNOWN[" << (int) data << "]";
            return os;
        }


        const std::set< SubTypeItem >& TrapBoxes();

        const std::set< SubTypeItem >& SubTypeItemSpells();

    }
}


#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SUBTYPEITEM_H_ */
