// Behaviour-written coordinate rows (Scout1091, native M8). NOT EXACT; never
// original code or bytes.
#define RA2_TACTICAL_SHADOW_COORD_CTOR_DECL   // the out-of-line constructor's declaration (shared/coord.hpp)
#include "shared/coord.hpp"

// 0x00434820 UnknownCoord434820::UnknownCoord434820(int, int, int): the
// coordinate's three stores (shared/coord.hpp's inline forms: X, Y, Z).
UnknownCoord434820::UnknownCoord434820(int x, int y, int z)
{
    X = x;
    Y = y;
    Z = z;
}

#include "shared/object.hpp"

// 0x00423E60 ObjectClass::unknown_slot115 (leaf src/recovered/classes/ObjectClass/00423e60.cpp:
// returns 0): no converter of its own.
intptr_t ObjectClass::unknown_slot115()
{
    return 0;
}

#include "shared/foot.hpp"

// 0x004C9BC0 FootClass::unknown_slot67 (slot 67, Draw_It; leaf
// src/recovered/classes/FootClass/004c9bc0.cpp: the body is `ret 8`, nothing
// drawn at this level). Pointer-width parameters as every slot 67 natively
// (native_units.py).
void FootClass::unknown_slot67(intptr_t point, intptr_t window)
{
}
