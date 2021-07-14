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

TEST_CASE("RoomsAvailableState_allplayer_explode", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( adiktedpp::PlayerType::PT_ALL, Room::R_GRAVEYARD, AvailableMode::AM_POSSIBLE );
    commandState.setStateMode( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD, AvailableMode::AM_DISABLED );
    CHECK( commandState.size() == 5 );

    const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_1, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( state->accessible == 1 );
    CHECK( state->available  == 0 );
}

TEST_CASE("RoomsAvailableState_allplayer_erase", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( adiktedpp::PlayerType::PT_0, Room::R_GRAVEYARD, AvailableMode::AM_DISABLED );
    CHECK( commandState.size() == 1 );

    commandState.setStateMode( adiktedpp::PlayerType::PT_ALL, Room::R_GRAVEYARD, AvailableMode::AM_POSSIBLE );
    CHECK( commandState.size() == 1 );

    const AvailableState* state = commandState.getState( adiktedpp::PlayerType::PT_ALL, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( state->accessible == 1 );
    CHECK( state->available  == 0 );
}
