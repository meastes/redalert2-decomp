// ScoreFontClass::ScoreFontClass() (0x00667840)
// The unit's shape helpers fall back on Score.cpp's empty rectangle.
#define SHAPE_FILE_EMPTY_RECT unknown_score_empty_rect_ab6f20
#include "shared/score_font.hpp"

ScoreFontClass::ScoreFontClass() :
    Width(0),
    Height(0),
    Image(0),
    Drawer(0)
{
    Load_Sounds();
    score_font_count++;
}
