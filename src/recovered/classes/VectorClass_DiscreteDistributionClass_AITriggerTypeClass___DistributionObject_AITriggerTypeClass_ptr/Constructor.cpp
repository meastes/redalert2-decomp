// VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>>::VectorClass(unsigned, T const *) (0x006C09A0).
// The generic body of shared/vector_class.hpp written as an explicit specialization (VC6 cannot
// explicitly instantiate a constructor alone, and the whole-class instantiation would emit an
// out-of-line ~VectorClass the original does not have). Its vtable 0x007ACA60 comes with it, with its scalar deleting destructor 0x006C0830 (the next row).
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/discrete_distribution.hpp"
class AITriggerTypeClass;

template <>
VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *> >::VectorClass(unsigned size, DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *> const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>[size];
        } else {
            Vector = new DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTypeClass *>[size];
            IsAllocated = true;
        }
    }
}
