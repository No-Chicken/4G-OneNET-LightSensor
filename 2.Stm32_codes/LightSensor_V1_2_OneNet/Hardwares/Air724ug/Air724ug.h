#ifndef __AIR724UG_H
#define __AIR724UG_H

#include "main.h"

void Air724_Reset(void);
void Air724_Init(void);
void Air724_PDPact(void);
void Air724_FlightMode(void);

extern const char * ProductID;
extern const char * DeviceName;

#endif
