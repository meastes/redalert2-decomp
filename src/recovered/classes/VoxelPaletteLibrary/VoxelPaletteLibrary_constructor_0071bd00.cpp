#pragma auto_inline(off)
VoxelPaletteLibrary::VoxelPaletteLibrary(unsigned char *palette, unsigned char *table)
{
    if (palette == 0) {
        Colors = (unsigned char *)operator new(0x300);
        ColorsAllocated = 1;
    } else {
        Colors = palette;
        ColorsAllocated = 0;
    }
    if (table == 0) {
        LUT = (unsigned char *)operator new(0x8000);
        LUTAllocated = 1;
    } else {
        LUT = table;
        LUTAllocated = 0;
    }
}

#pragma auto_inline(on)
