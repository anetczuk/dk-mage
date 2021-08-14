#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


BUILD_TYPE=Release


SRC_DIR=$SCRIPT_DIR/../src

BUILD_DIR_NAME=
if [ "$#" -ge 1 ]; then
    BUILD_DIR_NAME="$1"
else
    BUILD_DIR_NAME=win_$(echo $BUILD_TYPE"_gcc" | tr '[:upper:]' '[:lower:]')
fi
BUILD_DIR=$SCRIPT_DIR/../build/$BUILD_DIR_NAME


## remove old configuration
rm -rf $BUILD_DIR 

mkdir -p $BUILD_DIR
cd $BUILD_DIR


#export CC=/usr/bin/gcc
#export CXX=/usr/bin/g++

cmake -DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/cmake/toolchain-i686-w64-mingw32.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $SRC_DIR

#cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE $SRC_DIR
