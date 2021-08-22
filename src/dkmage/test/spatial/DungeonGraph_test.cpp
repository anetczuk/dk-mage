/*
 * DungeonGraph_test.cpp
 *
 */

#include "catch2/catch.hpp"

#include "dkmage/spatial/DungeonGraph.h"


using namespace dkmage;
using namespace spatial;


namespace {

    /**
     *
     */
    class RoomMock {
        ///
    };

    typedef DungeonGraph< RoomMock, NoEdgeData > DungeonGraphMock;

}


TEST_CASE("DungeonGraph_size_new", "[classic]") {
    DungeonGraphMock graph;
    REQUIRE( graph.nodesNum() == 0 );
}

TEST_CASE("DungeonGraph_itemByindex_invalid", "[classic]") {
    DungeonGraphMock graph;
    RoomMock* room = graph.itemByindex( 999 );
    REQUIRE( room == nullptr );
}

TEST_CASE("DungeonGraph_itemByindex_empty", "[classic]") {
    DungeonGraphMock graph;
    RoomMock* room = graph.itemByindex( 0 );
    REQUIRE( room == nullptr );
}

TEST_CASE("DungeonGraph_addItem_02", "[classic]") {
    DungeonGraphMock graph;
    graph.addItem();
    RoomMock* room = graph.itemByindex( 0 );                                /// id: 0
//    LOG() << "graph 1:\n" << graph.print();
    REQUIRE( room != nullptr );

    RoomMock* northRoom = graph.addItem( *room, Direction::D_NORTH );       /// id: 1
//    LOG() << "graph 2:\n" << graph.print();
    REQUIRE( northRoom != nullptr );
    REQUIRE( northRoom != room );

    RoomMock* northRoom2 = graph.addItem( *room, Direction::D_NORTH );
//    LOG() << "graph 3:\n" << graph.print();
    REQUIRE( northRoom2 == northRoom );
}

TEST_CASE("DungeonGraph_addItem_03", "[classic]") {
    DungeonGraphMock graph;
    graph.addItem();
    RoomMock* room = graph.itemByindex( 0 );                                /// id: 0
//    LOG() << "graph 1:\n" << graph.print();
    REQUIRE( room != nullptr );

    RoomMock* northRoom = graph.addItem( *room, Direction::D_NORTH );       /// id: 1
//    LOG() << "graph 2:\n" << graph.print();
    REQUIRE( northRoom != nullptr );
    REQUIRE( northRoom != room );

    RoomMock* southRoom = graph.addItem( *room, Direction::D_SOUTH );       /// id: 2
//    LOG() << "graph 3:\n" << graph.print();
    REQUIRE( southRoom != nullptr );
    REQUIRE( southRoom != room );
    REQUIRE( southRoom != northRoom );

    RoomMock* southRoom2 = graph.addItem( *room, Direction::D_SOUTH );
//    LOG() << "graph 4:\n" << graph.print();
    REQUIRE( southRoom2 == southRoom );
}
