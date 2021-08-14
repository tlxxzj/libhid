
#ifndef HIDREPORT_H
#define HIDREPORT_H


#include <vector>


namespace libhid {

using HidReport = std::vector<uint8_t>;

enum HidReportType {
    kHidInputReport = 0,
    kHidOutputReport,
    kHidFeatureReport
};

}

#endif



