#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Debug -B build-debug
cmake --build build-debug --parallel
