// void TagClass::Compute_CRC(WWCRCEngine &crc) const (0x006B4110)
#include "shared/object.hpp"

void TagClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    if (Trigger != 0) {
        crc(Trigger->Fetch_ID());
    }
    crc((char)IsToDie);
}
