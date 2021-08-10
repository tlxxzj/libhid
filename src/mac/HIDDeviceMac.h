#ifndef HIDDEVICEMAC_H
#define HIDDEVICEMAC_H

#include <future>

#include <IOKit/hid/IOHIDLib.h>

#include "libhid/HIDDevice.h"
#include "libhid/HIDReport.h"
#include "SharedCFTypeRef.h"
#include "ThreadSafeQueue.h"


namespace libhid {


class HIDDeviceMac:
    public HIDDevice,
    public std::enable_shared_from_this<HIDDeviceMac> {

protected:
    SharedCFTypeRef<IOHIDDeviceRef> m_device_ref;
    SharedCFTypeRef<CFRunLoopRef> m_run_loop_ref;
    std::future<void> m_run_loop_future;

    bool open();

    
public:
    HIDDeviceMac(const HIDDeviceMac &) = delete;
    HIDDeviceMac & operator=(const HIDDeviceMac &) = delete;
    HIDDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & device_ref);
    ~HIDDeviceMac();

    bool close() override;

    void sendOutputReport(std::vector<uint8_t> report) override;
    
    std::vector<uint8_t> getFeatureReport(uint8_t report_id) override;
    void sendFeatureReport(std::vector<uint8_t> report) override;

protected:
    SharedCFTypeRef<CFTypeRef> getProperty(CFStringRef key, bool retain = false);
    int64_t getIntProperty(CFStringRef key);
    std::string getStringProperty(CFStringRef key);
    
    void sendReport(IOHIDReportType report_type, std::vector<uint8_t> report);
    
    void collectProperty();

    static void inputReportCallback(void* context,
                                    IOReturn result,
                                    void* sender,
                                    IOHIDReportType type,
                                    uint32_t report_id,
                                    uint8_t * report_data,
                                    CFIndex report_size);
    
    static void deviceRemovalCallback(void * context,
                                      IOReturn result,
                                      void * sender);

};


}


#endif

