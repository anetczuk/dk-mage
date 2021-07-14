/*
 * Type.cpp
 *
 */

#include "adiktedpp/script/Type.h"


namespace adiktedpp {
    namespace script {

        std::ostream& operator<<( std::ostream& os, const Room data ) {
            switch( data ) {
            case Room::R_TREASURE:       { os << "TREASURE"; return os; }
            case Room::R_RESEARCH:       { os << "RESEARCH"; return os; }
            case Room::R_PRISON:         { os << "PRISON"; return os; }
            case Room::R_TORTURE:        { os << "TORTURE"; return os; }
            case Room::R_TRAINING:       { os << "TRAINING"; return os; }
            case Room::R_WORKSHOP:       { os << "WORKSHOP"; return os; }
            case Room::R_SCAVENGER:      { os << "SCAVENGER"; return os; }
            case Room::R_TEMPLE:         { os << "TEMPLE"; return os; }
            case Room::R_GRAVEYARD:      { os << "GRAVEYARD"; return os; }
            case Room::R_GARDEN:         { os << "GARDEN"; return os; }
            case Room::R_LAIR:           { os << "LAIR"; return os; }
            case Room::R_BARRACKS:       { os << "BARRACKS"; return os; }
            case Room::R_BRIDGE:         { os << "BRIDGE"; return os; }
            case Room::R_GUARD_POST:     { os << "GUARD_POST"; return os; }
            }
            os << "UNKNOWN_ROOM[" << (int) data << "]";
            return os;
        }

        const std::set< Room >& Rooms() {
            static std::set< Room > container;
            if ( container.empty() ) {
                container.insert( Room::R_TREASURE );
                container.insert( Room::R_RESEARCH );
                container.insert( Room::R_PRISON );
                container.insert( Room::R_TORTURE );
                container.insert( Room::R_TRAINING );
                container.insert( Room::R_WORKSHOP );
                container.insert( Room::R_SCAVENGER );
                container.insert( Room::R_TEMPLE );
                container.insert( Room::R_GRAVEYARD );
                container.insert( Room::R_GARDEN );
                container.insert( Room::R_LAIR );
                container.insert( Room::R_BARRACKS );
                container.insert( Room::R_BRIDGE );
                container.insert( Room::R_GUARD_POST );
            }
            return container;
        }


        /// ====================================================


        std::ostream& operator<<( std::ostream& os, const Creature data ) {
            switch( data ) {
            case Creature::C_WIZARD:      { os << "WIZARD"; return os; }
            case Creature::C_BARBARIAN:   { os << "BARBARIAN"; return os; }
            case Creature::C_ARCHER:      { os << "ARCHER"; return os; }
            case Creature::C_MONK:        { os << "MONK"; return os; }
            case Creature::C_DWARFA:      { os << "DWARFA"; return os; }
            case Creature::C_KNIGHT:      { os << "KNIGHT"; return os; }
            case Creature::C_AVATAR:      { os << "AVATAR"; return os; }
            case Creature::C_TUNNELLER:   { os << "TUNNELLER"; return os; }
            case Creature::C_WITCH:       { os << "WITCH"; return os; }
            case Creature::C_GIANT:       { os << "GIANT"; return os; }
            case Creature::C_FAIRY:       { os << "FAIRY"; return os; }
            case Creature::C_THIEF:       { os << "THIEF"; return os; }
            case Creature::C_SAMURAI:     { os << "SAMURAI"; return os; }

            case Creature::C_IMP:       { os << "IMP"; return os; }
            case Creature::C_HORNY:       { os << "HORNY"; return os; }
            case Creature::C_SKELETON:       { os << "SKELETON"; return os; }
            case Creature::C_TROLL:       { os << "TROLL"; return os; }
            case Creature::C_DRAGON:       { os << "DRAGON"; return os; }
            case Creature::C_DEMONSPAWN:       { os << "DEMONSPAWN"; return os; }
            case Creature::C_FLY:       { os << "FLY"; return os; }
            case Creature::C_DARK_MISTRESS:       { os << "DARK_MISTRESS"; return os; }
            case Creature::C_SORCEROR:       { os << "SORCEROR"; return os; }
            case Creature::C_BILE_DEMON:       { os << "BILE_DEMON"; return os; }
            case Creature::C_BUG:       { os << "BUG"; return os; }
            case Creature::C_VAMPIRE:       { os << "VAMPIRE"; return os; }
            case Creature::C_SPIDER:       { os << "SPIDER"; return os; }
            case Creature::C_HELL_HOUND:       { os << "HELL_HOUND"; return os; }
            case Creature::C_GHOST:       { os << "GHOST"; return os; }
            case Creature::C_TENTACLE:       { os << "TENTACLE"; return os; }
            case Creature::C_ORC:       { os << "ORC"; return os; }
            }
            os << "UNKNOWN_CEATURE[" << (int) data << "]";
            return os;
        }

        const std::set< Creature >& EvilCreatures() {
            static std::set< Creature > container;
            if ( container.empty() ) {
                container.insert( Creature::C_IMP );
                container.insert( Creature::C_HORNY );
                container.insert( Creature::C_SKELETON );
                container.insert( Creature::C_TROLL );
                container.insert( Creature::C_DRAGON );
                container.insert( Creature::C_DEMONSPAWN );
                container.insert( Creature::C_FLY );
                container.insert( Creature::C_DARK_MISTRESS );
                container.insert( Creature::C_SORCEROR );
                container.insert( Creature::C_BILE_DEMON );
                container.insert( Creature::C_BUG );
                container.insert( Creature::C_VAMPIRE );
                container.insert( Creature::C_SPIDER );
                container.insert( Creature::C_HELL_HOUND );
                container.insert( Creature::C_GHOST );
                container.insert( Creature::C_TENTACLE );
                container.insert( Creature::C_ORC );
            }
            return container;
        }

        const std::set< Creature >& HeroCreatures() {
            static std::set< Creature > container;
            if ( container.empty() ) {
                container.insert( Creature::C_WIZARD );
                container.insert( Creature::C_BARBARIAN );
                container.insert( Creature::C_ARCHER );
                container.insert( Creature::C_MONK );
                container.insert( Creature::C_DWARFA );
                container.insert( Creature::C_KNIGHT );
                container.insert( Creature::C_AVATAR );
                container.insert( Creature::C_TUNNELLER );
                container.insert( Creature::C_WITCH );
                container.insert( Creature::C_GIANT );
                container.insert( Creature::C_FAIRY );
                container.insert( Creature::C_THIEF );
                container.insert( Creature::C_SAMURAI );
            }
            return container;
        }


        /// ============================================================================


        std::ostream& operator<<( std::ostream& os, const Door data ) {
            switch( data ) {
            case Door::D_WOOD:           { os << "WOOD"; return os; }
            case Door::D_BRACED:         { os << "BRACED"; return os; }
            case Door::D_STEEL:          { os << "STEEL"; return os; }
            case Door::D_MAGIC:          { os << "MAGIC"; return os; }
            }
            os << "UNKNOWN_DOOR[" << (int) data << "]";
            return os;
        }

        const std::set< Door >& Doors() {
            static std::set< Door > container;
            if ( container.empty() ) {
                container.insert( Door::D_WOOD );
                container.insert( Door::D_BRACED );
                container.insert( Door::D_STEEL );
                container.insert( Door::D_MAGIC );
            }
            return container;
        }


        /// ============================================================================


        std::ostream& operator<<( std::ostream& os, const Trap data ) {
            switch( data ) {
            case Trap::T_BOULDER:        { os << "BOULDER"; return os; }
            case Trap::T_ALARM:          { os << "ALARM"; return os; }
            case Trap::T_POISON_GAS:     { os << "POISON_GAS"; return os; }
            case Trap::T_LIGHTNING:      { os << "LIGHTNING"; return os; }
            case Trap::T_WORD_OF_POWER:  { os << "WORD_OF_POWER"; return os; }
            case Trap::T_LAVA:           { os << "LAVA"; return os; }
            }
            os << "UNKNOWN_TRAP[" << (int) data << "]";
            return os;
        }

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


        std::ostream& operator<<( std::ostream& os, const Spell data ) {
            switch( data ) {
            case Spell::S_POWER_IMP:            { os << "POWER_IMP"; return os; }
            case Spell::S_POWER_OBEY:           { os << "POWER_OBEY"; return os; }
            case Spell::S_POWER_SIGHT:          { os << "POWER_SIGHT"; return os; }
            case Spell::S_POWER_CALL_TO_ARMS:   { os << "POWER_CALL_TO_ARMS"; return os; }
            case Spell::S_POWER_CAVE_IN:        { os << "POWER_CAVE_IN"; return os; }
            case Spell::S_POWER_HEAL_CREATURE:  { os << "POWER_HEAL_CREATURE"; return os; }
            case Spell::S_POWER_HOLD_AUDIENCE:  { os << "POWER_HOLD_AUDIENCE"; return os; }
            case Spell::S_POWER_LIGHTNING:      { os << "POWER_LIGHTNING"; return os; }
            case Spell::S_POWER_SPEED:          { os << "POWER_SPEED"; return os; }
            case Spell::S_POWER_PROTECT:        { os << "POWER_PROTECT"; return os; }
            case Spell::S_POWER_CONCEAL:        { os << "POWER_CONCEAL"; return os; }
            case Spell::S_POWER_DISEASE:        { os << "POWER_DISEASE"; return os; }
            case Spell::S_POWER_CHICKEN:        { os << "POWER_CHICKEN"; return os; }
            case Spell::S_POWER_DESTROY_WALLS:  { os << "POWER_DESTROY_WALLS"; return os; }
            case Spell::S_POWER_ARMAGEDDON:     { os << "POWER_ARMAGEDDON"; return os; }
            case Spell::S_POWER_POSSESS:        { os << "POWER_POSSESS"; return os; }
            case Spell::S_POWER_SLAP:           { os << "POWER_SLAP"; return os; }
            case Spell::S_POWER_HAND:           { os << "POWER_HAND"; return os; }
            }
            os << "UNKNOWN_SPELL[" << (int) data << "]";
            return os;
        }

        const std::set< Spell >& Spells() {
            static std::set< Spell > container;
            if ( container.empty() ) {
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
                container.insert( Spell::S_POWER_POSSESS );
                container.insert( Spell::S_POWER_SLAP );
                container.insert( Spell::S_POWER_HAND );
            }
            return container;
        }

    } /* namespace script */
} /* namespace adiktedpp */
