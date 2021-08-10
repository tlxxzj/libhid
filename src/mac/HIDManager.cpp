

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include "SharedCFTypeRef.h"
#include "CFVector.h"
#include "libhid/HIDManager.h"
#include "HIDDeviceMac.h"


#include <iostream>
#include <vector>
#include <memory>


namespace libhid {

    std::vector<std::shared_ptr<HIDDevice>> HIDManager::getDevices() {
        SharedCFTypeRef<IOHIDManagerRef> manager_ref(IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone));
        IOHIDManagerSetDeviceMatching(manager_ref, nullptr);
        SharedCFTypeRef<CFSetRef> devices_cf_ref = IOHIDManagerCopyDevices(manager_ref);
        
        std::vector<std::shared_ptr<HIDDevice>>devices;
        for(const IOHIDDeviceRef & device_cf_ref: toVector<IOHIDDeviceRef>(devices_cf_ref)) {
            std::shared_ptr<HIDDevice> device = std::make_shared<HIDDeviceMac>(SharedCFTypeRef<IOHIDDeviceRef>(device_cf_ref, true));
            devices.push_back(std::move(device));
        }
        return devices;
    }
}



