
all: RunITM libITM.so

libITM.o: ITMDLL.cpp
	g++ -c -fPIC -o libITM.o ITMDLL.cpp

libITM.so: libITM.o
	g++ -shared -fPIC -o libITM.so libITM.o

RunITM.o: RunITM.cpp
	g++ -c -o RunITM.o RunITM.cpp

RunITM: RunITM.o
	g++ -o RunITM RunITM.o -L. -lITM

