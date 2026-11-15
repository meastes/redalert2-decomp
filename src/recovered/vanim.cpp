// vanim.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vanim.json carries the addresses and the rows that
// stay in their own files.

#define RA2_VOXEL_DRAW_UNIT
// Header-static dynamic initializers of the unit at 0x0070C610:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0070C610-0x0070C7F6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x0070C640, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C660, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C680, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C6A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C6C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C6E0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C720, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C750, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C780, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C7B0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C7C0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.


// Dynamic initializer (0x0070C7E0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0070c610.cpp; no source of its own.




#include "shared/voxel_anim.hpp"
#include "shared/house.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/id_source.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/matrix3d.hpp"
#include "shared/color_scheme.hpp"
#include "shared/surface_blit.hpp"
#include "shared/convert.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tiberium.hpp"
#include "shared/terrain.hpp"
#include "shared/sound_play.hpp"
#include "shared/cd_timer.hpp"
#include "shared/unknown_geometry.hpp"


// VoxelAnimClass::VoxelAnimClass() (0x0070CD20)
// An empty voxel animation registered in the voxel animation list.

VoxelAnimClass::VoxelAnimClass() :
    unknown_e8(-1),
    Class(0),
    unknown_f0(0),
    unknown_f4(0),
    unknown_f8(false),
    unknown_124(false),
    unknown_128(0)
{
    AbstractClass::Create_ID();
    unknown_fc.Init();
    unknown_110.Init();
    unknown_VoxelAnimClass_list_008399d0.Add(this);
}


// VoxelAnimClass::~VoxelAnimClass() (0x0070CE40)

// The scalar deleting destructor (0x0070DF90) calls this out of line.
#pragma auto_inline(off)
VoxelAnimClass::~VoxelAnimClass()
{
    Unknown_006ECB90(this, true);
    unknown_VoxelAnimClass_list_008399d0.Delete(this);
    if (unknown_f0 != 0) {
        unknown_f0->Delete_Me();
        unknown_f0 = 0;
    }
    if (GameActive) {
        Limbo();
    }
    unknown_fc.Unknown_00406070();
    unknown_110.Unknown_00406070();
    unknown_fc.Unknown_00405C10();
    unknown_110.Unknown_00405C10();
    AbstractTypePtrTracker.Delete(this);
    Class = 0;
}
#pragma auto_inline(on)


bool VoxelAnimClass::Render(Rect *bounds, bool arg2, bool arg3)
{
    CellClass *cell = &Map[Center_Coord()];
    if (cell->unknown_12c & 0x10) {
        unknown_75 = true;
    }
    return ObjectClass::Render(bounds, arg2, false);
}


// void VoxelAnimClass::Draw_It(Point2D const &point, Rect const &window) (0x0070CFC0, slot 67)
// Draws the voxel animation at the screen point point within the window window:
// its shadow (the voxel flattened onto the ground) and then the voxel itself,
// lit and rotated by the bounce state.
void __fastcall Prepare_Voxel_Lighting_00717050(VoxelLibrary *voxel, int section, int arg3, Matrix3D const &matrix, Vector3 const &light);
void __cdecl Reset_Voxel_Renderer_007171d0();
void __fastcall Add_Voxel_Section_007174c0(VoxelLibrary *voxel, unsigned section, int arg3, Matrix3D const &matrix);
void __fastcall Voxel_Draw_Section_00717360(VoxelLibrary *voxel, int section, int arg3, Matrix3D const &transform, Matrix3D const &matrix, Vector3 const &light, Surface *mask, int mask_x, int mask_y);
void __fastcall Voxel_Render_Bounds_007176c0(Rect *rect, int *x, int *y);
Surface &__cdecl Voxel_Render_Surface_00717040();
extern Matrix3D unknown_voxel_transform_839a70;
extern Vector3 unknown_voxel_shadow_light_839a60;
extern Vector3 unknown_voxel_light_839ab0;
extern ConvertClass *unknown_drawer_8323b4;
extern ConvertClass *unknown_drawer_8323bc;
void VoxelAnimClass::Draw_It(Point2D const &point, Rect const &window)
{
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) != 0) {
        CoordStruct coord = Coord;
        if (Map.Is_Fogged(coord)) return;
    }
    Point2D pixel;
    Point2D position = point;
    int level;
    CoordStruct ground = Coord;
    ground.Z = Map.Get_Height_GL(ground);
    CellClass &cell = Map[ground];
    {
                if ((cell.unknown_140 & 0x100) != 0 && (CoordStruct(Coord).Z >= unknown_geometry_j + ground.Z)) {
            level = unknown_geometry_j + ground.Z;
            CoordStruct raised = ground + CoordStruct(0, 0, unknown_geometry_j + 1);
            TacticalMap->Coord_To_Pixel(raised, pixel);
        } else {
            level = ground.Z;
            TacticalMap->Coord_To_Pixel(ground, pixel);
        }
    }
    if (unknown_124) return;
    VoxelLibrary *voxel = Class->unknown_b0.unknown_00;
    if (voxel == 0) return;
    Matrix3D matrix;
    matrix = BounceClass::unknown_matrix_436fc0();
    Reset_Voxel_Renderer_007171d0();
    Voxel_Draw_Section_00717360(voxel, Class->VoxelIndex, 0, unknown_voxel_transform_839a70, matrix, unknown_voxel_shadow_light_839a60, 0, 0, 0);
    Rect rect;
    int offset_y;
    int offset_x;
    Voxel_Render_Bounds_007176c0(&rect, &offset_x, &offset_y);
    {
        RectPoint2D draw(pixel.X + offset_x, pixel.Y + offset_y);
        unknown_blit_49f3a0(*unknown_surface_83995c, unknown_drawer_8323b4, Voxel_Render_Surface_00717040(), rect, draw, window, 0,
                            unknown_drawer_8323b4->Select_Blitter_00485AA0(0x2001), -4 - Unknown_006A2970(level), 0, 1000, 0);
    }
    matrix = BounceClass::unknown_matrix_436fc0();
    Prepare_Voxel_Lighting_00717050(voxel, 0, 0, matrix, unknown_voxel_light_839ab0);
    Reset_Voxel_Renderer_007171d0();
    Add_Voxel_Section_007174c0(voxel, Class->VoxelIndex, 0, unknown_voxel_transform_839a70 * matrix);
    Voxel_Render_Bounds_007176c0(&rect, &offset_x, &offset_y);
    int flags = 0x2800;
    if (Class->Translucent) {
        flags = 0x2804;
    }
    int brightness;
    {
        CoordStruct coord = Coord;
        brightness = Map[coord].unknown_10a;
    }
    ConvertClass *drawer;
    if (unknown_f4 != 0) {
        drawer = ColorSchemes[unknown_f4->ColorSchemeIndex]->Converter;
    } else if (Class->IsTiberium) {
        drawer = unknown_drawer_8323bc;
    } else {
        drawer = unknown_drawer_8323b4;
        brightness = 1000;
    }
    {
        RectPoint2D draw2(position.X + offset_x, position.Y + offset_y);
        unknown_blit_49f3a0(*unknown_surface_83995c, drawer, Voxel_Render_Surface_00717040(), rect, draw2, window, 0,
                            drawer->Select_Blitter_00485AA0(flags), -2 - Unknown_006A2970(Get_Height()), 0, brightness, 0);
    }
}


// void VoxelAnimClass::Clear_All() (0x0070D350)
// Deletes every voxel animation; each destructor takes itself off the list.

void __cdecl VoxelAnimClass::Clear_All()
{
    while (unknown_VoxelAnimClass_list_008399d0.Count() != 0) {
        delete unknown_VoxelAnimClass_list_008399d0[0];
    }
}


// void VoxelAnimClass::AI() (0x0070D380, slot 23)
// One step of a voxel animation: keeps its start sound at the animation,
// counts its time down and, while it lasts, leaves trailer animations and
// moves the bounce state (a bounce damages what is in the cell); when the
// time runs out it splashes in water or leaves its expire animation and
// explosion, spawns its follow-up voxel animations (meteors) and ore, and
// removes itself.


void VoxelAnimClass::AI()
{
    if (Class->StartSound != -1) {
        Voc_Update_At(Center_Coord(), &unknown_fc);
    }
    if (unknown_f8) {
        Delete_Me();
        return;
    }
    if (unknown_128 != 0) {
        unknown_128--;
    }
    if (unknown_128 <= 0) {
        bool water = Map[CoordStruct(Coord)].unknown_ec == 2;
        bool bridge = CoordStruct(Coord).Z >= Map.Get_Height_GL(CoordStruct(Coord)) + unknown_geometry_j;
        if (water && !bridge) {
            if (Class->IsMeteor) {
                new AnimClass(Rules->SplashList[Rules->SplashList.Count() - 1], Coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
            } else {
                new AnimClass(Rules->unknown_09c, CoordStruct(Coord), 0, 1, 0x600, 0);
                new AnimClass(Rules->SplashList[0], Coord + CoordStruct(0, 0, 10), 0, 1, 0x600, 0);
            }
        } else if (Class->ExpireAnim != 0) {
            new AnimClass(Class->ExpireAnim,
                CoordStruct((int)BounceClass::unknown_00.Position.X, (int)BounceClass::unknown_00.Position.Y, (int)BounceClass::unknown_00.Position.Z),
                0, 1, 0x2600, -30);
            Unknown_0047E2C0(BounceClass::Coord(), Class->Damage, 0, Class->Warhead, 1);
            Unknown_0047F600(BounceClass::Coord(), Class->Damage, Class->Warhead, 0, 0);
        }
        if (!water || bridge) {
            Rect dirty(0, 0, 0, 0);
            if (Class->IsMeteor) {
                CoordStruct center = BounceClass::Coord();
                if (Class->Spawns != 0 && Class->SpawnCount > 0) {
                    int count = Random_Pick(0, Class->SpawnCount) + Random_Pick(0, Class->SpawnCount);
                    for (int index = 0; index < count; index++) {
                        new VoxelAnimClass(Class->Spawns, center, 0);
                    }
                }
                if (Class->IsTiberium && !bridge) {
                    for (int facing = 0; facing < 8; facing++) {
                        CellClass &cell = Map[Adjacent_Cell(Coord_Cell(center), facing)];
                        if (cell.Can_Tiberium_Germinate(0)) {
                            TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[OverlayClass::To_Tiberium(0x7F)];
                            if (cell.unknown_11c != 0) {
                                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + cell.unknown_11c * 2 + tiberium->NumFrames + Random_Pick(0, 1) - 2], Cell(cell.MapCoords), -1);
                            } else {
                                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + Random_Pick(0, 11)], Cell(cell.MapCoords), -1);
                            }
                            tiberium->Add_Growth_Cell(cell.MapCoords);
                            cell.OverlayData = 0;
                            Rect rect = cell.Overlay_Render_Rect();
                            rect.Y -= unknown_int_839634;
                            dirty = Union(dirty, rect);
                            Map.Radar_Background(cell.MapCoords);
                        }
                    }
                    TacticalMap->Register_Dirty_Area(dirty, 0);
                }
            } else if (Class->IsTiberium && !bridge) {
                CellClass &cell = Map[BounceClass::Coord()];
                if (cell.Can_Tiberium_Germinate(0)) {
                    TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[OverlayClass::To_Tiberium(0x7F)];
                    if (cell.unknown_11c != 0) {
                        new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + cell.unknown_11c * 2 + tiberium->NumFrames + Random_Pick(0, 1) - 2], Cell(cell.MapCoords), -1);
                    } else {
                        new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + Random_Pick(0, 11)], Cell(cell.MapCoords), -1);
                    }
                    tiberium->Add_Growth_Cell(cell.MapCoords);
                    cell.OverlayData = 0;
                    dirty = cell.Overlay_Render_Rect();
                    Map.Radar_Background(cell.MapCoords);
                }
            }
        }
        Delete_Me();
        return;
    }
    if (Class->TrailerAnim != 0 && Frame % 2 == 0) {
        new AnimClass(Class->TrailerAnim,
            CoordStruct((int)BounceClass::unknown_00.Position.X, (int)BounceClass::unknown_00.Position.Y,
                        (int)(BounceClass::unknown_00.Position.Z * unknown_geometry_diagonal * 0.019245008972988385 + 0.5)),
            1, 1, 0x600, 0);
    }
    int result = BounceClass::unknown_00.Unknown_004370E0();
    if (Class->IsMeteor) {
        BounceClass::unknown_00.unknown_2c += BounceClass::unknown_00.unknown_08;
    }
    switch (result) {
        case 1:
            if (Map[CoordStruct(Coord)].unknown_ec == 2) {
                unknown_128 = 0;
            } else {
                if (Class->BounceAnim != 0) {
                    new AnimClass(Class->BounceAnim, CoordStruct(Coord), 0, 1, 0x600, 0);
                }
                for (ObjectClass *object = Map[BounceClass::Coord()].OccupierPtr; object; object = object->unknown_30) {
                    CoordStruct delta = BounceClass::Coord() - CoordStruct(object->Coord);
                    int distance = abs(delta.Y) + abs(delta.X);
                    if (distance <= Class->DamageRadius) {
                        object->Take_Damage(&Class->Damage, Unknown_006A2970(distance), Class->Warhead, 0, 0, 0);
                    }
                }
            }
            break;
        case 2:
            unknown_128 = 0;
            break;
    }
    Set_Coord(BounceClass::Coord());
}


// int VoxelAnimClass::In_Which_Layer() const (0x0070DDA0)
// The layer (3).

int VoxelAnimClass::In_Which_Layer() const
{
    return 3;
}


// HRESULT __stdcall VoxelAnimClass::Load(IStream *stream) (0x0070DDB0)
// Restores the object in place, re-initializes the sound controllers and
// swizzles the pointers; returns ObjectClass::Load's result.

HRESULT __stdcall VoxelAnimClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) VoxelAnimClass(NoInitClass());
        unknown_fc.Init();
        unknown_110.Init();
        Swizzler.Swizzle((void **)&Class);
        Swizzler.Swizzle((void **)&unknown_f0);
        Swizzler.Swizzle((void **)&unknown_f4);
    }
    return result;
}


// HRESULT __stdcall VoxelAnimClass::Save(IStream *stream, BOOL clear_dirty) (0x0070DE50)

HRESULT __stdcall VoxelAnimClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// void VoxelAnimClass::Compute_CRC(WWCRCEngine &crc) const (0x0070DE70)

void VoxelAnimClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    crc((long)unknown_e8);
    crc(Class->Fetch_ID());
    if (unknown_f0 != 0) {
        crc(unknown_f0->Fetch_ID());
    }
    crc(unknown_f4->Fetch_ID());
    crc(unknown_f8);
    crc(unknown_124);
    crc((long)unknown_128);
}


// HRESULT __stdcall VoxelAnimClass::GetClassID(GUID *clsid) (0x0070DF10)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.


// int VoxelAnimClass::Size_Of() const (0x0070DF50)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.


// int VoxelAnimClass::Kind_Of() const (0x0070DF60)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.


// ObjectTypeClass *VoxelAnimClass::Class_Of() const (0x0070DF70)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.


// Cell const *VoxelAnimClass::Occupy_List(int arg1) (0x0070DF80)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.


// void *VoxelAnimClass::`scalar deleting destructor'(unsigned int) (0x0070DF90)
// Compiler-emitted from shared/voxel_anim.hpp in this unit; no source of its own.
