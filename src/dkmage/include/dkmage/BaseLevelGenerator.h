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

        ParametersMap parameters;

        adiktedpp::ScopeMessages messages;
        adiktedpp::Level level;


        BaseLevelGenerator(): messages( "adikted.log.txt" ) {
        }

        const ParametersMap& getParameters() const override {
            return parameters;
        }

        void setParameters( const ParametersMap& map ) override {
            parameters = map;
            Optional< std::string > dataPath = parameters.getString( ParameterName::PN_DATA_PATH );
            if ( dataPath.has_value() ) {
                const std::string& path = dataPath.value();
                setDataPath( path );
            }
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
            if ( saved == false ) {
                LOG() << "unable to save map: " << saved;
                return ;
            }
            writeIniFile();
        }

        void storePreview( const std::string& filePath ) override {
            LOG() << "storing bmp in: " << filePath;
            level.setRescale( 3 );
            if ( level.generateBmp( filePath ) == false ) {
                messages.readRecent();
            }
        }

        void generateLevel() override;


    protected:

        /// return 'true' if generation succeeded, otherwise 'false'
        bool generateAttempt();

        /// return 'true' if generation succeeded, otherwise 'false'
        virtual bool generate() = 0;

        /// return 'true' if generated map is correct, otherwise 'false'
        virtual bool check() {
            /// do not perform additional checks by default -- override if needed
            return true;
        }

        void writeIniFile() const;

        void generateLeftGoldVein( const std::size_t goldAmount, const std::size_t gemAmount );

        void generateRightGoldVein( const std::size_t goldAmount, const std::size_t gemAmount );

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_ */
