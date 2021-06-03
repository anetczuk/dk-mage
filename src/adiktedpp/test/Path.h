/*
 * Path.h
 *
 */

#ifndef ADIKTEDPP_TEST_PATH_H_
#define ADIKTEDPP_TEST_PATH_H_

/// inclusion: #include "Path.h"


inline std::string getTestDataPath( const std::string& subPath ) {
    #if defined(TEST_DATA_PATH)
        return std::string(TEST_DATA_PATH) + "/" + subPath;
    #else
        return subPath;
    #endif
}

inline std::string getKeeperDataPath() {
    #if defined(KEEPER_DATA_PATH)
        return std::string(KEEPER_DATA_PATH);
    #else
        return "";
    #endif
}


#endif /* ADIKTEDPP_TEST_PATH_H_ */
