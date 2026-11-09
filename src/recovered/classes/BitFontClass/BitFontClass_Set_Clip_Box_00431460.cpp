// void BitFontClass::Set_Clip_Box(tagRECT const *box) (0x00431460)
// Copies the clip box, or clears it without one.
#include "shared/text_print.hpp"

void BitFontClass::Set_Clip_Box(tagRECT const *box)
{
    if (box != 0) {
        ClipBox.left = box->left;
        ClipBox.top = box->top;
        ClipBox.right = box->right;
        ClipBox.bottom = box->bottom;
        return;
    }
    ClipBox.bottom = 0;
    ClipBox.right = 0;
    ClipBox.top = 0;
    ClipBox.left = 0;
}
