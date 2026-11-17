// DynamicVectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *>>::DynamicVectorClass(unsigned, T const *) (0x004CE710).
// The generic body of shared/vector_class.hpp written as an explicit specialization (VC6 cannot
// explicitly instantiate a constructor alone, and the whole-class instantiation would emit an
// out-of-line ~DynamicVectorClass the original does not have). The VectorClass and
// DynamicVectorClass vtables (0x007A17D4, 0x007A17FC) come with it, with their scalar deleting destructors (the next rows).
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/discrete_distribution.hpp"
class CellClass;

template <>
DynamicVectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> >::DynamicVectorClass(unsigned size, DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> const *array)
    : VectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *> >(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
