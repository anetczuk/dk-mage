/*
 * LakeGenerator.cpp
 *
 */

#include "adiktedpp/LakeGenerator.h"

#include "utils/Rand.h"

#include <sstream>


using namespace utils;


namespace adiktedpp {

    std::string LakeGenerator::print() const {
        std::stringstream stream;
        stream << "av: ";
        for ( const Point& item: available ) {
            stream << item << " ";
        }
        stream << "\n";
        stream << "add: ";
        for ( const Point& item: added ) {
            stream << item << " ";
        }
        stream << "\n";
        return stream.str();
    }

    void LakeGenerator::grow( std::set< Point >& data, const std::size_t deltaHV, const std::size_t deltaD ) {
        const std::size_t deltaBoth = std::min( deltaHV, deltaD );

        for ( std::size_t i=0; i<deltaBoth; ++i ) {
            std::set< Point > data2 = data;
            for ( auto item: data ) {
                data2.insert( item + Point(  1, 0 ) );
                data2.insert( item + Point( -1, 0 ) );
                data2.insert( item + Point( 0,  1 ) );
                data2.insert( item + Point( 0, -1 ) );

                data2.insert( item + Point(  1,  1 ) );
                data2.insert( item + Point( -1,  1 ) );
                data2.insert( item + Point(  1, -1 ) );
                data2.insert( item + Point( -1, -1 ) );
            }
            data = data2;
        }

        const std::size_t restHW = deltaHV - deltaBoth;
        for ( std::size_t i=0; i<restHW; ++i ) {
            std::set< Point > data2 = data;
            for ( auto item: data ) {
                data2.insert( item + Point(  1, 0 ) );
                data2.insert( item + Point( -1, 0 ) );
                data2.insert( item + Point( 0,  1 ) );
                data2.insert( item + Point( 0, -1 ) );
            }
            data = data2;
        }

        const std::size_t restD = deltaD - deltaBoth;
        for ( std::size_t i=0; i<restD; ++i ) {
            std::set< Point > data2 = data;
            for ( auto item: data ) {
                data2.insert( item + Point(  1,  1 ) );
                data2.insert( item + Point( -1,  1 ) );
                data2.insert( item + Point(  1, -1 ) );
                data2.insert( item + Point( -1, -1 ) );
            }
            data = data2;
        }
    }

    std::set< utils::Point > LakeGenerator::edge( std::set< utils::Point >& data ) {
        std::set< utils::Point > lake = data;
        grow( lake, 1, 1 );
        return disjoint_sets( lake, data );
    }

} /* namespace adiktedpp */
