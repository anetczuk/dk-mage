/*
 * Cave.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_

#include "dkmage/LevelGenerator.h"

///#include <memory>


namespace dkmage {
    namespace mode {

        class Cave: public LevelGenerator {

//            std::unique_ptr<CaveData> data;


        public:

            Cave();

            void generate( const std::size_t seed ) override;

            /**
             * "levelPath" is path without extension, e.g. "map00333" or "path/to/map/file"
             * in both cases various files will be created by adding extension to "levelPath"
             */
            void storeLevel( const std::string& levelPath ) override;

            void storePreview( const std::string& filePath ) override;

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_ */
