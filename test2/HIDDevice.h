//
//  HIDDevice.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#ifndef HIDDEVICE_H
#define HIDDEVICE_H

#include <stdio.h>

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>


class HIDDevice {
public:
    HIDDevice();
    
    HIDDevice(const HIDDevice & hid_device);
    
    HIDDevice(const IOHIDDeviceRef & device_ref);
    
    ~HIDDevice();
    
    void open();
    
    void read(uint8_t * data, size_t length);
    
    void write(const uint8_t * data, size_t length);
    
    std::string get_desc();
    
    std::string get_path();
    
    std::string get_product();
    
    int64_t get_product_id();
    
    int64_t get_vendor_id();
    
    int64_t get_version_number();
    
    std::string get_serial_number();
    
    std::string get_manufacturer();
    
    int64_t get_max_input_report_size();
    
    int64_t get_max_output_report_size();
    
    static void input_callback(void *             context,
                               IOReturn           result,
                               void *             sender,
                               IOHIDReportType    report_type,
                               uint32_t           report_id,
                               uint8_t *          report,
                               CFIndex            report_length);
    
protected:
    void cf_retain();
    
    CFTypeRef get_device_property(const CFStringRef key);
    
    int64_t get_device_int_property(const CFStringRef key);
    
    std::string get_device_string_property(const CFStringRef key);
    
    IOHIDDeviceRef device_ref = nullptr;
    
    size_t max_input_length = 0;
    uint8_t *input_data;
    
private:
};

#endif /* HIDDEVICE_H */
