#include <gtest/gtest.h>

#include "mac/SharedCFRunLoop.h"

TEST(SharedCFRunLoopTest, FunctionalTest) {
    using namespace libhid;

    EXPECT_EQ(0, SharedCFRunLoop::retainCount());

    SharedCFRunLoop * loop1 = new SharedCFRunLoop;
    EXPECT_EQ(CFRunLoopGetTypeID(), CFGetTypeID(loop1->get()));
    EXPECT_EQ(1, loop1->retainCount());

    SharedCFRunLoop * loop2 = new SharedCFRunLoop(*loop1);
    EXPECT_EQ(2, loop2->retainCount());

    delete loop1;
    delete loop2;
    EXPECT_EQ(0, SharedCFRunLoop::retainCount());

    SharedCFRunLoop * loop3 = new SharedCFRunLoop;
    SharedCFRunLoop * loop4 = new SharedCFRunLoop;
    *loop3 = *loop3;
    *loop3 = std::move(*loop4);
    EXPECT_EQ(2, loop3->retainCount());
    
    delete loop3;
    delete loop4;
    SharedCFRunLoop loop5;
    EXPECT_EQ(1, SharedCFRunLoop::retainCount());
}