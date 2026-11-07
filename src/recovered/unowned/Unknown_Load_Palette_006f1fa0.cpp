// void Unknown_Load_Palette_006F1FA0(char const *name, PaletteClass **palette, ConvertClass **drawer) (0x006F1FA0)
// Loads a 768-byte 6-bit palette file into a new palette (each component times four),
// releases the file data when it was allocated, and makes a converter for the palette
// on the 0x00839958 background surface (0 when the allocation fails).
// @ref ?Unknown_Load_Shape_00495270@@YIPBXPBDAA_N@Z=00495270
// @ref ?unknown_surface_839958@@3PAVSurface@@A=00839958
#include "shared/convert.hpp"

class Surface;
void const *__fastcall Unknown_Load_Shape_00495270(char const *filename, bool &allocated);
extern Surface *unknown_surface_839958;

void __fastcall Unknown_Load_Palette_006F1FA0(char const *name, PaletteClass **palette, ConvertClass **drawer)
{
    bool allocated = false;
    unsigned char const *data = (unsigned char const *)Unknown_Load_Shape_00495270(name, allocated);
    *palette = new PaletteClass;
    for (int index = 0; index < 256; index++) {
        int offset = index * 3;
        RGBClass color(data[offset] << 2, data[offset + 1] << 2, data[offset + 2] << 2);
        (**palette)[index] = color;
    }
    if (allocated) {
        delete (void *)data;
    }
    *drawer = new ConvertClass(**palette, **palette, unknown_surface_839958, 1, false);
}
