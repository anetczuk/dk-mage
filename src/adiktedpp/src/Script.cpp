/*
 * Script.cpp
 *
 */

#include "adiktedpp/Script.h"

#include "adiktedpp/Messages.h"

#include "utils/Log.h"

extern "C" {
    #include "libadikted/lev_files.h"
    #include "libadikted/lev_script.h"
    #include "libadikted/lev_data.h"
}


namespace adiktedpp {

    std::string scriptName( const PlayerType item ) {
        switch( item ) {
        case PlayerType::PT_0:      return "PLAYER0";
        case PlayerType::PT_1:      return "PLAYER1";
        case PlayerType::PT_2:      return "PLAYER2";
        case PlayerType::PT_3:      return "PLAYER3";
        case PlayerType::PT_GOOD:   return "PLAYER_GOOD";
        case PlayerType::PT_UNSET:  return "PLAYER_UNSET";          /// unavailable in script
        case PlayerType::PT_ALL:    return "ALL_PLAYERS";
        }
        return "UNKNOWN_CREATURE";
    }

    std::string scriptName( const SlabType item ) {
        switch( item ) {
        case SlabType::ST_TREASURE:     return "TREASURE";
        case SlabType::ST_LIBRARY:      return "RESEARCH";
        case SlabType::ST_PRISONCASE:   return "PRISON";
        case SlabType::ST_TORTURE:      return "TORTURE";
        case SlabType::ST_TRAINING:     return "TRAINING";
        case SlabType::ST_WORKSHOP:     return "WORKSHOP";
        case SlabType::ST_SCAVENGER:    return "SCAVENGER";
        case SlabType::ST_TEMPLE:       return "TEMPLE";
        case SlabType::ST_GRAVEYARD:    return "GRAVEYARD";
        case SlabType::ST_HATCHERY:     return "BARRACKS";
        case SlabType::ST_LAIR:         return "GARDEN";
        case SlabType::ST_BARRACKS:     return "LAIR";
        case SlabType::ST_BRIDGE:       return "BRIDGE";
        case SlabType::ST_GUARDPOST:    return "GUARD_POST";

        case SlabType::ST_DOORWOOD1:     return "WOOD";
        case SlabType::ST_DOORWOOD2:     return "WOOD";
        case SlabType::ST_DOORBRACE1:    return "BRACED";
        case SlabType::ST_DOORBRACE2:    return "BRACED";
        case SlabType::ST_DOORIRON1:     return "STEEL";
        case SlabType::ST_DOORIRON2:     return "STEEL";
        case SlabType::ST_DOORMAGIC1:    return "MAGIC";
        case SlabType::ST_DOORMAGIC2:    return "MAGIC";

        default:                        return "UNKNOWN_SLAB";
        }
    }

    std::string scriptName( const SubTypeItem item ) {
        switch( item ) {
        case SubTypeItem::STI_SPELLIMP:       return "POWER_IMP";
        case SubTypeItem::STI_SPELLMUST:      return "POWER_OBEY";
        case SubTypeItem::STI_SPELLSOE:       return "POWER_SIGHT";
        case SubTypeItem::STI_SPELLCTA:       return "POWER_CALL_TO_ARMS";
        case SubTypeItem::STI_SPELLCAVI:      return "POWER_CAVE_IN";
        case SubTypeItem::STI_SPELLHEAL:      return "POWER_HEAL_CREATURE";
        case SubTypeItem::STI_SPELLHLDA:      return "POWER_HOLD_AUDIENCE";
        case SubTypeItem::STI_SPELLLIGH:      return "POWER_LIGHTNING";
        case SubTypeItem::STI_SPELLSPDC:      return "POWER_SPEED";
        case SubTypeItem::STI_SPELLPROT:      return "POWER_PROTECT";
        case SubTypeItem::STI_SPELLCONC:      return "POWER_CONCEAL";
        case SubTypeItem::STI_SPELLDISE:      return "POWER_DISEASE";
        case SubTypeItem::STI_SPELLCHKN:      return "POWER_CHICKEN";
        case SubTypeItem::STI_SPELLDWAL:      return "POWER_DESTROY_WALLS";
        case SubTypeItem::STI_SPELLARMG:      return "POWER_ARMAGEDDON";

        case SubTypeItem::STI_SPELLHOE:     return "POWER_POSSESS";
        case SubTypeItem::STI_PWHANDWHP:    return "POWER_SLAP";
        case SubTypeItem::STI_PWHANDGRB:    return "POWER_HAND";

        case SubTypeItem::STI_TBBOULDER:      return "BOULDER";
        case SubTypeItem::STI_TBALARM:        return "ALARM";
        case SubTypeItem::STI_TBPOISONG:      return "POISON_GAS";
        case SubTypeItem::STI_TBLIGHTNG:      return "LIGHTNING";
        case SubTypeItem::STI_TBWRDOFPW:      return "WORD_OF_POWER";
        case SubTypeItem::STI_TBLAVA:         return "LAVA";

        default:                              return "UNKNOWN_ITEM";
        }
    }

    std::string scriptName( const SubTypeCreature item ) {
        switch( item ) {
        case SubTypeCreature::STC_WIZRD:    return "WIZARD";
        case SubTypeCreature::STC_BARBARIN: return "BARBARIAN";
        case SubTypeCreature::STC_ARCHER:   return "ARCHER";
        case SubTypeCreature::STC_MONK:     return "MONK";
        case SubTypeCreature::STC_DWAFT:    return "DWARFA";
        case SubTypeCreature::STC_KNIGHT:   return "KNIGHT";
        case SubTypeCreature::STC_AVATAR:   return "AVATAR";
        case SubTypeCreature::STC_TUNELER:  return "TUNNELLER";
        case SubTypeCreature::STC_WITCH:    return "WITCH";
        case SubTypeCreature::STC_GIANT:    return "GIANT";
        case SubTypeCreature::STC_FAIRY:    return "FAIRY";
        case SubTypeCreature::STC_THEFT:    return "THIEF";
        case SubTypeCreature::STC_SMURI:    return "SAMURAI";

        case SubTypeCreature::STC_HORNY:    return "HORNY";
        case SubTypeCreature::STC_SKELETON: return "SKELETON";
        case SubTypeCreature::STC_TROLL:    return "TROLL";
        case SubTypeCreature::STC_DRAGON:   return "DRAGON";
        case SubTypeCreature::STC_SPAWN:    return "DEMONSPAWN";
        case SubTypeCreature::STC_FLY:      return "FLY";
        case SubTypeCreature::STC_MISTRESS: return "DARK_MISTRESS";
        case SubTypeCreature::STC_WARLOCK:  return "SORCEROR";
        case SubTypeCreature::STC_BILEDEMN: return "BILE_DEMON";
        case SubTypeCreature::STC_IMP:      return "IMP";
        case SubTypeCreature::STC_BEETLE:   return "BUG";
        case SubTypeCreature::STC_VAMPIRE:  return "VAMPIRE";
        case SubTypeCreature::STC_SPIDER:   return "SPIDER";
        case SubTypeCreature::STC_HOUND:    return "HELL_HOUND";
        case SubTypeCreature::STC_GHOST:    return "GHOST";
        case SubTypeCreature::STC_TENTCL:   return "TENTACLE";
        case SubTypeCreature::STC_ORC:      return "ORC";
        case SubTypeCreature::STC_FLOAT:    return "FLOATING_SPIRIT";
        }
        return "UNKNOWN_CREATURE";
    }


    Script::Script( Level& level ): levelPtr(&level) {
        addLine( "REM *** script generated by dkmage -- DK map generator v. 1.0.1 ***" );
        addLine( "" );
    }

    void Script::rebuild() {
        LEVEL* level = levelPtr->rawData();
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return ;
        }

        convertToTextArray();
        decompose_script( &(level->script), &(level->optns.script) );
        script_decomposed_to_params( &(level->script), &(level->optns.script) );

//        recompose();
    }

    /// convert DK_SCRIPT::list to text lines
    bool Script::recompose() {
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

    void Script::execScriptADI( const std::string& path ) {
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

    bool Script::executeLine( const std::string& line ) {
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

    bool Script::addLine( const std::string& line ) {
        lines.push_back( line );
        return true;
    }

    bool Script::addLine( const std::string& line, const std::size_t position ) {
        if ( position >= lines.size() ) {
            lines.push_back( line );
            return true;
        }
        auto pos = lines.begin();
        std::advance( pos, position );
        lines.insert( pos, line );
        return true;
    }

    void Script::setEvilCreaturesPool( const std::size_t number ) {
        std::set< SubTypeCreature > list = EvilCreatures();
        list.erase( SubTypeCreature::STC_HORNY );
        list.erase( SubTypeCreature::STC_FLOAT );
        list.erase( SubTypeCreature::STC_IMP );
        list.erase( SubTypeCreature::STC_TENTCL );
        list.erase( SubTypeCreature::STC_VAMPIRE );
        list.erase( SubTypeCreature::STC_SKELETON );
        for ( const SubTypeCreature item: list ) {
            std::stringstream stream;
            stream << "ADD_CREATURE_TO_POOL( " << scriptName( item ) << ", " << number << " )";
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::setEvilCreaturesAvailable( const PlayerType player, const AvailableMode mode ) {
        std::set< SubTypeCreature > list = EvilCreatures();
        list.erase( SubTypeCreature::STC_HORNY );
        list.erase( SubTypeCreature::STC_FLOAT );
        list.erase( SubTypeCreature::STC_IMP );
        list.erase( SubTypeCreature::STC_TENTCL );
        list.erase( SubTypeCreature::STC_VAMPIRE );
        list.erase( SubTypeCreature::STC_SKELETON );
        for ( const SubTypeCreature item: list ) {
            std::stringstream stream;
            switch( mode ) {
            case AvailableMode::AM_DISABLED: {
                stream << "CREATURE_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, 0 )";
                break ;
            }
            case AvailableMode::AM_POSSIBLE: {
                stream << "CREATURE_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 0 )";
                break ;
            }
            case AvailableMode::AM_ENABLED: {
                stream << "CREATURE_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 1 )";
                break ;
            }
            }
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::setRoomsAvailable( const PlayerType player, const AvailableMode mode ) {
        std::set< SlabType > list = Rooms();
        for ( const SlabType item: list ) {
            std::stringstream stream;
            switch( mode ) {
            case AvailableMode::AM_DISABLED: {
                stream << "ROOM_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, 0 )";
                break ;
            }
            case AvailableMode::AM_POSSIBLE: {
                stream << "ROOM_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 0 )";
                break ;
            }
            case AvailableMode::AM_ENABLED: {
                stream << "ROOM_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 1 )";
                break ;
            }
            }
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::setDoorsAvailable( const PlayerType player, const int available ) {
        std::set< SlabType > list = Doors();
        for ( const SlabType item: list ) {
            std::stringstream stream;
            if ( available < 0 ) {
                stream << "DOOR_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, 0 )";
            } else if ( available == 0 ) {
                stream << "DOOR_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 0 )";
            } else {
                stream << "DOOR_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, " << available << " )";
            }
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::setTrapsAvailable( const PlayerType player, const int available ) {
        std::set< SubTypeItem > list = Traps();
        for ( const SubTypeItem item: list ) {
            std::stringstream stream;
            if ( available < 0 ) {
                stream << "TRAP_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, 0 )";
            } else if ( available == 0 ) {
                stream << "TRAP_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 0 )";
            } else {
                stream << "TRAP_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, " << available << " )";
            }
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::setMagicAvailable( const PlayerType player, const AvailableMode mode ) {
        std::set< SubTypeItem > list = Spells();
        for ( const SubTypeItem item: list ) {
            std::stringstream stream;
            switch( mode ) {
            case AvailableMode::AM_DISABLED: {
                stream << "MAGIC_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 0, 0 )";
                break ;
            }
            case AvailableMode::AM_POSSIBLE: {
                stream << "MAGIC_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 0 )";
                break ;
            }
            case AvailableMode::AM_ENABLED: {
                stream << "MAGIC_AVAILABLE( " << scriptName( player ) << ", " << scriptName( item ) << ", 1, 1 )";
                break ;
            }
            }
            const std::string& line = stream.str();
            addLine( line );
        }
    }

    void Script::convertToTextArray() {
        LEVEL* level = levelPtr->rawData();
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return ;
        }

        freeTxt();

        const std::size_t lines_count = lines.size();
        level->script.txt  = (char **) malloc( lines_count * sizeof(unsigned char *) );
        level->script.list = (struct DK_SCRIPT_COMMAND **) malloc( lines_count * sizeof(struct DK_SCRIPT_COMMAND *) );
        for (std::size_t currline=0; currline<lines_count; ++currline) {
            level->script.list[currline] = NULL;      /* decompose_script() will allocate memory for it */

            const std::string& dataLine = lines[ currline ];
            const std::size_t dataSize = dataLine.size();
            level->script.txt[currline] = (char *) malloc( (dataSize+1) * sizeof(char) );
            memcpy( level->script.txt[currline], dataLine.c_str(), dataSize );
            level->script.txt[currline][ dataSize ] = '\0';
        }
        level->script.lines_count = lines_count;
    }

    void Script::freeTxt() {
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

} /* namespace adiktedpp */
