/*
 * BaseLevelGenerator.h
 *
 */

#ifndef DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_
#define DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_

#include "dkmage/LevelGenerator.h"

#include "adiktedpp/GameMap.h"
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
        adiktedpp::GameMap map;


        BaseLevelGenerator();

        const ParametersMap& getParameters() const override {
            return parameters;
        }

        void setParameters( const ParametersMap& paramsMap ) override {
            parameters = paramsMap;
            Optional< std::string > dataPath = parameters.getString( ParameterName::PN_DATA_PATH );
            if ( dataPath.has_value() ) {
                const std::string& path = dataPath.value();
                setDataPath( path );
            }

            const std::string type = parameters.getString( ParameterName::PN_TYPE, "" );
            const std::string seed = parameters.getString( ParameterName::PN_SEED, "" );
            map.script.storeParameters( type, seed );
        }

        void setLevelName( const std::string& name ) override {
            map.level.getRawLevel().setLevelName( name );
        }

        void setDataPath( const std::string& dataPath ) override {
            map.level.setDataPath( dataPath );
        }

        void storeLevel( const std::string& levelPath ) override {
            LOG_INFO() << "storing level in: " << levelPath;
            const bool saved = map.level.saveMapByPath( levelPath );
        //    const bool saved = level.saveMapById( 333 );
            if ( saved == false ) {
                LOG_INFO() << "unable to save map: " << saved;
                return ;
            }
            writeIniFile();
        }

        void storePreview( const std::string& filePath ) override {
            LOG_INFO() << "storing bmp in: " << filePath;
            map.level.setRescale( 3 );
            if ( map.level.generateBmp( filePath ) == false ) {
                messages.readRecent();
            }
        }

        void generateLevel() override;


    protected:

        /// return 'true' if generation succeeded, otherwise 'false'
        bool generateAttempt();

        bool checkScriptLimits() const;

        /// return 'true' if generation succeeded, otherwise 'false'
        virtual bool generate() = 0;

        /// return 'true' if generated map is correct, otherwise 'false'
        virtual bool check() {
            /// do not perform additional checks by default -- override if needed
            return true;
        }

        void writeIniFile() const;

        void generateGoldSlabs( const std::string& defaultGoldNum,
                                const std::string& defaultGemSlabsNum, const std::string& defaultGemFacesNum, const std::string& defaultGemTrapsNum,
                                const std::size_t gemRegionWidth, const std::size_t gemRegionHeight );

        void generateLeftGoldVein( const std::size_t goldAmount, const std::size_t gemAmount );

        void generateRightGoldVein( const std::size_t goldAmount, const std::size_t gemAmount );

    };

} /* namespace dkmage */

#endif /* DKMAGE_INCLUDE_DKMAGE_BASELEVELGENERATOR_H_ */
