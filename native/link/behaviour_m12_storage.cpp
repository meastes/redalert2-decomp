// Behaviour row (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// 0x0069AE40 StorageClass::Get_Amount (TibSun's StorageClass: the amount held of one
// tiberium type, Amounts[index]). The harness refinery's harvester reaches it once
// it starts to harvest.
#include "shared/storage_class.hpp"

float StorageClass::Get_Amount(int type) const
{
    return Amounts[type];
}
