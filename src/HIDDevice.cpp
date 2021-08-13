#include <future>
#include <iostream>
#include <iomanip>

#include "HidDevice.h"

namespace libhid {

    void HidDevice::processInputReport(std::vector<uint8_t> report) {
        if(m_input_report_callback) {
            m_input_report_callback(report);
        } else {
            if(m_pending_input_reports.size() >= 100) {
                std::vector<uint8_t> tmp_reoprt;
                m_pending_input_reports.tryPop(tmp_reoprt, 0);
            }
            m_pending_input_reports.push(std::move(report));
        }
    }

    std::vector<uint8_t> HidDevice::getInputReport() {
        return m_pending_input_reports.pop();
    }

    bool HidDevice::tryGetInputReport(std::vector<uint8_t> & report, double timeout_in_seconds) {
        return m_pending_input_reports.tryPop(report, timeout_in_seconds);
    }

    void HidDevice::getInputReportAsync(ReportCallback callback) {
        m_pending_input_reports.clear();
        m_input_report_callback = callback;
    }
}