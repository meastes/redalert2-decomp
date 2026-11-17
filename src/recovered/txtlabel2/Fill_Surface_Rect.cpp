// void __fastcall Fill_Surface_Rect_006f3a20(Surface *surface, Rect const &window, bool arg3) -- 0x006F3A20
// Runs 0x006F3650, then 0x006F3930 with the drawer at 0x00AC1188 and the shapes at 0x00AC1128
// and 0x00AC1010 (hypothesis).
#include "shared/rect.hpp"
class Surface;
class ConvertClass;
struct ShapeFileStruct;
void __fastcall Unknown_Draw_Interface_006F3650(Surface *surface, Rect const &window, bool arg3);
void __fastcall Unknown_Draw_Clipped_Shape_006F3930(Surface *surface, Rect const &window, ConvertClass *drawer, ShapeFileStruct const *shape640, ShapeFileStruct const *shape);
extern ConvertClass *unknown_drawer_ac1188;
extern ShapeFileStruct *unknown_shape_ac1128;
extern ShapeFileStruct *unknown_shape_ac1010;

void __fastcall Fill_Surface_Rect_006f3a20(Surface *surface, Rect const &window, bool arg3)
{
    Unknown_Draw_Interface_006F3650(surface, window, arg3);
    Unknown_Draw_Clipped_Shape_006F3930(surface, window, unknown_drawer_ac1188, unknown_shape_ac1128, unknown_shape_ac1010);
}
