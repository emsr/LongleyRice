
DLL_EXT = .dll
EXE_EXT = .exe

all: orig new


# new...

new: RunITM$(EXE_EXT) libITM$(DLL_EXT)

ITM.o: ITM.h ITM.cpp
	g++ -c -fPIC -o ITM.o ITM.cpp

libITM$(DLL_EXT): ITM.o
	g++ -shared -fPIC -o libITM$(DLL_EXT) ITM.o

RunITM.o: RunITM.cpp
	g++ -c -o RunITM.o RunITM.cpp

RunITM$(EXE_EXT): RunITM.o libITM$(DLL_EXT)
	g++ -o RunITM$(EXE_EXT) RunITM.o -L. -lITM


# original...

orig: RunITMDLL$(EXE_EXT) libITMDLL$(DLL_EXT)

ITMDLL.o: ITMDLL.cpp
	g++ -c -fPIC -o ITMDLL.o ITMDLL.cpp

libITMDLL$(DLL_EXT): ITMDLL.o
	g++ -shared -fPIC -o libITMDLL$(DLL_EXT) ITMDLL.o

RunITMDLL.o: RunITMDLL.cpp
	g++ -c -o RunITMDLL.o RunITMDLL.cpp

RunITMDLL$(EXE_EXT): RunITMDLL.o libITMDLL$(DLL_EXT)
	g++ -o RunITMDLL$(EXE_EXT) RunITMDLL.o -L. -lITMDLL


clean:
	rm -f *.o *.dll *.so *.exe

