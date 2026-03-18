#ifndef __DINPUT_LOGITECH_E3DPRO_H__
#define __DINPUT_LOGITECH_E3DPRO_H__
#include <psp2kern/ctrl.h>
#include "../utils/rescale.h"

static inline void process_dpad_angle_high(Controller *c, int idx)
{
    uint8_t dpad = (c->buffer[idx] & 0xF0) >> 4;
    switch (dpad)
    {
      case 0x0:
        c->controlData.buttons |= SCE_CTRL_UP;
        break;
      case 0x1:
        c->controlData.buttons |= SCE_CTRL_UP;
        c->controlData.buttons |= SCE_CTRL_RIGHT;
        break;
      case 0x2:
        c->controlData.buttons |= SCE_CTRL_RIGHT;
        break;
      case 0x3:
        c->controlData.buttons |= SCE_CTRL_RIGHT;
        c->controlData.buttons |= SCE_CTRL_DOWN;
        break;
      case 0x4:
        c->controlData.buttons |= SCE_CTRL_DOWN;
        break;
      case 0x5:
        c->controlData.buttons |= SCE_CTRL_DOWN;
        c->controlData.buttons |= SCE_CTRL_LEFT;
        break;
      case 0x6:
        c->controlData.buttons |= SCE_CTRL_LEFT;
        break;
      case 0x7:
        c->controlData.buttons |= SCE_CTRL_LEFT;
        c->controlData.buttons |= SCE_CTRL_UP;
        break;
      case 0x8:
      default:
        break;
    }
}

uint8_t logitech_e3dpro_processReport(Controller *c, size_t length)
{

    process_dpad_angle_high(c, 2);


    if (bit(c->buffer + 4, 0))
      c->controlData.buttons |= SCE_CTRL_CROSS;

    if (bit(c->buffer + 4, 1))
      c->controlData.buttons |= SCE_CTRL_CIRCLE;

    if (bit(c->buffer + 4, 2))
      c->controlData.buttons |= SCE_CTRL_SQUARE;

    if (bit(c->buffer + 4, 3))
      c->controlData.buttons |= SCE_CTRL_TRIANGLE;

    if (bit(c->buffer + 4, 4))
      c->controlData.buttons |= SCE_CTRL_SELECT;

    if (bit(c->buffer + 4, 5))
      c->controlData.buttons |= SCE_CTRL_START;

    if (bit(c->buffer + 4, 6))
      c->controlData.buttons |= SCE_CTRL_L1;
    if (bit(c->buffer + 4, 7))
      c->controlData.buttons |= SCE_CTRL_R1;

    if (bit(c->buffer + 6, 0))
    {
      c->controlData.buttons |= SCE_CTRL_LTRIGGER;
      c->controlData.lt = 0xFF;
    }

    if (bit(c->buffer + 6, 1))
    {
      c->controlData.buttons |= SCE_CTRL_RTRIGGER;
      c->controlData.rt = 0xFF;
    }

    if (bit(c->buffer + 6, 2))
      c->controlData.buttons |= SCE_CTRL_L3;

    if (bit(c->buffer + 6, 3))
      c->controlData.buttons |= SCE_CTRL_R3;

    if (bit(c->buffer + 4, 6) && bit(c->buffer + 4, 7) && bit(c->buffer + 4, 5)) // R+L+START combo
    {
      c->controlData.buttons |= SCE_CTRL_PSBUTTON;
      c->controlData.buttons &= ~SCE_CTRL_START;
    }

    uint32_t lx = ((c->buffer[1] & 0x03) << 8) | c->buffer[0];

    uint32_t ly = ((c->buffer[2] & 0x0F) << 6) | ((c->buffer[1] & 0xFC) >> 2);

    c->controlData.leftX  = rescale(lx, 10, 8);
    c->controlData.leftY  = rescale(ly, 10, 8);

    c->controlData.rightX = c->buffer[3];
    c->controlData.rightY = c->buffer[5];


    return 1;
}

#endif
