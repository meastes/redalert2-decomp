// bool InfantryTypeClass::CanAttackMove() const (0x0050A9A0)
#include "shared/infantry_type.hpp"
// @ref ?CanAttackMove@TechnoTypeClass@@UBE_NXZ=006dacc0

bool InfantryTypeClass::CanAttackMove() const
{
    return TechnoTypeClass::CanAttackMove();
}
