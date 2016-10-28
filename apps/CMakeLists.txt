add_subdirectory(detector)

if(DETECTOR_WITH_RPI)
    add_subdirectory(pinled)
endif()
