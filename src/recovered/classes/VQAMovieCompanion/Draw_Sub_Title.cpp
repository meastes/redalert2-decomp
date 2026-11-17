// void VQAMovieCompanion::Draw_Sub_Title(SubTitle const *subtitle) (0x0069B500): clears the last caption
// and prints this one on its line of the surface.
void VQAMovieCompanion::Draw_Sub_Title(SubTitle const *subtitle)
{
    if (unknown_10 != 0) {
        unknown_10->Fill_Rect(unknown_14, 0);
        BitFontClass *font = BitFont;
        unsigned long rgb = subtitle->Get_RGB_Color();
        font->Set_Color(Build_Hicolor_Pixel((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF));
        int width = unknown_10->Get_Width();
        int height = unknown_10->Get_Height();
        tagRECT clip = {0, 0, width, height};
        font->Set_Clip_Box(&clip);
        unsigned short *text = subtitle->Get_Text();
        int textwidth;
        int textheight;
        font->Get_Text_Area(text, &textwidth, &textheight, width);
        int x = 0;
        int y = (subtitle->Get_Line_Position() - 1) * font->Height;
        int alignment = subtitle->Get_Alignment();
        if (alignment == 2) {
            x = (width - textwidth) / 2;
        } else if (alignment == 1) {
            x = width - textwidth;
        }
        BitText->Print_Box(font, unknown_10, text, x, y, width, height, 0, 0, 0);
        unknown_24 = unknown_14;
        unknown_14.Set(x, y, textwidth, textheight);
    }
}
