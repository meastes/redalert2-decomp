// ObjectClass *SmudgeTypeClass::Create_One_Of(HouseClass *house) const (0x006880A0)
// Creates an unplaced smudge of this type (at the zero coordinate).
#include "shared/smudge_type.hpp"
#include "shared/smudge.hpp"

ObjectClass *SmudgeTypeClass::Create_One_Of(HouseClass *house) const
{
    return new SmudgeClass((SmudgeTypeClass *)this, unknown_geometry_l, -1);
}
