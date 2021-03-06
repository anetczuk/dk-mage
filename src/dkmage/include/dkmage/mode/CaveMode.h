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
        public:

            CaveMode() {
                setMessagesOutput();
            }


        protected:

            bool generate() override;

            void generateCaves();

            void preparePlayerDungeon();

            void prepareEnemyDungeon();

            void prepareScript();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_CAVEMODE_H_ */
