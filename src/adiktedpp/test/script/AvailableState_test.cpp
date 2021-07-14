/*
 *
 */

#include "catch2/catch.hpp"

#include "adiktedpp/script/AvailableState.h"

//#include "adiktedpp/Messages.h"
//#include "Path.h"
//#include "utils/Log.h"
//#include "utils/Separator.h"


using namespace adiktedpp;
using namespace script;


//inline ScopeMessages initialize_messages() {
//    const std::string testName = Catch::getResultCapture().getCurrentTestName();
//    return ScopeMessages( testName, ".log" );
//}


TEST_CASE("RoomsAvailableState_enabled_disabled", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD, AvailableMode::AM_ENABLED );
    {
        const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( state->accessible == 1 );
        CHECK( state->available  == 1 );
    }

    commandState.setStateMode( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD, AvailableMode::AM_DISABLED );
    {
        const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( state->accessible == 0 );
        CHECK( state->available  == 0 );
    }
}

TEST_CASE("RoomsAvailableState_notdefined", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD, AvailableMode::AM_ENABLED );

    const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_0, Room::R_PRISON );
    REQUIRE( state == nullptr );
}

TEST_CASE("RoomsAvailableState_allplayer", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( adiktedpp::PlayerType::PT_ALL, Room::R_GRAVEYARD, AvailableMode::AM_POSSIBLE );

    const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( state->accessible == 1 );
    CHECK( state->available  == 0 );
}
