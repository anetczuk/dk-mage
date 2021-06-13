/*
 *
 */

#include "adiktedpp/Level.h"

#include "adiktedpp/Messages.h"
#include "adiktedpp/ThingType.h"

#include "utils/Log.h"

#include <sstream>
#include <iomanip>
///#include <queue>

extern "C" {
    #include "libadikted/adikted.h"
    #include "libadikted/lev_data.h"
    #include "libadikted/draw_map.h"
}


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


    void setTextField( char** fieldPtr, const char* data ) {
        free( *fieldPtr );
        char* desc = (char*) data;
        *fieldPtr = strdup_noquot( desc );
    }


    /// ===============================================================


    Level::Level(): data( new LevelData() ) {
        level_init( &data->lvl, MFV_DKGOLD, NULL );
    }

    Level::~Level() {
        level_free( data->lvl );
        level_deinit( &data->lvl );
    }

    LEVEL* Level::rawData() {
        return data->lvl;
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
        setInfo();
    }

    void Level::generateRandomMap() {
        LEVEL* level = data->lvl;
        free_map( level );
        generate_random_map( level );
        setInfo();
    }

    bool Level::verifyMap( const bool silent ) {
        LEVEL* level = data->lvl;
        struct IPOINT_2D errpt = {-1,-1};
        const short result = level_verify( level, (char*) "check", &errpt );
        const bool ok = ( result == VERIF_OK );
        if ( ok == false ) {
            if ( silent == false ) {
                const char* recentError = Messages::get().getRecent();
                if ( recentError != nullptr ) {
                    LOG() << "map problem found at position: (" << errpt.x << ", " << errpt.y << "): " << recentError;
                } else {
                    LOG() << "map problem found at position: (" << errpt.x << ", " << errpt.y << ")";
                }
            }
            return false;
        }

        if ( countSeparatedAreas() > 0 ) {
            if ( silent == false ) {
                LOG() << "map problem found: inaccessible areas";
            }
            return false;
        }

        return true;
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
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
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
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
//        set_tile_slab( level, x, y, (unsigned short) type );            /// much faster
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
        if ( startX >= MAP_SIZE_DKSTD_X || startY >= MAP_SIZE_DKSTD_Y ) {
            LOG() << "given point is outside map: [" << startX << " " << startY << "]";
            return ;
        }
        if ( endX >= MAP_SIZE_DKSTD_X || endY >= MAP_SIZE_DKSTD_Y ) {
            LOG() << "given point is outside map: [" << endX << " " << endY << "]";
            return ;
        }

        LEVEL* level = data->lvl;
        user_set_slab_rect( level, startX, endX, startY, endY, (unsigned short) type );

//        for (std::size_t x=startX; x<=endX; ++x) {
//            for (std::size_t y=startY; y<=endY; ++y) {
//                setSlab( x, y, type );
//            }
//        }
    }

    void Level::setSlab( const utils::Rect& rect, const SlabType type ) {
        setSlab( rect.min.x, rect.min.y, rect.max.x, rect.max.y, type );
    }

    void Level::setSlabRoom( const std::size_t startX, const std::size_t startY,
                             const std::size_t endX,   const std::size_t endY,
                             const SlabType room, const PlayerType owner )
    {
        if ( startX >= MAP_SIZE_DKSTD_X || startY >= MAP_SIZE_DKSTD_Y ) {
            LOG() << "given point is outside map: [" << startX << " " << startY << "]";
            return ;
        }
        if ( endX >= MAP_SIZE_DKSTD_X || endY >= MAP_SIZE_DKSTD_Y ) {
            LOG() << "given point is outside map: [" << endX << " " << endY << "]";
            return ;
        }

        LEVEL* level = data->lvl;
        user_set_slabown_rect( level, startX, endX, startY, endY, (unsigned short) room, (unsigned short) owner );

//        for (std::size_t x=startX; x<=endX; ++x) {
//            for (std::size_t y=startY; y<=endY; ++y) {
//                setSlab( x, y, room );
//                setOwner( x, y, owner );
//            }
//        }
    }

    void Level::setSlabRoom( const utils::Rect& rect, const SlabType room, const PlayerType owner ) {
        setSlabRoom( rect.min.x, rect.min.y, rect.max.x, rect.max.y, room, owner );
    }

    PlayerType Level::getOwner( const std::size_t x, const std::size_t y ) {
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return PlayerType::PT_UNSET;
        }
        LEVEL* level = data->lvl;
        const PlayerType player = (PlayerType) get_tile_owner( level, x, y );
        return player;
    }

    void Level::setOwner( const std::size_t x, const std::size_t y, const PlayerType owner ) {
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
        user_set_tile_owner( level, x, y, (unsigned short) owner );
    }

    std::string Level::printItems() const {
        std::stringstream stream;

        LEVEL* level = data->lvl;
        const int arr_entries_x = level->tlsize.x * MAP_SUBNUM_X;
        const int arr_entries_y = level->tlsize.y * MAP_SUBNUM_Y;

        std::size_t totalNum = 0;
        for (int i=0; i < arr_entries_y; ++i) {
            for (int j=0; j < arr_entries_x; ++j) {
                const int x = i / MAP_SUBNUM_X;
                const int y = j / MAP_SUBNUM_Y;
                const int sx = i % MAP_SUBNUM_X;
                const int sy = j % MAP_SUBNUM_Y;
                const int ti = sy * MAP_SUBNUM_X + sx;

                const int things_count = get_thing_subnums( level, i, j );
                totalNum += things_count;

                for (int k=0; k<things_count; ++k) {
                    unsigned char* thing = (unsigned char*) get_thing( level, i, j, k );
                    unsigned char type_idx = get_thing_type(thing);
                    unsigned short stype_idx = get_thing_subtype(thing);
                    char* obj_name = get_thing_subtype_fullname( type_idx, stype_idx );

                    const ThingType thingType = (ThingType) type_idx;
                    stream << "[" << x << " " << y << " " << ti << "]: " << thingType << ": " << obj_name << "\n";
                }
            }
        }
        stream << "items counted: " << totalNum << "\n";

        return stream.str();
    }

    void Level::setItem( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeItem item ) {
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
        const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
        const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;
        unsigned char * thing = create_item_adv( level, sx, sy, (unsigned char) item );
        if ( thing == nullptr ) {
            return ;
        }
        thing_add( level, thing );
    }

    void Level::setItem( const utils::Point& point, const std::size_t subIndex, const SubTypeItem item ) {
        setItem( point.x, point.y, subIndex, item );
    }

    void Level::setItem( const utils::Rect& rect, const std::size_t subIndex, const SubTypeItem item ) {
        for ( int x = rect.min.x; x<= rect.max.x; ++x ) {
            for ( int y = rect.min.y; y<= rect.max.y; ++y ) {
                setItem( x, y, subIndex, item );
            }
        }
    }

    void Level::setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeCreature creature, const std::size_t number ) {
        if ( x >= MAP_SIZE_DKSTD_X || y >= MAP_SIZE_DKSTD_Y ) {
            /// out of map
            LOG() << "given point is outside map: [" << x << " " << y << "]";
            return ;
        }
        LEVEL* level = data->lvl;
        const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
        const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;
        for (std::size_t i=0; i<number; ++i) {
            unsigned char * thing = create_creature( level, sx, sy, (unsigned char) creature );
            if ( thing == nullptr ) {
                return ;
            }
            thing_add( level, thing );
        }
    }

    void Level::setCreature( const utils::Point& point, const std::size_t subIndex, const SubTypeCreature creature, const std::size_t number ) {
        setCreature( point.x, point.y, subIndex, creature, number );
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

    void Level::setInfo() {
        setAuthor( "dkmage" );
        setDesciption( "map generated by dkmage -- DK map generator v. 1.0.1" );
    }

    void Level::setAuthor( const std::string& info ) {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return ;
        }
        setTextField( &level->info.author_text, info.c_str() );
        setTextField( &level->info.editor_text, info.c_str() );
    }

    void Level::setDesciption( const std::string& info ) {
        LEVEL* level = data->lvl;
        if ( level == nullptr ) {
            LOG() << "uninitialized level";
            return ;
        }
        setTextField( &level->info.desc_text, info.c_str() );
    }


    struct FillData {
        int data[ MAP_SIZE_DKSTD_X * MAP_SIZE_DKSTD_Y ] = { 0 };
//        int data[ MAP_SIZE_DKSTD_X ][ MAP_SIZE_DKSTD_Y ] = { {0} };

        void set( const std::size_t x, const std::size_t y, const int val ) {
            data[ y * MAP_SIZE_DKSTD_X + x ] = val;
//            data[y][x] = val;
        }

        int get( const std::size_t x, const std::size_t y ) const {
            return data[ y * MAP_SIZE_DKSTD_X + x ];
//            return data[y][x];
        }

        std::size_t fill( const int value ) {
            std::size_t areas = 0;
            for ( std::size_t y=0; y<MAP_SIZE_DKSTD_Y; ++y) {
                for ( std::size_t x=0; x<MAP_SIZE_DKSTD_X; ++x) {
                    if ( get( x, y ) == value ) {
                         /// cell not filled
                        ++areas;
                        fillValue( x, y, value, areas );
                    }
                }
            }
            return areas;
        }

//        void fillValue( const std::size_t startX, const std::size_t startY, const int oldValue, const int newValue ) {
//            std::queue< std::pair<std::size_t, std::size_t> > obj;
//            obj.push( { startX, startY } );
//            while ( obj.empty() == false ) {
//                const std::pair<std::size_t, std::size_t> coord = obj.front();
//                const std::size_t x = coord.first;
//                const std::size_t y = coord.second;
//
//                /// poping front pair of queue
//                obj.pop();
//
//                if ( x >= MAP_SIZE_DKSTD_X ) {
//                    continue ;
//                }
//                if ( y >= MAP_SIZE_DKSTD_Y ) {
//                    continue ;
//                }
//
//                const int preColor = get( x, y );
//
//                if ( preColor != oldValue ) {
//                    continue ;
//                }
//                if ( preColor == newValue ) {
//                    continue ;
//                }
//
//                set( x, y, newValue );
//
//                obj.push( { x + 1, y } );
//                obj.push( { x - 1, y } );
//                obj.push( { x, y + 1 } );
//                obj.push( { x, y - 1 } );
//            }
//        }

        /// recursive solution
        void fillValue( const std::size_t x, const std::size_t y, const int oldValue, const int newValue ) {
            if ( x >= MAP_SIZE_DKSTD_X ) {
                return ;
            }
            if ( y >= MAP_SIZE_DKSTD_Y ) {
                return ;
            }
            if ( get( x, y ) != oldValue ) {
                return ;
            }
            if ( get( x, y ) == newValue ) {
                return ;
            }
            set( x, y, newValue );
            fillValue( x+1, y, oldValue, newValue );
            fillValue( x-1, y, oldValue, newValue );
            fillValue( x, y+1, oldValue, newValue );
            fillValue( x, y-1, oldValue, newValue );
        }
    };


    std::size_t Level::countSeparatedAreas() {
        FillData data;
        /// mark rock
        for ( std::size_t y=0; y<MAP_SIZE_DKSTD_Y; ++y) {
            for ( std::size_t x=0; x<MAP_SIZE_DKSTD_X; ++x) {
                if ( getSlab(x, y) == SlabType::ST_ROCK ) {
                    data.set(x, y, -1 );
                }
//                else {
//                    data.set(x, y, 0 );
//                }
            }
        }

        const std::size_t areas = data.fill( 0 );
        if ( areas == 0 ) {
            return 0;
        }
        return (areas - 1);
    }

} /* namespace adiktedpp */
