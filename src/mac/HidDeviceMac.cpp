#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>

#include <mach/mach_error.h>

#include "HidDeviceMac.h"

namespace libhid {
    HidDeviceMac::HidDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & deviceRef):
    HidDevice(true),
    m_deviceRef(deviceRef) {
        collectProperty();
        open();
    }
    
    HidDeviceMac::~HidDeviceMac() {
        close();
    }

    bool HidDeviceMac::open() {
        if(!m_closed)
            return true;
        IOReturn ret = IOHIDDeviceOpen(m_deviceRef, kIOHIDOptionsTypeNone);
        if(ret != kIOReturnSuccess) {
            std::cerr<<"Failed to open device: "<<mach_error_string(ret)<<std::endl;
            return false;
        }
        
        m_inputReportBuffer.resize(maxInputReportSize());
        if(m_inputReportBuffer.size() > 0) {
            IOHIDDeviceRegisterInputReportCallback(m_deviceRef,
                m_inputReportBuffer.data(),
                m_inputReportBuffer.size(),
                &HidDeviceMac::inputReportCallback,
                this);
        }
        IOHIDDeviceRegisterRemovalCallback(m_deviceRef, &HidDeviceMac::deviceRemovalCallback, this);
        
        std::mutex runLoopMutex;
        std::unique_lock<std::mutex> runLoopLock(runLoopMutex);
        std::condition_variable runLoopCv;
        m_runLoopFuture = std::async(std::launch::async, [&](){
            m_runLoopRef = SharedCFTypeRef<CFRunLoopRef>(CFRunLoopGetCurrent(), true);
            IOHIDDeviceScheduleWithRunLoop(m_deviceRef, m_runLoopRef, kCFRunLoopDefaultMode);
            runLoopCv.notify_one();
            CFRunLoopRun();
        });
        runLoopCv.wait(runLoopLock, [&](){
            return m_runLoopRef.get() != nullptr;
        });
        
        m_closed = false;
        return true;
    }

    bool HidDeviceMac::close() {
        if(m_closed)
            return true;
        if(m_inputReportBuffer.size() > 0) {
            IOHIDDeviceRegisterInputReportCallback(m_deviceRef,
                m_inputReportBuffer.data(),
                m_inputReportBuffer.size(),
                nullptr,
                nullptr);
        }
        IOHIDDeviceUnscheduleFromRunLoop(m_deviceRef, m_runLoopRef, kCFRunLoopDefaultMode);
        CFRunLoopStop(m_runLoopRef);
        IOReturn ret = IOHIDDeviceClose(m_deviceRef, kIOHIDOptionsTypeNone);
        if(ret != kIOReturnSuccess) {
            std::cerr<<"Failed to close device: "<<mach_error_string(ret)<<std::endl;
            //return;
        }
        m_closed = true;
        return true;
    }

    SharedCFTypeRef<CFTypeRef> HidDeviceMac::getProperty(CFStringRef key, bool retain) {
        return SharedCFTypeRef<CFTypeRef>(IOHIDDeviceGetProperty(m_deviceRef, key), retain);
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
            CFStringRef strRef = (CFStringRef)(ref.get());
            CFRange range = CFRangeMake(0, CFStringGetLength(strRef));
            CFIndex max_buf_len = 0;
            // get max_buf_len
            CFStringGetBytes(strRef, range, kCFStringEncodingUTF8, '?', false, nullptr, 0, &max_buf_len);
            value.resize(max_buf_len);
            CFStringGetBytes(strRef, range, kCFStringEncodingUTF8, '?', false, (uint8_t *)value.data(), max_buf_len, &max_buf_len);
        }
        return value;
    }

    void HidDeviceMac::collectProperty() {
        m_vendorId = getIntProperty(CFSTR(kIOHIDVendorIDKey));
        m_productId = getIntProperty(CFSTR(kIOHIDProductIDKey));
        m_versionNumber = getIntProperty(CFSTR(kIOHIDVersionNumberKey));
        
        m_serialNumber = getStringProperty(CFSTR(kIOHIDSerialNumberKey));
        m_product = getStringProperty(CFSTR(kIOHIDProductKey));
        m_manufacturer = getStringProperty(CFSTR(kIOHIDManufacturerKey));
        
        m_maxInputReportSize = getIntProperty(CFSTR(kIOHIDMaxInputReportSizeKey));
        m_maxOutputReportSize = getIntProperty(CFSTR(kIOHIDMaxOutputReportSizeKey));
        m_maxFeatureReportSize = getIntProperty(CFSTR(kIOHIDMaxFeatureReportSizeKey));
    }

    HidReport HidDeviceMac::getFeatureReport(uint8_t reportId) {
        HidReport report(m_maxFeatureReportSize);
        CFIndex reportSize = report.size();
        IOReturn result = IOHIDDeviceGetReport(m_deviceRef, kIOHIDReportTypeFeature, reportId, report.data(), &reportSize);
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to get feature report("<<(int)reportId<<"): "<<mach_error_string(result)<<std::endl;
            reportSize = 0;
        }
        report.resize(reportSize);
        return report;
    }

    void HidDeviceMac::sendOutputReport(HidReport report) {
        sendReport(kIOHIDReportTypeOutput, report);
    }

    void HidDeviceMac::sendFeatureReport(HidReport report) {
        sendReport(kIOHIDReportTypeFeature, report);
    }

    void HidDeviceMac::sendReport(IOHIDReportType reportType, HidReport report) {
        uint8_t reportId = report[0];
        uint8_t * reportData = report.data();
        size_t reportSize = report.size();
        if(reportId == 0) {
            ++reportData;
            --reportSize;
        }
        IOReturn result = IOHIDDeviceSetReport(m_deviceRef, reportType, reportId, reportData, reportSize);
        if(result != kIOReturnSuccess) {
            std::cerr<<"Failed to send report: "<<mach_error_string(result)<<std::endl;
        }
    }
    
    // static functions

    void HidDeviceMac::inputReportCallback(void* context,
                                           IOReturn result,
                                           void* sender,
                                           IOHIDReportType type,
                                           uint32_t reportId,
                                           uint8_t * reportData,
                                           CFIndex reportSize) {
        std::shared_ptr<HidDeviceMac> device = static_cast<HidDeviceMac *>(context)->shared_from_this();
        HidReport report;
        if(reportId == 0) {
            report.resize(reportSize + 1);
            report[0] = 0u;
            std::copy(reportData, reportData + reportSize, &report[1]);
        } else {
            report.resize(reportSize);
            std::copy(reportData, reportData + reportSize, &report[0]);
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