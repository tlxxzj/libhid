#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <libhid/LibHid.h>

#include "G29.h"


static void printReport(std::vector<uint8_t> report) {
    std::cerr<<"\r";
    //std::cerr<< *((uint16_t *)&report[43])<<std::endl;
    for(const auto & byte: report) {
        std::cerr<<std::hex << std::setfill('0') << std::setw(2) << (int)byte <<std::dec<< " " ;
    }
    //std::cerr<<std::endl;
}


TEST(G29Test, InputReportTest) {
    /*
    libhid::HidDeviceFilterArray filters = {
        {
            { libhid::kHidDeviceFilterVendorId, logitech::LogitechG29::vendorId },
            { libhid::kHidDeviceFilterProductId, logitech::LogitechG29::productId }
        }
    };
    auto devices = libhid::HidManager::getDevices(filters);
    devices[0]->getInputReportAsync([](std::vector<uint8_t> report){
        printReport(report);
    });
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    */

    auto g29 = logitech::LogitechG29(0);
    while(false) {
        auto state = g29.getState();
        std::cerr<<"----------------------------"<<std::endl;
        std::cerr<<"Wheel: "<<state.wheel<<std::endl;
        std::cerr<<"Accelerator: "<<state.accelerator<<std::endl;
        std::cerr<<"Brake: "<<state.brake<<std::endl;
        std::cerr<<"Clutch: "<<state.clutch<<std::endl;
        std::cerr<<"Dpad: "<<(int)state.dpad<<std::endl;
        std::cerr<<"Cross: "<<(int)state.cross<<std::endl;
        std::cerr<<"Square: "<<(int)state.square<<std::endl;
        std::cerr<<"Triangle: "<<(int)state.triangle<<std::endl;
        std::cerr<<"Circle: "<<(int)state.circle<<std::endl;
        std::cerr<<"L1: "<<(int)state.l1<<std::endl;
        std::cerr<<"L2: "<<(int)state.l2<<std::endl;
        std::cerr<<"L3: "<<(int)state.l3<<std::endl;
        std::cerr<<"r1: "<<(int)state.r1<<std::endl;
        std::cerr<<"r2: "<<(int)state.r2<<std::endl;
        std::cerr<<"r3: "<<(int)state.r3<<std::endl;
        std::cerr<<"Plus: "<<(int)state.plus<<std::endl;
        std::cerr<<"Minus: "<<(int)state.minus<<std::endl;
        std::cerr<<"Share: "<<(int)state.share<<std::endl;
        std::cerr<<"Options: "<<(int)state.optoins<<std::endl;
        std::cerr<<"PS: "<<(int)state.ps<<std::endl;
        std::cerr<<"Red Dial: "<<(int)state.redDial<<std::endl;
        std::cerr<<"Red Dial Enter: "<<(int)state.redDialEnter<<std::endl;
    }
}

TEST(G29Test, OutputReportTest) {
    auto g29 = logitech::LogitechG29(0);
    g29.setRange(500);
    std::this_thread::sleep_for(std::chrono::seconds(10000));
}
