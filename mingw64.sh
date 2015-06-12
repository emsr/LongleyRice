#! /bin/bash

make -f Makefile.mingw64 ABI=32 clean
make -f Makefile.mingw64 ABI=32
make -f Makefile.mingw64 ABI=32 check

make -f Makefile.mingw64 ABI=64 clean
make -f Makefile.mingw64 ABI=64
make -f Makefile.mingw64 ABI=64 check

