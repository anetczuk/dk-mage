#!/bin/bash

set -eu


script_path=$(cd ${0%/*} && pwd -P)


$script_path/adikted/build.sh

$script_path/catch2/build.sh


echo "building done"
