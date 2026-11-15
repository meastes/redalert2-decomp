// VectorClass<HSVClass>::VectorClass(unsigned, HSVClass const *) (0x00605DA0): the out-of-line copy of the palette unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@VHSVClass@@@@UBE_NABV0@@Z=006051a0
// @ref ??_7?$VectorClass@VHSVClass@@@@6B@=007a7bf8
// @ref ??_E?$VectorClass@VHSVClass@@@@UAEPAXI@Z=00605710
// @ref ??_G?$VectorClass@VHSVClass@@@@UAEPAXI@Z=00605710
// @ref ?Clear@?$VectorClass@VHSVClass@@@@UAEXXZ=00605390
// @ref ?Fetch@?$VectorClass@VHSVClass@@@@UBE?AVHSVClass@@H@Z=00604d90
// @ref ?ID@?$VectorClass@VHSVClass@@@@UAEHABVHSVClass@@@Z=006053c0
// @ref ?ID@?$VectorClass@VHSVClass@@@@UAEHPBVHSVClass@@@Z=00605420
// @ref ?Resize@?$VectorClass@VHSVClass@@@@UAE_NHPBVHSVClass@@@Z=00605290
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/hsv_class.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<HSVClass>::VectorClass(unsigned size, HSVClass const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) HSVClass[size];
        } else {
            Vector = new HSVClass[size];
            IsAllocated = true;
        }
    }
}
