#include "xone_controller.h"
#include "../devicelist.h"

#include "utils/bit.h"
#include "utils/rescale.h"

#include <psp2kern/ctrl.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/sysmem/heap.h>
#include <string.h>

#define USB_IF_NUMBER_DATA 0x0
#define USB_IF_PROTOCOL 0xd0
#define USB_IF_CLASS 0xFF
#define USB_IF_SUBCLASS 0x47

extern SceUID heap;

uint8_t XboxOneController_probe(Controller *c, int device_id, int port, int vendor, int product)
{
  c->type          = PAD_XONE;
  c->buffer_size   = 64;
  c->device_id     = device_id;
  c->port          = port;
  c->battery_level = 5;
  c->vendor        = vendor;
  c->product       = product;

#if defined(DEBUG)
  ksceDebugPrintf("scanning endpoints for device %d\n", device_id);
#endif

  // init endpoints and stuff

  SceUsbdInterfaceDescriptor *interface;

  interface = (SceUsbdInterfaceDescriptor *)ksceUsbdScanStaticDescriptor(device_id, 0, SCE_USBD_DESCRIPTOR_INTERFACE);
  while (interface)
  {
    if (   interface->bInterfaceProtocol == USB_IF_PROTOCOL
        && interface->bInterfaceNumber == USB_IF_NUMBER_DATA
        && interface->bInterfaceClass == USB_IF_CLASS
        && interface->bInterfaceSubclass == USB_IF_SUBCLASS )
    {
      break;
    }
    interface = (SceUsbdInterfaceDescriptor *)ksceUsbdScanStaticDescriptor(device_id, interface,
                                                                           SCE_USBD_DESCRIPTOR_INTERFACE);
  }

  SceUsbdEndpointDescriptor *endpoint;

  endpoint
      = (SceUsbdEndpointDescriptor *)ksceUsbdScanStaticDescriptor(device_id, interface, SCE_USBD_DESCRIPTOR_ENDPOINT);
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
    c->attached = 1;
    c->inited   = 1;
  }

  c->xone_seq_system = 0;
  c->xone_seq_vendor = 0;
  c->xone_type = GIP_TYPE_GAMEPAD;
  c->xone_metadata = NULL;
  c->xone_fragment_full_size = 0;
  c->xone_fragment_offset = 0;
  c->xone_guide_pressed = 0;

  usb_read(c);
  return 1;
}

void XboxOneController_setRumble(Controller *c, uint8_t small, uint8_t large)
{
    // i have no idea how it's supposed to work for wheels/sticks
    if (c->xone_type != GIP_TYPE_GAMEPAD) return;

    c->xone_seq_vendor++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_vendor == 0) c->xone_seq_vendor++;

    // motor values are 0 - 0x64, but we get 0 - 0xFF here
    small = (float)small / 2.55f;
    large = (float)large / 2.55f;

    uint8_t rumblecmd[] __attribute__((aligned(64))) = {
        GIP_MSG_VENDOR_RUMBLE, 0x00, c->xone_seq_vendor, 9,
        0x00, GIP_MOTOR_ALL, 0x00, 0x00, large, small, 0xFF, 0x00, 0x00
    };

    // TODO: maybe triggers too?
    /*
    uint8_t rumblecmd[] __attribute__((aligned(64))) = {
        GIP_MSG_RUMBLE, 0x00, c->xone_seq_vendor, 9,
        0x00, GIP_MOTOR_ALL, small, large, small, large, 0xFF, 0x00, 0x00
    };
    */

    usb_write(c, rumblecmd, 13);
}

void _xone_ack_packet(Controller *c, uint8_t seq, uint8_t ref_msg_type, uint8_t ref_msg_flags, uint32_t offset, uint16_t buffer_left)
{
    uint8_t ack_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_CONTROL, GIP_MSG_FLAG_SYSTEM, seq, 0x09,
        GIP_CTRL_ACK,
        ref_msg_type,
        ref_msg_flags & GIP_MSG_FLAG_SYSTEM,
        (offset >> 0)  & 0xFF,
        (offset >> 8)  & 0xFF,
        (offset >> 16) & 0xFF,
        (offset >> 24) & 0xFF,
        (buffer_left >> 0)  & 0xFF,
        (buffer_left >> 8) & 0xFF
    };

    usb_write(c, ack_packet, 13);
}

void _xone_start_packet(Controller *c)
{
    c->xone_seq_system++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_system == 0) c->xone_seq_system++;

    uint8_t ack_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_STATE, GIP_MSG_FLAG_SYSTEM, c->xone_seq_system, 1, GIP_STATE_START
    };

    usb_write(c, ack_packet, 5);
}

void _xone_led_packet(Controller *c, uint8_t intensity)
{
    c->xone_seq_system++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_system == 0) c->xone_seq_system++;


    uint8_t led_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_LED, GIP_MSG_FLAG_SYSTEM, c->xone_seq_system, 3, 0x00, GIP_LED_ON, intensity
    };

    usb_write(c, led_packet, 7);
}

void _xone_auth_complete(Controller *c)
{
    // security uses separate seq
    uint8_t auth_done_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_SECURITY, GIP_MSG_FLAG_SYSTEM, 0x01, 2, 0x01, 0x00 // AUTH_CTX_CTRL, AUTH_CTRL_COMPLETE
    };

    usb_write(c, auth_done_packet, 6);
}

void _xone_protocol_reset(Controller *c)
{
    c->xone_seq_system++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_system == 0) c->xone_seq_system++;

    uint8_t reset_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_STATE, GIP_MSG_FLAG_SYSTEM, c->xone_seq_system, 0x0f, GIP_STATE_RESERVED,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x55, 0x53, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };

    usb_write(c, reset_packet, 19);
}

void _xone_request_metadata(Controller *c)
{
    c->xone_seq_system++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_system == 0) c->xone_seq_system++;

    uint8_t reset_packet[] __attribute__((aligned(64))) = {
        GIP_MSG_METADATA, GIP_MSG_FLAG_SYSTEM, c->xone_seq_system, 0x00
    };

    usb_write(c, reset_packet, 4);
}

void _xone_request_wheel_config(Controller *c)
{
    c->xone_seq_vendor++;
    // if we overflowed - reset to 1 instead. docs say 0x00 is reserved
    if (c->xone_seq_vendor == 0) c->xone_seq_vendor++;

    uint8_t packet[] __attribute__((aligned(64))) = {
        GIP_MSG_VENDOR_WHEEL, GIP_MSG_FLAG_NONE, c->xone_seq_vendor, 0x03, GIP_VENDOR_WHEEL_INITIAL_STATE, 0x00, 0x00
    };

    usb_write(c, packet, 7);
}

void _xone_parse_metadata(Controller *c)
{
    if (c->xone_metadata)
    {
        // TODO: error/size checks
        // we only care about types for now, so jump to them
        uint16_t types_offset = (c->xone_metadata[16 + 0x0B] << 8) | c->xone_metadata[16 + 0x0A];
        // offset is counted from device metadata header start, so add 16 (main header size)
        types_offset += 16;

        uint8_t types_count = c->xone_metadata[types_offset];
        types_offset++;
        for (int i = 0; i < types_count; i++)
        {
            uint16_t type_size = (c->xone_metadata[types_offset+1] << 8) | (c->xone_metadata[types_offset]);
            types_offset+=2;

            char* type = ksceKernelAllocHeapMemory(heap, type_size + 1);
            if (type)
            {
                memcpy(type, c->xone_metadata + types_offset, type_size);
                types_offset += type_size;
                type[type_size] = 0;
#if defined(DEBUG)
                ksceKernelPrintf("Type: %s\n", type);
#endif
                if (strcmp(type, "Microsoft.Xbox.Input.Wheel") == 0)
                {
                    c->xone_type = GIP_TYPE_WHEEL;
                }
                else if (strcmp(type, "Windows.Xbox.Input.Wheel") == 0)
                {
                    c->xone_type = GIP_TYPE_WHEEL;
                }
                ksceKernelFreeHeapMemory(heap, type);
            }
        }


        ksceKernelFreeHeapMemory(heap, c->xone_metadata);
    }
}

void _xone_processReportGamepad(Controller *c, size_t length)
{
    // check msg size for extended data
    // share button is here: trigger ps+start
    c->controlData.buttons = 0;
    c->controlData.leftX   = 128;
    c->controlData.leftY   = 128;
    c->controlData.rightX  = 128;
    c->controlData.rightY  = 128;
    c->controlData.lt      = 0;
    c->controlData.rt      = 0;

    if (bit(c->buffer + 4, 7))
      c->controlData.buttons |= SCE_CTRL_TRIANGLE;
    if (bit(c->buffer + 4, 6))
      c->controlData.buttons |= SCE_CTRL_SQUARE;
    if (bit(c->buffer + 4, 5))
      c->controlData.buttons |= SCE_CTRL_CIRCLE;
    if (bit(c->buffer + 4, 4))
      c->controlData.buttons |= SCE_CTRL_CROSS;
    if (bit(c->buffer + 4, 3))
      c->controlData.buttons |= SCE_CTRL_SELECT;
    if (bit(c->buffer + 4, 2))
      c->controlData.buttons |= SCE_CTRL_START;

    if (bit(c->buffer + 5, 7))
      c->controlData.buttons |= SCE_CTRL_R3;
    if (bit(c->buffer + 5, 6))
      c->controlData.buttons |= SCE_CTRL_L3;
    if (bit(c->buffer + 5, 5))
      c->controlData.buttons |= SCE_CTRL_R1;
    if (bit(c->buffer + 5, 4))
      c->controlData.buttons |= SCE_CTRL_L1;
    if (bit(c->buffer + 5, 3))
      c->controlData.buttons |= SCE_CTRL_RIGHT;
    if (bit(c->buffer + 5, 2))
      c->controlData.buttons |= SCE_CTRL_LEFT;
    if (bit(c->buffer + 5, 1))
      c->controlData.buttons |= SCE_CTRL_DOWN;
    if (bit(c->buffer + 5, 0))
      c->controlData.buttons |= SCE_CTRL_UP;

    int16_t lt = (c->buffer[7] << 8) | c->buffer[6];
    int16_t rt = (c->buffer[9] << 8) | c->buffer[8];

    int16_t lx = (c->buffer[11] << 8) | c->buffer[10];
    int16_t ly = (c->buffer[13] << 8) | c->buffer[12];
    int16_t rx = (c->buffer[15] << 8) | c->buffer[14];
    int16_t ry = (c->buffer[17] << 8) | c->buffer[16];


    c->controlData.leftX  = rescale(lx, 16, 8) + 128;
    c->controlData.leftY  = rescale(ly, 16, 8) + 128;
    c->controlData.rightX = rescale(rx, 16, 8) + 128;
    c->controlData.rightY = rescale(ry, 16, 8) + 128;

    // up and down are reversed
    c->controlData.leftY  = 255 - c->controlData.leftY;
    c->controlData.rightY = 255 - c->controlData.rightY;

    c->controlData.lt = rescale(lt, 10, 8);
    c->controlData.rt = rescale(rt, 10, 8);

    if (c->controlData.lt > 0)
      c->controlData.buttons |= SCE_CTRL_LTRIGGER;
    if (c->controlData.rt > 0)
      c->controlData.buttons |= SCE_CTRL_RTRIGGER;

    if (c->xone_guide_pressed)
        c->controlData.buttons |= SCE_CTRL_PSBUTTON;
    else
        c->controlData.buttons &= (~SCE_CTRL_PSBUTTON);

    if (c->buffer[3] > 0xE) // share button
    {
        if (c->buffer[22])
        {
            // screenshot (todo: maybe just call screenshot func instead?)
            c->controlData.buttons |= SCE_CTRL_PSBUTTON;
            c->controlData.buttons |= SCE_CTRL_START;
        }
    }
}

void _xone_processReportWheel(Controller *c, size_t length)
{
    //scale conversion: value = value / (1 << (wheel bits - 8))

    // check msg size for extended data
    // share button is here: trigger ps+start
    c->controlData.buttons = 0;
    c->controlData.leftX   = 128;
    c->controlData.leftY   = 128;
    c->controlData.rightX  = 128;
    c->controlData.rightY  = 128;
    c->controlData.lt      = 0;
    c->controlData.rt      = 0;

    if (bit(c->buffer + 4, 7))
      c->controlData.buttons |= SCE_CTRL_TRIANGLE;
    if (bit(c->buffer + 4, 6))
      c->controlData.buttons |= SCE_CTRL_SQUARE;
    if (bit(c->buffer + 4, 5))
      c->controlData.buttons |= SCE_CTRL_CIRCLE;
    if (bit(c->buffer + 4, 4))
      c->controlData.buttons |= SCE_CTRL_CROSS;
    if (bit(c->buffer + 4, 3))
      c->controlData.buttons |= SCE_CTRL_SELECT;
    if (bit(c->buffer + 4, 2))
      c->controlData.buttons |= SCE_CTRL_START;

    if (bit(c->buffer + 5, 5))
      c->controlData.buttons |= SCE_CTRL_R1;
    if (bit(c->buffer + 5, 4))
      c->controlData.buttons |= SCE_CTRL_L1;
    if (bit(c->buffer + 5, 3))
      c->controlData.buttons |= SCE_CTRL_RIGHT;
    if (bit(c->buffer + 5, 2))
      c->controlData.buttons |= SCE_CTRL_LEFT;
    if (bit(c->buffer + 5, 1))
      c->controlData.buttons |= SCE_CTRL_DOWN;
    if (bit(c->buffer + 5, 0))
      c->controlData.buttons |= SCE_CTRL_UP;

    int16_t wheel = (c->buffer[7] << 8) | c->buffer[6];

    int16_t throttle = (c->buffer[9] << 8) | c->buffer[8];

    int16_t brake = (c->buffer[11] << 8) | c->buffer[10];

    int16_t clutch = (c->buffer[13] << 8) | c->buffer[12];

    c->controlData.leftX  = rescale(wheel, c->xone_wheel_config.angle_bits, 8);

    if (brake > 10)
        c->controlData.rightY = 128 + rescale(brake, c->xone_wheel_config.brake_bits, 7);
    else
        c->controlData.rightY = 128 - rescale(throttle, c->xone_wheel_config.throttle_bits, 7);

    if (c->xone_guide_pressed)
        c->controlData.buttons |= SCE_CTRL_PSBUTTON;
    else
        c->controlData.buttons &= (~SCE_CTRL_PSBUTTON);
}


uint8_t XboxOneController_processReport(Controller *c, size_t length)
{
  if (c->buffer[1] & GIP_MSG_FLAG_SYSTEM) // system messages
  {
      switch(c->buffer[0])
      {
        case GIP_MSG_HELLO:
            // ask for metadata to detect wheels
            _xone_request_metadata(c);

            if (c->vendor == 0x045e)
            {
                if (c->product == 0x02ea || c->product == 0x0b00)
                {
                    // reset bluetooth-paired xone s protocol
                    _xone_protocol_reset(c);
                }
            }

            // some PowerA pads require rumble cycle
            if (c->vendor == 0x24c6)
            {
                if (c->product == 0x541a || c->product == 0x542a || c->product == 0x543a)
                {
                    XboxOneController_setRumble(c, 0x1D, 0x1D);
                    XboxOneController_setRumble(c, 0, 0);
                }
            }

            break;
        case GIP_MSG_STATUS:
            // TODO: if we have charge pack - report level
            break;
        case GIP_MSG_GUIDE:
            // TODO: do ps button
            // since we do main button processing elsewhere we need to:
            // 1. remember current guide state so we can update it in input report handler
            c->xone_guide_pressed = c->buffer[4];
            // 2. add/remove ps button state from buttons
            if (c->xone_guide_pressed)
                c->controlData.buttons |= SCE_CTRL_PSBUTTON;
            else
                c->controlData.buttons &= (~SCE_CTRL_PSBUTTON);
            break;
        case GIP_MSG_METADATA:
            // handled below
            break;
        default:
            break;
      }
  }
  else // vendor messages
  {
      switch(c->buffer[0])
      {
        case GIP_MSG_VENDOR_INPUT:
            // parse accordingly if wheel or gamepad
            if (c->xone_type == GIP_TYPE_WHEEL)
                _xone_processReportWheel(c, length);
            else
                _xone_processReportGamepad(c, length);
            break;
        case GIP_MSG_VENDOR_STATIC_CONFIG:
            // update wheel config
            if ((length >= 0x04) && (c->buffer[0x03] >= 0x0B))
            {
                memcpy(&c->xone_wheel_config, c->buffer + 0x04, sizeof(XoneWheelConfig));
#if defined(DEBUG)
                ksceKernelPrintf("Angle bits: %d\n", c->xone_wheel_config.angle_bits);
                ksceKernelPrintf("Throttle bits: %d\n", c->xone_wheel_config.throttle_bits);
                ksceKernelPrintf("Brake bits: %d\n", c->xone_wheel_config.brake_bits);
                ksceKernelPrintf("Clutch bits: %d\n", c->xone_wheel_config.clutch_bits);
                ksceKernelPrintf("Handbrake bits: %d\n", c->xone_wheel_config.handbrake_bits);
                ksceKernelPrintf("Min angle: %d\n", c->xone_wheel_config.min_angle);
                ksceKernelPrintf("Max angle: %d\n", c->xone_wheel_config.max_angle);
                ksceKernelPrintf("FF mask: %02X\n", c->xone_wheel_config.ff_mask);
                ksceKernelPrintf("Caps: %02X\n", c->xone_wheel_config.caps);
#endif
            }
            else
            {
                ksceKernelPrintf("Bad wheel static report!\n");
            }
            break;
        default:
            break;
      }
  }

  if (c->buffer[1] & GIP_MSG_FLAG_INITFRAG)
  {
    // first fragmented packet. remember full size, reset and update "received" size
    // TODO: technically, we should do that per msg+flags+seq combo
    if (c->buffer[3] & 0x80) // payload size is 2 bytes, total is 1
    {
        c->xone_fragment_offset = (c->buffer[4] * 128) + (c->buffer[3] & 0x7F);
        c->xone_fragment_full_size = c->buffer[5] & 0x7F;
    }
    else // payload size is 1 byte, total is 2
    {
        c->xone_fragment_offset = c->buffer[3] & 0x7F;
        c->xone_fragment_full_size = (c->buffer[5] * 128) + (c->buffer[4] & 0x7F);
    }

    if (c->buffer[0] == GIP_MSG_METADATA)
    {
        c->xone_metadata = ksceKernelAllocHeapMemory(heap, c->xone_fragment_full_size);
        // write data
        if (c->xone_metadata)
            memcpy(c->xone_metadata, c->buffer + 6, c->xone_fragment_offset);
    }
  }
  else if (c->buffer[1] & GIP_MSG_FLAG_FRAG) // middle/last packet
  {
    // middle/last fragmented packet. update "received" size
    // TODO: technically, we should do that per msg+flags+seq combo
    int current_offset;
    int fragment_size;
    if (c->buffer[3] & 0x80) // payload size is 2 bytes, total is 1
    {
        fragment_size = (c->buffer[4] & 0x7F) * 128 + c->buffer[3] & 0x7F;
        current_offset =  c->buffer[5] & 0x7F;
    }
    else
    {
        fragment_size = c->buffer[3] & 0x7F;
        current_offset = (c->buffer[5] & 0x7F) * 128 + c->buffer[4] & 0x7F;
    }

    if (c->buffer[0] == GIP_MSG_METADATA)
    {
        // end of metadata. parse
        if (c->buffer[3] == 0)
        {
            // parse metadata
            _xone_parse_metadata(c);

            // if wheel - ask config, then start
            if (c->xone_type == GIP_TYPE_WHEEL)
            {
                _xone_request_wheel_config(c);
            }

            _xone_start_packet(c);

            // The host always sends the LED Guide Button Command after START
            // even if the LED Guide Button message type is not specifically declared in the device’s metadata
            // 0 - 47%
            _xone_led_packet(c, 0x14);

            // tell device that it's authorised
            _xone_auth_complete(c);
        }
        else
        {
            if (c->xone_metadata)
                memcpy(c->xone_metadata + c->xone_fragment_offset, c->buffer + 6, fragment_size);
        }
    }
    c->xone_fragment_offset += fragment_size;

  }

  if (c->buffer[1] & GIP_MSG_FLAG_ACME)
  {
    // ack
    _xone_ack_packet(c, c->buffer[2], c->buffer[0],  c->buffer[1], c->xone_fragment_offset, c->xone_fragment_full_size - c->xone_fragment_offset);
  }

  return 1;
}
