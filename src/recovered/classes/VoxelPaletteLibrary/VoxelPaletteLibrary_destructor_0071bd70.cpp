#pragma auto_inline(off)
VoxelPaletteLibrary::~VoxelPaletteLibrary()
{
    if (unknown_18) {
        operator delete(Palette);
    }
    if (unknown_1c) {
        operator delete(unknown_14);
    }
}

#pragma auto_inline(on)
