#ifndef __DINPUT_LOGITECH_WHEEL_H__
#define __DINPUT_LOGITECH_WHEEL_H__
#include <psp2kern/ctrl.h>
#include "dpad_angle.h"
#include "../utils/rescale.h"

uint8_t logitech_wheel_processReport(Controller *c, size_t length)
{

    process_dpad_angle(c, 4);




/*
    if (brake > 10)
        c->controlData.rightY = 128 + rescale(brake, c->xone_wheel_config.brake_bits, 7);
    else
        c->controlData.rightY = 128 - rescale(throttle, c->xone_wheel_config.throttle_bits, 7);
*/

    if (bit(c->buffer + 1, 2))
      c->controlData.buttons |= SCE_CTRL_CROSS;
    if (bit(c->buffer + 1, 4))
      c->controlData.buttons |= SCE_CTRL_CIRCLE;
    if (bit(c->buffer + 1, 5))
      c->controlData.buttons |= SCE_CTRL_TRIANGLE;
    if (bit(c->buffer + 1, 3))
      c->controlData.buttons |= SCE_CTRL_SQUARE;

    if (bit(c->buffer + 1, 7))
      c->controlData.buttons |= SCE_CTRL_L1;
    if (bit(c->buffer + 1, 6))
      c->controlData.buttons |= SCE_CTRL_R1;

    if (bit(c->buffer + 2, 5))
      c->controlData.buttons |= SCE_CTRL_L3;
    if (bit(c->buffer + 2, 4))
      c->controlData.buttons |= SCE_CTRL_R3;

    if (bit(c->buffer + 2, 1))
    {
      c->controlData.buttons |= SCE_CTRL_LTRIGGER;
      c->controlData.lt = 0xFF;
    }

    if (bit(c->buffer + 2, 0))
    {
      c->controlData.buttons |= SCE_CTRL_RTRIGGER;
      c->controlData.rt = 0xFF;
    }

    if (bit(c->buffer + 2, 3))
      c->controlData.buttons |= SCE_CTRL_START;
    if (bit(c->buffer + 2, 2))
      c->controlData.buttons |= SCE_CTRL_SELECT;

    if (bit(c->buffer + 1, 7) && bit(c->buffer + 1, 6) && bit(c->buffer + 2, 3)) // L+R+START combo
    {
      c->controlData.buttons |= SCE_CTRL_PSBUTTON;
      c->controlData.buttons &= ~SCE_CTRL_START;
    }

    uint16_t wheel = ((c->buffer[1] & 0x3) << 8) | c->buffer[0];
    c->controlData.leftX  = rescale(wheel, 10, 8);

    // one axis
    // but they work simultaneously, so both pressed = neutral
//    c->controlData.rightY  = c->buffer[3];

    // separate axes
    uint8_t throttle = 255 - c->buffer[5];
    uint8_t brake = 255 - c->buffer[6];

    if (brake > 10)
        c->controlData.rightY = 128 + rescale(brake, 8, 7);
    else
        c->controlData.rightY = 128 - rescale(throttle, 8, 7);


    return 1;
}

#endif
