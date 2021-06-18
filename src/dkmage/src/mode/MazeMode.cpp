/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/Draw.h"
#include "dkmage/generator/Maze.h"
#include "dkmage/generator/Dungeon.h"
#include "dkmage/BaseLevelGenerator.h"


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
                maze.generate( 41, 12 );

                drawMaze( level, maze );

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

                /// =================

                const bool valid = level.verifyMap();
                if ( valid == false ) {
                    LOG() << "detected invalid map -- restarting generation";
                    generateLevel();
                    return ;
                }
            }

        };


        /// =======================================================


        MazeMode::MazeMode(): LevelGeneratorWrapper( new MazeGenImplementaton() ) {
        }

    } /* namespace mode */
} /* namespace dkmage */
