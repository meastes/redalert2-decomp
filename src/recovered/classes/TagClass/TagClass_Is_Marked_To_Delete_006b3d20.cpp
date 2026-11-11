// bool TagClass::Is_Marked_To_Delete() const (0x006B3D20)
#include "shared/object.hpp"

bool TagClass::Is_Marked_To_Delete() const
{
    return IsToDie;
}
