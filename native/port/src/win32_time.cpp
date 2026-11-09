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

// Native diagnostics (Scout1191, M24): the time spent in Sleep, so a test run
// can tell a frame's work from the game's own pacing (harness frame times).
thread_local uint64_t ra2port_slept_ns = 0;   // per thread: the main thread's own sleeps
uint64_t ra2port_monotonic_ns(void) { return monotonic_ns(); }

// Sleeps until a monotonic deadline at nanosecond resolution (the native frame
// floor's pacing; Sleep's whole milliseconds overshot it by about 1.5 ms a frame).
void ra2port_sleep_until_ns(uint64_t deadline)
{
    uint64_t start = monotonic_ns();
    if (deadline <= start) return;
    uint64_t wait = deadline - start;
    struct timespec t = {(time_t)(wait / 1000000000u), (long)(wait % 1000000000u)};
    nanosleep(&t, nullptr);
    ra2port_slept_ns += monotonic_ns() - start;
}

void Sleep(uint32_t milliseconds)
{
    uint64_t start = monotonic_ns();
    struct timespec t = {(time_t)(milliseconds / 1000u), (long)(milliseconds % 1000u) * 1000000L};
    nanosleep(&t, nullptr);
    ra2port_slept_ns += monotonic_ns() - start;
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
