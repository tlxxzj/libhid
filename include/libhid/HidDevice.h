#ifndef HIDDEVICE_H
#define HIDDEVICE_H

#include <cstdint>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>

#include "libhid/ThreadSafeQueue.h"
#include "libhid/HidReport.h"

namespace libhid {

class HidDevice {
public:
    using ReportCallback = std::function<void(HidReport report)>;

protected:
    bool m_closed = true;

    int64_t m_vendorId = 0;
    int64_t m_productId = 0;
    int64_t m_versionNumber = 0;
    
    std::string m_product;
    std::string m_manufacturer;
    std::string m_serialNumber;

    size_t m_maxInputReportSize = 0;
    size_t m_maxOutputReportSize = 0;
    size_t m_maxFeatureReportSize = 0;

    std::vector<uint8_t> m_inputReportBuffer;
    ThreadSafeQueue<HidReport> m_pendingInputReports;

    ReportCallback m_inputReportCallback;
    
    HidDevice(bool closed): m_closed(closed) {};

    virtual void processInputReport(HidReport report);

public:
    
    HidDevice() = delete;
    HidDevice(const HidDevice &) = delete;
    HidDevice(HidDevice &&) = delete;
    HidDevice & operator=(const HidDevice &) = delete;
    HidDevice & operator=(HidDevice &&) = delete;
    
    virtual ~HidDevice() {};

    virtual bool close() = 0;
    
    bool opened() { return !m_closed; }
    bool closed() { return m_closed; }
    
    int64_t vendorId() { return m_vendorId; }
    int64_t productId() { return m_productId; }
    int64_t versionNumber() { return m_versionNumber; }
    
    std::string product() { return m_product; }
    std::string manufacturer() { return m_manufacturer; }
    std::string serialNumber() { return m_serialNumber; }

    size_t maxInputReportSize() { return m_maxInputReportSize; }
    size_t maxOutputReportSize() { return m_maxOutputReportSize; }
    size_t maxFeatureReportSize() { return m_maxFeatureReportSize; }

    virtual HidReport getInputReport();
    virtual bool tryGetInputReport(HidReport & report, double timeoutInSeconds = 0);
    virtual void getInputReportAsync(ReportCallback callback);
    virtual void sendOutputReport(HidReport report) = 0;
    virtual void sendFeatureReport(HidReport report) = 0;
    virtual HidReport getFeatureReport(uint8_t reportId) = 0;
};


}


#endif

