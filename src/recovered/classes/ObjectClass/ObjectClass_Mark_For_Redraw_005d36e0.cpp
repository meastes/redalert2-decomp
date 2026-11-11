// void ObjectClass::Mark_For_Redraw() (0x005D36E0)
#include "shared/object_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0

void ObjectClass::Mark_For_Redraw()
{
    if (!unknown_75) {
        unknown_75 = true;
        Map.Flag_To_Redraw(0);
    }
}
