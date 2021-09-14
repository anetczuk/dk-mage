/*
 * Rand.cpp
 *
 */

#include "utils/Rand.h"


///// required for mtwist.h
//#undef MT_GENERATE_CODE_IN_HEADER
//#define MT_GENERATE_CODE_IN_HEADER 0

/// supports C++
#include "libadikted/mtwist/mtwist.h"


namespace libadikted {
    extern "C" {
        #include "libadikted/rng.h"
    }
}


namespace utils {

    /**
     *
     */
    class RNGState {
        mt_state state;

    private:

        RNGState() = default;


    public:

        void srand( unsigned int seed ) {
            mts_seed32( &state, seed );
//            libadikted::rng_srand( seed );
        }

        void goodseed() {
            mts_goodseed( &state );
//            libadikted::rng_srand_random();
        }

        unsigned int rand_max() const {
            return 4294967295;                      /** 2^32 - 1 **/
        }

        /// returns value from range [ 0, MAX )
        unsigned int randi() {
//            return libadikted::rng_rand();
            return mts_lrand( &state );
        }

        /// ============================================================

        /// returns value from range [ 0, maxValue )
        unsigned int randi( const unsigned int maxValue ) {
            const unsigned int rint = randi();
            return ( rint % maxValue );
        }

        /// returns value from range [ minValue, maxValue )
        unsigned int randi( const unsigned int minValue, const unsigned int maxValue ) {
            const unsigned int rint = randi();
            return ( rint % (maxValue - minValue) ) + minValue;
        }

        /**
         * Return value in range [0..1].
         */
        double randd() {
            const unsigned int rint = randi();
            const double val = ((double) rint / rand_max() );
            return val;
        }

        bool randb() {
            return ( randi() % 2 == 1 );
        }

        bool randb( const double probability ) {
            return ( randd() < probability );
        }

        void consume( const std::size_t num ) {
            for ( std::size_t i=0; i<num; ++i ) {
                randi();
            }
        }

        /// ==================================================================

        static RNGState& instance() {
            static RNGState state;
            return state;
        }

    };


    void rng_srand( unsigned int seed ) {
        libadikted::rng_srand( seed );

        RNGState& engine = RNGState::instance();
        engine.srand( seed );
    }

    void rng_srand() {
        libadikted::rng_srand_random();

        RNGState& engine = RNGState::instance();
        engine.goodseed();
    }

    bool rng_randb() {
        RNGState& engine = RNGState::instance();
        return engine.randb();
    }

    bool rng_randb( const double probability ) {
        RNGState& engine = RNGState::instance();
        return engine.randb( probability );
    }

    /// returns value from range [ 0, MAX )
    unsigned int rng_randi() {
        RNGState& engine = RNGState::instance();
        return engine.randi();
    }

    /// returns value from range [ 0, maxValue )
    unsigned int rng_randi( const unsigned int maxValue ) {
        RNGState& engine = RNGState::instance();
        return engine.randi( maxValue );
    }

    /// returns value from range [ minValue, maxValue )
    unsigned int rng_randi( const unsigned int minValue, const unsigned int maxValue ) {
        RNGState& engine = RNGState::instance();
        return engine.randi( minValue, maxValue );
    }

    /**
     * Return value in range [0..1].
     */
    double rng_randd() {
        RNGState& engine = RNGState::instance();
        return engine.randd();
    }

    void rng_consume( const std::size_t num ) {
        RNGState& engine = RNGState::instance();
        return engine.consume( num );
    }

    std::string genSeed( const std::size_t length ) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        std::string ret;
        ret.reserve( length );

        const int len = sizeof( alphanum ) - 1;
        for ( std::size_t i=0; i<length; ++i ) {
            const int index = rng_randi() % len;
            ret += alphanum[ index ];
        }
        return ret;
    }

}
