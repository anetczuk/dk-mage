/*
 * Type.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_

#include <set>
#include <ostream>


namespace adiktedpp {

    /**
     *
     */
    enum class Slab {
        S_ROCK,
        S_GOLD,
        S_GEMS,
        S_EARTH,
        S_PATH,
        S_LAVA,
        S_WATER
    };


    /// ===============================================================


    /**
     *
     */
    enum class Room {
        R_CLAIMED,              /// empty
        R_DUNGEON_HEART,
        R_PORTAL,
        R_TREASURE,
        R_LIBRARY,
        R_PRISON,
        R_TORTURE,
        R_TRAINING,
        R_WORKSHOP,
        R_SCAVENGER,
        R_TEMPLE,
        R_GRAVEYARD,
        R_HATCHERY,
        R_LAIR,
        R_BARRACKS,
        R_BRIDGE,
        R_GUARD_POST
    };

    const std::set< Room >& RoomsAll();

    const std::set< Room >& RoomsRegular();


    /// ===============================================================


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
        C_WARLOCK,
        C_BILE_DEMON,
        C_BUG,
        C_VAMPIRE,
        C_SPIDER,
        C_HELL_HOUND,
        C_GHOST,
        C_TENTACLE,
        C_ORC
    };

    const std::set< Creature >& EvilCreatures();

    const std::set< Creature >& HeroCreatures();


    /// ===============================================================


    /**
     *
     */
    enum class Door {
        D_WOOD,
        D_BRACED,
        D_IRON,
        D_MAGIC
    };

    const std::set< Door >& Doors();


    /// ===============================================================


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

    const std::set< Trap >& Traps();


    /// ===============================================================


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

    const std::set< Spell >& Spells();


    /// ===============================================================


    /**
     *
     */
    enum class Item {
        I_GOLDCHEST,
        I_GOLD_HOARD1,              /// small bag (800)
        I_GOLD_HOARD2,              /// medium bag (1200)
        I_GOLD_HOARD3,              /// big bag (1600)
        I_GOLD_HOARD4,              /// big bag (1600)
        I_GOLD_HOARD5,              /// big bag (1600)

        I_SPECIAL_REVMAP,
        I_SPECIAL_RESURCT,
        I_SPECIAL_TRANSFR,
        I_SPECIAL_STEALHR,
        I_SPECIAL_MULTPLY,
        I_SPECIAL_INCLEV,
        I_SPECIAL_MKSAFE,
        I_SPECIAL_HIDNWRL
    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_TYPE_H_ */
