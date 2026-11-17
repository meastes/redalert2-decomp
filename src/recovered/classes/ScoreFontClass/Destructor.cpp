// ScoreFontClass::~ScoreFontClass() (0x006678B0)
// Frees an owned sheet; the last font forgets the typing sounds.
#include "shared/score_font.hpp"

ScoreFontClass::~ScoreFontClass()
{
    if (Image && IsAllocated == true) {
        delete (void *)Image;
        Image = 0;
        IsAllocated = false;
    }
    if (--score_font_count == 0) {
        for (int i = 0; i < 3; i++) {
            text_sounds[i].Data = 0;
            text_sounds[i].IsAllocated = false;
        }
    }
}
