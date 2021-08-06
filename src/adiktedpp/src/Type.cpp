/*
 * Type.cpp
 *
 */

#include "adiktedpp/Type.h"


namespace adiktedpp {

    const std::set< Room >& RoomsAll() {
        static std::set< Room > container;
        if ( container.empty() ) {
            container.insert( Room::R_CLAIMED );
            container.insert( Room::R_DUNGEON_HEART );
            container.insert( Room::R_PORTAL );

            container.insert( Room::R_TREASURE );
            container.insert( Room::R_LAIR );
            container.insert( Room::R_HATCHERY );
            container.insert( Room::R_TRAINING );
            container.insert( Room::R_LIBRARY );
            container.insert( Room::R_BRIDGE );
            container.insert( Room::R_WORKSHOP );
            container.insert( Room::R_GUARD_POST );
            container.insert( Room::R_PRISON );
            container.insert( Room::R_TORTURE );
            container.insert( Room::R_BARRACKS );
            container.insert( Room::R_TEMPLE );
            container.insert( Room::R_GRAVEYARD );
            container.insert( Room::R_SCAVENGER );
        }
        return container;
    }

    const std::set< Room >& RoomsRegular() {
        static std::set< Room > container;
        if ( container.empty() ) {
            container.insert( Room::R_TREASURE );
            container.insert( Room::R_LAIR );
            container.insert( Room::R_HATCHERY );
            container.insert( Room::R_TRAINING );
            container.insert( Room::R_LIBRARY );
            container.insert( Room::R_BRIDGE );
            container.insert( Room::R_WORKSHOP );
            container.insert( Room::R_GUARD_POST );
            container.insert( Room::R_PRISON );
            container.insert( Room::R_TORTURE );
            container.insert( Room::R_BARRACKS );
            container.insert( Room::R_TEMPLE );
            container.insert( Room::R_GRAVEYARD );
            container.insert( Room::R_SCAVENGER );
        }
        return container;
    }


    /// ============================================================================


    const std::set< Trap >& Traps() {
        static std::set< Trap > container;
        if ( container.empty() ) {
            container.insert( Trap::T_BOULDER );
            container.insert( Trap::T_ALARM );
            container.insert( Trap::T_POISON_GAS );
            container.insert( Trap::T_LIGHTNING );
            container.insert( Trap::T_WORD_OF_POWER );
            container.insert( Trap::T_LAVA );
        }
        return container;
    }


    /// ============================================================================


    const std::set< Door >& Doors() {
        static std::set< Door > container;
        if ( container.empty() ) {
            container.insert( Door::D_WOOD );
            container.insert( Door::D_BRACED );
            container.insert( Door::D_IRON );
            container.insert( Door::D_MAGIC );
        }
        return container;
    }


    /// ============================================================================


    const std::set< Spell >& Spells() {
        static std::set< Spell > container;
        if ( container.empty() ) {
            container.insert( Spell::S_POWER_HAND );
            container.insert( Spell::S_POWER_SLAP );
            container.insert( Spell::S_POWER_POSSESS );
            container.insert( Spell::S_POWER_IMP );
            container.insert( Spell::S_POWER_OBEY );
            container.insert( Spell::S_POWER_SIGHT );
            container.insert( Spell::S_POWER_CALL_TO_ARMS );
            container.insert( Spell::S_POWER_CAVE_IN );
            container.insert( Spell::S_POWER_HEAL_CREATURE );
            container.insert( Spell::S_POWER_HOLD_AUDIENCE );
            container.insert( Spell::S_POWER_LIGHTNING );
            container.insert( Spell::S_POWER_SPEED );
            container.insert( Spell::S_POWER_PROTECT );
            container.insert( Spell::S_POWER_CONCEAL );
            container.insert( Spell::S_POWER_DISEASE );
            container.insert( Spell::S_POWER_CHICKEN );
            container.insert( Spell::S_POWER_DESTROY_WALLS );
            container.insert( Spell::S_POWER_ARMAGEDDON );
        }
        return container;
    }


    /// ====================================================


    const std::set< Creature >& EvilCreatures() {
        static std::set< Creature > container;
        if ( container.empty() ) {
            container.insert( Creature::C_IMP );
            container.insert( Creature::C_FLY );
            container.insert( Creature::C_GHOST );
            container.insert( Creature::C_WARLOCK );
            container.insert( Creature::C_BEETLE );
            container.insert( Creature::C_SPIDER );
            container.insert( Creature::C_TROLL );
            container.insert( Creature::C_DEMONSPAWN );
            container.insert( Creature::C_TENTACLE );
            container.insert( Creature::C_SKELETON );
            container.insert( Creature::C_HELL_HOUND );
            container.insert( Creature::C_MISTRESS );
            container.insert( Creature::C_ORC );
            container.insert( Creature::C_BILE_DEMON );
            container.insert( Creature::C_DRAGON );
            container.insert( Creature::C_VAMPIRE );
            container.insert( Creature::C_HORNY );
        }
        return container;
    }

    const std::set< Creature >& HeroCreatures() {
        static std::set< Creature > container;
        if ( container.empty() ) {
            container.insert( Creature::C_TUNNELLER );
            container.insert( Creature::C_FAIRY );
            container.insert( Creature::C_ARCHER );
            container.insert( Creature::C_PRIESTESS );
            container.insert( Creature::C_WIZARD );
            container.insert( Creature::C_THIEF );
            container.insert( Creature::C_MONK );
            container.insert( Creature::C_DWARF );
            container.insert( Creature::C_BARBARIAN );
            container.insert( Creature::C_SAMURAI );
            container.insert( Creature::C_GIANT );
            container.insert( Creature::C_KNIGHT );
            container.insert( Creature::C_AVATAR );
        }
        return container;
    }

} /* namespace adiktedpp */
