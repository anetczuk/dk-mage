/*
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_

#include "adiktedpp/SlabType.h"
#include "adiktedpp/PlayerType.h"

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

        /// ===========================================================================

        /// returns path of recent attempt to load map (fname)
        std::string inputFileName() const;

        /// returns path of recent successful load of map (savfname)
        std::string outputFileName() const;

        std::string levelsPath() const;

        void setLevelsPath( const std::string& path );

        std::string dataPath() const;

        void setDataPath( const std::string& path );

        /// ===========================================================================

        /// map ide template to load: "Levels/MAP{mapId}"
        bool loadMapById( const std::size_t mapId );

        /// if relative path is given, then it refers to current working directory or
        /// directory of executable
        /// example of map path: "Levels/MAP00001"
        bool loadMapByPath( const std::string& mapPath );

        bool saveMapById( const std::size_t mapId ) const;

        bool saveMapByPath( const std::string& mapPath ) const;

        /// ===========================================================================

        void startNewMap();

        void generateRandomMap();

        bool verifyMap();

        SlabType getSlab( const std::size_t x, const std::size_t y );

        void setSlab( const std::size_t x, const std::size_t y, const SlabType type );

        void setSlab( const std::size_t startX, const std::size_t startY,
                      const std::size_t endX,   const std::size_t endY,
                      const SlabType type );

        void setSlabRoom( const std::size_t startX, const std::size_t startY,
                          const std::size_t endX,   const std::size_t endY,
                          const SlabType room, const PlayerType owner );

        PlayerType getOwner( const std::size_t x, const std::size_t y );

        void setOwner( const std::size_t x, const std::size_t y, const PlayerType owner );

        /// ===========================================================================

        void setRescale( const std::size_t rescale );

        /// saves bmp next to map input file
        void generateBmp();

        void generateBmp( const std::string& path );

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_ */
