// void ObjectTypeClass::Generic_Naming_Convention(char *buffer) (0x005D7C20)
// The theater letter of a file name becomes the generic one ('G').
#include "shared/object_type.hpp"

void __fastcall ObjectTypeClass::Generic_Naming_Convention(char *buffer)
{
    buffer[1] = 'G';
}
