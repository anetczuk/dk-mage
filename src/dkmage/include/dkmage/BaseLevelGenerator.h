/*
 * BaseLevelGenerator.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_
#define DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_

#include "dkmage/LevelGenerator.h"

#include "adiktedpp/Level.h"
#include "adiktedpp/Messages.h"

#include "utils/Log.h"


namespace dkmage {

    /**
     *
     */
    class BaseLevelGenerator: public LevelGenerator {
    public:

        adiktedpp::ScopeMessages messages;
        adiktedpp::Level level;


        BaseLevelGenerator(): messages( "adikted.log.txt" ) {
        }

        void setLevelName( const std::string& name ) override {
            level.getRawLevel().setLevelName( name );
        }

        void setDataPath( const std::string& dataPath ) override {
            level.setDataPath( dataPath );
        }

        void storeLevel( const std::string& levelPath ) override {
            LOG() << "storing level in: " << levelPath;
            const bool saved = level.saveMapByPath( levelPath );
        //    const bool saved = level.saveMapById( 333 );
            LOG() << "is map saved: " << saved;
            if ( saved == false ) {
                return ;
            }
        }

        void storePreview( const std::string& filePath ) override {
            LOG() << "storing bmp in: " << filePath;
            level.setRescale( 3 );
            if ( level.generateBmp( filePath ) == false ) {
                messages.readRecent();
            }
        }

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_ */
