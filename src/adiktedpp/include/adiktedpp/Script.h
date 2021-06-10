/*
 * Script.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_

#include "adiktedpp/Level.h"


namespace adiktedpp {

    enum class AvailableMode {
        AM_DISABLED,
        AM_POSSIBLE,            /// available after meet certain criteria (e.g. research or build proper room)
        AM_ENABLED
    };


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

        bool addLine( const std::string& line );

        bool addLine( const std::string& line, const std::size_t position );

        /// set creature pool
        void setEvilCreaturesPool( const std::size_t number );

        /// set available creatures from pool
        void setEvilCreaturesAvailable( const PlayerType player, const AvailableMode mode = AvailableMode::AM_DISABLED );

        void setRoomsAvailable( const PlayerType player, const AvailableMode mode = AvailableMode::AM_DISABLED );

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


    private:

        void convertToTextArray();

        void freeTxt();

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_ */
