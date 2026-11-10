// void BinkClass::Set_Position(int x, int y) (0x00430280)
// Places the movie at x, y, clipped to the target surface.
#include "shared/movie.hpp"

void BinkClass::Set_Position(int x, int y)
{
    Area = Intersect(Target->Get_Rect(), Rect(x, y, Handle->Width, Handle->Height), 0, 0);
}
