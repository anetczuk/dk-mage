/*
 * RawLevel.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RAWLEVEL_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RAWLEVEL_H_

/// inclusion: #include "adiktedpp/raw/RawLevel.h"

#include "adiktedpp/raw/SlabType.h"
#include "adiktedpp/raw/SubTypeItem.h"
#include "adiktedpp/raw/SubTypeTrap.h"
#include "adiktedpp/raw/SubTypeDoor.h"
#include "adiktedpp/raw/SubTypeCreature.h"
#include "adiktedpp/raw/PlayerType.h"

#include "utils/Rect.h"

#include <memory>


struct LEVEL;


namespace adiktedpp {
    namespace raw {

        struct LevelData;

        /**
         * By default new level is filled with rock slabs.
         */
        class RawLevel {
        public:

            /// aliases for adikted constants
            static const std::size_t MAP_SIZE_X;
            static const std::size_t MAP_SIZE_Y;


        private:

            std::shared_ptr<LevelData> data;                /// shared_ptr needed for shallow copy


        public:

            RawLevel();

            ~RawLevel();

//            /// make move/swap of data
//            RawLevel& operator=( RawLevel& object ) {
//                data.swap( object.data );
//                return *this;
//            }

            LEVEL* rawData();

            /// ===========================================================================

            bool autoUpdateObjects() const;

            void autoUpdateObjects( const bool newVal );

            bool autoUpdateDatclm() const;

            void autoUpdateDatclm( const bool newVal );

            /// ===========================================================================

            /// level name is stored in *.lif file
            void setLevelName( const std::string& name );

            /// returns path of recent attempt to load map (fname)
            std::string inputFileName() const;

            /// returns path of recent successful load of map (savfname)
            std::string outputFileName() const;

            std::string levelsPath() const;

            void setLevelsPath( const std::string& path );

            std::string dataPath() const;

            /// set path to game's 'data' directory containing required assets
            void setDataPath( const std::string& path );

            /// ===========================================================================

            /// map id template to load: "levels/MAP{mapId}"
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

            bool verifyMap( const bool silent = false, const bool skipScriptVerification = false, const bool skipSlabsVerification = false );

            /// count separated chambers by impassable rocks and gems
            std::size_t countSeparatedAreas();

            /// count separated claim areas by impassable rocks, gems, water and lava
            std::size_t countClaimAreas();

            void fillSeparatedAreas( const std::size_t areaLimit );

            /// ===========================================================================

            /// count walls, earth and impassable slabs
            std::size_t countSolid( const utils::Rect& area ) const;

            std::size_t countSlabs( const SlabType slab ) const;

            std::size_t countSlabs( const utils::Rect& area, const SlabType slab ) const;

            SlabType getSlab( const std::size_t x, const std::size_t y ) const;

            SlabType getSlab( const utils::Point& point ) const;

            bool isSlab( const std::size_t x, const std::size_t y, const SlabType slab ) const;

            bool isSlab( const utils::Point& point, const SlabType slab ) const;

            bool isFortified( const utils::Point& point ) const;

            void setSlab( const std::size_t x, const std::size_t y, const SlabType type );

            void setSlab( const utils::Point& point, const SlabType type );

            void setSlab( const std::size_t startX, const std::size_t startY,
                          const std::size_t endX,   const std::size_t endY,
                          const SlabType type );

            void setSlab( const utils::Rect& rect, const SlabType type );

            void setSlab( const utils::Rect& rect, const SlabType room, const PlayerType owner );

            void setSlab( const std::set< utils::Point >& positions, const SlabType type );

            void setSlab( const utils::PointList& positions, const SlabType type );

            void setSlab( const std::size_t x, const std::size_t y, const SlabType type, const PlayerType owner );

            void setSlab( const utils::Point& point, const SlabType type, const PlayerType owner );

            void setSlab( const std::size_t startX, const std::size_t startY,
                          const std::size_t endX,   const std::size_t endY,
                          const SlabType room, const PlayerType owner );

            void setSlabOutline( const utils::Rect& rect, const SlabType type );

            PlayerType getOwner( const std::size_t x, const std::size_t y ) const;

            void setOwner( const std::size_t x, const std::size_t y, const PlayerType owner );

            /// ======================================================================================

            std::string printItems() const;

            std::size_t countItems( const utils::Rect& rect ) const;

            std::size_t countItems( const utils::Point& point ) const;

            std::size_t countItems( const std::size_t x, const std::size_t y ) const;

            unsigned char* setItem( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeItem item );

            unsigned char* setItem( const utils::Point& point, const SubTypeItem item );

            unsigned char* setItem( const utils::Point& point, const std::size_t subIndex, const SubTypeItem item );

            void setItem( const utils::Rect& rect, const std::size_t subIndex, const SubTypeItem item );

            void setTrap( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeTrap trap );

            void setTrap( const std::size_t x, const std::size_t y, const SubTypeTrap trap );

            void setTrap( const utils::Point& point, const std::size_t subIndex, const SubTypeTrap trap );

            void setTrap( const utils::Point& point, const SubTypeTrap trap );

            bool canPlaceDoor( const utils::Point& point ) const;

            void setDoor( const std::size_t x, const std::size_t y, const SubTypeDoor door, const bool locked = false );

            void setDoor( const utils::Point& point, const SubTypeDoor door, const bool locked = false );

            std::size_t countAllCreatures();

            /// 'expLevel' counts from 1
            void setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeCreature creature,
                              const std::size_t number=1, const std::size_t expLevel=1, const PlayerType owner = PlayerType::PT_UNSET );

            /// 'expLevel' counts from 1
            void setCreature( const utils::Point& point, const std::size_t subIndex, const SubTypeCreature creature,
                              const std::size_t number=1, const std::size_t expLevel=1, const PlayerType owner = PlayerType::PT_UNSET );

            /// ===========================================================================

            std::size_t countActionPoints();

            unsigned char* getActionPointByNumber( const std::size_t actionPoint );

            utils::Point getActionPointPosition( const std::size_t actionPoint );

            std::size_t addActionPoint( const std::size_t x, const std::size_t y, const std::size_t subIndex, const std::size_t radius = 0 );

            unsigned char* getHeroGateByNumber( const std::size_t heroGate );

            utils::Point getHeroGatePosition( const std::size_t heroGate );

            void setHeroGateNumber( unsigned char* heroGate, const std::size_t number );

            /// ===========================================================================

            void setRoom( const utils::Rect& position, const SlabType room, const PlayerType owner, const bool fortify );

            void fortify( const utils::Point& point, const PlayerType owner );

            void fortify( const utils::Rect& room, const PlayerType owner );

            /// ===========================================================================

            void setRescale( const std::size_t rescale );

            /// saves bmp next to map input file
            void generateBmp();

            bool generateBmp( const std::string& path );


            static utils::Rect mapSize();

            static utils::Rect mapRect( const std::size_t shrink = 0 );

            static std::string prepareMapName( const std::size_t mapId );

            static std::size_t extractMapIdFromPath( const std::string& mapPath );


        private:

            void setInfo();

            /// author is stored in *.adi file
            void setAuthor( const std::string& info );

            /// description is stored in *.adi file
            void setDesciption( const std::string& info );

        };

    }
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RAWLEVEL_H_ */
