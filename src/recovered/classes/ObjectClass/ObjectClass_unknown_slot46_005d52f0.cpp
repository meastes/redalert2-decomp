// bool ObjectClass::unknown_slot46(TechnoClass *techno) (0x005D52F0, slot 46)
// Starts from +0x79 (on a bridge). Compares the ground height under slot 19's
// coordinate for techno with the ground height under the object: off a bridge,
// a drop of more than three height levels (unknown_geometry_g) onto a bridge
// cell answers true; on a bridge, a rise of more than three levels answers
// false. techno is passed to slot 19 (TechnoClass argument; the walk
// locomotor 0x0071E220 passes its LinkedTo).
#include "shared/object_type.hpp"
#include "shared/techno.hpp"
#include "shared/techno_map.hpp"
// @ref _unknown_geometry_g=00a72f40

bool ObjectClass::unknown_slot46(TechnoClass *techno)
{
    bool result = unknown_79;
    CoordStruct coord = unknown_slot19(techno);
    int there = Map.Unknown_0055A280(coord);
    int ground = Map.Unknown_0055A280(CoordStruct(Coord));
    if (!unknown_79 && ground - there > unknown_geometry_g * 3 && (Map_Cell(coord)->Flags & 0x100)) {
        return true;
    }
    if (unknown_79 && there - ground > unknown_geometry_g * 3) {
        return false;
    }
    return result;
}
