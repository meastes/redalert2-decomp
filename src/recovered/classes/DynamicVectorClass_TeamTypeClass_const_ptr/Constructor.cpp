// DynamicVectorClass<TeamTypeClass const *>::DynamicVectorClass(unsigned, TeamTypeClass const * const *) (0x006C03E0): the out-of-line copy of the teamtype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@PBVTeamTypeClass@@@@UBE_NABV0@@Z=004f7be0
// @ref ??_7?$DynamicVectorClass@PBVTeamTypeClass@@@@6B@=007a2fec
// @ref ??_7?$VectorClass@PBVTeamTypeClass@@@@6B@=007a2ee4
// @ref ??_E?$DynamicVectorClass@PBVTeamTypeClass@@@@UAEPAXI@Z=004f6fc0
// @ref ??_E?$VectorClass@PBVTeamTypeClass@@@@UAEPAXI@Z=004f8710
// @ref ??_G?$DynamicVectorClass@PBVTeamTypeClass@@@@UAEPAXI@Z=004f6fc0
// @ref ??_G?$VectorClass@PBVTeamTypeClass@@@@UAEPAXI@Z=004f8710
// @ref ?Clear@?$DynamicVectorClass@PBVTeamTypeClass@@@@UAEXXZ=004f6da0
// @ref ?Clear@?$VectorClass@PBVTeamTypeClass@@@@UAEXXZ=004f7cd0
// @ref ?Fetch@?$VectorClass@PBVTeamTypeClass@@@@UBEPBVTeamTypeClass@@H@Z=004f6df0
// @ref ?ID@?$DynamicVectorClass@PBVTeamTypeClass@@@@UAEHABQBVTeamTypeClass@@@Z=004f7e10
// @ref ?ID@?$DynamicVectorClass@PBVTeamTypeClass@@@@UAEHPBQBVTeamTypeClass@@@Z=004f6dd0
// @ref ?ID@?$VectorClass@PBVTeamTypeClass@@@@UAEHABQBVTeamTypeClass@@@Z=004f7d00
// @ref ?ID@?$VectorClass@PBVTeamTypeClass@@@@UAEHPBQBVTeamTypeClass@@@Z=004f7d40
// @ref ?Resize@?$DynamicVectorClass@PBVTeamTypeClass@@@@UAE_NHPBQBVTeamTypeClass@@@Z=004f7d60
// @ref ?Resize@?$VectorClass@PBVTeamTypeClass@@@@UAE_NHPBQBVTeamTypeClass@@@Z=004f7c20
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class TeamTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<TeamTypeClass const *>::DynamicVectorClass(unsigned size, TeamTypeClass const * const *array)
    : VectorClass<TeamTypeClass const *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
