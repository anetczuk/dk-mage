/*
 * RNG.cpp
 *
 */

#include "adiktedpp/raw/RNG.h"

#include "utils/Log.h"


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


namespace adiktedpp {
    namespace raw {

        void adikted_srand( unsigned int seed ) {
            libadikted::rng_srand( seed );
        }

        void adikted_srandr() {
            libadikted::rng_srand_random();
        }


        /// ===============================================================


        /**
         *
         */
        class RNGState {
            mt_state state;


        public:

            RNGState() = default;

            void srand( unsigned int seed ) {
                mts_seed32( &state, seed );
            }

            void goodseed() {
                mts_goodseed( &state );
            }

            unsigned int rand_max() const {
                return 4294967295;                      /** 2^32 - 1 **/
            }

            /// returns value from range [ 0, MAX )
            unsigned int randi() {
                return mts_lrand( &state );
            }

            /// ============================================================

//            /// returns value from range [ 0, maxValue )
//            unsigned int randin( const unsigned int maxValue ) {
//                const unsigned int rint = randi();
//                return ( rint % maxValue );
//            }

//            /// returns value from range [ minValue, maxValue )
//            unsigned int randir( const unsigned int minValue, const unsigned int maxValue ) {
//                const unsigned int rint = randi();
//                return ( rint % (maxValue - minValue) ) + minValue;
//            }

            /**
             * Return value in range [0..1].
             */
            double randd() {
                const unsigned int rint = randi();
                const double val = ((double) rint / rand_max() );
                return val;
            }

//            bool randb() {
//                return ( randi() % 2 == 1 );
//            }

//            bool randb( const double probability ) {
//                return ( randd() < probability );
//            }

//            void consume( const std::size_t num ) {
//                for ( std::size_t i=0; i<num; ++i ) {
//                    randi();
//                }
//            }

        };


        /// ==================================================================


        unsigned int BaseRNG::randin( const unsigned int maxValue ) {
            const unsigned int rint = randi();
            return ( rint % maxValue );
        }

        unsigned int BaseRNG::randir( const unsigned int minValue, const unsigned int maxValue ) {
            const unsigned int rint = randi();
            return ( rint % (maxValue - minValue) ) + minValue;
        }

        bool BaseRNG::randb() {
            return ( randi() % 2 == 1 );
        }

        bool BaseRNG::randb( const double probability ) {
            return ( randd() < probability );
        }


        /// ==================================================================


        RNG::RNG(): state( new RNGState() ) {
        }

        RNG::~RNG() {
        }

        void RNG::srand( unsigned int seed ) {
            RNGState* ptr = state.get();
            return ptr->srand( seed );
        }

        void RNG::srandr() {
            RNGState* ptr = state.get();
            return ptr->goodseed();
        }

//        unsigned int RNG::rand_max() const {
//            const RNGState* ptr = state.get();
//            return ptr->rand_max();
//        }

        /// returns value from range [ 0, MAX )
        unsigned int RNG::randi() {
            RNGState* ptr = state.get();
            return ptr->randi();
        }

        /**
         * Return value in range [0..1].
         */
        double RNG::randd() {
            RNGState* ptr = state.get();
            return ptr->randd();
        }

//        void RNG::consume( const std::size_t num ) {
//            RNGState* ptr = state.get();
//            ptr->consume( num );
//        }

    } /* namespace raw */
} /* namespace adiktedpp */
