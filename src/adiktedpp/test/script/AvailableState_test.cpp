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

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, AvailableRoomMode::ARM_ENABLED );
    {
        const RoomsAvailableState::State* state = commandState.getState( Player::P_P0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( state->researchable == 1 );
        CHECK( state->available  == 1 );
    }

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, AvailableRoomMode::ARM_DISABLED );
    {
        const RoomsAvailableState::State* state = commandState.getState( Player::P_P0, Room::R_GRAVEYARD );
        REQUIRE( state != nullptr );
        CHECK( state->researchable == 0 );
        CHECK( state->available  == 0 );
    }
}

TEST_CASE("RoomsAvailableState_notdefined", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, AvailableRoomMode::ARM_ENABLED );

    const RoomsAvailableState::State* state = commandState.getState( Player::P_P0, Room::R_PRISON );
    REQUIRE( state == nullptr );
}

TEST_CASE("RoomsAvailableState_allplayer_explode", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, AvailableRoomMode::ARM_POSSIBLE );
    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, AvailableRoomMode::ARM_DISABLED );
    CHECK( commandState.size() == 5 );

    const RoomsAvailableState::State* state = commandState.getState( Player::P_P1, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( state->researchable == 1 );
    CHECK( state->available  == 0 );
}

TEST_CASE("RoomsAvailableState_allplayer_erase", "[classic]") {
    RoomsAvailableState commandState;

    commandState.setStateMode( Player::P_P0, Room::R_GRAVEYARD, AvailableRoomMode::ARM_DISABLED );
    CHECK( commandState.size() == 1 );

    commandState.setStateMode( Player::P_ALL, Room::R_GRAVEYARD, AvailableRoomMode::ARM_POSSIBLE );
    CHECK( commandState.size() == 1 );

    const RoomsAvailableState::State* state = commandState.getState( Player::P_ALL, Room::R_GRAVEYARD );
    REQUIRE( state != nullptr );
    CHECK( state->researchable == 1 );
    CHECK( state->available  == 0 );
}
