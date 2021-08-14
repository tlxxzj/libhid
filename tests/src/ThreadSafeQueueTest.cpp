#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "libhid/ThreadSafeQueue.h"

TEST(ThreadSafeQueueTest, FunctionalTest) {
    using namespace libhid;
    ThreadSafeQueue<int> q;
    EXPECT_EQ(0, q.size());
    EXPECT_EQ(true, q.empty());

    q.push(1);
    EXPECT_EQ(1, q.size());
    EXPECT_EQ(false, q.empty());

    EXPECT_EQ(1, q.pop());
    EXPECT_EQ(0, q.size());
    EXPECT_EQ(true, q.empty());

    q.push(2);
    EXPECT_EQ(1, q.size());
    int v;
    EXPECT_EQ(true, q.tryPop(v));
    EXPECT_EQ(2, v);
    EXPECT_EQ(false, q.tryPop(v));
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        q.push(3);
    });
    EXPECT_EQ(true, q.tryPop(v, 2));
    EXPECT_EQ(3, v);
    t.join();
}