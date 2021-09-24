#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


##
##  Prepare deployment artifacts
##


DEPLOY_SUBDIR="deploy"

DEPLOY_DIR="$SCRIPT_DIR/../build/$DEPLOY_SUBDIR"


## $1 -- build subdir
## $2 -- build script
## $3 -- artifact
## $4 -- cmake params
prepare_artifact() {
    local BUILD_SUBDIR=$1
    local BUILD_SCRIPT=$2
    local ARTIFACT=$3
    local CMAKE_PARAMS=
    if [ "$#" -ge 4 ]; then
        CMAKE_PARAMS="$4"
    fi
    
    echo "executing $SCRIPT_DIR/$BUILD_SCRIPT"
    $SCRIPT_DIR/$BUILD_SCRIPT $DEPLOY_SUBDIR/$BUILD_SUBDIR "$CMAKE_PARAMS"
    
    local TARGET_DIR=$DEPLOY_DIR/$BUILD_SUBDIR
    cd $TARGET_DIR
    
    make -j6
    ctest
    ./deploy_zip.sh
    
    cp $TARGET_DIR/deploy/$ARTIFACT $DEPLOY_DIR
}


prepare_artifact win_exe win_release_gcc.sh dkmage_*.zip

prepare_artifact lin_py lin_release_gcc.sh pydkmage_*.zip "-DBUILD_PYTHON_BINDINGS=ON"
