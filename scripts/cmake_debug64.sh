#! /bin/bash

dir=debug64

rm -rf $dir
mkdir $dir
cd $dir
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$nproc
cd ..
