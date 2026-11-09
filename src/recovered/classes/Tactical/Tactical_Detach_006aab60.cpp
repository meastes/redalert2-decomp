// void Tactical::Detach(AbstractClass *target, bool all) (0x006AAB60, slot 10)
// Forgets a target: clears it from the selectable object table and, when it is
// the object waiting for placement, cancels the placement cursor (TibSun
// Tactical::Detach; the placement part is RA2's).
#include "shared/tactical.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"

void Tactical::Detach(AbstractClass *target, bool all)
{
    for (int index = 0; index < 500; index++) {
        TacticalObjectPixel &sel = unknown_tactical_pixels_abe5f8[index];
        if (sel.Object == (ObjectClass *)target) {
            sel.Object = 0;
        }
    }
    if (target == Map.unknown_118c) {
        Map.Set_Cursor_Shape(0);
        Map.unknown_118c = 0;
        Map.unknown_1190 = 0;
        Map.unknown_1194 = -1;
    }
}
