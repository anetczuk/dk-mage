/*
 * Maze.cpp
 *
 */

#include "dkmage/mode/Maze.h"

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
                //TODO: finish
                LOG() << "maze not implemented";
            }

        };


        /// =======================================================


        Maze::Maze(): LevelGeneratorWrapper( new MazeGenImplementaton() ) {
        }

    } /* namespace mode */
} /* namespace dkmage */
