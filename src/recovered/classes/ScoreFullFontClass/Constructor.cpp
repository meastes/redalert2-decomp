// ScoreFullFontClass::ScoreFullFontClass(ConvertClass *drawer) (0x00667DC0)
// The FULLFNT3.SHP sheet: from the mixfiles, or else from a loose file (then owned).
#include "shared/score_font.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/palette.hpp"
#include "shared/debug_printf.hpp"

ScoreFullFontClass::ScoreFullFontClass(ConvertClass *drawer)
{
    IsAllocated = false;
    Image = (ShapeFileStruct const *)MixFileClass::Retrieve("FULLFNT3.SHP");
    if (!Image) {
        CCFileClass file("FULLFNT3.SHP");
        Image = (ShapeFileStruct const *)Load_Alloc_Data(file);
        IsAllocated = true;
        file.Close();
        Debug_Printf("ScoreScreen: Loaded FULLFNT3.SHP\n");
    }
    Width = 11;
    Drawer = drawer;
    Height = 21;
}
