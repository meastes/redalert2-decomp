// VectorClass<RGBClass>::VectorClass(unsigned, RGBClass const *) (0x0046DC90): the out-of-line copy of the ccini unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@VRGBClass@@@@UBE_NABV0@@Z=0046d720
// @ref ??_7?$VectorClass@VRGBClass@@@@6B@=0079da54
// @ref ??_E?$VectorClass@VRGBClass@@@@UAEPAXI@Z=0046db10
// @ref ??_G?$VectorClass@VRGBClass@@@@UAEPAXI@Z=0046db10
// @ref ?Clear@?$VectorClass@VRGBClass@@@@UAEXXZ=0046d880
// @ref ?Fetch@?$VectorClass@VRGBClass@@@@UBE?AVRGBClass@@H@Z=0046cda0
// @ref ?ID@?$VectorClass@VRGBClass@@@@UAEHABVRGBClass@@@Z=0046d8b0
// @ref ?ID@?$VectorClass@VRGBClass@@@@UAEHPBVRGBClass@@@Z=0046d900
// @ref ?Resize@?$VectorClass@VRGBClass@@@@UAE_NHPBVRGBClass@@@Z=0046d780
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/rgb_class.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<RGBClass>::VectorClass(unsigned size, RGBClass const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) RGBClass[size];
        } else {
            Vector = new RGBClass[size];
            IsAllocated = true;
        }
    }
}
