
#include <mach/mach_error.h>

#include "HIDDeviceMac.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>


namespace libhid {
    HIDDeviceMac::HIDDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & device_ref):
    HIDDevice(true),
    m_device_ref(device_ref) {
        collectProperty();
        open();
    }
    
    HIDDeviceMac::~HIDDeviceMac() {
        close();
    }

    bool HIDDeviceMac::open() {
        if(!m_closed)
            return true;
        IOReturn ret = IOHIDDeviceOpen(m_device_ref, kIOHIDOptionsTypeSeizeDevice);
        if(ret != kIOReturnSuccess) {
            std::cerr<<"Failed to open device: "<<mach_error_string(ret)<<std::endl;
            return false;
        }
        
        m_input_report_buffer.resize(maxInputReportSize());
        if(m_input_report_buffer.size() > 0) {
            IOHIDDeviceRegisterInputReportCallback(m_device_ref,
                m_input_report_buffer.data(),
                m_input_report_buffer.size(),
                &HIDDeviceMac::inputReportCallback,
                this);
        }
        IOHIDDeviceRegisterRemovalCallback(m_device_ref, &HIDDeviceMac::deviceRemovalCallback, this);
        
        std::mutex run_loop_mutex;
        std::unique_lock<std::mutex> run_loop_ulock(run_loop_mutex);
        std::condition_variable run_loop_cv;
        m_run_loop_future = std::async(std::launch::async, [&](){
            m_run_loop_ref = SharedCFTypeRef<CFRunLoopRef>(CFRunLoopGetCurrent(), true);
            IOHIDDeviceScheduleWithRunLoop(m_device_ref, m_run_loop_ref, kCFRunLoopDefaultMode);
            run_loop_cv.notify_one();
            CFRunLoopRun();
        });
        run_loop_cv.wait(run_loop_ulock, [&](){
            return m_run_loop_ref.get() != nullptr;
        });
        
        m_closed = false;
        return true;
    }

    bool HIDDeviceMac::close() {
        if(m_closed)
            return true;
        if(m_input_report_buffer.size() > 0) {
            IOHIDDeviceRegisterInputReportCallback(m_device_ref,
                m_input_report_buffer.data(),
                m_input_report_buffer.size(),
                nullptr,
                nullptr);
        }
        IOHIDDeviceUnscheduleFromRunLoop(m_device_ref, m_run_loop_ref, kCFRunLoopDefaultMode);
        CFRunLoopStop(m_run_loop_ref);
        IOReturn ret = IOHIDDeviceClose(m_device_ref, kIOHIDOptionsTypeNone);
        if(ret != kIOReturnSuccess) {
            std::cerr<<"Failed to close device: "<<mach_error_string(ret)<<std::endl;
            //return;
        }
        m_closed = true;
        return true;
    }

    SharedCFTypeRef<CFTypeRef> HIDDeviceMac::getProperty(CFStringRef key, bool retain) {
        return SharedCFTypeRef<CFTypeRef>(IOHIDDeviceGetProperty(m_device_ref, key), retain);
    }

    int64_t HIDDeviceMac::getIntProperty(CFStringRef key)
    {
        SharedCFTypeRef<CFTypeRef> ref = getProperty(key);
        int64_t value = 0;
        if(ref) {
            CFNumberGetValue((CFNumberRef)(ref.get()), kCFNumberSInt64Type, &value);
        }
        return value;
    }

    std::string HIDDeviceMac::getStringProperty(CFStringRef key)
    {
        SharedCFTypeRef<CFTypeRef> ref = getProperty(key, true);
        std::string value;
        if(ref) {
            CFStringRef str_ref = (CFStringRef)(ref.get());
            CFRange range = CFRangeMake(0, CFStringGetLength(str_ref));
            CFIndex max_buf_len = 0;
            // get max_buf_len
            CFStringGetBytes(str_ref, range, kCFStringEncodingUTF8, '?', false, nullptr, 0, &max_buf_len);
            value.resize(max_buf_len);
            CFStringGetBytes(str_ref, range, kCFStringEncodingUTF8, '?', false, (uint8_t *)value.data(), max_buf_len, &max_buf_len);
        }
        return value;
    }

    void HIDDeviceMac::collectProperty() {
        m_vendor_id = getIntProperty(CFSTR(kIOHIDVendorIDKey));
        m_product_id = getIntProperty(CFSTR(kIOHIDProductIDKey));
        m_version_number = getIntProperty(CFSTR(kIOHIDVersionNumberKey));
        
        m_serial_number = getStringProperty(CFSTR(kIOHIDSerialNumberKey));
        m_product = getStringProperty(CFSTR(kIOHIDProductKey));
        m_manufacturer = getStringProperty(CFSTR(kIOHIDManufacturerKey));
        
        m_max_input_report_size = getIntProperty(CFSTR(kIOHIDMaxInputReportSizeKey));
        m_max_output_report_size = getIntProperty(CFSTR(kIOHIDMaxOutputReportSizeKey));
        m_max_feature_report_size = getIntProperty(CFSTR(kIOHIDMaxFeatureReportSizeKey));
    }

    std::vector<uint8_t> HIDDeviceMac::getFeatureReport(uint8_t report_id) {
        std::vector<uint8_t> report(m_max_feature_report_size);
        CFIndex report_size = report.size();
        IOReturn result = IOHIDDeviceGetReport(m_device_ref, kIOHIDReportTypeFeature, report_id, report.data(), &report_size);
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to get feature report("<<(int)report_id<<"): "<<mach_error_string(result)<<std::endl;
            report_size = 0;
        }
        report.resize(report_size);
        return report;
    }

    void HIDDeviceMac::sendOutputReport(std::vector<uint8_t> report) {
        sendReport(kIOHIDReportTypeOutput, report);
    }

    void HIDDeviceMac::sendFeatureReport(std::vector<uint8_t> report) {
        sendReport(kIOHIDReportTypeFeature, report);
    }

    void HIDDeviceMac::sendReport(IOHIDReportType report_type, std::vector<uint8_t> report) {
        IOReturn result = IOHIDDeviceSetReport(m_device_ref, report_type, report[0], report.data(), report.size());
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to send report("<<(int)(report[0])<<"): "<<mach_error_string(result)<<std::endl;
        }
    }
    
    // static functions

    void HIDDeviceMac::inputReportCallback(void* context,
                                           IOReturn result,
                                           void* sender,
                                           IOHIDReportType type,
                                           uint32_t report_id,
                                           uint8_t * report_data,
                                           CFIndex report_size) {
        std::shared_ptr<HIDDeviceMac> device = static_cast<HIDDeviceMac *>(context)->shared_from_this();
        device->processInputReport(std::vector<uint8_t>(report_data, report_data + report_size));
    }

    void HIDDeviceMac::deviceRemovalCallback(void * context,
                                             IOReturn result,
                                             void * sender) {
        std::shared_ptr<HIDDeviceMac> device = static_cast<HIDDeviceMac *>(context)->shared_from_this();
        device->close();
    }
}