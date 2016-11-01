include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(core)
add_subdirectory(detect)

if(DETECTOR_WITH_RPI)
    add_subdirectory(rpi)
endif()
