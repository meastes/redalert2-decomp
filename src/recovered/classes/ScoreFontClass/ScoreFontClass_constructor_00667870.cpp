// ScoreFontClass::ScoreFontClass(int width, int height, ShapeFileStruct const *image, ConvertClass *drawer) (0x00667870)
#include "shared/score_font.hpp"
// @ref ??_7ScoreFontClass@@6B@=007a92a0
// @ref ?score_font_count@@3HA=00ab6fa4
// @ref ?Load_Sounds@ScoreFontClass@@QAEXXZ=00667900

ScoreFontClass::ScoreFontClass(int width, int height, ShapeFileStruct const *image, ConvertClass *drawer) :
    Width(width),
    Height(height),
    Image(image),
    Drawer(drawer)
{
    Load_Sounds();
    score_font_count++;
}
