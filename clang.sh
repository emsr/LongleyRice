#! /bin/bash

make -f Makefile.linux ABI=32 CXX_BIN=clang++ clean
make -f Makefile.linux ABI=32 CXX_BIN=clang++
make -f Makefile.linux ABI=32 CXX_BIN=clang++ check

make -f Makefile.linux ABI=64 CXX_BIN=clang++ clean
make -f Makefile.linux ABI=64 CXX_BIN=clang++
make -f Makefile.linux ABI=64 CXX_BIN=clang++ check

