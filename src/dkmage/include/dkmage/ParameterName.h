/*
 * ParameterName.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_PARAMETERNAME_H_
#define DKMAGE_INCLUDE_DKMAGE_PARAMETERNAME_H_

#include <set>
#include <string>


namespace dkmage {

    /**
     *
     */
    enum class ParameterName {
        PN_CONFIG,
        PN_DATA_PATH,
        PN_LEVELS_PATH,
        PN_OUTPUT_PATH,
        PN_OUTPUT_SUBPATH,
        PN_OUTPUT_ID,
        PN_OUTPUT_BMP,

        PN_SEED,
        PN_TYPE,

        PN_ACTION_POINTS_LIMIT,

        PN_SCRIPT_TUNNELLERS_LIMIT,
        PN_SCRIPT_PARTIES_LIMIT,
        PN_SCRIPT_VALUES_LIMIT,
        PN_SCRIPT_IF_CONDS_LIMIT,
        PN_SCRIPT_PARTY_DEFS_LIMIT,

        PN_GOLD_SLABS_NUMBER,
        PN_GEM_SLABS_NUMBER,
        PN_GEM_FACES_NUMBER,
        PN_GEM_TRAPS_NUMBER,
        PN_GEM_GUARD_NUMBER,
        PN_GEM_GUARD_LEVEL,
        PN_INIT_GOLD_AMOUNT,
        PN_CREATURES_LIMIT,

        PN_ENEMY_KEEPER_ATTITUDE,
        PN_ENEMY_KEEPER_ROOMS_NUMBER,
        PN_ENEMY_KEEPER_CREATURES_LIMIT,

        PN_CENTAL_CAVERNS_NUMBER,
        PN_CAVERN_CREATURES_NUMBER,
        PN_CAVERN_CREATURES_LEVEL,

        PN_ENTRANCES_NUMBER,
        PN_BRIDGE_GUARD_LEVEL,
        PN_CORRIDOR_GUARD_LEVEL,
        PN_TORTURE_GUARD_LEVEL,
        PN_GRAVEYARD_GUARD_LEVEL,
        PN_DUNGEON_HEADER_GUARD_LEVEL,

        PN_TEST_MODE,
        PN_TEST_ADDONS,
        PN_STOP_ON_FAIL                         /// do not retry generation if attempt failed
    };

    const std::set< ParameterName >& getAllParameterNames();

    /**
     * User specific parameters (e.g. paths) that does not affect map content itself.
     */
    const std::set< ParameterName >& getLocalParameterNames();

    std::string getParameterName( const ParameterName parameter );

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters );


} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_PARAMETERNAME_H_ */
