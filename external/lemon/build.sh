#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=lemon-1.3.1.zip


SRC_DIR=$(basename -s .zip $SRC_ARCHIVE)
SRC_PATH="$SCRIPT_DIR/src/$SRC_DIR"
BUILD_ROOT_PATH="$SCRIPT_DIR/build"
INSTALL_PATH="$SCRIPT_DIR/install"


unzip_src() {
    mkdir -p "$SRC_PATH"
    
    unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$SCRIPT_DIR/src"
    
    ## copy fixes
    if compgen -G "$SCRIPT_DIR/fix/*" > /dev/null; then
        cp -r $SCRIPT_DIR/fix/* "$SRC_PATH"
    fi
}

unzip_src


mkdir -p "$INSTALL_PATH"


build_windows() {
    ## building for Windows
    echo "building for Windows"
    
    local BUILD_PATH="${BUILD_ROOT_PATH}_win"
    mkdir -p "$BUILD_PATH"
    cd "$BUILD_PATH"
    
    cmake -DCMAKE_TOOLCHAIN_FILE=$SCRIPT_DIR/../../src/cmake/toolchain-i686-w64-mingw32.cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" $SRC_PATH
    
    cmake --build . --target install
}

# build_windows


build_linux() {
    ## building for Linux
    echo "building for Linux"
    
    local BUILD_PATH="${BUILD_ROOT_PATH}_lin"
    mkdir -p "$BUILD_PATH"
    cd "$BUILD_PATH"
    
    cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" $SRC_PATH
    
    cmake --build . --target install
}

# build_linux


echo -e "\nbuilding lemon done"
