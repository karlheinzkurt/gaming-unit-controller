#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/CodeLite
SOURCE_DIR=$DIR/Source

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   cmake \
   -DCMAKE_BUILD_TYPE=Debug\
   -G"CodeLite - Unix Makefiles"\
    $SOURCE_DIR 
popd

