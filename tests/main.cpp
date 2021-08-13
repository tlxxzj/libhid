

#include <iostream>
#include <thread>
#include <chrono>

#include <CoreFoundation/CoreFoundation.h>

#include "mac/SharedCFRunLoop.h"
#include "HIDManager.h"


int main(int argc, const char ** argv) {
    std::cout<<"Hello World"<<std::endl;
    auto devices = libhid::HidManager::getDevices();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}