// The original's win.h (TibSun code/win.h:35-43, mischa85 include/win.h): WIN32_LEAN_AND_MEAN, then windows.h,
// mmsystem.h, windowsx.h, winnt.h and winuser.h. RA2_SDK_HEADERS migration switch only (docs/DECISIONS.md
// "SDK headers for a unit"): an unconverted unit sees nothing here.
#ifndef RA2_SHARED_WIN_HPP
#define RA2_SHARED_WIN_HPP
#ifdef RA2_SDK_HEADERS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <mmsystem.h>
#include <windowsx.h>
#include <winnt.h>
#include <winuser.h>
#endif
#endif
