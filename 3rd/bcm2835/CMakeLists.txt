project(bcm2835)

set(BCM2835_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/" CACHE STRING "")

add_library(${PROJECT_NAME} STATIC src/bcm2835.h src/bcm2835.c)
