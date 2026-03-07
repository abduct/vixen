#include "devicelist.h"

#include "controllers/utils/bit.h"
#include "controllers/dinput/logitech.h"
#include "controllers/dinput/logitech_wingman.h"
#include "controllers/dinput/logitech_wheel.h"
#include "controllers/dinput/psclassic.h"
#include "controllers/dinput/horidiva.h"
#include "controllers/dinput/horidiva_ps4.h"
#include "controllers/dinput/raphnetpsx.h"
#include "controllers/dinput/p3converter.h"
#include "controllers/dinput/sfcconverter.h"
#include "controllers/dinput/p2top3converter.h"
#include "controllers/dinput/smartjoypad.h"
#include "controllers/dinput/mayflash.h"
#include "controllers/dinput/neogeox.h"
#include "controllers/dinput/8bitdoadapter.h"

gamepad_t _devices[] = {{PAD_XBOX360, 0x0079, 0x18d4, NULL, NULL},  // GPD Win 2 X-Box Controller
                        {PAD_XBOX360, 0x03eb, 0xff01, NULL, NULL},  // Wooting One (Legacy)
                        {PAD_XBOX360, 0x03eb, 0xff02, NULL, NULL},  // Wooting Two (Legacy)
                        {PAD_XBOX360, 0x044f, 0xb326, NULL, NULL},  // Thrustmaster Gamepad GP XID
                        {PAD_XBOX360, 0x045e, 0x028e, NULL, NULL},  // Microsoft X-Box 360 pad
                        {PAD_XBOX360, 0x045e, 0x028f, NULL, NULL},  // Microsoft X-Box 360 pad v2
                        {PAD_XBOX360, 0x045e, 0x02a1, NULL, NULL},  // Open-Frame1
                        {PAD_XBOX360, 0x046d, 0xc21d, NULL, NULL},  // Logitech Gamepad F310
                        {PAD_XBOX360, 0x046d, 0xc21e, NULL, NULL},  // Logitech Gamepad F510
                        {PAD_XBOX360, 0x046d, 0xc21f, NULL, NULL},  // Logitech Gamepad F710
                        {PAD_XBOX360, 0x046d, 0xc242, NULL, NULL},  // Logitech Chillstream Controller
                        {PAD_XBOX360, 0x046d, 0xcaa3, NULL, NULL},  // Logitech DriveFx Racing Wheel
                        {PAD_XBOX360, 0x056e, 0x2004, NULL, NULL},  // Elecom JC-U3613M
                        {PAD_XBOX360, 0x06a3, 0xf51a, NULL, NULL},  // Saitek P3600
                        {PAD_XBOX360, 0x0738, 0x4716, NULL, NULL},  // Mad Catz Wired Xbox 360 Controller
                        {PAD_XBOX360, 0x0738, 0x4718, NULL, NULL},  // Mad Catz Street Fighter IV FightStick SE
                        {PAD_XBOX360, 0x0738, 0x4726, NULL, NULL},  // Mad Catz Xbox 360 Controller
                        {PAD_XBOX360, 0x0738, 0x4728, NULL, NULL},  // Mad Catz Street Fighter IV FightPad
                        {PAD_XBOX360, 0x0738, 0x4736, NULL, NULL},  // Mad Catz MicroCon Gamepad
                        {PAD_XBOX360, 0x0738, 0x4738, NULL, NULL},  // Mad Catz Wired Xbox 360 Controller (SFIV)
                        {PAD_XBOX360, 0x0738, 0x4740, NULL, NULL},  // Mad Catz Beat Pad
                        {PAD_XBOX360, 0x0738, 0x4758, NULL, NULL},  // Mad Catz Arcade Game Stick
                        {PAD_XBOX360, 0x0738, 0x9871, NULL, NULL},  // Mad Catz Portable Drum
                        {PAD_XBOX360, 0x0738, 0xcb02, NULL, NULL},  // Saitek Cyborg Rumble Pad - PC/Xbox 360
                        {PAD_XBOX360, 0x0738, 0xcb03, NULL, NULL},  // Saitek P3200 Rumble Pad - PC/Xbox 360
                        {PAD_XBOX360, 0x0738, 0xb726, NULL, NULL},  // Mad Catz Xbox controller - MW2
                        {PAD_XBOX360, 0x0738, 0xf738, NULL, NULL},  // Super SFIV FightStick TE S
                        {PAD_XBOX360, 0x0738, 0xb738, NULL, NULL},  // Mad Catz MVC2TE Stick 2
                        {PAD_XBOX360, 0x0738, 0xbeef, NULL, NULL},  // Mad Catz JOYTECH NEO SE Advanced GamePad
                        {PAD_XBOX360, 0x0738, 0xcb29, NULL, NULL},  // Saitek Aviator Stick AV8R02
                        {PAD_XBOX360, 0x07ff, 0xffff, NULL, NULL},  // Mad Catz GamePad
                        {PAD_XBOX360, 0x0e6f, 0x0105, NULL, NULL},  // HSM3 Xbox360 dancepad
                        {PAD_XBOX360, 0x0e6f, 0x011f, NULL, NULL},  // Rock Candy Gamepad Wired Controller
                        {PAD_XBOX360, 0x0e6f, 0x0113, NULL, NULL},  // Afterglow AX.1 Gamepad for Xbox 360
                        {PAD_XBOX360, 0x0e6f, 0x0201, NULL, NULL},  // Pelican PL-3601 'TSZ' Wired Xbox 360 Controller
                        {PAD_XBOX360, 0x0e6f, 0x0213, NULL, NULL},  // Afterglow Gamepad for Xbox 360
                        {PAD_XBOX360, 0x0e6f, 0x021f, NULL, NULL},  // Rock Candy Gamepad for Xbox 360
                        {PAD_XBOX360, 0x0e6f, 0x0301, NULL, NULL},  // Logic3 Controller
                        {PAD_XBOX360, 0x0e6f, 0x0131, NULL, NULL},  // PDP EA Sports Controller
                        {PAD_XBOX360, 0x0e6f, 0x0133, NULL, NULL},  // Xbox 360 Wired Controller
                        {PAD_XBOX360, 0x0e6f, 0x0401, NULL, NULL},  // Logic3 Controller
                        {PAD_XBOX360, 0x0e6f, 0x0413, NULL, NULL},  // Afterglow AX.1 Gamepad for Xbox 360
                        {PAD_XBOX360, 0x0e6f, 0x0501, NULL, NULL},  // PDP Xbox 360 Controller
                        {PAD_XBOX360, 0x0e6f, 0xf900, NULL, NULL},  // PDP Afterglow AX.1
                        {PAD_XBOX360, 0x0e6f, 0x0413, NULL, NULL},  // Afterglow AX.1 Gamepad for Xbox 360
                        {PAD_XBOX360, 0x0f0d, 0x000a, NULL, NULL},  // Hori Co. DOA4 FightStick
                        {PAD_XBOX360, 0x0f0d, 0x000c, NULL, NULL},  // Hori PadEX Turbo
                        {PAD_XBOX360, 0x0f0d, 0x000d, NULL, NULL},  // Hori Fighting Stick EX2
                        {PAD_XBOX360, 0x0f0d, 0x0016, NULL, NULL},  // Hori Real Arcade Pro.EX
                        {PAD_XBOX360, 0x0f0d, 0x001b, NULL, NULL},  // Hori Real Arcade Pro VX
                        {PAD_XBOX360, 0x1038, 0x1430, NULL, NULL},  // SteelSeries Stratus Duo
                        {PAD_XBOX360, 0x1038, 0x1431, NULL, NULL},  // SteelSeries Stratus Duo
                        {PAD_XBOX360, 0x11c9, 0x55f0, NULL, NULL},  // Nacon GC-100XF
                        {PAD_XBOX360, 0x1209, 0x2882, NULL, NULL},  // Ardwiino Controller
                        {PAD_XBOX360, 0x12ab, 0x0004, NULL, NULL},  // Honey Bee Xbox360 dancepad
                        {PAD_XBOX360, 0x12ab, 0x0301, NULL, NULL},  // PDP AFTERGLOW AX.1
                        {PAD_XBOX360, 0x12ab, 0x0303, NULL, NULL},  // Mortal Kombat Klassic FightStick
                        {PAD_XBOX360, 0x1430, 0x4748, NULL, NULL},  // RedOctane Guitar Hero X-plorer
                        {PAD_XBOX360, 0x1430, 0xf801, NULL, NULL},  // RedOctane Controller
                        {PAD_XBOX360, 0x146b, 0x0601, NULL, NULL},  // BigBen Interactive XBOX 360 Controller
                        {PAD_XBOX360, 0x146b, 0x0604, NULL, NULL},  // Bigben Interactive DAIJA Arcade Stick
                        {PAD_XBOX360, 0x1532, 0x0037, NULL, NULL},  // Razer Sabertooth
                        {PAD_XBOX360, 0x15e4, 0x3f00, NULL, NULL},  // Power A Mini Pro Elite
                        {PAD_XBOX360, 0x15e4, 0x3f10, NULL, NULL},  // Batarang Xbox 360 controller
                        {PAD_XBOX360, 0x15e4, 0x3f0a, NULL, NULL},  // Xbox Airflo wired controller
                        {PAD_XBOX360, 0x1689, 0xfd00, NULL, NULL},  // Razer Onza Tournament Edition
                        {PAD_XBOX360, 0x1689, 0xfd01, NULL, NULL},  // Razer Onza Classic Edition
                        {PAD_XBOX360, 0x162e, 0xbeef, NULL, NULL},  // Joytech Neo-Se Take2
                        {PAD_XBOX360, 0x1689, 0xfe00, NULL, NULL},  // Razer Sabertooth
                        {PAD_XBOX360, 0x1949, 0x041a, NULL, NULL},  // Amazon Game Controller
                        {PAD_XBOX360, 0x1bad, 0x0002, NULL, NULL},  // Harmonix Guitar for Xbox 360
                        {PAD_XBOX360, 0x1bad, 0x0003, NULL, NULL},  // Harmonix Drum Kit for Xbox 360
                        {PAD_XBOX360, 0x1bad, 0x0130, NULL, NULL},  // Ion Drum Rocker
                        {PAD_XBOX360, 0x1bad, 0xf016, NULL, NULL},  // Mad Catz Xbox 360 Controller
                        {PAD_XBOX360, 0x1bad, 0xf018, NULL, NULL},  // Mad Catz Street Fighter IV SE Fighting Stick
                        {PAD_XBOX360, 0x1bad, 0xf019, NULL, NULL},  // Mad Catz Brawlstick for Xbox 360
                        {PAD_XBOX360, 0x1bad, 0xf021, NULL, NULL},  // Mad Cats Ghost Recon FS GamePad
                        {PAD_XBOX360, 0x1bad, 0xf023, NULL, NULL},  // MLG Pro Circuit Controller (Xbox)
                        {PAD_XBOX360, 0x1bad, 0xf025, NULL, NULL},  // Mad Catz Call Of Duty
                        {PAD_XBOX360, 0x1bad, 0xf027, NULL, NULL},  // Mad Catz FPS Pro
                        {PAD_XBOX360, 0x1bad, 0xf028, NULL, NULL},  // Street Fighter IV FightPad
                        {PAD_XBOX360, 0x1bad, 0xf02e, NULL, NULL},  // Mad Catz Fightpad
                        {PAD_XBOX360, 0x1bad, 0xf030, NULL, NULL},  // Mad Catz Xbox 360 MC2 MicroCon Racing Wheel
                        {PAD_XBOX360, 0x1bad, 0xf036, NULL, NULL},  // Mad Catz MicroCon GamePad Pro
                        {PAD_XBOX360, 0x1bad, 0xf038, NULL, NULL},  // Street Fighter IV FightStick TE
                        {PAD_XBOX360, 0x1bad, 0xf039, NULL, NULL},  // Mad Catz MvC2 TE
                        {PAD_XBOX360, 0x1bad, 0xf03a, NULL, NULL},  // Mad Catz SFxT Fightstick Pro
                        {PAD_XBOX360, 0x1bad, 0xf03d, NULL, NULL},  // Street Fighter IV Arcade Stick TE - Chun Li
                        {PAD_XBOX360, 0x1bad, 0xf03e, NULL, NULL},  // Mad Catz MLG FightStick TE
                        {PAD_XBOX360, 0x1bad, 0xf03f, NULL, NULL},  // Mad Catz FightStick SoulCaliber
                        {PAD_XBOX360, 0x1bad, 0xf042, NULL, NULL},  // Mad Catz FightStick TES+
                        {PAD_XBOX360, 0x1bad, 0xf080, NULL, NULL},  // Mad Catz FightStick TE2
                        {PAD_XBOX360, 0x1bad, 0xf501, NULL, NULL},  // HoriPad EX2 Turbo
                        {PAD_XBOX360, 0x1bad, 0xf502, NULL, NULL},  // Hori Real Arcade Pro.VX SA
                        {PAD_XBOX360, 0x1bad, 0xf503, NULL, NULL},  // Hori Fighting Stick VX
                        {PAD_XBOX360, 0x1bad, 0xf504, NULL, NULL},  // Hori Real Arcade Pro. EX
                        {PAD_XBOX360, 0x1bad, 0xf505, NULL, NULL},  // Hori Fighting Stick EX2B
                        {PAD_XBOX360, 0x1bad, 0xf506, NULL, NULL},  // Hori Real Arcade Pro.EX Premium VLX
                        {PAD_XBOX360, 0x1bad, 0xf900, NULL, NULL},  // Harmonix Xbox 360 Controller
                        {PAD_XBOX360, 0x1bad, 0xf901, NULL, NULL},  // Gamestop Xbox 360 Controller
                        {PAD_XBOX360, 0x1bad, 0xf903, NULL, NULL},  // Tron Xbox 360 controller
                        {PAD_XBOX360, 0x1bad, 0xf904, NULL, NULL},  // PDP Versus Fighting Pad
                        {PAD_XBOX360, 0x1bad, 0xf906, NULL, NULL},  // MortalKombat FightStick
                        {PAD_XBOX360, 0x1bad, 0xfa01, NULL, NULL},  // MadCatz GamePad
                        {PAD_XBOX360, 0x1bad, 0xfd00, NULL, NULL},  // Razer Onza TE
                        {PAD_XBOX360, 0x1bad, 0xfd01, NULL, NULL},  // Razer Onza
                        {PAD_XBOX360, 0x20d6, 0x281f, NULL, NULL},  // PowerA Wired Controller For Xbox 360
                        {PAD_XBOX360, 0x24c6, 0x5000, NULL, NULL},  // Razer Atrox Arcade Stick
                        {PAD_XBOX360, 0x24c6, 0x5300, NULL, NULL},  // PowerA MINI PROEX Controller
                        {PAD_XBOX360, 0x24c6, 0x5303, NULL, NULL},  // Xbox Airflo wired controller
                        {PAD_XBOX360, 0x24c6, 0x530a, NULL, NULL},  // Xbox 360 Pro EX Controller
                        {PAD_XBOX360, 0x24c6, 0x531a, NULL, NULL},  // PowerA Pro Ex
                        {PAD_XBOX360, 0x24c6, 0x5397, NULL, NULL},  // FUS1ON Tournament Controller
                        {PAD_XBOX360, 0x24c6, 0x5500, NULL, NULL},  // Hori XBOX 360 EX 2 with Turbo
                        {PAD_XBOX360, 0x24c6, 0x5501, NULL, NULL},  // Hori Real Arcade Pro VX-SA
                        {PAD_XBOX360, 0x24c6, 0x5502, NULL, NULL},  // Hori Fighting Stick VX Alt
                        {PAD_XBOX360, 0x24c6, 0x5503, NULL, NULL},  // Hori Fighting Edge
                        {PAD_XBOX360, 0x24c6, 0x5506, NULL, NULL},  // Hori SOULCALIBUR V Stick
                        {PAD_XBOX360, 0x24c6, 0x550d, NULL, NULL},  // Hori GEM Xbox controller
                        {PAD_XBOX360, 0x24c6, 0x550e, NULL, NULL},  // Hori Real Arcade Pro V Kai 360
                        {PAD_XBOX360, 0x24c6, 0x5510, NULL, NULL},  // Hori Fighting Commander ONE (Xbox 360/PC Mode)
                        {PAD_XBOX360, 0x24c6, 0x5b00, NULL, NULL},  // ThrustMaster Ferrari 458 Racing Wheel
                        {PAD_XBOX360, 0x24c6, 0x5b02, NULL, NULL},  // Thrustmaster, Inc. GPX Controller
                        {PAD_XBOX360, 0x24c6, 0x5b03, NULL, NULL},  // Thrustmaster Ferrari 458 Racing Wheel
                        {PAD_XBOX360, 0x24c6, 0x5d04, NULL, NULL},  // Razer Sabertooth
                        {PAD_XBOX360, 0x24c6, 0xfafe, NULL, NULL},  // Rock Candy Gamepad for Xbox 360
                        {PAD_XBOX360, 0x2563, 0x058d, NULL, NULL},  // OneXPlayer Gamepad
                        {PAD_XBOX360, 0x2dc8, 0x3106, NULL, NULL},  // 8BitDo Pro 2 Wired Controller
                        {PAD_XBOX360, 0x2f24, 0x0087, NULL, NULL},  // Mayflash F500 V2 [XInput Mode]
                        {PAD_XBOX360, 0x31e3, 0x1100, NULL, NULL},  // Wooting One
                        {PAD_XBOX360, 0x31e3, 0x1200, NULL, NULL},  // Wooting Two
                        {PAD_XBOX360, 0x31e3, 0x1210, NULL, NULL},  // Wooting Lekker
                        {PAD_XBOX360, 0x31e3, 0x1220, NULL, NULL},  // Wooting Two HE
                        {PAD_XBOX360, 0x31e3, 0x1300, NULL, NULL},  // Wooting 60HE (AVR)
                        {PAD_XBOX360, 0x31e3, 0x1310, NULL, NULL},  // Wooting 60HE (ARM)
                        {PAD_XBOX360, 0x3285, 0x0607, NULL, NULL},  // Nacon GC-100

                        {PAD_XBOX360W, 0x045e, 0x0719, NULL, NULL}, // Xbox 360 Wireless Receiver
                        {PAD_XBOX360W, 0x045e, 0x0291, NULL, NULL}, // Xbox 360 Wireless Receiver (xbox)

                        {PAD_XBOX, 0x044f, 0x0f00, NULL, NULL},     // Thrustmaster Wheel
                        {PAD_XBOX, 0x044f, 0x0f03, NULL, NULL},     // Thrustmaster Wheel
                        {PAD_XBOX, 0x044f, 0x0f07, NULL, NULL},     // Thrustmaster, Inc. Controller
                        {PAD_XBOX, 0x044f, 0x0f10, NULL, NULL},     // Thrustmaster Modena GT Wheel
                        {PAD_XBOX, 0x045e, 0x0202, NULL, NULL},     // Microsoft X-Box pad v1 (US)
                        {PAD_XBOX, 0x045e, 0x0285, NULL, NULL},     // Microsoft X-Box pad (Japan)
                        {PAD_XBOX, 0x045e, 0x0287, NULL, NULL},     // Microsoft Xbox Controller S
                        {PAD_XBOX, 0x045e, 0x0289, NULL, NULL},     // Microsoft X-Box pad v2 (US)
                        {PAD_XBOX, 0x046d, 0xca84, NULL, NULL},     // Logitech Xbox Cordless Controller
                        {PAD_XBOX, 0x046d, 0xca88, NULL, NULL},     // Logitech Compact Controller for Xbox
                        {PAD_XBOX, 0x046d, 0xca8a, NULL, NULL},     // Logitech Precision Vibration Feedback Wheel
                        {PAD_XBOX, 0x05fd, 0x1007, NULL, NULL},     // Mad Catz Controller (unverified)
                        {PAD_XBOX, 0x05fd, 0x107a, NULL, NULL},     // InterAct 'PowerPad Pro' X-Box pad (Germany)
                        {PAD_XBOX, 0x05fe, 0x3030, NULL, NULL},     // Chic Controller
                        {PAD_XBOX, 0x05fe, 0x3031, NULL, NULL},     // Chic Controller
                        {PAD_XBOX, 0x062a, 0x0020, NULL, NULL},     // Logic3 Xbox GamePad
                        {PAD_XBOX, 0x062a, 0x0033, NULL, NULL},     // Competition Pro Steering Wheel
                        {PAD_XBOX, 0x06a3, 0x0200, NULL, NULL},     // Saitek Racing Wheel
                        {PAD_XBOX, 0x06a3, 0x0201, NULL, NULL},     // Saitek Adrenalin
                        {PAD_XBOX, 0x0738, 0x4506, NULL, NULL},     // Mad Catz 4506 Wireless Controller
                        {PAD_XBOX, 0x0738, 0x4516, NULL, NULL},     // Mad Catz Control Pad
                        {PAD_XBOX, 0x0738, 0x4520, NULL, NULL},     // Mad Catz Control Pad Pro
                        {PAD_XBOX, 0x0738, 0x4522, NULL, NULL},     // Mad Catz LumiCON
                        {PAD_XBOX, 0x0738, 0x4526, NULL, NULL},     // Mad Catz Control Pad Pro
                        {PAD_XBOX, 0x0738, 0x4530, NULL, NULL},     // Mad Catz Universal MC2 Racing Wheel and Pedals
                        {PAD_XBOX, 0x0738, 0x4536, NULL, NULL},     // Mad Catz MicroCON
                        {PAD_XBOX, 0x0738, 0x4540, NULL, NULL},     // Mad Catz Beat Pad
                        {PAD_XBOX, 0x0738, 0x4556, NULL, NULL},     // Mad Catz Lynx Wireless Controller
                        {PAD_XBOX, 0x0738, 0x4586, NULL, NULL},     // Mad Catz MicroCon Wireless Controller
                        {PAD_XBOX, 0x0738, 0x4588, NULL, NULL},     // Mad Catz Blaster
                        {PAD_XBOX, 0x0738, 0x45ff, NULL, NULL},     // Mad Catz Beat Pad (w/ Handle)
                        {PAD_XBOX, 0x0738, 0x4743, NULL, NULL},     // Mad Catz Beat Pad Pro
                        {PAD_XBOX, 0x0738, 0x6040, NULL, NULL},     // Mad Catz Beat Pad Pro
                        {PAD_XBOX, 0x0c12, 0x0005, NULL, NULL},     // Intec wireless
                        {PAD_XBOX, 0x0c12, 0x8801, NULL, NULL},     // Nyko Xbox Controller
                        {PAD_XBOX, 0x0c12, 0x8802, NULL, NULL},     // Zeroplus Xbox Controller
                        {PAD_XBOX, 0x0c12, 0x8809, NULL, NULL},     // RedOctane Xbox Dance Pad
                        {PAD_XBOX, 0x0c12, 0x880a, NULL, NULL},     // Pelican Eclipse PL-2023
                        {PAD_XBOX, 0x0c12, 0x8810, NULL, NULL},     // Zeroplus Xbox Controller
                        {PAD_XBOX, 0x0c12, 0x9902, NULL, NULL},     // HAMA VibraX - *FAULTY HARDWARE*
                        {PAD_XBOX, 0x0d2f, 0x0002, NULL, NULL},     // Andamiro Pump It Up pad
                        {PAD_XBOX, 0x0e4c, 0x1097, NULL, NULL},     // Radica Gamester Controller
                        {PAD_XBOX, 0x0e4c, 0x1103, NULL, NULL},     // Radica Gamester Reflex
                        {PAD_XBOX, 0x0e4c, 0x2390, NULL, NULL},     // Radica Games Jtech Controller
                        {PAD_XBOX, 0x0e4c, 0x3510, NULL, NULL},     // Radica Gamester
                        {PAD_XBOX, 0x0e6f, 0x0003, NULL, NULL},     // Logic3 Freebird wireless Controller
                        {PAD_XBOX, 0x0e6f, 0x0005, NULL, NULL},     // Eclipse wireless Controller
                        {PAD_XBOX, 0x0e6f, 0x0006, NULL, NULL},     // Edge wireless Controller
                        {PAD_XBOX, 0x0e6f, 0x0008, NULL, NULL},     // After Glow Pro Controller
                        {PAD_XBOX, 0x0e8f, 0x0201, NULL, NULL},     // SmartJoy Frag Xpad/PS2 adaptor
                        {PAD_XBOX, 0x0e8f, 0x3008, NULL, NULL},     // Generic xbox control (dealextreme)
                        {PAD_XBOX, 0x0f30, 0x010b, NULL, NULL},     // Philips Recoil
                        {PAD_XBOX, 0x0f30, 0x0202, NULL, NULL},     // Joytech Advanced Controller
                        {PAD_XBOX, 0x0f30, 0x8888, NULL, NULL},     // BigBen XBMiniPad Controller
                        {PAD_XBOX, 0x102c, 0xff0c, NULL, NULL},     // Joytech Wireless Advanced Controller
                        {PAD_XBOX, 0x12ab, 0x8809, NULL, NULL},     // Xbox DDR dancepad
                        {PAD_XBOX, 0x1430, 0x8888, NULL, NULL},     // TX6500+ Dance Pad (first generation)
                        {PAD_XBOX, 0x3767, 0x0101, NULL, NULL},     // Fanatec Speedster 3 Forceshock Wheel

                        {PAD_XONE, 0x044f, 0xb671, NULL, NULL},     // ThrustMaster, Inc. Ferrari 458 Spider
                        {PAD_XONE, 0x045e, 0x02d1, NULL, NULL},     // Microsoft X-Box One pad
                        {PAD_XONE, 0x045e, 0x02dd, NULL, NULL},     // Microsoft X-Box One pad (fw 2015)
                        {PAD_XONE, 0x045e, 0x02e3, NULL, NULL},     // Microsoft X-Box One Elite pad
                        {PAD_XONE, 0x045e, 0x0b00, NULL, NULL},     // Microsoft X-Box One Elite 2 pad
                        {PAD_XONE, 0x045e, 0x02ea, NULL, NULL},     // Microsoft X-Box One S pad
                        {PAD_XONE, 0x045e, 0x0b0a, NULL, NULL},     // Microsoft X-Box Adaptive Controller
                        {PAD_XONE, 0x045e, 0x0b12, NULL, NULL},     // Microsoft Xbox Series S|X Controller
                        {PAD_XONE, 0x0738, 0x4a01, NULL, NULL},     // Mad Catz FightStick TE 2
                        {PAD_XONE, 0x0e6f, 0x0139, NULL, NULL},     // Afterglow Prismatic Wired Controller
                        {PAD_XONE, 0x0e6f, 0x013a, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x0146, NULL, NULL},     // "Rock Candy Wired Controller for Xbox One
                        {PAD_XONE, 0x0e6f, 0x0147, NULL, NULL},     // "PDP Marvel Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x015c, NULL, NULL},     // "PDP Xbox One Arcade Stick
                        {PAD_XONE, 0x0e6f, 0x0161, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x0162, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x0163, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x0164, NULL, NULL},     // PDP Battlefield One
                        {PAD_XONE, 0x0e6f, 0x0165, NULL, NULL},     // PDP Titanfall 2
                        {PAD_XONE, 0x0e6f, 0x0246, NULL, NULL},     // Rock Candy Gamepad for Xbox One 2015
                        {PAD_XONE, 0x0e6f, 0x02a0, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x02a1, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x02a2, NULL, NULL},     // PDP Wired Controller for Xbox One - Crimson Red
                        {PAD_XONE, 0x0e6f, 0x02a4, NULL, NULL},     // PDP Wired Controller for Xbox One - Stealth Series
                        {PAD_XONE, 0x0e6f, 0x02a6, NULL, NULL},     // PDP Wired Controller for Xbox One - Camo Series
                        {PAD_XONE, 0x0e6f, 0x02a7, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x02a8, NULL, NULL},     // PDP Xbox One Controller
                        {PAD_XONE, 0x0e6f, 0x02ab, NULL, NULL},     // PDP Controller for Xbox One
                        {PAD_XONE, 0x0e6f, 0x02ad, NULL, NULL},     // PDP Wired Controller for Xbox One - Stealth Series
                        {PAD_XONE, 0x0e6f, 0x02b3, NULL, NULL},     // Afterglow Prismatic Wired Controller
                        {PAD_XONE, 0x0e6f, 0x02b8, NULL, NULL},     // Afterglow Prismatic Wired Controller
                        {PAD_XONE, 0x0e6f, 0x0346, NULL, NULL},     // Rock Candy Gamepad for Xbox One 2016
                        {PAD_XONE, 0x0f0d, 0x0063, NULL, NULL},     // Hori Real Arcade Pro Hayabusa (USA) Xbox One
                        {PAD_XONE, 0x0f0d, 0x0067, NULL, NULL},     // HORIPAD ONE
                        {PAD_XONE, 0x0f0d, 0x0078, NULL, NULL},     // Hori Real Arcade Pro V Kai Xbox One
                        {PAD_XONE, 0x0f0d, 0x00c5, NULL, NULL},     // Hori Fighting Commander ONE
                        {PAD_XONE, 0x1532, 0x0a00, NULL, NULL},     // Razer Atrox Arcade Stick
                        {PAD_XONE, 0x1532, 0x0a03, NULL, NULL},     // Razer Wildcat
                        {PAD_XONE, 0x20d6, 0x2001, NULL, NULL},     // BDA Xbox Series X Wired Controller
                        {PAD_XONE, 0x20d6, 0x2009, NULL, NULL},     // PowerA Enhanced Wired Controller for Xbox Series X|S
                        {PAD_XONE, 0x2e24, 0x0652, NULL, NULL},     // Hyperkin Duke X-Box One pad
                        {PAD_XONE, 0x24c6, 0x541a, NULL, NULL},     // PowerA Xbox One Mini Wired Controller
                        {PAD_XONE, 0x24c6, 0x542a, NULL, NULL},     // Xbox ONE spectra
                        {PAD_XONE, 0x24c6, 0x543a, NULL, NULL},     // PowerA Xbox One wired controller
                        {PAD_XONE, 0x24c6, 0x551a, NULL, NULL},     // PowerA FUSION Pro Controller
                        {PAD_XONE, 0x24c6, 0x561a, NULL, NULL},     // PowerA FUSION Controller
                        {PAD_XONE, 0x24c6, 0x5b00, NULL, NULL},     // ThrustMaster Ferrari 458 Racing Wheel
                        {PAD_XONE, 0x2dc8, 0x2000, NULL, NULL},     // 8BitDo Pro 2 Wired Controller fox Xbox

#if defined(WITH_DS3)
                        {PAD_DS3, 0x054c, 0x0268, NULL}, // DS3 wired
#endif

                        {PAD_DINPUT, 0x0079, 0x1804, sfcconverter_processReport,        NULL},               // NES/FC/SFC Joypad TO USB BOX
                        {PAD_DINPUT, 0x046d, 0xc20c, logitechWingman_processReport,     NULL},               // Logitech Wingman
                        {PAD_DINPUT, 0x0810, 0x0001, p2top3converter_processReport,     NULL},               // Dual PSX Adaptor
                        {PAD_DINPUT, 0x0810, 0x0003, p2top3converter_processReport,     NULL},               // PS2 to PS3 USB
                        {PAD_DINPUT, 0x0925, 0x1700, mayflash_processReport,            NULL},               // Mayflash SS
                        {PAD_DINPUT, 0x0b43, 0x0001, smartjoypad_processReport,         NULL},               // Smart Joypad 3 adapter
                        {PAD_DINPUT, 0x0c12, 0x0c30, horiDivaps4_processReport,         NULL},               // Brook universal fighting board (ds4 mode, same as hori diva X ds4)
                        {PAD_DINPUT, 0x0e8f, 0x0003, p3converter_processReport,         NULL},               // PIII Converter Model: 538
                        {PAD_DINPUT, 0x0f0d, 0x0022, horiDiva_processReport,            NULL},               // Brook universal fighting board (ds3 mode, same as hori diva X ds3)
                        {PAD_DINPUT, 0x0f0d, 0x0049, horiDiva_processReport,            NULL},               // Hori ps3 mini diva
                        {PAD_DINPUT, 0x0f0d, 0x0092, horiDiva_processReport,            NULL},               // Raspberry Pi Pico GP2040CE
                        {PAD_DINPUT, 0x0f0d, 0x00a6, horiDiva_processReport,            NULL},               // Hori diva X ps3
                        {PAD_DINPUT, 0x0f0d, 0x00a5, horiDivaps4_processReport,         NULL},               // Hori diva X ps4
                        {PAD_DINPUT, 0x0f0d, 0x0101, horiDivaps4_processReport,         NULL},               // Hori diva FT DX ps4
                        {PAD_DINPUT, 0x0f0d, 0x0102, horiDiva_processReport,            NULL},               // Hori diva FT DX ps3
                        {PAD_DINPUT, 0x1292, 0x4e47, neogeox_processReport,             NULL},               // NEOGEO X
                        {PAD_DINPUT, 0x12ba, 0x0100, horiDiva_processReport,            NULL},               // Raspberry Pi Pico GH
                        {PAD_DINPUT, 0x289b, 0x0044, raphnetpsx_processReport,          NULL},               // Raphnet Technologies PSX to USB v.1.0
                        {PAD_DINPUT, 0x2dc8, 0x3105, eightbitdoadapter_processReport,   NULL},               // 8bitdo adapter 2 (dinput mode)
                        // note: technically, F310 / dualaction doesn't have rumble, but they both can be upgraded to support it
                        {PAD_DINPUT, 0x046d, 0xc216, logitech_processReport,            logitech_setRumble}, // Logitech F310 Gamepad [DirectInput Mode] / DualAction
                        {PAD_DINPUT, 0x046d, 0xc218, logitech_processReport,            logitech_setRumble}, // Logitech F510 Gamepad [DirectInput Mode] / Rumblepad 2
                        {PAD_DINPUT, 0x046d, 0xc294, logitech_wheel_processReport,      NULL},               // Logitech Formula Force EX wheel
                        {PAD_DINPUT, 0x054c, 0x0cda, psclassic_processReport,           NULL},               // Playstation Classic


                        {PAD_GC, 0x057e, 0x0337, NULL, NULL},     // WiiU gamecube adapter (wiiu/switch mode or original)

                        {PAD_UNKNOWN, 0x0000, 0x0000, NULL, NULL}}; // Null
