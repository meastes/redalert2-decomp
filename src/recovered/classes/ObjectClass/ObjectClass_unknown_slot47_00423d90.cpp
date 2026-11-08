// bool ObjectClass::unknown_slot47() (0x00423D90): slot 47 (+0xBC); the base answer is true
// (CellClass::Occupy_Down 0x00473B30 asks it before moving a placed object to the cell's coordinate).
// Its body sits in this unit, after AnimClass's members (the original's copy may be an inline of
// the class emitted with the AnimClass vtable; as an inline in shared/object.hpp it moves
// Print_CRCs 0x0062A360 through the declaration counter).
#include "shared/object.hpp"

bool ObjectClass::unknown_slot47()
{
    return true;
}
