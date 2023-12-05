#! /bin/bash

abi=64
build_dir=build/itm$abi

# cmake-3.24 has a --fresh
rm -rf $build_dir

cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Release -B $build_dir
cmake --build $build_dir -j$nproc
cmake --build $build_dir --target test
