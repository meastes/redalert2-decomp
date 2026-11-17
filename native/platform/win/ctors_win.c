// SPDX-License-Identifier: GPL-3.0-or-later
// Static initialisers in link order on Windows (lane C7; README "Windows build").
//
// The game links its units in the original's _initterm order (units.cmake), so the
// recovered static initialisers run as they did in the original: on macOS dyld runs
// each object's initialisers in link order. The mingw-w64 runtime does not: its
// __do_global_ctors (libmingw32's gccmain.o) runs the .ctors list backwards, so the
// last unit linked started first (RA2_NATIVE_INIT_TRACE=1 printed #3609 down to #0).
//
// lld lays the list out as GNU ld does: -1, every object's .ctors entry in link order,
// then the prioritised .ctors.NNNNN entries sorted by name (NNNNN = 65535 - priority),
// then 0. This file replaces gccmain.o (__main, __do_global_ctors, __do_global_dtors;
// linked into the game before libmingw32, so that member is never pulled in) and runs
// the prioritised entries first, backwards as the runtime does (priority 101 first:
// libc++'s own), then the plain entries forwards, in link order. A marker in
// .ctors.00000 (priority 65535, which no compiler emits) sorts between the two parts.
// Without the marker in the list everything runs as the runtime would run it.
#include <stddef.h>
#include <stdlib.h>

typedef void (*ra2_ctor)(void);
extern ra2_ctor __CTOR_LIST__[];
extern ra2_ctor __DTOR_LIST__[];

static void ra2_ctor_boundary(void) {}
__attribute__((section(".ctors.00000"), used)) static ra2_ctor ra2_ctor_marker = ra2_ctor_boundary;

void __do_global_dtors(void)
{
    static ra2_ctor *p = __DTOR_LIST__ + 1;
    while (*p) {
        (*p)();
        ++p;
    }
}

void __do_global_ctors(void)
{
    size_t n = (size_t)(ptrdiff_t)__CTOR_LIST__[0];
    if (n == (size_t)-1)
        for (n = 0; __CTOR_LIST__[n + 1] != 0; ++n) {}
    size_t mark = 0;
    for (size_t i = 1; i <= n; ++i)
        if (__CTOR_LIST__[i] == ra2_ctor_boundary) mark = i;
    if (mark == 0) {
        for (size_t i = n; i >= 1; --i) __CTOR_LIST__[i]();
    } else {
        for (size_t i = n; i > mark; --i) __CTOR_LIST__[i]();
        for (size_t i = 1; i < mark; ++i) __CTOR_LIST__[i]();
    }
    atexit(__do_global_dtors);
}

void __main(void)
{
    static int initialized;
    if (!initialized) {
        initialized = 1;
        __do_global_ctors();
    }
}
