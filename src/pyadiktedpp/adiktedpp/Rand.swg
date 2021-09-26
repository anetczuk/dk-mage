%module rand

%{
#include "adiktedpp/Rand.h"
%}


## export only selected methods


namespace adiktedpp {

    void rng_srand( unsigned int seed );

    /// initialize RNG with random seed
    void rng_srand();
    
    unsigned int rng_randi();
    
}
