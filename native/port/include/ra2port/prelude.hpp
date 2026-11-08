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

// Port helpers referenced by native_units.py rewrites and port/fixups.json.
// A dialog's DWL_USER slot, pointer-sized (port/src/user32.cpp): native_units.py
// rewrites SetWindowLongA(w, 8, (LONG)p) / GetWindowLongA(w, 8) to these.
extern "C" intptr_t ra2port_set_dialog_user(const volatile void *window, const volatile void *value);
extern "C" intptr_t ra2port_get_dialog_user(const volatile void *window);
namespace ra2port {
// VC6 accepts a cast of an rvalue to a reference type and reinterprets a
// temporary copy. native_units.py wraps such cast operands in as_lvalue: it
// binds the temporary until the end of the full expression (the lifetime VC6
// gives it) and is the identity on lvalues.
template <class T> inline T &as_lvalue(T &&value) { return value; }

// DynamicVectorClass<T> declares Delete(T const &) and Delete(int). For T = int
// ISO C++ finds an int argument ambiguous; VC6 always takes Delete(int index)
// (verified inline at 0x0047D910, 0x0047D970, 0x004F9D84, 0x0057C30A: each
// shifts the elements after the argument used as an index, with no ID call).
// The by-object overload's parameter goes through this trait, which for int is
// a wrapper reachable only through a user conversion, so Delete(int) wins as on
// VC6; for every other T it is exactly T const &.
template <class T> struct vector_object_ref {
    T const &value;
    vector_object_ref(T const &v) : value(v) {}
    operator T const &() const { return value; }
};
template <class T> struct delete_object_arg { typedef T const &type; };
template <> struct delete_object_arg<int> { typedef vector_object_ref<int> type; };

// An isometric tile image's per-cell record reference (IsoTileImageStruct::Tiles,
// 4 bytes per cell in the .tem/.sno/... file): the file stores the offset of the
// cell's record from the image start; VC6 code converts it in place to a 32-bit
// pointer (Load_Tile_Data 0x0052B560, Read_Control_File 0x00529870) and casts it
// back ((IsoTileCellStruct *)image->Tiles[i]). A 64-bit pointer does not fit, so
// natively Resolve stores the record's distance from the slot itself with the
// high bit set (idempotent, as the original's "already converted" test), a
// pointer cast returns the record, and the int value still tests against 0.
struct IsoTileRef {
    int v;
    operator int() const { return v; }
    void *get() const { return v < 0 ? (char *)this + (v & 0x7FFFFFFF) : (void *)0; }
    template <class T> explicit operator T *() const { return (T *)get(); }
    void Resolve(void const *base)
    {
        if (v > 0) {
            v = (int)(((char const *)base + v - (char const *)this) | 0x80000000);
        }
    }
};
static_assert(sizeof(IsoTileRef) == 4, "IsoTileRef keeps the file's 4-byte slot");

// Allocation for the COM class factories' light views (native_units.py RULES):
// twice the VC6-sized view, room for the real class's arm64 layout.
struct factory_tag {};
constexpr factory_tag factory_alloc = {};
}  // namespace ra2port

inline void *operator new(size_t size, ra2port::factory_tag) { return ::operator new(2 * size); }
inline void operator delete(void *p, ra2port::factory_tag) { ::operator delete(p); }

#endif
