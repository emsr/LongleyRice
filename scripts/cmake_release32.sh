#! /bin/bash

abi=32
dir=itm$abi

rm -rf $dir
mkdir $dir
cd $dir
cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Release ..
make -j$nproc
cd ..
