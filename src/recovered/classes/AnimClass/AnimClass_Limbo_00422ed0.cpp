// bool AnimClass::Limbo() (0x00422ED0)
// Leaves the map: a vein animation clears its cell's 0x20000 flag (+0x140)
// first; when the ObjectClass slot succeeds the map entry is removed too.
// Needs ObjectClass::Limbo declared bool (0x005D3700 returns al).
#include "shared/anim.hpp"
#include "shared/map_view.hpp"
// @ref ?Limbo@ObjectClass@@UAE_NXZ=005d3700

bool AnimClass::Limbo()
{
    if (Class->IsVeins) {
        CoordStruct coord = Coord;
        unsigned long &flags = ((UnknownAnimCell *)&Map[coord])->Flags;
        flags = flags & ~0x20000;
    }
    if (ObjectClass::Limbo()) {
        Map.Remove(this);
        return true;
    }
    return false;
}
