#include <gtest/gtest.h>

#include "mac/SharedCFTypeRef.h"

TEST(SharedCFTypeRefTest, FunctionalTest) {
    using namespace libhid;
    uint8_t value[] = "Hello Wrold!";
    SharedCFTypeRef<CFStringRef> ref = CFStringCreateWithBytes(kCFAllocatorDefault, &value[0], sizeof(value), kCFStringEncodingASCII, false);
    EXPECT_EQ(1, ref.getRetainCount());

    // copy
    SharedCFTypeRef<CFStringRef> ref2 = ref;
    EXPECT_EQ(2, ref2.getRetainCount());

    // move
    SharedCFTypeRef<CFStringRef>  * ref3 = new SharedCFTypeRef<CFStringRef>(std::move(ref2));
    EXPECT_EQ(nullptr, ref2.get());
    EXPECT_EQ(2, ref3->getRetainCount());
    EXPECT_EQ(ref.get(), ref3->get());

    // asign move
    *ref3 = std::move(*ref3);
    EXPECT_EQ(2, ref3->getRetainCount());
    EXPECT_EQ(ref.get(), ref3->get());

    // destroy
    delete ref3;
    EXPECT_EQ(1, ref.getRetainCount());
}