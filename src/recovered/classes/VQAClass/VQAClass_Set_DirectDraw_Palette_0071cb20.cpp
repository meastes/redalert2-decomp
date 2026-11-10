// void VQAClass::Set_DirectDraw_Palette() (0x0071CB20): makes the movie palette current.
#include "shared/movie.hpp"
#include "shared/palette.hpp"
// @ref ?Set@PaletteClass@@QAEXXZ=00496120

void VQAClass::Set_DirectDraw_Palette()
{
    ((PaletteClass *)Palette)->Set();
}
