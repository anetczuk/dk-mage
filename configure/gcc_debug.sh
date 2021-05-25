#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


BUILD_TYPE=Debug


SRC_DIR=$SCRIPT_DIR/../src

BUILD_DIR_NAME=$(echo $BUILD_TYPE"_gcc" | tr '[:upper:]' '[:lower:]')
BUILD_DIR=$SCRIPT_DIR/../build/$BUILD_DIR_NAME


## remove old configuration
rm -rf $BUILD_DIR 

mkdir -p $BUILD_DIR
cd $BUILD_DIR


export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE $SRC_DIR
