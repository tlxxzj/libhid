
//
//  main.cpp
//  test2
//
//  Created by Lang Tian on 2021/7/29.
//

#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/IOKitLib.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "HIDDevice.h"
#include "CFVector.h"
#include "HIDManager.h"
#include "HIDUsage.h"
#include "HIDReportDescriptor.h"
#include <curses.h>

#include <thread>

#include <cstdio>



static void callback (
               void * _Nullable        context,
               IOReturn                result,
               void * _Nullable        sender,
               IOHIDValueRef           value)
{
   // return;
    std::cerr<<std::flush<<"callbak"<<std::endl;
    CFShow(value);
    puts("callback ...");
}

static void callback2 (
                void * _Nullable        context,
                IOReturn                result,
                void * _Nullable        sender,
                IOHIDReportType         type,
                uint32_t                reportID,
                uint8_t *               report,
                CFIndex                 reportLength)
{
    puts("callback2...");
}


static void match_callback(void *context, IOReturn result,
               void *sender, IOHIDDeviceRef device)
{
    
    puts("callback3...");
    //IOHIDDeviceOpen(device, kIOHIDOptionsTypeSeizeDevice);
    //IOHIDDeviceRegisterInputValueCallback(device, callback, NULL);
    
}

void read_thread(IOHIDManagerRef manager)
{
    IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    CFRunLoopRun();
}

void test() {
    //std::cout<<HIDUsageID<HIDUsagePageID::GenericDesktop>::Keyboard<<std::endl;
    //kHIDPage_GenericDesktop
    auto mgr = new HIDManager();
    auto mgr2 = mgr;
    std::vector<HIDDevice> devices = mgr2->get_devices(0x046d, 0xc260);
    for(auto hid_device: devices) {
        //break;
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
        auto report_descriptor = hid_device.get_report_descriptor();
        for(auto byte: report_descriptor)
        {
            std::cout<< std::hex << std::setfill('0') << std::setw(2) << (int)byte << " " ;
        }
        std::cout<<"-------------------"<<std::endl;
    }
    
    //IOHIDManagerRegisterDeviceMatchingCallback(mgr->manager, match_callback, NULL);
    //IOHIDManagerOpen(mgr->manager, kIOHIDOptionsTypeNone );
    //IOHIDManagerRegisterInputValueCallback(mgr->manager, callback, nullptr);
    //IOHIDManagerRegisterInputReportCallback(mgr->manager, callback2, NULL);
    //auto t = new std::thread(read_thread, mgr->manager);
   CFRunLoopRun();
    //sleep(5);
    
}



int main(int argc, const char * argv[]) {
    // insert code here...
    
    initscr();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    
    std::cout << "Hello, World!!!!\n";
    
    
   test();
    return 0;
}

