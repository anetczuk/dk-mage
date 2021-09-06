/*
 * Creator.cpp
 *
 */

#include "adiktedpp/script/Creator.h"

#include "utils/Log.h"


namespace adiktedpp {
    namespace script {

        void AmbushCreator::prepare() {
            if ( partyId.empty() ) {
                LOG_ERR() << "invalid partyId: " << partyId;
                std::stringstream stream;
                stream << FILE_NAME << ": invalid partyId: " << partyId;
                throw std::invalid_argument( stream.str() );
            }
            if ( actionPoint == 0 ) {
                LOG_ERR() << "invalid actionPoint: " << actionPoint;
                std::stringstream stream;
                stream << FILE_NAME << ": invalid actionPoint: " << actionPoint;
                throw std::invalid_argument( stream.str() );
            }

            script::ScriptCommand& parties = script.partiesSection();
            parties.addEmptyLine();
            if ( description.empty() == false )
                parties.REM( description );
            parties.CREATE_PARTY( partyId );

            script::ScriptCommand& actions = script.actionSection();
            actions.addEmptyLine();
            if ( description.empty() == false )
                actions.REM( description );
            actions.REM( std::to_string( actionPoint ) + " -- ambush point" );
            actions.IF_ACTION_POINT( actionPoint, Player::P_P0 );
            actions.ADD_PARTY_TO_LEVEL( owner, partyId, actionPoint, partyCopies );
            actions.ENDIF();
        }

        void AmbushCreator::addToParty( const Creature creature, const std::size_t crNum, const std::size_t crExp, const std::size_t gold, const PartyObjective objective, const std::size_t countdown ) {
            script::ScriptCommand& parties = script.partiesSection();
            parties.ADD_TO_PARTY( partyId, creature, crNum, crExp, gold, objective, countdown );
        }

    } /* namespace script */
} /* namespace adiktedpp */
