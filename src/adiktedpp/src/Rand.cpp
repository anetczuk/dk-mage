/*
 * Rand.cpp
 *
 */

#include "adiktedpp/Rand.h"

#include "utils/Log.h"


namespace adiktedpp {

    std::unique_ptr<raw::AbstractRNG>& rng_instance() {
        static std::unique_ptr<raw::AbstractRNG> rng( new raw::RNG() );
        return rng;
    }

    /// ===============================================================

    /// pass RNG with ownership (release object's memory)
    void rng_set( std::unique_ptr<raw::AbstractRNG>& newRNG ) {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        rngPtr.swap( newRNG );
    }

    /// pass RNG without granting ownership (do not release object's memory)
    void rng_set_generator( raw::AbstractRNG& newRNG ) {
        std::unique_ptr<raw::AbstractRNG> rng( new raw::WrapperRNG( &newRNG ) );
        rng_set( rng );
    }

    void rng_srand( unsigned int seed ) {
        raw::adikted_srand( seed );

        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        rngPtr.reset( new raw::RNG() );
        raw::AbstractRNG& engine = *rngPtr;
        engine.srand( seed );
    }

    void rng_srandr() {
        raw::adikted_srandr();

        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        rngPtr.reset( new raw::RNG() );
        raw::AbstractRNG& engine = *rngPtr;
        engine.srandr();
    }

    /// ===============================================================

    bool rng_randb() {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randb();
    }

    bool rng_randb( const double probability ) {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randb( probability );
    }

    /// returns value from range [ 0, MAX )
    unsigned int rng_randi() {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randi();
    }

    /// returns value from range [ 0, maxValue )
    unsigned int rng_randi( const unsigned int maxValue ) {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randin( maxValue );
    }

    /// returns value from range [ minValue, maxValue )
    unsigned int rng_randi( const unsigned int minValue, const unsigned int maxValue ) {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randir( minValue, maxValue );
    }

    /**
     * Return value in range [0..1].
     */
    double rng_randd() {
        std::unique_ptr<raw::AbstractRNG>& rngPtr = rng_instance();
        raw::AbstractRNG& engine = *rngPtr;
        return engine.randd();
    }

//    void rng_consume( const std::size_t num ) {
//        raw::RNG& engine = rng_instance();
//        return engine.consume( num );
//    }

    std::string gen_seed( const std::size_t length ) {
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

    std::uint32_t hash_code( const std::string& text ) {
        const std::size_t strlen = text.length();
        if ( strlen == 0 ) {
            return 0;
        }
        std::uint32_t hash = 0;
        for ( std::size_t i = 0; i < strlen; ++i ) {
            hash = (31 * hash) + text.at(i);
        }
        return hash;
    }

}
