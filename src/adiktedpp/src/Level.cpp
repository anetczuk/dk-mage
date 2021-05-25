/*
 *
 */

#include "adiktedpp/Level.h"

#include "libadikted/adikted.h"


namespace adiktedpp {

    struct LevelData {
        ///
    };


    /// ===============================================================


    Level::Level(): data( new LevelData() ) {
    }

    Level::~Level() {
    }

} /* namespace adiktedpp */
