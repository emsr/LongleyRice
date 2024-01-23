#! /bin/bash

g++ -g -shared -o libITMDLL.so ITMDLL.cpp
g++ -g -o RunITMDLL RunITMDLL.cpp -L. -lITMDLL
LD_LIBRARY_PATH=. ./RunITMDLL < itm_1.in

g++ -g -shared -fPIC -o libLongleyRice.so LongleyRice.cpp
g++ -g -o RunLongleyRice RunLongleyRice.cpp -L. -lLongleyRice
LD_LIBRARY_PATH=. ./RunLongleyRice < itm_1.in

LD_LIBRARY_PATH=. ./RunLongleyRice tls < test1.in > test1.out
LD_LIBRARY_PATH=. ./RunLongleyRice tls < test2.in > test2.out
LD_LIBRARY_PATH=. ./RunLongleyRice tls < test3.in > test3.out
LD_LIBRARY_PATH=. ./RunLongleyRice tls < test4.in > test4.out
LD_LIBRARY_PATH=. ./RunLongleyRice tls < test5.in > test5.out
