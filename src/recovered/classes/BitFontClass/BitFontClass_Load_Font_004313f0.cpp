// bool BitFontClass::Load_Font(char const *filename) (0x004313F0)
// Takes the font file's glyph data and metrics; false when it cannot be loaded.
#include "shared/text_print.hpp"
// @ref ?Load_Font_Data@BitFontClass@@SIPAUFontDataType@@PBD@Z=00431150
// @ref ?Make_Missing_Char@BitFontClass@@QAEXXZ=00431ec0

bool BitFontClass::Load_Font(char const *filename)
{
    Data = Load_Font_Data(filename);
    if (Data) {
        Width = Data->Width;
        Height = Data->Height;
        Make_Missing_Char();
        return true;
    }
    return false;
}
