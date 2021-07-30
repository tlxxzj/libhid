//
//  HIDManager.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#ifndef HIDManager_h
#define HIDManager_h

#include <vector>
#include <stdio.h>

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include "HIDDevice.h"

class HIDManager
{
public:
    HIDManager(IOHIDOptionsType options = kIOHIDOptionsTypeNone);
    ~HIDManager();
    
    //std::vector<HIDDevice> get_devices();
    std::vector<HIDDevice> get_devices(int64_t vendor_id = 0, int64_t product_id = 0);
protected:
    
    IOHIDManagerRef manager;
    IOHIDOptionsType options;
    CFRunLoopRef run_loop;
    CFStringRef loop_mode;
private:
    
};

#endif /* HIDManager_h */
