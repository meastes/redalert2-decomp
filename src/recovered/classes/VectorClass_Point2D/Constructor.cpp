// VectorClass<Point2D>::VectorClass(unsigned, Point2D const *) (0x00635090): the out-of-line copy of the radar unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
// @ref ??2@YAPAXI@Z=00782017
// @ref ??8?$VectorClass@UPoint2D@@@@UBE_NABV0@@Z=005b1930
// @ref ??_7?$VectorClass@UPoint2D@@@@6B@=007a6fc8
// @ref ??_E?$VectorClass@UPoint2D@@@@UAEPAXI@Z=005b2800
// @ref ??_G?$VectorClass@UPoint2D@@@@UAEPAXI@Z=005b2800
// @ref ?Clear@?$VectorClass@UPoint2D@@@@UAEXXZ=005b1a60
// @ref ?Fetch@?$VectorClass@UPoint2D@@@@UBE?AUPoint2D@@H@Z=005b1730
// @ref ?ID@?$VectorClass@UPoint2D@@@@UAEHABUPoint2D@@@Z=005b1a90
// @ref ?ID@?$VectorClass@UPoint2D@@@@UAEHPBUPoint2D@@@Z=005b1ae0
// @ref ?Resize@?$VectorClass@UPoint2D@@@@UAE_NHPBUPoint2D@@@Z=005b1990
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/rect.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<Point2D>::VectorClass(unsigned size, Point2D const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) Point2D[size];
        } else {
            Vector = new Point2D[size];
            IsAllocated = true;
        }
    }
}
