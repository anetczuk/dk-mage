/*
 * RawLevel.cpp
 *
 */

#include "adiktedpp/raw/RawLevel.h"

#include "adiktedpp/AreaDetector.h"
#include "adiktedpp/Messages.h"
#include "adiktedpp/raw/ThingType.h"
#include "adiktedpp/Version.h"

#include "utils/Log.h"

#include <sstream>
#include <iomanip>


extern "C" {
    #include "libadikted/adikted.h"
    #include "libadikted/lev_data.h"
    #include "libadikted/draw_map.h"
}


using namespace utils;


namespace adiktedpp {
    namespace raw {

        /// taken from "libadikted/lev_files.c"
        static const char* find_last( const char *buffer, const char item )
        {
            const char *found = strrchr( buffer, item );
            if ( found != NULL ) {
                return found;
            }
            return buffer;
        }

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


        const std::size_t RawLevel::MAP_SIZE_X = MAP_SIZE_DKSTD_X;
        const std::size_t RawLevel::MAP_SIZE_Y = MAP_SIZE_DKSTD_Y;


        RawLevel::RawLevel(): data( new LevelData() ) {
            level_init( &data->lvl, MFV_DKGOLD, NULL );
            setInfo();
        }

        RawLevel::~RawLevel() {
            level_free( data->lvl );
            level_deinit( &data->lvl );
        }

        LEVEL* RawLevel::rawData() {
            return data->lvl;
        }

        /// ===========================================================================

        bool RawLevel::autoUpdateObjects() const {
            return get_obj_auto_update( data->lvl );
        }
        void RawLevel::autoUpdateObjects( const bool newVal ) {
            set_obj_auto_update( data->lvl, newVal );
        }

        bool RawLevel::autoUpdateDatclm() const {
            return get_datclm_auto_update( data->lvl );
        }
        void RawLevel::autoUpdateDatclm( const bool newVal ) {
            set_datclm_auto_update( data->lvl, newVal );
        }

        /// ===========================================================================

        void RawLevel::setLevelName( const std::string& name ) {
            const char* rawName = name.c_str();
            char* str_array = strdup( rawName );                   /// will be freed by libadikted internals
            set_lif_name_text( data->lvl, str_array );
        }

        std::string RawLevel::inputFileName() const {
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

        std::string RawLevel::outputFileName() const {
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

        std::string RawLevel::levelsPath() const {
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

        void RawLevel::setLevelsPath( const std::string& path ) {
            LEVEL* level = data->lvl;
            char* levelPath = (char*)path.c_str();
            set_levels_path( level, levelPath );
        }

        std::string RawLevel::dataPath() const {
            const char* path = data->getDataPath();
            if ( path == nullptr ) {
    //            LOG() << "uninitialized data_path";
                return "";
            }
            return std::string( path );
        }

        void RawLevel::setDataPath( const std::string& path ) {
            LEVEL* level = data->lvl;
            char* dataPath = (char*)path.c_str();
            set_data_path( level, dataPath );
        }

        Rect RawLevel::mapSize() {
            return Rect( 0, 0, MAP_SIZE_X - 1, MAP_SIZE_Y - 1 );
        }

        Rect RawLevel::mapRect( const std::size_t shrink ) {
            return Rect( 1 + shrink, 1 + shrink, MAP_SIZE_X - 2 - shrink, MAP_SIZE_Y - 2 - shrink );
        }

        std::string RawLevel::prepareMapName( const std::size_t mapId ) {
            if ( mapId > 99999 || mapId == 0 ) {
                std::stringstream stream;
                stream << FILE_NAME << ": invalid argument -- map id outside range [1, 99999]: " << mapId;
                throw std::invalid_argument( stream.str() );
            }
            std::stringstream name;
            name << "map" << std::setw(5) << std::setfill('0') << mapId;
            return name.str();
        }

        std::size_t RawLevel::extractMapIdFromPath( const std::string& mapPath ) {
            /// taken from "libadikted/lev_files.c"

            /*Acquiring map number */
            const char *fname_num = mapPath.c_str();

            /* Separating filename from directory subpath */
            fname_num = find_last( fname_num, '/' );                 /* Linux path separator */
            fname_num = find_last( fname_num, '\\' );                /* Windows path separator */

            while ( ((*fname_num)!='\0') && (!isdigit(*fname_num)) ) fname_num++;

            const long lvl_num = atol(fname_num);
            return lvl_num;
        }

        void RawLevel::startNewMap() {
            LEVEL* level = data->lvl;
            free_map( level );
            start_new_map( level );
            setInfo();
        }

        void RawLevel::generateEmpty() {
            Rect rect( 0, 0, MAP_SIZE_X-1, MAP_SIZE_Y-1 );
            setSlabOutline( rect, SlabType::ST_ROCK );
            rect.grow( -1 );
            setSlab( rect, SlabType::ST_EARTH );
        }

        void RawLevel::generateRandomMap() {
            LEVEL* level = data->lvl;
            free_map( level );
            generate_random_map( level );
            setInfo();
        }

        void RawLevel::generateRandomMap( const std::size_t areaLimit ) {
            while ( true ) {
                generateRandomMap();
                fillSeparatedAreas( areaLimit );
                if ( countSeparatedAreas() < 1 ) {
                    break ;
                }
                LOG_WARN() << "found inaccessible areas -- restarting generation";
            }
        }

        bool RawLevel::verifyMap( const bool silent, const bool skipScriptVerification, const bool skipSlabsVerification ) {
            LEVEL* level = data->lvl;
            struct IPOINT_2D errpt = {-1,-1};

            unsigned long skip_steps = 0;
            if ( skipScriptVerification ) {
                skip_steps |= VSF_TXT;
            }
            if ( skipSlabsVerification ) {
                skip_steps |= VSF_SLABS;
            }

            short result = level_verify_control( level, (char*) "check", skip_steps, &errpt );
            if ( result != VERIF_OK ) {
                if ( silent == false ) {
                    const char* recentError = Messages::get().getRecent();
                    if ( recentError != nullptr ) {
                        LOG_WARN() << "map problem found at position: (" << errpt.x << ", " << errpt.y << "): " << recentError;
                    } else {
                        LOG_WARN() << "map problem found at position: (" << errpt.x << ", " << errpt.y << ")";
                    }
                }
                return false;
            }

            if ( countSeparatedAreas() > 0 ) {
                if ( silent == false ) {
                    LOG_WARN() << "map problem found: inaccessible areas";
                }
                return false;
            }

            return true;
        }

        bool RawLevel::loadMapById( const std::size_t mapId ) {
            const std::string idString = prepareMapName( mapId );
            const std::string mapName = "levels/" + idString;
            return loadMapByPath( idString );
        }

        bool RawLevel::loadMapByPath( const std::string& mapPath ) {
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

        bool RawLevel::saveMapById( const std::size_t mapId ) const {
            const std::string idString = prepareMapName( mapId );
            const std::string mapName = "levels/" + idString;
            return saveMapByPath( idString );
        }

        bool RawLevel::saveMapByPath( const std::string& mapPath ) const {
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

        std::size_t RawLevel::countSolid( const utils::Rect& area ) const {
            std::size_t ret = 0;
            for ( int x = area.min.x; x<= area.max.x; ++x ) {
                for ( int y = area.min.y; y<= area.max.y; ++y ) {
                    const raw::SlabType currSlab = getSlab( x, y );
                    if ( raw::isSolid( currSlab ) ) {
                        ++ret;
                    }
                }
            }
            return ret;
        }

        std::size_t RawLevel::countSlabs( const SlabType slab ) const {
            const Rect mapRect = mapSize();
            return countSlabs( mapRect, slab );
        }

        std::size_t RawLevel::countSlabs( const utils::Rect& area, const SlabType slab ) const {
            std::size_t ret = 0;
            for ( int x = area.min.x; x<= area.max.x; ++x ) {
                for ( int y = area.min.y; y<= area.max.y; ++y ) {
                    const raw::SlabType currSlab = getSlab( x, y );
                    if ( currSlab == slab ) {
                        ++ret;
                    }
                }
            }
            return ret;
        }

        SlabType RawLevel::getSlab( const std::size_t x, const std::size_t y ) const {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return SlabType::ST_ROCK;
            }
            const LEVEL* level = data->lvl;
            const SlabType slab = (SlabType) get_tile_slab( level, x, y );
            return slab;
        }

        SlabType RawLevel::getSlab( const Point& point ) const {
            return getSlab( point.x, point.y );
        }

        bool RawLevel::isSlab( const std::size_t x, const std::size_t y, const SlabType slab ) const {
            const SlabType currSlab = getSlab( x, y );
            return ( currSlab == slab );
        }

        bool RawLevel::isSlab( const Point& point, const SlabType slab ) const {
            const SlabType currSlab = getSlab( point.x, point.y );
            return ( currSlab == slab );
        }

        bool RawLevel::isFortified( const Point& point ) const {
            const SlabType currSlab = getSlab( point.x, point.y );
            return isWall( currSlab );
        }

        void RawLevel::setSlab( const std::size_t x, const std::size_t y, const SlabType type ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return ;
            }
            LEVEL* level = data->lvl;
    //        set_tile_slab( level, x, y, (unsigned short) type );            /// much faster
            user_set_slab( level, x, y, (unsigned short) type );
        }

        void RawLevel::setSlab( const Point& point, const SlabType type ) {
            setSlab( point.x, point.y, type );
        }

        void RawLevel::setSlab( const std::size_t x, const std::size_t y, const SlabType type, const PlayerType owner ) {
            setSlab( x, y, type );
            setOwner( x, y, owner );
        }

        void RawLevel::setSlab( const Point& point, const SlabType type, const PlayerType owner ) {
            setSlab( point.x, point.y, type );
            setOwner( point.x, point.y, owner );
        }

        void RawLevel::setSlab( const std::size_t startX, const std::size_t startY,
                             const std::size_t endX,   const std::size_t endY,
                             const SlabType type )
        {
            if ( startX >= MAP_SIZE_X || startY >= MAP_SIZE_Y ) {
                LOG() << "given point is outside map: [" << startX << " " << startY << "]";
                return ;
            }
            if ( endX >= MAP_SIZE_X || endY >= MAP_SIZE_Y ) {
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

        void RawLevel::setSlab( const Rect& rect, const SlabType type ) {
            setSlab( rect.min.x, rect.min.y, rect.max.x, rect.max.y, type );
        }

        void RawLevel::setSlab( const std::set< Point >& positions, const SlabType type ) {
            for ( const Point& item: positions ) {
                setSlab( item, type );
            }
        }

        void RawLevel::setSlab( const PointList& positions, const SlabType type ) {
            for ( const Point& item: positions ) {
                setSlab( item, type );
            }
        }

        void RawLevel::setSlab( const std::size_t startX, const std::size_t startY,
                             const std::size_t endX,   const std::size_t endY,
                             const SlabType room, const PlayerType owner )
        {
            if ( startX >= MAP_SIZE_X || startY >= MAP_SIZE_Y ) {
                LOG() << "given point is outside map: [" << startX << " " << startY << "]";
                return ;
            }
            if ( endX >= MAP_SIZE_X || endY >= MAP_SIZE_Y ) {
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

        void RawLevel::setSlab( const Rect& rect, const SlabType room, const PlayerType owner ) {
            setSlab( rect.min.x, rect.min.y, rect.max.x, rect.max.y, room, owner );
        }

        void RawLevel::setSlabOutline( const Rect& rect, const SlabType type ) {
            setSlab( rect.min.x, rect.min.y, rect.min.x, rect.max.y, type );
            setSlab( rect.min.x, rect.min.y, rect.max.x, rect.min.y, type );
            setSlab( rect.min.x, rect.max.y, rect.max.x, rect.max.y, type );
            setSlab( rect.max.x, rect.min.y, rect.max.x, rect.max.y, type );
        }

        PlayerType RawLevel::getOwner( const std::size_t x, const std::size_t y ) const {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return PlayerType::PT_UNSET;
            }
            LEVEL* level = data->lvl;
            const PlayerType player = (PlayerType) get_tile_owner( level, x, y );
            return player;
        }

        void RawLevel::setOwner( const std::size_t x, const std::size_t y, const PlayerType owner ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return ;
            }
            LEVEL* level = data->lvl;
            user_set_tile_owner( level, x, y, (unsigned short) owner );
        }

        std::string RawLevel::printItems() const {
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

        std::size_t RawLevel::countItems( const utils::Point& point ) const {
            return countItems( point.x, point.y );
        }

        std::size_t RawLevel::countItems( const utils::Rect& rect ) const {
            std::size_t sum = 0;
            const std::size_t rectSize = rect.area();
            for ( std::size_t i=0; i<rectSize; ++i ) {
                const Point point = rect.pointByIndex( i );
                sum += countItems( point );
            }
            return sum;
        }

        std::size_t RawLevel::countItems( const std::size_t x, const std::size_t y ) const {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return 0;
            }
            LEVEL* level = data->lvl;

            static const std::size_t slabSubSize = MAP_SUBNUM_X * MAP_SUBNUM_Y;

            std::size_t ret = 0;
            for ( std::size_t subIndex=0; subIndex<slabSubSize; ++subIndex ) {
                const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
                const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;
                const int things_count = get_thing_subnums( level, sx, sy );
                if ( things_count < 0 ) {
                    continue ;
                }
                ret += (std::size_t) things_count;
            }
            return ret;
        }

        /// prevents setting two items inside the same subtile
        unsigned char* RawLevel::setItem( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeItem item ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return nullptr;
            }
            LEVEL* level = data->lvl;
            const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
            const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;

            /// remove already existing items in given position
            const int things_count = get_thing_subnums( level, sx, sy );
            for (int k=things_count-1; k>=0; --k) {
                thing_del( level, sx, sy, k );
            }

            /// create new item
            unsigned char * thing = create_item_adv( level, sx, sy, (unsigned char) item );
            if ( thing == nullptr ) {
                return nullptr;
            }
            thing_add( level, thing );
            return thing;
        }

        unsigned char* RawLevel::setItem( const Point& point, const SubTypeItem item ) {
            return setItem( point.x, point.y, 4, item );
        }

        unsigned char* RawLevel::setItem( const Point& point, const std::size_t subIndex, const SubTypeItem item ) {
            return setItem( point.x, point.y, subIndex, item );
        }

        void RawLevel::setItem( const Rect& rect, const std::size_t subIndex, const SubTypeItem item ) {
            for ( int x = rect.min.x; x<= rect.max.x; ++x ) {
                for ( int y = rect.min.y; y<= rect.max.y; ++y ) {
                    setItem( x, y, subIndex, item );
                }
            }
        }

        void RawLevel::setTrap( const Point& point, const std::size_t subIndex, const SubTypeTrap trap ) {
            setTrap( point.x, point.y, subIndex, trap );
        }

        void RawLevel::setTrap( const Point& point, const SubTypeTrap trap ) {
            setTrap( point.x, point.y, trap );
        }

        void RawLevel::setTrap( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeTrap trap ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return ;
            }

            const SlabType currSlab = getSlab( x, y );
            if ( isPassage( currSlab ) == false ) {
                setSlab( x, y, SlabType::ST_PATH );
            }

            LEVEL* level = data->lvl;
            const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
            const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;
            unsigned char * thing = create_trap( level, sx, sy, (unsigned char) trap );
            if ( thing == nullptr ) {
                return ;
            }
            thing_add( level, thing );
        }

        void RawLevel::setTrap( const std::size_t x, const std::size_t y, const SubTypeTrap trap ) {
            setTrap( x, y, 4, trap );
        }

        void RawLevel::setDoor( const Point& point, const SubTypeDoor door, const bool locked ) {
            setDoor( point.x, point.y, door, locked );
        }

        bool RawLevel::canPlaceDoor( const utils::Point& point ) const {
            {
                const raw::SlabType currSlab1 = getSlab( point.x+1, point.y );
                const raw::SlabType currSlab2 = getSlab( point.x-1, point.y );
                if ( raw::isSolid( currSlab1 ) && raw::isSolid( currSlab2 ) ) {
                    return true;
                }
            }
            {
                const raw::SlabType currSlab1 = getSlab( point.x, point.y+1 );
                const raw::SlabType currSlab2 = getSlab( point.x, point.y-1 );
                if ( raw::isSolid( currSlab1 ) && raw::isSolid( currSlab2 ) ) {
                    return true;
                }
            }
            return false;
        }

        void RawLevel::setDoor( const std::size_t x, const std::size_t y, const SubTypeDoor door, const bool locked ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return ;
            }

            switch( door ) {
            case SubTypeDoor::STD_WOOD: {
                setSlab( x, y, SlabType::ST_DOORWOOD1 );
                break ;
            }
            case SubTypeDoor::STD_BRACED: {
                setSlab( x, y, SlabType::ST_DOORBRACE1 );
                break ;
            }
            case SubTypeDoor::STD_IRON: {
                setSlab( x, y, SlabType::ST_DOORIRON1 );
                break ;
            }
            case SubTypeDoor::STD_MAGIC: {
                setSlab( x, y, SlabType::ST_DOORMAGIC1 );
                break ;
            }
            }

            LEVEL* level = data->lvl;
            const std::size_t sx = x * MAP_SUBNUM_X + 1;
            const std::size_t sy = y * MAP_SUBNUM_Y + 1;

            for ( int i=get_thing_subnums(level,sx,sy)-1; i >=0; --i) {
                unsigned char *sec_thing = (unsigned char *) get_thing(level,sx,sy,i);
                if ( is_door(sec_thing) ) {
                    set_door_lock( level, sec_thing, locked );
                }
            }

//            unsigned char * thing = create_door( level, sx, sy, (unsigned char) door );
//            if ( thing == nullptr ) {
//                return ;
//            }
//            set_door_lock( level, thing, locked );
//            thing_add( level, thing );
        }

        void RawLevel::setCreature( const std::size_t x, const std::size_t y, const std::size_t subIndex, const SubTypeCreature creature, const std::size_t number, const std::size_t expLevel, const PlayerType owner ) {
            if ( x >= MAP_SIZE_X || y >= MAP_SIZE_Y ) {
                /// out of map
                LOG() << "given point is outside map: [" << x << " " << y << "]";
                return ;
            }
            if ( expLevel < 1 ) {
                LOG() << "invalid experience level value: " << expLevel;
                return ;
            }
            if ( expLevel > 10 ) {
                LOG() << "invalid experience level value: " << expLevel;
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
                set_thing_level( thing, expLevel - 1 );
                set_thing_owner( thing, (unsigned char) owner );
                thing_add( level, thing );
            }
        }

        void RawLevel::setCreature( const Point& point, const std::size_t subIndex, const SubTypeCreature creature, const std::size_t number, const std::size_t expLevel, const PlayerType owner ) {
            setCreature( point.x, point.y, subIndex, creature, number, expLevel, owner );
        }

        /// ============================================================

        unsigned char* RawLevel::getActionPointByNumber( const std::size_t actionPoint ) {
            LEVEL* level = data->lvl;

            const int arr_entries_x = level->tlsize.x * MAP_SUBNUM_X;
            const int arr_entries_y = level->tlsize.y * MAP_SUBNUM_Y;

            for (int i=0; i < arr_entries_y; ++i) {
                for (int j=0; j < arr_entries_x; ++j) {
                    const int actnpt_count = get_actnpt_subnums( level, i, j );
                    for ( int k=0; k<actnpt_count; ++k ) {
                        unsigned char *actnpt = (unsigned char *) get_actnpt( level, i, j, k );
                        unsigned short num = get_actnpt_number( actnpt );
                        if ( num == actionPoint ) {
                            return actnpt;
                        }
                    }
                }
            }

            return nullptr;
        }

        utils::Point RawLevel::getActionPointPosition( const std::size_t actionPoint ) {
            unsigned char *actnpt = getActionPointByNumber( actionPoint );
            if ( actnpt == nullptr ) {
                return Point( -1, -1 );
            }

            unsigned char x = get_actnpt_subtile_x( actnpt );
            unsigned char y = get_actnpt_subtile_y( actnpt );
            return utils::Point( x / MAP_SUBNUM_X, y / MAP_SUBNUM_Y );
        }

        std::size_t RawLevel::addActionPoint( const std::size_t x, const std::size_t y, const std::size_t subIndex, const std::size_t radius ) {
            LEVEL* level = data->lvl;
            const std::size_t sx = x * MAP_SUBNUM_X + subIndex % MAP_SUBNUM_X;
            const std::size_t sy = y * MAP_SUBNUM_Y + subIndex / MAP_SUBNUM_X;
            unsigned char *thing = create_actnpt( level, sx, sy );
            set_actnpt_range_subtile( thing, 2 + radius * 3 );
            actnpt_add( level, thing );
            return get_actnpt_number( thing );
        }

        unsigned char* RawLevel::getHeroGateByNumber( const std::size_t heroGate ) {
            LEVEL* level = data->lvl;
            const int arr_entries_x = level->tlsize.x * MAP_SUBNUM_X;
            const int arr_entries_y = level->tlsize.y * MAP_SUBNUM_Y;

            for (int i=0; i < arr_entries_y; ++i) {
                for (int j=0; j < arr_entries_x; ++j) {
                    const int things_count = get_thing_subnums( level, i, j );

                    for (int k=0; k<things_count; ++k) {
                        unsigned char* thing = (unsigned char*) get_thing( level, i, j, k );
                        if ( is_herogate( thing ) == 0 ) {
                            continue ;
                        }
                        const unsigned char gateNum = get_thing_level( thing );
                        if ( gateNum == heroGate ) {
                            return thing;
                        }
                    }
                }
            }

            return nullptr;
        }

        utils::Point RawLevel::getHeroGatePosition( const std::size_t heroGate ) {
            unsigned char *thing = getHeroGateByNumber( heroGate );
            if ( thing == nullptr ) {
                return Point( -1, -1 );
            }
            unsigned char x = get_thing_subtile_x( thing );
            unsigned char y = get_thing_subtile_y( thing );
            return utils::Point( x / MAP_SUBNUM_X, y / MAP_SUBNUM_Y );
        }

        void RawLevel::setHeroGateNumber( unsigned char* heroGate, const std::size_t number ) {
            set_thing_level( heroGate, number );
        }

        /// ============================================================

        void RawLevel::setRoom( const Rect& position, const raw::SlabType room, const PlayerType owner, const bool fortify ) {
            if ( room != raw::SlabType::ST_DUNGHEART ) {
                setSlab( position, room, owner );
                if ( fortify ) {
                    this->fortify( position, owner );
                }
                return ;
            }
            if ( position.width() > 3 && position.height() > 3 ) {
                setSlab( position, raw::SlabType::ST_CLAIMED, owner );
                const Point center = position.center();
                const Rect shrink(center, 3, 3);
                setSlab( shrink, room, owner );
            } else {
                setSlab( position, room, owner );
            }
            if ( fortify ) {
                this->fortify( position, owner );
            }
        }

        void RawLevel::fortify( const Point& point, const PlayerType owner ) {
            for ( int x = point.x-1; x<= point.x+1; ++x ) {
                for ( int y = point.y-1; y<= point.y+1; ++y ) {
                    const raw::SlabType currSlab = getSlab( x, y );
                    if ( raw::isEarth( currSlab ) ||
                         raw::isLiquid( currSlab ) ) {
                        setSlab( x, y, raw::SlabType::ST_WALLDRAPE, owner );
                    }
                }
            }
        }

        void RawLevel::fortify( const Rect& room, const PlayerType owner ) {
            for ( int x = room.min.x-1; x<= room.max.x+1; ++x ) {
                for ( int y = room.min.y-1; y<= room.max.y+1; ++y ) {
                    const raw::SlabType currSlab = getSlab( x, y );
                    if ( raw::isEarth( currSlab ) ||
                         raw::isLiquid( currSlab ) ) {
                        setSlab( x, y, raw::SlabType::ST_WALLDRAPE, owner );
                    }
                }
            }
        }

        /// ============================================================

        void RawLevel::setRescale( const std::size_t rescale ) {
            LEVEL* level = data->lvl;
            level->optns.picture.rescale = rescale;
        }

        void RawLevel::generateBmp() {
            LEVEL* level = data->lvl;
            if ( level == nullptr ) {
                LOG_WARN() << "unable to generate bmp -- uninitialized level";
                return ;
            }
            const char* dataPath = data->getDataPath();
            if ( dataPath == nullptr ) {
                /// data path is not set -- initialize with empty string
                /// setDataPath( "" );
                LOG_WARN() << "unable to generate bmp -- data path not set";
                return ;
            }
            generate_map_bitmap_mapfname( level );
        }

        bool RawLevel::generateBmp( const std::string& path ) {
            LEVEL* level = data->lvl;
            if ( level == nullptr ) {
                LOG_WARN() << "unable to generate bmp -- uninitialized level";
                return false;
            }
            const char* dataPath = data->getDataPath();
            if ( dataPath == nullptr ) {
                /// data path is not set -- initialize with empty string
                /// setDataPath( "" );
                LOG_WARN() << "unable to generate bmp -- data path not set";
                return false;
            }

            level->optns.picture.data_path = level->optns.data_path;
            const char *bmpfname = path.c_str();
            ///level->optns.picture.tngflags = TNGFLG_SHOW_CIRCLES;
    //        level->optns.picture.rescale = 2;
            const short result = generate_map_bitmap( bmpfname, level, &(level->optns.picture));
            if ( result != ERR_NONE ) {
                LOG_WARN() << "unable to generate bmp";
                return false;
            }
            message_info("Bitmap \"%s\" created.",bmpfname);
            return true;
        }

        void RawLevel::setInfo() {
            setAuthor( "dkmage" );
            setDesciption( "map generated by dk-mage ver. " + VERSION_FULL_STRING );
            setLevelName( "dk-mage map" );
        }

        void RawLevel::setAuthor( const std::string& info ) {
            LEVEL* level = data->lvl;
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }
            setTextField( &level->info.author_text, info.c_str() );
            setTextField( &level->info.editor_text, info.c_str() );
        }

        void RawLevel::setDesciption( const std::string& info ) {
            LEVEL* level = data->lvl;
            if ( level == nullptr ) {
                LOG() << "uninitialized level";
                return ;
            }
            setTextField( &level->info.desc_text, info.c_str() );
        }

        std::size_t RawLevel::countAllCreatures() {
            std::size_t creaturesCount = 0;

            LEVEL* level = data->lvl;
            const int arr_entries_x = level->tlsize.x * MAP_SUBNUM_X;
            const int arr_entries_y = level->tlsize.y * MAP_SUBNUM_Y;

            for (int i=0; i < arr_entries_y; ++i) {
                for (int j=0; j < arr_entries_x; ++j) {
                    const int things_count = get_thing_subnums( level, i, j );

                    for (int k=0; k<things_count; ++k) {
                        unsigned char* thing = (unsigned char*) get_thing( level, i, j, k );
                        if ( is_creature( thing ) != 0 ) {
                            ++creaturesCount;
                        }
                    }
                }
            }

            return creaturesCount;
        }

        std::size_t RawLevel::countActionPoints() {
            const LEVEL* level = data->lvl;
            const std::size_t arr_entries_x = level->tlsize.x * MAP_SUBNUM_X;
            const std::size_t arr_entries_y = level->tlsize.y * MAP_SUBNUM_Y;

            std::size_t ret = 0;
            for ( std::size_t cy=0; cy<arr_entries_y; ++cy ) {
                for ( std::size_t cx=0; cx<arr_entries_x; ++cx ) {
                    ret += get_actnpt_subnums( level, cx, cy );
                }
            }
            return ret;
        }

        std::size_t RawLevel::countSeparatedAreas() {
            AreaDetector data;

            /// mark rock
            for ( std::size_t y=0; y<MAP_SIZE_Y; ++y) {
                for ( std::size_t x=0; x<MAP_SIZE_X; ++x) {
                    const SlabType stub = getSlab(x, y);
                    if ( raw::isImpassable( stub ) ) {
                        data.set(x, y, -1 );
                    }
                }
            }

            /// In this place all 'ROCK' slabs have value '-1'. Rest of 'data' is filled with zeros.

            const std::vector< AreaDetector::AreaInfo > areas = data.fill( 0 );           /// assign numbers to 'zero' areas
            const std::size_t aSize = areas.size();
            if ( aSize == 0 ) {
                return 0;
            }
            return (aSize - 1);
        }

        std::size_t RawLevel::countClaimAreas() {
            AreaDetector data;

            /// mark rock
            for ( std::size_t y=0; y<MAP_SIZE_Y; ++y) {
                for ( std::size_t x=0; x<MAP_SIZE_X; ++x) {
                    const SlabType stub = getSlab(x, y);
                    if ( raw::isImpassable( stub ) ) {
                        data.set(x, y, -1 );
                    }
                    if ( raw::isLiquid( stub ) ) {
                        data.set(x, y, -1 );
                    }
                }
            }

            /// In this place all 'ROCK' slabs have value '-1'. Rest of 'data' is filled with zeros.

            const std::vector< AreaDetector::AreaInfo > areas = data.fill( 0 );           /// assign numbers to 'zero' areas
            const std::size_t aSize = areas.size();
            if ( aSize == 0 ) {
                return 0;
            }
            return (aSize - 1);
        }

        void RawLevel::fillSeparatedAreas( const std::size_t areaLimit ) {
            AreaDetector data;

            /// mark rock
            for ( std::size_t y=0; y<MAP_SIZE_Y; ++y) {
                for ( std::size_t x=0; x<MAP_SIZE_X; ++x) {
                    const SlabType stub = getSlab(x, y);
                    if ( raw::isImpassable( stub ) ) {
                        data.set(x, y, -1 );
                    }
                }
            }

            /// In this place all 'ROCK' slabs have value '-1'. Rest of 'data' is filled with zeros.

            const std::vector< AreaDetector::AreaInfo > areas = data.fill( 0 );           /// assign numbers to 'zero' areas
            const std::size_t aSize = areas.size();
            if ( aSize < 2 ) {
                return ;
            }

            for ( const AreaDetector::AreaInfo& item: areas ) {
                const std::vector< std::size_t >& cells = item.cells;
                if ( cells.size() > areaLimit ) {
                    continue ;
                }
                /// fill cells
                const std::vector< utils::Point > points = item.cellsAsPoints();
                setSlab( points, SlabType::ST_ROCK );
            }
        }

    }
} /* namespace adiktedpp */
