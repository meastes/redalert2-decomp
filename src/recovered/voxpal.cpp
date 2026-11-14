// voxpal.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voxpal.json carries the addresses and the rows that
// stay in their own files.


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


int VoxelPaletteLibrary::Read_Colors(FileClass *file)
{
    int failed = 0;
    file->Open(1);
    if (file->Read(Colors, 0x300) != 0x300) {
        failed = 1;
    }
    file->Close();
    return failed;
}


int VoxelPaletteLibrary::Read_File(FileClass *file)
{
    int failed = 0;
    UnknownVoxelPaletteHeader header;
    if (!file->Open(1)) {
        failed = 1;
    } else if (file->Read(&header, sizeof(header)) != sizeof(header)) {
        failed = 1;
    } else {
        Header.unknown_00 = header.unknown_00;
        Header.unknown_04 = header.unknown_04;
        Header.unknown_08 = header.unknown_08;
        Header.unknown_0c = header.unknown_0c;
        if (file->Read(Colors, 0x300) != 0x300) {
            failed = 1;
        } else if (file->Read(LUT, Header.unknown_08 << 8) != Header.unknown_08 << 8) {
            failed = 1;
        }
    }
    file->Close();
    return failed;
}


int VoxelPaletteLibrary::Write_File(FileClass *file)
{
    int failed = 0;
    UnknownVoxelPaletteHeader header;
    if (!file->Open(2)) {
        failed = 1;
    } else {
        header.unknown_00 = Header.unknown_00;
            header.unknown_04 = Header.unknown_04;
            header.unknown_08 = Header.unknown_08;
            header.unknown_0c = Header.unknown_0c;
        if (file->Write(&header, sizeof(header)) != sizeof(header)) {
            failed = 1;
        } else if (file->Write(Colors, 0x300) != 0x300) {
            failed = 1;
        } else if (file->Write(LUT, Header.unknown_08 << 8) != Header.unknown_08 << 8) {
            failed = 1;
        }
    }
    file->Close();
    return failed;
}


// Builds the lookup: for each section a brightness level (from `levels`, or
// 32 default levels, 0.6..1.35 and 1.4..3.275, built in place
// in unknown_levels_af8f10) scales every palette color
// 1..255, and the nearest color of the same remap kind is stored. The upper
// 16 default sections clamp each channel to 255.
void VoxelPaletteLibrary::Calculate_Lookup_Table(float *levels, int count)
{
    int index;
    int entry;
    int pos;
    static const float _addend = 0.6f;     // 0x007AEA48 (a const static, not a merged literal)
    float *shades;
    if (levels && count) {
        Header.unknown_08 = count;
        shades = levels;
    } else {
        shades = unknown_levels_af8f10;
        Header.unknown_08 = 32;
        for (index = 0; index < 16; index++) {
            unknown_levels_af8f10[index] = (index * (1.0f / 16)) * 0.8f + _addend;
        }
        for (index = 16; index < 32; index++) {
            unknown_levels_af8f10[index] = (index - 16) / 16.0f + index / 16.0f + 0.4f;
        }
    }
    bool remap;
    remap = false;
    for (entry = 0; entry < 16; entry++) {
        for (index = 1; index < 256; index++) {
            remap = index >= Header.unknown_00 && index <= Header.unknown_04;
            float red = Colors[index * 3];
            red *= shades[entry];
            float green = Colors[index * 3 + 1];
            green *= shades[entry];
            float blue = Colors[index * 3 + 2];
            blue *= shades[entry];
            pos = (entry << 8) | index;
            LUT[pos] = Closest_Color(red, green, blue, remap);
        }
    }
    remap = false;
    for (entry = 16; entry < 32; entry++) {
        for (index = 1; index < 256; index++) {
            remap = index >= Header.unknown_00 && index <= Header.unknown_04;
            float red = Colors[index * 3];
            red *= shades[entry];
            float green = Colors[index * 3 + 1];
            green *= shades[entry];
            float blue = Colors[index * 3 + 2];
            blue *= shades[entry];
            red = red <= 255 ? red : 255;
            green = green <= 255 ? green : 255;
            blue = blue <= 255 ? blue : 255;
            pos = (entry << 8) | index;
            LUT[pos] = Closest_Color(red, green, blue, remap);
        }
    }
}


// The index of the palette entry nearest to a color (squared distance in
// float; the search starts at a distance of 100000).
unsigned char VoxelPaletteLibrary::Closest_Color(float red, float green, float blue)
{
    unsigned char *entry = Colors;
    int best = 0;
    float best_distance = 100000.0f;
    for (int index = 0; index < 256; index++) {
        float dx = red - Colors[index * 3];
        float dy = green - Colors[index * 3 + 1];
        float dz = blue - Colors[index * 3 + 2];
        float distance = dx * dx + dy * dy + dz * dz;
        if (distance < best_distance) {
            best = index;
            best_distance = distance;
        }
    }
    return best;
}


// The nearest palette entry (1..255) inside the remap range when `remap`,
// outside it otherwise.
unsigned char VoxelPaletteLibrary::Closest_Color(float red, float green, float blue, bool remap)
{
    int best = remap ? Header.unknown_00 : 1;
    float best_distance = 100000.0f;
    for (int index = 1; index < 256; index++) {
        if (remap) {
            if (index < Header.unknown_00 || index > Header.unknown_04) {
                continue;
            }
        } else {
            if (index >= Header.unknown_00 && index <= Header.unknown_04) {
                continue;
            }
        }
        float dx = red - Colors[index * 3];
        float dy = green - Colors[index * 3 + 1];
        float dz = blue - Colors[index * 3 + 2];
        float distance = dx * dx + dy * dy + dz * dz;
        if (distance < best_distance) {
            best_distance = distance;
            best = index;
        }
    }
    return best;
}
