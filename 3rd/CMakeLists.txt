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

patch_3rd(opencv opencv-cmake_install_prefix.patch)
patch_3rd(opencv_contrib opencv_contrib-cmake_install_prefix.patch)


# OpenCV
set(WITH_QT        ON CACHE BOOL "Build with Qt Backend support")
set(OPENCV_EXTRA_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/opencv_contrib/modules")
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
      cp -rf ${CMAKE_SOURCE_DIR}/config/* config/
  COMMAND
      sed "s#@PREFIX@#${CMAKE_INSTALL_PREFIX}#g" config/motion/motion.conf.in > config/motion/motion.conf
  COMMAND
      rm -f config/motion/motion.conf.in
)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/3rd/motion/motion DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/config/ DESTINATION ${CMAKE_INSTALL_PREFIX}/etc)


# bcm2835 (http://www.airspayce.com/mikem/bcm2835/)
if (DETECTOR_WITH_RPI)
    add_custom_target(bcm2835 ALL
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}

      # bin
      COMMAND
        rm -rf bcm2835 || true
      COMMAND
        cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/bcm2835 .
      COMMAND
        cd bcm2835/ && ./configure --prefix="${CMAKE_INSTALL_PREFIX}" && make
    )

    set(BCM2835_LIBS "${CMAKE_CURRENT_BINARY_DIR}/bcm2835/src/libbcm2835.a" PARENT_SCOPE)
    set(BCM2835_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/bcm2835/src/" PARENT_SCOPE)
endif()
