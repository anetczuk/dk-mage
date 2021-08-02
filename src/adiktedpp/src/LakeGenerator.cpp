/*
 * LakeGenerator.cpp
 *
 */

#include "adiktedpp/LakeGenerator.h"

#include <sstream>


namespace adiktedpp {

    std::string LakeGenerator::print() const {
        std::stringstream stream;
        stream << "av: ";
        for ( const utils::Point& item: available ) {
            stream << item << " ";
        }
        stream << "\n";
        stream << "add: ";
        for ( const utils::Point& item: added ) {
            stream << item << " ";
        }
        stream << "\n";
        return stream.str();
    }

    void LakeGenerator::grow( std::set< utils::Point >& data, const std::size_t delta ) {
        for ( std::size_t i=0; i<delta; ++i ) {
            std::set< utils::Point > data2 = data;
            for ( auto item: data ) {
                data2.insert( item + utils::Point(  1, 0 ) );
                data2.insert( item + utils::Point( -1, 0 ) );
                data2.insert( item + utils::Point( 0,  1 ) );
                data2.insert( item + utils::Point( 0, -1 ) );

//                data2.insert( item + utils::Point(  1,  1 ) );
//                data2.insert( item + utils::Point( -1,  1 ) );
//                data2.insert( item + utils::Point(  1, -1 ) );
//                data2.insert( item + utils::Point( -1, -1 ) );
            }
            data = data2;
        }
    }

} /* namespace adiktedpp */
