// void __fastcall rgb32_to_rgb24_clamped(unsigned char *palette, int count) -- 0x004E12D0
// Clears the three low bits of the three colour components of `count` four-byte pixels.
void __fastcall rgb32_to_rgb24_clamped_004e12d0(unsigned char *palette, int count)
{
    for (int index = count; index > 0; index--) {
        *palette++ &= 0xf8;
        *palette++ &= 0xf8;
        *palette++ &= 0xf8;
        palette++;
    }
}
