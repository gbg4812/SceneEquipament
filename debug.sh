#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=DEBUG -B build
pushd build/
make "$1"
gdb ./"$1"
popd
