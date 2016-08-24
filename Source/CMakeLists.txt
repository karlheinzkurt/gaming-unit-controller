cmake_minimum_required(VERSION 2.8.11)
project(GamingUnitController)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  SET(CMAKE_BUILD_TYPE "Debug")
  SET(COVERAGE_COMPILE_FLAGS -fPIC -O0 -fprofile-arcs -ftest-coverage)
  SET(COVERAGE_LINK_FLAGS -fprofile-arcs -ftest-coverage)
  MESSAGE(STATUS "Configure build for coverage analysis")
ENDIF()

GET_FILENAME_COMPONENT( SOURCE_ROOT . ABSOLUTE )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin)

add_compile_options(--std=c++14)
add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(${COVERAGE_COMPILE_FLAGS})

link_libraries(${COVERAGE_LINK_FLAGS})
include_directories(${SOURCE_ROOT})

FIND_PACKAGE(Boost 1.45.0 REQUIRED COMPONENTS system filesystem program_options)

# For the following gtest/gmock stuff see: http://www.kaizou.org/2014/11/gtest-cmake/
# This downloads, builds and provides gtest/gmock
find_package(Threads REQUIRED)   # We need thread support
include(ExternalProject)         # Enable ExternalProject CMake module

ExternalProject_Add(
    gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
    # Disable install step
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gmock source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

add_library(libgmock IMPORTED STATIC GLOBAL)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

SET(GMOCK_INCLUDE_DIRECTORIES
   "${source_dir}/include"
   "${source_dir}/gtest/include")

SET(GMOCK_ALL_LIBRARIES 
   "${binary_dir}/gtest/libgtest.a" 
   "${binary_dir}/gtest/libgtest_main.a" 
   "${binary_dir}/libgmock.a" 
   "${binary_dir}/libgmock_main.a"
   pthread)

add_subdirectory(Lib)
add_subdirectory(App)
