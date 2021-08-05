//
//  HIDReport.cpp
//  test2
//
//  Created by Lang Tian on 2021/8/2.
//

#include "HIDReport.hpp"


HIDReport::HIDReport()
{
}

HIDReport::HIDReport(const uint8_t * data, size_t size)
{
    this->byte_array.assign(data, data+size);
    this->data = byte_array.data();
    this->size = size;
    this->max_size = size;
    
}

void HIDReport::set_max_size(size_t size)
{
    byte_array.resize(size);
    data = byte_array.data();
    this->max_size = size;
}
