// ObjectClass *CellClass::Find_Object(int kind, bool alt) const (0x00471760):
// the first occupant (alt: of the alternate list) whose Kind_Of is kind;
// none while unknown_flag_a40958 is clear (TibSun cell.cpp Cell_Find_Object: the list head once, then a while
// loop; this form lets Draw_Placement_Cursor 0x00473F20 expand it through Cell_Terrain as the original does).
#include "shared/cell.hpp"
#include "shared/object.hpp"

ObjectClass *CellClass::Find_Object(int kind, bool alt) const
{
    if (unknown_flag_a40958) {
        ObjectClass *object = alt ? AltOccupierPtr : OccupierPtr;

        while (object != 0) {
            if (object->Kind_Of() == kind) {
                return object;
            }
            object = object->unknown_30;
        }
    }
    return 0;
}
