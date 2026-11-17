// VectorClass<ScoutStruct>::VectorClass(unsigned, ScoutStruct const *) (0x004F8B80): the out-of-line copy of the house unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@UScoutStruct@@@@UBE_NABV0@@Z=004f7650
// @ref ??_7?$VectorClass@UScoutStruct@@@@6B@=007a2ea4
// @ref ??_G?$VectorClass@UScoutStruct@@@@UAEPAXI@Z=004f85d0
// @ref ?Clear@?$VectorClass@UScoutStruct@@@@UAEXXZ=004f77a0
// @ref ?Fetch@?$VectorClass@UScoutStruct@@@@UBE?AUScoutStruct@@H@Z=004f6d20
// @ref ?ID@?$VectorClass@UScoutStruct@@@@UAEHABUScoutStruct@@@Z=004f77d0
// @ref ?ID@?$VectorClass@UScoutStruct@@@@UAEHPBUScoutStruct@@@Z=004f7820
// @ref ?Resize@?$VectorClass@UScoutStruct@@@@UAE_NHPBUScoutStruct@@@Z=004f76b0
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/house_nodes.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<ScoutStruct>::VectorClass(unsigned size, ScoutStruct const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) ScoutStruct[size];
        } else {
            Vector = new ScoutStruct[size];
            IsAllocated = true;
        }
    }
}
