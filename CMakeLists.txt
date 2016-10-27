cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

# Get Version
file(READ version.txt VERSION_TXT)
string(REPLACE "\n" "" VERSION_TXT ${VERSION_TXT})
add_definitions(-DVERSION_TXT="${VERSION_TXT}")


# Options
option(DETECTOR_WITH_RPI "Support work with gpio on RPi board" ON)


# 3rd
add_subdirectory(3rd)


# Detector
project(detector VERSION ${VERSION_TXT})
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib -std=c++11")

if (DETECTOR_WITH_RPI)
    message(STATUS "With RPi support")
    add_definitions(-DWITH_RPI=1)
    include_directories("${BCM2835_INCLUDE}")
else()
    message(WARNING "Without RPi support")
endif()

include_directories(.)
add_subdirectory(app)
add_subdirectory(lib)


set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "A.Arkhangelsky <alexei.developer@gmail.com>")
set(CPACK_GENERATOR "DEB")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
    ${CMAKE_INSTALL_PREFIX}
    ${CMAKE_INSTALL_PREFIX}/bin
    ${CMAKE_INSTALL_PREFIX}/lib
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/share
    /usr/lib/python3.5
    /usr/lib/python3.5/site-packages
    /usr/lib/python2.7
    /usr/lib/python2.7/site-packages)
include(CPack)
