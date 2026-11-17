// The native YR build's second prelude (lane Y1), after ra2port/prelude.hpp: the host's C
// library types first, then the switches that make the mingw-w64 Win32/DirectX headers
// (build/yr-native/mingw, native/yr/tools/yr_sdk.py) take them instead of declaring their own.
#ifndef YR_NATIVE_PRELUDE_H
#define YR_NATIVE_PRELUDE_H
#ifdef __MINGW32__
// The x86_64 Windows build (lane Y13): LLP64's long is 32 bits, VC6's width, and the Win32 headers,
// the C library and clang's MS intrinsics all say long; the YR units' RA2_LONG (the port rewrites'
// long) is spelt long there too, so their types are the headers' (yr_sdk.py keeps long).
#define RA2_LONG long
#define RA2_ULONG unsigned long
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <time.h>
// The C runtime types are the host's (mingw's corecrt.h skips each one it finds defined).
#define _SIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#define _RSIZE_T_DEFINED
typedef size_t rsize_t;
#define _INTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#define _PTRDIFF_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCTYPE_T_DEFINED
#define _TIME_T_DEFINED
#define _OFF_T_DEFINED
#define _OFF64_T_DEFINED
typedef long long _off64_t;
typedef long long off64_t;
#ifndef __MINGW32__
typedef int _off_t;   // (Windows: the toolchain's own, a 32-bit long)
#endif
#define _VA_LIST_DEFINED
// No inline C-runtime helpers from the mingw headers (stralign.h, ...): they call its own CRT.
#define __CRT__NO_INLINE 1
#define __GNUC_VA_LIST
typedef va_list __gnuc_va_list;
// Win64 headers (LLP64): pointer-sized WPARAM/LPARAM/handles, as the RA2 native tree (--win64-params).
// VC6 keeps the old calling-convention keywords (the prelude empties __cdecl and friends).
#define cdecl
#define pascal
#ifndef _WIN64
#define _WIN64 1
#endif

// The C++ Interlocked overloads take unsigned and RA2_ULONG, the same type here.
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
// The window-data indexes the Win64 headers name DWLP_/GWLP_ only; the YR tree uses the Win32
// names (the RA2 tree's port rewrites turn these calls into ra2port_set/get_dialog_user and
// ra2port_set/get_window_ptr, which keep pointer-sized values).
#define DWL_MSGRESULT 0
#define DWL_DLGPROC 4
#define DWL_USER 8
#define GWL_WNDPROC (-4)
#define GWL_HINSTANCE (-6)
#define GWL_HWNDPARENT (-8)
#define GWL_USERDATA (-21)

#include "yr_crt.h"
#endif
