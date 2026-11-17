// VectorClass<HashObject<unsigned int, unsigned int> >::VectorClass(unsigned, HashObject<unsigned int, unsigned int>  const *) (0x0056CDD0): the out-of-line copy of the map unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@U?$HashObject@II@@@@UBE_NABV0@@Z=0056bef0
// @ref ??_7?$VectorClass@U?$HashObject@II@@@@6B@=007a5af8
// @ref ??_E?$VectorClass@U?$HashObject@II@@@@UAEPAXI@Z=0056c910
// @ref ??_G?$VectorClass@U?$HashObject@II@@@@UAEPAXI@Z=0056c910
// @ref ?Clear@?$VectorClass@U?$HashObject@II@@@@UAEXXZ=0056c590
// @ref ?Fetch@?$VectorClass@U?$HashObject@II@@@@UBE?AU?$HashObject@II@@H@Z=0056ad90
// @ref ?ID@?$VectorClass@U?$HashObject@II@@@@UAEHABU?$HashObject@II@@@Z=0056c5c0
// @ref ?ID@?$VectorClass@U?$HashObject@II@@@@UAEHPBU?$HashObject@II@@@Z=0056c600
// @ref ?Resize@?$VectorClass@U?$HashObject@II@@@@UAE_NHPBU?$HashObject@II@@@Z=0056c4c0
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/hash_object.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<HashObject<unsigned int, unsigned int> >::VectorClass(unsigned size, HashObject<unsigned int, unsigned int>  const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) HashObject<unsigned int, unsigned int> [size];
        } else {
            Vector = new HashObject<unsigned int, unsigned int> [size];
            IsAllocated = true;
        }
    }
}
