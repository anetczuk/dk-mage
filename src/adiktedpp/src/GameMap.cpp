/*
 * GameMap.cpp
 *
 */

#include "adiktedpp/GameMap.h"


namespace adiktedpp {

    void GameMap::applyScript() {
        script::LevelScript dataLevel( level );
        dataLevel.rebuild( script );
    }

} /* namespace adiktedpp */
