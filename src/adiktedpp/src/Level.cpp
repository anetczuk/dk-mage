/*
 *
 */

#include "adiktedpp/Level.h"

#include "adiktedpp/Log.h"

#include <sstream>
#include <iomanip>

extern "C" {
    #include "libadikted/adikted.h"
    #include "libadikted/draw_map.h"
}


namespace adiktedpp {

    struct LevelData {
        struct LEVEL *lvl;
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
        if ( data->lvl == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = data->lvl->fname;
        if ( path == nullptr ) {
//            LOG() << "uninitialized fname";
            return "";
        }
        return std::string( path );
    }

    std::string Level::outputFileName() const {
        if ( data->lvl == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = data->lvl->savfname;
        if ( path == nullptr ) {
//            LOG() << "uninitialized savfname";
            return "";
        }
        return std::string( path );
    }

    std::string Level::levelsPath() const {
        if ( data->lvl == nullptr ) {
            LOG() << "uninitialized level";
            return "";
        }
        const char* path = data->lvl->optns.levels_path;
        if ( path == nullptr ) {
//            LOG() << "uninitialized levels_path";
            return "";
        }
        return std::string( path );
    }

    void Level::setLevelsPath( const std::string& path ) {
        char* levelPath = (char*)path.c_str();
        set_levels_path( data->lvl, levelPath );
    }

    std::string Level::dataPath() const {
        if ( data->lvl == nullptr ) {
//            LOG() << "uninitialized level";
            return "";
        }
        const char* path = data->lvl->optns.data_path;
        if ( path == nullptr ) {
//            LOG() << "uninitialized data_path";
            return "";
        }
        return std::string( path );
    }

    void Level::startNewMap() {
        free_map( data->lvl );
        start_new_map( data->lvl );
    }

    bool Level::loadMapById( const std::size_t mapId ) {
        std::stringstream id;
        /// example format: "Levels/MAP00001"
        id << "Levels/MAP" << std::setw(5) << std::setfill('0') << mapId;
        const std::string& idString = id.str();
//        LOG() << idString;
        return loadMapByPath( idString );
    }

    bool Level::loadMapByPath( const std::string& mapId ) {
        /// setting file name of the map to load
        char* levelName = (char*)mapId.c_str();
        format_lvl_fname( data->lvl, levelName );

        /// loading map
        short result = user_load_map( data->lvl, 0 );
        if (result == ERR_NONE) {
            return true;
        }

//        LOG() << "cannot load map, error: " << result;

        // The following two commands should be used to free memory
        // allocated for level
//        level_free( data->lvl );
//        level_deinit( &data->lvl );

        // This command should be always last function used from library
//            free_messages();
        return false;
    }

    void Level::generateBmp() {
        generate_map_bitmap_mapfname( data->lvl );
    }

} /* namespace adiktedpp */
