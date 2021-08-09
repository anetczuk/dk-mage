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

            void storeParameters( const std::string& mapType, const std::string& seed );

            bool addREM( const std::string& comment );

            void setStartMoney( const adiktedpp::Player player, const std::size_t amount );

            bool addAvailable( const adiktedpp::Player player, const Room item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::Player player, const Creature item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::Player player, const Door item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::Player player, const Trap item, const int accessible, const int available );

            bool addAvailable( const adiktedpp::Player player, const Spell item, const int accessible, const int available );

            bool addLine( const std::string& line );

            bool addLine( const std::string& line, const std::size_t position );

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

            void setRoomsAvailable( const Player player, const AvailableMode mode = AvailableMode::AM_DISABLED );

            void setRoomAvailable( const Player player, const Room room, const AvailableMode mode = AvailableMode::AM_DISABLED );

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

            void setWinConditionStandard( const Player player );


        private:

            void convertToTextArray();

            void freeTxt();

        };

    }
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_ */
