cmake_minimum_required(VERSION 3.0)
include(../cmake/detector.cmake)

project(detector_3rdparty VERSION ${DETECTOR_VERSION})


# Patch
macro(patch_3rd name name_patch)
    message(STATUS "Begin patch 3rd: ${name}")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${name}/
        COMMAND patch -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/patch/${name_patch}
        RESULT_VARIABLE RESULT_PATCH
    )
    if ("${RESULT_PATCH}" STREQUAL "0" OR "${RESULT_PATCH}" STREQUAL "1")
        message(STATUS "Success patched 3rd: ${name}")
    else()
        message(FATAL_ERROR "Error patched ${name} with file ${name_patch}")
    endif()
endmacro()


# OpenCV
patch_3rd(opencv opencv-cmake_install_prefix.patch)
patch_3rd(opencv_contrib opencv_contrib-cmake_install_prefix.patch)

set(WITH_QT          ON  CACHE BOOL "Build with Qt Backend support")
set(BUILD_PERF_TESTS OFF CACHE BOOL "Build performance tests")
set(BUILD_TESTS      OFF CACHE BOOL "Build accuracy & regression tests")
set(BUILD_opencv_dnn OFF CACHE BOOL "Error: Not find file in version 3.2")
set(OPENCV_EXTRA_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/opencv_contrib/modules" CACHE STRING "Contrib must have" FORCE)
add_subdirectory(opencv)


# motion
add_custom_target(motion ALL
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}

  # bin
  COMMAND
    cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/motion .
  COMMAND
    cd motion && autoreconf -fiv && ./configure --prefix=${CMAKE_INSTALL_PREFIX} && make

  # config
  COMMAND
      mkdir -p config
  COMMAND
      cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/config/* config/
  COMMAND
      sed "s#@PREFIX@#${CMAKE_INSTALL_PREFIX}#g" config/motion/motion.conf.in > config/motion/motion.conf
  COMMAND
      rm -f config/motion/motion.conf.in
)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/motion/motion DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/config/ DESTINATION ${CMAKE_INSTALL_PREFIX}/etc)


# bcm2835 (http://www.airspayce.com/mikem/bcm2835/)
if (DETECTOR_WITH_RPI)
    add_subdirectory(bcm2835)
endif()


set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "A.Arkhangelsky <alexei.developer@gmail.com>")
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

if(EXISTS /etc/debian_version)
    set(CPACK_GENERATOR "DEB")
elseif(EXISTS /etc/redhat-release)
    set(CPACK_GENERATOR "RPM")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
