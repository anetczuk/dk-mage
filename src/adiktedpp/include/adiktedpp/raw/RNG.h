/*
 * RNG.h
 *
 */

#ifndef ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RNG_H_
#define ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RNG_H_

#include <memory>


namespace adiktedpp {
    namespace raw {

        /// initialize libadikted RNG with seed
        void adikted_srand( unsigned int seed );

        /// initialize libadikted RNG with random seed
        void adikted_srand();


        /// ================================


        class RNGState;


        class RNG {
            std::unique_ptr<RNGState> state;

        public:

            RNG();

            ~RNG();

            void srand( unsigned int seed );

            void goodseed();

//            unsigned int rand_max() const;

            /// returns value from range [ 0, MAX )
            unsigned int randi();

            /// ============================================================

            /// returns value from range [ 0, maxValue )
            unsigned int randi( const unsigned int maxValue );

            /// returns value from range [ minValue, maxValue )
            unsigned int randi( const unsigned int minValue, const unsigned int maxValue );

            /**
             * Return value in range [0..1].
             */
            double randd();

            bool randb();

            bool randb( const double probability );

//            void consume( const std::size_t num );

        };

    } /* namespace raw */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RNG_H_ */
