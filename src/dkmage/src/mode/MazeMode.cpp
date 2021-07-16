/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/Draw.h"
#include "dkmage/spatial/Maze.h"
#include "dkmage/spatial/Dungeon.h"
#include "dkmage/BaseLevelGenerator.h"
#include "dkmage/Generator.h"

#include "adiktedpp/script/Script.h"

#include "utils/Rand.h"


namespace dkmage {
    namespace mode {

        void MazeMode::generate( const std::size_t seed ) {
            LOG() << "generating level";
            srand( seed );
            generateLevel();
        }

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

            preparePlayerDungeon();

            prepareEnemyDungeon();

            /// =========== scripting ===========

            adiktedpp::script::Script script( level );

            script.addLine( "SET_GENERATE_SPEED( 500 )" );

            script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
            script.addLine( "MAX_CREATURES( PLAYER0, 10 )" );
            script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );

            script.addLine( "START_MONEY( PLAYER0,  20000 )" );                 /// does not show in treasure
            script.addLine( "START_MONEY( PLAYER1, 250000 )" );                 /// does not show in treasure

            script.addLine( "" );
            script.setHeroCreaturesPool( 20 );

            const std::set< adiktedpp::PlayerType > availablePlayers = { adiktedpp::PlayerType::PT_0, adiktedpp::PlayerType::PT_1 };

            adiktedpp::script::CreatureAvailableState availableCreatures( availablePlayers );
            availableCreatures.setHeroAvailable( adiktedpp::PlayerType::PT_1 );

            adiktedpp::script::RoomsAvailableState availableRooms( availablePlayers );
            availableRooms.setStandard();
            availableRooms.setStateMode( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Room::R_BRIDGE, adiktedpp::script::AvailableMode::AM_DISABLED );

            /// necromancer mode
            availableCreatures.setEvilAvailable( adiktedpp::PlayerType::PT_0, false );
            availableRooms.setStateMode( adiktedpp::PlayerType::PT_0, adiktedpp::script::Room::R_TORTURE, adiktedpp::script::AvailableMode::AM_DISABLED );

            adiktedpp::script::TrapAvailableState availableTraps( availablePlayers );
            availableTraps.setAllAvailable( adiktedpp::PlayerType::PT_ALL, true );
            availableTraps.setStateFlag( adiktedpp::PlayerType::PT_ALL, adiktedpp::script::Trap::T_LAVA, false );

            script.addLine( "" );
            script.set( availableCreatures );

            script.addLine( "" );
            script.set( availableRooms );

            script.addLine( "" );
            script.setDoorsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );

            script.addLine( "" );
            script.set( availableTraps );

            script.addLine( "" );
            script.setMagicStandard( adiktedpp::PlayerType::PT_ALL );

            script.addLine( "" );
            script.addLine( "" );
            script.addLine( "REM --- main script ---" );
            script.addLine( "" );
            script.setWinConditionStandard( adiktedpp::PlayerType::PT_0 );

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
//                        const utils::Rect furthest = maze.nodeRect( furthestIndex );
//                        drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPMULTPLY );
//                    }
//                }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const utils::Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPREVMAP );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const utils::Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 0, 5 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const utils::Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
            }
            {
                const std::size_t furthestIndex = maze.getFurthestIndex( 20, 0 );
                if ( indexSet.erase( furthestIndex ) > 0 ) {
                    const utils::Rect furthest = maze.nodeRect( furthestIndex );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
            }

            {
                const std::set< std::size_t > deadEndList = maze.getDeadEnds();
                for ( std::size_t item: deadEndList ) {
                    if ( indexSet.erase( item ) > 0 ) {
                        const utils::Rect furthest = maze.nodeRect( item );
                        drawGoldChests( level, furthest );
                    }
                }
            }

            std::set< adiktedpp::SubTypeTrap > damageTraps = adiktedpp::DamageTraps();
            damageTraps.erase( adiktedpp::SubTypeTrap::STT_LAVA );                          /// remove lava traps, because bridges are disabled
            const std::size_t trapsSize = damageTraps.size();
            const int maxDistance = maze.maxDistance();

            const std::size_t trapsChambers = maze.corridorsNum() * 0.3;
            LOG() << "generating traps chambers: " << trapsChambers;
            for ( std::size_t i=0; i<trapsChambers; ++i ) {
                const std::size_t setIndex = rand() % indexSet.size();
                const std::size_t cIndex = utils::getSetItem( indexSet, setIndex, true );
                const std::size_t nx = cIndex % maze.nodesX();
                const std::size_t ny = cIndex / maze.nodesX();
                const utils::Rect nodeRect = maze.nodeRect( nx, ny );
                const utils::Point nodeCenter = nodeRect.center();
                const int nodeDistance = maze.nodeDistance( nx, ny );

                const std::size_t trapIndex = rand() % trapsSize;
                const adiktedpp::SubTypeTrap trapType = utils::getSetItem( damageTraps, trapIndex );

                if ( maze.corridorSize == 1 ) {
                    level.setSlab( nodeCenter, adiktedpp::SlabType::ST_PATH );
                    level.setTrap( nodeCenter, 4, trapType );
                } else if ( maze.corridorSize == 3 ) {
                    const std::size_t chamberIndex = rand() % 4;
                    switch( chamberIndex ) {
                    case 0: {
                        drawTrap3x3X( level, nodeCenter, trapType, adiktedpp::SlabType::ST_PATH );
                        break ;
                    }
                    case 1: {
                        drawTrap3x3Diamond( level, nodeCenter, trapType, adiktedpp::SlabType::ST_PATH );
                        break ;
                    }
                    case 2: {
                        drawTrap3x3Corners( level, nodeCenter, trapType, adiktedpp::SlabType::ST_PATH );
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

        void MazeMode::randHeroTrap( spatial::Maze& maze, const utils::Point& nodeCenter, const double distanceFactor ) {
//                level.setVein( chamber, adiktedpp::SlabType::ST_PATH, cSize );
            const utils::Rect chamber( nodeCenter, maze.corridorSize, maze.corridorSize );
            level.setSlab( chamber, adiktedpp::SlabType::ST_PATH );

            int creatureLevel = std::min( (int)(distanceFactor * 9 + 1), 9 );          /// in range [1, 9]
            creatureLevel = std::max( creatureLevel, 3 );                              /// in range [3, 9]

            drawHeroTrap( level, nodeCenter, creatureLevel );
        }

        void MazeMode::preparePlayerDungeon() {
            dkmage::spatial::Dungeon dungeon;
            dungeon.limitNorth = 1;
            dungeon.limitSouth = 0;
            dungeon.fortify( true );

            dungeon.generate( 1, 5 );
//                dungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 3, false, 1 );
            dungeon.moveToBottomEdge( 4 );

//                LOG() << "dungeon:\n" << dungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, dungeon );

            const Point firstCenter = dungeon.roomCenter( 0 );
            const Rect bbox = dungeon.boundingBox();

            /// add neutral portal
            const Point portalCenter( bbox.max.x + 16, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.setSlab( portalRect, adiktedpp::SlabType::ST_PORTAL );

            /// add gold vein
            const utils::Rect mapRect = adiktedpp::Level::mapRect();
            const int veinY = mapRect.max.y - 3;
            const Point veinCenter( bbox.min.x - 20, veinY );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 2 );

            /// add other
            Point pos = firstCenter + Point(0, 2);
            level.setItem( pos, 4, adiktedpp::SubTypeItem::STI_SPREVMAP );
            level.setCreature( firstCenter.x, firstCenter.y-2, 1, adiktedpp::SubTypeCreature::STC_WARLOCK, 1 );
            level.setCreature( firstCenter.x, firstCenter.y-2, 7, adiktedpp::SubTypeCreature::STC_SKELETON, 3 );
            level.setCreature( firstCenter.x, firstCenter.y-2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
            level.setCreature( firstCenter.x, firstCenter.y-2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );

//                    /// fill treasure with gold
//                    dkmage::spatial::Room* treasure = dungeon.findRoomFirst( adiktedpp::SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
//                    }
        }

        void MazeMode::prepareEnemyDungeon() {
        //    dkmage::spatial::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_GOOD );
            dkmage::spatial::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_1 );
            enemyDungeon.limitNorth = 0;
            enemyDungeon.limitSouth = 2;
            enemyDungeon.fortify( true );

//                enemyDungeon.generate( 6, 5 );
            enemyDungeon.generate( 12, 5 );
//                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 5 );
//                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_PORTAL, 3 );
            enemyDungeon.moveToTopEdge( 4 );

        //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

            /// dungeon have to be drawn before placing items inside it's rooms
            drawDungeon( level, enemyDungeon );

            const Point firstCenter = enemyDungeon.roomCenter( 0 );
            const Rect bbox = enemyDungeon.boundingBox();
            const adiktedpp::PlayerType owner = enemyDungeon.owner();

            /// add portal
            const Point portalCenter( bbox.min.x - 8, firstCenter.y );
            const Rect portalRect( portalCenter, 3, 3 );
            level.digLine( firstCenter, portalCenter, owner, true );
            level.setRoom( portalRect, adiktedpp::SlabType::ST_PORTAL, owner, true );

            /// add gold ore
            const Point veinCenter( bbox.max.x + 12, 4 );
            level.digLine( firstCenter, veinCenter );
            const Rect veinRect( veinCenter, 9, 5 );
            drawGoldVein( level, veinRect, 3 );

            /// add other
            level.setCreature( firstCenter.x, firstCenter.y+2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
            level.setCreature( firstCenter.x, firstCenter.y+2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//                    level.setCreature( firstCenter.x, firstCenter.y-2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 1 );

            /// fill treasure with gold
            const std::vector< dkmage::spatial::Room* > treasures = enemyDungeon.findRoom( adiktedpp::SlabType::ST_TREASURE );
            for ( const auto* treasure: treasures ) {
                const Rect& roomRect = treasure->position();
                level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
            }
        }


        /// =======================================================


        static GeneratorRegister<MazeMode> registerMode( "maze" );

    } /* namespace mode */
} /* namespace dkmage */
