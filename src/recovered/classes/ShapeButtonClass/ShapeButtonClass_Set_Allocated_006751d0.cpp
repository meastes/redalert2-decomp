// void ShapeButtonClass::Set_Allocated(bool allocated) (0x006751D0)
// Whether the button owns (and frees) its shape.
#include "shared/gadget.hpp"

void ShapeButtonClass::Set_Allocated(bool allocated)
{
    IsShapeAllocated = allocated;
}
