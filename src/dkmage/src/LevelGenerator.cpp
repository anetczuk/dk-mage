/*
 * LevelGenerator.cpp
 *
 */

#include "dkmage/LevelGenerator.h"

#include "utils/Seed.h"
#include "utils/Log.h"


namespace dkmage {

    void LevelGenerator::generate( const std::string& seed ) {
        if ( seed.empty() ) {
            const unsigned int timeSeed = time(NULL);
            srand( timeSeed );
            const std::string newSeed = utils::genSeed();
            if ( newSeed.empty() ) {
                LOG() << "unable to generate seed";
                return ;
            }
            generate( newSeed );
            return ;
        }

        LOG() << "generating map with seed '" << seed << "'";
        const std::size_t seedValue = std::hash< std::string >{}( seed );
        generate( seedValue );
    }

} /* namespace dkmage */
