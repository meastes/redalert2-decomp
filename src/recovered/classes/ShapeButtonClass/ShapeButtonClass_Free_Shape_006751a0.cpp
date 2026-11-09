// void ShapeButtonClass::Free_Shape() (0x006751A0)
// Frees a shape the button owns and zeroes the button's size.
#include "shared/gadget.hpp"

void ShapeButtonClass::Free_Shape()
{
    if (IsShapeAllocated && Shape != 0) {
        operator delete((void *)Shape);
        Shape = 0;
        IsShapeAllocated = false;
        Width = 0;
        Height = 0;
    }
}
