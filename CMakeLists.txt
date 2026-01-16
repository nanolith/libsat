cmake_minimum_required(VERSION 3.22)
cmake_policy(SET CMP0048 NEW)
PROJECT(libsat VERSION 0.0.1 LANGUAGES C CXX)

string(REPLACE "." ";" LIBSAT_VERSION_LIST "${CMAKE_PROJECT_VERSION}")
list(GET LIBSAT_VERSION_LIST 0 LIBSAT_VERSION_MAJOR)
list(GET LIBSAT_VERSION_LIST 1 LIBSAT_VERSION_MINOR)
list(GET LIBSAT_VERSION_LIST 2 LIBSAT_VERSION_REL)

INCLUDE(CheckSymbolExists)

#pkgconfig
find_package(PkgConfig REQUIRED)
#minunit package
pkg_search_module(minunit REQUIRED IMPORTED_TARGET minunit)
#rcpr package
pkg_search_module(rcpr REQUIRED IMPORTED_TARGET rcpr)

#Build config.h
configure_file(config.h.cmake include/libsat/config.h)

#source files
file(GLOB_RECURSE LIBSAT_SOURCES src/*.c)

#test files
file(GLOB_RECURSE LIBSAT_TEST_SOURCES test/*.cpp)

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)

ADD_LIBRARY(sat STATIC
    ${LIBSAT_SOURCES})
TARGET_COMPILE_OPTIONS(
    sat PRIVATE -fPIC -O2 ${rcpr_CFLAGS}
    -Wall -Werror -Wextra -Wpedantic -Wno-unused-command-line-argument)

ADD_LIBRARY(sat-${CMAKE_PROJECT_VERSION} SHARED
    ${LIBSAT_SOURCES})
TARGET_COMPILE_OPTIONS(
    sat-${CMAKE_PROJECT_VERSION} PRIVATE -fPIC -O2
    -Wall -Werror -Wextra -Wpedantic -Wno-unused-command-line-argument)
TARGET_LINK_LIBRARIES(
    sat-${CMAKE_PROJECT_VERSION} PRIVATE PkgConfig::rcpr)

ADD_EXECUTABLE(testlibsat
    ${LIBSAT_SOURCES} ${LIBSAT_TEST_SOURCES})
TARGET_COMPILE_OPTIONS(
    testlibsat PRIVATE -g -O0 --coverage ${minunit_CFLAGS} ${rcpr_CFLAGS}
                       -Wall -Werror -Wextra -Wpedantic
                       -Wno-unused-command-line-argument)
TARGET_LINK_OPTIONS(testlibsat PRIVATE --coverage)
TARGET_LINK_LIBRARIES(
    testlibsat PRIVATE -g -O0 --coverage PkgConfig::minunit PkgConfig::rcpr)
set_source_files_properties(
    ${LIBSAT_TEST_SOURCES} PROPERTIES
    COMPILE_FLAGS "${STD_CXX_20}")

ADD_CUSTOM_TARGET(
    test
    COMMAND testlibsat
    DEPENDS testlibsat
    USES_TERMINAL)

#Build a pkg-config file
SET(LIBSAT_PC "${CMAKE_BINARY_DIR}/libsat.pc")
FILE(WRITE  ${LIBSAT_PC} "Name: libsat")
FILE(APPEND ${LIBSAT_PC} "\nDescription: Simple DPLL based SAT library")
FILE(APPEND ${LIBSAT_PC} "\nVersion: ${CMAKE_PROJECT_VERSION}")
FILE(APPEND ${LIBSAT_PC} "\nURL: https://github.com/nanolith/libsat")
FILE(APPEND ${LIBSAT_PC} "\nprefix=\${pcfiledir}/../..")
FILE(APPEND ${LIBSAT_PC} "\nlibdir=\${prefix}/lib")
FILE(APPEND ${LIBSAT_PC} "\nincludedir=\${prefix}/include")
FILE(APPEND ${LIBSAT_PC} "\nLibs: -L\${libdir} -lsat")
FILE(APPEND ${LIBSAT_PC} "\nCflags: -I\${includedir}")
INSTALL(FILES ${LIBSAT_PC} DESTINATION lib/pkgconfig)

#Install headers
INSTALL(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION include)
INSTALL(FILES ${CMAKE_BINARY_DIR}/include/libsat/config.h
    DESTINATION include/libsat)

#Install library
INSTALL(TARGETS sat sat-${CMAKE_PROJECT_VERSION}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
