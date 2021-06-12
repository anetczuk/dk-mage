/*
 * Cave.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_

#include "dkmage/LevelGenerator.h"

#include <memory>


namespace dkmage {
    namespace mode {

        /**
         *
         */
        class Cave: public LevelGenerator {

            std::unique_ptr< LevelGenerator > data;


        public:

            Cave();

            void setDataPath( const std::string& dataPath ) override;

            void generate( const std::size_t seed ) override;

            void storeLevel( const std::string& levelPath ) override;

            void storePreview( const std::string& filePath ) override;

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_ */
