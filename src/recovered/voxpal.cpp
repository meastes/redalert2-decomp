/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/voxpal.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/voxpal.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/voxpal.cpp where both functions declare the same local types (names only).

// voxpal.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voxpal.json carries the addresses and the rows that
// stay in their own files.


#pragma auto_inline(off)
/// <summary>
/// Creates a voxel palette library.
/// The library holds the palette every voxel is colored from together with the tables the
/// drawing routines shade with. The caller may hand over its own buffers -- the voxel draw
/// system does exactly that, so the loaded data lands straight in the globals the drawing
/// routines read -- or leave them NULL and let the library find its own storage.
/// </summary>
/// <param name="rgb">Pointer to the palette buffer to use, or NULL to allocate one.</param>
/// <param name="lut">Pointer to the shading table buffer to use, or NULL to allocate
/// one.</param>
VoxelPaletteLibrary::VoxelPaletteLibrary(unsigned char *rgb, unsigned char *lut)
{
    if (rgb == 0) {
        Colors = (unsigned char *)operator new(0x300);
        ColorsAllocated = 1;
    } else {
        Colors = rgb;
        ColorsAllocated = 0;
    }
    if (lut == 0) {
        LUT = (unsigned char *)operator new(0x8000);
        LUTAllocated = 1;
    } else {
        LUT = lut;
        LUTAllocated = 0;
    }
}

#pragma auto_inline(on)


#pragma auto_inline(off)
/// <summary>
/// Destroys the palette library.
/// Only the buffers this library allocated for itself are freed; any that the creator
/// supplied are left alone.
/// </summary>
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


/// <summary>
/// Reads just the palette from a file.
/// Use this routine when the colors alone are wanted and the header and shading tables are
/// of no interest.
/// </summary>
/// <returns>Returns with zero if the palette was read, or one if it could not be.</returns>
int VoxelPaletteLibrary::Read_Colors(FileClass *file)
{
    int result = 0;
    file->Open(1);
    if (file->Read(Colors, 0x300) != 0x300) {
        result = 1;
    }
    file->Close();
    return result;
}


/// <summary>
/// Reads a complete palette library from a file.
/// This routine loads the header, the palette, and the prebuilt shading tables, which is
/// everything the voxel drawing routines need to color and light a model.
/// </summary>
/// <returns>Returns with zero if the library was read, or one if it could not be.</returns>
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


/// <summary>
/// Writes this palette library out to a file.
/// This is the counterpart of Read_File and produces a VPL the game can load again --
/// header, palette, and shading tables.
/// </summary>
/// <returns>Returns with zero if the library was written, or one if it could not be.</returns>
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


/// <summary>
/// Builds the shading lookup tables for this palette.
/// This routine is used when a voxel object carries its own palette rather than the one out
/// of VOXELS.VPL. Every palette color is dimmed and brightened through the scale ramp and
/// matched back onto the nearest palette entry, so the drawing routines can shade a voxel by
/// table lookup instead of by color arithmetic.
/// </summary>
/// <param name="scale">The ramp of brightness scales to build with, or NULL to use the
/// built in ramp.</param>
/// <param name="lut_count">The number of brightness steps in the ramp supplied.</param>
// Builds the lookup: for each section a brightness level (from `levels`, or
// 32 default levels, 0.6..1.35 and 1.4..3.275, built in place
// in unknown_levels_af8f10) scales every palette color
// 1..255, and the nearest color of the same remap kind is stored. The upper
// 16 default sections clamp each channel to 255.
void VoxelPaletteLibrary::Calculate_Lookup_Table(float *scale, int lut_count)
{
    int index;
    int entry;
    int pos;
    static const float _addend = 0.6f;     // 0x007AEA48 (a const static, not a merged literal)
    float *shades;
    if (scale && lut_count) {
        Header.unknown_08 = lut_count;
        shades = scale;
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


/// <summary>
/// Finds the palette entry closest to the color specified.
/// This routine searches the whole voxel palette, house colors included, for the entry
/// nearest the color given.
/// </summary>
/// <returns>Returns with the index of the closest color in the palette.</returns>
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


/// <summary>
/// Finds the palette entry closest to the color specified, respecting the remap band.
/// This routine is used while the shading tables are built. The house color remap range is
/// kept apart from the rest of the palette, so that a remappable color never shades into a
/// fixed one and a fixed color never strays into the house colors.
/// </summary>
/// <param name="is_remap">Should the search be confined to the house color remap range?</param>
/// <returns>Returns with the index of the closest color on the side of the palette asked
/// for.</returns>
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
