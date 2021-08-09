/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Maze.h"
#include "dkmage/spatial/Dungeon.h"

#include "adiktedpp/script/Script.h"

#include "utils/Set.h"

#include <cmath>


using namespace utils;
using namespace adiktedpp;


namespace dkmage {
    namespace mode {

        void MazeMode::generateLevel() {
            level.generateRandomMap( 9 );

            spatial::Maze maze;
            maze.corridorSize = 3;
            maze.generate( 21, 6 );
//                maze.corridorSize = 1;
//                maze.generate( 41, 12 );

            maze.centerizeOnMap();

            drawMaze( level, maze );

            generateMazeItems( maze );

            LOG() << "preparing dungeons";
            preparePlayerDungeon();

            prepareEnemyDungeon();

            /// =========== scripting ===========
            LOG() << "preparing script";
            script::Script script( level );
            {
                const std::string type = parameters.getString( ParameterName::PN_TYPE, "" );
                const std::string seed = parameters.getString( ParameterName::PN_SEED, "" );
                script.storeParameters( type, seed );
            }

            script.addLine( "" );
            script.addLine( "SET_GENERATE_SPEED( 500 )" );

            script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
            script.addLine( "MAX_CREATURES( PLAYER0, 10 )" );
            script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                script.addLine( "START_MONEY( PLAYER0, 200000 )" );             /// does not show in treasure
            } else {
                script.addLine( "START_MONEY( PLAYER0,  20000 )" );             /// does not show in treasure
            }
            script.addLine( "START_MONEY( PLAYER1, 250000 )" );                 /// does not show in treasure

            script.addLine( "" );
            script.setHeroCreaturesPool( 20 );

            const std::set< Player > availablePlayers = { Player::P_P0, Player::P_P0 };

            script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setHeroAvailable( Player::P_P0 );

            script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( Player::P_ALL, Room::R_BRIDGE, script::AvailableMode::AM_DISABLED );

            /// necromancer mode
            availableCreatures.setEvilAvailable( Player::P_P0, false );
            availableRooms.setStateMode( Player::P_P0, Room::R_TORTURE, script::AvailableMode::AM_DISABLED );

            script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( Player::P_ALL, true );
            availableTraps.setStateFlag( Player::P_ALL, Trap::T_LAVA, false );

            script.addLine( "" );
            script.set( availableCreatures );

            script.addLine( "" );
            script.set( availableRooms );

            script.addLine( "" );
            script.setDoorsAvailable( Player::P_ALL, 0 );

            script.addLine( "" );
            script.set( availableTraps );

            script.addLine( "" );
            script.setMagicStandard( Player::P_ALL );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( Player::P_P0 );

            script.rebuild();

            /// ========================================================

            const bool valid = level.verifyMap();
            if ( valid == false ) {
                LOG() << "detected invalid map -- restarting generation";
//                    storePreview( "level.bmp" );
                generateLevel();
                return ;
            }
        }

        void MazeMode::generateMazeItems( spatial::Maze& maze ) {
            const std::size_t corrNum = maze.corridorsNum();
            std::set< std::size_t > indexSet;
            for ( std::size_t i=0; i<corrNum; ++i ) {
                indexSet.insert( i );
            }

//                {
//                    const std::size_t furthestIndex = maze.getFurthestIndex();
//                    if ( indexSet.erase( furthestIndex ) > 0 ) {
//                        const Rect furthest = maze.nodeRect( furthestIndex );
//                        drawSpecial( level, furthest, SubTypeItem::STI_SPMULTPLY );
//                    }
//                }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_REVMAP );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, Item::I_SPECIAL_INCLEV );
                }
            }

            {
                const std::set< std::size_t > deadEndList = maze.getDeadEnds();
                for ( std::size_t item: deadEndList ) {
                    if ( indexSet.erase( item ) > 0 ) {
                        const Rect furthest = maze.nodeRect( item );
                        drawGoldChests( level, furthest );
                    }
                }
            }

            std::set< Trap > damageTraps = Traps();
            damageTraps.erase( Trap::T_LAVA );                          /// remove lava traps, because bridges are disabled
            const std::size_t trapsSize = damageTraps.size();
            const int maxDistance = maze.maxDistance();

            const std::size_t trapsChambers = maze.corridorsNum() * 0.3;
            LOG() << "generating traps chambers: " << trapsChambers;
            for ( std::size_t i=0; i<trapsChambers; ++i ) {
                const std::size_t setIndex = rand() % indexSet.size();
                const std::size_t cIndex = getSetItem( indexSet, setIndex, true );
                const std::size_t nx = cIndex % maze.nodesX();
                const std::size_t ny = cIndex / maze.nodesX();
                const Rect nodeRect = maze.nodeRect( nx, ny );
                const Point nodeCenter = nodeRect.center();
                const int nodeDistance = maze.nodeDistance( nx, ny );

                const std::size_t trapIndex = rand() % trapsSize;
                const Trap trapType = getSetItem( damageTraps, trapIndex );

                if ( maze.corridorSize == 1 ) {
                    level.setSlab( nodeCenter, Slab::S_PATH );
                    level.setTrap( nodeCenter, 4, trapType );
                } else if ( maze.corridorSize == 3 ) {
                    const std::size_t chamberIndex = rand() % 4;
                    switch( chamberIndex ) {
                    case 0: {
                        drawTrap3x3X( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    case 1: {
                        drawTrap3x3Diamond( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    case 2: {
                        drawTrap3x3Corners( level, nodeCenter, trapType, Slab::S_PATH );
                        break ;
                    }
                    default: {
                        const double distanceFactor = ((double) nodeDistance) / maxDistance * 2.0;
                        randHeroTrap( maze, nodeCenter, distanceFactor );
                        break ;
                    }
                    }
                }
            }
        }

        void MazeMode::randHeroTrap( spatial::Maze& maze, const Point& nodeCenter, const double distanceFactor ) {
//                level.setVein( chamber, SlabType::ST_PATH, cSize );
            const Rect chamber( nodeCenter, maze.corridorSize, maze.corridorSize );
            level.setSlab( chamber, Slab::S_PATH );

            int creatureLevel = std::min( (int)(distanceFactor * 9 + 1), 9 );          /// in range [1, 9]
            creatureLevel = std::max( creatureLevel, 3 );                              /// in range [3, 9]

            drawHeroTrap( level, nodeCenter, creatureLevel );
        }

        void MazeMode::preparePlayerDungeon() {
            spatial::Dungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                dungeon.generate( 4, 5 );
            } else {
                dungeon.generate( 1, 5 );
            }

            dungeon.moveToBottomEdge( 4 );

//                LOG() << "dungeon:\n" << dungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, dungeon );

            const spatial::DungeonRoom* heart = dungeon.room( 0 );
            const Point firstCenter = heart->position().center();
            const Rect bbox = dungeon.boundingBox();

            /// add neutral portal
            const Point portalCenter( bbox.max.x + 16, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.setRoom( portalRect, Room::R_PORTAL );

            {
                /// add gold vein
                std::size_t goldSlabsNum = parameters.getSizeT( ParameterName::PN_GOLD_SLABS_NUMBER, 40 );
                LOG() << "gold slabs number: " << goldSlabsNum;

                std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 1 );
                gemsNum = std::min( gemsNum, (std::size_t)4 );
                LOG() << "gems number: " << gemsNum;

                const std::size_t leftVeinGold = goldSlabsNum;
                const std::size_t leftVeinDimm = (std::size_t) sqrt( leftVeinGold ) * 1.5;
                const Rect mapRect = raw::RawLevel::mapRect();
                Rect leftVeinRect( leftVeinDimm, leftVeinDimm );
                leftVeinRect.move( 10, 0 );
                leftVeinRect.moveBottomTo( mapRect.max.y );
                drawGoldVein( level, leftVeinRect, leftVeinGold, gemsNum );
            }

//            /// add gold vein
//            const Rect mapRect = raw::RawLevel::mapRect();
//            const int veinY = mapRect.max.y - 4;
//            const Point veinCenter( bbox.min.x - 20, veinY );
//            const Rect veinRect( veinCenter, 9, 5 );
//
//            std::size_t gemsNum = parameters.getSizeT( ParameterName::PN_GEM_FACES_NUMBER, 2 );
//            gemsNum = std::min( gemsNum, (std::size_t)4 );
//            LOG() << "gems number: " << gemsNum;
//
//            drawGoldVein( level, veinRect, gemsNum );

            /// add other
            if ( parameters.isSet( ParameterName::PN_TEST_MODE ) ) {
                const Point revPos = heart->edgePoint( spatial::Direction::D_SOUTH );
                level.setItem( revPos, 4, Item::I_SPECIAL_REVMAP );

                const adiktedpp::Player player = dungeon.owner();
                level.setFortified( revPos + Point(0, 1), player );
                level.setSlab( revPos + Point(0, 2), Slab::S_EARTH );
                const Point monstersPos = revPos + Point(0, 3);
                level.setSlab( monstersPos, Slab::S_PATH );
                level.setCreatureAuto( monstersPos, Creature::C_MISTRESS, 20, 9 );
            }

            level.setCreature( firstCenter.x-1, firstCenter.y-2, 4, Creature::C_WARLOCK, 1 );
            level.setCreature( firstCenter.x+1, firstCenter.y-2, 4, Creature::C_SKELETON, 3 );
            level.setCreatureAuto( firstCenter.x, firstCenter.y+2, Creature::C_IMP, 8 );

//                    /// fill treasure with gold
//                    spatial::DungeonRoom* treasure = dungeon.findRoomFirst( SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void MazeMode::prepareEnemyDungeon() {
        //    spatial::Dungeon enemyDungeon( Player::PT_GOOD );
            spatial::Dungeon enemyDungeon( Player::P_P1 );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

//                enemyDungeon.generate( 6, 5 );
            enemyDungeon.generate( 12, 5 );
//                enemyDungeon.addRandomRoom( SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( SlabType::ST_PORTAL, 3 );
            enemyDungeon.moveToTopEdge( 4 );

        //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

            const Point firstCenter = enemyDungeon.roomCenter( 0 );
            const Rect bbox = enemyDungeon.boundingBox();
            const Player owner = enemyDungeon.owner();

            /// add portal
            const Point portalCenter( bbox.min.x - 8, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.digLine( firstCenter, portalCenter, owner, true );
            level.setRoom( portalRect, Room::R_PORTAL, owner, true );

            /// add gold ore
            const Point veinCenter( bbox.max.x + 12, 4 );
            level.digLine( firstCenter, veinCenter );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 3 );

            /// add other
            level.setCreatureAuto( firstCenter.x, firstCenter.y+2, Creature::C_IMP, 8 );
//                    level.setCreature( firstCenter.x, firstCenter.y-2, 4, SubTypeCreature::STC_SKELETON, 1 );

            /// fill treasure with gold
            const std::vector< spatial::DungeonRoom* > treasures = enemyDungeon.findRoom( Room::R_TREASURE );
            for ( const spatial::DungeonRoom* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, Item::I_GOLD_HOARD3 );
            }
        }

    } /* namespace mode */
} /* namespace dkmage */
