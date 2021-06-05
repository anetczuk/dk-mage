/*
 *
 */

#include "adiktedpp/Level.h"

#include "adiktedpp/Messages.h"

#include "utils/Log.h"

#include <sstream>
#include <iomanip>

extern "C" {
    #include "libadikted/adikted.h"
    #include "libadikted/lev_data.h"
    #include "libadikted/draw_map.h"
}


#define MAP_MAX_COORD   84


namespace adiktedpp {

    struct LevelData {
        LEVEL* lvl;


        const char* getDataPath() const {
            if ( lvl == nullptr ) {
                return nullptr;
            }
            return lvl->optns.data_path;
        }

        char* getDataPath() {
            if ( lvl == nullptr ) {
                return nullptr;
            }
            return lvl->optns.data_path;
        }
    };


    /// ===============================================================


    Level::Level(): data( new LevelData() ) {
        level_init( &data->lvl, MFV_DKGOLD, NULL );
    }

    Level::~Level() {
        level_free( data->lvl );
        level_deinit( &data->lvl );
    }

    std::string Level::inputFileName() const {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = level->fname;
        if ( path == nullptr ) {
//            LOG() << "uninitialized fname";
            return "";
        }
        return std::string( path );
    }

    std::string Level::outputFileName() const {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = level->savfname;
        if ( path == nullptr ) {
//            LOG() << "uninitialized savfname";
            return "";
        }
        return std::string( path );
    }

    std::string Level::levelsPath() const {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = level->optns.levels_path;
        if ( path == nullptr ) {
//            LOG() << "uninitialized levels_path";
            return "";
        }
        return std::string( path );
    }

    void Level::setLevelsPath( const std::string& path ) {
        LEVEL* level = data->lvl;
        char* levelPath = (char*)path.c_str();
        set_levels_path( level, levelPath );
    }

    std::string Level::dataPath() const {
        const char* path = data->getDataPath();
        if ( path == nullptr ) {
//            LOG() << "uninitialized data_path";
            return "";
        }
        return std::string( path );
    }

    void Level::setDataPath( const std::string& path ) {
        LEVEL* level = data->lvl;
        char* dataPath = (char*)path.c_str();
        set_data_path( level, dataPath );
    }

    void Level::startNewMap() {
        LEVEL* level = data->lvl;
        free_map( level );
        start_new_map( level );
    }

    void Level::generateRandomMap() {
        LEVEL* level = data->lvl;
        free_map( level );
        generate_random_map( level );
    }

    bool Level::verifyMap() {
        LEVEL* level = data->lvl;
        struct IPOINT_2D errpt = {-1,-1};
        const short result = level_verify( level, (char*) "check", &errpt );
        const bool ok = ( result == VERIF_OK );
        if ( ok == false ) {
            const char* recentError = Messages::get().getRecent();
            if ( recentError != nullptr ) {
                LOG() << "map problem found at position: (" << errpt.x << ", " << errpt.y << "): " << recentError;
            } else {
                LOG() << "map problem found at position: (" << errpt.x << ", " << errpt.y << ")";
            }
        }
        return ok;
    }

    bool Level::loadMapById( const std::size_t mapId ) {
        std::stringstream id;
        /// example format: "Levels/MAP00001"
        id << "Levels/MAP" << std::setw(5) << std::setfill('0') << mapId;
        const std::string& idString = id.str();
//        LOG() << idString;
        return loadMapByPath( idString );
    }

    bool Level::loadMapByPath( const std::string& mapPath ) {
        /// setting file name of the map to load
        LEVEL* level = data->lvl;
        char* levelName = (char*)mapPath.c_str();
        format_lvl_fname( level, levelName );

        /// loading map
        const short result = user_load_map( level, 0 );
        if (result == ERR_NONE) {
            return true;
        }

        return false;
    }

    bool Level::saveMapById( const std::size_t mapId ) const {
        std::stringstream id;
        /// example format: "Levels/MAP00001"
        id << "Levels/MAP" << std::setw(5) << std::setfill('0') << mapId;
        const std::string& idString = id.str();
//        LOG() << idString;
        return saveMapByPath( idString );
    }

    bool Level::saveMapByPath( const std::string& mapPath ) const {
        LEVEL* level = data->lvl;
        char* levelName = (char*)mapPath.c_str();
        format_lvl_savfname( level, levelName );

        const short result = user_save_map( level, 0 );
        if (result != ERR_NONE) {
            const char* recentError = Messages::get().getRecent();
            LOG() << recentError;
            return false;
        }
        return true;
    }

    /// ============================================================

    SlabType Level::getSlab( const std::size_t x, const std::size_t y ) {
        if ( x > MAP_MAX_COORD || y > MAP_MAX_COORD ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return SlabType::ST_ROCK;
        }
        LEVEL* level = data->lvl;
        const SlabType slab = (SlabType) get_tile_slab( level, x, y );
        return slab;
    }

    SlabType Level::getSlab( const utils::Point& point ) {
        return getSlab( point.x, point.y );
    }

    bool Level::isSlab( const std::size_t x, const std::size_t y, const SlabType slab ) {
        const SlabType currSlab = getSlab( x, y );
        return ( currSlab == slab );
    }

    bool Level::isSlab( const utils::Point& point, const SlabType slab ) {
        const SlabType currSlab = getSlab( point.x, point.y );
        return ( currSlab == slab );
    }

    void Level::setSlab( const std::size_t x, const std::size_t y, const SlabType type ) {
        if ( x > MAP_MAX_COORD || y > MAP_MAX_COORD ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
        user_set_slab( level, x, y, (unsigned short) type );
    }

    void Level::setSlab( const utils::Point& point, const SlabType type ) {
        setSlab( point.x, point.y, type );
    }

    void Level::setSlab( const std::size_t x, const std::size_t y, const SlabType type, const PlayerType owner ) {
        setSlab( x, y, type );
        setOwner( x, y, owner );
    }

    void Level::setSlab( const utils::Point& point, const SlabType type, const PlayerType owner ) {
        setSlab( point.x, point.y, type );
        setOwner( point.x, point.y, owner );
    }

    void Level::setSlab( const std::size_t startX, const std::size_t startY,
                         const std::size_t endX,   const std::size_t endY,
                         const SlabType type )
    {
        if ( startX > MAP_MAX_COORD || startY > MAP_MAX_COORD ) {
            LOG() << "given point is outside map: [" << startX << " " << startY << "]";
            return ;
        }
        if ( endX > MAP_MAX_COORD || endY > MAP_MAX_COORD ) {
            LOG() << "given point is outside map: [" << endX << " " << endY << "]";
            return ;
        }

//        /// does not seem to work (buggy?)
//        LEVEL* level = data->lvl;
//        user_set_slab_rect( level, startX, startY, endX, endY, (unsigned short) type );

        for (std::size_t x=startX; x<=endX; ++x) {
            for (std::size_t y=startY; y<=endY; ++y) {
                setSlab( x, y, type );
            }
        }
    }

    void Level::setSlab( const utils::Rect& rect, const SlabType type ) {
        setSlab( rect.min.x, rect.min.y, rect.max.x, rect.max.y, type );
    }

    void Level::setSlabRoom( const std::size_t startX, const std::size_t startY,
                             const std::size_t endX,   const std::size_t endY,
                             const SlabType room, const PlayerType owner )
    {
        if ( startX > MAP_MAX_COORD || startY > MAP_MAX_COORD ) {
            LOG() << "given point is outside map: [" << startX << " " << startY << "]";
            return ;
        }
        if ( endX > MAP_MAX_COORD || endY > MAP_MAX_COORD ) {
            LOG() << "given point is outside map: [" << endX << " " << endY << "]";
            return ;
        }

//        /// does not seem to work (buggy?)
//        LEVEL* level = data->lvl;
//        user_set_slabown_rect( level, startX, startY, endX, endY, (unsigned short) room, (unsigned short) owner );

        for (std::size_t x=startX; x<=endX; ++x) {
            for (std::size_t y=startY; y<=endY; ++y) {
                setSlab( x, y, room );
                setOwner( x, y, owner );
            }
        }
    }

    void Level::setSlabRoom( const utils::Rect& rect, const SlabType room, const PlayerType owner ) {
        setSlabRoom( rect.min.x, rect.min.y, rect.max.x, rect.max.y, room, owner );
    }

    PlayerType Level::getOwner( const std::size_t x, const std::size_t y ) {
        if ( x > MAP_MAX_COORD || y > MAP_MAX_COORD ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return PlayerType::PT_UNSET;
        }
        LEVEL* level = data->lvl;
        const PlayerType player = (PlayerType) get_tile_owner( level, x, y );
        return player;
    }

    void Level::setOwner( const std::size_t x, const std::size_t y, const PlayerType owner ) {
        if ( x > MAP_MAX_COORD || y > MAP_MAX_COORD ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
        user_set_tile_owner( level, x, y, (unsigned short) owner );
    }

    /// ============================================================

    void Level::setRescale( const std::size_t rescale ) {
        LEVEL* level = data->lvl;
        level->optns.picture.rescale = rescale;
    }

    void Level::generateBmp() {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return ;
        }
        const char* dataPath = data->getDataPath();
        if ( dataPath == nullptr ) {
            /// data path is not set -- initialize with empty string
            setDataPath( "" );
        }
        generate_map_bitmap_mapfname( level );
    }

    bool Level::generateBmp( const std::string& path ) {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return false;
        }
        const char* dataPath = data->getDataPath();
        if ( dataPath == nullptr ) {
            /// data path is not set -- initialize with empty string
            setDataPath( "" );
        }

        level->optns.picture.data_path = level->optns.data_path;
        const char *bmpfname = path.c_str();
        ///level->optns.picture.tngflags = TNGFLG_SHOW_CIRCLES;
//        level->optns.picture.rescale = 2;
        const short result = generate_map_bitmap( bmpfname, level, &(level->optns.picture));
        if ( result != ERR_NONE ) {
            return false;
        }
        message_info("Bitmap \"%s\" created.",bmpfname);
        return true;
    }

} /* namespace adiktedpp */
