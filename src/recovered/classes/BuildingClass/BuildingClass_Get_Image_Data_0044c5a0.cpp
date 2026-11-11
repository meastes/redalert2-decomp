// void *BuildingClass::Get_Image_Data() const (0x0044C5A0, slot 26)
// The type's image: slot 39 while +0x42C is set and +0x5B4 holds, otherwise
// the buildup image (type slot 48).
#include "shared/building.hpp"

void *BuildingClass::Get_Image_Data()
{
    if (unknown_42c && unknown_5b4) {
        return Type->Get_Image_Data();
    }
    return Type->Get_Buildup_Data();
}
