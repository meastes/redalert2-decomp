// The frame and tick timers of the unit at 0x0040E3C0 (Westwood's FrameTimer, NetFrameTimer
// and TickCount; names hypotheses). Each dynamic initializer reads its time source before the
// source member is constructed (Started precedes Timer), then registers the atexit destructor
// that follows it: 0x0040E9A0 and 0x0040EA20 only return, 0x0040E9E0 runs ~MSTimerClass.
#include "shared/main_loop.hpp"
#include "shared/comm_buffer.hpp"

CDTimerClass<SystemTimerClass> FrameTimer;
CDTimerClass<MSTimerClass> NetFrameTimer;
TTimerClass<SystemTimerClass> TickCount;
