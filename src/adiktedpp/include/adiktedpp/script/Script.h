/*
 * Script.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_

#include "adiktedpp/Level.h"

#include "adiktedpp/script/AvailableState.h"


namespace adiktedpp {
    namespace script {

        enum class Flag {
            F_FLAG_0,
            F_FLAG_1,
            F_FLAG_2,
            F_FLAG_3,
            F_FLAG_4,
            F_FLAG_5,
            F_FLAG_6,
            F_FLAG_7
        };


        enum class Timer {
            T_TIMER_0,
            T_TIMER_1,
            T_TIMER_2,
            T_TIMER_3,
            T_TIMER_4,
            T_TIMER_5,
            T_TIMER_6,
            T_TIMER_7
        };


        enum class IfOption {
            IO_MONEY,
            IO_GAME_TURN,
            IO_HEART_HEALTH,
            IO_TOTAL_DIGGERS,
            IO_TOTAL_CREATURES,
            IO_EVIL_CREATURES,
            IO_GOOD_CREATURES,
            IO_TOTAL_RESEARCH,
            IO_TOTAL_DOORS,
            IO_TOTAL_DOORS_MANUFACTURED,
            IO_TOTAL_DOORS_USED,
            IO_TOTAL_TRAPS_MANUFACTURED,
            IO_TOTAL_TRAPS_USED,
            IO_TOTAL_MANUFACTURED,
            IO_TOTAL_AREA,
            IO_TOTAL_CREATURES_LEFT,
            IO_TOTAL_SALARY,
            IO_CURRENT_SALARY,
            IO_CREATURES_ANNOYED,
            IO_TIMES_ANNOYED_CREATURE,
            IO_TIMES_TORTURED_CREATURE,
            IO_TIMES_LEVELUP_CREATURE,
            IO_BATTLES_WON,
            IO_BATTLES_LOST,
            IO_ROOMS_DESTROYED,
            IO_SPELLS_STOLEN,
            IO_TIMES_BROKEN_INTO,
            IO_DUNGEON_DESTROYED,
            IO_CREATURES_SCAVENGED_LOST,
            IO_CREATURES_SCAVENGED_GAINED,
            IO_CREATURES_SACRIFICED,
            IO_CREATURES_FROM_SACRIFICE,
            IO_CREATURES_CONVERTED,
            IO_ALL_DUNGEONS_DESTROYED,
            IO_KEEPERS_DESTROYED,
            IO_DOORS_DESTROYED,
            IO_TOTAL_GOLD_MINED,
            IO_GOLD_POTS_STOLEN,
            IO_BREAK_IN,
            IO_GHOSTS_RAISED,
            IO_SKELETONS_RAISED,
            IO_VAMPIRES_RAISED
        };


        enum class PartyObjective {
            PO_ATTACK_DUNGEON_HEART,            /// Attack the nearest Dungeon Heart.
            PO_ATTACK_ENEMIES,                  /// Attack any enemies.
            PO_ATTACK_ROOMS,                    /// Attack the nearest rooms.
            PO_DEFEND_HEART,                    /// Go to own heart and defend it until destroyed.
            PO_DEFEND_LOCATION,                 /// Never do any objective.
            PO_DEFEND_PARTY,                    /// Will not do any objective so will not assume party leadership.
            PO_DEFEND_ROOMS,                    /// Defend nearest owned room, not counting portals, hearts or bridges.
            PO_STEAL_GOLD,                      /// Steal gold from the Treasure Room until unit carries 1000 gold.
            PO_STEAL_SPELLS                     /// Go to Library and steal one spell book.
        };


        /// ==================================================================


        std::string script_keyword( const Player data );
        std::string script_keyword( const Slab data );
        std::string script_keyword( const Room data );
        std::string script_keyword( const Creature data );
        std::string script_keyword( const Door data );
        std::string script_keyword( const Trap data );
        std::string script_keyword( const Spell data );
        std::string script_keyword( const Flag data );
        std::string script_keyword( const Timer data );
        std::string script_keyword( const IfOption data );
        std::string script_keyword( const PartyObjective data );


        /// ==================================================================


        /**
         *
         */
        class ScriptContainer {
        protected:

            std::vector< std::string > lines;
            std::size_t level;                      /// indent level


        public:

            ScriptContainer(): level(0) {
            }

            const std::vector< std::string >& data() const {
                return lines;
            }

            void clear() {
                lines.clear();
            }

            bool empty() const {
                return lines.empty();
            }

            std::size_t indentLevel() const {
                return level;
            }

            void indentLevel( const std::size_t newIndent ) {
                level = newIndent;
            }

            std::vector< std::string >::const_iterator begin() const {
                return lines.begin();
            }

            std::vector< std::string >::const_iterator end() const {
                return lines.end();
            }

            void pushFront( const ScriptContainer& script ) {
                lines.insert( lines.begin(), script.begin(), script.end() );
            }

            void pushBack( const ScriptContainer& script ) {
                lines.insert( lines.end(), script.begin(), script.end() );
            }

            void addEmptyLine( const std::size_t number = 1 );

            void addLine( const std::string& line );

            void addLineIndent( const std::string& line, const std::size_t forceIndent = 0 );

            std::size_t countTunnellerTriggers() const;
            std::size_t countPartyTriggers() const;
            std::size_t countScriptValues() const;
            std::size_t countIfConditions() const;
            std::size_t countPartyDefinitions() const;


            static std::size_t countScriptValues( const std::vector< std::string >& lines );


        protected:

            void updateLevelIf( const std::string& line );

            void updateLevelEndIf( const std::string& line );

        };


        /**
         *
         */
        class ScriptCommand: public ScriptContainer {
        public:

//            ScriptCommand(): ScriptContainer() {
//            }

            void REM( const std::string& comment ) {
                return addLine( "rem " + comment );
            }

            void LEVEL_VERSION( const int version = 1 );

            /// flag is in range [0..255]
            /// need to be reused explicit
            void SET_FLAG( const adiktedpp::Player player, const Flag flag, const int value=0 );

            /// flag is in range [0..255]
            /// need to be reused explicit
            void ADD_TO_FLAG( const adiktedpp::Player player, const Flag flag, const int value=1 );

            /// randomizes flag value in range [1..maxValue]
            void RANDOMISE_FLAG( const adiktedpp::Player player, const Flag flag, const int maxValue );

            void SET_TIMER( const adiktedpp::Player player, const Timer timer );

            void QUICK_OBJECTIVE( const std::size_t infoIndex, const std::string& message );

            void QUICK_INFORMATION( const std::size_t infoIndex, const std::string& message );

            void PLAY_MESSAGE( const adiktedpp::Player player, const std::size_t speechId );

            void START_MONEY( const adiktedpp::Player player, const std::size_t amount );

            /// value in range [0..10]
            /// note: the official manual claims that numbers for computer players other than 0, 1, 3, 4, 5, or 10 will not make the game work properly
            /// further details: https://lubiki.keeperklan.com/dk1_docs/dk_scripting_ref.htm#scrpt_cmd_computer_player
            void COMPUTER_PLAYER( const Player player, const std::size_t attitude );

            void MAX_CREATURES( const Player player, const std::size_t limit );

            void CONCEAL_MAP_RECT( const Player player, const utils::Rect rect, const bool hideAll = false );

            void CHANGE_SLAB_TYPE( const utils::Point position, const Slab slab );

            void IF_flag( const adiktedpp::Player player, const Flag flag, const std::string& comparison, const int value );

            void IF_timer( const adiktedpp::Player player, const Timer timer, const std::string& comparison, const int value );

            void IF_option( const adiktedpp::Player player, const IfOption option, const std::string& comparison, const int value );

            void IF_SLAB_TYPE( const utils::Point position, const Slab slab );

            void IF_SLAB_OWNER( const utils::Point position, const adiktedpp::Player player );

            void ENDIF();

            /// signed, hero gates has negative numbers
            void IF_ACTION_POINT( const int actionNumber, const adiktedpp::Player player );

            /// 'crExp' in range [1..10]
            void ADD_CREATURE_TO_LEVEL( const Player player, const Creature creature, const int actionPoint, const std::size_t crNum, const std::size_t crExp, const std::size_t gold );

            void CREATE_PARTY( const std::string& partyName );

            void ADD_TO_PARTY( const std::string& partyName, const Creature creature, const std::size_t crNum, const std::size_t crExp, const std::size_t gold, const PartyObjective objective, const std::size_t countdown = 0 );

            /// 'copies' -- how many copies of party will be placed at given action point?
            void ADD_PARTY_TO_LEVEL( const Player player, const std::string& partyName, const int actionPoint, const std::size_t copies = 1 );

            /// 'enemyNumber' in range [0..4]. 0 is for red player.
            void ADD_TUNNELLER_PARTY_TO_LEVEL( const Player player, const std::string& partyName, const int actionPoint, const std::size_t enemyNumber, const std::size_t tunnelerExp, const std::size_t tunnelerGold );

            void NEXT_COMMAND_REUSABLE();
        };



        /// ================================================================================


        enum class ScriptSection {
            SS_HEADER,
            SS_INIT,
            SS_FLAGS,
            SS_PARTIES,
            SS_MAIN,
            SS_ACTION,
            SS_ENDCOND,
            SS_REST
        };

        const std::set< ScriptSection >& AllScriptSections();


        /**
         *
         */
        class Script {

            ScriptCommand header;
            ScriptCommand init;
            ScriptCommand flags;
            ScriptCommand parties;
            ScriptCommand main;
            ScriptCommand action;
            ScriptCommand other;
            ScriptCommand endConditions;

            bool isFX;
            std::size_t objectiveIndex;
            std::size_t infoIndex;


        public:

            Script();

            ScriptCommand& operator[]( const ScriptSection section ) {
                return getSection( section );
            }

            ScriptCommand& headerSection() {
                return header;
            }
            ScriptCommand& initSection() {
                return init;
            }
            ScriptCommand& flagsSection() {
                return flags;
            }
            ScriptCommand& partiesSection() {
                return parties;
            }
            ScriptCommand& mainSection() {
                return main;
            }
            ScriptCommand& actionSection() {
                return action;
            }
            ScriptCommand& otherSection() {
                return other;
            }

            void clearData();

            std::size_t nextObjectiveIndex() {
                return ++objectiveIndex;
            }

            std::size_t nextInfoIndex() {
                return ++infoIndex;
            }

//            void copyIndexes( const Script& script );

            void pushFrontBySections( const Script& script );

            std::vector< std::string > build() const;

            void setHeaderInfo();

            void storeParameters( const std::string& mapType, const std::string& seed );

            /// limit: 16
            std::size_t countTunnellerTriggers() const;

            /// limit: 48
            std::size_t countPartyTriggers() const;

            /// limit of script commands (almost every) inside IF body allowed by game
            /// limit: 64
            std::size_t countScriptValues() const;

            /// limit: 48
            std::size_t countIfConditions() const;

            /// limit: 16
            std::size_t countPartyDefinitions() const;

            void addLine( const std::string& line ) {
                other.addLine( line );
            }

            void addLine( const ScriptSection section, const std::string& line ) {
                ScriptCommand& script = getSection( section );
                script.addLine( line );
            }

            void addLineInit( const std::string& line ) {
                init.addLine( line );
            }

            void addLineAction( const std::string& line ) {
                action.addLine( line );
            }

            bool isFXLevel() const {
                return isFX;
            }

            void setFXLevel() {
                isFX = true;
                init.REM( "- set script compatibility with Keeper FX -" );
                init.LEVEL_VERSION( 1 );
            }

            /// hide whole map under fog of war (rocks, gems and other visible objects included)
            void concealWholeMap( const Player player );

            /// set creature pool
            void setEvilCreaturesPool( const std::size_t number );

            void setHeroCreaturesPool( const std::size_t number );

            void setCreaturePool( const Creature creature, const std::size_t number );

            /// ================================================================================

            void set( const RoomsAvailableState& stateMap );
            void set( const CreatureAvailableState& stateMap );
            void set( const DoorAvailableState& stateMap );
            void set( const TrapAvailableState& stateMap );
            void set( const MagicAvailableState& stateMap );

            /// available -- 0 or 1 -- if 1 then creatures appear from the portal
            void setCreatureAvailable( const adiktedpp::Player player, const Creature item, const bool available, const int numberForced );

            /// set available creatures from pool
            void setEvilCreaturesAvailable( const Player player, const bool available = true );

            /// set available creatures from pool
            void setHeroCreaturesAvailable( const Player player, const bool available = true );

            void setRoomAvailable( const Player player, const Room room, const RoomAvailableMode mode = RoomAvailableMode::RAM_DISABLED );

            void setRoomsAvailable( const Player player, const RoomAvailableMode mode = RoomAvailableMode::RAM_DISABLED );

            void setRoomsStandard();

            /// constructable -- 0 or 1 -- if 1 then door can be constructed
            void setDoorAvailable( const adiktedpp::Player player, const Door item, const bool constructable, const int numberAvailable );

            /**
             * "numberAvailable" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setDoorsAvailable( const Player player, const int numberAvailable = -1 );

            /// constructable -- 0 or 1 -- if 1 then door can be constructed
            void setTrapAvailable( const adiktedpp::Player player, const Trap item, const bool constructable, const int numberAvailable );

            /**
             * "numberAvailable" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setTrapsAvailable( const Player player, const int numberAvailable = -1 );

            void setMagicAvailable( const adiktedpp::Player player, const Spell item, const MagicAvailableMode mode );

            void setMagicAvailable( const Player player, const MagicAvailableMode mode = MagicAvailableMode::AM_DISABLED );

            void setMagicStandard( const Player player );

            /// ================================================================================

            /// disable or enable imps corpses be brought to graveyard
            /// not supported by adikted
            void setImpRotting( const bool rotting = true );

            void setPrisonConvertLimits();

            void setTortureConvertLimits();

            void setStunChance();

            void setSacrificeLimits();

            void setGraveyardLimits();

            void setWinConditionStandard( const Player player );

            void setWinConditionKillGood();

            void setLoseConditionStandard( const Player player );


        private:

            const ScriptCommand& getSection( const ScriptSection section ) const;

            ScriptCommand& getSection( const ScriptSection section );

        };


        /**
         * Wrapper class for script related functions of adikted lib.
         */
        class LevelScript {

            Level* levelPtr;


        public:

            LevelScript( Level& level );

            void rebuild( const script::Script& scriptData );

            void rebuild( std::vector< std::string >& content );

            bool recompose();

            /**
             * Execute script function against level.
             * Function name and arguments are given in string format, e.g. 'LEVEL_AUTHORS("dkmage","dkmage")'.
             * Currently there is possible to execute functions from adikted specific group (CMD_ADIKTED).
             */
            bool executeLine( const std::string& line );

            /// execute commands stored in ADI file
            void execScriptADI( const std::string& path );


        protected:

            void convertToTextArray( const std::vector< std::string >& content );

            void freeTxt();

        };

    }
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_ */
