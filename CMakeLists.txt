cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

# Get Version
file(READ version.txt VERSION_TXT)
string(REPLACE "\n" "" VERSION_TXT ${VERSION_TXT})


project(detector VERSION ${VERSION_TXT})
message(STATUS "Project version: ${PROJECT_VERSION}")


# Options
option(DETECTOR_WITH_RPI OFF "Support work with gpio on RPi board")


include_directories(.)
add_subdirectory(3rd)
add_subdirectory(app)
add_subdirectory(lib)


set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "A.Arkhangelsky <alexei.developer@gmail.com>")
set(CPACK_GENERATOR "DEB")
include(CPack)
