// bool TechnoClass::unknown_slot275() (0x006D8F40, slot 275)
// The type's slot 41, defined out of line (the Infantry and Unit overrides jump to this copy).
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"

bool TechnoClass::unknown_slot275()
{
    return Techno_Type_Class()->CanAttackMove();
}
