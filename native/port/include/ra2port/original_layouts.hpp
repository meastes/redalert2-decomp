// Field maps from the original game's i686 (VC6) object layouts to the port's
// arm64 ones (lane C6): written by tools/original_layouts.py into the build's
// gen/original_layouts.cpp, applied by port/src/original_save.cpp.
#ifndef RA2PORT_ORIGINAL_LAYOUTS_HPP
#define RA2PORT_ORIGINAL_LAYOUTS_HPP

#include <stdint.h>

enum OriginalLayoutOpKind {
    ORIGINAL_OP_COPY = 0,      // n bytes as they are
    ORIGINAL_OP_WIDEN = 1,     // n 4-byte values to 8 bytes, zero-extended (pointers)
    ORIGINAL_OP_WIDEN_S = 2,   // the same, sign-extended (intptr_t)
    ORIGINAL_OP_BITS = 3,      // a bit-field: width bits at sbit of a unit-byte value to dbit of a dunit-byte value
    ORIGINAL_OP_POINTER = 4,   // n pointers: read as WIDEN; written as swizzle ids or the original's addresses (C17)
    ORIGINAL_OP_VPTR = 5,      // a vtable pointer at src: not read (the object keeps its own); written as the
                               // original's vtable for offset dst of the class whose primary vtable is n (C17)
    ORIGINAL_OP_VPTR_NAMED = 6,  // the same, the class named by its RTTI type descriptor name
                                 // ra2port_original_vtable_names[n] (".?AVUnitClass@@"): the vtables are found
                                 // in the executable at run time (Yuri's Revenge, lane Y10: gamemd.exe)
};

struct OriginalLayoutOp {
    uint32_t kind, src, dst, n;
    uint8_t unit, sbit, dbit, width, dunit;
};

struct OriginalLayout {
    const char *name;          // as original_save.cpp spells a type: no class/struct keywords, no spaces
    uint32_t i686_size, arm64_size;
    uint32_t first, count;     // its operations in ra2port_original_ops
};

extern "C" const OriginalLayoutOp ra2port_original_ops[];
extern "C" const OriginalLayout ra2port_original_layouts[];
extern "C" const unsigned ra2port_original_layout_count;
// ORIGINAL_OP_VPTR_NAMED's type descriptor names (weak and empty when a build has none).
extern "C" const char *const ra2port_original_vtable_names[];

#endif
