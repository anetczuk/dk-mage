#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=ADiKtEd-master.zip


SRC_DIR=$(basename -s .zip $SRC_ARCHIVE)
BUILD_PATH="$SCRIPT_DIR/build/$SRC_DIR"
INSTALL_PATH="$SCRIPT_DIR/install"
LIB_PATH="$BUILD_PATH/libadikted"


mkdir -p "$BUILD_PATH"

unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$SCRIPT_DIR/build"

cp $SCRIPT_DIR/fix/globals.h "$LIB_PATH"


cd "$LIB_PATH"

make all BUILD_FOR_LINUX=1


mkdir -p "$INSTALL_PATH"
mkdir -p "$INSTALL_PATH/include/libadikted"
mkdir -p "$INSTALL_PATH/lib"

cp $LIB_PATH/*.h "$INSTALL_PATH/include/libadikted"
cp "$LIB_PATH/bin/adikted.so" -T "$INSTALL_PATH/lib/libadikted.so"


echo "building adikted done"
