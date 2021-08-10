#ifndef G29_H
#define G29_H


struct HIDInputReportData {};

struct G29InputReport: HIDInputReportData {
    uint8_t report_id; /* 0x01 */
    uint8_t x; /* 0x80 */
    uint8_t y; /* 0x80 */
    uint8_t z; /* 0x80 */
    uint8_t rz; /* 0x80 */
    uint8_t hatswitchandbuttons[3];
    uint8_t rx;
    uint8_t ry;
    uint8_t unknown[33];
    uint8_t wheel[2]; /*  */
    uint8_t accelerator[2];
    uint8_t brake[2];
    uint8_t clutch[2];
    uint8_t buttons2;
    uint8_t unknown2[2];
    uint8_t buttons3;
    uint8_t unknown3[9];
};

struct G29State {
    uint16_t wheel;
    uint8_t wheel_left;
    uint8_t wheel_right;
    
    uint16_t accelerator;
    uint16_t brake;
    uint16_t clutch;

    /*
    uint8_t dpad_up;
    uint8_t dpad_up_right;
    uint8_t dpad_right;
    uint8_t dpad_right_down;
    uint8_t dpad_down;
    uint8_t dpad_down_left;
    uint8_t dpad_left;
    uint8_t dpad_left_up;
    */
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
    
    uint8_t optoins
    uint8_t share;
    uint8_t ps;

    
};

#endif