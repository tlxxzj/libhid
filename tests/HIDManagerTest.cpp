#include <gtest/gtest.h>

#include "libhid/HIDManager.h"

TEST(HIDManagerTest, FunctionalTest) {
    EXPECT_NO_THROW(libhid::HIDManager::getDevices());
}