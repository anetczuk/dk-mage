/*
 *
 */

#include "adiktedpp/Level.h"

//extern "C" {
//    #include "libadikted/adikted.h"
//}


namespace adiktedpp {

    struct LevelData {
//        struct LEVEL *lvl;
    };


    /// ===============================================================


    Level::Level(): data( new LevelData() ) {
//        level_init( &data->lvl, MFV_DKGOLD, NULL );
    }

    Level::~Level() {
//        level_free( data->lvl );
//        level_deinit( &data->lvl );
    }

} /* namespace adiktedpp */
