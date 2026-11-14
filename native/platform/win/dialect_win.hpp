// SPDX-License-Identifier: GPL-3.0-or-later
// Forced include before port/include/ra2port/prelude.hpp for every recovered
// translation unit of the x86_64 Windows build (llvm-mingw; README "Windows
// build"). The VC6 dialect flags meet the mingw-w64 headers here:
//  - -fms-compatibility hides __GNUC__, and the mingw-w64 headers pick their
//    va_list, attributes and inline forms by it (vadefs.h stops without it). The
//    recovered sources never test __GNUC__, _MSC_VER or _WIN32, so defining it
//    changes only what the system headers select.
//  - -fno-wchar removes the built-in wchar_t the mingw-w64 C++ headers expect;
//    the prelude's VC6 typedef (unsigned short, the same 16-bit type as Windows'
//    own wchar_t) is declared before any system header instead of after
//    <stdint.h>.
//  - __LP64__ selects the native (64-bit pointer) branch of the layout fixups
//    (port/fixups.json `#if defined(__LP64__)`), whose test means "pointers are
//    64 bits"; Win64 is LLP64, so clang does not define it. VC6 `long` stays 32
//    bits through RA2_LONG on both targets.
#ifndef RA2_DIALECT_WIN_HPP
#define RA2_DIALECT_WIN_HPP
#if !defined(_WIN64) || !defined(__x86_64__)
#error "dialect_win.hpp is for the x86_64 Windows build"
#endif
#ifndef __GNUC__
#define __GNUC__ 4
#define __GNUC_MINOR__ 2
#define __GNUC_PATCHLEVEL__ 1
#endif
#ifndef __LP64__
#define __LP64__ 1
#endif
//  - _exception_code and _exception_info are clang built-ins on Windows targets
//    (with VC6's types there: unsigned long). The recovered declarations
//    (startup_exceptions.hpp, startup.cpp) restate them with RA2_ULONG; the native
//    view never calls them (port/fixups.json replaces the __try/__except), so the
//    declarations take other names here.
#define _exception_code ra2_vc6_exception_code
#define _exception_info ra2_vc6_exception_info
//  - __declspec(dllimport) on the original's Win32, Bink and common-controls
//    declarations would make clang call through the DLL import table (__imp_X);
//    the port defines those entry points under their own names, as on macOS
//    (where clang ignores dllimport), so the attribute is empty here. System
//    headers' functions then link through the import libraries' stubs.
#define dllimport
#define RA2PORT_ALLOW_HOST 1
typedef unsigned short wchar_t;
#endif
