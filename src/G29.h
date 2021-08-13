#ifndef G29_H
#define G29_H

#include <vector>
#include <cstdint>

#include "HidManager.h"


namespace logitech {

struct LogitechG29State {
    uint16_t wheel;
    
    uint16_t accelerator;
    uint16_t brake;
    uint16_t clutch;

    uint8_t dpad;
    uint8_t cross;
    uint8_t square;
    uint8_t triangle;
    uint8_t circle;
    
    uint8_t l1;
    uint8_t l2;
    uint8_t l3;
    uint8_t r1;
    uint8_t r2;
    uint8_t r3;
    uint8_t plus;
    uint8_t minus;
    
    uint8_t optoins;
    uint8_t share;
    uint8_t ps;
    uint8_t redDialEnter;
    int8_t redDial;
};


class LogitechG29 {
public:
    static constexpr const char * name = "G29 Driving Force Racing Wheel";
    static constexpr const int64_t vendorId = 0x046d;
    static constexpr const int64_t productId = 0xc260;

protected:
    std::shared_ptr<libhid::HidDevice> mDevice;

public:
    LogitechG29(int32_t index = 0);
    LogitechG29State getState();
};

}

#endif