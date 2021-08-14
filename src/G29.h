#ifndef G29_H
#define G29_H

#include <vector>
#include <cstdint>

#include "HidManager.h"

// https://opensource.logitech.com/opensource/images/6/6e/Logitech_Force_Feedback_Protocol_V1.6.pdf

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

    static constexpr const uint16_t minRange = 40;
    static constexpr const uint16_t maxRange = 900;

protected:
    std::shared_ptr<libhid::HidDevice> mDevice;
    //void sendCommand(uint8_t cmd[]);

public:
    LogitechG29(int32_t index = 0);
    LogitechG29State getState();
    void setRange(uint16_t range);
    void setAutocenter();
    void setLeds(uint8_t leds);
};

}

#endif