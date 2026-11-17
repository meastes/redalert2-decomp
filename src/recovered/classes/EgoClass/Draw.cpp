#include "shared/ego.hpp"
#include "shared/text_print.hpp"
extern int VideoModeWidth;
extern int VideoModeHeight;
extern Surface *unknown_surface_853118;
extern bool GameInFocus;
extern "C" unsigned int __cdecl wcslen(wchar_t const *text);
int __fastcall unknown_text_draw_005fe1b0(unsigned int color, BitFontClass *font,
    Rect const &area, wchar_t const *text, int length, int x_alignment,
    int y_alignment, Surface *surface, int unknown9);
void EgoClass::Draw(bool force)
{
    int y = Y;
    if (y < LogicalSurface->Get_Height()) {
        y = Y;
        if (y > LogicalSurface->Get_Height() - 52) goto draw;
    }
    if ((y < -16 || y > 32) && !force) return;
draw:
    Rect area((VideoModeWidth - 520) / 2, 0, 520, VideoModeHeight);
    area.Y = y;
    int alignment = 0;
    if (TextFlags & 0x100) alignment = 1;
    else if (TextFlags & 0x200) alignment = 4;
    if (GameInFocus)
        unknown_text_draw_005fe1b0(0x80ffff, 0, area, Text, wcslen(Text), alignment, 0, unknown_surface_853118, 0);
}
