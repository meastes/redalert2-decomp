// VectorClass<UnitTypeClass *>::VectorClass(unsigned, UnitTypeClass * const *) (0x004FB610): the out-of-line copy of the HouseTypeClass unit
// (shared/vector_class.hpp). The vtable and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??_7?$VectorClass@PAVUnitTypeClass@@@@6B@=007a3138
// @ref ??_E?$VectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004fa800
// @ref ??_G?$VectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004fa800
// @ref ??8?$VectorClass@PAVUnitTypeClass@@@@UBE_NABV0@@Z=004face0
// @ref ?Resize@?$VectorClass@PAVUnitTypeClass@@@@UAE_NHPBQAVUnitTypeClass@@@Z=004fad20
// @ref ?Clear@?$VectorClass@PAVUnitTypeClass@@@@UAEXXZ=004fadd0
// @ref ?ID@?$VectorClass@PAVUnitTypeClass@@@@UAEHABQAVUnitTypeClass@@@Z=004fae00
// @ref ?ID@?$VectorClass@PAVUnitTypeClass@@@@UAEHPBQAVUnitTypeClass@@@Z=004fae40
// @ref ?Fetch@?$VectorClass@PAVUnitTypeClass@@@@UBEPAVUnitTypeClass@@H@Z=004fa620
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"

class UnitTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<UnitTypeClass *>::VectorClass(unsigned size, UnitTypeClass * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) UnitTypeClass *[size];
        } else {
            Vector = new UnitTypeClass *[size];
            IsAllocated = true;
        }
    }
}
