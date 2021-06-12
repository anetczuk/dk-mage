#!/bin/bash

set -eu


script_path=$(cd ${0%/*} && pwd -P)


$script_path/adikted/build.sh

$script_path/catch2/build.sh

$script_path/lemon/build.sh

$script_path/tclap/build.sh

$script_path/inih/build.sh


echo "building done"
