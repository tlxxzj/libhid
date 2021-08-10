cmake_minimum_required(VERSION 3.0)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(
    SharedCFTypeRefTest
    mac/SharedCFTypeRefTest.cpp
)

target_link_libraries(
    SharedCFTypeRefTest
    libhid
    gtest_main
)

gtest_discover_tests(
    SharedCFTypeRefTest
)

add_executable(
    SharedCFRunLoopTest
    mac/SharedCFRunLoopTest.cpp
)

target_link_libraries(
    SharedCFRunLoopTest
    libhid
    gtest_main
)

gtest_discover_tests(
    SharedCFRunLoopTest
)



add_executable(
    HIDManagerTest
    HIDManagerTest.cpp
)

target_link_libraries(
    HIDManagerTest
    libhid
    gtest_main
)

gtest_discover_tests(
    HIDManagerTest
)

add_executable(
    HIDDeviceTest
    HIDDeviceTest.cpp
)

target_link_libraries(
    HIDDeviceTest
    libhid
    gtest_main
)

gtest_discover_tests(
    HIDDeviceTest
)

add_executable(
    ThreadSafeQueueTest
    ThreadSafeQueueTest.cpp
)

target_link_libraries(
    ThreadSafeQueueTest
    gtest_main
)

gtest_discover_tests(
    ThreadSafeQueueTest
)
