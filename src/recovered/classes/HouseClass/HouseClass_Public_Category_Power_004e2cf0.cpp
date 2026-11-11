// long __stdcall HouseClass::Public_Category_Power(int category) (0x004E2CF0)
// The summed power values of this house's technos of a category.
#include "shared/techno.hpp"

long __stdcall HouseClass::Public_Category_Power(int category)
{
    int power = 0;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        TechnoTypeClass *type = techno->Techno_Type_Class();
        if (techno->House == this && type->Category == category) {
            power += type->unknown_59c + type->ThreatPosed;
        }
    }
    return power;
}
