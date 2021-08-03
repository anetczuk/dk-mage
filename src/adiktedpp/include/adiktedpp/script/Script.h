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
        class Script {

            Level* levelPtr;
            std::vector< std::string > lines;


        public:

            Script( Level& level );

            void rebuild();

            bool recompose();

            /**
             * Execute script function against level.
             * Function name and arguments are given in string format, e.g. 'LEVEL_AUTHORS("dkmage","dkmage")'.
             * Currently there is possible to execute functions from adikted specific group (CMD_ADIKTED).
             */
            bool executeLine( const std::string& line );

            /// execute commands stored in ADI file
            void execScriptADI( const std::string& path );

            bool addAvailable( const adiktedpp::PlayerType player, const Room item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::PlayerType player, const Creature item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::PlayerType player, const Door item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::PlayerType player, const Trap item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::PlayerType player, const Spell item, const int accessible, const int available );

            bool addLine( const std::string& line );

            bool addLine( const std::string& line, const std::size_t position );

            template <typename TValue>
            void set( const AvailableCommandStateMap<TValue>& stateMap ) {
                auto iter1 = stateMap.begin();
                for ( ; iter1 != stateMap.end(); ++iter1 ) {
                    const adiktedpp::PlayerType player = iter1->first;
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
            void setEvilCreaturesAvailable( const PlayerType player, const bool available = true );

            /// set available creatures from pool
            void setHeroCreaturesAvailable( const PlayerType player, const bool available = true );

            void setRoomsAvailable( const PlayerType player, const AvailableMode mode = AvailableMode::AM_DISABLED );

            void setRoomAvailable( const PlayerType player, const Room room, const AvailableMode mode = AvailableMode::AM_DISABLED );

            void setRoomsStandard();

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setDoorsAvailable( const PlayerType player, const int available = -1 );

            /**
             * "available" parameter meaning:
             *      - negative -- disabled
             *      - zero     -- possible to workshop
             *      - positive -- number of available items
             */
            void setTrapsAvailable( const PlayerType player, const int available = -1 );

            void setMagicAvailable( const PlayerType player, const AvailableMode mode = AvailableMode::AM_DISABLED );

            void setMagicStandard( const PlayerType player );

            /// ================================================================================

            void setWinConditionStandard( const PlayerType player );


        private:

            void convertToTextArray();

            void freeTxt();

        };

    }
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_ */
