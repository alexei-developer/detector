cmake_minimum_required(VERSION 3.5)

project(detector VERSION 0.1)


# Patch
macro(patch_3rd name name_patch)
    message(STATUS "Begin patch 3rd: ${name}")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${name}/
        COMMAND patch -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/3rd/patch/${name_patch}
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


# Config
add_custom_target(config ALL
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND
        mkdir -p config
    COMMAND
        cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/config/* config/
    COMMAND
        sed "s#@CMAKE_INSTALL_PREFIX@#${CMAKE_INSTALL_PREFIX}#g" config/motion/motion.conf.in > config/motion/motion.conf
    COMMAND
        rm -f config/motion/motion.conf.in
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/config/ DESTINATION ${CMAKE_INSTALL_PREFIX}/etc)


# OpenCV
set(WITH_QT        ON CACHE BOOL "Build with Qt Backend support")
set(BUILD_EXAMPLES ON CACHE BOOL "Build all examples")
set(OPENCV_EXTRA_MODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd/opencv_contrib/modules")
add_subdirectory(3rd/opencv)


include_directories(.)
add_subdirectory(app)
add_subdirectory(lib)


set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "A.Arkhangelsky <alexei.developer@gmail.com>")
set(CPACK_GENERATOR "DEB")
include(CPack)
