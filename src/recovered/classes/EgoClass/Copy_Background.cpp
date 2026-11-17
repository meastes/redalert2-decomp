#include "shared/ego.hpp"
#include "shared/text_print.hpp"
extern int VideoModeWidth;
void EgoClass::Copy_Background(Surface *source)
{
    BitFontClass *font = Font_For_Style(TextFlags);
    font->String_Pixel_Width(Text, 0);
    Rect area(0, Y, VideoModeWidth, 18);
    LogicalSurface->Copy_From(area, *source, area, false, true);
}
