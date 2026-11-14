// vanim.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vanim.json carries the addresses and the rows that
// stay in their own files.


#include "shared/voxel_anim.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tiberium.hpp"
#include "shared/terrain.hpp"
#include "shared/sound_play.hpp"
#include "shared/cd_timer.hpp"
#include "shared/unknown_geometry.hpp"


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
                    unknown_tactical_83996c->Register_Dirty_Area(dirty, 0);
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
