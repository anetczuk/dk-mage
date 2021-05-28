#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=ADiKtEd-master.zip


SRC_DIR=$(basename -s .zip $SRC_ARCHIVE)
BUILD_PATH="$SCRIPT_DIR/build/$SRC_DIR"
INSTALL_PATH="$SCRIPT_DIR/install"
LIB_PATH="$BUILD_PATH/libadikted"

rm -rf "$BUILD_PATH"
rm -rf "$INSTALL_PATH"

mkdir -p "$BUILD_PATH"
mkdir -p "$INSTALL_PATH"
mkdir -p "$INSTALL_PATH/include/libadikted"
mkdir -p "$INSTALL_PATH/lib"


unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$SCRIPT_DIR/build"

## cpy fixes
cp $SCRIPT_DIR/fix/* "$LIB_PATH"


cd "$LIB_PATH"


cp $LIB_PATH/*.h "$INSTALL_PATH/include/libadikted"


## building for Windows
make clean
make all

cp "$LIB_PATH/bin/adikted.dll" "$INSTALL_PATH/lib" && true


make clean
make all DEBUG=1

cp "$LIB_PATH/bin/adikted.dll" "$INSTALL_PATH/lib/adikted_d.dll" && true



## building for Linux
make clean
make all BUILD_FOR_LINUX=1

cp "$LIB_PATH/bin/adikted.so" -T "$INSTALL_PATH/lib/libadikted.so" && true


make clean
make all BUILD_FOR_LINUX=1 DEBUG=1

cp "$LIB_PATH/bin/adikted.so" -T "$INSTALL_PATH/lib/libadikted_d.so" && true


echo "building adikted done"
