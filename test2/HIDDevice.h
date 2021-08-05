//
//  HIDDevice.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#ifndef HIDDEVICE_H
#define HIDDEVICE_H


#include <memory>
#include <type_traits>
#include <stdio.h>

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include "HIDReport.hpp"


class HIDDevice {
protected:
    class Impl;
    std::shared_ptr<Impl>impl = nullptr;

public:
    
    using input_report_callback = std::function<void(HIDDevice, HIDReport)>;
    
    HIDDevice() = delete;
    
    HIDDevice(IOHIDDeviceRef device_ref);
    
    HIDDevice(const HIDDevice & hid_device);
    
    HIDDevice(const std::shared_ptr<Impl> & impl);
    
    ~HIDDevice();
    
    std::string get_path();
    
    std::string get_product();
    
    int64_t get_product_id();
    
    int64_t get_vendor_id();
    
    int64_t get_version_number();
    
    std::string get_serial_number();
    
    std::string get_manufacturer();
    
    int64_t get_max_input_report_size();
    
    int64_t get_max_output_report_size();
    
    void add_input_report_callback();
    
    std::vector<uint8_t> get_report_descriptor();
    
private:
};

#endif /* HIDDEVICE_H */
