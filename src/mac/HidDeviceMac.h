#ifndef HIDDEVICEMAC_H
#define HIDDEVICEMAC_H

#include <future>

#include <IOKit/hid/IOHIDLib.h>

#include "libhid/HidDevice.h"
#include "SharedCFTypeRef.h"


namespace libhid {


class HidDeviceMac:
    public HidDevice,
    public std::enable_shared_from_this<HidDeviceMac> {

protected:
    SharedCFTypeRef<IOHIDDeviceRef> m_device_ref;
    SharedCFTypeRef<CFRunLoopRef> m_run_loop_ref;
    std::future<void> m_run_loop_future;

    bool open();

    
public:
    HidDeviceMac(const HidDeviceMac &) = delete;
    HidDeviceMac & operator=(const HidDeviceMac &) = delete;
    HidDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & device_ref);
    ~HidDeviceMac();

    bool close() override;

    void sendOutputReport(HidReport report) override;
    
    HidReport getFeatureReport(uint8_t report_id) override;
    void sendFeatureReport(HidReport report) override;

protected:
    SharedCFTypeRef<CFTypeRef> getProperty(CFStringRef key, bool retain = false);
    int64_t getIntProperty(CFStringRef key);
    std::string getStringProperty(CFStringRef key);
    
    void sendReport(IOHIDReportType report_type, HidReport report);
    
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

