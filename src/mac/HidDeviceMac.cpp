#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>

#include <mach/mach_error.h>

#include "HidDeviceMac.h"

namespace libhid {
    HidDeviceMac::HidDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & device_ref):
    HidDevice(true),
    m_device_ref(device_ref) {
        collectProperty();
        open();
    }
    
    HidDeviceMac::~HidDeviceMac() {
        close();
    }

    bool HidDeviceMac::open() {
        if(!m_closed)
            return true;
        IOReturn ret = IOHIDDeviceOpen(m_device_ref, kIOHIDOptionsTypeNone);
        if(ret != kIOReturnSuccess) {
            std::cerr<<"Failed to open device: "<<mach_error_string(ret)<<std::endl;
            return false;
        }
        
        m_input_report_buffer.resize(maxInputReportSize());
        if(m_input_report_buffer.size() > 0) {
            IOHIDDeviceRegisterInputReportCallback(m_device_ref,
                m_input_report_buffer.data(),
                m_input_report_buffer.size(),
                &HidDeviceMac::inputReportCallback,
                this);
        }
        IOHIDDeviceRegisterRemovalCallback(m_device_ref, &HidDeviceMac::deviceRemovalCallback, this);
        
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

    bool HidDeviceMac::close() {
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

    SharedCFTypeRef<CFTypeRef> HidDeviceMac::getProperty(CFStringRef key, bool retain) {
        return SharedCFTypeRef<CFTypeRef>(IOHIDDeviceGetProperty(m_device_ref, key), retain);
    }

    int64_t HidDeviceMac::getIntProperty(CFStringRef key)
    {
        SharedCFTypeRef<CFTypeRef> ref = getProperty(key);
        int64_t value = 0;
        if(ref) {
            CFNumberGetValue((CFNumberRef)(ref.get()), kCFNumberSInt64Type, &value);
        }
        return value;
    }

    std::string HidDeviceMac::getStringProperty(CFStringRef key)
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

    void HidDeviceMac::collectProperty() {
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

    HidReport HidDeviceMac::getFeatureReport(uint8_t report_id) {
        HidReport report(m_max_feature_report_size);
        CFIndex report_size = report.size();
        IOReturn result = IOHIDDeviceGetReport(m_device_ref, kIOHIDReportTypeFeature, report_id, report.data(), &report_size);
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to get feature report("<<(int)report_id<<"): "<<mach_error_string(result)<<std::endl;
            report_size = 0;
        }
        report.resize(report_size);
        return report;
    }

    void HidDeviceMac::sendOutputReport(HidReport report) {
        sendReport(kIOHIDReportTypeOutput, report);
    }

    void HidDeviceMac::sendFeatureReport(HidReport report) {
        sendReport(kIOHIDReportTypeFeature, report);
    }

    void HidDeviceMac::sendReport(IOHIDReportType report_type, HidReport report) {
        uint8_t report_id = report[0];
        uint8_t * report_data = report.data();
        size_t report_size = report.size();
        if(report_id == 0) {
            ++report_data;
            --report_size;
        }
        IOReturn result = IOHIDDeviceSetReport(m_device_ref, report_type, report_id, report_data, report_size);
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to send report: "<<mach_error_string(result)<<std::endl;
        }
    }
    
    // static functions

    void HidDeviceMac::inputReportCallback(void* context,
                                           IOReturn result,
                                           void* sender,
                                           IOHIDReportType type,
                                           uint32_t report_id,
                                           uint8_t * report_data,
                                           CFIndex report_size) {
        std::shared_ptr<HidDeviceMac> device = static_cast<HidDeviceMac *>(context)->shared_from_this();
        HidReport report;
        if(report_id == 0) {
            report.resize(report_size + 1);
            report[0] = 0u;
            std::copy(report_data, report_data + report_size, &report[1]);
        } else {
            report.resize(report_size);
            std::copy(report_data, report_data + report_size, &report[0]);
        }
        device->processInputReport(report);
    }

    void HidDeviceMac::deviceRemovalCallback(void * context,
                                             IOReturn result,
                                             void * sender) {
        std::shared_ptr<HidDeviceMac> device = static_cast<HidDeviceMac *>(context)->shared_from_this();
        device->close();
    }
}