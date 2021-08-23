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

        using FortressDungeon::createRoom;

        FortressRoom* createRoom( const FortressRoomType roomType, const std::size_t roomSizeX, const std::size_t roomSizeY, const FortressRoom& from, const Direction direction, const std::size_t corridorLength ) {
            std::unique_ptr< FortressRoom > roomPtr = spawn_object( *this, roomType );
            FortressRoom& newRoom = *roomPtr;

            newRoom.setPosition( Rect( roomSizeX, roomSizeY ) );

            const bool added = createRoom( newRoom, from, direction, corridorLength );
            if ( added == false ) {
                return nullptr;
            }

             /// room prepared/added properly
            roomPtr.release();                              //// release ownership (prevent deleting the object)
            newRoom.owner( player );
            return &newRoom;
        }

    };

}


//TEST_CASE( "FortressDungeon_single" ) {
//    utils::rng_srand( 5 );
//
//    FortressDungeonMock dungeon;
//    const FortressRoom* next = nullptr;
//    next = dungeon.setFirstRoom( FortressRoomType::FR_DUNGEON_HEART, 5 );
////    next = dungeon.addRandomRoom( FortressRoomType::FR_EMPTY, 3, 3, *next, Direction::D_SOUTH, 3 );
//
//    LOG() << "xxx: " << dungeon.print();
//
//    REQUIRE( next != nullptr );
//    const std::size_t dSize = dungeon.size();
//    REQUIRE( dSize == 4 );
//}

TEST_CASE( "FortressDungeon_addBoulderCorridor" ) {
    utils::rng_srand( 5 );

    FortressDungeonMock dungeon;
    const FortressRoom* next = nullptr;
    next = dungeon.setFirstRoom( FortressRoomType::FR_DUNGEON_HEART, 5 );
    next = dungeon.createRoom( FortressRoomType::FR_EMPTY, 3, 3, *next, Direction::D_SOUTH, 3 );
    next = dungeon.createRoom( FortressRoomType::FR_BOULDER_CORRIDOR, 9, 3, *next, Direction::D_WEST, 1 );
    next = dungeon.createRoom( FortressRoomType::FR_BOULDER_CORRIDOR, 9, 3, *next, Direction::D_WEST, 1 );
    REQUIRE( next != nullptr );
    const std::size_t dSize = dungeon.roomsNum();
    REQUIRE( dSize == 4 );
}

TEST_CASE( "FortressDungeon_freeDirections" ) {
    utils::rng_srand( 5 );

    FortressDungeonMock dungeon;
    const FortressRoom* next = nullptr;
    next = dungeon.setFirstRoom( FortressRoomType::FR_DUNGEON_HEART, 5 );
    next = dungeon.addRandomRoom( FortressRoomType::FR_BOULDER_CORRIDOR, *next );
    REQUIRE( next != nullptr );
    CHECK( dungeon.roomsNum() == 2 );

    const std::vector< Direction > freeDirections = dungeon.freeDirections( *next );
    CHECK( freeDirections.size() == 1 );
}
