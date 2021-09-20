%module rand

%{
#include "utils/Rand.h"
%}


## export only selected methods


namespace utils {

    void rng_srand( unsigned int seed );

    /// initialize RNG with random seed
    void rng_srand();
    
    unsigned int rng_randi();
    
}
