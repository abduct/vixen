#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

//#include "devicelist.h"

#include <psp2common/types.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/usbd.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
  uint32_t buttons;
  uint8_t leftX;
  uint8_t leftY;
  uint8_t rightX;
  uint8_t rightY;
  uint8_t lt;
  uint8_t rt;
} ControlData;

struct Controller;
typedef struct Controller Controller;

typedef struct
{
  uint8_t angle_bits;
  uint8_t throttle_bits;
  uint8_t brake_bits;
  uint8_t clutch_bits;
  uint8_t handbrake_bits;
  uint16_t min_angle;
  uint16_t max_angle;
  uint8_t ff_mask;
  uint8_t caps;
} __attribute__((packed)) XoneWheelConfig;


struct Controller
{
  uint8_t type;
  uint8_t attached; // actual gamepad attached
  uint8_t inited;   // usb device attached and inited
  uint8_t battery_level;
  ControlData controlData;
  int device_id;
  uint8_t port;
  SceUID pipe_in;
  SceUID pipe_out;
  SceUID pipe_control;
  unsigned char buffer[64] __attribute__((aligned(64)));
  size_t buffer_size;
  int vendor;
  int product;
  uint8_t (*processReport)(Controller *c, size_t length);
  void (*setRumble)(Controller *c, uint8_t small, uint8_t large);

  // TODO: move to struct?
  uint8_t xone_seq_system;
  uint8_t xone_seq_vendor;
  uint16_t xone_fragment_full_size;
  uint16_t xone_fragment_offset;
  uint8_t xone_type;
  uint8_t *xone_metadata;
  uint8_t xone_guide_pressed;
  // wheel config
  XoneWheelConfig xone_wheel_config;
};

void usb_read(Controller *c);
void usb_write(Controller *c, uint8_t *data, int len);


#define HID_GET_REPORT 0x01
#define HID_GET_IDLE 0x02
#define HID_GET_PROTOCOL 0x03
#define HID_SET_REPORT 0x09
#define HID_SET_IDLE 0x0A
#define HID_SET_PROTOCOL 0x0B

#define HID_REPORT_TYPE_INPUT 0x01
#define HID_REPORT_TYPE_OUTPUT 0x02
#define HID_REPORT_TYPE_FEATURE 0x03

#define EVF_SEND 1
#define EVF_RECV 2
#define EVF_CTRL 4


#endif // __CONTROLLER_H__
