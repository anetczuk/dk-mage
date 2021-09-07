/*
 * Script.cpp
 *
 */

#include "adiktedpp/script/Script.h"

#include "adiktedpp/Messages.h"
#include "adiktedpp/Version.h"

#include "utils/Log.h"

#include <algorithm>


extern "C" {
    #include "libadikted/lev_files.h"
    #include "libadikted/lev_script.h"
    #include "libadikted/lev_data.h"
}


namespace adiktedpp {
    namespace script {

        static const std::string WHITESPACE = " \n\r\t\f\v";

        static std::string ltrim( const std::string &s ) {
            std::size_t start = s.find_first_not_of(WHITESPACE);
            return (start == std::string::npos) ? "" : s.substr(start);
        }

        std::string rtrim( const std::string &s ) {
            std::size_t end = s.find_last_not_of(WHITESPACE);
            return (end == std::string::npos) ? "" : s.substr(0, end + 1);
        }

        std::string trim( const std::string &s ) {
            return rtrim(ltrim(s));
        }


        /// =================================================================================


        std::string script_keyword( const Player data ) {
            switch( data ) {
            case Player::P_P0:      return "PLAYER0";
            case Player::P_P1:      return "PLAYER1";
            case Player::P_P2:      return "PLAYER2";
            case Player::P_P3:      return "PLAYER3";
            case Player::P_GOOD:    return "PLAYER_GOOD";
            case Player::P_UNSET:   return "PLAYER_NEUTRAL";          /// available in Keeper FX
            case Player::P_ALL:     return "ALL_PLAYERS";
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }

        std::string script_keyword( const Slab data ) {
            /// names come from 'fxdata\terrain.cfg'
            switch( data ) {
            case Slab::S_ROCK:      return "HARD";
            case Slab::S_GOLD:      return "GOLD";
            case Slab::S_GEMS:      return "GEMS";
            case Slab::S_EARTH:     return "DIRT";
            case Slab::S_PATH:      return "PATH";
            case Slab::S_LAVA:      return "LAVA";
            case Slab::S_WATER:     return "WATER";
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Room data ) {
            switch( data ) {
            case Room::R_CLAIMED: {
                /// value not allowed in script file
                LOG_ERR() << "unsupported argument: " << (int)data;
                std::stringstream stream;
                stream << FILE_NAME << ": unsupported argument: " << (int)data;
                throw std::invalid_argument( stream.str() );
            }
            case Room::R_PORTAL: {
                /// value not allowed in script file
                LOG_ERR() << "unsupported argument: " << (int)data;
                std::stringstream stream;
                stream << FILE_NAME << ": unsupported argument: " << (int)data;
                throw std::invalid_argument( stream.str() );
            }

            case Room::R_DUNGEON_HEART:  { return "DUNGEON_HEART"; }

            case Room::R_TREASURE:       { return "TREASURE"; }
            case Room::R_LIBRARY:        { return "RESEARCH"; }
            case Room::R_TRAINING:       { return "TRAINING"; }
            case Room::R_HATCHERY:       { return "GARDEN"; }
            case Room::R_LAIR:           { return "LAIR"; }
            case Room::R_BRIDGE:         { return "BRIDGE"; }
            case Room::R_WORKSHOP:       { return "WORKSHOP"; }
            case Room::R_GUARD_POST:     { return "GUARD_POST"; }
            case Room::R_PRISON:         { return "PRISON"; }
            case Room::R_TORTURE:        { return "TORTURE"; }
            case Room::R_BARRACKS:       { return "BARRACKS"; }
            case Room::R_TEMPLE:         { return "TEMPLE"; }
            case Room::R_GRAVEYARD:      { return "GRAVEYARD"; }
            case Room::R_SCAVENGER:      { return "SCAVENGER"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Creature data ) {
            switch( data ) {
            case Creature::C_TUNNELLER:   { return "TUNNELLER"; }
            case Creature::C_FAIRY:       { return "FAIRY"; }
            case Creature::C_ARCHER:      { return "ARCHER"; }
            case Creature::C_PRIESTESS:   { return "WITCH"; }
            case Creature::C_WIZARD:      { return "WIZARD"; }
            case Creature::C_THIEF:       { return "THIEF"; }
            case Creature::C_MONK:        { return "MONK"; }
            case Creature::C_DWARF:       { return "DWARFA"; }
            case Creature::C_BARBARIAN:   { return "BARBARIAN"; }
            case Creature::C_SAMURAI:     { return "SAMURAI"; }
            case Creature::C_GIANT:       { return "GIANT"; }
            case Creature::C_KNIGHT:      { return "KNIGHT"; }
            case Creature::C_AVATAR:      { return "AVATAR"; }

            case Creature::C_IMP:           { return "IMP"; }
            case Creature::C_FLY:           { return "FLY"; }
            case Creature::C_GHOST:         { return "GHOST"; }
            case Creature::C_WARLOCK:       { return "SORCEROR"; }
            case Creature::C_BEETLE:        { return "BUG"; }
            case Creature::C_SPIDER:        { return "SPIDER"; }
            case Creature::C_TROLL:         { return "TROLL"; }
            case Creature::C_DEMONSPAWN:    { return "DEMONSPAWN"; }
            case Creature::C_TENTACLE:      { return "TENTACLE"; }
            case Creature::C_SKELETON:      { return "SKELETON"; }
            case Creature::C_HELL_HOUND:    { return "HELL_HOUND"; }
            case Creature::C_MISTRESS:      { return "DARK_MISTRESS"; }
            case Creature::C_ORC:           { return "ORC"; }
            case Creature::C_BILE_DEMON:    { return "BILE_DEMON"; }
            case Creature::C_DRAGON:        { return "DRAGON"; }
            case Creature::C_VAMPIRE:       { return "VAMPIRE"; }
            case Creature::C_HORNY:         { return "HORNY"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Door data ) {
            switch( data ) {
            case Door::D_WOOD:           { return "WOOD"; }
            case Door::D_BRACED:         { return "BRACED"; }
            case Door::D_IRON:           { return "STEEL"; }
            case Door::D_MAGIC:          { return "MAGIC"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Trap data ) {
            switch( data ) {
            case Trap::T_BOULDER:        { return "BOULDER"; }
            case Trap::T_ALARM:          { return "ALARM"; }
            case Trap::T_POISON_GAS:     { return "POISON_GAS"; }
            case Trap::T_LIGHTNING:      { return "LIGHTNING"; }
            case Trap::T_WORD_OF_POWER:  { return "WORD_OF_POWER"; }
            case Trap::T_LAVA:           { return "LAVA"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Spell data ) {
            switch( data ) {
            case Spell::S_POWER_HAND:           { return "POWER_HAND"; }
            case Spell::S_POWER_SLAP:           { return "POWER_SLAP"; }
            case Spell::S_POWER_POSSESS:        { return "POWER_POSSESS"; }
            case Spell::S_POWER_IMP:            { return "POWER_IMP"; }
            case Spell::S_POWER_OBEY:           { return "POWER_OBEY"; }
            case Spell::S_POWER_SIGHT:          { return "POWER_SIGHT"; }
            case Spell::S_POWER_CALL_TO_ARMS:   { return "POWER_CALL_TO_ARMS"; }
            case Spell::S_POWER_CAVE_IN:        { return "POWER_CAVE_IN"; }
            case Spell::S_POWER_HEAL_CREATURE:  { return "POWER_HEAL_CREATURE"; }
            case Spell::S_POWER_HOLD_AUDIENCE:  { return "POWER_HOLD_AUDIENCE"; }
            case Spell::S_POWER_LIGHTNING:      { return "POWER_LIGHTNING"; }
            case Spell::S_POWER_SPEED:          { return "POWER_SPEED"; }
            case Spell::S_POWER_PROTECT:        { return "POWER_PROTECT"; }
            case Spell::S_POWER_CONCEAL:        { return "POWER_CONCEAL"; }
            case Spell::S_POWER_DISEASE:        { return "POWER_DISEASE"; }
            case Spell::S_POWER_CHICKEN:        { return "POWER_CHICKEN"; }
            case Spell::S_POWER_DESTROY_WALLS:  { return "POWER_DESTROY_WALLS"; }
            case Spell::S_POWER_ARMAGEDDON:     { return "POWER_ARMAGEDDON"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const Flag data ) {
            switch( data ) {
            case Flag::F_FLAG_0: { return "FLAG0"; }
            case Flag::F_FLAG_1: { return "FLAG1"; }
            case Flag::F_FLAG_2: { return "FLAG2"; }
            case Flag::F_FLAG_3: { return "FLAG3"; }
            case Flag::F_FLAG_4: { return "FLAG4"; }
            case Flag::F_FLAG_5: { return "FLAG5"; }
            case Flag::F_FLAG_6: { return "FLAG6"; }
            case Flag::F_FLAG_7: { return "FLAG7"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const IfOption data ) {
            switch( data ) {
            case IfOption::IO_MONEY:                            return "MONEY";
            case IfOption::IO_GAME_TURN:                        return "GAME_TURN";
            case IfOption::IO_HEART_HEALTH:                     return "HEART_HEALTH";
            case IfOption::IO_TOTAL_DIGGERS:                    return "TOTAL_DIGGERS";
            case IfOption::IO_TOTAL_CREATURES:                  return "TOTAL_CREATURES";
            case IfOption::IO_EVIL_CREATURES:                   return "EVIL_CREATURES";
            case IfOption::IO_GOOD_CREATURES:                   return "GOOD_CREATURES";
            case IfOption::IO_TOTAL_RESEARCH:                   return "TOTAL_RESEARCH";
            case IfOption::IO_TOTAL_DOORS:                      return "TOTAL_DOORS";
            case IfOption::IO_TOTAL_DOORS_MANUFACTURED:         return "TOTAL_DOORS_MANUFACTURED";
            case IfOption::IO_TOTAL_DOORS_USED:                 return "TOTAL_DOORS_USED";
            case IfOption::IO_TOTAL_TRAPS_MANUFACTURED:         return "TOTAL_TRAPS_MANUFACTURED";
            case IfOption::IO_TOTAL_TRAPS_USED:                 return "TOTAL_TRAPS_USED";
            case IfOption::IO_TOTAL_MANUFACTURED:               return "TOTAL_MANUFACTURED";
            case IfOption::IO_TOTAL_AREA:                       return "TOTAL_AREA";
            case IfOption::IO_TOTAL_CREATURES_LEFT:             return "TOTAL_CREATURES_LEFT";
            case IfOption::IO_TOTAL_SALARY:                     return "TOTAL_SALARY";
            case IfOption::IO_CURRENT_SALARY:                   return "CURRENT_SALARY";
            case IfOption::IO_CREATURES_ANNOYED:                return "CREATURES_ANNOYED";
            case IfOption::IO_TIMES_ANNOYED_CREATURE:           return "TIMES_ANNOYED_CREATURE";
            case IfOption::IO_TIMES_TORTURED_CREATURE:          return "TIMES_TORTURED_CREATURE";
            case IfOption::IO_TIMES_LEVELUP_CREATURE:           return "TIMES_LEVELUP_CREATURE";
            case IfOption::IO_BATTLES_WON:                      return "BATTLES_WON";
            case IfOption::IO_BATTLES_LOST:                     return "BATTLES_LOST";
            case IfOption::IO_ROOMS_DESTROYED:                  return "ROOMS_DESTROYED";
            case IfOption::IO_SPELLS_STOLEN:                    return "SPELLS_STOLEN";
            case IfOption::IO_TIMES_BROKEN_INTO:                return "TIMES_BROKEN_INTO";
            case IfOption::IO_DUNGEON_DESTROYED:                return "DUNGEON_DESTROYED";
            case IfOption::IO_CREATURES_SCAVENGED_LOST:         return "CREATURES_SCAVENGED_LOST";
            case IfOption::IO_CREATURES_SCAVENGED_GAINED:       return "CREATURES_SCAVENGED_GAINED";
            case IfOption::IO_CREATURES_SACRIFICED:             return "CREATURES_SACRIFICED";
            case IfOption::IO_CREATURES_FROM_SACRIFICE:         return "CREATURES_FROM_SACRIFICE";
            case IfOption::IO_CREATURES_CONVERTED:              return "CREATURES_CONVERTED";
            case IfOption::IO_ALL_DUNGEONS_DESTROYED:           return "ALL_DUNGEONS_DESTROYED";
            case IfOption::IO_KEEPERS_DESTROYED:                return "KEEPERS_DESTROYED";
            case IfOption::IO_DOORS_DESTROYED:                  return "DOORS_DESTROYED";
            case IfOption::IO_TOTAL_GOLD_MINED:                 return "TOTAL_GOLD_MINED";
            case IfOption::IO_GOLD_POTS_STOLEN:                 return "GOLD_POTS_STOLEN";
            case IfOption::IO_BREAK_IN:                         return "BREAK_IN";
            case IfOption::IO_GHOSTS_RAISED:                    return "GHOSTS_RAISED";
            case IfOption::IO_SKELETONS_RAISED:                 return "SKELETONS_RAISED";
            case IfOption::IO_VAMPIRES_RAISED:                  return "VAMPIRES_RAISED";
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string script_keyword( const PartyObjective data ) {
            switch( data ) {
            case PartyObjective::PO_ATTACK_DUNGEON_HEART:   { return "ATTACK_DUNGEON_HEART"; }
            case PartyObjective::PO_ATTACK_ENEMIES:         { return "ATTACK_ENEMIES"; }
            case PartyObjective::PO_ATTACK_ROOMS:           { return "ATTACK_ROOMS"; }
            case PartyObjective::PO_DEFEND_HEART:           { return "DEFEND_HEART"; }
            case PartyObjective::PO_DEFEND_LOCATION:        { return "DEFEND_LOCATION"; }
            case PartyObjective::PO_DEFEND_PARTY:           { return "DEFEND_PARTY"; }
            case PartyObjective::PO_DEFEND_ROOMS:           { return "DEFEND_ROOMS"; }
            case PartyObjective::PO_STEAL_GOLD:             { return "STEAL_GOLD"; }
            case PartyObjective::PO_STEAL_SPELLS:           { return "STEAL_SPELLS"; }
            }

            LOG_ERR() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        /// =========================================================================


        void ScriptContainer::addEmptyLine( const std::size_t number ) {
            for ( std::size_t i=0; i<number; ++i ) {
                lines.push_back( "" );
            }
        }

        void ScriptContainer::addLine( const std::string& line ) {
            updateLevelEndIf( line );

            const std::string indent( level * 4, ' ' );
            lines.push_back( indent + line );

            updateLevelIf( line );
        }

        void ScriptContainer::addLineIndent( const std::string& line, const std::size_t forceIndent ) {
            updateLevelEndIf( line );

            const std::string indent( forceIndent * 4, ' ' );
            lines.push_back( indent + line );

            updateLevelIf( line );
        }

        static bool isIFCondition( const std::string& line ) {
            if ( line.find( "IF(" ) != std::string::npos ) {
                return true;
            } else if ( line.find( "IF_" ) != std::string::npos ) {
                return true;
            }
            return false;
        }

        static bool isENDIF( const std::string& line ) {
            if ( line.find( "ENDIF" ) != std::string::npos ) {
                return true;
            }
            return false;
        }

        static bool isTunnellerTrigger( const std::string& line ) {
            if ( line.find( "ADD_TUNNELLER_PARTY_TO_LEVEL(" ) != std::string::npos ) {
                return true;
            }
            return false;
        }

        static bool isPartyTrigger( const std::string& line ) {
            if ( line.find( "ADD_PARTY_TO_LEVEL(" ) != std::string::npos ) {
                return true;
            } else if ( line.find( "ADD_CREATURE_TO_LEVEL(" ) != std::string::npos ) {
                return true;
            }
            return false;
        }

        static bool isPartyDef( const std::string& line ) {
            if ( line.find( "CREATE_PARTY(" ) != std::string::npos ) {
                return true;
            }
            return false;
        }

        std::size_t ScriptContainer::countTunnellerTriggers() const {
            std::size_t counter = 0;
            for ( const std::string& line: lines ) {
                if ( isTunnellerTrigger( line ) ) {
                    ++counter;
                }
            }
            return counter;
        }

        std::size_t ScriptContainer::countPartyTriggers() const {
            std::size_t counter = 0;
            for ( const std::string& line: lines ) {
                if ( isPartyTrigger( line ) ) {
                    ++counter;
                }
            }
            return counter;
        }

        std::size_t ScriptContainer::countScriptValues() const {
            return countScriptValues( lines );
        }

        std::size_t ScriptContainer::countIfConditions() const {
            std::size_t counter = 0;
            for ( const std::string& line: lines ) {
                if ( isIFCondition(line) ) {
                    ++counter;
                }
            }
            return counter;
        }

        std::size_t ScriptContainer::countPartyDefinitions() const {
            std::size_t counter = 0;
            for ( const std::string& line: lines ) {
                if ( isPartyDef(line) ) {
                    ++counter;
                }
            }
            return counter;
        }

        void ScriptContainer::updateLevelIf( const std::string& line ) {
            if ( isIFCondition(line) ) {
                ++level;
            }
        }

        void ScriptContainer::updateLevelEndIf( const std::string& line ) {
            if ( isENDIF(line) ) {
                if (level > 0) {
                    --level;
                }
            }
        }

        std::size_t ScriptContainer::countScriptValues( const std::vector< std::string >& lines ) {
            std::size_t nestLevel = 0;
            std::size_t counter   = 0;
            for ( const std::string& item: lines ) {
                std::string line = trim( item );
                if ( line.empty() ) {
                    /// only white spaces
                    continue;
                }
                const bool whiteSpacesOnly = std::all_of( line.begin(), line.end(), ::isspace );
                if ( whiteSpacesOnly ) {
                    continue ;
                }

                std::transform( line.begin(), line.end(), line.begin(), ::toupper );

                if ( line.find( "REM " ) != std::string::npos ) {
                    /// comment
                    continue;
                }

                if ( isIFCondition(line) ) {
                    ++nestLevel;
                    continue ;
                }
                if ( isENDIF(line) ) {
                    if (nestLevel > 0) {
                        --nestLevel;
                    }
                    continue ;
                }

                if ( nestLevel < 1 ) {
                    /// ignore commands outside IF
                    continue ;
                }

                if ( isTunnellerTrigger(line) ) {
                    continue;
                }
                if ( isPartyTrigger(line) ) {
                    continue;
                }
                if ( isPartyDef(line) ) {
                    continue;
                }

                if ( line.find( "WIN_GAME" ) != std::string::npos ) {
                    continue;
                } else if ( line.find( "LOSE_GAME" ) != std::string::npos ) {
                    continue;
                }

                ++counter;
            }
            return counter;
        }


        /// =========================================================================


        void ScriptCommand::LEVEL_VERSION( const int version ) {
            std::stringstream stream;
            stream << "LEVEL_VERSION( " << version << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::SET_FLAG( const adiktedpp::Player player, const Flag flag, const int value ) {
            std::stringstream stream;
            stream << "SET_FLAG( " << script_keyword( player ) << ", " << script_keyword( flag ) << ", " << value << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::ADD_TO_FLAG( const adiktedpp::Player player, const Flag flag, const int value ) {
            std::stringstream stream;
            stream << "ADD_TO_FLAG( " << script_keyword( player ) << ", " << script_keyword( flag ) << ", " << value << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::QUICK_INFORMATION( const std::string& comment ) {
            QUICK_INFORMATION( infoIndex, comment );
            ++infoIndex;
        }

        void ScriptCommand::QUICK_INFORMATION( const std::size_t infoIndex, const std::string& comment ) {
            std::stringstream stream;
            /// ALL_PLAYERS -- zoom inactive
            stream << "QUICK_INFORMATION( " << infoIndex << ", \"" << comment << "\", ALL_PLAYERS )";
            addLine( stream.str() );
        }

        void ScriptCommand::START_MONEY( const adiktedpp::Player player, const std::size_t amount ) {
            std::stringstream stream;
            stream << "START_MONEY( " << script_keyword( player ) << ", " << amount << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::COMPUTER_PLAYER( const Player player, const std::size_t attitude ) {
            std::stringstream stream;
            stream << "COMPUTER_PLAYER( " << script_keyword( player ) + ", " << attitude << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::MAX_CREATURES( const Player player, const std::size_t limit ) {
            std::stringstream stream;
            stream << "MAX_CREATURES( " << script_keyword( player ) << ", " << limit << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::CONCEAL_MAP_RECT( const Player player, const utils::Rect rect, const bool hideAll ) {
            REM( "- hide whole map -" );
//            const utils::Point center = rect.center();
//            const std::size_t xSub = center.x * 3;
//            const std::size_t ySub = center.y * 3;
            const std::size_t widthSub  = rect.width()  * 3;
            const std::size_t heightSub = rect.height() * 3;

            static const std::size_t MAX_DIM = 128;

            const std::size_t wSteps = widthSub  / MAX_DIM + ( (widthSub  % MAX_DIM > 0) ? 1 : 0 );
            const std::size_t hSteps = heightSub / MAX_DIM + ( (heightSub % MAX_DIM > 0) ? 1 : 0 );

            const std::size_t rWidth  = widthSub  / wSteps;
            const std::size_t rHeight = heightSub / wSteps;

            const utils::Point corner = rect.leftBottom() * 3;

            for ( std::size_t w=0; w<wSteps; ++w ) {
                std::size_t xCenter = corner.x + rWidth  / 2 + rWidth  * w;
                for ( std::size_t h=0; h<hSteps; ++h ) {
                    std::size_t yCenter = corner.y + rHeight / 2 + rHeight * h;

        //            const std::size_t xSub = center.x * 3;
        //            const std::size_t ySub = center.y * 3;
        //            const std::size_t widthSub  = rect.width()  * 3;
        //            const std::size_t heightSub = rect.height() * 3;
                    std::stringstream stream;
                    stream << "CONCEAL_MAP_RECT( " << script_keyword( player )
                                                   << ", " << xCenter << ", " << yCenter
                                                   << ", " << rWidth << ", " << rHeight;
                    if ( hideAll ) {
                        stream << ", ALL";
                    }
                    stream << " )";
                    addLine( stream.str() );
                }
            }
        }

        void ScriptCommand::CHANGE_SLAB_TYPE( const utils::Point position, const Slab slab ) {
            std::stringstream stream;
            stream << "CHANGE_SLAB_TYPE( " << position.x << ", " << position.y << ", " << script_keyword( slab ) << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void ScriptCommand::IF( const adiktedpp::Player player, const Flag flag, const std::string& comparison, const int value ) {
            std::stringstream stream;
            stream << "IF( " << script_keyword( player ) << ", " << script_keyword( flag ) << " " << comparison << " " << value << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void ScriptCommand::IF( const adiktedpp::Player player, const IfOption option, const std::string& comparison, const int value ) {
            std::stringstream stream;
            stream << "IF( " << script_keyword( player ) << ", " << script_keyword( option ) << " " << comparison << " " << value << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void ScriptCommand::IF_SLAB_TYPE( const utils::Point position, const Slab slab ) {
            std::stringstream stream;
            stream << "IF_SLAB_TYPE( " << position.x << ", " << position.y << ", " << script_keyword( slab ) << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void ScriptCommand::IF_SLAB_OWNER( const utils::Point position, const adiktedpp::Player player ) {
            std::stringstream stream;
            stream << "IF_SLAB_OWNER( " << position.x << ", " << position.y << ", " << script_keyword( player ) << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void ScriptCommand::ENDIF() {
            addLine( "ENDIF" );
        }

        void ScriptCommand::IF_ACTION_POINT( const int actionNumber, const adiktedpp::Player player ) {
            std::stringstream stream;
            stream << "IF_ACTION_POINT( " << actionNumber << ", " << script_keyword( player ) << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        /// 'crExp' in range [1..10]
        void ScriptCommand::ADD_CREATURE_TO_LEVEL( const Player player, const Creature creature, const int actionPoint, const std::size_t crNum, const std::size_t crExp, const std::size_t gold ) {
            std::stringstream stream;
            stream << "ADD_CREATURE_TO_LEVEL( " << script_keyword( player ) + ", " << script_keyword( creature ) << ", "
                   << actionPoint << ", " << crNum << ", " << crExp << ", " << gold << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::CREATE_PARTY( const std::string& partyName ) {
            std::stringstream stream;
            stream << "CREATE_PARTY( " << partyName << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::ADD_TO_PARTY( const std::string& partyName, const Creature creature, const std::size_t crNum, const std::size_t crExp, const std::size_t gold, const PartyObjective objective, const std::size_t countdown ) {
            for ( std::size_t i=0; i<crNum; ++i ) {
                std::stringstream stream;
                stream << "ADD_TO_PARTY( ";
                stream << partyName << ", ";
                stream << script_keyword( creature ) << ", ";
                stream << crExp << ", ";
                stream << gold << ", ";
                stream << script_keyword( objective ) << ", ";
                stream << countdown << " )";
                addLine( stream.str() );
            }
        }

        void ScriptCommand::ADD_PARTY_TO_LEVEL( const Player player, const std::string& partyName, const int actionPoint, const std::size_t copies ) {
            std::stringstream stream;
            stream << "ADD_PARTY_TO_LEVEL( ";
            stream << script_keyword( player ) << ", ";
            stream << partyName << ", ";
            stream << actionPoint << ", ";
            stream << copies << " )";
            addLine( stream.str() );
        }

        void ScriptCommand::ADD_TUNNELLER_PARTY_TO_LEVEL( const Player player, const std::string& partyName, const int actionPoint, const std::size_t enemyNumber,
                                                          const std::size_t tunnelerExp, const std::size_t tunnelerGold )
        {
            std::stringstream stream;
            stream << "ADD_TUNNELLER_PARTY_TO_LEVEL( ";
            stream << script_keyword( player ) << ", ";
            stream << partyName << ", ";
            stream << actionPoint << ", ";
            stream << "DUNGEON" << ", ";
//            stream << "DUNGEON_HEART" << ", ";
            stream << enemyNumber << ", ";
            stream << tunnelerExp << ", ";
            stream << tunnelerGold << " )";
            addLine( stream.str() );
        }


        /// =========================================================================


        const std::set< ScriptSection >& AllScriptSections() {
            static std::set< ScriptSection > data;
            if ( data.empty() ) {
                data.insert( ScriptSection::SS_HEADER );
                data.insert( ScriptSection::SS_INIT );
                data.insert( ScriptSection::SS_PARTIES );
                data.insert( ScriptSection::SS_MAIN );
                data.insert( ScriptSection::SS_ACTION );
                data.insert( ScriptSection::SS_ENDCOND );
                data.insert( ScriptSection::SS_REST );
            }
            return data;
        }


        /// =========================================================================


        Script::Script(): isFX(false) {
            header.REM( "*** script generated by dk-mage ver. " + VERSION_FULL_STRING + " ***" );
        }

        void Script::clearData() {
            ///header.clear();      /// do not clear header information
            init.clear();
            parties.clear();
            main.clear();
            action.clear();
            endConditions.clear();
            other.clear();
        }

        std::size_t Script::countTunnellerTriggers() const {
            std::size_t counter = 0;
            const std::set< ScriptSection >& sectionList = AllScriptSections();
            for ( const auto item: sectionList ) {
                const ScriptCommand& currSection = getSection( item );
                counter += currSection.countTunnellerTriggers();
            }
            return counter;
        }

        std::size_t Script::countPartyTriggers() const {
            std::size_t counter = 0;
            const std::set< ScriptSection >& sectionList = AllScriptSections();
            for ( const auto item: sectionList ) {
                const ScriptCommand& currSection = getSection( item );
                counter += currSection.countPartyTriggers();
            }
            return counter;
        }

        std::size_t Script::countScriptValues() const {
            const std::vector< std::string > scriptLines = build();
            return ScriptContainer::countScriptValues( scriptLines );
        }

        std::size_t Script::countIfConditions() const {
            std::size_t counter = 0;
            const std::set< ScriptSection >& sectionList = AllScriptSections();
            for ( const auto item: sectionList ) {
                const ScriptCommand& currSection = getSection( item );
                counter += currSection.countIfConditions();
            }
            return counter;
        }

        std::size_t Script::countPartyDefinitions() const {
            std::size_t counter = 0;
            const std::set< ScriptSection >& sectionList = AllScriptSections();
            for ( const auto item: sectionList ) {
                const ScriptCommand& currSection = getSection( item );
                counter += currSection.countPartyDefinitions();
            }
            return counter;
        }

        void Script::storeParameters( const std::string& mapType, const std::string& seed ) {
            header.REM( "*** generated map type: '" + mapType + "' ***" );
            header.REM( "*** seed used for generation: '" + seed + "' ***" );
        }

        void Script::concealWholeMap( const Player player ) {
            const utils::Rect map = adiktedpp::raw::RawLevel::mapRect();
            init.CONCEAL_MAP_RECT( player, map, true );
        }

        void Script::addAvailable( const Player player, const Creature item, const int available, const int numberForced ) {
            /**
             * old interface:
             *      [can be available] should be always 1
             *      CREATURE_AVAILABLE(​ [player],​ [creature],​ [can be available],​ [is available] )
             *
             * new interface:
             *      [can be attracted] is [is available] from old interface
             *      ​[amount forced] number of attracted even if player doesn't have rooms required by that creature
             *      CREATURE_AVAILABLE(​ [player], ​[creature], ​[can be attracted], ​[amount forced] )
             */
            std::stringstream stream;
            stream << AvailableCommand::AC_CREATURE_AVAILABLE << "( " << script_keyword( player ) << ", " << script_keyword( item ) << ", " << available << ", " << numberForced << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Room item, const int researchable, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::AC_ROOM_AVAILABLE << "( " << script_keyword( player ) << ", " << script_keyword( item ) << ", " << researchable << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Door item, const int constructable, const int numberAvailable ) {
            std::stringstream stream;
            stream << AvailableCommand::AC_DOOR_AVAILABLE << "( " << script_keyword( player ) << ", " << script_keyword( item ) << ", " << constructable << ", " << numberAvailable << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Trap item, const int constructable, const int numberAvailable ) {
            std::stringstream stream;
            stream << AvailableCommand::AC_TRAP_AVAILABLE << "( " << script_keyword( player ) << ", " << script_keyword( item ) << ", " << constructable << ", " << numberAvailable << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Spell item, const int researchable, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::AC_MAGIC_AVAILABLE << "( " << script_keyword( player ) << ", " << script_keyword( item ) << ", " << researchable << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::set( const RoomsAvailableState& stateMap ) {
            auto iter1 = stateMap.begin();
            for ( ; iter1 != stateMap.end(); ++iter1 ) {
                const adiktedpp::Player player = iter1->first;
                const auto& submap = iter1->second;

                auto iter2 = submap.begin();
                for ( ; iter2 != submap.end(); ++iter2 ) {
                    const auto item   = iter2->first;
                    const auto& state = iter2->second;
                    addAvailable( player, item, state.accessible, state.available );
                }
            }
        }

        void Script::set( const CreatureAvailableState& stateMap ) {
            auto iter1 = stateMap.begin();
            for ( ; iter1 != stateMap.end(); ++iter1 ) {
                const adiktedpp::Player player = iter1->first;
                const auto& submap = iter1->second;

                auto iter2 = submap.begin();
                for ( ; iter2 != submap.end(); ++iter2 ) {
                    const auto item   = iter2->first;
                    const auto& state = iter2->second;
                    addAvailable( player, item, state.available, state.accessible );
                }
            }
        }

        void Script::set( const DoorAvailableState& stateMap ) {
            auto iter1 = stateMap.begin();
            for ( ; iter1 != stateMap.end(); ++iter1 ) {
                const adiktedpp::Player player = iter1->first;
                const auto& submap = iter1->second;

                auto iter2 = submap.begin();
                for ( ; iter2 != submap.end(); ++iter2 ) {
                    const auto item   = iter2->first;
                    const auto& state = iter2->second;
                    addAvailable( player, item, state.accessible, state.available );
                }
            }
        }

        void Script::set( const TrapAvailableState& stateMap ) {
            auto iter1 = stateMap.begin();
            for ( ; iter1 != stateMap.end(); ++iter1 ) {
                const adiktedpp::Player player = iter1->first;
                const auto& submap = iter1->second;

                auto iter2 = submap.begin();
                for ( ; iter2 != submap.end(); ++iter2 ) {
                    const auto item   = iter2->first;
                    const auto& state = iter2->second;
                    addAvailable( player, item, state.accessible, state.available );
                }
            }
        }

        void Script::set( const MagicAvailableState& stateMap ) {
            auto iter1 = stateMap.begin();
            for ( ; iter1 != stateMap.end(); ++iter1 ) {
                const adiktedpp::Player player = iter1->first;
                const auto& submap = iter1->second;

                auto iter2 = submap.begin();
                for ( ; iter2 != submap.end(); ++iter2 ) {
                    const auto item   = iter2->first;
                    const auto& state = iter2->second;
                    addAvailable( player, item, state.accessible, state.available );
                }
            }
        }

        void Script::setEvilCreaturesPool( const std::size_t number ) {
            std::set< Creature > list = EvilCreatures();
            list.erase( Creature::C_IMP );
            list.erase( Creature::C_GHOST );
            list.erase( Creature::C_TENTACLE );
            list.erase( Creature::C_SKELETON );
            list.erase( Creature::C_VAMPIRE );
            list.erase( Creature::C_HORNY );
            for ( const Creature item: list ) {
                setCreaturePool( item, number );
            }
        }

        void Script::setHeroCreaturesPool( const std::size_t number ) {
            std::set< Creature > list = HeroCreatures();
            list.erase( Creature::C_AVATAR );
            for ( const Creature item: list ) {
                setCreaturePool( item, number );
            }
        }

        void Script::setCreaturePool( const Creature creature, const std::size_t number ) {
            std::stringstream stream;
            stream << "ADD_CREATURE_TO_POOL( " << script_keyword( creature ) << ", " << number << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::setEvilCreaturesAvailable( const Player player, const bool available ) {
            CreatureAvailableState availableState;
            availableState.setEvilAvailable( player, available );
            set( availableState );
        }

        void Script::setHeroCreaturesAvailable( const Player player, const bool available ) {
            CreatureAvailableState availableState;
            availableState.setHeroAvailable( player, available );
            set( availableState );
        }

        void Script::setRoomsAvailable( const Player player, const AvailableRoomMode mode ) {
            RoomsAvailableState availableState;
            availableState.setAllAvailable( player, mode );
            set( availableState );
        }

        void Script::setRoomAvailable( const Player player, const Room room, const AvailableRoomMode mode ) {
            RoomsAvailableState availableState;
            availableState.setStateMode( player, room, mode );
            set( availableState );
        }

        void Script::setRoomsStandard() {
            RoomsAvailableState availableState;
            availableState.setStandard();
            set( availableState );
        }

        void Script::setDoorsAvailable( const Player player, const int available ) {
            DoorAvailableState availableState;
            availableState.setAllAvailableAmount( player, available );
            set( availableState );
        }

        void Script::setTrapsAvailable( const Player player, const int available ) {
            TrapAvailableState availableState;
            availableState.setAllAvailableAmount( player, available );
            set( availableState );
        }

        void Script::setMagicAvailable( const Player player, const AvailableMode mode ) {
            MagicAvailableState availableState;
            availableState.setAllAvailable( player, mode );
            set( availableState );
        }

        void Script::setMagicStandard( const Player player ) {
            MagicAvailableState availableState;
            availableState.setStandard( player );
            set( availableState );
        }

        void Script::setImpRotting( const bool rotting ) {
            if ( rotting ) {
                init.addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 0 )" );
            } else {
                init.REM( "- prevent imp bodies be taken to graveyard -" );
                init.addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 1 )" );
            }
        }

        void Script::setPrisonConvertLimits() {
            init.addLineIndent( "rem - prevent prison convert snow-balling -" );
            init.addLineIndent( "IF( PLAYER0, SKELETON > 2 )" );
            init.addLineIndent( "    SET_GAME_RULE( PrisonSkeletonChance, 80 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,SKELETON > 5 )" );
            init.addLineIndent( "    SET_GAME_RULE( PrisonSkeletonChance, 60 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,SKELETON > 8 )" );
            init.addLineIndent( "    SET_GAME_RULE( PrisonSkeletonChance, 40 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,GHOST > 3 )" );
            init.addLineIndent( "    SET_GAME_RULE( GhostConvertChance, 30 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
        }

        void Script::setTortureConvertLimits() {
            init.addLineIndent( "rem - prevent torture convert snow-balling -" );
            init.addLineIndent( "IF( PLAYER0, CREATURES_CONVERTED > 2 )" );
            init.addLineIndent( "    SET_GAME_RULE( TortureDeathChance, 33 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, CREATURES_CONVERTED > 5 )" );
            init.addLineIndent( "    SET_GAME_RULE( TortureDeathChance, 50 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,CREATURES_CONVERTED > 8 )" );
            init.addLineIndent( "    SET_GAME_RULE( TortureDeathChance, 80 )" );
            init.ADD_TO_FLAG( Player::P_P0, Flag::F_FLAG_7, 1 );
            init.addLineIndent( "ENDIF" );
        }

        void Script::setStunChance() {
            init.addLineIndent( "rem - modify stun chance -" );
            init.addLineIndent( "IF( PLAYER0,FLAG7 >= 1 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunEvilEnemyChance, 80 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunGoodEnemyChance, 80 )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,FLAG7 >= 2 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunGoodEnemyChance, 60 )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,FLAG7 >= 3 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunEvilEnemyChance, 60 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunGoodEnemyChance, 50 )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0,FLAG7 >= 4 )" );
            init.addLineIndent( "    SET_GAME_RULE( StunGoodEnemyChance, 30 )" );
            init.addLineIndent( "ENDIF" );
        }

        void Script::setSacrificeLimits() {
            init.addLineIndent( "rem - set sacrifice limits -" );
            init.addLineIndent( "IF( PLAYER0, SACRIFICED[IMP] >= 9 )" );
            init.addLineIndent( "    REMOVE_SACRIFICE_RECIPE( IMP )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, REWARDED[BILE_DEMON] >= 1 )" );
            init.addLineIndent( "    REMOVE_SACRIFICE_RECIPE( SPIDER, SPIDER, SPIDER )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, REWARDED[DARK_MISTRESS] >= 1 )" );
            init.addLineIndent( "    REMOVE_SACRIFICE_RECIPE( SPIDER, BUG )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, REWARDED[TENTACLE] >= 1 )" );
            init.addLineIndent( "    REMOVE_SACRIFICE_RECIPE( SPIDER, TROLL )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, REWARDED[HORNY] >= 1 )" );
            init.addLineIndent( "    REMOVE_SACRIFICE_RECIPE( TROLL, BILE_DEMON, DARK_MISTRESS )" );
            init.addLineIndent( "ENDIF" );
        }

        void Script::setGraveyardLimits() {
            init.addLineIndent( "rem - set graveyard limits -" );
            init.addLineIndent( "IF( PLAYER0, VAMPIRE >= 2 )" );
            init.addLineIndent( "    SET_GAME_RULE( BodiesForVampire, 15 )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, VAMPIRE >= 4 )" );
            init.addLineIndent( "    SET_GAME_RULE( BodiesForVampire, 20 )" );
            init.addLineIndent( "ENDIF" );
            init.addLineIndent( "IF( PLAYER0, VAMPIRE >= 6 )" );
            init.addLineIndent( "    SET_GAME_RULE( BodiesForVampire, 25 )" );
            init.addLineIndent( "ENDIF" );
        }

        void Script::setWinConditionStandard( const Player player ) {
            const std::size_t lvl = endConditions.indentLevel();
            endConditions.addLineIndent( std::string() + "IF( " + script_keyword( player ) + ", ALL_DUNGEONS_DESTROYED == 1 )", lvl );
            endConditions.addLineIndent( std::string() + "    WIN_GAME", lvl );
            endConditions.addLineIndent( std::string() + "ENDIF", lvl );
        }

        void Script::setWinConditionKillGood() {
            const std::size_t lvl = endConditions.indentLevel();
            endConditions.addLineIndent( "rem - require killing all heroes -", lvl );
            endConditions.addLineIndent( "IF( PLAYER_GOOD, DUNGEON_DESTROYED == 1 )", lvl );
            endConditions.addLineIndent( "    IF_CONTROLS( PLAYER_GOOD, GOOD_CREATURES == 0 )", lvl );
            endConditions.addLineIndent( "        WIN_GAME", lvl );
            endConditions.addLineIndent( "    ENDIF", lvl );
            endConditions.addLineIndent( "ENDIF", lvl );
        }

        void Script::setLoseConditionStandard( const Player player ) {
            const std::size_t lvl = endConditions.indentLevel();
            endConditions.addLineIndent( std::string() + "IF( " + script_keyword( player ) + ", DUNGEON_DESTROYED == 1 )", lvl );
            endConditions.addLineIndent( std::string() + "    LOSE_GAME", lvl );
            endConditions.addLineIndent( std::string() + "ENDIF", lvl );
        }

        const ScriptCommand& Script::getSection( const ScriptSection section ) const {
            switch( section ) {
            case ScriptSection::SS_HEADER:  return header;
            case ScriptSection::SS_INIT:    return init;
            case ScriptSection::SS_PARTIES: return parties;
            case ScriptSection::SS_MAIN:    return main;
            case ScriptSection::SS_ACTION:  return action;
            case ScriptSection::SS_ENDCOND: return endConditions;
            case ScriptSection::SS_REST:    return other;
            }
            LOG() << "unhandled value: " << (int)section;
            return other;
        }

        ScriptCommand& Script::getSection( const ScriptSection section ) {
            switch( section ) {
            case ScriptSection::SS_HEADER:  return header;
            case ScriptSection::SS_INIT:    return init;
            case ScriptSection::SS_PARTIES: return parties;
            case ScriptSection::SS_MAIN:    return main;
            case ScriptSection::SS_ACTION:  return action;
            case ScriptSection::SS_ENDCOND: return endConditions;
            case ScriptSection::SS_REST:    return other;
            }
            LOG() << "unhandled value: " << (int)section;
            return other;
        }

        static void addSection( std::vector< std::string >& merged, const ScriptCommand& section, const std::string& header ) {
            if ( section.empty() ) {
                return ;
            }
            if ( header.empty() == false ) {
                merged.push_back( "" );
                merged.push_back( "" );
                merged.push_back( header );
            }
            merged.insert( merged.end(), section.begin(), section.end() );
        }

        std::vector< std::string > Script::build() const {
            std::vector< std::string > merged;

            addSection( merged, header, "" );
            addSection( merged, init,          "REM ===============  SETUP  ===============" );
            addSection( merged, parties,       "REM ===========  PARTIES SETUP  ===========" );
            addSection( merged, main,          "REM ===============  MAIN  ================" );
            addSection( merged, action,        "REM ===========  ACTION POINTS  ===========" );
            addSection( merged, other,         "REM ===============  OTHER  ===============" );
            addSection( merged, endConditions, "REM ===========  END CONDITIONS  ==========" );

            return merged;
        }


        /// ==========================================================


        LevelScript::LevelScript( Level& level ): levelPtr(&level) {
        }

        void LevelScript::rebuild( std::vector< std::string >& content ) {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }

            convertToTextArray( content );

            decompose_script( &(level->script), &(level->optns.script) );
            script_decomposed_to_params( &(level->script), &(level->optns.script) );
        }

        /// convert DK_SCRIPT::list to text lines
        bool LevelScript::recompose() {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return false;
            }
            struct DK_SCRIPT *scrpt = get_lvl_script( level );
            recompute_script_levels( scrpt );
            const bool retcode = recompose_script( scrpt, &(level->optns.script) );
            if (retcode)
                LOG() << "Script recomposed successfully";
            else
                LOG() << "Error occured during the update";
            return retcode;
        }

        void LevelScript::execScriptADI( const std::string& path ) {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }
            char msg[ LINEMSG_SIZE ];
            char* msgPtr = &msg[0];
            memset( msgPtr, 0, sizeof(msg) );
            char* fname = (char*) path.c_str();
            script_load_and_execute_file( level, fname, msgPtr );
            LOG() << "message: " << msg;
        }

        bool LevelScript::executeLine( const std::string& line ) {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return false;
            }
            const char* linePtr = line.c_str();
            char msg[ LINEMSG_SIZE ];
            char* msgPtr = &msg[0];
            memset( msgPtr, 0, sizeof(msg) );
            const bool valid = execute_script_line( level, (char*)linePtr, msgPtr );
            if ( valid == false ) {
                LOG() << "message: " << msg;
            }
            return valid;
        }

        void LevelScript::convertToTextArray( const std::vector< std::string >& content ) {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }

            freeTxt();

            const std::size_t lines_count = content.size();
            level->script.txt  = (char **) malloc( lines_count * sizeof(unsigned char *) );
            level->script.list = (struct DK_SCRIPT_COMMAND **) malloc( lines_count * sizeof(struct DK_SCRIPT_COMMAND *) );
            for (std::size_t currline=0; currline<lines_count; ++currline) {
                level->script.list[currline] = NULL;      /* decompose_script() will allocate memory for it */

                const std::string& dataLine = content[ currline ];
                const std::size_t dataSize = dataLine.size();
                level->script.txt[currline] = (char *) malloc( (dataSize+1) * sizeof(char) );
                memcpy( level->script.txt[currline], dataLine.c_str(), dataSize );
                level->script.txt[currline][ dataSize ] = '\0';
            }
            level->script.lines_count = lines_count;
        }

        void LevelScript::freeTxt() {
            LEVEL* level = levelPtr->rawData();
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }

            int idx = level->script.lines_count;
            while (idx > 0) {
              idx--;
              free( level->script.txt[idx] );
              free( level->script.list[idx] );
            }
            free( level->script.txt );
            free( level->script.list );
            level->script.lines_count = 0;
        }

    }
} /* namespace adiktedpp */
