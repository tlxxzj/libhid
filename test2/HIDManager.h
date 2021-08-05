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
    HIDManager();
    HIDManager(const HIDManager & hid_manager);
    ~HIDManager();
    
    std::vector<HIDDevice> get_devices(int64_t vendor_id = 0, int64_t product_id = 0);
    
protected:
    class Impl;
    std::shared_ptr<Impl> impl = nullptr;
};

#endif /* HIDManager_h */
