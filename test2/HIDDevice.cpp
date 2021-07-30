//
//  HIDDevice.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#include <iostream>
#include <iomanip>
#include <string>

#include "HIDDevice.h"


HIDDevice::HIDDevice():
device_ref(nullptr)
{
    cf_retain();
}

HIDDevice::HIDDevice(const HIDDevice & hid_device):
device_ref(hid_device.device_ref)
{
    cf_retain();
}

HIDDevice::HIDDevice(const IOHIDDeviceRef & device_ref):
device_ref(device_ref)
{
    cf_retain();
}

HIDDevice::~HIDDevice()
{
    if(device_ref)
    {
        IOHIDDeviceClose(device_ref, kIOHIDOptionsTypeNone);
        CFRelease(device_ref);
    }
}

void HIDDevice::input_callback(void *             context,
                               IOReturn           result,
                               void *             sender,
                               IOHIDReportType    report_type,
                               uint32_t           report_id,
                               uint8_t *          report,
                               CFIndex            report_length)
{
    
}

void HIDDevice::open()
{
    
    IOHIDDeviceRegisterInputReportCallback(device_ref,
                                           input_data,
                                           max_input_length,
                                           input_callback,
                                           this);
}

void HIDDevice::read(uint8_t * data, size_t length)
{

}

void HIDDevice::write(const uint8_t * data, size_t length)
{
    IOReturn ret = IOHIDDeviceSetReport(device_ref, kIOHIDReportTypeOutput, 0, data, length);
    if (ret != kIOReturnSuccess)
    {
        std::cerr<<"Failed to write data to device: "<<get_path()<<std::endl;
    }
}


std::string HIDDevice::get_desc()
{
    return get_device_string_property(CFSTR(kIOHIDReportDescriptorKey));
}

std::string HIDDevice::get_path()
{
    io_service_t service = IOHIDDeviceGetService(device_ref);
    io_string_t str;
    kern_return_t ret = IORegistryEntryGetPath(service, kIOServicePlane, str);
    
    std::string path;
    if (ret == KERN_SUCCESS) {
        path = str;
    }
    return path;
}

std::string HIDDevice::get_product()
{
    return get_device_string_property(CFSTR(kIOHIDProductKey));
}

int64_t HIDDevice::get_product_id()
{
    return get_device_int_property(CFSTR(kIOHIDProductIDKey));
}

int64_t HIDDevice::get_vendor_id()
{
    return get_device_int_property(CFSTR(kIOHIDVendorIDKey));
}

int64_t HIDDevice::get_version_number()
{
    return get_device_int_property(CFSTR(kIOHIDVersionNumberKey));
}

std::string HIDDevice::get_serial_number()
{
    return get_device_string_property(CFSTR(kIOHIDSerialNumberKey));
}

std::string HIDDevice::get_manufacturer()
{
    return get_device_string_property(CFSTR(kIOHIDManufacturerKey));
}

int64_t HIDDevice::get_max_input_report_size()
{
    return get_device_int_property(CFSTR(kIOHIDMaxInputReportSizeKey));
}

int64_t HIDDevice::get_max_output_report_size()
{
    return get_device_int_property(CFSTR(kIOHIDMaxOutputReportSizeKey));
}


void HIDDevice::cf_retain()
{
    if(device_ref)
    {
        CFRetain(device_ref);
    }
}


CFTypeRef HIDDevice::get_device_property(const CFStringRef key)
{
    if(device_ref)
    {
        CFTypeRef type_ref = IOHIDDeviceGetProperty(device_ref, key);
        return type_ref;
    }
    return kCFNull;
}

int64_t HIDDevice::get_device_int_property(const CFStringRef key)
{
    CFTypeRef type_ref = get_device_property(key);
    CFNumberRef num_ref = (CFNumberRef)type_ref;
    
    int value = 0;
    
    if (type_ref && CFGetTypeID(type_ref) == CFNumberGetTypeID())
    {
        CFNumberGetValue(num_ref, CFNumberGetType(num_ref), &value);
    }
    
    if(type_ref) CFRelease(type_ref);
    return value;
}


std::string HIDDevice::get_device_string_property(const CFStringRef key)
{
    CFTypeRef type_ref = get_device_property(key);
    CFStringRef str_ref = (CFStringRef)type_ref;
    
    std::string value;
    
    std::cout<<CFGetTypeID(type_ref)<<CFDataGetTypeID();
    CFShow(type_ref);
    
    const UInt8 *bytes = CFDataGetBytePtr((CFDataRef)type_ref);
    CFIndex length = CFDataGetLength((CFDataRef)type_ref);
    
    std::cerr << "HID descriptors: [ " << std::flush ;
    for (int i=0; i<length; ++i)
      std::cerr << std::hex << std::setfill('0') << std::setw(2) << (int)bytes[i] << " " ;
    std::cerr << "]" << std::endl ;
    
    
    if (type_ref && CFGetTypeID(type_ref) == CFStringGetTypeID())
    {
        CFRange range = CFRangeMake(0, CFStringGetLength(str_ref));
        CFIndex max_buf_len = 0;
        // get max_buf_len
        CFStringGetBytes(str_ref, range, kCFStringEncodingUTF32LE, '?', false, nullptr, 0, &max_buf_len);
        
        value.resize(max_buf_len);
        
        CFStringGetBytes(str_ref, range, kCFStringEncodingUTF32LE, '?', false, (UInt8 *)value.data(), max_buf_len, &max_buf_len);
    }
    
    if(type_ref) CFRelease(type_ref);
    return value;
}
