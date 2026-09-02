#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Release -B build-release
cmake --build build-release --parallel
