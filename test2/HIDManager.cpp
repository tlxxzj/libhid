//
//  HIDManager.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#include "HIDManager.h"
#include "utils.h"
#include "CFVector.h"
#include <iostream>


class HIDManager::Impl
{
public:
    IOHIDManagerRef manager = nullptr;
    IOHIDOptionsType options;
    CFRunLoopRef run_loop;
    CFRunLoopMode loop_mode;
    
    Impl()
    {
        options = kIOHIDOptionsTypeNone;
        run_loop = CFRunLoopGetCurrent();
        loop_mode = kCFRunLoopDefaultMode;
        manager = IOHIDManagerCreate(kCFAllocatorDefault, options);
        IOHIDManagerScheduleWithRunLoop(manager, run_loop, loop_mode);
        open();
    }
    
    ~Impl()
    {
        IOHIDManagerUnscheduleFromRunLoop(manager, run_loop, loop_mode);
        close();
        cf_release();
    }
    
    bool open()
    {
        IOReturn ret = IOHIDManagerOpen(manager, options);
        if(ret != kIOReturnSuccess)
        {
            std::cerr<<"Failed to open HID Manager"<<manager<<std::endl;
            return false;
        }
        return true;
    }
    
    void close()
    {
        IOHIDManagerClose(manager, options);
    }
    
    void cf_retain()
    {
        CFRetain(manager);
    }
    
    void cf_release()
    {
        CFRelease(manager);
    }
    
    
    std::vector<HIDDevice> get_devices(int64_t vendor_id, int64_t product_id)
    {
        CFMutableDictionaryRef matching_dict = nullptr;
        if (vendor_id != 0 || product_id != 0)
        {
            matching_dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            
            if (vendor_id != 0)
            {
                CFNumberRef value = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &vendor_id);
                CFDictionarySetValue(matching_dict, CFSTR(kIOHIDVendorIDKey), value);
                CFRelease(value);
            }
            if (product_id != 0)
            {
                CFNumberRef value = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &product_id);
                CFDictionarySetValue(matching_dict, CFSTR(kIOHIDProductIDKey), value);
                CFRelease(value);
            }
            
        }
        IOHIDManagerSetDeviceMatching(manager, matching_dict);
        if(matching_dict)
        {
            CFRelease(matching_dict);
        }
        CFVector<IOHIDDeviceRef, HIDDevice> devices = IOHIDManagerCopyDevices(manager);
        return std::move(devices);
    }
};


static CFMutableDictionaryRef CreateMatchingDictionary(UInt32 usage_page, UInt32 usage) {
    CFMutableDictionaryRef dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFNumberRef page_number = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage_page);
    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsagePageKey), page_number);
    CFRelease(page_number);

    CFNumberRef usage_number = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsageKey), usage_number);
    CFRelease(usage_number);

    return dictionary;
}


HIDManager::HIDManager():
impl(std::make_shared<HIDManager::Impl>())
{
}

HIDManager::HIDManager(const HIDManager & hid_manager):
impl(hid_manager.impl)
{
}

HIDManager::~HIDManager()
{
}


std::vector<HIDDevice> HIDManager::get_devices(int64_t vendor_id, int64_t product_id)
{
    return impl->get_devices(vendor_id, product_id);
}
