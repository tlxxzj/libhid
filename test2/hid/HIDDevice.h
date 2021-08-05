//
//  HIDDevice.hpp
//  test2
//
//  Created by Lang Tian on 2021/8/5.
//

#ifndef HIDDevice_h
#define HIDDevice_h

#include <cstdint>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>


namespace hid {

class HIDDevice {
public:
    struct HIDDeviceImpl;

protected:
    std::shared_ptr<HIDDeviceImpl> impl_;
    
public:
    int32_t get_vendor_id();
    int32_t get_product_id();
    int32_t get_version_number();
    std::string get_serial_number();
    std::string get_product_name();
    std::string get_manufacturer_name();

    size_t get_max_input_report_size();
    size_t get_max_output_report_size();
    size_t get_max_feature_report_size();
    
    std::vector<uint8_t> get_report_descriptor();
};


}


#endif /* HIDDevice_hpp */
