#ifndef HIDDEVICE_H
#define HIDDEVICE_H

#include <cstdint>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>

#include "ThreadSafeQueue.h"
#include "HidReport.h"

namespace libhid {

class HidDevice {
public:
    using ReportCallback = std::function<void(std::vector<uint8_t> report)>;

protected:
    bool m_closed = true;

    int64_t m_vendor_id = 0;
    int64_t m_product_id = 0;
    int64_t m_version_number = 0;
    
    std::string m_product;
    std::string m_manufacturer;
    std::string m_serial_number;

    size_t m_max_input_report_size = 0;
    size_t m_max_output_report_size = 0;
    size_t m_max_feature_report_size = 0;

    std::vector<uint8_t> m_input_report_buffer;
    ThreadSafeQueue<std::vector<uint8_t>> m_pending_input_reports;

    ReportCallback m_input_report_callback;
    
    HidDevice(bool closed): m_closed(closed) {};

    virtual void processInputReport(std::vector<uint8_t> report);

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
    
    int64_t vendorId() { return m_vendor_id; }
    int64_t productId() { return m_product_id; }
    int64_t versionNumber() { return m_version_number; }
    
    std::string product() { return m_product; }
    std::string manufacturer() { return m_manufacturer; }
    std::string serialNumber() { return m_serial_number; }

    size_t maxInputReportSize() { return m_max_input_report_size; }
    size_t maxOutputReportSize() { return m_max_output_report_size; }
    size_t maxFeatureReportSize() { return m_max_feature_report_size; }

    virtual std::vector<uint8_t> getInputReport();
    virtual bool tryGetInputReport(std::vector<uint8_t> & report, double timeout_in_seconds = 0);
    virtual void getInputReportAsync(ReportCallback callback);
    virtual void sendOutputReport(std::vector<uint8_t> report) = 0;
    virtual void sendFeatureReport(std::vector<uint8_t> report) = 0;
    virtual std::vector<uint8_t> getFeatureReport(uint8_t report_id) = 0;
};


}


#endif

