include_directories(${core_INCLUDE_DIRS})

add_executable(pinled main.cpp)
target_link_libraries(pinled rpi core)

install(TARGETS pinled DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
