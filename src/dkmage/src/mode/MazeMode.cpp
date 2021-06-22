/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/Draw.h"
#include "dkmage/generator/Maze.h"
#include "dkmage/generator/Dungeon.h"
#include "dkmage/BaseLevelGenerator.h"

#include "adiktedpp/Script.h"

#include "utils/Rand.h"


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class MazeGenImplementaton: public BaseLevelGenerator {
        public:

            void generate( const std::size_t seed ) override {
                LOG() << "generating level";
                srand( seed );
                generateLevel();
            }

            void generateLevel() {
//                level.generateEmpty();
                level.generateRandomMap( 9 );

                generator::Maze maze;
                maze.corridorSize = 3;
                maze.generate( 21, 6 );
//                maze.corridorSize = 1;
//                maze.generate( 41, 12 );

                maze.centerizeOnMap();

                drawMaze( level, maze );

                generateMazeTraps( maze );

                {
                    const utils::Rect furthest = maze.getFurthest( 0, 0 );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
                {
                    const utils::Rect furthest = maze.getFurthest( 0, 5 );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
                {
                    const utils::Rect furthest = maze.getFurthest( 20, 0 );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPINCLEV );
                }
                {
                    const utils::Rect furthest = maze.getFurthest( 20, 5 );
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPREVMAP );
                }
                {
                    const utils::Rect furthest = maze.getFurthest();
                    drawSpecial( level, furthest, adiktedpp::SubTypeItem::STI_SPMULTPLY );
                }

                dkmage::generator::Dungeon dungeon;
                dungeon.limitNorth = 1;
                dungeon.limitSouth = 0;
                dungeon.fortify( true );

                dungeon.generate( 1, 5 );
//                dungeon.addRandomRoom( adiktedpp::SlabType::ST_TREASURE, 3, false, 1 );
                dungeon.moveToBottomEdge( 0 );

//                LOG() << "dungeon:\n" << dungeon.print();

                drawDungeon( level, dungeon );

                {
                    const Point firstCenter = dungeon.roomCenter( 0 );
                    const Rect bbox = dungeon.boundingBox();

                    /// add neutral portal
                    const Point portalCenter( bbox.max.x + 16, firstCenter.y );
                    const Rect portalRect( portalCenter, 3, 3 );
                    level.setSlab( portalRect, adiktedpp::SlabType::ST_PORTAL );

                    /// add gold vein
                    const Point veinCenter( bbox.min.x - 20, firstCenter.y - 1 );
                    const Rect veinRect( veinCenter, 9, 5 );
                    drawGoldVein( level, veinRect, 1 );

                    /// add other
                    Point pos = firstCenter + Point(0, 2);
                    level.setItem( pos, 4, adiktedpp::SubTypeItem::STI_SPREVMAP );
                    level.setCreature( firstCenter.x, firstCenter.y-2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
                    level.setCreature( firstCenter.x, firstCenter.y-2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );

//                    /// fill treasure with gold
//                    dkmage::generator::Room* treasure = dungeon.findRoomFirst( adiktedpp::SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
//                    }
                }


            //    dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_GOOD );
                dkmage::generator::Dungeon enemyDungeon( adiktedpp::PlayerType::PT_1 );
                enemyDungeon.limitNorth = 0;
                enemyDungeon.limitSouth = 1;
                enemyDungeon.fortify( true );

//                enemyDungeon.generate( 6, 5 );
                enemyDungeon.generate( 12, 5 );
                enemyDungeon.addRandomRoom( adiktedpp::SlabType::ST_PORTAL, 3 );
                enemyDungeon.moveToTopEdge( 0 );

            //    LOG() << "enemy dungeon:\n" << enemyDungeon.print();

                drawDungeon( level, enemyDungeon );

                {
                    const Point firstCenter = enemyDungeon.roomCenter( 0 );
                    const Rect bbox = enemyDungeon.boundingBox();

                    /// add gold ore
                    const Point veinCenter( bbox.max.x + 12, firstCenter.y + 1 );
                    const Rect veinRect( veinCenter, 9, 5 );
                    drawGoldVein( level, veinRect, 3 );

                    /// add other
                    level.setCreature( firstCenter.x, firstCenter.y+2, 3, adiktedpp::SubTypeCreature::STC_IMP, 4 );
                    level.setCreature( firstCenter.x, firstCenter.y+2, 5, adiktedpp::SubTypeCreature::STC_IMP, 4 );
//                    level.setCreature( firstCenter.x, firstCenter.y-2, 4, adiktedpp::SubTypeCreature::STC_SKELETON, 1 );

//                    /// fill treasure with gold
//                    dkmage::generator::Room* treasure = enemyDungeon.findRoomFirst( adiktedpp::SlabType::ST_TREASURE );
//                    if ( treasure != nullptr ) {
//                        const Rect& roomRect = treasure->position();
//                        level.setItem( roomRect, 4, adiktedpp::SubTypeItem::STI_GLDHOARD3 );
//                    }
                }

                /// === scripting ===

                adiktedpp::Script script( level );

                script.addLine( "SET_GENERATE_SPEED( 500 )" );

                script.addLine( "COMPUTER_PLAYER( PLAYER1, 0 )" );
                script.addLine( "MAX_CREATURES( PLAYER0, 30 )" );
                script.addLine( "MAX_CREATURES( PLAYER1, 50 )" );

                script.addLine( "START_MONEY( PLAYER0, 20000 )" );               /// does not show in treasure
                script.addLine( "START_MONEY( PLAYER1, 100000 )" );              /// does not show in treasure

                script.addLine( "" );
                script.setEvilCreaturesPool( 30 );
                script.addLine( "" );
                script.setEvilCreaturesAvailable( adiktedpp::PlayerType::PT_ALL );
//                script.setEvilCreaturesAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_POSSIBLE );
                script.addLine( "" );
                script.setRoomsStandard();
//                script.setRoomsAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
                script.addLine( "" );
                script.setDoorsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
                script.addLine( "" );
                script.setTrapsAvailable( adiktedpp::PlayerType::PT_ALL, 0 );
                script.addLine( "" );
                script.setMagicStandard( adiktedpp::PlayerType::PT_ALL );
//                script.setMagicAvailable( adiktedpp::PlayerType::PT_ALL, adiktedpp::AvailableMode::AM_ENABLED );
                script.addLine( "" );
//                script.addLine( "MAGIC_AVAILABLE( ALL_PLAYERS, POWER_DESTROY_WALLS, 1, 1 )" );

                script.addLine( "" );
                script.addLine( "REM --- main script ---" );
                script.addLine( "" );
                script.setWinConditionStandard( adiktedpp::PlayerType::PT_0 );

                script.rebuild();

                /// =================

                const bool valid = level.verifyMap();
                if ( valid == false ) {
                    LOG() << "detected invalid map -- restarting generation";
//                    storePreview( "level.bmp" );
                    generateLevel();
                    return ;
                }
            }

            void generateMazeTraps( generator::Maze& maze ) {
                const std::set< adiktedpp::SubTypeTrap >& damageTraps = adiktedpp::DamageTraps();
                const std::size_t trapsSize = damageTraps.size();

                const std::size_t corrNum = maze.corridorsNum();
                const std::size_t trapsChambers = maze.corridorsNum() * 0.3;
                for ( std::size_t i=0; i<trapsChambers; ++i ) {
                    const std::size_t cIndex = rand() % corrNum;
                    const std::size_t nx = cIndex % maze.nodesX();
                    const std::size_t ny = cIndex / maze.nodesX();
                    const utils::Rect nodeRect = maze.nodeRect( nx, ny );
                    const utils::Point nodeCenter = nodeRect.center();

                    const std::size_t trapIndex = rand() % trapsSize;
                    const adiktedpp::SubTypeTrap trapType = utils::randSetItem( damageTraps, trapIndex );

                    if ( maze.corridorSize == 1 ) {
                        level.setSlab( nodeCenter, adiktedpp::SlabType::ST_PATH );
                        level.setTrap( nodeCenter, 4, trapType );
                    } else if ( maze.corridorSize == 3 ) {
                        const std::size_t chamberIndex = rand() % 3;
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
                        }
                    }
                }
            }

        };


        /// =======================================================


        MazeMode::MazeMode(): LevelGeneratorWrapper( new MazeGenImplementaton() ) {
        }

    } /* namespace mode */
} /* namespace dkmage */
