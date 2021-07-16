/*
 * MazeMode.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_MAZEMODE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_MAZEMODE_H_

#include "dkmage/BaseLevelGenerator.h"


namespace dkmage {

    namespace spatial {
        class Maze;
    }


    namespace mode {

        /**
         *
         */
        class MazeMode: public BaseLevelGenerator {
        public:

            void generate( const std::size_t seed ) override;


        protected:

            void generateLevel();

            void generateMazeItems( spatial::Maze& maze );

            void randHeroTrap( spatial::Maze& maze, const utils::Point& nodeCenter, const double distanceFactor );

            void preparePlayerDungeon();

            void prepareEnemyDungeon();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_MAZEMODE_H_ */
