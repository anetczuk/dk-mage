/*
 * Script.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_

#include "adiktedpp/Level.h"


namespace adiktedpp {

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


    private:

        void convertToTextArray();

        void freeTxt();

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_H_ */
