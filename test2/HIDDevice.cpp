//
//  HIDDevice.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <type_traits>
#include <vector>
#include <set>
#include <thread>
#include "HIDDevice.h"
#include "HIDReport.hpp"

#include <mach/mach_error.h>

#include <curses.h>

template <typename T>
struct FunctionLessComparator
{
    bool operator()(const T &lhs, const T &rhs) const
    {
        return &lhs < &rhs;
    }
};

class HIDDevice::Impl: public std::enable_shared_from_this<HIDDevice::Impl>
{
public:
    IOHIDDeviceRef device;
    CFRunLoopRef run_loop;
    CFRunLoopMode run_loop_mode;
    std::thread run_loop_thread;
    input_report_callback input_callback = nullptr;
    HIDReport input_report;
    HIDReport input_report_buffer;
    bool unplugged = false;
    
    Impl() = delete;
    
    Impl(IOHIDDeviceRef device_ref):
    device(device_ref)
    {
        cf_retain();
        open();
    }
    
    ~Impl()
    {
        close();
        cf_release();
    }
    
    std::shared_ptr<Impl> get_self()
    {
        return shared_from_this();
    }
    
    bool open()
    {
        IOReturn ret = IOHIDDeviceOpen(device, kIOHIDOptionsTypeSeizeDevice);
        if (ret != kIOReturnSuccess)
        {
            std::cerr<<"Failed to open device: "<<device<<std::endl;
            //return false;
            //std::cerr<<mach_error_string(ret)<<std::endl;
        }
        size_t max_size = get_int_property(CFSTR(kIOHIDMaxInputReportSizeKey));
        input_report.set_max_size(max_size);
        IOHIDDeviceRegisterInputReportCallback(device, input_report.data, input_report.max_size, handle_report_callback, this);
        IOHIDDeviceRegisterRemovalCallback(device, handle_device_removal_callback, this);
        run_loop_thread = std::thread(&Impl::run_loop_func, this);
        return true;
    }
    
    void close()
    {
        IOHIDDeviceRegisterInputReportCallback(device, input_report.data, input_report.max_size, nullptr, nullptr);
        IOHIDDeviceRegisterRemovalCallback(device, nullptr, nullptr);
        IOHIDDeviceUnscheduleFromRunLoop(device, run_loop, run_loop_mode);
        CFRunLoopStop(run_loop);
        if(std::this_thread::get_id() !=  run_loop_thread.get_id())
        {
            
        }
        if(run_loop_thread.joinable())
        {
            run_loop_thread.join();
        }
        IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
        IOHIDDeviceClose(device, kIOHIDOptionsTypeNone);
    }
    
    void cf_retain()
    {
        CFRetain(device);
    }
    
    void cf_release()
    {
        CFRelease(device);
    }
    
    CFTypeRef get_property(CFStringRef key)
    {
        CFTypeRef type_ref = IOHIDDeviceGetProperty(device, key);
        return type_ref;
    }
    
    int64_t get_int_property(CFStringRef key)
    {
        CFTypeRef type_ref = get_property(key);
        CFNumberRef num_ref = static_cast<CFNumberRef>(type_ref);
        int64_t value = 0;
        
        if (type_ref && (CFGetTypeID(type_ref) == CFNumberGetTypeID()))
        {
            CFNumberGetValue(num_ref, CFNumberGetType(num_ref), &value);
        }
        
        if(type_ref)
        {
            CFRelease(type_ref);
        }
        return value;
    }
    
    
    std::string get_string_property(CFStringRef key)
    {
        CFTypeRef type_ref = get_property(key);
        CFStringRef str_ref = static_cast<CFStringRef>(type_ref);
        
        std::string value;
        
        if (type_ref && CFGetTypeID(type_ref) == CFStringGetTypeID())
        {
            CFRange range = CFRangeMake(0, CFStringGetLength(str_ref));
            CFIndex max_buf_len = 0;
            // get max_buf_len
            CFStringGetBytes(str_ref, range, kCFStringEncodingUTF8, '?', false, nullptr, 0, &max_buf_len);
            
            value.resize(max_buf_len);
            
            CFStringGetBytes(str_ref, range, kCFStringEncodingUTF8, '?', false, (UInt8 *)value.data(), max_buf_len, &max_buf_len);
        }
        
        if(type_ref)
        {
            CFRelease(type_ref);
        }
        return value;
    }
    
    std::vector<uint8_t> get_data_property(CFStringRef key)
    {
        CFTypeRef type_ref = get_property(key);
        std::vector<uint8_t>value;
        if(type_ref && CFGetTypeID(type_ref) == CFDataGetTypeID())
        {
            CFDataRef data_ref = static_cast<CFDataRef>(type_ref);
            const uint8_t * byte_ptr = CFDataGetBytePtr(data_ref);
            const size_t length = CFDataGetLength(data_ref);
            value.insert(value.begin(), byte_ptr, byte_ptr + length);
        }
        if(type_ref) CFRelease(type_ref);
        return value;
    }
    
    std::string get_path()
    {
        io_service_t service = IOHIDDeviceGetService(device);
        io_string_t str;
        IOReturn ret = IORegistryEntryGetPath(service, kIOServicePlane, str);
        std::string path;
        if (ret == kIOReturnSuccess) {
            path = str;
        }
        return path;
    }
                                      
    void run_loop_func()
    {
        run_loop = CFRunLoopGetCurrent();
        run_loop_mode = kCFRunLoopDefaultMode;
        IOHIDDeviceScheduleWithRunLoop(device, run_loop, run_loop_mode);
        CFRunLoopRun();
    }
    
    std::vector<uint8_t> get_report_descriptor()
    {
        
        return get_data_property(CFSTR(kIOHIDReportDescriptorKey));
    }
    
    static void handle_report_callback(void *                  context,
                                       IOReturn                result,
                                       void *                  sender,
                                       IOHIDReportType         report_type,
                                       uint32_t                report_id,
                                       uint8_t *               report_data,
                                       CFIndex                 report_size)
    {
        if(result != kIOReturnSuccess)
        {
            std::cerr<<"Failed to read input report: "<<mach_error_string(result)<<std::endl;
            return;
        }
        std::shared_ptr<HIDDevice::Impl> self = static_cast<HIDDevice::Impl *>(context)->get_self();
        self->input_report.id = report_id;
        //self->input_report.type = report_type;
        self->input_report.size = report_size;
        //std::cerr<<"Report ID: "<<report_id<<" Report Size: "<<report_size<<std::endl;
        //self->input_callback();
        //std::cerr<<"callback from "<<self->get_path()<<std::endl;
        move(0, 0);
        clrtoeol();
        for(int i = 0; i < report_size; i++)
        {
            if (report_data[i]) {
                attron(COLOR_PAIR(1));
            }
            printw(" %.2x", report_data[i]);
            if (report_data[i]) {
                attroff(COLOR_PAIR(1));
            }
        }
        refresh();
    }
    
    static void handle_device_removal_callback(void *       context,
                                               IOReturn     result,
                                               void *       sender)
    {
        std::shared_ptr<HIDDevice::Impl> self = static_cast<HIDDevice::Impl *>(context)->get_self();
        std::cerr<<"Device is unplugged"<<std::endl;
        self->unplugged = true;
        //self->close();
    }
    
};


HIDDevice::HIDDevice(const HIDDevice & hid_device):
impl(hid_device.impl)
{
}

HIDDevice::HIDDevice(IOHIDDeviceRef device_ref):
impl(new Impl(device_ref))
{
}

HIDDevice:: HIDDevice(const std::shared_ptr<Impl> & impl):
impl(impl)
{
}

HIDDevice::~HIDDevice()
{
}

std::string HIDDevice::get_path()
{
    return impl->get_path();
}

std::string HIDDevice::get_product()
{
    return impl->get_string_property(CFSTR(kIOHIDProductKey));
}

int64_t HIDDevice::get_product_id()
{
    return impl->get_int_property(CFSTR(kIOHIDProductIDKey));
}

int64_t HIDDevice::get_vendor_id()
{
    return impl->get_int_property(CFSTR(kIOHIDVendorIDKey));
}

int64_t HIDDevice::get_version_number()
{
    return impl->get_int_property(CFSTR(kIOHIDVersionNumberKey));
}

std::string HIDDevice::get_serial_number()
{
    return impl->get_string_property(CFSTR(kIOHIDSerialNumberKey));
}

std::string HIDDevice::get_manufacturer()
{
    return impl->get_string_property(CFSTR(kIOHIDManufacturerKey));
}

int64_t HIDDevice::get_max_input_report_size()
{
    return impl->get_int_property(CFSTR(kIOHIDMaxInputReportSizeKey));
}

int64_t HIDDevice::get_max_output_report_size()
{
    return impl->get_int_property(CFSTR(kIOHIDMaxOutputReportSizeKey));
}

std::vector<uint8_t> HIDDevice::get_report_descriptor()
{
    return impl->get_report_descriptor();
}
