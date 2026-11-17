// Integer vector length through the original table square root.
#include "shared/rect.hpp"
#include "shared/fast_math.hpp"
int Point2D::Length() const
{
    return (int)Fast_Sqrt((double)X * X + (double)Y * Y);
}
