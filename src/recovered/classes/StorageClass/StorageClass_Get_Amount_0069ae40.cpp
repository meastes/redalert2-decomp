// float StorageClass::Get_Amount(int type) const (0x0069AE40): the amount held of one
// tiberium type (TibSun StorageClass::Get_Amount).
#include "shared/storage_class.hpp"

float StorageClass::Get_Amount(int type) const
{
    return Amounts[type];
}
