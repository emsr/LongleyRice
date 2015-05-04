

OUT_BASE = itm
ABI = 64


OUT_DIR = $(OUT_BASE)$(ABI)
LIB_DIR = $(OUT_DIR)
BIN_DIR = $(OUT_DIR)
INC_DIR = .
SRC_DIR = .
OBJ_DIR = $(OUT_DIR)/obj
INST_DIR = .
INST_INC_DIR = $(INST_DIR)/include
ifeq ($(ABI),64)
  INST_LIB_DIR = $(INST_DIR)/lib64
else
  INST_LIB_DIR = $(INST_DIR)/lib
endif
INST_BIN_DIR = $(INST_DIR)/bin
CD_DIR = CD
PROD_DIR = $(CD_DIR)/itm-$(VERSION_TIREM)
OS_DIR = $(PROD_DIR)/mingw64
ABI_DIR = $(OS_DIR)/$(ABI)
DOCS_DIR = $(PROD_DIR)/docs
HTML_DIR = $(DOCS_DIR)/html

COPY = cp -f
SED = sed
#AR = ar
#RANLIB = ranlib
#CXX_DIR = /home/ed/bin
CXX_DIR = /usr
CXX_BIN_DIR = $(CXX_DIR)/bin
#CXX_OPTS = -fdiagnostics-color=always
CXX = $(CXX_BIN_DIR)/g++ -std=c++1y $(CXX_OPTS)
ifeq ($(ABI),64)
  CXX_LIB_DIR = /usr/x86_64-w64-mingw32/sys-root/mingw/bin
  CXX = $(CXX_BIN_DIR)/x86_64-w64-mingw32-g++ -std=c++1y $(CXX_OPTS)
  AR = $(CXX_BIN_DIR)/x86_64-w64-mingw32-ar
  RANLIB = $(CXX_BIN_DIR)/x86_64-w64-mingw32-ranlib
else
  CXX_LIB_DIR = /usr/i686-w64-mingw32/sys-root/mingw/bin
  CXX = $(CXX_BIN_DIR)/i686-w64-mingw32-g++ -std=c++1y $(CXX_OPTS)
  AR = $(CXX_BIN_DIR)/i686-w64-mingw32-ar
  RANLIB = $(CXX_BIN_DIR)/i686-w64-mingw32-ranlib
endif
OPT = -O3
#OPT = -g
CXXFLAGS = -Wall -Wextra -Wno-deprecated-declarations $(OPT) -m$(ABI) -c
OFLAGS = -Wall -Wextra $(OPT) -m$(ABI) -static-libgcc -static-libstdc++
LIB_DL = 
#  These flags control library symbol visibility.
VIS_DEFAULT = -fvisibility=default
VIS_HIDDEN = -fvisibility=hidden
DLL_EXT = .dll
EXE_EXT = .exe
LIB_EXT = .lib
OBJ_EXT = .o
DLL_PRE = 

INCS = \
  $(INC_DIR)/ITM.h

SRCS = \
  $(SRC_DIR)/ITM.cpp \
  $(SRC_DIR)/ITMDLL.cpp

BIN_SRCS = \
  $(SRC_DIR)/RunITM.cpp \
  $(SRC_DIR)/RunITMDLL.cpp

BINS = \
  $(BIN_DIR)/RunITM$(EXE_EXT) \
  $(BIN_DIR)/RunITMDLL$(EXE_EXT)

all: $(OBJ_DIR) \
     orig new


# new...

new: $(BIN_DIR)/RunITM$(EXE_EXT) $(LIB_DIR)/libITM$(DLL_EXT)

$(OBJ_DIR)/ITM$(OBJ_EXT): ITM.h $(SRC_DIR)/ITM.cpp
	g++ -c -fPIC -o $(OBJ_DIR)/ITM$(OBJ_EXT) ITM.cpp

$(LIB_DIR)/libITM$(DLL_EXT): $(OBJ_DIR)/ITM$(OBJ_EXT)
	g++ -shared -fPIC -o $(LIB_DIR)/libITM$(DLL_EXT) $(OBJ_DIR)/ITM$(OBJ_EXT)

$(OBJ_DIR)/RunITM$(OBJ_EXT): $(SRC_DIR)/RunITM.cpp
	g++ -c -o $(OBJ_DIR)/RunITM$(OBJ_EXT) RunITM.cpp

$(BIN_DIR)/RunITM$(EXE_EXT): $(OBJ_DIR)/RunITM$(OBJ_EXT) $(LIB_DIR)/libITM$(DLL_EXT)
	g++ -o $(BIN_DIR)/RunITM$(EXE_EXT) $(OBJ_DIR)/RunITM$(OBJ_EXT) -L$(LIB_DIR) -lITM


# original...

orig: $(BIN_DIR)/RunITMDLL$(EXE_EXT) $(LIB_DIR)/libITMDLL$(DLL_EXT)

$(OBJ_DIR)/ITMDLL$(OBJ_EXT): $(SRC_DIR)/ITMDLL.cpp
	g++ -c -fPIC -o $(OBJ_DIR)/ITMDLL$(OBJ_EXT) $(SRC_DIR)/ITMDLL.cpp

$(LIB_DIR)/libITMDLL$(DLL_EXT): $(OBJ_DIR)/ITMDLL$(OBJ_EXT)
	g++ -shared -fPIC -o $(LIB_DIR)/libITMDLL$(DLL_EXT) $(OBJ_DIR)/ITMDLL$(OBJ_EXT)

$(OBJ_DIR)/RunITMDLL$(OBJ_EXT): $(SRC_DIR)/RunITMDLL.cpp
	g++ -c -o $(OBJ_DIR)/RunITMDLL$(OBJ_EXT) $(SRC_DIR)/RunITMDLL.cpp

$(BIN_DIR)/RunITMDLL$(EXE_EXT): $(OBJ_DIR)/RunITMDLL$(OBJ_EXT) $(LIB_DIR)/libITMDLL$(DLL_EXT)
	g++ -o $(BIN_DIR)/RunITMDLL$(EXE_EXT) $(OBJ_DIR)/RunITMDLL$(OBJ_EXT) -L$(LIB_DIR) -lITMDLL


clean:
	rm -rf $(OUT_DIR)/obj
	rm -f $(OUT_DIR)/*


tarball:
	rm -rf itm_cpp
	mkdir itm_cpp
	cp Makefile $(INCS) $(SRCS) $(BIN_SRCS) *.dat *.in *.txt TODO README Doxyfile itm_cpp
	tar -cvf itm_cpp.tar itm_cpp
	bzip2 -f itm_cpp.tar
	rm -rf itm_cpp


$(OUT_DIR):
	if test ! -d $(OUT_DIR); then \
	  mkdir $(OUT_DIR); \
	fi

$(OBJ_DIR): $(OUT_DIR)
	if test ! -d $(OBJ_DIR); then \
	  mkdir $(OBJ_DIR); \
	fi

