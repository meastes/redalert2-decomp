// DynamicVectorClass<int>::DynamicVectorClass(unsigned, int const *) (0x0046D1F0): the out-of-line copy of the ccini unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@H@@UBE_NABV0@@Z=0046d070
// @ref ??_7?$DynamicVectorClass@H@@6B@=0079da94
// @ref ??_7?$VectorClass@H@@6B@=0079d9d4
// @ref ??_E?$DynamicVectorClass@H@@UAEPAXI@Z=0046cf50
// @ref ??_E?$VectorClass@H@@UAEPAXI@Z=0046da70
// @ref ??_G?$DynamicVectorClass@H@@UAEPAXI@Z=0046cf50
// @ref ??_G?$VectorClass@H@@UAEPAXI@Z=0046da70
// @ref ?Add@?$DynamicVectorClass@H@@QAE_NABH@Z=0046d300
// @ref ?Clear@?$DynamicVectorClass@H@@UAEXXZ=0046cc80
// @ref ?Clear@?$VectorClass@H@@UAEXXZ=0046d160
// @ref ?Fetch@?$VectorClass@H@@UBEHH@Z=0046ccd0
// @ref ?ID@?$DynamicVectorClass@H@@UAEHABH@Z=0046d350
// @ref ?ID@?$DynamicVectorClass@H@@UAEHPBH@Z=0046ccb0
// @ref ?ID@?$VectorClass@H@@UAEHABH@Z=0046d190
// @ref ?ID@?$VectorClass@H@@UAEHPBH@Z=0046d1d0
// @ref ?Resize@?$DynamicVectorClass@H@@UAE_NHPBH@Z=0046d250
// @ref ?Resize@?$VectorClass@H@@UAE_NHPBH@Z=0046d0b0
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<int>::DynamicVectorClass(unsigned size, int const *array)
    : VectorClass<int>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
