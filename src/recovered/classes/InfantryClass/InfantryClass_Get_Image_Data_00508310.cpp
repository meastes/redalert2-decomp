// void *InfantryClass::Get_Image_Data() (0x00508310, slot 26)
// FootClass's (a tail jump).
#include "shared/infantry.hpp"
// @ref ?Get_Image_Data@FootClass@@UAEPAXXZ=004cd650

void *InfantryClass::Get_Image_Data()
{
    return FootClass::Get_Image_Data();
}
