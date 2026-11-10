// TiberiumClass::Compute_CRC (0x006E90A0, slot 13): the AbstractTypeClass part, then the spread and growth rates,
// value, power, colour scheme and the image fields (TibSun tiberium.cpp: SpreadDelay, GrowthDelay, CreditValue,
// Power, Color, FrameCount, Variety).
#include "shared/tiberium.hpp"
// @ref ?Compute_CRC@AbstractTypeClass@@UBEXAAVWWCRCEngine@@@Z=00410960
// @ref ??RWWCRCEngine@@QAEXJ@Z=00493f30

void TiberiumClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Spread);
    crc((long)Growth);
    crc((long)Value);
    crc((long)Power);
    crc((long)unknown_c0);
    crc((long)unknown_e4);
    crc((long)NumFrames);
}
