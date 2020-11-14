#! /bin/bash

make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=32 clean
make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=32
make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=32 check

make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=64 clean
make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=64
make -f Makefile.linux CXX_DIR=/home/$USER/bin ABI=64 check

