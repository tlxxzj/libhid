cmake_minimum_required(VERSION 3.0)

# set the project name and version
project(
    libhid
    VERSION 0.0.1
    LANGUAGES C CXX
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBHID_TARGET_NAME libhid)

add_library(
    ${LIBHID_TARGET_NAME}
    STATIC
)

set_target_properties(
    ${LIBHID_TARGET_NAME}
    PROPERTIES PREFIX ""
)

target_include_directories(
    ${LIBHID_TARGET_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_sources(
    ${LIBHID_TARGET_NAME}
    PRIVATE
    src/HidReport.cpp
    src/HidDeviceFilter.cpp
    src/HidDevice.cpp
)

if(APPLE) 
    target_include_directories(
        ${LIBHID_TARGET_NAME}
        PRIVATE
        "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
    )

    target_sources(
        ${LIBHID_TARGET_NAME}
        PRIVATE
        src/mac/HidDeviceMac.cpp
        src/mac/HidManager.cpp
    )
    
    target_link_libraries(
        ${LIBHID_TARGET_NAME}
        "-framework CoreFoundation"
        "-framework IOKit"
    )
endif()


# add tests
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    enable_testing()
    add_subdirectory(tests)
endif()
