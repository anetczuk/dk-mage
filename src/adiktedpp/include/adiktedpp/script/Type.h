/*
 * Type.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_

#include <set>
#include <ostream>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        enum class AvailableMode {
            AM_DISABLED,
            AM_POSSIBLE,            /// available after meet certain criteria (e.g. research or build proper room)
            AM_ENABLED
        };


        /**
         *
         */
        enum class AvailableCommand {
            SAC_ROOM_AVAILABLE,
            SAC_CREATURE_AVAILABLE,
            SAC_DOOR_AVAILABLE,
            SAC_TRAP_AVAILABLE,
            SAC_MAGIC_AVAILABLE
        };

        inline std::ostream& operator<<( std::ostream& os, const AvailableCommand data ) {
            switch( data ) {
            case AvailableCommand::SAC_ROOM_AVAILABLE:       { os << "ROOM_AVAILABLE"; return os; }
            case AvailableCommand::SAC_CREATURE_AVAILABLE:       { os << "CREATURE_AVAILABLE"; return os; }
            case AvailableCommand::SAC_DOOR_AVAILABLE:         { os << "DOOR_AVAILABLE"; return os; }
            case AvailableCommand::SAC_TRAP_AVAILABLE:        { os << "TRAP_AVAILABLE"; return os; }
            case AvailableCommand::SAC_MAGIC_AVAILABLE:       { os << "MAGIC_AVAILABLE"; return os; }
            }
            os << "UNKNOWN_ROOM[" << (int) data << "]";
            return os;
        }


        /**
         *
         */
        enum class Room {
            R_TREASURE,
            R_RESEARCH,         /// library
            R_PRISON,
            R_TORTURE,
            R_TRAINING,
            R_WORKSHOP,
            R_SCAVENGER,
            R_TEMPLE,
            R_GRAVEYARD,
            R_GARDEN,           /// hatchery
            R_LAIR,
            R_BARRACKS,
            R_BRIDGE,
            R_GUARD_POST
        };

        std::ostream& operator<<( std::ostream& os, const Room data );

        const std::set< Room >& Rooms();


        /**
         *
         */
        enum class Creature {
            C_WIZARD,
            C_BARBARIAN,
            C_ARCHER,
            C_MONK,
            C_DWARFA,
            C_KNIGHT,
            C_AVATAR,
            C_TUNNELLER,
            C_WITCH,
            C_GIANT,
            C_FAIRY,
            C_THIEF,
            C_SAMURAI,

            C_IMP,
            C_HORNY,
            C_SKELETON,
            C_TROLL,
            C_DRAGON,
            C_DEMONSPAWN,
            C_FLY,
            C_DARK_MISTRESS,
            C_SORCEROR,
            C_BILE_DEMON,
            C_BUG,
            C_VAMPIRE,
            C_SPIDER,
            C_HELL_HOUND,
            C_GHOST,
            C_TENTACLE,
            C_ORC
        };

        std::ostream& operator<<( std::ostream& os, const Creature data );

        const std::set< Creature >& EvilCreatures();

        const std::set< Creature >& HeroCreatures();


        /**
         *
         */
        enum class Door {
            D_WOOD,
            D_BRACED,
            D_STEEL,
            D_MAGIC
        };

        std::ostream& operator<<( std::ostream& os, const Door data );

        const std::set< Door >& Doors();


        /**
         *
         */
        enum class Trap {
            T_BOULDER,
            T_ALARM,
            T_POISON_GAS,
            T_LIGHTNING,
            T_WORD_OF_POWER,
            T_LAVA
        };

        std::ostream& operator<<( std::ostream& os, const Trap data );

        const std::set< Trap >& Traps();


        /**
         *
         */
        enum class Spell {
            S_POWER_HAND,
            S_POWER_SLAP,
            S_POWER_POSSESS,
            S_POWER_IMP,
            S_POWER_OBEY,
            S_POWER_SIGHT,
            S_POWER_CALL_TO_ARMS,
            S_POWER_CAVE_IN,
            S_POWER_HEAL_CREATURE,
            S_POWER_HOLD_AUDIENCE,
            S_POWER_LIGHTNING,
            S_POWER_SPEED,
            S_POWER_PROTECT,
            S_POWER_CONCEAL,
            S_POWER_DISEASE,
            S_POWER_CHICKEN,
            S_POWER_DESTROY_WALLS,
            S_POWER_ARMAGEDDON
        };

        std::ostream& operator<<( std::ostream& os, const Spell data );

        const std::set< Spell >& Spells();


    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_ */
