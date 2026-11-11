// VoxelPaletteLibrary (TibSun voxlib.h; mischa85 names.csv agrees at every address): a .VPL voxel palette
// lookup (members 0x0071BD00-0x0071C314). A 0x10-byte header (remap range, section count),
// a 256-entry RGB palette (0x300 bytes) and section count * 256 bytes of
// lookup; each buffer is either supplied or owned.
#ifndef RA2_SHARED_UNKNOWN_VOXEL_PALETTE_HPP
#define RA2_SHARED_UNKNOWN_VOXEL_PALETTE_HPP

class FileClass;

struct UnknownVoxelPaletteHeader {
    int unknown_00;                     // +0x00 first remap index (hypothesis)
    int unknown_04;                     // +0x04 last remap index (hypothesis)
    int unknown_08;                     // +0x08 section count
    int unknown_0c;                     // +0x0C
};

class VoxelPaletteLibrary {
public:
    VoxelPaletteLibrary(unsigned char *palette, unsigned char *table);     // 0x0071BD00
    ~VoxelPaletteLibrary();                                     // 0x0071BD70
    int Read_Colors(FileClass *file);                           // 0x0071BDA0 (reads the palette; 1 on failure)
    int Read_File(FileClass *file);                             // 0x0071BDE0 (reads; 1 on failure)
    int Write_File(FileClass *file);                            // 0x0071BE80 (writes; 1 on failure)
    void Calculate_Lookup_Table(float *levels, int count);      // 0x0071BF20 (builds the lookup)
    unsigned char Closest_Color(float red, float green, float blue);    // 0x0071C1C0
    unsigned char Closest_Color(float red, float green, float blue, bool remap);     // 0x0071C250

    UnknownVoxelPaletteHeader Header;   // +0x00
    unsigned char *Colors;              // +0x10 256 RGB byte triples
    unsigned char *LUT;                 // +0x14 section count * 256 lookup
    int ColorsAllocated;                // +0x18 Colors is owned
    int LUTAllocated;                   // +0x1C LUT is owned
};

#endif
