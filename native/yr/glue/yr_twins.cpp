// YR functions ra2-decomp has not matched, written from the RA2 tree's exact counterparts (lane Y1,
// stage 1 onward). NOT EXACT for YR: behaviour code from the RA2 twin (docs/YR_OVERLAP.md), each
// with its RA2 source named. They replace link-closure traps reached on the way to the menu.
#include <windows.h>
#include <mmsystem.h>
#include "stimer.h"

// SystemTimerClass::operator() / operator int (RA2 0x0069A430, src/recovered/stimer.cpp): the
// system clock in 16 ms units, timeGetTime() >> 4.
int SystemTimerClass::operator()(void) const { return (int)(timeGetTime() >> 4); }
SystemTimerClass::operator int(void) const { return (int)(timeGetTime() >> 4); }
