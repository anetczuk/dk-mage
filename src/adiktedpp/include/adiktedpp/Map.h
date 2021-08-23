/*
 * Map.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_MAP_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_MAP_H_

#include "adiktedpp/Level.h"
#include "adiktedpp/script/Script.h"


namespace adiktedpp {

    class Map {
    public:

        Level level;
        script::Script script;

    };

} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_MAP_H_ */
