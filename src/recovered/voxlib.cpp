// voxlib.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voxlib.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_static_rect.hpp"
#include "shared/unknown_pair_first.hpp"
#include "shared/unknown_voxel_palette.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"
#include "shared/matrix3d.hpp"
#include "shared/surface.hpp"


// The .VXL voxel library (VoxelLibrary, 0x00719060-0x0071BCD6) and the
// voxel palette lookup (VoxelPaletteLibrary, 0x0071BD00-0x0071C314). The unit's
// only dynamic initializer (0x00719040) is shared/unknown_static_rect.hpp's.

// Three unsigned shorts (8.8 fixed-point screen steps; TibSun's Vector3i16).
struct Vector3i16 {
    unsigned short I;
    unsigned short J;
    unsigned short K;
};

// The span drawer's parameters (0x34 bytes; TibSun's VoxelFuncArgumentStruct,
// names hypotheses).
struct VoxelFuncArgumentStruct {
    int *StartOffset;                   // the tailer's span start offsets
    int *EndOffset;                     // span end offsets
    unsigned char *DataOffset;          // span data
    int StartIndex;                     // current column
    int StrideX;
    int StrideY;
    Vector3i16 TransformMatrix[4];      // origin, x, y and z steps
    unsigned char XSize;                // +0x30
    unsigned char YSize;
    unsigned char ZSize;
};

// Per view: the drawer flags, the origin corner and the x/y/z edge corners
// (indexes into the 8 box corners) and four span strides.
struct UnknownVoxelView {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
    int unknown_18;
    int unknown_1c;
    int unknown_20;
};

extern UnknownVoxelView unknown_views_7f9e60[];
extern void (*unknown_drawers_7f9de0[])(VoxelFuncArgumentStruct *parameters);
extern int unknown_af6610;
extern int unknown_af6614;

// The screen offsets per depth (8.8 fixed point x/y) of the span drawers.
short unknown_steps_af8a10[256][2];

// The 256x256 byte grid the span drawers write (another unit's).
extern unsigned char unknown_grid_ae3408[];

// The normal vector tables per normal type (0 none) and their sizes.
extern Vector3 const *unknown_normal_tables_7f9f80[5];
extern int unknown_normal_counts_7f9f94[5];

// The default brightness levels of VoxelPaletteLibrary::Calculate_Lookup_Table.
float unknown_levels_af8f10[32];

// The shade per normal (0x10 is unlit).
unsigned char unknown_shades_af8e10[256];


VoxelLibrary::VoxelLibrary()
{
    LoadFailed = false;
    LayerCount = 0;
    LayerInfoCount = 0;
    DataSize = 0;
    LayerHeaders = 0;
    LayerInfos = 0;
    Data = 0;
}


VoxelLibrary::VoxelLibrary(FileClass *file, int arg2)
{
    LoadFailed = false;
    LayerCount = 0;
    LayerInfoCount = 0;
    DataSize = 0;
    LayerHeaders = 0;
    LayerInfos = 0;
    Data = 0;
    if (Read_File(file, arg2) == 0) {
        LoadFailed = true;
    }
}


VoxelLibrary::~VoxelLibrary()
{
    if (LayerHeaders != 0) {
        operator delete(LayerHeaders);
    }
    LayerHeaders = 0;
    if (LayerInfos != 0) {
        operator delete(LayerInfos);
    }
    LayerInfos = 0;
    if (Data != 0) {
        operator delete(Data);
    }
    Data = 0;
}


void VoxelLibrary::Clear()
{
    if (LayerHeaders != 0) {
        operator delete(LayerHeaders);
    }
    LayerHeaders = 0;
    if (LayerInfos != 0) {
        operator delete(LayerInfos);
    }
    LayerInfos = 0;
    if (Data != 0) {
        operator delete(Data);
    }
    Data = 0;
}


struct VxlRGB { unsigned char Red,Green,Blue; };
extern VxlRGB VoxelRGBColors[256];
extern unsigned char VoxelPaletteTranslateTable[];
struct VxlFileHeader { char Name[16]; unsigned PaletteCount; int LayerCount, LayerInfoCount, DataSize; };
struct VxlLayerHeader { char Name[16]; int InfoIndex, Unused1; unsigned char Unused2; };
struct VxlLayerInfo { int StartOffset, EndOffset, DataOffset; float Scale; float Transform[12]; float MinBounds[3], MaxBounds[3]; unsigned char XSize,YSize,ZSize,NormalType; };
struct VxlMatrixData { float Data[12]; };
int VoxelLibrary::Read_File(FileClass *file, int load_palette)
{
    unsigned i = 0;
    Clear();
    if (!file->Open(1)) return 0;
    VxlLayerInfo layer_info;
    VxlFileHeader header;
    VxlLayerHeader layer_header;
    file->Read(&header, sizeof(header));
    LayerCount = header.LayerCount;
    LayerInfoCount = header.LayerInfoCount;
    DataSize = header.DataSize;
    LayerHeaders = new UnknownVoxelSection[LayerCount];
    LayerInfos = new UnknownVoxelTailer[LayerInfoCount];
    Data = new unsigned char[DataSize];
    if (!LayerHeaders || !LayerInfos || !Data) {
        Clear(); file->Close(); return 0;
    }
    if (load_palette) {
        VoxelPaletteLibrary palette((unsigned char *)VoxelRGBColors,VoxelPaletteTranslateTable);
        unsigned char first, last;
        if (file->Read(&first,1)!=1) { file->Close(); Clear(); return 0; }
        if (file->Read(&last,1)!=1) { file->Close(); Clear(); return 0; }
        palette.Header.unknown_00=first;
        palette.Header.unknown_04=last;
        for (unsigned char *green=&VoxelRGBColors[0].Green;green<&VoxelRGBColors[256].Green;green+=3) {
            unsigned channel=0;
            file->Read(green+channel++-1,1); file->Read(green+channel++-1,1); file->Read(green+channel-1,1);
        }
        if (header.PaletteCount>1) file->Seek(768*header.PaletteCount-768,1);
        palette.Calculate_Lookup_Table(0,0);
    } else file->Seek(770*header.PaletteCount,1);
    for (i=0;i<(unsigned)LayerCount;++i) {

        if (file->Read(&layer_header,sizeof(layer_header))!=sizeof(layer_header)) { Clear(); file->Close(); return 0; }
        LayerHeaders[i].unknown_00=layer_header.InfoIndex;
        LayerHeaders[i].unknown_04=layer_header.Unused1;
        *(unsigned char *)&LayerHeaders[i].unknown_08=layer_header.Unused2;
    }
    if (file->Read(Data,DataSize)!=DataSize) { Clear(); file->Close(); return 0; }
    for (i=0;i<LayerInfoCount;++i) {

        if (file->Read(&layer_info,sizeof(layer_info))!=sizeof(layer_info)) { Clear(); file->Close(); return 0; }
        LayerInfos[i].unknown_a0=layer_info.XSize;
        LayerInfos[i].unknown_a1=layer_info.YSize;
        LayerInfos[i].unknown_a2=layer_info.ZSize;
        LayerInfos[i].unknown_a3=layer_info.NormalType;
        LayerInfos[i].unknown_0c=layer_info.Scale;
        *(VxlMatrixData *)LayerInfos[i].unknown_10 = (VxlMatrixData &)Matrix3D(layer_info.Transform);
        Vector3 min; min.X=layer_info.MinBounds[0]; min.Y=layer_info.MinBounds[1]; min.Z=layer_info.MinBounds[2];
        Vector3 max; max=*(Vector3 const *)layer_info.MaxBounds;
        LayerInfos[i].unknown_40[0]=Vector3(max.X,max.Y,min.Z);
        LayerInfos[i].unknown_40[1]=Vector3(max.X,min.Y,min.Z);
        LayerInfos[i].unknown_40[2]=Vector3(min.X,min.Y,min.Z);
        LayerInfos[i].unknown_40[3]=Vector3(min.X,max.Y,min.Z);
        LayerInfos[i].unknown_40[4]=Vector3(max.X,max.Y,max.Z);
        LayerInfos[i].unknown_40[5]=Vector3(max.X,min.Y,max.Z);
        LayerInfos[i].unknown_40[6]=Vector3(min.X,min.Y,max.Z);
        LayerInfos[i].unknown_40[7]=Vector3(min.X,max.Y,max.Z);
        LayerInfos[i].unknown_00=(int *)((unsigned char *)Data+layer_info.StartOffset);
        LayerInfos[i].unknown_04=(int *)((unsigned char *)Data+layer_info.EndOffset);
        LayerInfos[i].unknown_08=(unsigned char *)Data+layer_info.DataOffset;
    }
    file->Close(); return 1;
}


UnknownVoxelSection *VoxelLibrary::Get_Layer(int section)
{
    return &LayerHeaders[section];
}


UnknownVoxelTailer *VoxelLibrary::Get_Layer_Info(int section, int arg2)
{
    return &LayerInfos[Get_Layer(section)->unknown_00 + arg2];
}


Vector3 VoxelLibrary::Get_Bounding_Box_Center(int section, int arg2)
{
    UnknownVoxelTailer *tailer = Get_Layer_Info(section, arg2);
    return (tailer->unknown_40[4] + tailer->unknown_40[2]) * 0.5f;
}


int VoxelLibrary::Memory_Used()
{
    return LayerCount * sizeof(UnknownVoxelSection) + LayerInfoCount * sizeof(UnknownVoxelTailer) + sizeof(VoxelLibrary) + DataSize;
}


// Draws a section: fills the span drawer's parameters from the section's
// tailer and the view's corner choice (origin corner and the three edge
// corners, in 8.8 fixed point; the z steps only when unknown_af6610) and calls
// the drawer chosen by the view's flags, 2 (unknown_af6610), 4
// (unknown_af6614) and 8 (no normal table).
void VoxelLibrary::Render_Object(UnknownVoxelDrawState *state, Vector3 const *origin)
{
    UnknownVoxelTailer *tailer = Get_Layer_Info(state->unknown_04, state->unknown_08);
    unsigned char size_x = tailer->unknown_a0;
    unsigned char size_y = tailer->unknown_a1;
    unsigned char size_z = tailer->unknown_a2;
    VoxelFuncArgumentStruct parameters;
    parameters.XSize = size_x;
    parameters.YSize = size_y;
    parameters.ZSize = size_z;
    parameters.StartOffset = tailer->unknown_00;
    parameters.EndOffset = tailer->unknown_04;
    parameters.DataOffset = tailer->unknown_08;
    Vector3 const *corner = &state->unknown_28[unknown_views_7f9e60[state->unknown_0c].unknown_04];
    Vector3 const *corner_x = &state->unknown_28[unknown_views_7f9e60[state->unknown_0c].unknown_08];
    Vector3 const *corner_y = &state->unknown_28[unknown_views_7f9e60[state->unknown_0c].unknown_0c];
    Vector3 const *corner_z = &state->unknown_28[unknown_views_7f9e60[state->unknown_0c].unknown_10];
    parameters.StrideX = unknown_views_7f9e60[state->unknown_0c].unknown_1c;
    parameters.StrideY = unknown_views_7f9e60[state->unknown_0c].unknown_20 * size_x;
    parameters.StartIndex = unknown_views_7f9e60[state->unknown_0c].unknown_18 * (size_y - 1) * size_x + unknown_views_7f9e60[state->unknown_0c].unknown_14 * (size_x - 1);
    parameters.TransformMatrix[0].I = (short)((corner->X + 128.0f - origin->X) * 256.0f);
    parameters.TransformMatrix[0].J = (short)((corner->Y + 128.0f - origin->Y) * 256.0f);
    parameters.TransformMatrix[0].K = (short)((corner->Z + 128.0f - origin->Z) * 256.0f);
    parameters.TransformMatrix[1].I = (short)((corner_x->X - corner->X) / size_x * 256.0f);
    parameters.TransformMatrix[2].I = (short)((corner_y->X - corner->X) / size_y * 256.0f);
    parameters.TransformMatrix[3].I = (short)((corner_z->X - corner->X) / size_z * 256.0f);
    parameters.TransformMatrix[1].J = (short)((corner_x->Y - corner->Y) / size_x * 256.0f);
    parameters.TransformMatrix[2].J = (short)((corner_y->Y - corner->Y) / size_y * 256.0f);
    parameters.TransformMatrix[3].J = (short)((corner_z->Y - corner->Y) / size_z * 256.0f);
    if (unknown_af6610) {
        parameters.TransformMatrix[1].K = (short)((corner_x->Z - corner->Z) / size_x * 256.0f);
        parameters.TransformMatrix[2].K = (short)((corner_y->Z - corner->Z) / size_y * 256.0f);
        parameters.TransformMatrix[3].K = (short)((corner_z->Z - corner->Z) / size_z * 256.0f);
    }
    int drawer = unknown_views_7f9e60[state->unknown_0c].unknown_00;
    if (unknown_af6610) {
        drawer |= 2;
    }
    if (unknown_af6614) {
        drawer |= 4;
    }
    if (tailer->unknown_a3 == 0) {
        drawer |= 8;
    }
    unknown_drawers_7f9de0[drawer](&parameters);
}


inline void Draw_Shadow(VoxelFuncArgumentStruct *parameters, Surface *surface, int left, int top)
{
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    unsigned row;
    unsigned row_column;
    unsigned short row_x;
    unsigned short row_y;
    if (surface) {
        for (row = 0; row < parameters->YSize; row++) {
            row_column = parameters->StartIndex;
            row_x = x;
            row_y = y;
            for (unsigned column = 0; column < parameters->XSize; column++) {
                unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
                unsigned short column_x = x;
                unsigned short column_y = y;
                if (offset != 0xFFFFFFFF) {
                    unsigned index = (x >> 8) | (y & 0xFF00);
                    unsigned short pixel_x = left + (x >> 8);
                    unsigned short pixel_y = top + (y >> 8);
                    int pixel = surface->Get_Pixel(Point2D(pixel_x, pixel_y));
                    int next = surface->Get_Pixel(Point2D(pixel_x + 1, pixel_y));
                    unknown_grid_ae3408[index] = pixel == 0;
                    unknown_grid_ae3408[index + 1] = next == 0;
                }
                x = column_x + parameters->TransformMatrix[1].I;
                y = column_y + parameters->TransformMatrix[1].J;
                parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
            }
            x = row_x + parameters->TransformMatrix[2].I;
            y = row_y + parameters->TransformMatrix[2].J;
            parameters->StartIndex = parameters->StrideY + row_column;
        }
    } else {
        for (row = 0; row < parameters->YSize; row++) {
            row_column = parameters->StartIndex;
            row_x = x;
            row_y = y;
            for (unsigned column = 0; column < parameters->XSize; column++) {
                unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
                unsigned short column_x = x;
                unsigned short column_y = y;
                if (offset != 0xFFFFFFFF) {
                    unsigned char color = 1;
                    unsigned index = (x >> 8) | (y & 0xFF00);
                    unknown_grid_ae3408[index] = color;
                    unknown_grid_ae3408[index + 1] = color;
                }
                x = column_x + parameters->TransformMatrix[1].I;
                y = column_y + parameters->TransformMatrix[1].J;
                parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
            }
            x = row_x + parameters->TransformMatrix[2].I;
            y = row_y + parameters->TransformMatrix[2].J;
            parameters->StartIndex = parameters->StrideY + row_column;
        }
    }
}

void VoxelLibrary::Render_Shadow(UnknownVoxelRecord48 *record, Vector3 const *origin)
{
    UnknownVoxelTailer *tailer = Get_Layer_Info(record->unknown_04, record->unknown_08);
    VoxelFuncArgumentStruct parameters;
    unsigned char size_x = tailer->unknown_a0;
    unsigned char size_y = tailer->unknown_a1;
    unsigned char size_z = 1;
    parameters.XSize = size_x;
    parameters.YSize = size_y;
    parameters.ZSize = size_z;
    parameters.StartOffset = 0;
    parameters.EndOffset = tailer->unknown_04;
    parameters.DataOffset = 0;
    Vector3 const &corner_x = record->Points[1];
    Vector3 const &corner = record->Points[2];
    Vector3 const &corner_y = record->Points[3];
    parameters.StrideX = 1;
    parameters.StrideY = size_x;
    parameters.StartIndex = 0;
    parameters.TransformMatrix[0].I = short(float(corner.X + 128 - origin->X) * 256);
    parameters.TransformMatrix[0].J = short(float(corner.Y + 128 - origin->Y) * 256);
    record->unknown_40 = record->unknown_40 + origin->X;
    record->unknown_44 = record->unknown_44 + origin->Y;
    parameters.TransformMatrix[1].I = (short)((corner_x.X - corner.X) / size_x * 256.0f);
    parameters.TransformMatrix[2].I = (short)((corner_y.X - corner.X) / size_y * 256.0f);
    parameters.TransformMatrix[1].J = (short)((corner_x.Y - corner.Y) / size_x * 256.0f);
    parameters.TransformMatrix[2].J = (short)((corner_y.Y - corner.Y) / size_y * 256.0f);
    Draw_Shadow(&parameters, record->unknown_3c, record->unknown_40, record->unknown_44);
}


// Sets the eight corners of each tailer's box from its size, centered on the
// origin.
void VoxelLibrary::Compute_Bounding_Box()
{
    for (unsigned index = 0; index < LayerInfoCount; index++) {
        UnknownVoxelTailer *tailer = &LayerInfos[index];
        tailer->unknown_40[0] = Vector3(tailer->unknown_a0 * 0.5f, tailer->unknown_a1 * 0.5f, -tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[1] = Vector3(tailer->unknown_a0 * 0.5f, -tailer->unknown_a1 * 0.5f, -tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[2] = Vector3(-tailer->unknown_a0 * 0.5f, -tailer->unknown_a1 * 0.5f, -tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[3] = Vector3(-tailer->unknown_a0 * 0.5f, tailer->unknown_a1 * 0.5f, -tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[4] = Vector3(tailer->unknown_a0 * 0.5f, tailer->unknown_a1 * 0.5f, tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[5] = Vector3(tailer->unknown_a0 * 0.5f, -tailer->unknown_a1 * 0.5f, tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[6] = Vector3(-tailer->unknown_a0 * 0.5f, -tailer->unknown_a1 * 0.5f, tailer->unknown_a2 * 0.5f);
        tailer->unknown_40[7] = Vector3(-tailer->unknown_a0 * 0.5f, tailer->unknown_a1 * 0.5f, tailer->unknown_a2 * 0.5f);
    }
}


// Draws a section's spans into the byte grid: each column's span list holds
// (skip, count, count (color, normal) pairs, count) runs; every voxel's color
// lands at the 8.8 fixed-point screen position stepped per depth, column and
// row. Not in the drawer table 0x007F9DE0 (hypothesis: the C version of an
// assembly drawer).
// Fill the per-depth screen offsets from the depth step (two inline
// variants with different code shapes).
inline void Fill_Delta_Table1(VoxelFuncArgumentStruct *parameters)
{
    short (&steps)[256][2] = unknown_steps_af8a10;
    steps[0][0] = 0;
    steps[0][1] = 0;
    Vector3i16 &step = parameters->TransformMatrix[3];
    for (unsigned depth = 1; depth < parameters->ZSize; depth++) {
        steps[depth][0] = steps[depth - 1][0] + step.I;
        steps[depth][1] = steps[depth - 1][1] + step.J;
    }
}

inline void Fill_Delta_Table2(VoxelFuncArgumentStruct *parameters)
{
    unknown_steps_af8a10[0][0] = 0;
    unknown_steps_af8a10[0][1] = 0;
    Vector3i16 *step = &parameters->TransformMatrix[3];
    for (unsigned depth = 1; depth < parameters->ZSize; depth++) {
        unknown_steps_af8a10[depth][0] = unknown_steps_af8a10[depth - 1][0] + step->I;
        unknown_steps_af8a10[depth][1] = unknown_steps_af8a10[depth - 1][1] + step->J;
    }
}

void Draw_Voxel_Regular_Normals(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->StartOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned char skip = *data;
                    data++;
                    x += unknown_steps_af8a10[skip][0];
                    y += unknown_steps_af8a10[skip][1];
                    remaining -= skip;
                    unsigned count = *data;
                    data++;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            unsigned char color = *data;
                            data++;
                            data++;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            unknown_grid_ae3408[index] = color;
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data++;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// The drawer of Draw_Voxel_Regular_Normals walking each column's spans
// backwards from its end offset.
void Draw_Voxel_Reverse_Normals(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table1(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned count = *data;
                    data--;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            data--;
                            unsigned char color = *data;
                            data--;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            unknown_grid_ae3408[index] = color;
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data--;
                    unsigned char skip = *data;
                    data--;
                    x += unknown_steps_af8a10[skip][0];
                    y += unknown_steps_af8a10[skip][1];
                    remaining -= skip;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// The z-buffered drawer of Draw_Voxel_Regular_Normals: a voxel lands only where
// its depth (8.8 fixed point, stepped like x and y) is above the depth grid's
// byte, and it writes two neighbouring cells of both grids.
// The depth byte per grid cell of the z-buffered drawers (another unit's).
extern unsigned char unknown_zbuffer_ad0a70[];

// The lit color per shade and color (the voxel palette lookup; another unit's).
extern unsigned char unknown_lit_colors_af4608[][256];

void Draw_Voxel_Regular_Normals_ZBuffer(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    unsigned short z = parameters->TransformMatrix[0].K;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        unsigned short row_z = z;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned short column_x = x;
            unsigned short column_y = y;
            unsigned short column_z = z;
            unsigned offset = ((unsigned *)parameters->StartOffset)[parameters->StartIndex];
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned char value = *data;
                    data++;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    z += parameters->TransformMatrix[3].K * value;
                    remaining -= value;
                    unsigned count = *data;
                    data++;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            value = *data;
                            data++;
                            data++;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            if ((z >> 8) > unknown_zbuffer_ad0a70[index]) {
                                unknown_zbuffer_ad0a70[index] = z >> 8;
                                unknown_grid_ae3408[index] = value;
                                unknown_grid_ae3408[index + 1] = value;
                                unknown_zbuffer_ad0a70[index + 1] = z >> 8;
                            }
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            z += parameters->TransformMatrix[3].K;
                            count--;
                        }
                    }
                    data++;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            z = column_z + parameters->TransformMatrix[1].K;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        z = row_z + parameters->TransformMatrix[2].K;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Draw_Voxel_Regular_Normals_ZBuffer walking each column backwards.
void Draw_Voxel_Reverse_Normals_ZBuffer(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    unsigned short z = parameters->TransformMatrix[0].K;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        unsigned short row_z = z;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned short column_x = x;
            unsigned short column_y = y;
            unsigned short column_z = z;
            unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned count = *data;
                    data--;
                    unsigned char value;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            data--;
                            value = *data;
                            data--;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            if ((z >> 8) > unknown_zbuffer_ad0a70[index]) {
                                unknown_zbuffer_ad0a70[index] = z >> 8;
                                unknown_grid_ae3408[index] = value;
                                unknown_grid_ae3408[index + 1] = value;
                                unknown_zbuffer_ad0a70[index + 1] = z >> 8;
                            }
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            z += parameters->TransformMatrix[3].K;
                            count--;
                        }
                    }
                    data--;
                    value = *data;
                    data--;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    z += parameters->TransformMatrix[3].K * value;
                    remaining -= value;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            z = column_z + parameters->TransformMatrix[1].K;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        z = row_z + parameters->TransformMatrix[2].K;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Draw_Voxel_Regular_Normals with each color lit through its normal's shade.
void Draw_Voxel_Regular_Normals_Lighting(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->StartOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned char value = *data;
                    data++;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    remaining -= value;
                    unsigned count = *data;
                    data++;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            value = *data;
                            data++;
                            unsigned char shade = *data;
                            data++;
                            shade = unknown_shades_af8e10[shade];
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            unknown_grid_ae3408[index] = unknown_lit_colors_af4608[shade][value];
                            unknown_grid_ae3408[index + 1] = unknown_lit_colors_af4608[shade][value];
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data++;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Draw_Voxel_Regular_Normals_Lighting walking each column backwards.
void Draw_Voxel_Reverse_Normals_Lighting(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned count = *data;
                    data--;
                    unsigned char value;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            unsigned char shade = *data;
                            data--;
                            shade = unknown_shades_af8e10[shade];
                            value = *data;
                            data--;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            unknown_grid_ae3408[index] = unknown_lit_colors_af4608[shade][value];
                            unknown_grid_ae3408[index + 1] = unknown_lit_colors_af4608[shade][value];
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data--;
                    value = *data;
                    data--;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    remaining -= value;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Draw_Voxel_Regular_Normals_ZBuffer_Lighting(VoxelFuncArgumentStruct *parameters) (0x0071AD30)
// Adopted from ra2-decomp Draw_Voxel_Regular_Normals_ZBuffer_Lighting (YR 0x00757980).
// Draw_Voxel_Regular_Normals_ZBuffer with each colour lit through its normal's
// shade: a voxel lands only where its depth is above the depth grid's byte and
// writes two neighbouring cells of both grids.
void Draw_Voxel_Regular_Normals_ZBuffer_Lighting(VoxelFuncArgumentStruct *state)
{
    Fill_Delta_Table2(state);

    unsigned char value;
    unsigned char normal_index;
    unsigned short pixel_x = state->TransformMatrix[0].I;
    unsigned short pixel_y = state->TransformMatrix[0].J;
    unsigned short pixel_z = state->TransformMatrix[0].K;

    for (unsigned int y = 0; y < state->YSize; y++) {
        unsigned int base_index = state->StartIndex;
        unsigned short row_start_x = pixel_x;
        unsigned short row_start_y = pixel_y;
        unsigned short row_start_z = pixel_z;
        for (unsigned int x = 0; x < state->XSize; x++) {
            unsigned short column_start_x = pixel_x;
            unsigned short column_start_y = pixel_y;
            unsigned short column_start_z = pixel_z;
            unsigned int data_offset = ((unsigned int *)state->StartOffset)[state->StartIndex];
            if (data_offset != 0xFFFFFFFF) {
                unsigned char * ptr = state->DataOffset + data_offset;
                unsigned int remaining = state->ZSize;
                while (remaining) {
                    value = *ptr;
                    ptr++;
                    pixel_x += unknown_steps_af8a10[value][0];
                    pixel_y += unknown_steps_af8a10[value][1];
                    pixel_z += state->TransformMatrix[3].K * value;
                    remaining -= value;
                    unsigned int run_length = *ptr;
                    ptr++;
                    if (run_length) {
                        remaining -= run_length;
                        while (run_length) {
                            unsigned int buffer_index = (pixel_x >> 8) | (pixel_y & 0xFF00);
                            if ((pixel_z >> 8) > unknown_zbuffer_ad0a70[buffer_index]) {
                                value = *ptr;
                                ptr++;
                                normal_index = *ptr;
                                ptr++;
                                normal_index = unknown_shades_af8e10[normal_index];
                                unknown_grid_ae3408[buffer_index] = unknown_lit_colors_af4608[normal_index][value];
                                unknown_grid_ae3408[buffer_index + 1] = unknown_lit_colors_af4608[normal_index][value];
                                unknown_zbuffer_ad0a70[buffer_index] = (pixel_z >> 8);
                                unknown_zbuffer_ad0a70[buffer_index + 1] = (pixel_z >> 8);
                            } else {
                                ptr++;
                                ptr++;
                            }
                            pixel_x += state->TransformMatrix[3].I;
                            pixel_y += state->TransformMatrix[3].J;
                            pixel_z += state->TransformMatrix[3].K;
                            run_length--;
                        }
                    }
                    ptr++;
                }
            }
            pixel_x = column_start_x + state->TransformMatrix[1].I;
            pixel_y = column_start_y + state->TransformMatrix[1].J;
            pixel_z = column_start_z + state->TransformMatrix[1].K;
            state->StartIndex = state->StrideX + state->StartIndex;
        }
        pixel_x = row_start_x + state->TransformMatrix[2].I;
        pixel_y = row_start_y + state->TransformMatrix[2].J;
        pixel_z = row_start_z + state->TransformMatrix[2].K;
        state->StartIndex = state->StrideY + base_index;
    }
}


// Reverse span drawing with normal lighting and the native unsigned depth test.
void Draw_Voxel_Reverse_Normals_ZBuffer_Lighting(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x=parameters->TransformMatrix[0].I;
    unsigned short y=parameters->TransformMatrix[0].J;
    unsigned short z=parameters->TransformMatrix[0].K;
    for (unsigned row=0;row<parameters->YSize;++row) {
        unsigned row_column=parameters->StartIndex;
        unsigned short row_x=x, row_y=y, row_z=z;
        for (unsigned column=0;column<parameters->XSize;++column) {
            unsigned short column_x=x, column_y=y, column_z=z;
            unsigned offset=((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            if (offset!=0xffffffff) {
                unsigned char *data=parameters->DataOffset+offset;
                unsigned remaining=parameters->ZSize;
                while (remaining) {
                    unsigned count=*data;
                    --data;
                    unsigned char value;
                    unsigned char normal_index;
                    unsigned char &shade=normal_index;
                    if (count) {
                        remaining-=count;
                        while (count) {
                            unsigned index=(x>>8)|(y&0xff00);
                            if ((z>>8)>unknown_zbuffer_ad0a70[index]) {
                                shade=*data;
                                --data;
                                shade=unknown_shades_af8e10[shade];
                                value=*data;
                                --data;
                                value=unknown_lit_colors_af4608[shade][value];
                                unknown_zbuffer_ad0a70[index]=z>>8;
                                unknown_zbuffer_ad0a70[index+1]=z>>8;
                                unknown_grid_ae3408[index]=value;
                                unknown_grid_ae3408[index+1]=value;
                            } else data-=2;
                            x+=parameters->TransformMatrix[3].I;
                            y+=parameters->TransformMatrix[3].J;
                            z+=parameters->TransformMatrix[3].K;
                            --count;
                        }
                    }
                    --data;
                    value=*data;
                    --data;
                    x+=unknown_steps_af8a10[value][0];
                    y+=unknown_steps_af8a10[value][1];
                    z+=parameters->TransformMatrix[3].K*value;
                    remaining-=value;
                }
            }
            x=column_x+parameters->TransformMatrix[1].I;
            y=column_y+parameters->TransformMatrix[1].J;
            z=column_z+parameters->TransformMatrix[1].K;
            parameters->StartIndex=parameters->StrideX+parameters->StartIndex;
        }
        x=row_x+parameters->TransformMatrix[2].I;
        y=row_y+parameters->TransformMatrix[2].J;
        z=row_z+parameters->TransformMatrix[2].K;
        parameters->StartIndex=parameters->StrideY+row_column;
    }
}


void Draw_Voxel_Regular(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table1(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->StartOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned char skip = *data;
                    data++;
                    x += unknown_steps_af8a10[skip][0];
                    y += unknown_steps_af8a10[skip][1];
                    remaining -= skip;
                    unsigned count = *data;
                    data++;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            unsigned char color = *data;
                            data++;
                            unknown_grid_ae3408[(x >> 8) | (y & 0xFF00)] = color;
                            unknown_grid_ae3408[(x >> 8) | (y & 0xFF00) + 1] = color;
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data++;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


void Draw_Voxel_Reverse(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            unsigned short column_x = x;
            unsigned short column_y = y;
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned count = *data;
                    data--;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            unsigned char color = *data;
                            data--;
                            unknown_grid_ae3408[(x >> 8) | (y & 0xFF00)] = color;
                            unknown_grid_ae3408[(x >> 8) | (y & 0xFF00) + 1] = color;
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            count--;
                        }
                    }
                    data--;
                    unsigned char skip = *data;
                    data--;
                    x += unknown_steps_af8a10[skip][0];
                    y += unknown_steps_af8a10[skip][1];
                    remaining -= skip;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Draw_Voxel_Regular with the depth test of Draw_Voxel_Regular_Normals_ZBuffer.
inline void Fill_Delta_Table3(VoxelFuncArgumentStruct *parameters)
{
    short (&steps)[256][2] = unknown_steps_af8a10;
    steps[0][0] = 0;
    steps[0][1] = 0;
    Vector3i16 &step = parameters->TransformMatrix[3];
    for (unsigned depth = 1; depth < parameters->ZSize; depth++) {
        steps[depth][0] = steps[depth - 1][0] + step.I;
        unknown_steps_af8a10[depth][1] = unknown_steps_af8a10[depth - 1][1] + step.J;
    }
}

void Draw_Voxel_Regular_ZBuffer(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table3(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    unsigned short z = parameters->TransformMatrix[0].K;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        unsigned short row_z = z;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned short column_x = x;
            unsigned short column_y = y;
            unsigned short column_z = z;
            unsigned offset = ((unsigned *)parameters->StartOffset)[parameters->StartIndex];
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned char value = *data;
                    data++;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    z += parameters->TransformMatrix[3].K * value;
                    remaining -= value;
                    unsigned count = *data;
                    data++;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            value = *data;
                            data++;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            if ((z >> 8) > unknown_zbuffer_ad0a70[index]) {
                                unknown_zbuffer_ad0a70[index] = z >> 8;
                                unknown_grid_ae3408[index] = value;
                                unknown_grid_ae3408[index + 1] = value;
                                unknown_zbuffer_ad0a70[index + 1] = z >> 8;
                            }
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            z += parameters->TransformMatrix[3].K;
                            count--;
                        }
                    }
                    data++;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            z = column_z + parameters->TransformMatrix[1].K;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        z = row_z + parameters->TransformMatrix[2].K;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Draw_Voxel_Reverse with the depth test of Draw_Voxel_Regular_Normals_ZBuffer.
void Draw_Voxel_Reverse_ZBuffer(VoxelFuncArgumentStruct *parameters)
{
    Fill_Delta_Table2(parameters);
    unsigned short x = parameters->TransformMatrix[0].I;
    unsigned short y = parameters->TransformMatrix[0].J;
    unsigned short z = parameters->TransformMatrix[0].K;
    for (unsigned row = 0; row < parameters->YSize; row++) {
        unsigned row_column = parameters->StartIndex;
        unsigned short row_x = x;
        unsigned short row_y = y;
        unsigned short row_z = z;
        for (unsigned column = 0; column < parameters->XSize; column++) {
            unsigned short column_x = x;
            unsigned short column_y = y;
            unsigned short column_z = z;
            unsigned offset = ((unsigned *)parameters->EndOffset)[parameters->StartIndex];
            if (offset != 0xFFFFFFFF) {
                unsigned char *data = parameters->DataOffset + offset;
                unsigned remaining = parameters->ZSize;
                while (remaining) {
                    unsigned count = *data;
                    data--;
                    unsigned char value;
                    if (count) {
                        remaining -= count;
                        while (count) {
                            value = *data;
                            data--;
                            unsigned index = (x >> 8) | (y & 0xFF00);
                            if ((z >> 8) > unknown_zbuffer_ad0a70[index]) {
                                unknown_zbuffer_ad0a70[index] = z >> 8;
                                unknown_grid_ae3408[index] = value;
                                unknown_grid_ae3408[index + 1] = value;
                                unknown_zbuffer_ad0a70[index + 1] = z >> 8;
                            }
                            x += parameters->TransformMatrix[3].I;
                            y += parameters->TransformMatrix[3].J;
                            z += parameters->TransformMatrix[3].K;
                            count--;
                        }
                    }
                    data--;
                    value = *data;
                    data--;
                    x += unknown_steps_af8a10[value][0];
                    y += unknown_steps_af8a10[value][1];
                    z += parameters->TransformMatrix[3].K * value;
                    remaining -= value;
                }
            }
            x = column_x + parameters->TransformMatrix[1].I;
            y = column_y + parameters->TransformMatrix[1].J;
            z = column_z + parameters->TransformMatrix[1].K;
            parameters->StartIndex = parameters->StrideX + parameters->StartIndex;
        }
        x = row_x + parameters->TransformMatrix[2].I;
        y = row_y + parameters->TransformMatrix[2].J;
        z = row_z + parameters->TransformMatrix[2].K;
        parameters->StartIndex = parameters->StrideY + row_column;
    }
}


// Fills the shade table from a light direction: the lit share of each normal
// of normal type `type` times 16; the last three entries are unlit.
void __fastcall unknown_71ba20(Vector3 const *light, int type)
{
    Vector3 const *normals = unknown_normal_tables_7f9f80[type];
    int count = unknown_normal_counts_7f9f94[type];
    for (int index = 0; index < count; index++) {
        Vector3 const &normal = normals[index];
        float dot = normal.X * light->X + normal.Z * light->Z + light->Y * normal.Y;
        if (dot < 0.0) {
            unknown_shades_af8e10[index] = 0;
        } else {
            unknown_shades_af8e10[index] = (unsigned char)(dot * 16.0f);
        }
    }
    unknown_shades_af8e10[255] = 0x10;
    unknown_shades_af8e10[254] = 0x10;
    unknown_shades_af8e10[253] = 0x10;
}


// Westwood's Vector3 dot product, Length2, division by a scalar and
// Normalize (the RA2 shared Vector3 does not declare them).
static inline float Vector3_Dot(Vector3 const &a, Vector3 const &b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}
static inline float Vector3_Length2(Vector3 const &v)
{
    return v.Y * v.Y + (v.X * v.X + v.Z * v.Z);
}
static inline Vector3 Vector3_Divide(Vector3 const &a, double k)
{
    return Vector3((float)(a.X / k), (float)(a.Y / k), (float)(a.Z / k));
}
static inline Vector3 Vector3_Normalize(Vector3 const &vec)
{
    float len = (float)Fast_Sqrt(Vector3_Length2(vec));
    if (len != 0.0) {
        return Vector3_Divide(vec, len);
    }
    return vec;
}
// Fills the shade table from a light and a view direction: the lit share of
// each normal of normal type `type` plus a specular term from the halfway
// vector (Schlick: d / (k - d * k + d)), each clamped at 0, times 16; the
// last three entries are unlit.
void __fastcall unknown_71baa0(Vector3 const *light, Vector3 const *view, float specular, int type)
{
    Vector3 const *normals = unknown_normal_tables_7f9f80[type];
    Vector3 sum = *view + *light;
    Vector3 halfway = Vector3_Normalize(sum);
    for (int index = 0; index < unknown_normal_counts_7f9f94[type]; index++) {
        Vector3 const &normal = normals[index];
        float diffuse = Vector3_Dot(normal, *light);
        float dot = Vector3_Dot(normal, halfway);
        float shine = dot / (specular - dot * specular + dot);
        shine = shine >= 0.0f ? shine : 0.0f;
        diffuse = diffuse >= 0.0f ? diffuse : 0.0f;
        float total = diffuse + shine;
        if (total < 0.0) {
            unknown_shades_af8e10[index] = 0;
        } else {
            unknown_shades_af8e10[index] = (unsigned char)(total * 16.0f);
        }
    }
    unknown_shades_af8e10[255] = 0x10;
    unknown_shades_af8e10[254] = 0x10;
    unknown_shades_af8e10[253] = 0x10;
}


// The index of the normal of normal type `type` closest in direction to
// `normal` (largest dot product); 0xFF when `normal` is shorter than 0.2.
int __fastcall unknown_71bc30(Vector3 const *normal, int type)
{
    Vector3 const *normals = unknown_normal_tables_7f9f80[type];
    float best = 0;
    int best_index = 0;
    if (Fast_Sqrt(normal->X * normal->X + normal->Y * normal->Y + normal->Z * normal->Z) < 0.2f) {
        return 0xFF;
    }
    int count = unknown_normal_counts_7f9f94[type];
    for (int index = 0; index < count; index++) {
        float dot = normals[index].X * normal->X + normals[index].Z * normal->Z + normals[index].Y * normal->Y;
        if (dot > best) {
            best = dot;
            best_index = index;
        }
    }
    return best_index;
}


// Marks every normal unlit.
void unknown_71bce0()
{
    memset(unknown_shades_af8e10, 0x10, sizeof(unknown_shades_af8e10));
}
