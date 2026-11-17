// ScoreFontClass::ScoreFontClass(int width, int height, ShapeFileStruct const *image, ConvertClass *drawer) (0x00667870)
#include "shared/score_font.hpp"

ScoreFontClass::ScoreFontClass(int width, int height, ShapeFileStruct const *image, ConvertClass *drawer) :
    Width(width),
    Height(height),
    Image(image),
    Drawer(drawer)
{
    Load_Sounds();
    score_font_count++;
}
