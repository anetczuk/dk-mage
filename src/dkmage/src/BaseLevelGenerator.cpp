/*
 * BaseLevelGenerator.cpp
 *
 */

#include "dkmage/BaseLevelGenerator.h"

#include "adiktedpp/Version.h"

#include <fstream>


using namespace adiktedpp;


namespace dkmage {

    void BaseLevelGenerator::writeIniFile() const {
        const std::string output = level.getRawLevel().outputFileName() + ".mage.ini";

        std::ofstream iniFile;
        iniFile.open( output );

        iniFile << "##\n";
        iniFile << "## map generated by dk-mage ver. " << VERSION_FULL_STRING << "\n";
        iniFile << "##\n";

        iniFile << "\n\n";
        iniFile << "[GENERAL]\n\n";

        const std::set< ParameterName >& localParameters = getLocalParameterNames();
        const std::set< std::string > localNames = getParameterName( localParameters );

        const ParametersMap::Data& rawParameters = parameters.rawData();
        auto iter  = rawParameters.begin();
        auto eiter = rawParameters.end();
        for ( ; iter != eiter; ++iter ) {
            if ( localNames.count( iter->first ) ) {
                /// skip local parameter
                continue ;
            }
            iniFile << iter->first << " = " << iter->second << "\n";
        }

        iniFile.close();
    }

} /* namespace dkmage */
