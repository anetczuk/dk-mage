/*
 * Script.cpp
 *
 */

#include "adiktedpp/script/Script.h"

#include "adiktedpp/Messages.h"
#include "adiktedpp/Version.h"

#include "utils/Log.h"

extern "C" {
    #include "libadikted/lev_files.h"
    #include "libadikted/lev_script.h"
    #include "libadikted/lev_data.h"
}


namespace adiktedpp {
    namespace script {

        std::string scriptName( const Player item ) {
            switch( item ) {
            case Player::P_P0:      return "PLAYER0";
            case Player::P_P1:      return "PLAYER1";
            case Player::P_P2:      return "PLAYER2";
            case Player::P_P3:      return "PLAYER3";
            case Player::P_GOOD:    return "PLAYER_GOOD";
            case Player::P_UNSET:   return "PLAYER_UNSET";          /// unavailable in script
            case Player::P_ALL:     return "ALL_PLAYERS";
            }
            return "UNKNOWN_PLAYER";
        }


        std::string scriptName( const Room data ) {
            switch( data ) {
            case Room::R_CLAIMED:        { return "UNSUPPORTED_CLAIMED"; }
            case Room::R_DUNGEON_HEART:  { return "DUNGEON_HEART"; }
            case Room::R_PORTAL:         { return "UNSUPPORTED_PORTAL"; }

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

            LOG() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string scriptName( const Creature data ) {
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

            LOG() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string scriptName( const Door data ) {
            switch( data ) {
            case Door::D_WOOD:           { return "WOOD"; }
            case Door::D_BRACED:         { return "BRACED"; }
            case Door::D_IRON:           { return "STEEL"; }
            case Door::D_MAGIC:          { return "MAGIC"; }
            }

            LOG() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string scriptName( const Trap data ) {
            switch( data ) {
            case Trap::T_BOULDER:        { return "BOULDER"; }
            case Trap::T_ALARM:          { return "ALARM"; }
            case Trap::T_POISON_GAS:     { return "POISON_GAS"; }
            case Trap::T_LIGHTNING:      { return "LIGHTNING"; }
            case Trap::T_WORD_OF_POWER:  { return "WORD_OF_POWER"; }
            case Trap::T_LAVA:           { return "LAVA"; }
            }

            LOG() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        std::string scriptName( const Spell data ) {
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

            LOG() << "invalid argument: " << (int)data;
            std::stringstream stream;
            stream << FILE_NAME << ": invalid argument: " << (int)data;
            throw std::invalid_argument( stream.str() );
        }


        /// =========================================================================


        void BasicScript::addLine( const std::string& line, const std::size_t position ) {
            if ( position >= lines.size() ) {
                lines.push_back( line );
                return ;
            }
            auto pos = lines.begin();
            std::advance( pos, position );
            lines.insert( pos, line );
        }

        void BasicScript::setFXLevel() {
            addREM( "*** set script compatibility with Keeper FX ***" );
            addLine( "LEVEL_VERSION(1)" );
        }

        void BasicScript::setStartMoney( const adiktedpp::Player player, const std::size_t amount ) {
            std::stringstream stream;
            stream << "START_MONEY( " << scriptName( player ) << ", " << amount << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::addAvailable( const Player player, const Room item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_ROOM_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::addAvailable( const Player player, const Creature item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_CREATURE_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::addAvailable( const Player player, const Door item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_DOOR_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::addAvailable( const Player player, const Trap item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_TRAP_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::addAvailable( const Player player, const Spell item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_MAGIC_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::setEvilCreaturesPool( const std::size_t number ) {
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

        void BasicScript::setHeroCreaturesPool( const std::size_t number ) {
            std::set< Creature > list = HeroCreatures();
            list.erase( Creature::C_AVATAR );
            for ( const Creature item: list ) {
                setCreaturePool( item, number );
            }
        }

        void BasicScript::setCreaturePool( const Creature creature, const std::size_t number ) {
            std::stringstream stream;
            stream << "ADD_CREATURE_TO_POOL( " << scriptName( creature ) << ", " << number << " )";
            const std::string& line = stream.str();
            addLine( line );
        }

        void BasicScript::setImpRotting( const bool rotting ) {
            if ( rotting ) {
                addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 1 )" );
            } else {
                addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 0 )" );
            }
        }

        void BasicScript::setWinConditionStandard( const Player player ) {
            addLine( std::string() + "IF( " + scriptName( player ) + ", ALL_DUNGEONS_DESTROYED == 1 )" );
            addLine( std::string() + "    WIN_GAME" );
            addLine( std::string() + "ENDIF" );
        }

        void BasicScript::setWinConditionKillGood() {
            addREM( "require killing all heroes" );
            addLine( std::string() + "IF( PLAYER_GOOD, DUNGEON_DESTROYED == 1 )" );
            addLine( std::string() + "    IF_CONTROLS( PLAYER_GOOD, GOOD_CREATURES == 0 )" );
            addLine( std::string() + "        WIN_GAME" );
            addLine( std::string() + "    ENDIF" );
            addLine( std::string() + "ENDIF" );
        }

        void BasicScript::setLoseConditionStandard( const Player player ) {
            addLine( std::string() + "IF( " + scriptName( player ) + ", DUNGEON_DESTROYED == 1 )" );
            addLine( std::string() + "    LOSE_GAME" );
            addLine( std::string() + "ENDIF" );
        }


        /// =========================================================================


        Script::Script() {
            header.addREM( "*** script generated by dk-mage ver. " + VERSION_FULL_STRING + " ***" );
        }

        void Script::storeParameters( const std::string& mapType, const std::string& seed ) {
            header.addREM( "*** generated map type: '" + mapType + "' ***" );
            header.addREM( "*** seed used for generation: '" + seed + "' ***" );
        }

        void Script::addAvailable( const Player player, const Room item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_ROOM_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Creature item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_CREATURE_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Door item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_DOOR_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Trap item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_TRAP_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
        }

        void Script::addAvailable( const Player player, const Spell item, const int accessible, const int available ) {
            std::stringstream stream;
            stream << AvailableCommand::SAC_MAGIC_AVAILABLE << "( " << scriptName( player ) << ", " << scriptName( item ) << ", " << accessible << ", " << available << " )";
            const std::string& line = stream.str();
            init.addLine( line );
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
            stream << "ADD_CREATURE_TO_POOL( " << scriptName( creature ) << ", " << number << " )";
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
            availableState.setAllAvailable( player, available );
            set( availableState );
        }

        void Script::setTrapsAvailable( const Player player, const int available ) {
            TrapAvailableState availableState;
            availableState.setAllAvailable( player, available );
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
                init.addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 1 )" );
            } else {
                init.addLine( "SET_CREATURE_PROPERTY( IMP, NO_CORPSE_ROTTING, 0 )" );
            }
        }

        void Script::setWinConditionStandard( const Player player ) {
            endConditions.addLine( std::string() + "IF( " + scriptName( player ) + ", ALL_DUNGEONS_DESTROYED == 1 )" );
            endConditions.addLine( std::string() + "    WIN_GAME" );
            endConditions.addLine( std::string() + "ENDIF" );
        }

        void Script::setWinConditionKillGood() {
            endConditions.addREM( "require killing all heroes" );
            endConditions.addLine( "IF( PLAYER_GOOD, DUNGEON_DESTROYED == 1 )" );
            endConditions.addLine( "    IF_CONTROLS( PLAYER_GOOD, GOOD_CREATURES == 0 )" );
            endConditions.addLine( "        WIN_GAME" );
            endConditions.addLine( "    ENDIF" );
            endConditions.addLine( "ENDIF" );
        }

        void Script::setLoseConditionStandard( const Player player ) {
            endConditions.addLine( std::string() + "IF( " + scriptName( player ) + ", DUNGEON_DESTROYED == 1 )" );
            endConditions.addLine( std::string() + "    LOSE_GAME" );
            endConditions.addLine( std::string() + "ENDIF" );
        }

        BasicScript& Script::getSection( const ScriptSection section ) {
            switch( section ) {
            case ScriptSection::SS_HEADER:  return header;
            case ScriptSection::SS_INIT:    return init;
            case ScriptSection::SS_ACTION:  return action;
            case ScriptSection::SS_ENDCOND: return endConditions;
            case ScriptSection::SS_REST:    return other;
            }
            LOG() << "unhandled value: " << (int)section;
            return other;
        }

        std::vector< std::string > Script::build() {
            std::vector< std::string > merged;

            if ( header.empty() == false ) {
                merged.insert( merged.end(), header.begin(), header.end() );
            }
            if ( init.empty() == false ) {
                merged.push_back( "" );
                merged.push_back( "" );
                merged.push_back( "REM --- initialization ---" );
                merged.push_back( "" );
                merged.insert( merged.end(), init.begin(), init.end() );
            }
            if ( action.empty() == false ) {
                merged.push_back( "" );
                merged.push_back( "" );
                merged.push_back( "REM --- action points ---" );
                merged.push_back( "" );
                merged.insert( merged.end(), action.begin(), action.end() );
            }
            if ( endConditions.empty() == false ) {
                merged.push_back( "" );
                merged.push_back( "" );
                merged.push_back( "REM --- win conditions ---" );
                merged.push_back( "" );
                merged.insert( merged.end(), endConditions.begin(), endConditions.end() );
            }
            if ( other.empty() == false ) {
                merged.push_back( "" );
                merged.push_back( "" );
                merged.push_back( "REM --- other ---" );
                merged.push_back( "" );
                merged.insert( merged.end(), other.begin(), other.end() );
            }

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
