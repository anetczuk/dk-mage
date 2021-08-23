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

        /**
         *
         */
        class BasicScript {

            std::vector< std::string > lines;
            std::size_t level;                      /// indent level


        public:

            BasicScript(): level(0) {
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

            void addLine( const std::string& line );

            void addLineIndent( const std::string& line, const std::size_t forceIndent );

//            void addLine( const std::string& line, const std::size_t position );

            void addREM( const std::string& comment ) {
                return addLine( "REM " + comment );
            }

            void setFXLevel();

            void setStartMoney( const adiktedpp::Player player, const std::size_t amount );

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


        enum class ScriptSection {
            SS_HEADER,
            SS_INIT,
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
            BasicScript action;
            BasicScript endConditions;
            BasicScript other;


        public:

            Script();

            BasicScript& operator[]( const ScriptSection section ) {
                switch( section ) {
                case ScriptSection::SS_HEADER:      return header;
                case ScriptSection::SS_INIT:        return init;
                case ScriptSection::SS_ACTION:      return action;
                case ScriptSection::SS_ENDCOND:     return endConditions;
                case ScriptSection::SS_REST:        return other;
                }
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

            /// signed, hero gates has negative numbers
            void addLineActionIf( const int actionNumber, const adiktedpp::Player player );

            void addLineActionEndIf() {
                action.addLine( "ENDIF" );
            }

            void addREM( const ScriptSection section, const std::string& comment ) {
                BasicScript& script = getSection( section );
                script.addLine( "REM " + comment );
            }

            void setFXLevel() {
                init.setFXLevel();
            }

            void setStartMoney( const adiktedpp::Player player, const std::size_t amount ) {
                init.setStartMoney( player, amount );
            }

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

            void setWinConditionStandard( const Player player );

            void setWinConditionKillGood();

            void setLoseConditionStandard( const Player player );


        private:

            BasicScript& getSection( const ScriptSection section );

        };


        /**
         *
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
