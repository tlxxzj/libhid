#include <iostream>
#include <vector>
#include <memory>

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include "SharedCFTypeRef.h"
#include "CFVector.h"
#include "HidManager.h"
#include "HidDeviceMac.h"


namespace libhid {

    static SharedCFTypeRef<CFArrayRef> createDeviceMatchingMultiple(HidDeviceFilterArray filters) {
        if(filters.size() == 0)
            return SharedCFTypeRef<CFArrayRef>(nullptr);
        
        std::vector<CFMutableDictionaryRef> dictArray;
        for(const HidDeviceFilter & filter: filters) {
            CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            
            for(const auto & item: filter) {
                const auto key = item.first;
                const auto value = item.second;

                CFStringRef cfKey = nullptr;
                if(key == kHidDeviceFilterVendorId)
                    cfKey = CFSTR(kIOHIDVendorIDKey);
                else if(key == kHidDeviceFilterProductId)
                    cfKey = CFSTR(kIOHIDProductIDKey);
                else if(key == kHidDeviceFilterUsage)
                    cfKey = CFSTR(kIOHIDDeviceUsageKey);
                else if(key == kHidDeviceFilterUsagePage)
                    cfKey = CFSTR(kIOHIDDeviceUsagePageKey);
                else
                    continue;
                
                SharedCFTypeRef<CFNumberRef> cfValue = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &value);
                CFDictionarySetValue(dict, cfKey, cfValue);
            }
            dictArray.push_back(dict);
        }
        SharedCFTypeRef<CFArrayRef> cfArray = CFArrayCreate(kCFAllocatorDefault, (const void **)(dictArray.data()), dictArray.size(), &kCFTypeArrayCallBacks);
        return cfArray;
    }

    std::vector<std::shared_ptr<HidDevice>> HidManager::getDevices(HidDeviceFilterArray filters) {
        SharedCFTypeRef<IOHIDManagerRef> managerRef(IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone));
        //IOHIDManagerSetDeviceMatching(managerRef, nullptr);
        SharedCFTypeRef<CFArrayRef> multipleFilter = createDeviceMatchingMultiple(filters);
        IOHIDManagerSetDeviceMatchingMultiple(managerRef, multipleFilter);
        SharedCFTypeRef<CFSetRef> devicesSetRef = IOHIDManagerCopyDevices(managerRef);
        
        std::vector<std::shared_ptr<HidDevice>>devices;
        for(const IOHIDDeviceRef & deviceRef: toVector<IOHIDDeviceRef>(devicesSetRef)) {
            std::shared_ptr<HidDevice> device = std::make_shared<HidDeviceMac>(SharedCFTypeRef<IOHIDDeviceRef>(deviceRef, true));
            devices.push_back(std::move(device));
        }
        return devices;
    }
}