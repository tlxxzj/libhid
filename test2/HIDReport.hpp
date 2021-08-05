//
//  HIDReport.hpp
//  test2
//
//  Created by Lang Tian on 2021/8/2.
//

#ifndef HIDReport_hpp
#define HIDReport_hpp

#include <vector>
#include <cstdint>
#include <cstddef>


class HIDReport
{
public:
    enum Type
    {
        Input = 0,
        Output = 1,
        Feature = 2,
        Count = 3,
    };
    
    uint32_t id = 0;
    Type type = Type::Input;
    std::vector<uint8_t>byte_array;
    uint8_t *data = nullptr;
    size_t size = 0;
    size_t max_size = 0;
    
    HIDReport();
    HIDReport(const uint8_t * data, size_t size);
    
    void set_max_size(size_t size);
    
};

#endif /* HIDReport_hpp */
