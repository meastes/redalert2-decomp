// bool TagClass::Is_Allow_Win() const (0x006B3DC0)
#include "shared/object.hpp"

bool TagClass::Is_Allow_Win() const
{
    return Class->Is_Allow_Win();
}
