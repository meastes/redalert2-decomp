// DynamicVectorClass<VoxelAnimTypeClass *>::DynamicVectorClass(unsigned, VoxelAnimTypeClass * const *) (0x00653340): the out-of-line copy of the rules unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@PAVVoxelAnimTypeClass@@@@UBE_NABV0@@Z=006531c0
// @ref ??_7?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@6B@=007a9184
// @ref ??_7?$VectorClass@PAVVoxelAnimTypeClass@@@@6B@=007a9118
// @ref ??_E?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAEPAXI@Z=006543e0
// @ref ??_E?$VectorClass@PAVVoxelAnimTypeClass@@@@UAEPAXI@Z=00654390
// @ref ??_G?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAEPAXI@Z=006543e0
// @ref ??_G?$VectorClass@PAVVoxelAnimTypeClass@@@@UAEPAXI@Z=00654390
// @ref ?Add@?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@QAE_NABQAVVoxelAnimTypeClass@@@Z=00653450
// @ref ?Clear@?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAEXXZ=00652af0
// @ref ?Clear@?$VectorClass@PAVVoxelAnimTypeClass@@@@UAEXXZ=006532b0
// @ref ?Fetch@?$VectorClass@PAVVoxelAnimTypeClass@@@@UBEPAVVoxelAnimTypeClass@@H@Z=00652b40
// @ref ?ID@?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAEHABQAVVoxelAnimTypeClass@@@Z=006534a0
// @ref ?ID@?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAEHPBQAVVoxelAnimTypeClass@@@Z=00652b20
// @ref ?ID@?$VectorClass@PAVVoxelAnimTypeClass@@@@UAEHABQAVVoxelAnimTypeClass@@@Z=006532e0
// @ref ?ID@?$VectorClass@PAVVoxelAnimTypeClass@@@@UAEHPBQAVVoxelAnimTypeClass@@@Z=00653320
// @ref ?Resize@?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@UAE_NHPBQAVVoxelAnimTypeClass@@@Z=006533a0
// @ref ?Resize@?$VectorClass@PAVVoxelAnimTypeClass@@@@UAE_NHPBQAVVoxelAnimTypeClass@@@Z=00653200
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class VoxelAnimTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<VoxelAnimTypeClass *>::DynamicVectorClass(unsigned size, VoxelAnimTypeClass * const *array)
    : VectorClass<VoxelAnimTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
