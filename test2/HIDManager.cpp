//
//  HIDManager.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#include "HIDManager.h"
#include "utils.h"
#include "CFVector.h"

HIDManager::HIDManager(IOHIDOptionsType options):
options(options)
{
    manager = IOHIDManagerCreate(kCFAllocatorDefault, options);
    run_loop = CFRunLoopGetMain();
    loop_mode = kCFRunLoopDefaultMode;
    IOHIDManagerScheduleWithRunLoop(manager, run_loop, loop_mode);
}

HIDManager::~HIDManager()
{
    if(manager)
    {
        IOHIDManagerUnscheduleFromRunLoop(manager, run_loop, loop_mode);
        IOHIDManagerClose(manager, options);
        CFRelease(manager);
    }
}

std::vector<HIDDevice> HIDManager::get_devices(int64_t vendor_id, int64_t product_id)
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
