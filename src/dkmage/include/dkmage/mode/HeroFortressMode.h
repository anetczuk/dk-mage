/*
 * HeroFortressMode.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_

#include "dkmage/BaseLevelGenerator.h"


namespace dkmage {

    namespace spatial {
        class Maze;
    }


    namespace mode {

        /**
         *
         */
        class HeroFortressMode: public BaseLevelGenerator {
        public:

            void generateLevel() override;


        protected:

//            void generateMazeItems( spatial::Maze& maze );
//
//            void randHeroTrap( spatial::Maze& maze, const utils::Point& nodeCenter, const double distanceFactor );

            void preparePlayerDungeon();

            void prepareEnemyDungeon();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_ */
