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

} /* namespace adiktedpp */
