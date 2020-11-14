cmake_minimum_required (VERSION 3.0)

project (ITModel)
set (ITModel_VERSION_MAJOR 1)
set (ITModel_VERSION_MINOR 0)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ABI 64 CACHE STRING "The ABI of the VLF libraries and executables.")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ABI 32 CACHE STRING "The ABI of the VLF libraries and executables.")
endif ()
message("ABI                : ${ABI}")

string(APPEND CMAKE_ADDTIONAL_CXX_FLAGS " -m${ABI}")
string(APPEND CMAKE_SHARED_LINKER_FLAGS " -m${ABI}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -m${ABI}")

# Enable testing for all systems
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_ADDTIONAL_CXX_FLAGS} -Wall -Wextra -Wno-deprecated-declarations -Wold-style-cast -std=c++14")

add_library(ITM SHARED ITM.h ITM.cpp)

add_executable(RunITModel RunITModel.cpp)
target_link_libraries(RunITModel ITM)
add_test(NAME RunITModel COMMAND bash -c "./RunITModel < ${CMAKE_CURRENT_SOURCE_DIR}/itm_1.in > itm_1.out")

add_executable(TestITModel TestITModel.cpp)
target_link_libraries(TestITModel ITM)
add_test(NAME TestITModel COMMAND bash -c "./TestITModel > TestITModel.out")

add_library(ITMDLL SHARED ITMDLL.cpp)

add_executable(RunITModelOld RunITModelOld.cpp)
target_link_libraries(RunITModelOld ITMDLL)
add_test(NAME RunITModelOld COMMAND bash -c "./RunITModelOld < ${CMAKE_CURRENT_SOURCE_DIR}/itm_1.in > itm_old_1.out")

add_executable(TestITModelOld TestITModelOld.cpp)
target_link_libraries(TestITModelOld ITMDLL)
add_test(NAME TestITModelOld COMMAND bash -c "./TestITModelOld > TestITModelOld.out")
