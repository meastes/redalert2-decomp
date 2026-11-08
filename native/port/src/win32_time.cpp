// Win32 API mapping, first slice: the time functions the recovered code imports
// (WINMM timeGetTime, KERNEL32 GetTickCount/Sleep/QueryPerformance*). The
// recovered sources declare them extern "C" through shared/win32_api.hpp and
// friends; the native link resolves those names here. This file deliberately
// does not include the recovered headers: it is the platform side of the seam.
// Widths follow Win32: DWORD and the counters' halves are 32 bits.
#include <mach/mach_time.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

namespace {
uint64_t monotonic_ns()
{
    static mach_timebase_info_data_t base;
    if (base.denom == 0) {
        mach_timebase_info(&base);
    }
    return mach_absolute_time() * base.numer / base.denom;
}
}

extern "C" {

// Milliseconds since an arbitrary start, wrapping at 2^32 like the originals.
uint32_t timeGetTime(void) { return (uint32_t)(monotonic_ns() / 1000000u); }
uint32_t GetTickCount(void) { return (uint32_t)(monotonic_ns() / 1000000u); }

void Sleep(uint32_t milliseconds)
{
    struct timespec t = {(time_t)(milliseconds / 1000u), (long)(milliseconds % 1000u) * 1000000L};
    nanosleep(&t, nullptr);
}

// LARGE_INTEGER is passed as a pointer to a 64-bit little-endian value.
int QueryPerformanceFrequency(int64_t *frequency)
{
    *frequency = 1000000000;
    return 1;
}

int QueryPerformanceCounter(int64_t *counter)
{
    *counter = (int64_t)monotonic_ns();
    return 1;
}

}
