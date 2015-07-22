cmake_minimum_required (VERSION 3.0)

project (ITModel)
set (ITModel_VERSION_MAJOR 1)
set (ITModel_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -Wold-style-cast -std=c++14")

add_library(ITM SHARED ITM.h ITM.cpp)

add_executable(RunITModel RunITModel.cpp)
target_link_libraries(RunITModel ITM)

add_executable(TestITModel TestITModel.cpp)
target_link_libraries(TestITModel ITM)

add_library(ITMDLL SHARED ITMDLL.cpp)

add_executable(RunITModelOld RunITModelOld.cpp)
target_link_libraries(RunITModelOld ITMDLL)

add_executable(TestITModelOld TestITModelOld.cpp)
target_link_libraries(TestITModelOld ITMDLL)
