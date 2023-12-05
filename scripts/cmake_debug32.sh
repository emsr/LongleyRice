#! /bin/bash

abi=32
build_dir=build/debug$abi

# cmake-3.24 has a --fresh
rm -rf $build_dir

cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Debug -B $build_dir
cmake --build $build_dir -j$nproc
cmake --build $build_dir --target test
