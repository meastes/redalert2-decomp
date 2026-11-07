// void BuildingClass::Assign_Destination(AbstractClass *target, bool immediate) (0x00450F00, slot 260)
// Ignored in mission 19; otherwise records the target (0x006D6E50) when slot 153
// or the type flag +0x1353 holds, then the techno behaviour.
#include "shared/building.hpp"
// @ref ?unknown_6d6e50@TechnoClass@@QAEXPAVAbstractClass@@@Z=006d6e50
// @ref ?Assign_Destination@TechnoClass@@UAEXPAVAbstractClass@@_N@Z=006d48e0

void BuildingClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    if (Mission != 19) {
        if (unknown_slot153() || Type->ConstructionYard) {
            unknown_6d6e50(target);
        }
        TechnoClass::Assign_Destination(target, immediate);
    }
}
