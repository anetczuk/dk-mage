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

        void setDataPath( const std::string& path );

        void startNewMap();

        /// map ide template to load: "Levels/MAP{mapId}"
        bool loadMapById( const std::size_t mapId );

        /// if relative path is given, then it refers to current working directory or
        /// directory of executable
        /// example of map path: "Levels/MAP00001"
        bool loadMapByPath( const std::string& mapPath );

        bool saveMapById( const std::size_t mapId ) const;

        bool saveMapByPath( const std::string& mapPath ) const;

        //TODO: user_set_slab

        /// saves bmp next to map input file
        void generateBmp();

        void generateBmp( const std::string& path );

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_ */
