#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=tclap-1.4.0-rc1.tar.bz2


SRC_DIR=$(basename -s .tar.bz2 $SRC_ARCHIVE)
SRC_PATH="$SCRIPT_DIR/src/$SRC_DIR"
BUILD_PATH="$SCRIPT_DIR/build"
INSTALL_PATH="$SCRIPT_DIR/install"


mkdir -p "$SRC_PATH"
mkdir -p "$BUILD_PATH"

tar xjf "$SCRIPT_DIR/$SRC_ARCHIVE" -C "$SCRIPT_DIR/src"

## copy fixes
if compgen -G "$SCRIPT_DIR/fix/*" > /dev/null; then
    cp -r $SCRIPT_DIR/fix/* "$SRC_PATH"
fi


mkdir -p "$INSTALL_PATH"
cd "$BUILD_PATH"

cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" $SRC_PATH

cmake --build . --target install


echo -e "\nbuilding tclap done"
