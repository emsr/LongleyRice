#! /bin/bash

abi=32
dir=debug$abi

rm -rf $dir
mkdir $dir
cd $dir
cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Debug ..
make -j$nproc
cd ..
