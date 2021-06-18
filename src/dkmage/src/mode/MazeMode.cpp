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
                maze.generate( 41, 20 );

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
