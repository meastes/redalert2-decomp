// VoxelLibrary (TibSun voxlib.h; mischa85 names.csv agrees at every address): a .VXL voxel library (members
// 0x00719060-0x0071BCD6, owned by ObjectTypeClass's UnknownObjectPair).
// Split out of shared/object_type.hpp so the units of its members include no
// ObjectTypeClass header statics.
//
// Layout (from the constructors 0x00719060/0x00719080 and the size
// 0x00719920; names are hypotheses):
//   +0x00  bool  load failed
//   +0x04  int   section (limb) count
//   +0x08  int   tailer count (section count * frame count)
//   +0x0C  int   body size
//   +0x10  LayerStruct *  section headers (0x0C bytes each)
//   +0x14  LayerInfoStruct *   section tailers (0xA4 bytes each)
//   +0x18  void *                 body
//   size 0x1C
#ifndef RA2_SHARED_UNKNOWN_PAIR_FIRST_HPP
#define RA2_SHARED_UNKNOWN_PAIR_FIRST_HPP

#include "shared/vector3.hpp"

class FileClass;

// A section header (0x0C bytes; +0x00 the index of its first tailer).
struct LayerStruct {
    int unknown_00;                     // +0x00 first tailer index
    int unknown_04;
    int unknown_08;
};

// A voxel section tailer (hypothesis: +0x0C the HVA scale multiplier,
// +0x40 the eight corners of the section's box (0x00719F60), +0xA0..+0xA2
// the section's x/y/z size).
struct LayerInfoStruct {
    int *unknown_00;                    // +0x00 span start offsets (hypothesis)
    int *unknown_04;                    // +0x04 span end offsets
    unsigned char *unknown_08;          // +0x08 span data
    float unknown_0c;                   // +0x0C
    char unknown_10[0x40 - 0x10];
    Vector3 unknown_40[8];              // +0x40 box corners
    unsigned char XSize;           // +0xA0
    unsigned char YSize;           // +0xA1
    unsigned char ZSize;           // +0xA2
    unsigned char NormalType;           // +0xA3 normal table type (hypothesis)
};

// The voxel draw state the section drawers read (hypothesis; built by the
// voxel draw 0x00717xxx): +0x04 section, +0x08 frame, +0x0C view index,
// +0x28 the section box corners after transformation.
struct UnknownVoxelDrawState {
    void *unknown_00;                   // +0x00 the library (VoxelRenderStruct::Library)
    int unknown_04;                     // +0x04 section
    int unknown_08;                     // +0x08 frame
    int unknown_0c;                     // +0x0C view index into VoxelRenderOrientations
    char unknown_10[0x28 - 0x10];
    Vector3 unknown_28[8];              // +0x28 transformed box corners
};

class Surface;
class VoxelLibrary;

// A voxel shadow record (0x48 bytes; the 64 at 0x00AF3408, constructed by
// 0x00716F10): the library, section and frame, four transformed shadow
// corners, an optional surface whose pixels mask the shadow and its offset
// (fields from 0x00719C10; names are hypotheses).
struct VoxelShadowRenderStruct {
    VoxelLibrary *unknown_00;           // +0x00 the library (hypothesis)
    int unknown_04;                     // +0x04 section
    int unknown_08;                     // +0x08 frame
    Vector3 Points[4];                  // +0x0C shadow corners
    Surface *unknown_3c;                // +0x3C mask surface (0 = none)
    int unknown_40;                     // +0x40 mask left
    int unknown_44;                     // +0x44 mask top
};

class VoxelLibrary {
public:
    VoxelLibrary();                                         // 0x00719060
    VoxelLibrary(FileClass *file, int arg2 = 0);            // 0x00719080
    ~VoxelLibrary();                                        // 0x007190C0
    void Clear();                                           // 0x00719110 (frees the buffers)
    int Read_File(FileClass *file, int arg2);               // 0x00719160 (loads; 0 on success)
    LayerStruct *Get_Layer(int section);            // 0x00719850
    LayerInfoStruct *Get_Layer_Info(int section, int arg2);      // 0x00719860
    Vector3 Get_Bounding_Box_Center(int section, int arg2); // 0x00719890 (bounds center)
    int Memory_Used();                                      // 0x00719920 (memory size)
    void Render_Object(UnknownVoxelDrawState *state, Vector3 const *origin);   // 0x00719940 (draws a section)
    void Render_Shadow(VoxelShadowRenderStruct *record, Vector3 const *origin);   // 0x00719C10 (draws a shadow)
    void Compute_Bounding_Box();                            // 0x00719F60 (sets the box corners)

    bool LoadFailed;                    // +0x00 load failed
    int LayerCount;                     // +0x04 section count
    unsigned LayerInfoCount;            // +0x08 tailer count
    int DataSize;                       // +0x0C body size
    LayerStruct *LayerHeaders;  // +0x10
    LayerInfoStruct *LayerInfos;     // +0x14
    void *Data;                         // +0x18
};

#endif
