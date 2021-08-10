

#include <iostream>
#include "libhid/HIDManager.h"
#include <thread>
#include <chrono>

#include <CoreFoundation/CoreFoundation.h>
#include "src/mac/SharedCFRunLoop.h"

int main(int argc, const char ** argv) {
    std::cout<<"Hello World"<<std::endl;
    auto devices = libhid::HIDManager::getDevices();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}