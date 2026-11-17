// DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DistributionObject<BuildingTypeClass *>>::ID (T const *) (0x004F6EA0): the member the reference tables bind at this address.
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/discrete_distribution.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_ID_POINTER
#include "shared/discrete_distribution.hpp"

class BuildingTypeClass;

template class DynamicVectorClass<DiscreteDistributionClass<BuildingTypeClass>::DistributionObject<BuildingTypeClass *> >;
