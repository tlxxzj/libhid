#ifndef HIDDEVICEFILTER_H
#define HIDDEVICEFILTER_H

#include <map>

namespace libhid {

enum HidDeviceFilterKey {
    kHidDeviceFilterVendorId = 0,
    kHidDeviceFilterProductId,
    kHidDeviceFilterUsage,
    kHidDeviceFilterUsagePage
};

using HidDeviceFilterValue = int64_t;
using HidDeviceFilter = std::map<HidDeviceFilterKey, HidDeviceFilterValue>;
using HidDeviceFilterArray = std::vector<HidDeviceFilter>;


}

#endif