find_package (Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})

add_library(core STATIC core.cpp core.h)
target_link_libraries(core Qt5::Core)
#install(TARGETS core DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
