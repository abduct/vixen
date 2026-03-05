#include "gc_controller.h"
#include "../devicelist.h"

#include "utils/bit.h"

#include <psp2kern/ctrl.h>
#include <psp2kern/kernel/debug.h>
#include "utils/scale_range.h"

extern Controller controllers[4];

uint8_t GCController_probe(Controller *c, int device_id, int port)
{
  c->type          = PAD_GC;
  c->buffer_size   = 37;
  c->device_id     = device_id;
  c->port          = port;
  c->battery_level = 5;

#if defined(DEBUG)
  ksceDebugPrintf("scanning endpoints for device %d\n", device_id);
#endif

  SceUsbdEndpointDescriptor *endpoint;

  endpoint
      = (SceUsbdEndpointDescriptor *)ksceUsbdScanStaticDescriptor(device_id, NULL, SCE_USBD_DESCRIPTOR_ENDPOINT);
  while (endpoint)
  {
#if defined(DEBUG)
    ksceDebugPrintf("got EP: %02x\n", endpoint->bEndpointAddress);
#endif
    if ((endpoint->bEndpointAddress & SCE_USBD_ENDPOINT_DIRECTION_BITS) == SCE_USBD_ENDPOINT_DIRECTION_IN)
    {
#if defined(DEBUG)
      ksceDebugPrintf("opening in pipe\n");
#endif
      c->pipe_in = ksceUsbdOpenPipe(device_id, endpoint);
#if defined(DEBUG)
      ksceDebugPrintf("= 0x%08x\n", c->pipe_in);
      ksceDebugPrintf("bmAttributes = %x\n", endpoint->bmAttributes);
#endif
    }
    else if ((endpoint->bEndpointAddress & SCE_USBD_ENDPOINT_DIRECTION_BITS) == SCE_USBD_ENDPOINT_DIRECTION_OUT)
    {
#if defined(DEBUG)
      ksceDebugPrintf("opening out pipe\n");
#endif
      c->pipe_out = ksceUsbdOpenPipe(device_id, endpoint);
#if defined(DEBUG)
      ksceDebugPrintf("= 0x%08x\n", c->pipe_out);
      ksceDebugPrintf("bmAttributes = %x\n", endpoint->bmAttributes);
#endif
    }

    if (c->pipe_in > 0 && c->pipe_out > 0)
      break;

    endpoint
        = (SceUsbdEndpointDescriptor *)ksceUsbdScanStaticDescriptor(device_id, endpoint, SCE_USBD_DESCRIPTOR_ENDPOINT);
  }

  if (c->pipe_in > 0 && c->pipe_out > 0)
  {
    SceUsbdConfigurationDescriptor *cdesc;
    if ((cdesc = (SceUsbdConfigurationDescriptor *)ksceUsbdScanStaticDescriptor(device_id, NULL,
                                                                                SCE_USBD_DESCRIPTOR_CONFIGURATION))
        == NULL)
      return 0;

    SceUID control_pipe_id = ksceUsbdOpenPipe(device_id, NULL);
    // set default config
    int r = ksceUsbdSetConfiguration(control_pipe_id, cdesc->bConfigurationValue, NULL, NULL);
#if defined(DEBUG)
    ksceDebugPrintf("ksceUsbdSetConfiguration = 0x%08x\n", r);
#endif
    if (r < 0)
      return 0;
    c->attached = 0;
    c->inited   = 1;
  }

  // required for original adapter
  uint8_t initcmd[] __attribute__((aligned(64))) = {0x13};
  usb_write(c, initcmd, 1);

  usb_read(c);
  return 1;
}

void GCController_setRumble(Controller *c, uint8_t small, uint8_t large)
{
  uint8_t rumblecmd[] __attribute__((aligned(64))) = {0x11, 0x00, 0x00, 0x00, 0x00};
  uint8_t val = ((small < large) ? large : small);
  rumblecmd[c->port + 1] = val;
  usb_write(c, rumblecmd, 5);
}

uint8_t GCController_processReport(Controller *c, size_t length)
{
  // we have one report for all controllers
  // report consists of 9 bytes per controller * 4, plus header byte 
  // this function receives only controller in port 0
  // example with two controllers attached
  // 21 14 00 00 7F 7F 7F 7F 00 00 14 00 00 7C 7C 7D  80 1F 1F 04 00 00 00 00 00 00 00 00 04 00 00 00  00 00 00 00 00

  for (int i = 0; i < 4; i++)
  {
    uint8_t *data = c->buffer + (i * 9 + 1);

    if (data[0] & 0x10 || data[0] & 0x20)
    {
#if defined(DEBUG)
      ksceKernelPrintf("attaching and processing port %d\n", i);
#endif
      controllers[i].attached = 1;
      // reset everything
      controllers[i].controlData.buttons = 0;
      controllers[i].controlData.leftX   = 128;
      controllers[i].controlData.leftY   = 128;
      controllers[i].controlData.rightX  = 128;
      controllers[i].controlData.rightY  = 128;
      controllers[i].controlData.lt      = 0;
      controllers[i].controlData.rt      = 0;


      // set buttons
      if (bit(data + 1, 4))
          controllers[i].controlData.buttons |= SCE_CTRL_LEFT;
      if (bit(data + 1, 5))
          controllers[i].controlData.buttons |= SCE_CTRL_RIGHT;
      if (bit(data + 1, 6))
          controllers[i].controlData.buttons |= SCE_CTRL_DOWN;
      if (bit(data + 1, 7))
          controllers[i].controlData.buttons |= SCE_CTRL_UP;

      if (bit(data + 1, 0))
          controllers[i].controlData.buttons |= SCE_CTRL_CROSS;
      if (bit(data + 1, 1))
          controllers[i].controlData.buttons |= SCE_CTRL_CIRCLE;
      if (bit(data + 1, 2))
          controllers[i].controlData.buttons |= SCE_CTRL_TRIANGLE;
      if (bit(data + 1, 3))
          controllers[i].controlData.buttons |= SCE_CTRL_SQUARE;

      if (bit(data + 2, 0))
          controllers[i].controlData.buttons |= SCE_CTRL_START;

      if (bit(data + 2, 1))
          controllers[i].controlData.buttons |= SCE_CTRL_SELECT;

      if (scale_range(data[7], 0x20, 0xE0) > 0x10)
          controllers[i].controlData.buttons |= SCE_CTRL_L1;
      if (scale_range(data[8], 0x20, 0xE0) > 0x10)
          controllers[i].controlData.buttons |= SCE_CTRL_R1;


      controllers[i].controlData.leftX = scale_range(data[3], 0x20, 0xE0);
      controllers[i].controlData.leftY = scale_range(data[4], 0x20, 0xE0);

      controllers[i].controlData.rightX = scale_range(data[5], 0x20, 0xE0);
      controllers[i].controlData.rightY = scale_range(data[6], 0x20, 0xE0);

      // up and down are reversed
      c->controlData.leftY  = 255 - c->controlData.leftY;
      c->controlData.rightY = 255 - c->controlData.rightY;

      if (
        (controllers[i].controlData.buttons & SCE_CTRL_L1)
        && (controllers[i].controlData.buttons & SCE_CTRL_R1)
        && bit(data + 2, 0)
      ) // L+R+START combo
      {
        c->controlData.buttons |= SCE_CTRL_PSBUTTON;
        c->controlData.buttons &= ~SCE_CTRL_START;
      }
    }
    else
        controllers[i].attached = 0;
  }
  return 1;
}
