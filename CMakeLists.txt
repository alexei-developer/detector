cmake_minimum_required(VERSION 3.0)

# Detector
cmake_policy(SET CMP0048 NEW)
include(cmake/detector.cmake)

project(detector VERSION ${DETECTOR_VERSION})
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,${CMAKE_INSTALL_PREFIX}/lib -std=c++11")

if(DETECTOR_WITH_RPI)
    message(STATUS "With RPi support")
    add_definitions(-DWITH_RPI=1)
else()
    message(WARNING "Without RPi support")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
add_subdirectory(apps)
add_subdirectory(libs)


set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "A.Arkhangelsky <alexei.developer@gmail.com>")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
    ${CMAKE_INSTALL_PREFIX}
    ${CMAKE_INSTALL_PREFIX}/bin
    ${CMAKE_INSTALL_PREFIX}/lib
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/share)

if(EXISTS /etc/debian_version)
    set(CPACK_GENERATOR "DEB")
elseif(EXISTS /etc/redhat-release)
    set(CPACK_GENERATOR "RPM")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
