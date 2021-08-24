/*
 * Fortress_test.cpp
 *
 */

#include "dkmage/spatial/Fortress.h"

#include "catch2/catch.hpp"

#include "dkmage/spatial/Fortress.h"

//#include "dkmage/spatial/Fortress.h"


using namespace adiktedpp;
using namespace dkmage;
using namespace spatial;


namespace {

    class FortressDungeonMock: public FortressDungeon {
    public:

//        using FortressDungeon::createRoom;

    };

}


TEST_CASE( "FortressDungeon_freeDirections" ) {
    utils::rng_srand( 5 );

    FortressDungeonMock dungeon;
    const FortressRoom* next = nullptr;
    next = dungeon.setFirstRoom( FortressRoomType::FR_DUNGEON_HEART );
    next = dungeon.addRandomRoom( FortressRoomType::FR_BOULDER_CORRIDOR, *next );
    REQUIRE( next != nullptr );
    CHECK( dungeon.roomsNum() == 2 );

    const std::vector< Direction > freeDirections = dungeon.freeDirections( *next );
    CHECK( freeDirections.size() == 1 );
}
