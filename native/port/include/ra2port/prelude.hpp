// Forced include for every native translation unit built from src/recovered.
// It maps the VC6/x86 dialect the recovered sources are written in onto clang
// for arm64 macOS without changing their behaviour:
//  - calling conventions do not exist on arm64: __cdecl/__stdcall/__fastcall/
//    __thiscall expand to nothing (the sources never depend on stack cleanup);
//  - VC6 `long` is 32 bits: native_units.py rewrites long/unsigned long to
//    RA2_LONG/RA2_ULONG, defined here as exact 32-bit types;
//  - VC6 has no native wchar_t: it is a typedef of unsigned short. The native
//    build passes -Xclang -fno-wchar -fshort-wchar and typedefs it here, so
//    wchar_t and unsigned short are one type again (L"" literals included);
//    the wide CRT helpers are renamed ra2_* by native_units.py and supplied by
//    the port (libc's take a 32-bit wchar_t).
#ifndef RA2PORT_PRELUDE_HPP
#define RA2PORT_PRELUDE_HPP

#include <stddef.h>
#include <stdint.h>

#if !defined(__aarch64__) && !defined(RA2PORT_ALLOW_HOST)
#error "the native tree targets arm64; define RA2PORT_ALLOW_HOST for host experiments"
#endif

#define __cdecl
#define __stdcall
#define __fastcall
#define __thiscall
#define _cdecl
#define _stdcall
#define _fastcall
#ifndef __forceinline
#define __forceinline inline __attribute__((always_inline))
#endif

typedef unsigned short wchar_t;
typedef int32_t RA2_LONG;
typedef uint32_t RA2_ULONG;
typedef size_t RA2_SIZE_T;

static_assert(sizeof(RA2_LONG) == 4 && sizeof(RA2_ULONG) == 4, "VC6 long is 32 bits");
static_assert(sizeof(wchar_t) == 2, "build with -fshort-wchar: RA2 strings are UTF-16");

#endif
