#!/bin/bash

set -eu


SCRIPT_DIR=$(cd ${0%/*} && pwd -P)


cd "$SCRIPT_DIR"


SRC_ARCHIVE=ADiKtEd-master.zip


SRC_DIR=$(basename -s .zip $SRC_ARCHIVE)
SRC_ROOT_PATH="$SCRIPT_DIR/src"
SRC_PATH="$SRC_ROOT_PATH/${SRC_DIR}"
SRC_LIB_PATH="$SRC_PATH/libadikted"

BUILD_ROOT_PATH="$SCRIPT_DIR/build"
BUILD_PATH="$BUILD_ROOT_PATH/$SRC_DIR"
BUILD_LIB_PATH="$BUILD_PATH/libadikted"

INSTALL_PATH="$SCRIPT_DIR/install"


unzip_src_makefile() {
    echo "extracting and overwriting sources"

    rm -rf "$INSTALL_PATH"
    
    mkdir -p "$INSTALL_PATH"
    mkdir -p "$INSTALL_PATH/include/libadikted"
    

    unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$BUILD_ROOT_PATH"
    
    ## copy fixes
    if compgen -G "$SCRIPT_DIR/fix/*" > /dev/null; then
        cp -r $SCRIPT_DIR/fix/* "$BUILD_PATH"
    fi
    
    cp $BUILD_LIB_PATH/*.h "$INSTALL_PATH/include/libadikted"
}

# unzip_src_makefile


unzip_src() {
    echo "extracting and overwriting sources"

    rm -rf "$SRC_PATH"
    rm -rf "$INSTALL_PATH"
    
    mkdir -p "$SRC_PATH"
    

    unzip -o "$SCRIPT_DIR/$SRC_ARCHIVE" -d "$SRC_ROOT_PATH"
    
    ## copy fixes
    if compgen -G "$SCRIPT_DIR/fix/*" > /dev/null; then
        cp -r $SCRIPT_DIR/fix/* "$SRC_PATH"
    fi
}

unzip_src



build_windws_makefile() {
    ## building for Windows
    echo "building for Windows"

    cd "$BUILD_LIB_PATH"
   
    mkdir -p "$INSTALL_PATH"
    mkdir -p "$INSTALL_PATH/lib"
   
    make clean
    make all
    
    cp "$BUILD_LIB_PATH/bin/adikted.dll" "$INSTALL_PATH/lib" && true
    
    
    make clean
    make all DEBUG=1
    
    cp "$BUILD_LIB_PATH/bin/adikted.dll" "$INSTALL_PATH/lib/adikted_d.dll" && true
}

# build_windws_makefile


build_linux_makefile() {
    ## building for Linux
    echo "building for Linux"
    
    cd "$BUILD_LIB_PATH"
    
    mkdir -p "$INSTALL_PATH"
    mkdir -p "$INSTALL_PATH/lib"
    
    make clean
    make all BUILD_FOR_LINUX=1
    
    cp "$BUILD_LIB_PATH/bin/adikted.so" -T "$INSTALL_PATH/lib/libadikted.so" && true
    
    
    make clean
    make all BUILD_FOR_LINUX=1 DEBUG=1
    
    cp "$BUILD_LIB_PATH/bin/adikted.so" -T "$INSTALL_PATH/lib/libadikted_d.so" && true
}

# build_linux_makefile


build_linux_cmake() {
    ## building for Linux
    echo "building for Linux"
    
    #make all BUILD_FOR_LINUX=1 DEBUG=1
        
    local BUILD_TARGET_DIR="${BUILD_PATH}_lin"
    rm -rf "$BUILD_TARGET_DIR"
    mkdir -p "$BUILD_TARGET_DIR"
    cd "$BUILD_TARGET_DIR"
    
    cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" $SRC_LIB_PATH
    
    cmake --build . --target install
}

#build_linux_cmake


echo -e "\nbuilding adikted done"
