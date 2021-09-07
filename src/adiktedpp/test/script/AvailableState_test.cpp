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


TEST_CASE("CreatureAvailableState_setStateMode", "[classic]") {
    CreatureAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Creature::C_SKELETON, AvailableMode::AM_ENABLED );
    {
        const CreatureAvailableState::State* state = commandState.getState( Player::P_P0, Creature::C_SKELETON );
        REQUIRE( state != nullptr );
        CHECK( state->available  == 1 );
        CHECK( state->numberForced == 0 );
    }

    {
        /// configured
        const CreatureAvailableState::State* state = commandState.getState( Player::P_P0, Creature::C_SKELETON );
        REQUIRE( state != nullptr );
        CHECK( state->available  == 1 );
        CHECK( state->numberForced == 0 );
    }
    {
        /// default
        const CreatureAvailableState::State* state = commandState.getState( Player::P_P0, Creature::C_DRAGON );
        REQUIRE( state == nullptr );
    }
}

/// ===============================================================================================

TEST_CASE("RoomsAvailableState_enabled_disabled", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, RoomAvailableMode::RAM_AVAILABLE );
    {
        const RoomAvailableMode* state = commandState.getState( Player::P_P0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( *state == RoomAvailableMode::RAM_AVAILABLE );
    }

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, RoomAvailableMode::RAM_DISABLED );
    {
        const RoomAvailableMode* state = commandState.getState( Player::P_P0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( *state == RoomAvailableMode::RAM_DISABLED );
    }
}

TEST_CASE("RoomsAvailableState_notdefined", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, RoomAvailableMode::RAM_AVAILABLE );

    const RoomAvailableMode* state = commandState.getState( Player::P_P0, Room::R_PRISON );
    REQUIRE( state == nullptr );
}

TEST_CASE("RoomsAvailableState_allplayer_explode", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, RoomAvailableMode::RAM_RESEARCHABLE );
    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, RoomAvailableMode::RAM_DISABLED );
    CHECK( commandState.size() == 5 );

    const RoomAvailableMode* state = commandState.getState( Player::P_P1, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( *state == RoomAvailableMode::RAM_RESEARCHABLE );
}

TEST_CASE("RoomsAvailableState_allplayer_erase", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, RoomAvailableMode::RAM_DISABLED );
    CHECK( commandState.size() == 1 );

    commandState.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, RoomAvailableMode::RAM_RESEARCHABLE );
    CHECK( commandState.size() == 1 );

    const RoomAvailableMode* state = commandState.getState( Player::P_ALL, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( *state == RoomAvailableMode::RAM_RESEARCHABLE );
}
