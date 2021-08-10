cmake_minimum_required(VERSION 3.0)

# set the project name and version
project(libhid VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

if(APPLE) 
    add_library(
        libhid
        STATIC
    )

    set_target_properties(
        libhid
        PROPERTIES PREFIX ""
    )

    target_sources(
        libhid
        PRIVATE
        src/ThreadSafeQueue.h
        src/mac/SharedCFTypeRef.h
        src/mac/SharedCFRunLoop.cpp
        src/mac/CFVector.h
        src/mac/HIDManager.cpp
        src/HIDDevice.cpp
        src/mac/HIDDeviceMac.cpp
    )
    
    target_link_libraries(
        libhid
        "-framework CoreFoundation"
        "-framework IOKit"
    )
endif()

add_executable(
    main
    main.cpp
)

target_link_libraries(
    main
    libhid
)

# add tests
enable_testing()
add_subdirectory(tests)