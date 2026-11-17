// Force-included first in the YR units of the x86_64 Windows build (lane Y13), before
// platform/win/dialect_win.hpp: mingw-w64's _mingw.h is read while __LP64__ is still undefined, so
// __LONG32 (the Win32 headers' long) is long, 32 bits on Win64 as in VC6, the type clang's MS
// intrinsics and the C library use. dialect_win.hpp then defines __LP64__ for the pointer-width
// branches as in the RA2 units (the Win32 headers' LP64 branches are the Mac build's, which the
// YR units are written against). yr_prelude.h spells the units' RA2_LONG as long to match.
#ifndef YR_WIN_FIRST_H
#define YR_WIN_FIRST_H
#include <_mingw.h>
#endif
