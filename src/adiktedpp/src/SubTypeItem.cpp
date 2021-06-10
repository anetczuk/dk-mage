/*
 * SubTypeItem.cpp
 *
 */

#include "adiktedpp/SubTypeItem.h"


namespace adiktedpp {

    const std::set< SubTypeItem >& Traps() {
        static std::set< SubTypeItem > traps;
        if ( traps.empty() ) {
            traps.insert( SubTypeItem::STI_TBBOULDER );
            traps.insert( SubTypeItem::STI_TBALARM );
            traps.insert( SubTypeItem::STI_TBPOISONG );
            traps.insert( SubTypeItem::STI_TBLIGHTNG );
            traps.insert( SubTypeItem::STI_TBWRDOFPW );
            traps.insert( SubTypeItem::STI_TBLAVA );
        }
        return traps;
    }

    const std::set< SubTypeItem >& Spells() {
        static std::set< SubTypeItem > traps;
        if ( traps.empty() ) {
            traps.insert( SubTypeItem::STI_SPELLIMP );      /// disabled by default
            traps.insert( SubTypeItem::STI_SPELLMUST );
            traps.insert( SubTypeItem::STI_SPELLSOE );
            traps.insert( SubTypeItem::STI_SPELLCTA );
            traps.insert( SubTypeItem::STI_SPELLCAVI );
            traps.insert( SubTypeItem::STI_SPELLHEAL );
            traps.insert( SubTypeItem::STI_SPELLHLDA );
            traps.insert( SubTypeItem::STI_SPELLLIGH );
            traps.insert( SubTypeItem::STI_SPELLSPDC );
            traps.insert( SubTypeItem::STI_SPELLPROT );
            traps.insert( SubTypeItem::STI_SPELLCONC );
            traps.insert( SubTypeItem::STI_SPELLDISE );
            traps.insert( SubTypeItem::STI_SPELLCHKN );
            traps.insert( SubTypeItem::STI_SPELLDWAL );
            traps.insert( SubTypeItem::STI_SPELLARMG );

//            traps.insert( SubTypeItem::STI_SPELLHOE );      /// posses, enabled by default
//            traps.insert( SubTypeItem::STI_PWHANDGRB );     /// grab creature, enabled by default
//            traps.insert( SubTypeItem::STI_PWHANDWHP );     /// slap creature, enabled by default
        }
        return traps;
    }

}
