#pragma auto_inline(off)
VoxelPaletteLibrary::~VoxelPaletteLibrary()
{
    if (ColorsAllocated) {
        operator delete(Colors);
    }
    if (LUTAllocated) {
        operator delete(LUT);
    }
}

#pragma auto_inline(on)
