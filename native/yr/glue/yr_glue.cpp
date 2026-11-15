// Native stand-ins for YR functions cut from the generated units (lane Y1; native/yr/fixups.json).
// NOT EXACT and not original code.

// Dump_Exception_Info, YR 0x004C85E0 (facing.cpp): wrote the x86 registers and a stack dump to
// except.txt for the crash dialog. Natively the port's fault handler reports a fault (the RA2
// native tree does the same); Exception_Handler still runs and calls this.
struct _EXCEPTION_POINTERS;
void Dump_Exception_Info(_EXCEPTION_POINTERS *) {}

// FUN_004A4AC0, YR 0x004A4AC0 (1 byte: `ret`): the release build's debug print without the
// prefix (the tree's DebugStringNoPrefix, which it calls by this address name too); gamemd.exe's
// body is empty, and so is this one (lane Y3).
void __cdecl FUN_004A4AC0(const char *)
{
}

// Get_CPU_Clock (detproc.asm, TibSun mpu.h): rdtsc, the high half through the reference and the
// low half returned. Natively the time-stamp counter is the port's virtual 1 GHz game clock, as
// ra2-native's (native/link/behaviour_asm_lib.cpp); MillisecondTimerClass divides it by the rate
// Get_CPU_Rate measures from the same clock (lane Y3).
#include <stdint.h>
extern "C" uint64_t ra2port_game_ns(void);
extern "C" unsigned int __cdecl Get_CPU_Clock(unsigned int & high)
{
	uint64_t t = ra2port_game_ns();
	high = (unsigned int)(t >> 32);
	return (unsigned int)t;
}
