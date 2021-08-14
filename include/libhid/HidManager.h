
#ifndef HIDMANAGER_H
#define HIDMANAGER_H

#include "HidDevice.h"
#include "HidDeviceFilter.h"

namespace libhid {

class HidManager {
public:
    static std::vector<std::shared_ptr<HidDevice>> getDevices(HidDeviceFilterArray filters = {});
};

}




#endif
