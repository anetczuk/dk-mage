#!/bin/bash

set -eu


script_path=$(cd ${0%/*} && pwd -P)


$script_path/tclap/build.sh


echo "building done"
