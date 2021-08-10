
#ifndef HIDMANAGER_H
#define HIDMANAGER_H

#include "HIDDevice.h"

namespace libhid {

class HIDManager {
public:
    static std::vector<std::shared_ptr<HIDDevice>> getDevices();
    //static void getDevices(int64_t vendor_id, int64_t product_id);
};

}

#endif
