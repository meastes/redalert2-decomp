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

// A light view's padding before a member of a real class (port/view_layouts.json,
// written by tools/view_audit.py): the arm64 size on the native target, VC6's
// size where the audit lays the record out for i686-msvc (Scout1111).
#define RA2_NATIVE_PAD(arm64, vc6) (sizeof(void *) == 8 ? (arm64) : (vc6))

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
// The seeds' CRT free 0x00781D3B with a pointer-sized argument (native_units.py
// rewrites the seed-generated `FUN_00781d3b((undefined4)(x))` calls to it; Scout1187, M23).
extern "C" void ra2port_seed_free(intptr_t block);
// GWL_WNDPROC (-4) and GWL_USERDATA (-21), pointer-sized (Win64's
// Set/GetWindowLongPtr shape): native_units.py rewrites SetWindowLongA(w, -4|-21,
// (LONG)p) / GetWindowLongA(w, -4|-21) to these (Scout1119, M14: the owner-draw
// subclassing 0x005ED180 and its control records 0x005ECB00/0x005EDD30).
extern "C" intptr_t ra2port_set_window_ptr(const volatile void *window, int index, intptr_t value);
extern "C" intptr_t ra2port_get_window_ptr(const volatile void *window, int index);
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

// Allocation for the COM class factories' light views and every `new <Name>Class(`
// (native_units.py RULES; a unit may see a light stand-in of the class):
// twice the VC6-sized view, room for the real class's arm64 layout.
struct factory_tag {};
constexpr factory_tag factory_alloc = {};

// VC6's (int) of a floating value: _ftol (0x0077F110) stores the x87 value with
// fistp qword under the game's round-toward-zero control word and returns the low
// 32 bits. A value outside the 64-bit range (or a NaN) stores the integer
// indefinite 0x8000000000000000, low half 0; a value between 2^31 and 2^63 wraps.
// arm64's fcvtzs saturates to INT_MIN/INT_MAX instead (and NaN gives 0), so a
// table index the original wraps or zeroes comes out INT_MIN natively, which abs()
// keeps negative (C5: Fast_Atan2 0x004BA600 reading 8 GB before its table).
inline int x87_ftol(double value)
{
    if (!(value >= -9223372036854775808.0 && value < 9223372036854775808.0)) {
        return 0;
    }
    return (int)(unsigned int)(unsigned long long)(long long)value;
}

// Original saves (lane C6, roadmap (d); port/src/original_save.cpp). The user's
// saves from the original game are compound files whose CONTENTS stream holds
// i686 object images: pointers and pointer-sized integers are 4 bytes there, 8
// here. While one is being read (original_save_mode, set by StgOpenStorage for a
// compound file), the typed IStream::Read of com_persist.hpp's overlay sorts each
// read by its destination type: pointer-wide values are read as 4 bytes and
// widened (the swizzle ids and pointer fields: original addresses, below 2 GB),
// objects are converted from their i686 layout to the arm64 one, the rest is read
// as is.
enum OriginalReadKind { ORIGINAL_READ_RAW, ORIGINAL_READ_UNSIGNED, ORIGINAL_READ_SIGNED, ORIGINAL_READ_OBJECT };
template <class T> struct original_read_kind {
    enum { value = (__is_class(T) || __is_union(T)) ? ORIGINAL_READ_OBJECT : ORIGINAL_READ_RAW };
};
template <class T> struct original_read_kind<T *> { enum { value = ORIGINAL_READ_UNSIGNED }; };
// VC6's long is RA2_LONG natively; long and unsigned long here are only the
// pointer-width rewrites (intptr_t, uintptr_t, size_t: LONG_PTR, WPARAM, ...).
template <> struct original_read_kind<long> { enum { value = ORIGINAL_READ_SIGNED }; };
template <> struct original_read_kind<unsigned long> { enum { value = ORIGINAL_READ_UNSIGNED }; };
template <class T> inline char const *original_type_name() { return __PRETTY_FUNCTION__; }
template <class T, bool polymorphic = __is_polymorphic(T)> struct original_vptr {
    static void const *get(T const *) { return 0; }
};
template <class T> struct original_vptr<T, true> {
    static void const *get(T const *object) { return *(void const *const *)object; }
};
}  // namespace ra2port

extern "C" int ra2port_original_save_mode;
// The port save container version being read (port/src/storage.cpp): 1 (RA2NSTG1) or 2.
extern "C" int ra2port_port_save_version;
// stream_read: the stream's own (virtual) Read, through a thunk; vptr: the object's
// primary vtable pointer when its type is polymorphic (its most derived type gives
// the layout), else type_name (original_type_name's text) names the static type.
typedef int32_t (*ra2port_stream_read_fn)(void *stream, void *buffer, uint32_t size, uint32_t *read);
extern "C" int32_t ra2port_original_read(ra2port_stream_read_fn stream_read, void *stream, void *buffer,
                                         uint32_t size, uint32_t *read, int kind, void const *vptr,
                                         char const *type_name);

inline void *operator new(size_t size, ra2port::factory_tag) { return ::operator new(2 * size); }
inline void operator delete(void *p, ra2port::factory_tag) { ::operator delete(p); }

#endif
