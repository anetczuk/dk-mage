#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


source $SCRIPT_DIR/env.sh


BUILD_TYPE=Release
BUILD_DIR_NAME=lin_$(echo $BUILD_TYPE"_gcc" | tr '[:upper:]' '[:lower:]')


parse_args $@

configure_cmake
