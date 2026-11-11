// void TriggerClass::Compute_CRC(WWCRCEngine &crc) const (0x006EDA00)
#include "shared/object.hpp"

void TriggerClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(Class->Fetch_ID());
    if (LinkedTo != 0) {
        crc(LinkedTo->Fetch_ID());
    }
    crc((char)IsToDelete);
    crc((char)IsActive);
    crc(Timer.Value());
    crc((long)IsTripped);
}
