/*
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_

#include <memory>


namespace adiktedpp {

    struct LevelData;


    /**
     *
     */
    class Level {

        std::unique_ptr<LevelData> data;


    public:

        Level();

        ~Level();

        /// returns path of recent attempt to load map (fname)
        std::string inputFileName() const;

        /// returns path of recent successful load of map (savfname)
        std::string outputFileName() const;

        std::string levelsPath() const;

        void setLevelsPath( const std::string& path );

        std::string dataPath() const;

        void startNewMap();

        /// map ide template to load: "Levels/MAP{mapId}"
        bool loadMapById( const std::size_t mapId );

        /// if relative path is given, then it refers to current working directory or
        /// directory of executable
        /// example of map ID: "Levels/MAP00001"
        bool loadMapByPath( const std::string& mapId );

        //TODO: user_set_slab

        void generateBmp();

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_ */
