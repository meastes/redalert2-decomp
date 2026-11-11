// void TriggerTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x006EED50)
#include "shared/object.hpp"

void TriggerTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc(House->Fetch_ID());
    if (LinkedTo != 0) {
        crc(LinkedTo->Fetch_ID());
    }
    if (FirstEvent != 0) {
        crc(FirstEvent->Fetch_ID());
    }
    if (FirstAction != 0) {
        crc(FirstAction->Fetch_ID());
    }
}
