#ifndef __XONE_CONTROLLER_H__
#define __XONE_CONTROLLER_H__

#include "../controller.h"

uint8_t XboxOneController_probe(Controller *c, int device_id, int port, int vendor, int product);
uint8_t XboxOneController_processReport(Controller *c, size_t length);
void XboxOneController_setRumble(Controller *c, uint8_t small, uint8_t large);

enum gip_ctrl {
  GIP_CTRL_ACK = 0x00,
  GIP_CTRL_NACK = 0x01, // UNUSED
};

// have GIP_MSG_FLAG_SYSTEM
enum gip_system_messages {
  GIP_MSG_CONTROL = 0x01,
  GIP_MSG_HELLO = 0x02,
  GIP_MSG_STATUS = 0x03,
  GIP_MSG_METADATA = 0x04,
  GIP_MSG_STATE = 0x05,
  GIP_MSG_SECURITY = 0x06,
  GIP_MSG_GUIDE = 0x07,
  GIP_MSG_AUDIO_CONTROL = 0x08,
  GIP_MSG_LED = 0x0a,
  GIP_MSG_HID_REPORT = 0x0b,
  GIP_MSG_FIRMWARE = 0x0c,
  GIP_MSG_EXTENDED = 0x1e,
  GIP_MSG_DEBUG = 0x1f,
  GIP_MSG_AUDIO_SAMPLES = 0x60,
};

// don't have GIP_MSG_FLAG_SYSTEM
enum gip_vendor_messages {
  GIP_MSG_VENDOR_DEVICE_CAPABILITIES = 0x00,
  GIP_MSG_VENDOR_LED_CAPABILITIES = 0x01,
  GIP_MSG_VENDOR_LED_STATE = 0x01,
  GIP_MSG_VENDOR_RUMBLE = 0x09,
  GIP_MSG_VENDOR_WHEEL = 0x0a,
  GIP_MSG_VENDOR_INPUT = 0x20,
  GIP_MSG_VENDOR_STATIC_CONFIG = 0x21,
  GIP_MSG_VENDOR_OVERFLOW_INPUT = 0x26,
};

enum gip_vendor_wheel_requests {
    GIP_VENDOR_WHEEL_INITIAL_STATE = 0x00,
    GIP_VENDOR_WHEEL_ANGLE_CHANGE = 0x01,
    GIP_VENDOR_WHEEL_POWER_LEVEL = 0x02,
    GIP_VENDOR_WHEEL_VIBRATION_MOTOR = 0x03,
};

enum gip_device_state {
  GIP_STATE_START = 0x00,
  GIP_STATE_STOP = 0x01,
  GIP_STATE_STANDBY = 0x03, // UNUSED
  GIP_STATE_OFF = 0x04,
  GIP_STATE_QUIESCE = 0x05,
  GIP_STATE_RESERVED = 0x06,
  GIP_STATE_RESET = 0x07,
};

enum gip_msg_flags {
  GIP_MSG_FLAG_NONE = 0x0,
  GIP_MSG_FLAG_ACME = 0x10,
  GIP_MSG_FLAG_SYSTEM = 0x20,
  GIP_MSG_FLAG_INITFRAG = 0x40,
  GIP_MSG_FLAG_FRAG = 0x80,
};

enum gip_type {
  GIP_TYPE_GAMEPAD = 0x0,
  GIP_TYPE_WHEEL = 0x1,
  GIP_TYPE_FLIGHTSTICK = 0x2,
};

#define GIP_LED_ON 0x01

#define GIP_MOTOR_R  0x0
#define GIP_MOTOR_L  0x1
#define GIP_MOTOR_RT 0x2
#define GIP_MOTOR_LT 0x4
#define GIP_MOTOR_ALL (GIP_MOTOR_R | GIP_MOTOR_L | GIP_MOTOR_RT | GIP_MOTOR_LT)


#endif // __XONE_CONTROLLER_H__
