/*
 * Cave.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_
#define DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_

#include <string>


namespace dkmage {
    namespace mode {

        class Cave {
        public:

            Cave();

            void generate( const std::size_t seed );

            /**
             * "levelPath" is path without extension, e.g. "map00333" or "path/to/map/file"
             * in both cases various files will be created by adding extension to "levelPath"
             */
            void saveLevel( const std::string& levelPath );

            void saveBmp( const std::string& outputFile );

        };

    } /* namespace mode */
} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_MODE_CAVE_H_ */
