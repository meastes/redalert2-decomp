// void *ObjectClass::Get_Image_Data() (0x005D27B0)
#include "shared/object_type.hpp"

void *ObjectClass::Get_Image_Data()
{
    return Class_Of()->Get_Image_Data();
}
