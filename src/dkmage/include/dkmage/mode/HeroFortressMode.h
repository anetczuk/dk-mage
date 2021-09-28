/*
 * HeroFortressMode.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_

#include "dkmage/BaseLevelGenerator.h"


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class HeroFortressMode: public BaseLevelGenerator {
        protected:

            utils::Point evilHeartPosition;


        public:

            HeroFortressMode() {
                setMessagesOutput();
            }


        protected:

            bool generate() override;

            bool check() override;

            void generateCaves( const std::size_t cavesNum );

            void preparePlayerDungeon();

            void prepareScript();

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_HEROFORTRESSMODE_H_ */
