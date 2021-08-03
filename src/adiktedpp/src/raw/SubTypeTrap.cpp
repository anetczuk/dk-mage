/*
 * SubTypeTrap.cpp
 *
 */

#include "adiktedpp/raw/SubTypeTrap.h"


namespace adiktedpp {
    namespace raw {

        const std::set< SubTypeTrap >& DamageTraps() {
            static std::set< SubTypeTrap > traps;
            if ( traps.empty() ) {
                traps.insert( SubTypeTrap::STT_BOULDER );
                traps.insert( SubTypeTrap::STT_GAS );
                traps.insert( SubTypeTrap::STT_LIGHTNG );
                traps.insert( SubTypeTrap::STT_WORDPWR );
                traps.insert( SubTypeTrap::STT_LAVA );
            }
            return traps;
        }

    }
}
