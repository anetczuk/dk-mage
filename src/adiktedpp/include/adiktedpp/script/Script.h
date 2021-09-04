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


        /**
         *
         */
        class ScriptContainer {

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

            std::vector< std::string >::const_iterator begin() const {
                return lines.begin();
            }

            std::vector< std::string >::const_iterator end() const {
                return lines.end();
            }

            void addEmptyLine( const std::size_t number = 1 );

            void addLine( const std::string& line );

            void addLineIndent( const std::string& line, const std::size_t forceIndent = 0 );

//            void addLine( const std::string& line, const std::size_t position );


        protected:

            void countIf( const std::string& line );

            void countEndIf( const std::string& line );

        };


        /**
         *
         */
        class ScriptCommand: public ScriptContainer {
        public:

            void REM( const std::string& comment ) {
                return addLine( "REM " + comment );
            }

            /// flag is in range [0..255]
            void ADD_TO_FLAG( const adiktedpp::Player player, const Flag flag, const int value=1 );

            void QUICK_INFORMATION( const std::size_t infoIndex, const std::string& comment );

            void START_MONEY( const adiktedpp::Player player, const std::size_t amount );

            /// value in range [0..10]
            /// note: the official manual claims that numbers for computer players other than 0, 1, 3, 4, 5, or 10 will not make the game work properly
            /// further details: https://lubiki.keeperklan.com/dk1_docs/dk_scripting_ref.htm#scrpt_cmd_computer_player
            void COMPUTER_PLAYER( const Player player, const std::size_t attitude );

            void MAX_CREATURES( const Player player, const std::size_t limit );

            void CONCEAL_MAP_RECT( const Player player, const utils::Rect rect, const bool hideAll = false );

            void CHANGE_SLAB_TYPE( const utils::Point position, const Slab slab );

            void IF( const adiktedpp::Player player, const Flag flag, const std::string& comparison, const int value );

            void IF( const adiktedpp::Player player, const IfOption option, const std::string& comparison, const int value );

            void IF_SLAB_TYPE( const utils::Point position, const Slab slab );

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

        };


        /**
         *
         */
        class BasicScript: public ScriptCommand {
        public:

            void setFXLevel();

            void addAvailable( const adiktedpp::Player player, const Room item );

            void addAvailable( const adiktedpp::Player player, const Room item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Creature item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Door item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Trap item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Spell item, const int accessible, const int available );

            /// set creature pool
            void setEvilCreaturesPool( const std::size_t number );

            void setHeroCreaturesPool( const std::size_t number );

            void setCreaturePool( const Creature creature, const std::size_t number );

            /// ================================================================================

            /// disable or enable imps corpses be brought to graveyard
            /// not supported by adikted
            void setImpRotting( const bool rotting = true );

            void setWinConditionStandard( const Player player );

            void setWinConditionKillGood();

            void setLoseConditionStandard( const Player player );

        };


        /// ================================================================================


        enum class ScriptSection {
            SS_HEADER,
            SS_INIT,
            SS_MAIN,
            SS_ACTION,
            SS_ENDCOND,
            SS_REST
        };


        /**
         *
         */
        class Script {

            BasicScript header;
            BasicScript init;
            BasicScript main;
            BasicScript action;
            BasicScript other;
            BasicScript endConditions;

            bool isFX;


        public:

            Script();

            BasicScript& operator[]( const ScriptSection section ) {
                switch( section ) {
                case ScriptSection::SS_HEADER:      return header;
                case ScriptSection::SS_INIT:        return init;
                case ScriptSection::SS_MAIN:        return main;
                case ScriptSection::SS_ACTION:      return action;
                case ScriptSection::SS_ENDCOND:     return endConditions;
                case ScriptSection::SS_REST:        return other;
                }
                return other;
            }

            BasicScript& headerSection() {
                return header;
            }

            BasicScript& initSection() {
                return init;
            }

            BasicScript& mainSection() {
                return main;
            }

            BasicScript& actionSection() {
                return action;
            }

            BasicScript& otherSection() {
                return other;
            }

            void clearData();

            std::vector< std::string > build();

            void storeParameters( const std::string& mapType, const std::string& seed );

            void addLine( const std::string& line ) {
                other.addLine( line );
            }

            void addLine( const ScriptSection section, const std::string& line ) {
                BasicScript& script = getSection( section );
                script.addLine( line );
            }

            void addLineInit( const std::string& line ) {
                init.addLine( line );
            }

            void addLineAction( const std::string& line ) {
                action.addLine( line );
            }

            void addREM( const ScriptSection section, const std::string& comment ) {
                BasicScript& script = getSection( section );
                script.addLine( "REM " + comment );
            }

            bool isFXLevel() const {
                return isFX;
            }

            void setFXLevel() {
                isFX = true;
                init.setFXLevel();
            }

            void concealWholeMap( const Player player );

            void addAvailable( const adiktedpp::Player player, const Room item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Creature item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Door item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Trap item, const int accessible, const int available );

            void addAvailable( const adiktedpp::Player player, const Spell item, const int accessible, const int available );

            template <typename TValue>
            void set( const AvailableCommandStateMap<TValue>& stateMap ) {
                auto iter1 = stateMap.begin();
                for ( ; iter1 != stateMap.end(); ++iter1 ) {
                    const adiktedpp::Player player = iter1->first;
                    const auto& submap = iter1->second;

                    auto iter2 = submap.begin();
                    for ( ; iter2 != submap.end(); ++iter2 ) {
                        const auto item = iter2->first;
                        const auto& state = iter2->second;
                        addAvailable( player, item, state.accessible, state.available );
                    }
                }
            }

            /// set creature pool
            void setEvilCreaturesPool( const std::size_t number );

            void setHeroCreaturesPool( const std::size_t number );

            void setCreaturePool( const Creature creature, const std::size_t number );

            /// ================================================================================

            /// set available creatures from pool
            void setEvilCreaturesAvailable( const Player player, const bool available = true );

            /// set available creatures from pool
            void setHeroCreaturesAvailable( const Player player, const bool available = true );

            void setRoomsAvailable( const Player player, const AvailableRoomMode mode = AvailableRoomMode::ARM_DISABLED );

            void setRoomAvailable( const Player player, const Room room, const AvailableRoomMode mode = AvailableRoomMode::ARM_DISABLED );

            void setRoomsStandard();

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setDoorsAvailable( const Player player, const int available = -1 );

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setTrapsAvailable( const Player player, const int available = -1 );

            void setMagicAvailable( const Player player, const AvailableMode mode = AvailableMode::AM_DISABLED );

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

            BasicScript& getSection( const ScriptSection section );

        };


        /**
         * Wrapper class for script related functions of adikted lib.
         */
        class LevelScript {

            Level* levelPtr;


        public:

            LevelScript( Level& level );

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
