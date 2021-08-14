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

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(
    SharedCFTypeRefTest
    src/mac/SharedCFTypeRefTest.cpp
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
    HIDManagerTest
    src/HIDManagerTest.cpp
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
    HidDeviceTest
    src/HidDeviceTest.cpp
)
target_link_libraries(
    HidDeviceTest
    libhid
    gtest_main
)
gtest_discover_tests(
    HidDeviceTest
)

add_executable(
    ThreadSafeQueueTest
    src/ThreadSafeQueueTest.cpp
)
target_link_libraries(
    ThreadSafeQueueTest
    gtest_main
)
gtest_discover_tests(
    ThreadSafeQueueTest
)

add_executable(
    G29Test
    ../src/G29.cpp
    src/G29Test.cpp
)
target_link_libraries(
    G29Test
    libhid
    gtest_main
)
gtest_discover_tests(
    G29Test
)
