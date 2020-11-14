#! /bin/bash

make -f Makefile.cygwin ABI=32 clean
make -f Makefile.cygwin ABI=32
make -f Makefile.cygwin ABI=32 check

make -f Makefile.cygwin ABI=64 clean
make -f Makefile.cygwin ABI=64
make -f Makefile.cygwin ABI=64 check

