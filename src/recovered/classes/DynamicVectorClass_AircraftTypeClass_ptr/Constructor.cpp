// DynamicVectorClass<AircraftTypeClass *>::DynamicVectorClass(unsigned, AircraftTypeClass * const *) (0x004FB230): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@PAVAircraftTypeClass@@@@UBE_NABV0@@Z=004fb0b0
// @ref ??_7?$DynamicVectorClass@PAVAircraftTypeClass@@@@6B@=007a31b8
// @ref ??_7?$VectorClass@PAVAircraftTypeClass@@@@6B@=007a3158
// @ref ??_E?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAEPAXI@Z=004fb290
// @ref ??_E?$VectorClass@PAVAircraftTypeClass@@@@UAEPAXI@Z=004fa850
// @ref ??_G?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAEPAXI@Z=004fb290
// @ref ??_G?$VectorClass@PAVAircraftTypeClass@@@@UAEPAXI@Z=004fa850
// @ref ?Add@?$DynamicVectorClass@PAVAircraftTypeClass@@@@QAE_NABQAVAircraftTypeClass@@@Z=004fb390
// @ref ?Clear@?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAEXXZ=004fa660
// @ref ?Clear@?$VectorClass@PAVAircraftTypeClass@@@@UAEXXZ=004fb1a0
// @ref ?Fetch@?$VectorClass@PAVAircraftTypeClass@@@@UBEPAVAircraftTypeClass@@H@Z=004fa6b0
// @ref ?ID@?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAEHABQAVAircraftTypeClass@@@Z=004fb3e0
// @ref ?ID@?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAEHPBQAVAircraftTypeClass@@@Z=004fa690
// @ref ?ID@?$VectorClass@PAVAircraftTypeClass@@@@UAEHABQAVAircraftTypeClass@@@Z=004fb1d0
// @ref ?ID@?$VectorClass@PAVAircraftTypeClass@@@@UAEHPBQAVAircraftTypeClass@@@Z=004fb210
// @ref ?Resize@?$DynamicVectorClass@PAVAircraftTypeClass@@@@UAE_NHPBQAVAircraftTypeClass@@@Z=004fb2e0
// @ref ?Resize@?$VectorClass@PAVAircraftTypeClass@@@@UAE_NHPBQAVAircraftTypeClass@@@Z=004fb0f0
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class AircraftTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<AircraftTypeClass *>::DynamicVectorClass(unsigned size, AircraftTypeClass * const *array)
    : VectorClass<AircraftTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
