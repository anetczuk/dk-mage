/*
 * ParameterName.cpp
 *
 */

#include "dkmage/ParameterName.h"

#include "utils/Log.h"


namespace dkmage {

    const std::set< ParameterName >& getAllParameterNames() {
        static std::set< ParameterName > data;
        if ( data.empty() ) {
            data.insert( ParameterName::PN_CONFIG );
            data.insert( ParameterName::PN_DATA_PATH );
            data.insert( ParameterName::PN_LEVELS_PATH );
            data.insert( ParameterName::PN_OUTPUT_PATH );
            data.insert( ParameterName::PN_OUTPUT_SUBPATH );
            data.insert( ParameterName::PN_OUTPUT_ID );
            data.insert( ParameterName::PN_OUTPUT_BMP );

            data.insert( ParameterName::PN_SEED );
            data.insert( ParameterName::PN_TYPE );

            data.insert( ParameterName::PN_ACTION_POINTS_LIMIT );

            data.insert( ParameterName::PN_SCRIPT_TUNNELLERS_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_PARTIES_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_VALUES_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_IF_CONDS_LIMIT );
            data.insert( ParameterName::PN_SCRIPT_PARTY_DEFS_LIMIT );

            data.insert( ParameterName::PN_GOLD_SLABS_NUMBER );
            data.insert( ParameterName::PN_GEM_SLABS_NUMBER );
            data.insert( ParameterName::PN_GEM_FACES_NUMBER );
            data.insert( ParameterName::PN_GEM_TRAPS_NUMBER );
            data.insert( ParameterName::PN_GEM_GUARD_NUMBER );
            data.insert( ParameterName::PN_GEM_GUARD_LEVEL );
            data.insert( ParameterName::PN_INIT_GOLD_AMOUNT );
            data.insert( ParameterName::PN_CREATURES_LIMIT );

            data.insert( ParameterName::PN_ENEMY_KEEPER_ATTITUDE );
            data.insert( ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER );
            data.insert( ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT );

            data.insert( ParameterName::PN_CENTAL_CAVERNS_NUMBER );
            data.insert( ParameterName::PN_CAVERN_CREATURES_NUMBER );
            data.insert( ParameterName::PN_CAVERN_CREATURES_LEVEL );

            data.insert( ParameterName::PN_ENTRANCES_NUMBER );
            data.insert( ParameterName::PN_BRIDGE_GUARD_LEVEL );
            data.insert( ParameterName::PN_CORRIDOR_GUARD_LEVEL );
            data.insert( ParameterName::PN_TORTURE_GUARD_LEVEL );
            data.insert( ParameterName::PN_GRAVEYARD_GUARD_LEVEL );
            data.insert( ParameterName::PN_DUNGEON_HEADER_GUARD_LEVEL );

            data.insert( ParameterName::PN_TEST_MODE );
            data.insert( ParameterName::PN_STOP_ON_FAIL );
        }
        return data;
    }

    const std::set< ParameterName >& getLocalParameterNames() {
        static std::set< ParameterName > data;
        if ( data.empty() ) {
            data.insert( ParameterName::PN_CONFIG );
            data.insert( ParameterName::PN_DATA_PATH );
            data.insert( ParameterName::PN_LEVELS_PATH );
            data.insert( ParameterName::PN_OUTPUT_PATH );
            data.insert( ParameterName::PN_OUTPUT_SUBPATH );
            data.insert( ParameterName::PN_OUTPUT_ID );
            data.insert( ParameterName::PN_OUTPUT_BMP );

            data.insert( ParameterName::PN_STOP_ON_FAIL );
        }
        return data;
    }

    std::string getParameterName( const ParameterName parameter ) {
        switch( parameter ) {
        case ParameterName::PN_CONFIG:          return "config";
        case ParameterName::PN_DATA_PATH:       return "data_path";
        case ParameterName::PN_LEVELS_PATH:     return "levels_path";
        case ParameterName::PN_OUTPUT_PATH:     return "output_path";
        case ParameterName::PN_OUTPUT_SUBPATH:  return "output_subpath";
        case ParameterName::PN_OUTPUT_ID:       return "output_id";
        case ParameterName::PN_OUTPUT_BMP:      return "output_bmp";

        case ParameterName::PN_SEED:                return "seed";
        case ParameterName::PN_TYPE:                return "type";

        case ParameterName::PN_ACTION_POINTS_LIMIT:   return "action_points_limit";

        case ParameterName::PN_SCRIPT_TUNNELLERS_LIMIT:   return "script_tunnellers_limit";
        case ParameterName::PN_SCRIPT_PARTIES_LIMIT:      return "script_parties_limit";
        case ParameterName::PN_SCRIPT_VALUES_LIMIT:       return "script_values_limit";
        case ParameterName::PN_SCRIPT_IF_CONDS_LIMIT:     return "script_if_conds_limit";
        case ParameterName::PN_SCRIPT_PARTY_DEFS_LIMIT:   return "script_party_defs_limit";

        case ParameterName::PN_GOLD_SLABS_NUMBER:   return "gold_slabs_number";
        case ParameterName::PN_GEM_SLABS_NUMBER:    return "gem_slabs_number";
        case ParameterName::PN_GEM_FACES_NUMBER:    return "gem_faces_number";
        case ParameterName::PN_GEM_TRAPS_NUMBER:    return "gem_traps_number";
        case ParameterName::PN_GEM_GUARD_NUMBER:    return "gem_guard_number";
        case ParameterName::PN_GEM_GUARD_LEVEL:     return "gem_guard_level";
        case ParameterName::PN_INIT_GOLD_AMOUNT:    return "init_gold_amount";
        case ParameterName::PN_CREATURES_LIMIT:     return "creatures_limit";

        case ParameterName::PN_ENEMY_KEEPER_ATTITUDE:           return "enemy_keeper_attitude";
        case ParameterName::PN_ENEMY_KEEPER_ROOMS_NUMBER:       return "enemy_keeper_rooms_number";
        case ParameterName::PN_ENEMY_KEEPER_CREATURES_LIMIT:    return "enemy_keeper_creatures_limit";

        case ParameterName::PN_CENTAL_CAVERNS_NUMBER:    return "cental_caverns_number";
        case ParameterName::PN_CAVERN_CREATURES_NUMBER:  return "cavern_creatures_number";
        case ParameterName::PN_CAVERN_CREATURES_LEVEL:   return "cavern_creatures_level";

        case ParameterName::PN_ENTRANCES_NUMBER:            return "entrances_number";
        case ParameterName::PN_BRIDGE_GUARD_LEVEL:          return "bridge_guard_level";
        case ParameterName::PN_CORRIDOR_GUARD_LEVEL:        return "corridor_guard_level";
        case ParameterName::PN_TORTURE_GUARD_LEVEL:         return "torture_guard_level";
        case ParameterName::PN_GRAVEYARD_GUARD_LEVEL:       return "graveyard_guard_level";
        case ParameterName::PN_DUNGEON_HEADER_GUARD_LEVEL:  return "dungeon_header_guard_level";

        case ParameterName::PN_TEST_MODE:       return "test_mode";
        case ParameterName::PN_TEST_ADDONS:     return "test_addons";
        case ParameterName::PN_STOP_ON_FAIL:    return "stop_on_fail";
        }

        std::stringstream stream;
        stream << FILE_NAME << ": invalid argument -- unavailable player: " << (int)parameter;
        LOG() << stream.str();
        throw std::invalid_argument( stream.str() );
    }

    std::set< std::string > getParameterName( const std::set< ParameterName >& parameters ) {
        std::set< std::string > ret;
        for ( const auto item: parameters ) {
            const std::string name = getParameterName( item );
            ret.insert( name );
        }
        return ret;
    }

} /* namespace dkmage */
