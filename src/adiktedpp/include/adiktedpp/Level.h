/*
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_

#include "adiktedpp/SlabType.h"
#include "adiktedpp/PlayerType.h"
#include "adiktedpp/SubTypeItem.h"
#include "adiktedpp/SubTypeTrap.h"
#include "adiktedpp/SubTypeDoor.h"
#include "adiktedpp/SubTypeCreature.h"

#include "utils/Rect.h"

#include <memory>


struct LEVEL;


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

        LEVEL* rawData();

        /// ===========================================================================

        bool autoUpdateObjects() const;

        void autoUpdateObjects( const bool newVal );

        bool autoUpdateDatclm() const;

        void autoUpdateDatclm( const bool newVal );

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

        /// map ide template to load: "levels/MAP{mapId}"
        bool loadMapById( const std::size_t mapId );

        /// if relative path is given, then it refers to current working directory or
        /// directory of executable
        /// example of map path: "levels/MAP00001"
        bool loadMapByPath( const std::string& mapPath );

        bool saveMapById( const std::size_t mapId ) const;

        bool saveMapByPath( const std::string& mapPath ) const;

        /// ===========================================================================

        void startNewMap();

        void generateEmpty();

        void generateRandomMap();

        void generateRandomMap( const std::size_t areaLimit );

        bool verifyMap( const bool silent = false );

        /// count separated chambers by impassable rocks
        std::size_t countSeparatedAreas();

        void fillSeparatedAreas( const std::size_t areaLimit );

        /// ===========================================================================

        SlabType getSlab( const std::size_t x, const std::size_t y );

        SlabType getSlab( const utils::Point& point );

        bool isSlab( const std::size_t x, const std::size_t y, const SlabType slab );

        bool isSlab( const utils::Point& point, const SlabType slab );

        void setSlab( const std::size_t x, const std::size_t y, const SlabType type );

        void setSlab( const utils::Point& point, const SlabType type );

        void setSlab( const std::size_t startX, const std::size_t startY,
                      const std::size_t endX,   const std::size_t endY,
                      const SlabType type );

        void setSlab( const utils::Rect& rect, const SlabType type );

        void setSlab( const utils::Rect& rect, const SlabType room, const PlayerType owner );

        void setSlab( const std::set< utils::Point >& positions, const SlabType type );

        void setSlab( const std::size_t x, const std::size_t y, const SlabType type, const PlayerType owner );

        void setSlab( const utils::Point& point, const SlabType type, const PlayerType owner );

        void setSlab( const std::size_t startX, const std::size_t startY,
                      const std::size_t endX,   const std::size_t endY,
                      const SlabType room, const PlayerType owner );

        void setSlabOutline( const utils::Rect& rect, const SlabType type );

        PlayerType getOwner( const std::size_t x, const std::size_t y );

        void setOwner( const std::size_t x, const std::size_t y, const PlayerType owner );

        std::string printItems() const;

        void setItem( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeItem item );

        void setItem( const utils::Point& point, const std::size_t subIndex, const SubTypeItem item );

        void setItem( const utils::Rect& rect, const std::size_t subIndex, const SubTypeItem item );

        void setTrap( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeTrap trap );

        void setTrap( const utils::Point& point, const std::size_t subIndex, const SubTypeTrap trap );

        void setDoor( const std::size_t x, const std::size_t y, const SubTypeDoor door, const bool locked = false );

        void setDoor( const utils::Point& point, const SubTypeDoor door, const bool locked = false );

        void setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeCreature creature,
                          const std::size_t number=1, const std::size_t expLevel=0, const PlayerType owner = PlayerType::PT_UNSET );

        void setCreature( const utils::Point& point, const std::size_t subIndex, const SubTypeCreature creature,
                          const std::size_t number=1, const std::size_t expLevel=0, const PlayerType owner = PlayerType::PT_UNSET );

        /// ===========================================================================

        std::size_t setChamber( const utils::Rect& rect, const SlabType type ) {
            const std::size_t chamberSize = rect.area() * 0.7;
            return setChamber( rect, type, chamberSize );
        }

        /// generate random chamber
        std::size_t setChamber( const utils::Rect& rect, const SlabType type, const std::size_t itemsNum );

        void setRoom( const utils::Rect& position, const SlabType room, const PlayerType owner, const bool fortify );

        void fortify( const utils::Point& point, const PlayerType owner );

        void fortify( const utils::Rect& room, const PlayerType owner );

        void digLine( const utils::Point& from, const utils::Point& to );

        void digLine( const utils::Point& from, const utils::Point& to, const PlayerType owner, const bool fortify );

        /// ===========================================================================

        void setRescale( const std::size_t rescale );

        /// saves bmp next to map input file
        void generateBmp();

        bool generateBmp( const std::string& path );


        static utils::Rect mapSize();

        static utils::Rect mapRect();

        static std::string prepareMapName( const std::size_t mapId );


    private:

        void setInfo();

        void setAuthor( const std::string& info );

        void setDesciption( const std::string& info );

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_ */
