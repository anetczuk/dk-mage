/*
 * SubTypeDoor.cpp
 *
 */

#include "adiktedpp/SubTypeDoor.h"


namespace adiktedpp {

    const std::set< SubTypeDoor >& Doors() {
        static std::set< SubTypeDoor > items;
        if ( items.empty() ) {
            items.insert( SubTypeDoor::STD_WOOD );
            items.insert( SubTypeDoor::STD_BRACED );
            items.insert( SubTypeDoor::STD_IRON );
            items.insert( SubTypeDoor::STD_MAGIC );
        }
        return items;
    }

}
