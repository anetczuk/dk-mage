/*
 * LevelGenerator.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_
#define DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_

/// inclusion: #include "dkmage/LevelGenerator.h"

#include <string>
#include <memory>


namespace dkmage {

    /**
     *
     */
    class LevelGenerator {
    public:

        virtual ~LevelGenerator() {
        }

        virtual void setLevelName( const std::string& name ) = 0;

        virtual void setDataPath( const std::string& dataPath ) = 0;

        virtual void generateLevel() = 0;

        /**
         * "levelPath" is path without extension, e.g. "map00333" or "path/to/map00333"
         * in both cases various files will be created by adding extension to "levelPath"
         */
        virtual void storeLevel( const std::string& levelPath ) = 0;

        virtual void storePreview( const std::string& filePath ) = 0;

    };


    /**
     *
     */
    class EmptyLevelGenerator: public LevelGenerator {
    public:

        void setLevelName( const std::string& /*name*/ ) override {
            /// do nothing
        }

        void setDataPath( const std::string& /*dataPath*/ ) override {
            /// do nothing
        }

        void generateLevel() override {
            /// do nothing
        }

        void storeLevel( const std::string& /*levelPath*/ ) override {
            /// do nothing
        }

        void storePreview( const std::string& /*filePath*/ ) override {
            /// do nothing
        }

    };


    /**
     *
     */
    class LevelGeneratorWrapper: public LevelGenerator {

        std::unique_ptr< LevelGenerator > data;


    public:

        LevelGeneratorWrapper( LevelGenerator* generator ): data( generator ) {
        }

        void setLevelName( const std::string& name ) override {
            LevelGenerator* implementation = data.get();
            implementation->setLevelName( name );
        }

        void setDataPath( const std::string& dataPath ) override {
            LevelGenerator* implementation = data.get();
            implementation->setDataPath( dataPath );
        }

        void generateLevel() override {
            LevelGenerator* implementation = data.get();
            implementation->generateLevel();
        }

        void storeLevel( const std::string& levelPath ) override {
            LevelGenerator* implementation = data.get();
            implementation->storeLevel( levelPath );
        }

        void storePreview( const std::string& filePath ) override {
            LevelGenerator* implementation = data.get();
            implementation->storePreview( filePath );
        }

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_LEVELGENERATOR_H_ */
