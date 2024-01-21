cmake_minimum_required (VERSION 3.20)

project (LongleyRiceModel
         VERSION 1.0.0
         LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ABI 64 CACHE STRING "The ABI of the ITM Longley-Rice libraries and executables.")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ABI 32 CACHE STRING "The ABI of the ITM Longley-Rice libraries and executables.")
endif ()
message("ABI                : ${ABI}")

string(APPEND CMAKE_ADDTIONAL_CXX_FLAGS " -m${ABI}")
string(APPEND CMAKE_SHARED_LINKER_FLAGS " -m${ABI}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -m${ABI}")

# Enable testing for all systems
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_ADDTIONAL_CXX_FLAGS} -Wall -Wextra -Wno-deprecated-declarations -Wold-style-cast -std=c++14")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Profile.txt" "${CMAKE_CURRENT_BINARY_DIR}/Profile.txt" COPYONLY)

add_library(LongleyRice SHARED LongleyRice.cpp)
target_include_directories(LongleyRice PUBLIC  
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)

add_executable(RunLongleyRice RunLongleyRice.cpp)
target_link_libraries(RunLongleyRice LongleyRice)
add_test(NAME RunLongleyRice COMMAND bash -c "./RunLongleyRice < ${CMAKE_CURRENT_SOURCE_DIR}/itm_1.in > itm_1.out")

add_executable(TestLongleyRice TestLongleyRice.cpp)
target_link_libraries(TestLongleyRice LongleyRice)
add_test(NAME TestLongleyRice COMMAND bash -c "./TestLongleyRice > TestLongleyRice.out")

add_library(ITMDLL SHARED ITMDLL.cpp)

add_executable(RunITMDLL RunITMDLL.cpp)
target_link_libraries(RunITMDLL ITMDLL)
add_test(NAME RunITMDLL COMMAND bash -c "./RunITMDLL < ${CMAKE_CURRENT_SOURCE_DIR}/itm_1.in > itm_old_1.out")

add_executable(TestLongleyRiceOld TestLongleyRiceOld.cpp)
target_link_libraries(TestLongleyRiceOld ITMDLL)
add_test(NAME TestLongleyRiceOld COMMAND bash -c "./TestLongleyRiceOld > TestLongleyRiceOld.out")

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/LongleyRiceConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/LongleyRiceConfig.cmake"
    INSTALL_DESTINATION lib/cmake/LongleyRice
)

install(FILES
        "${PROJECT_BINARY_DIR}/LongleyRiceConfig.cmake"
        DESTINATION lib/cmake/LongleyRice)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/LongleyRice.h"
        DESTINATION include
        )

install(TARGETS LongleyRice RunLongleyRice
  EXPORT LongleyRiceTargets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  INCLUDES DESTINATION include
)

install(EXPORT LongleyRiceTargets DESTINATION lib/cmake/LongleyRice)
