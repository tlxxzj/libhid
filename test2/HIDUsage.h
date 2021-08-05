//
//  HIDUsage.hpp
//  test2
//
//  Created by Lang Tian on 2021/7/30.
//

#ifndef HIDUsage_hpp
#define HIDUsage_hpp

#include <cstdio>
#include <cstdint>

enum class HIDUsagePageID
{
    Undefinied = 0x00,
    GenericDesktop = 0x01,
    SimulationControls = 0x02,
    VRControls = 0x03,
    SportControls = 0x04,
    GameControls = 0x05,
    GenericDeviceControls = 0x06,
    KeyboardOrKeypad = 0x07,
    LED = 0x08,
    Button = 0x09,
    Ordinal = 0x0a,
    TelephonyDevice = 0x0b,
    Consumer = 0x0c,
    Digitizers = 0x0d,
    Haptics = 0x0e,
    PID = 0x0f,
    Unicode = 0x10,
    EyeAndHeadTrackers = 0x12,
    AuxiliaryDisplay = 0x14,
    Sensors = 0x20,
    MedicalInstrument = 0x40,
    BrailleDisplay = 0x41,
    LightingAndIllumination = 0x59,
    BarCodeScanner = 0x8c,
    Scale = 0x8d,
    MagneticStripeReadingDevices = 0x8e,
    CameraControl = 0x90,
    Arcade = 0x91,
    GamingDevice = 0x92,
};


template <HIDUsagePageID>
struct HIDUsageID;

template <>
struct HIDUsageID<HIDUsagePageID::GenericDesktop>
{
    enum
    {
        Undefinied = 0x00,
        Pointer = 0x01,
        Mouse = 0x03,
        Joystick = 0x04,
        Gamepad = 0x05,
        Keyboard = 0x06,
        Keypad = 0x07,
        MultiaxisController = 0x08,
        TabletPCSystemControls = 0x09,
    };
};




#endif /* HIDUsage_hpp */
