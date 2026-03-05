#ifndef __GC_CONTROLLER_H__
#define __GC_CONTROLLER_H__

#include "../controller.h"

uint8_t GCController_probe(Controller *c, int device_id, int port);
uint8_t GCController_processReport(Controller *c, size_t length);
void GCController_setRumble(Controller *c, uint8_t small, uint8_t large);

#endif // __GC_CONTROLLER_H__
