cmake_minimum_required (VERSION 3.20)

project (ITModel
         VERSION 1.0.0
         LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ABI 64 CACHE STRING "The ABI of the ITM libraries and executables.")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ABI 32 CACHE STRING "The ABI of the ITM libraries and executables.")
endif ()
message("ABI                : ${ABI}")

string(APPEND CMAKE_ADDTIONAL_CXX_FLAGS " -m${ABI}")
string(APPEND CMAKE_SHARED_LINKER_FLAGS " -m${ABI}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -m${ABI}")

# Enable testing for all systems
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_ADDTIONAL_CXX_FLAGS} -Wall -Wextra -Wno-deprecated-declarations -Wold-style-cast -std=c++14")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Profile.txt" "${CMAKE_CURRENT_BINARY_DIR}/Profile.txt" COPYONLY)

add_library(ITM SHARED ITM.cpp)
target_include_directories(ITM PUBLIC  
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)

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

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/ITMConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/ITMConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ITM
)

install(FILES
        "${PROJECT_BINARY_DIR}/ITMConfig.cmake"
        DESTINATION lib/cmake/ITM)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/ITM.h"
        DESTINATION include
        )

install(TARGETS ITM RunITModel
  EXPORT ITMTargets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  INCLUDES DESTINATION include
)

install(EXPORT ITMTargets DESTINATION lib/cmake/ITM)
