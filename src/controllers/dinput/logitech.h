#ifndef __DINPUT_LOGITECH_H__
#define __DINPUT_LOGITECH_H__
#include <psp2kern/ctrl.h>
#include "dpad_angle.h"

uint8_t logitech_processReport(Controller *c, size_t length)
{
    process_dpad_angle(c, 4);

    if (bit(c->buffer + 4, 5))
      c->controlData.buttons |= SCE_CTRL_CROSS;
    if (bit(c->buffer + 4, 6))
      c->controlData.buttons |= SCE_CTRL_CIRCLE;
    if (bit(c->buffer + 4, 7))
      c->controlData.buttons |= SCE_CTRL_TRIANGLE;
    if (bit(c->buffer + 4, 4))
      c->controlData.buttons |= SCE_CTRL_SQUARE;

    if (bit(c->buffer + 5, 0))
      c->controlData.buttons |= SCE_CTRL_L1;
    if (bit(c->buffer + 5, 1))
      c->controlData.buttons |= SCE_CTRL_R1;
    if (bit(c->buffer + 5, 6))
      c->controlData.buttons |= SCE_CTRL_L3;
    if (bit(c->buffer + 5, 7))
      c->controlData.buttons |= SCE_CTRL_R3;

    if (bit(c->buffer + 5, 2))
    {
      c->controlData.buttons |= SCE_CTRL_LTRIGGER;
      c->controlData.lt = 0xFF;
    }

    if (bit(c->buffer + 5, 3))
    {
      c->controlData.buttons |= SCE_CTRL_RTRIGGER;
      c->controlData.rt = 0xFF;
    }

    if (bit(c->buffer + 5, 5))
      c->controlData.buttons |= SCE_CTRL_START;
    if (bit(c->buffer + 5, 4))
      c->controlData.buttons |= SCE_CTRL_SELECT;

    if (bit(c->buffer + 5, 0) && bit(c->buffer + 5, 1) && bit(c->buffer + 5, 5)) // L+R+START combo
    {
      c->controlData.buttons |= SCE_CTRL_PSBUTTON;
      c->controlData.buttons &= ~SCE_CTRL_START;
    }

    c->controlData.leftX  = c->buffer[0];
    c->controlData.leftY  = c->buffer[1];
    c->controlData.rightX = c->buffer[2];
    c->controlData.rightY = c->buffer[3];
    return 1;
}

void logitech_setRumble(Controller *c, uint8_t small, uint8_t large)
{
    if (small || large)
    {
      uint8_t rumblecmd[] __attribute__((aligned(64))) = {0x51, 0x00, small, 0x00, large, 0x00, 0x00};
      usb_write(c, rumblecmd, 7);
    } else {
      uint8_t rumblecmd[] __attribute__((aligned(64))) = {0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      usb_write(c, rumblecmd, 7);
    }
}


#endif
