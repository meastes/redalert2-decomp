// DynamicVectorClass<InfantryTypeClass *>::DynamicVectorClass(unsigned, InfantryTypeClass * const *) (0x004FAA90): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@PAVInfantryTypeClass@@@@UBE_NABV0@@Z=004fa910
// @ref ??_7?$DynamicVectorClass@PAVInfantryTypeClass@@@@6B@=007a3178
// @ref ??_7?$VectorClass@PAVInfantryTypeClass@@@@6B@=007a3118
// @ref ??_E?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAEPAXI@Z=004faaf0
// @ref ??_E?$VectorClass@PAVInfantryTypeClass@@@@UAEPAXI@Z=004fa7b0
// @ref ??_G?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAEPAXI@Z=004faaf0
// @ref ??_G?$VectorClass@PAVInfantryTypeClass@@@@UAEPAXI@Z=004fa7b0
// @ref ?Add@?$DynamicVectorClass@PAVInfantryTypeClass@@@@QAE_NABQAVInfantryTypeClass@@@Z=004fabf0
// @ref ?Clear@?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAEXXZ=004fa570
// @ref ?Clear@?$VectorClass@PAVInfantryTypeClass@@@@UAEXXZ=004faa00
// @ref ?Fetch@?$VectorClass@PAVInfantryTypeClass@@@@UBEPAVInfantryTypeClass@@H@Z=004fa5c0
// @ref ?ID@?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAEHABQAVInfantryTypeClass@@@Z=004fac40
// @ref ?ID@?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAEHPBQAVInfantryTypeClass@@@Z=004fa5a0
// @ref ?ID@?$VectorClass@PAVInfantryTypeClass@@@@UAEHABQAVInfantryTypeClass@@@Z=004faa30
// @ref ?ID@?$VectorClass@PAVInfantryTypeClass@@@@UAEHPBQAVInfantryTypeClass@@@Z=004faa70
// @ref ?Resize@?$DynamicVectorClass@PAVInfantryTypeClass@@@@UAE_NHPBQAVInfantryTypeClass@@@Z=004fab40
// @ref ?Resize@?$VectorClass@PAVInfantryTypeClass@@@@UAE_NHPBQAVInfantryTypeClass@@@Z=004fa950
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class InfantryTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<InfantryTypeClass *>::DynamicVectorClass(unsigned size, InfantryTypeClass * const *array)
    : VectorClass<InfantryTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
