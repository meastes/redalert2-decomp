// VectorClass<TechnoClass *>::VectorClass(unsigned, TechnoClass * const *) (0x0040BC10): the out-of-line
// copy (shared/vector_class.hpp). The vtable and the scalar deleting destructor it emits are the
// copies at their recorded addresses (0x0079A7FC; ??_G 0x0040BB70).
// @ref ??2@YAPAXI@Z=00782017
// @ref ??_7?$VectorClass@PAVTechnoClass@@@@6B@=0079a7fc
// @ref ??_E?$VectorClass@PAVTechnoClass@@@@UAEPAXI@Z=0040bb70
// @ref ??_G?$VectorClass@PAVTechnoClass@@@@UAEPAXI@Z=0040bb70
// @ref ??8?$VectorClass@PAVTechnoClass@@@@UBE_NABV0@@Z=0040b870
// @ref ?Resize@?$VectorClass@PAVTechnoClass@@@@UAE_NHPBQAVTechnoClass@@@Z=0040b8b0
// @ref ?Clear@?$VectorClass@PAVTechnoClass@@@@UAEXXZ=0040b960
// @ref ?ID@?$VectorClass@PAVTechnoClass@@@@UAEHPBQAVTechnoClass@@@Z=0040b9d0
// @ref ?ID@?$VectorClass@PAVTechnoClass@@@@UAEHABQAVTechnoClass@@@Z=0040b990
// @ref ?Fetch@?$VectorClass@PAVTechnoClass@@@@UBEPAVTechnoClass@@H@Z=0040b600
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"

class TechnoClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<TechnoClass *>::VectorClass(unsigned size, TechnoClass * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) TechnoClass *[size];
        } else {
            Vector = new TechnoClass *[size];
            IsAllocated = true;
        }
    }
}
