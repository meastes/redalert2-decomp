// Win32 API mapping, first slice: the time functions the recovered code imports
// (WINMM timeGetTime, KERNEL32 GetTickCount/Sleep/QueryPerformance*). The
// recovered sources declare them extern "C" through shared/win32_api.hpp and
// friends; the native link resolves those names here. This file deliberately
// does not include the recovered headers: it is the platform side of the seam.
// Widths follow Win32: DWORD and the counters' halves are 32 bits.
#include "ra2port/win32_port.hpp"
#include "os.hpp"
#include <sched.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

namespace {
uint64_t monotonic_ns() { return ra2port::os::monotonic_ns(); }
}

extern "C" {

// Milliseconds since an arbitrary start, wrapping at 2^32 like the originals.
// The clocks are what the game's busy waits poll (Init_Game 0x005116A0 spins on
// Audio_Now until its minimum loading time): each read lets the host keep the
// window responsive (ra2port::keep_alive, rate-limited).
// They are the game's clock (ra2port::game_clock_ns): it stands still while the
// host pause holds the game (Scout1247).
uint32_t timeGetTime(void) { ra2port::keep_alive(); return (uint32_t)(ra2port::game_clock_ns() / 1000000u); }
uint32_t GetTickCount(void) { ra2port::keep_alive(); return (uint32_t)(ra2port::game_clock_ns() / 1000000u); }
// The game's clock for the link layer (the virtual TSC, link/behaviour_asm_lib.cpp).
uint64_t ra2port_game_ns(void) { return ra2port::game_clock_ns(); }

// Native diagnostics (Scout1191, M24): the time spent in Sleep, so a test run
// can tell a frame's work from the game's own pacing (harness frame times).
thread_local uint64_t ra2port_slept_ns = 0;   // per thread: the main thread's own sleeps
uint64_t ra2port_monotonic_ns(void) { return monotonic_ns(); }

// Sleeps until a monotonic deadline at nanosecond resolution (the native frame
// floor's pacing; Sleep's whole milliseconds overshot it by about 1.5 ms a frame).
// The host first shows what the main thread drew (Host::wait); the deadline
// keeps the wait's length whatever the present cost.
static void wait_until(uint64_t start, uint64_t deadline)
{
    const ra2port::Host *h = ra2port::host();
    if (h && h->wait) h->wait();
    ra2port::keep_alive();
    uint64_t now = monotonic_ns();
    if (deadline <= start) {
        sched_yield();   // Sleep(0): give up the rest of the time slice
    } else if (deadline > now) {
        uint64_t wait = deadline - now;
        struct timespec t = {(time_t)(wait / 1000000000u), (long)(wait % 1000000000u)};
        nanosleep(&t, nullptr);
    }
    ra2port_slept_ns += monotonic_ns() - start;
}

void ra2port_sleep_until_ns(uint64_t deadline)
{
    uint64_t start = monotonic_ns();
    if (deadline <= start) return;
    wait_until(start, deadline);
}

// Sleep(n) waits n ms from the call (Windows: at least n ms). The main thread's
// sleeps are where the original's drawing became visible: the menus' slide
// (0x005E5150: draw a step, copy it to the primary, Sleep(30)) and the loading
// screen (GScreenClass::Blit 0x004E0C90: the cursor drawn for Sleep(50)).
// Natively the primary reached the window only at the next event pump or after
// 100 ms, so the 18-step slide showed about 5 of its steps (Scout1209).
void Sleep(uint32_t milliseconds)
{
    uint64_t start = monotonic_ns();
    wait_until(start, start + (uint64_t)milliseconds * 1000000u);
}

// LARGE_INTEGER is passed as a pointer to a 64-bit little-endian value.
int QueryPerformanceFrequency(int64_t *frequency)
{
    *frequency = 1000000000;
    return 1;
}

int QueryPerformanceCounter(int64_t *counter)
{
    ra2port::keep_alive();
    *counter = (int64_t)ra2port::game_clock_ns();
    return 1;
}

}
