#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


SRC_ROOT=$SCRIPT_DIR/../src
BUILD_ROOT=$SCRIPT_DIR/../build


## check if variables are unbound/unset
if [ -z ${BUILD_DIR_NAME+x} ]; then 
    BUILD_DIR_NAME= 
fi
if [ -z ${CMAKE_PARAMS+x} ]; then 
    CMAKE_PARAMS= 
fi

## $@ -- params
parse_args() {
    if [ "$#" -ge 1 ]; then
        BUILD_DIR_NAME=$1
    fi
    if [ "$#" -ge 2 ]; then
        CMAKE_PARAMS=$2
    fi
}


## check if variables are unbound/unset
if [ -z ${BUILD_TYPE+x} ]; then 
    BUILD_TYPE= 
fi
if [ -z ${CMAKE_TOOLCHAIN+x} ]; then 
    CMAKE_TOOLCHAIN= 
fi

configure_cmake() {
    if [ -z "$BUILD_TYPE" ]; then
        echo "required variable BUILD_TYPE is empty"
        exit 1
    fi
    if [ -z "$BUILD_DIR_NAME" ]; then
        echo "required variable BUILD_DIR_NAME is empty"
        exit 1
    fi
    
    local BUILD_DIR=$BUILD_ROOT/$BUILD_DIR_NAME
    
    ## remove old configuration
    rm -rf $BUILD_DIR 
    
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    
    local PARAMS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
    if [ ! -z "$CMAKE_PARAMS" ]; then
        PARAMS="$PARAMS $CMAKE_PARAMS"
    fi
    if [ ! -z "$CMAKE_TOOLCHAIN" ]; then
        PARAMS="$PARAMS -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN"
    else
        export CC=/usr/bin/gcc
        export CXX=/usr/bin/g++
    fi
    
    cmake $PARAMS $SRC_ROOT
}
