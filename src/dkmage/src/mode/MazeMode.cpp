/*
 * MazeMode.cpp
 *
 */

#include "dkmage/mode/MazeMode.h"

#include "dkmage/generator/Maze.h"
#include "dkmage/BaseLevelGenerator.h"


static void drawMaze( adiktedpp::Level& level, dkmage::generator::Maze& maze ) {
    const std::size_t xDimm = maze.dimmensionX();
    const std::size_t yDimm = maze.dimmensionY();

    const utils::Rect mapRect = level.mapRect();
    const utils::Point start = mapRect.center() - utils::Point( xDimm / 2 + 1, yDimm / 2 + 1 );

    const utils::Point end = start + utils::Point( xDimm + 1, yDimm + 1 );
    level.setSlabOutline( utils::Rect( start, end ), adiktedpp::SlabType::ST_ROCK );

    const utils::Point corner = start + utils::Point( 1, 1 );
    for ( std::size_t y=0; y<yDimm; ++y ) {
        for ( std::size_t x=0; x<xDimm; ++x ) {
            const bool val = maze.state( x, y );
            if ( val == false ) {
                /// closed
                level.setSlab( corner + utils::Point( x, y ), adiktedpp::SlabType::ST_ROCK );
            }
//            else {
//                /// open
//                level.setSlab( corner + utils::Point( x, y ), adiktedpp::SlabType::ST_EARTH );
//            }
        }
    }
}


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
                level.generateEmpty();
//                level.generateRandomMap( 9 );

                generator::Maze maze;
                maze.generate( 42, 24 );

                drawMaze( level, maze );

                /// =================

                const bool valid = level.verifyMap();
                LOG() << "is map valid: " << valid;
                if ( valid == false ) {
                    LOG() << "detected invalid map -- restarting generation";
//                    generateLevel();
                    return ;
                }
            }

        };


        /// =======================================================


        MazeMode::MazeMode(): LevelGeneratorWrapper( new MazeGenImplementaton() ) {
        }

    } /* namespace mode */
} /* namespace dkmage */
