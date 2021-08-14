
#ifndef HIDMANAGER_H
#define HIDMANAGER_H

#include "libhid/HidDevice.h"
#include "libhid/HidDeviceFilter.h"

namespace libhid {

class HidManager {
public:
    static std::vector<std::shared_ptr<HidDevice>> getDevices(HidDeviceFilterArray filters = {});
};

}




#endif
