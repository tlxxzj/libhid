#include <gtest/gtest.h>

#include "libhid/HidManager.h"

TEST(HIDManagerTest, FunctionalTest) {
    EXPECT_NO_THROW(libhid::HidManager::getDevices());
}