/*
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_

#include "adiktedpp/raw/RawLevel.h"

#include "adiktedpp/Type.h"


namespace adiktedpp {

    /**
     *
     */
    class Level {

        raw::RawLevel rawLevel;


    public:

        const raw::RawLevel& getRawLevel() const {
            return rawLevel;
        }

        raw::RawLevel& getRawLevel() {
            return rawLevel;
        }

        LEVEL* rawData() {
            return rawLevel.rawData();
        }

        void setDataPath( const std::string& path ) {
            rawLevel.setDataPath( path );
        }

        /// ==============================================================================

        /// map ide template to load: "levels/MAP{mapId}"
        bool loadMapById( const std::size_t mapId ) {
            return rawLevel.loadMapById( mapId );
        }

        /// if relative path is given, then it refers to current working directory or
        /// directory of executable
        /// example of map path: "levels/MAP00001"
        bool loadMapByPath( const std::string& mapPath ) {
            return rawLevel.loadMapByPath( mapPath );
        }

        bool saveMapById( const std::size_t mapId ) const {
            return rawLevel.saveMapById( mapId );
        }

        bool saveMapByPath( const std::string& mapPath ) const {
            return rawLevel.saveMapByPath( mapPath );
        }

        /// ==============================================================================

        void setRescale( const std::size_t rescale ) {
            rawLevel.setRescale( rescale );
        }

        /// saves bmp next to map input file
        void generateBmp() {
            rawLevel.generateBmp();
        }

        bool generateBmp( const std::string& path ) {
            return rawLevel.generateBmp( path );
        }

        /// ==============================================================================

        void startNewMap() {
            rawLevel.startNewMap();
        }

        void generateEmpty() {
            rawLevel.generateEmpty();
        }

        void generateRandomMap() {
            rawLevel.generateRandomMap();
        }

        void generateRandomMap( const std::size_t areaLimit ) {
            return rawLevel.generateRandomMap( areaLimit );
        }

        bool verifyMap( const bool silent = false );

        /// count separated chambers by impassable rocks and gems
        std::size_t countSeparatedAreas() {
            return rawLevel.countSeparatedAreas();
        }

        /// count separated claim areas by impassable rocks, gems, water and lava
        std::size_t countClaimAreas() {
            return rawLevel.countClaimAreas();
        }

        void fillSeparatedAreas( const std::size_t areaLimit ) {
            return rawLevel.fillSeparatedAreas( areaLimit );
        }

        /// ==============================================================================

        std::size_t countSolid( const utils::Point& point, const std::size_t radius ) const;

        std::size_t countSolid( const utils::Rect& rect ) const;

        Slab getSlab( const std::size_t x, const std::size_t y );

        Slab getSlab( const utils::Point& point );

        bool isSlab( const utils::Point& point, const Slab type ) const;

        bool isFortified( const utils::Point& point ) const;

        void setSlab( const std::size_t x, const std::size_t y, const Slab type );

        void setSlab( const utils::Point& point, const Slab type );

        void setSlab( const utils::Rect& rect, const Slab type );

        void setSlab( const std::set< utils::Point >& positions, const Slab type );

        void setSlab( const utils::PointList& positions, const Slab type );

        void setClaimed( const utils::Point& point, const Player owner );

        void setClaimed( const utils::Rect& rect, const Player owner );

        /// set given point a fortified wall
        void setFortified( const utils::Point& point, const Player owner );

        /// set outline of given rect a fortified wall
        void setFortified( const utils::Rect& rect, const Player owner );

        void setRoom( const utils::Rect& position, const Room room );

        void setRoom( const utils::Rect& position, const Room room, const Player owner, const bool fortify );

        void setRoom( const utils::Point& position, const Room room, const Player owner, const bool fortify );

        std::size_t countItems( const utils::Point& point ) const;

        void setTrap( const std::size_t x, const std::size_t y, const Trap trap );

        void setTrap( const utils::Point& point, const Trap trap );

        void setTrap( const utils::Point& point, const std::size_t subIndex, const Trap trap );

        bool canPlaceDoor( const utils::Point& point ) const;

        void setDoor( const std::size_t x, const std::size_t y, const Door door, const bool locked = false );

        void setDoor( const utils::Point& point, const Door door, const bool locked = false );

        void setCreature( const utils::Point& point, const std::size_t subIndex, const Creature creature,
                          const std::size_t number=1, const std::size_t expLevel=0, const Player owner = Player::P_UNSET );

        void setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const Creature creature,
                          const std::size_t number=1, const std::size_t expLevel=0, const Player owner = Player::P_UNSET );

        void setCreatureAuto( const std::size_t x, const std::size_t y, const Creature creature,
                              const std::size_t number=1, const std::size_t expLevel=0, const Player owner = Player::P_UNSET );

        void setCreatureAuto( const utils::Point& point, const Creature creature,
                              const std::size_t number=1, const std::size_t expLevel=0, const Player owner = Player::P_UNSET );

        void setItem( const utils::Point& point, const std::size_t subIndex, const Item item );

        void setItem( const utils::Rect& rect, const std::size_t subIndex, const Item item );

        /// ==============================================================================

        std::size_t addActionPoint( const utils::Point& point, const std::size_t radius = 0 );

        std::size_t addActionPoint( const utils::Point& point, const std::size_t subIndex, const std::size_t radius );

        /// ==============================================================================

        std::size_t setCave( const utils::Rect& rect, const Slab type ) {
            const std::size_t chamberSize = rect.area() * 0.7;
            return setCave( rect, type, chamberSize );
        }

        /// generate random chamber
        std::size_t setCave( const utils::Rect& rect, const Slab type, const std::size_t itemsNum );

        void digLine( const utils::Point& from, const utils::Point& to, const Slab type = Slab::S_PATH );

        void digLine( const utils::Point& from, const utils::Point& to, const Player owner, const bool fortify );

        void digCorridor( const utils::PointList& corridor, const Slab type );

        void digCorridor( const utils::PointList& corridor, const Player owner, const bool fortify );

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_LEVEL_H_ */
