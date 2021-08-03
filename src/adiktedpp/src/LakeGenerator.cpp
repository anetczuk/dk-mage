/*
 * LakeGenerator.cpp
 *
 */

#include "adiktedpp/LakeGenerator.h"

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

    void LakeGenerator::grow( std::set< Point >& data, const std::size_t delta ) {
        for ( std::size_t i=0; i<delta; ++i ) {
            std::set< Point > data2 = data;
            for ( auto item: data ) {
                data2.insert( item + Point(  1, 0 ) );
                data2.insert( item + Point( -1, 0 ) );
                data2.insert( item + Point( 0,  1 ) );
                data2.insert( item + Point( 0, -1 ) );

//                data2.insert( item + Point(  1,  1 ) );
//                data2.insert( item + Point( -1,  1 ) );
//                data2.insert( item + Point(  1, -1 ) );
//                data2.insert( item + Point( -1, -1 ) );
            }
            data = data2;
        }
    }

} /* namespace adiktedpp */
