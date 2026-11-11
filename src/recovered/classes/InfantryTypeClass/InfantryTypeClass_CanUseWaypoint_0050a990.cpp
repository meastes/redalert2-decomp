// bool InfantryTypeClass::CanUseWaypoint() const (0x0050A990)
#include "shared/infantry_type.hpp"
// @ref ?CanUseWaypoint@TechnoTypeClass@@UBE_NXZ=006dacb0

bool InfantryTypeClass::CanUseWaypoint() const
{
    return TechnoTypeClass::CanUseWaypoint();
}
