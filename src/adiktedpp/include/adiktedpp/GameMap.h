/*
 * GameMap.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_GAMEMAP_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_GAMEMAP_H_

#include "adiktedpp/Level.h"
#include "adiktedpp/script/Script.h"


namespace adiktedpp {

    class GameMap {
    public:

        Level level;
        script::Script script;

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_GAMEMAP_H_ */
