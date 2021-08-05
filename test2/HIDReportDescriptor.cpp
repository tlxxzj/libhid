//
//  HIDReportDescriptor.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#include "HIDReportDescriptor.h"
#include <iostream>


#define bitmask_bSize (0b11u)
#define bitmask_bType (0b11u)
#define bitmask_bTag  (0b1111u)

#define get_bSize0(val)    ((val) & bitmask_bSize)
#define get_bSize(val)     (get_bSize0(val) == 3 ? 4 : get_bSize0(val))
#define get_bType(val)     (((val) >> 2) & bitmask_bType)
#define get_bTag(val)      (((val) >> 4) & bitmask_bTag)


void parse(uint8_t data[], size_t length)
{
}
