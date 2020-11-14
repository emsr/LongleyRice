#! /bin/bash

make -f Makefile.linux ABI=32 clean
make -f Makefile.linux ABI=32
make -f Makefile.linux ABI=32 check

make -f Makefile.linux ABI=64 clean
make -f Makefile.linux ABI=64
make -f Makefile.linux ABI=64 check

