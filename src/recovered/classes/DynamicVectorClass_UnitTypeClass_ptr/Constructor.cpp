// DynamicVectorClass<UnitTypeClass *>::DynamicVectorClass(unsigned, UnitTypeClass * const *) (0x004FAE60): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@PAVUnitTypeClass@@@@UBE_NABV0@@Z=004face0
// @ref ??_7?$DynamicVectorClass@PAVUnitTypeClass@@@@6B@=007a3198
// @ref ??_7?$VectorClass@PAVUnitTypeClass@@@@6B@=007a3138
// @ref ??_E?$DynamicVectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004faec0
// @ref ??_E?$VectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004fa800
// @ref ??_G?$DynamicVectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004faec0
// @ref ??_G?$VectorClass@PAVUnitTypeClass@@@@UAEPAXI@Z=004fa800
// @ref ?Add@?$DynamicVectorClass@PAVUnitTypeClass@@@@QAE_NABQAVUnitTypeClass@@@Z=004fafc0
// @ref ?Clear@?$DynamicVectorClass@PAVUnitTypeClass@@@@UAEXXZ=004fa5d0
// @ref ?Clear@?$VectorClass@PAVUnitTypeClass@@@@UAEXXZ=004fadd0
// @ref ?Fetch@?$VectorClass@PAVUnitTypeClass@@@@UBEPAVUnitTypeClass@@H@Z=004fa620
// @ref ?ID@?$DynamicVectorClass@PAVUnitTypeClass@@@@UAEHABQAVUnitTypeClass@@@Z=004fb010
// @ref ?ID@?$DynamicVectorClass@PAVUnitTypeClass@@@@UAEHPBQAVUnitTypeClass@@@Z=004fa600
// @ref ?ID@?$VectorClass@PAVUnitTypeClass@@@@UAEHABQAVUnitTypeClass@@@Z=004fae00
// @ref ?ID@?$VectorClass@PAVUnitTypeClass@@@@UAEHPBQAVUnitTypeClass@@@Z=004fae40
// @ref ?Resize@?$DynamicVectorClass@PAVUnitTypeClass@@@@UAE_NHPBQAVUnitTypeClass@@@Z=004faf10
// @ref ?Resize@?$VectorClass@PAVUnitTypeClass@@@@UAE_NHPBQAVUnitTypeClass@@@Z=004fad20
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class UnitTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<UnitTypeClass *>::DynamicVectorClass(unsigned size, UnitTypeClass * const *array)
    : VectorClass<UnitTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
