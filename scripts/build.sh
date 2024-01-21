#! /bin/bash

g++ -g -shared -o libITMDLL.so ITMDLL.cpp
g++ -g -o RunITMDLL RunITMDLL.cpp -L. -lITMDLL
LD_LIBRARY_PATH=. ./RunITMDLL < itm_1.in

g++ -g -shared -fPIC -o libITM.so ITM.cpp
g++ -g -o RunITM RunITM.cpp -L. -lITM
LD_LIBRARY_PATH=. ./RunITM < itm_1.in
