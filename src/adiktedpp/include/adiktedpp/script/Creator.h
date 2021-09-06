/*
 * Creator.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_

#include "adiktedpp/script/Script.h"
#include "adiktedpp/Type.h"

#include <string>


namespace adiktedpp {
    namespace script {

        /**
         *
         */
        class AmbushCreator {
        public:

            adiktedpp::script::Script& script;

            std::string partyId;                    /// unique value among parties
            std::string description;                /// use in comment
            std::size_t actionPoint = 0;
            std::size_t partyCopies = 1;
            Player owner = Player::P_GOOD;


            AmbushCreator( adiktedpp::script::Script& script ): script(script) {
            }

            void prepare();

            void addToParty( const Creature creature, const std::size_t crNum, const std::size_t crExp, const std::size_t gold, const PartyObjective objective, const std::size_t countdown = 0 );

        };

    } /* namespace script */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_SCRIPT_CREATOR_H_ */
