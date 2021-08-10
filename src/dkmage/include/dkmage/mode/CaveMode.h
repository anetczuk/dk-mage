/*
 * CaveMode.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_CAVEMODE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_CAVEMODE_H_

#include "dkmage/BaseLevelGenerator.h"


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class CaveMode: public BaseLevelGenerator {
        protected:

            bool generate() override;

            void generateCaves( const std::size_t cavesNum );

            void preparePlayerDungeon();

            void prepareEnemyDungeon();

            void prepareScript();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_CAVEMODE_H_ */
