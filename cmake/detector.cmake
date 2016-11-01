cmake_minimum_required(VERSION 3.0)


# Get Version
file(READ ${CMAKE_CURRENT_LIST_DIR}/../version.txt DETECTOR_VERSION)
string(REPLACE "\n" "" DETECTOR_VERSION ${DETECTOR_VERSION})
add_definitions(-DDETECTOR_VERSION="${DETECTOR_VERSION}")


# Options

# RPi support
if(NOT DETECTOR_WITH_RPI)
    if(EXISTS /opt/vc/include/bcm_host.h)
        set(DETECTOR_WITH_RPI ON CACHE BOOL "Support work with gpio on RPi board")
    endif()
endif()
option(DETECTOR_WITH_RPI "Support work with gpio on RPi board" OFF)

