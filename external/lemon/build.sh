#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=lemon-1.3.1.zip


SRC_DIR=$(basename -s .zip $SRC_ARCHIVE)
SRC_PATH="$SCRIPT_DIR/src/$SRC_DIR"
BUILD_PATH="$SCRIPT_DIR/build"
INSTALL_PATH="$SCRIPT_DIR/install"


mkdir -p "$SRC_PATH"
mkdir -p "$BUILD_PATH"

unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$SCRIPT_DIR/src"


mkdir -p "$INSTALL_PATH"
cd "$BUILD_PATH"

cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" $SRC_PATH

cmake --build . --target install


echo -e "\nbuilding lemon done"
