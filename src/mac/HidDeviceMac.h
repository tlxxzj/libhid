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
    SharedCFTypeRef<IOHIDDeviceRef> m_deviceRef;
    SharedCFTypeRef<CFRunLoopRef> m_runLoopRef;
    std::future<void> m_runLoopFuture;

    bool open();

    
public:
    HidDeviceMac(const HidDeviceMac &) = delete;
    HidDeviceMac & operator=(const HidDeviceMac &) = delete;
    HidDeviceMac(const SharedCFTypeRef<IOHIDDeviceRef> & deviceRef);
    ~HidDeviceMac();

    bool close() override;

    void sendOutputReport(HidReport report) override;
    
    HidReport getFeatureReport(uint8_t reportId) override;
    void sendFeatureReport(HidReport report) override;

protected:
    SharedCFTypeRef<CFTypeRef> getProperty(CFStringRef key, bool retain = false);
    int64_t getIntProperty(CFStringRef key);
    std::string getStringProperty(CFStringRef key);
    
    void sendReport(IOHIDReportType reportType, HidReport report);
    
    void collectProperty();

    static void inputReportCallback(void* context,
                                    IOReturn result,
                                    void* sender,
                                    IOHIDReportType type,
                                    uint32_t reportId,
                                    uint8_t * reportData,
                                    CFIndex reportSize);
    
    static void deviceRemovalCallback(void * context,
                                      IOReturn result,
                                      void * sender);

};


}


#endif

