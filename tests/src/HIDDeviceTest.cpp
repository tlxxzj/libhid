#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "libhid/HidManager.h"
#include "libhid/HidDevice.h"


static void printReport(std::vector<uint8_t> report) {
    std::cerr<<(int)report[0]<<":";
    for(const auto & byte: report) {
        std::cerr<< std::hex << std::setfill('0') << std::setw(2) << (int)byte <<std::dec<< " " ;
    }
    std::cerr<<std::endl;
}

TEST(HidDeviceTest, PropertiesTest) {
    using namespace libhid;
    std::vector<std::shared_ptr<HidDevice>> devices = libhid::HidManager::getDevices();
    for(auto &device: devices) {
        std::cout<<"----------------------------------------"<<std::endl;
        std::cout<<"Vendor ID: "<<device->vendorId()<<std::endl;
        std::cout<<"Product ID: "<<device->productId()<<std::endl;
        std::cout<<"Version Number: "<<device->versionNumber()<<std::endl;
        
        std::cout<<"Product: "<<device->product()<<std::endl;
        std::cout<<"Manufacturer: "<<device->manufacturer()<<std::endl;
        std::cout<<"Serial Number: "<<device->serialNumber()<<std::endl;

        std::cout<<"Max Input Report Size: "<<device->maxInputReportSize()<<std::endl;
        std::cout<<"Max Output Report Size: "<<device->maxOutputReportSize()<<std::endl;
        std::cout<<"Max Feature Report Size: "<<device->maxFeatureReportSize()<<std::endl;
    }
}

TEST(HidDeviceTest, getInputReportAsyncTest) {
    using namespace libhid;
    HidDeviceFilterArray filters = {
        {
            { kHidDeviceFilterVendorId, 0x05ac },
            { kHidDeviceFilterProductId, 0x8302 }
        }
    };
    auto devices = libhid::HidManager::getDevices(filters);
    for(auto & device: devices) {
        device->getInputReportAsync([&](std::vector<uint8_t> report){
            printReport(report);
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for(auto & device: devices) {
        device->close();
    }
}

TEST(HidDeviceTest, tryGetInputReportTest) {
    using namespace libhid;
    HidDeviceFilterArray filters = {
        {
            {kHidDeviceFilterVendorId, 0x05ac},
            {kHidDeviceFilterProductId, 0x8302}
        }
    };
    auto devices = libhid::HidManager::getDevices(filters);
    for(auto & device: devices) {
        std::vector<uint8_t> report;
        bool result = device->tryGetInputReport(report, 0.1);
    }
    for(auto & device: devices) {
        device->close();
    }
}
