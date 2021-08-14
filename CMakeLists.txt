cmake_minimum_required(VERSION 3.0)

# set the project name and version
project(libhid VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBHID_PROJECT_NAME libhid)

add_library(
    ${LIBHID_PROJECT_NAME}
    STATIC
)

set_target_properties(
    ${LIBHID_PROJECT_NAME}
    PROPERTIES PREFIX ""
)

target_include_directories(
    ${LIBHID_PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(APPLE) 
    target_include_directories(
        ${LIBHID_PROJECT_NAME}
        PUBLIC
        "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
    )

    target_sources(
        ${LIBHID_PROJECT_NAME}
        PRIVATE
        src/HidReport.cpp
        src/HidDeviceFilter.cpp
        src/HidDevice.cpp
        src/mac/HidDeviceMac.cpp
        src/mac/HidManager.cpp
    )
    
    target_link_libraries(
        ${LIBHID_PROJECT_NAME}
        "-framework CoreFoundation"
        "-framework IOKit"
    )
endif()


# add tests
enable_testing()
add_subdirectory(tests)