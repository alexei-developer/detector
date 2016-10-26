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
include(CPack)
