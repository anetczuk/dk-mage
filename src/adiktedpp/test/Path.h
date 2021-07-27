/*
 * Path.h
 *
 */

#ifndef ADIKTEDPP_TEST_PATH_H_
#define ADIKTEDPP_TEST_PATH_H_

/// inclusion: #include "Path.h"

#include <ghc/filesystem.hpp>


using ghc::filesystem::path;
using ghc::filesystem::exists;
using ghc::filesystem::current_path;
using ghc::filesystem::create_directories;


inline std::string getTestDataPath( const std::string& subPath ) {
    #if defined(TEST_DATA_PATH)
        return std::string(TEST_DATA_PATH) + "/" + subPath;
    #else
        return subPath;
    #endif
}

inline path getTestOutputPath() {
    path currPath = current_path() / TEST_MODULE_NAME;
    currPath += "_output";
    create_directories( currPath );
    return currPath;
}

//inline std::string getKeeperDataPath() {
//    #if defined(KEEPER_DATA_PATH)
//        return std::string(KEEPER_DATA_PATH);
//    #else
//        return "";
//    #endif
//}


#endif /* ADIKTEDPP_TEST_PATH_H_ */
