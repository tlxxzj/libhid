//
//  main.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include <iostream>
#include <vector>
#include <string>

#include "HIDDevice.h"
#include "CFVector.h"
#include "HIDManager.h"

void test() {
    
    std::vector<HIDDevice> devices = HIDManager().get_devices();
    
    for(auto hid_device: devices) {
        /*
        std::cout<<"-------------------"<<std::endl;
        std::cout<<"Product: "<<hid_device.get_product()<<std::endl;
        std::cout<<"Vendor ID: "<<hid_device.get_vendor_id()<<std::endl;
        std::cout<<"Product ID: "<<hid_device.get_product_id()<<std::endl;
        //std::cout<<"Device Path: "<<hid_device.get_path()<<std::endl;
        std::cout<<"Version: "<<hid_device.get_version_number()<<std::endl;
        std::cout<<"Serial Number: "<<hid_device.get_serial_number()<<std::endl;
        std::cout<<"Manufacturer: "<<hid_device.get_manufacturer()<<std::endl;
        std::cout<<"Max Input Report Size: "<<hid_device.get_max_input_report_size()<<std::endl;
        std::cout<<"Max Output Report Size: "<<hid_device.get_max_output_report_size()<<std::endl;
        
        std::cout<<"-------------------"<<std::endl;
         */
        
        std::cout<<"Desc: "<<hid_device.get_desc()<<std::endl;
        return;
    }
}

void test2()
{
    //IOHIDDeviceOpen
}




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!!!!\n";
    test();
    return 0;
}
