#! /bin/bash

dir=itm64

rm -rf $dir
mkdir $dir
cd $dir
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$nproc
cd ..
