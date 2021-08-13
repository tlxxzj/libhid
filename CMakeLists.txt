cmake_minimum_required(VERSION 3.0)

# set the project name and version
project(libhid VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include_directories("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if(APPLE) 
    add_library(
        ${CMAKE_PROJECT_NAME}
        STATIC
    )

    set_target_properties(
        ${CMAKE_PROJECT_NAME}
        PROPERTIES PREFIX ""
    )

    target_sources(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        src/HidReport.h
        src/HidReport.cpp
        src/HidDeviceFilter.cpp
        src/HidDevice.cpp
        src/mac/HidDeviceMac.cpp
        src/mac/HIDManager.cpp
        src/mac/SharedCFRunLoop.cpp
    )
    
    target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        "-framework CoreFoundation"
        "-framework IOKit"
    )
endif()

# add tests
enable_testing()
add_subdirectory(tests)