//
//  HIDReportDescriptor.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#ifndef HIDReportDescriptor_hpp
#define HIDReportDescriptor_hpp

#include <cstdint>
#include <cstdlib>

struct report_item_header
{
    uint8_t bSize:2;
    uint8_t bType:2;
    uint8_t bTag:4;
};


void parse(uint8_t *data, size_t length);

#endif /* HIDReportDescriptor_hpp */
