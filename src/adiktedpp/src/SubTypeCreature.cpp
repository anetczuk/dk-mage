/*
 * SubTypeCreature.cpp
 *
 */

#include "adiktedpp/SubTypeCreature.h"


namespace adiktedpp {

    const std::set< SubTypeCreature >& EvilCreatures() {
        static std::set< SubTypeCreature > creatures;
        if ( creatures.empty() ) {
            creatures.insert( SubTypeCreature::STC_HORNY );
            creatures.insert( SubTypeCreature::STC_SKELETON );
            creatures.insert( SubTypeCreature::STC_TROLL );
            creatures.insert( SubTypeCreature::STC_DRAGON );
            creatures.insert( SubTypeCreature::STC_SPAWN );
            creatures.insert( SubTypeCreature::STC_FLY );
            creatures.insert( SubTypeCreature::STC_MISTRESS );
            creatures.insert( SubTypeCreature::STC_WARLOCK );
            creatures.insert( SubTypeCreature::STC_BILEDEMN );
            creatures.insert( SubTypeCreature::STC_IMP );
            creatures.insert( SubTypeCreature::STC_BEETLE );
            creatures.insert( SubTypeCreature::STC_VAMPIRE );
            creatures.insert( SubTypeCreature::STC_SPIDER );
            creatures.insert( SubTypeCreature::STC_HOUND );
            creatures.insert( SubTypeCreature::STC_GHOST );
            creatures.insert( SubTypeCreature::STC_TENTCL );
            creatures.insert( SubTypeCreature::STC_ORC );
            creatures.insert( SubTypeCreature::STC_FLOAT );
        }
        return creatures;
    }

}
