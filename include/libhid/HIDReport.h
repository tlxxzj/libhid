//
//  HIDReport.hpp
//  test2
//
//  Created by Lang Tian on 2021/8/5.
//

#ifndef HIDReport_hpp
#define HIDReport_hpp


#include <vector>


namespace libhid {

enum HidReportType {
    kInput = 0,
    kOutput,
    kFeature
};

class HIDReport{
protected:
    HidReportType m_type = HidReportType::kInput;
    uint8_t m_id = 0;
    std::vector<uint8_t> m_data;

public:
    HIDReport() {};
    
    HIDReport(HidReportType type, uint8_t id, uint8_t * data, size_t size):
    m_type(type),
    m_id(id),
    m_data(data, data + size) {
    }

    HIDReport(const HIDReport & other):
    m_type(other.m_type),
    m_id(other.m_id),
    m_data(other.m_data) {
    }

    HIDReport(HIDReport && other):
    m_type(other.m_type),
    m_id(other.m_id),
    m_data(std::move(other.m_data)) {
    }

    HIDReport & operator=(const HIDReport & other) {
        m_type = other.m_type;
        m_id = other.m_id;
        m_data = other.m_data;
        return *this;
    }

    HIDReport & operator=(HIDReport && other) {
        m_type = other.m_type;
        m_id = other.m_id;
        m_data = std::move(other.m_data);
        return *this;
    }

    HidReportType type() { return m_type; };
    uint8_t id() { return m_id; };
    size_t size() { return m_data.size(); }
    uint8_t * data() { return m_data.data(); }
    uint8_t * begin() { return m_data.data(); }
    uint8_t * end() { return m_data.data() + m_data.size(); }
};

}

#endif



