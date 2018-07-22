cmake_minimum_required(VERSION 2.8.11)
project(gsc)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  SET(CMAKE_BUILD_TYPE "Debug")
  add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
  link_libraries(gcov)
  MESSAGE(STATUS "Configure build for coverage analysis")
ENDIF()

GET_FILENAME_COMPONENT( SOURCE_ROOT . ABSOLUTE )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})

add_compile_options(--std=c++14)
add_compile_options(-Wall)
add_compile_options(-Werror)

include_directories(${SOURCE_ROOT})

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
FIND_PACKAGE(Boost 1.45.0 REQUIRED COMPONENTS system filesystem program_options date_time)

find_library(Log4cxx REQUIRED log4cxx)
SET(Log4cxx_LIBRARIES log4cxx) # There is no findpackage

find_library(cpprestsdk REQUIRED cpprest)
SET(cpprest_LIBRARIES ssl crypto cpprest)

FIND_PACKAGE(GTest 1.8.0 REQUIRED)
SET(GMOCK_ALL_LIBRARIES gmock) # Not contained in GTest

add_subdirectory(gsclib)
add_subdirectory(gsc)

SET(CPACK_PACKAGE_NAME "gsc")
SET(CPACK_PACKAGE_DESCRIPTION "Tool to monitor and control gaming sessions")
SET(CPACK_PACKAGE_CONTACT "Sascha Lumma")
SET(CPACK_PACKAGE_VERSION "0.1.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_FILE_NAME "gsc-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_RESOURCE_FILE_LICENSE "${SOURCE_ROOT}/../LICENSE")
SET(CPACK_RESOURCE_FILE_README  "${SOURCE_ROOT}/../README.md")

SET(CPACK_GENERATOR "DEB")
#SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1.3), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.2.1)") 
#libexiv2.so.14 liblog4cxx.so.10 libstdc++.so.6 libgcc_s.so.1 libc.so.6
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "utils")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

# systemctl --system daemon-reload
# systemctl enable gsc
# systemctl start gsc
 
INCLUDE(CPack)

