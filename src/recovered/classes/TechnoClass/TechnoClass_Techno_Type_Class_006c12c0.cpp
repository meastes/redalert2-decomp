// TechnoTypeClass *TechnoClass::Techno_Type_Class() (0x006C12C0, slot 32)
// The type as a techno type (Class_Of).
#include "shared/techno.hpp"

TechnoTypeClass *TechnoClass::Techno_Type_Class()
{
    return (TechnoTypeClass *)Class_Of();
}
