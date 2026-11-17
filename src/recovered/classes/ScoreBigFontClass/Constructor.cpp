// ScoreBigFontClass::ScoreBigFontClass(ConvertClass *drawer) (0x00667E80)
// The BIGFONT.SHP sheet: from the mixfiles, or else from a loose file (then owned).
#include "shared/score_font.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/palette.hpp"
#include "shared/debug_printf.hpp"

ScoreBigFontClass::ScoreBigFontClass(ConvertClass *drawer)
{
    IsAllocated = false;
    Image = (ShapeFileStruct const *)MixFileClass::Retrieve("BIGFONT.SHP");
    if (!Image) {
        CCFileClass file("BIGFONT.SHP");
        Image = (ShapeFileStruct const *)Load_Alloc_Data(file);
        IsAllocated = true;
        file.Close();
        Debug_Printf("ScoreScreen: Loaded BIGFONT.SHP\n");
    }
    Width = 20;
    Drawer = drawer;
    Height = 35;
}
