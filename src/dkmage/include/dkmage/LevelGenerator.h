/*
 * LevelGenerator.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_
#define DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_

/// inclusion: #include "dkmage/LevelGenerator.h"

#include <string>


namespace dkmage {

    /**
     *
     */
    class LevelGenerator {
    public:

        virtual ~LevelGenerator() {
        }

        void generate( const std::string& seed );

        virtual void generate( const std::size_t seed ) = 0;

        /**
         * "levelPath" is path without extension, e.g. "map00333" or "path/to/map/file"
         * in both cases various files will be created by adding extension to "levelPath"
         */
        virtual void storeLevel( const std::string& levelPath ) = 0;

        virtual void storePreview( const std::string& filePath ) = 0;

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_ */
