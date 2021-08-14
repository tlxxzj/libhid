#include "libhid/HidDevice.h"

namespace libhid {

    void HidDevice::processInputReport(HidReport report) {
        if(m_inputReportCallback) {
            m_inputReportCallback(report);
        } else {
            if(m_pendingInputReports.size() >= 100) {
                HidReport tmpReoprt;
                m_pendingInputReports.tryPop(tmpReoprt, 0);
            }
            m_pendingInputReports.push(std::move(report));
        }
    }

    HidReport HidDevice::getInputReport() {
        return m_pendingInputReports.pop();
    }

    bool HidDevice::tryGetInputReport(HidReport & report, double timeoutInSeconds) {
        return m_pendingInputReports.tryPop(report, timeoutInSeconds);
    }

    void HidDevice::getInputReportAsync(ReportCallback callback) {
        m_pendingInputReports.clear();
        m_inputReportCallback = callback;
    }
}