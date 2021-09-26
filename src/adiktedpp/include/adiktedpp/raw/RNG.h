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


        /// ==========================================================================


        class AbstractRNG {
        public:

            virtual ~AbstractRNG() = default;


            virtual void srand( unsigned int seed ) = 0;

            virtual void srand() = 0;

            /// returns value from range [ 0, MAX )
            virtual unsigned int randi() = 0;

            /**
             * Return value in range [0..1].
             */
            virtual double randd() = 0;

            /// =======================================

//            virtual unsigned int rand_max() const = 0;

            virtual unsigned int randin( const unsigned int maxValue ) = 0;

            virtual unsigned int randir( const unsigned int minValue, const unsigned int maxValue ) = 0;

            virtual bool randb() = 0;

            virtual bool randb( const double probability ) = 0;

//            virtual void consume( const std::size_t num ) = 0;

        };


        class BaseRNG: public AbstractRNG {
        public:

            using AbstractRNG::randi;

            /// returns value from range [ 0, maxValue )
            unsigned int randin( const unsigned int maxValue ) override;

            /// returns value from range [ minValue, maxValue )
            unsigned int randir( const unsigned int minValue, const unsigned int maxValue ) override;

            bool randb();

            bool randb( const double probability );

        };


        /// ==========================================================================


        class WrapperRNG: public AbstractRNG {
            AbstractRNG* rng;


        public:

            WrapperRNG( AbstractRNG* rng = nullptr ): rng(rng) {
            }

            void srand( unsigned int seed ) override {
                rng->srand( seed );
            }

            void srand() override {
                rng->srand();
            }

            /// returns value from range [ 0, MAX )
            unsigned int randi() override {
                return rng->randi();
            }

            unsigned int randin( const unsigned int maxValue ) override {
                return rng->randin( maxValue );
            }

            unsigned int randir( const unsigned int minValue, const unsigned int maxValue ) override {
                return rng->randir( minValue, maxValue );
            }

            /**
             * Return value in range [0..1].
             */
            double randd() override {
                return rng->randd();
            }

            bool randb() override {
                return rng->randb();
            }

            bool randb( const double probability ) override {
                return rng->randb( probability );
            }

        };


        /// ==========================================================================


        class RNGState;


        /**
         * Wrapper class for Mersenne-Twist pseudo-RNG C implementation provided by libadikted.
         */
        class RNG: public BaseRNG {
            std::unique_ptr<RNGState> state;


        public:

            RNG();

            ~RNG();

            void srand( unsigned int seed ) override;

            void srand() override;

//            unsigned int rand_max() const;

            using BaseRNG::randi;

            /// returns value from range [ 0, MAX )
            unsigned int randi() override;

            /**
             * Return value in range [0..1].
             */
            double randd() override;

//            void consume( const std::size_t num );

        };

    } /* namespace raw */
} /* namespace adiktedpp */

#endif /* ADIKTEDPP_INCLUDE_ADIKTEDPP_RAW_RNG_H_ */
