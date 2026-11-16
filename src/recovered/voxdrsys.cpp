// voxdrsys.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voxdrsys.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MATRIX3D_INLINE_COPY
#include "shared/vector3.hpp"
#include "shared/matrix3d.hpp"
#include "shared/rect.hpp"
#include "shared/rgb_class.hpp"
#include "shared/unknown_pair_first.hpp"
#include "shared/crt_memory.hpp"
#include "shared/voxel_draw_system.hpp"
#include "shared/voxel_render_record.hpp"


// int __fastcall unknown_run_716f40(void *source) -- 0x00716F40
// Purpose: builds a local worker (0x0071BD00) over the buffers at
// 0x00AE3008 and 0x00AF4608, runs it on source (0x0071BDE0), copies its
// first four words to the globals 0x00AE3004, 0x00AF660C, 0x00AE2FE8 and
// 0x00AF6608, destroys it (0x0071BD70) and returns the run's result.
// Assumptions: the worker class (0x20 bytes; its constructor allocates
// 0x300 bytes when given no first buffer) and every name are unidentified
// placeholders.

class UnknownWorker_716f40 {
public:
    UnknownWorker_716f40(void *buffer1, void *buffer2);
    ~UnknownWorker_716f40();
    int Run(void *source);

    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
    char unknown_10[0x10];
};

extern char unknown_buffer_ae3008[];
extern char unknown_buffer_af4608[];
extern int VPLRemapStart;
extern int VPLRemapEnd;
extern int VPLLUTCount;
extern int VPLUnused;

int __fastcall unknown_run_716f40(void *source)
{
    UnknownWorker_716f40 worker(unknown_buffer_ae3008, unknown_buffer_af4608);
    int result = worker.Run(source);
    VPLRemapStart = worker.unknown_00;
    VPLRemapEnd = worker.unknown_04;
    VPLLUTCount = worker.unknown_08;
    VPLUnused = worker.unknown_0c;
    return result;
}


// void VoxelDrawSystem::Convert_Voxel_Colors(int red_left, int red_right, int green_left, int green_right, int blue_left,
// int blue_right) (0x00716FC0; TibSun voxdrsys.cpp): converts the 256-entry voxel palette into screen pixels, each
// channel shifted right by its left count and left by its right count. The original walks a pointer to each entry's
// Green byte (red at -1, blue at +1) and the output pointer together; the TibSun index form gives a signed loop test.
// VoxelDrawSystem (Westwood voxdrsys.cpp as in Tiberian Sun; unit 0x00716D80-0x00717CDF): the voxel draw buffer
// (256x256, 8 bits) and its depth buffer, the per-frame object and shadow lists and their bounds, and the
// render that draws them centred in the buffer.

class Surface;


// The draw result of the second Render (0x18 bytes; TibSun SurfaceRegion): where to put the buffer's rectangle.
struct SurfaceRegion {
    Point2D Point;
    Rect Bounds;
};

extern unsigned char unknown_buffer_ae3408[256 * 256];      // 0x00AE3408 the draw buffer
extern unsigned char unknown_zbuffer_ad0a70[256 * 256];     // 0x00AD0A70 the depth buffer
extern int unknown_af6610;                                  // 0x00AF6610 depth buffering enabled
extern Vector3 MinVoxelBounds;                   // 0x00AE0A70
extern Vector3 MaxVoxelBounds;                   // 0x00AE0DD8
extern Rect LastRenderRect;                            // 0x00AE2FF0 the last rendered rectangle
extern VoxelShadowRenderStruct VoxelShadowRenderData[64];     // 0x00AF3408
extern RGBClass unknown_voxel_colors_ae3008[256];           // 0x00AE3008 the voxel palette
extern unsigned short Voxel16BitColors[256];   // 0x00AE0A7C the palette in screen format
extern int VoxelRenderDataCount;               // 0x00AE0CB0
extern int VoxelShadowRenderDataCount;               // 0x00AE3000

void __fastcall unknown_normal_lookup_71ba20(Vector3 const &light, int type);              // 0x0071BA20
void __fastcall unknown_normal_lookup_71baa0(Vector3 const &light, Vector3 const &viewer, float specular, int type);  // 0x0071BAA0

namespace VoxelDrawSystem {
    void __fastcall Convert_Voxel_Colors(int red_left, int red_right, int green_left, int green_right, int blue_left, int blue_right);
    unsigned char *Get_Surface_Buffer();
    void __fastcall Precalculate_Light(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &light_transform, Vector3 const &light);
    void __fastcall Precalculate_Light(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &light_transform, Matrix3D const &view_transform, Vector3 const &light, float specular_strength);
    void Reset();
    void __fastcall Prep_For_Shadow(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &camera, Matrix3D const &motion, Vector3 const &light, Surface *mask, int mask_x, int mask_y);
    void __fastcall Prep_For_Object(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &transform);
    void __fastcall Render(Rect &rect, int &x, int &y);
    SurfaceRegion __fastcall Render();
    void Clear_Buffer();
    void __fastcall Clear_Buffer(Rect &region);
    void Clear_Z_Buffer();
    void __fastcall Clear_Z_Buffer(int x, int y, int width, int height);
}

void __fastcall VoxelDrawSystem::Convert_Voxel_Colors(int red_left, int red_right, int green_left, int green_right, int blue_left, int blue_right)
{
    unsigned short *out = Voxel16BitColors;
    for (unsigned char const *channel = &unknown_voxel_colors_ae3008[0].Green; channel < &unknown_voxel_colors_ae3008[256].Green; channel += sizeof(RGBClass)) {
        unsigned char red = channel[-1];
        unsigned char green = channel[0];
        unsigned char blue = channel[1];
        *out++ = (((red >> red_left) << red_right) | ((green >> green_left) << green_right) | ((blue >> blue_left) << blue_right));
    }
}


// unsigned char *VoxelDrawSystem::Get_Surface_Buffer() (0x00717030): the 256x256 draw buffer. The unit's declarations
// are in its first source (Convert_Voxel_Colors 0x00716FC0).

unsigned char *VoxelDrawSystem::Get_Surface_Buffer()
{
    return unknown_buffer_ae3408;
}


// VoxelDrawSystem::Precalculate_Light (0x00717050): the light in the section's frame, then the normal lookup for the
// section's normal table (TibSun voxdrsys.cpp).

void __fastcall VoxelDrawSystem::Precalculate_Light(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &light_transform, Vector3 const &light)
{
    Matrix3D light_transform_inv = Matrix3D_Rigid_Inverse_00591420(light_transform);
    Vector3 light_tr = light_transform_inv.Rotate_Vector(light);
    int normal_type = voxlib->Get_Layer_Info(layer, info)->NormalType;
    unknown_normal_lookup_71ba20(light_tr, normal_type);
}


// VoxelDrawSystem::Precalculate_Light with a viewer and specular strength (0x007170D0).

void __fastcall VoxelDrawSystem::Precalculate_Light(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &light_transform, Matrix3D const &view_transform, Vector3 const &light, float specular_strength)
{
    Matrix3D light_transform_inv = Matrix3D_Rigid_Inverse_00591420(light_transform);
    Matrix3D view_transform_inv = Matrix3D_Rigid_Inverse_00591420(view_transform);
    Vector3 light_tr = light_transform_inv.Rotate_Vector(light);
    Vector3 viewer_tr = view_transform_inv.Rotate_Vector(Vector3(0, 0, 1));
    int normal_type = voxlib->Get_Layer_Info(layer, info)->NormalType;
    unknown_normal_lookup_71baa0(light_tr, viewer_tr, specular_strength, normal_type);
}


// VoxelDrawSystem::Reset (0x007171D0): empties the lists, clears the buffers (all of them the first time, then the
// last rendered rectangle) and resets the bounds.

void VoxelDrawSystem::Reset()
{
    static int _need_buffer_init = true;

    VoxelRenderDataCount = 0;
    VoxelShadowRenderDataCount = 0;

    if (_need_buffer_init) {
        Clear_Buffer();
        Clear_Z_Buffer();
        _need_buffer_init = false;
    } else {
        if (LastRenderRect.X >= 0 && LastRenderRect.Y >= 0 && LastRenderRect.X + LastRenderRect.Width <= 255 && LastRenderRect.Y + LastRenderRect.Height <= 255) {
            for (int i = LastRenderRect.Y; i <= LastRenderRect.Y + LastRenderRect.Height; i++) {
                memset(&unknown_buffer_ae3408[256 * i + LastRenderRect.X], 0, LastRenderRect.Width + 1);
            }
        } else {
            memset(unknown_buffer_ae3408, 0, sizeof(unknown_buffer_ae3408));
        }
        if (unknown_af6610) {
            if (LastRenderRect.Y <= LastRenderRect.Height + LastRenderRect.Y) {
                for (int i = LastRenderRect.Y; i <= LastRenderRect.Y + LastRenderRect.Height; i++) {
                    memset(&unknown_zbuffer_ad0a70[256 * i + LastRenderRect.X], 0, LastRenderRect.Width + 1);
                }
            }
        }
    }

    MinVoxelBounds = Vector3(10000, 10000, 10000);
    MaxVoxelBounds = Vector3(-10000, -10000, -10000);
}


// VoxelDrawSystem::Prep_For_Shadow (0x00717360): queues a section's shadow (its four ground corners; RA2 adds the
// mask surface and offset) and grows the bounds.

void __fastcall VoxelDrawSystem::Prep_For_Shadow(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &camera, Matrix3D const &motion, Vector3 const &light, Surface *mask, int mask_x, int mask_y)
{
    LayerInfoStruct const *layer_info = voxlib->Get_Layer_Info(layer, info);

    VoxelShadowRenderStruct &data = VoxelShadowRenderData[VoxelShadowRenderDataCount];

    data.unknown_3c = mask;
    data.unknown_40 = mask_x;
    data.unknown_44 = mask_y;
    data.unknown_00 = voxlib;
    data.unknown_04 = layer;
    data.unknown_08 = info;

    for (int i = 0; i < 4; i++) {
        data.Points[i] = motion * layer_info->unknown_40[i];
        data.Points[i].Z = 0;
        data.Points[i] = camera * data.Points[i];
        data.Points[i] = data.Points[i] + light;
        data.Points[i].Y = -data.Points[i].Y;

        if (data.Points[i].X < MinVoxelBounds.X) {
            MinVoxelBounds.X = data.Points[i].X;
        }
        if (data.Points[i].Y < MinVoxelBounds.Y) {
            MinVoxelBounds.Y = data.Points[i].Y;
        }
        if (data.Points[i].X > MaxVoxelBounds.X) {
            MaxVoxelBounds.X = data.Points[i].X;
        }
        if (data.Points[i].Y > MaxVoxelBounds.Y) {
            MaxVoxelBounds.Y = data.Points[i].Y;
        }
    }

    VoxelShadowRenderDataCount++;
}


// VoxelDrawSystem::Prep_For_Object (0x007174C0): queues a section with its transformed box and bounds.

void __fastcall VoxelDrawSystem::Prep_For_Object(VoxelLibrary *voxlib, int layer, int info, Matrix3D const &transform)
{
    LayerInfoStruct const *layer_info = voxlib->Get_Layer_Info(layer, info);

    VoxelRenderStruct &data = VoxelRenderData[VoxelRenderDataCount];

    data.MinBounds = Vector3(10000, 10000, 10000);
    data.MaxBounds = Vector3(-10000, -10000, -10000);
    data.Frame = info;
    data.Section = layer;
    data.Library = voxlib;

    for (int i = 0; i < 8; i++) {
        data.Points[i] = transform * layer_info->unknown_40[i];
        data.Points[i].Y = -data.Points[i].Y;
        if (data.Points[i].X < data.MinBounds.X) {
            data.MinBounds.X = data.Points[i].X;
        }
        if (data.Points[i].X > data.MaxBounds.X) {
            data.MaxBounds.X = data.Points[i].X;
        }
        if (data.Points[i].Y < data.MinBounds.Y) {
            data.MinBounds.Y = data.Points[i].Y;
        }
        if (data.Points[i].Y > data.MaxBounds.Y) {
            data.MaxBounds.Y = data.Points[i].Y;
        }
        if (data.Points[i].Z < data.MinBounds.Z) {
            data.MinBounds.Z = data.Points[i].Z;
            data.AnchorCorner = i;
        }
        if (data.Points[i].Z > data.MaxBounds.Z) {
            data.MaxBounds.Z = data.Points[i].Z;
        }
    }

    if (data.MinBounds.X < MinVoxelBounds.X) {
        MinVoxelBounds.X = data.MinBounds.X;
    }
    if (data.MinBounds.Y < MinVoxelBounds.Y) {
        MinVoxelBounds.Y = data.MinBounds.Y;
    }
    if (data.MinBounds.Z < MinVoxelBounds.Z) {
        MinVoxelBounds.Z = data.MinBounds.Z;
    }
    if (data.MaxBounds.X > MaxVoxelBounds.X) {
        MaxVoxelBounds.X = data.MaxBounds.X;
    }
    if (data.MaxBounds.Y > MaxVoxelBounds.Y) {
        MaxVoxelBounds.Y = data.MaxBounds.Y;
    }
    if (data.MaxBounds.Z > MaxVoxelBounds.Z) {
        MaxVoxelBounds.Z = data.MaxBounds.Z;
    }

    VoxelRenderDataCount++;
}


// VoxelDrawSystem::Render(Rect &rect, int &x, int &y) (0x007176C0; TibSun voxdrsys.cpp): draws the queued shadows,
// then the queued objects sorted by their far Z (back to front, or front to back with the depth buffer), and
// returns the drawn rectangle of the 256x256 buffer (padded by 4) and where its centre goes.

void __fastcall VoxelDrawSystem::Render(Rect &rect, int &x, int &y)
{
    static VoxelRenderStruct *render_data[64];

    int index;

    Vector3 center;
    center.X = (MaxVoxelBounds.X + MinVoxelBounds.X) * 0.5f;
    center.Y = (MaxVoxelBounds.Y + MinVoxelBounds.Y) * 0.5f;
    center.Z = (MaxVoxelBounds.Z + MinVoxelBounds.Z) * 0.5f;

    for (index = 0; index < VoxelShadowRenderDataCount; index++) {
        VoxelShadowRenderData[index].unknown_00->Render_Shadow(&VoxelShadowRenderData[index], &center);
    }

    for (index = 0; index < VoxelRenderDataCount; index++) {
        render_data[index] = &VoxelRenderData[index];
    }

    for (index = 0; index < VoxelRenderDataCount - 1; index++) {
        for (int index2 = index + 1; index2 < VoxelRenderDataCount; index2++) {
            VoxelRenderStruct *first = render_data[index];
            VoxelRenderStruct *second = render_data[index2];

            if (first->MaxBounds.Z > second->MaxBounds.Z) {
                render_data[index] = second;
                render_data[index2] = first;
            }
        }
    }

    if (unknown_af6610) {
        for (index = VoxelRenderDataCount - 1; index >= 0; index--) {
            render_data[index]->Library->Render_Object((UnknownVoxelDrawState *)render_data[index], &center);
        }
    } else {
        for (index = 0; index < VoxelRenderDataCount; index++) {
            render_data[index]->Library->Render_Object((UnknownVoxelDrawState *)render_data[index], &center);
        }
    }

    rect.Width = MaxVoxelBounds.X - MinVoxelBounds.X;

    rect.Height = MaxVoxelBounds.Y - MinVoxelBounds.Y;

    rect.Set(256 / 2 - rect.Width / 2, 256 / 2 - rect.Height / 2, rect.Width + 8, rect.Height + 8);

    rect.X -= 4;
    rect.Y -= 4;
    LastRenderRect = rect;

    x = (int)center.X - rect.Width / 2;
    y = (int)center.Y - rect.Height / 2;
}


// SurfaceRegion VoxelDrawSystem::Render() (0x007178E0; TibSun voxdrsys.cpp): the same draw as Render(Rect &, int &,
// int &) 0x007176C0, returning the drawn rectangle of the 256x256 buffer (padded by 4) and where its centre goes as
// one region. The padded rectangle is a named local whose Y stays in its home slot (frame 0x34).

SurfaceRegion __fastcall VoxelDrawSystem::Render()
{
    static VoxelRenderStruct *render_data[64];

    int index;

    Vector3 center;
    center.X = (MaxVoxelBounds.X + MinVoxelBounds.X) * 0.5f;
    center.Y = (MaxVoxelBounds.Y + MinVoxelBounds.Y) * 0.5f;
    center.Z = (MaxVoxelBounds.Z + MinVoxelBounds.Z) * 0.5f;

    for (index = 0; index < VoxelShadowRenderDataCount; index++) {
        VoxelShadowRenderData[index].unknown_00->Render_Shadow(&VoxelShadowRenderData[index], &center);
    }

    for (index = 0; index < VoxelRenderDataCount; index++) {
        render_data[index] = &VoxelRenderData[index];
    }

    for (index = 0; index < VoxelRenderDataCount - 1; index++) {
        for (int index2 = index + 1; index2 < VoxelRenderDataCount; index2++) {
            VoxelRenderStruct *first = render_data[index];
            VoxelRenderStruct *second = render_data[index2];

            if (first->MaxBounds.Z > second->MaxBounds.Z) {
                render_data[index] = second;
                render_data[index2] = first;
            }
        }
    }

    if (unknown_af6610) {
        for (index = VoxelRenderDataCount - 1; index >= 0; index--) {
            render_data[index]->Library->Render_Object((UnknownVoxelDrawState *)render_data[index], &center);
        }
    } else {
        for (index = 0; index < VoxelRenderDataCount; index++) {
            render_data[index]->Library->Render_Object((UnknownVoxelDrawState *)render_data[index], &center);
        }
    }

    int width = MaxVoxelBounds.X - MinVoxelBounds.X;
    int height = MaxVoxelBounds.Y - MinVoxelBounds.Y;

    Rect rect;
    rect.X = 256 / 2 - width / 2;
    rect.Y = 256 / 2 - height / 2;
    width += 8;
    height += 8;
    rect.X -= 4;
    rect.Y -= 4;
    rect.Width = width;
    rect.Height = height;
    LastRenderRect = rect;

    Point2D point;
    point.X = (int)center.X - width / 2;
    point.Y = (int)center.Y - height / 2;

    SurfaceRegion region;
    region.Point = point;
    region.Bounds = rect;
    return region;
}


// VoxelDrawSystem::Clear_Buffer() (0x00717AF0)

void VoxelDrawSystem::Clear_Buffer()
{
    memset(unknown_buffer_ae3408, 0, sizeof(unknown_buffer_ae3408));
}


// void __fastcall Unknown_Clear_Table_Region_00717B10(int x, int y, unsigned int width, int height) (0x00717B10)
// Clears a width x height rectangle of the 256 x 256 byte table at 0x00AE3408 (row stride 256), or the whole
// table when the rectangle leaves it (x or y negative, or the right/bottom edge above 255).
extern "C" void *__cdecl memset(void *destination, int value, unsigned int length);
#pragma intrinsic(memset)

extern unsigned char unknown_table_ae3408[0x10000];

void __fastcall Unknown_Clear_Table_Region_00717B10(int x, int y, unsigned int width, int height)
{
    if (x < 0 || y < 0 || x + (int)width > 255 || (height += y) > 255) {
        memset(unknown_table_ae3408, 0, sizeof(unknown_table_ae3408));
    } else if (y <= height) {
        unsigned char *row = &unknown_table_ae3408[x + y * 256];
        int rows = height - y + 1;
        do {
            memset(row, 0, width);
            row += 256;
            rows--;
        } while (rows != 0);
    }
}


// VoxelDrawSystem::Clear_Buffer(Rect &) (0x00717B90): the rectangle's rows (one more column), or all of it when
// the rectangle leaves the buffer.

void __fastcall VoxelDrawSystem::Clear_Buffer(Rect &region)
{
    if (region.X >= 0 && region.Y >= 0 && region.X + region.Width <= 255 && region.Y + region.Height <= 255) {
        for (int i = region.Y; i <= region.Y + region.Height; i++) {
            memset(&unknown_buffer_ae3408[256 * i + region.X], 0, region.Width + 1);
        }
    } else {
        memset(unknown_buffer_ae3408, 0, sizeof(unknown_buffer_ae3408));
    }
}


// VoxelDrawSystem::Clear_Z_Buffer() (0x00717C10)

void VoxelDrawSystem::Clear_Z_Buffer()
{
    memset(unknown_zbuffer_ad0a70, 0, sizeof(unknown_zbuffer_ad0a70));
}


// VoxelDrawSystem::Clear_Z_Buffer(int x, int y, int width, int height) (0x00717C30)

void __fastcall VoxelDrawSystem::Clear_Z_Buffer(int x, int y, int width, int height)
{
    for (int i = y; i <= y + height; i++) {
        memset(&unknown_zbuffer_ad0a70[256 * i + x], 0, width);
    }
}
