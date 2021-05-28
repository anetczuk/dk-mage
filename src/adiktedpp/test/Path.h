/*
 * Path.h
 *
 *  Created on: May 29, 2021
 *      Author: bob
 */

#ifndef ADIKTEDPP_TEST_PATH_H_
#define ADIKTEDPP_TEST_PATH_H_


inline std::string getDataPath( const std::string& subPath ) {
    #if defined(TEST_DATA_PATH)
        return std::string(TEST_DATA_PATH) + "/" + subPath;
    #else
        return subPath;
    #endif
}


#endif /* ADIKTEDPP_TEST_PATH_H_ */
