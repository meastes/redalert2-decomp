"""Typed scratch rendering of one frozen Unit Draw_Voxel export."""
import hashlib

from seed_guarded_cfg import X, guarded_graph, require

ENTRY = 0x6ff690
RAW_SHA256 = "e4417ac4d933db7cbe6b068cdbd0a28944b5e8d5eb216535f46178779678c1dc"
BODY_SHA256 = "f88eb6252fd660c67ad96a21b44aa48ad61cec423bcad250f3c6fc6bd72cfab6"

PROVIDERS = {
    0x58fe10: ('??0Matrix3D@@QAE@ABV0@@Z', 4),
    0x591180: ('??D@YI?AVMatrix3D@@ABV0@0@Z', 4),
    0x717f90: ('?Unknown_00717F90@@YI?AVMatrix3D@@XZ', 0),
    0x6d9850: ('?Search_For_Node_006d9850@UnknownBuffer@@QBEPBUNodeElement@1@ABH@Z', 4),
    0x70a350: ('?Is_Present_0070a350@UnknownBuffer@@QBE_NABH@Z', 4),
    0x4c9a40: ('?unknown_4c9a40@FootClass@@QAEXABVUnknownObjectPair@@HHPAVUnknownBuffer@@ABVRect@@ABUPoint2D@@ABVMatrix3D@@_NPAVSurface@@II@Z', 44),
}

SURFACE_SLOTS = {30: (4, ('?Get_Rect@Surface@@UAE?AVRect@@XZ',)), 5: (8, ('?Fill_Rect@XSurface@@UAE_NABVRect@@H@Z',))}

def verify_native(inventory, repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit

    audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
    graph = guarded_graph(audit, inventory, ENTRY)
    require(graph["body_sha256"] == BODY_SHA256, "Frozen voxel draw body differs")
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    receivers = Receivers(repository, catalog, audit, inventory, graph, None)
    for target, (symbol, cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)["symbol"] == symbol and receivers.cleanup(target) == cleanup,
                "Voxel draw provider ABI differs")
    for slot, (cleanup, symbols) in SURFACE_SLOTS.items():
        family = receivers.family(("input", 0, "BSurface"), slot)
        require(family["pop"] == cleanup and {row["symbol"] for row in family["targets"]} == set(symbols),
                "Voxel surface slot ABI differs")
    nodes = graph["nodes"]
    expected = {
        0x6ff713: ("mov", "ecx, dword ptr [0xad0604]"),
        0x6ff74d: ("lea", "eax, [esp + 0x78]"),
        0x6ff751: ("push", "eax"),
        0x6ff752: ("call", "dword ptr [edx + 0x78]"),
        0x6ff7c5: ("mov", "ecx, 0xc"),
        0x6ff7ca: ("lea", "esi, [esp + 0xd0]"),
        0x6ff7d1: ("lea", "edi, [esp + 0x90]"),
        0x700590: ("push", "0"),
        0x700592: ("push", "edx"),
        0x700595: ("call", "dword ptr [eax + 0x14]"),
    }
    require(all((nodes[at].mnemonic, nodes[at].op_str) == instruction for at, instruction in expected.items()),
            "Voxel record output or copy operands differ")
    require({at for at, ins in nodes.items() if ins.mnemonic == "rep movsd"} ==
            {0x6ff7db, 0x6ffbe5, 0x6ffce7, 0x6ffda2, 0x6ffdcd, 0x700297},
            "Voxel record copy sequence differs")


def optional_voxel_seed(raw, inventory, repository):
    if hashlib.sha256(raw.encode()).hexdigest() != RAW_SHA256:
        return None
    try:
        verify_native(inventory, repository)
    except (ValueError, KeyError, TypeError, OSError):
        return None
    return CANDIDATE


CANDIDATE = """// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from EA/OpenTS unit.cpp Unit_Draw_Voxel; EA GPL Section 7 terms apply.
// RA2-native composite caching and shadow order, original006FF690. Provisional.
#include "shared/unit.hpp"
#include "shared/matrix3d.hpp"
#include "shared/surface.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/ilocomotion.hpp"
#include "shared/cell_class.hpp"
#include "shared/map_view.hpp"
#include "shared/unknown_geometry.hpp"
extern bool unknown_voxel_cache_enabled_839a58;
extern void *unknown_cache_empty_b2b624;
// @ref ?unknown_voxel_cache_enabled_839a58@@3_NA=00839a58
// @ref ?unknown_cache_empty_b2b624@@3PAXA=00b2b624
Matrix3D __fastcall Unknown_00717F90();
inline Vector3 Unit_Matrix_Translation_006ff690(Matrix3D const &matrix)
{
    return Vector3(matrix.Row[0][3],matrix.Row[1][3],matrix.Row[2][3]);
}

inline int Unit_Dir_To_4_006ff690(UnknownFacingDir const &direction)
{
    return (((*(unsigned const *)&direction>>13)+1)>>1)%4u;
}

inline double Unit_Direction_Radians32_006ff690(UnknownFacingDir const &direction)
{
    return (Dir_To_32(direction)-8)*-0.19634954084936207;
}

inline bool Unit_Voxel_Loaded_006ff690(UnknownObjectPair const &voxel)
{
    return voxel.unknown_00!=0 && voxel.unknown_04!=0;
}

// The cache index's ordinary lookup, same API/body as shared/voxel_cache.hpp.
inline void *const &UnknownBuffer::operator[](int const &id) const
{
    if(Is_Present_0070a350(id)) return ((NodeElement const *)unknown_10)->Data;
    return unknown_cache_empty_b2b624;
}
// Native cache predicate: the key has its own by-value home, and either
// an invalid key or an absent cache refuses a lookup.
inline bool Unit_Voxel_Is_Cached(UnknownBuffer const *cache,int key)
{
    if(key==-1 || !cache) return false;
    return (*cache)[key]!=0;
}
void UnitClass::unknown_slot310(Point2D point,Rect window,int brightness)
{
    Point2D drawpoint=point;
    UnitTypeClass *type=Type;
    Matrix3D main_matrix;
    if(!unknown_slot245(unknown_player_a35db4)) type=(UnitTypeClass *)unknown_slot50(true);
    int frame=unknown_430%(int)type->unknown_b0.unknown_04->unknown_frame_count;
    Rect cliprect=window;
    Surface *old_surface=unknown_surface_83995c;
    int key=0;
    BSurface *voxel_surface=unknown_surface_ad0604;
    unknown_surface_83995c=voxel_surface;
    drawpoint=Point2D(128,128);
    cliprect=voxel_surface->Get_Rect();
    unknown_5bb=true;
    key=unknown_voxel_cache_enabled_839a58 ? 0 : -1;
    Matrix3D draw_result;
    main_matrix=*Locomotor.Checked()->Draw_Matrix(&draw_result,&key);
    if(key!=-1) key=(int)((unsigned)key<<5)|(frame&31);
    int shadow_facing=(key>>5)&1023;
    Rect rect=cliprect;
    unknown_unit_rect_ad0490=unknown_static_rect;
    if(Type->unknown_1b8 && SpawnManager->Docked_Count()==0) {
        key|=0x10000;
        unknown_slot246(type->unknown_b8,frame,key,&type->unknown_214,rect,drawpoint,
                        Unknown_00717F90()*main_matrix,brightness,0x2800);
    } else {
        unknown_slot246(type->unknown_b0,frame,key,&type->unknown_214,rect,drawpoint,
                        Unknown_00717F90()*main_matrix,brightness,0x2800);
    }
    int turret_key=key;
    if(turret_key!=-1) {
        turret_key=(turret_key&~31)|Dir_To_32(unknown_2f8.Current());
        if(((TechnoTypeClass *)unknown_slot32())->TurretOffset==0) turret_key&=~0x3e0;
    }
    if(!type->Turret && (!unknown_6d83e0() || !unknown_6d8460())) {
        if(Unit_Voxel_Loaded_006ff690(type->unknown_c0)) {
            main_matrix.Rotate_Y((float)-(Unit_Direction_Radians32_006ff690(unknown_2c8.Current())-main_matrix.Get_Y_Rotation()));
            main_matrix.Translate((float)(unknown_slot231(0)->FLH.X*unknown_geometry_c),0,
                                  (float)(unknown_slot231(0)->FLH.Z*unknown_geometry_c));
            unknown_slot246(type->unknown_c0,frame,turret_key,&type->unknown_250,cliprect,drawpoint,
                            Unknown_00717F90()*main_matrix,brightness,0);
        }
    } else {
        int turret_index=0;
        bool barrel_loaded;
        if(unknown_6d83e0()) {
            turret_index=unknown_6d8470();
            turret_key|=turret_index<<16;
            barrel_loaded=Unit_Voxel_Loaded_006ff690(type->unknown_140[turret_index]);
        } else barrel_loaded=Unit_Voxel_Loaded_006ff690(type->unknown_c0);
        bool turret_cached=Unit_Voxel_Is_Cached(&type->unknown_228,turret_key);
        bool barrel_cached=barrel_loaded && Unit_Voxel_Is_Cached(&type->unknown_250,turret_key);
        Matrix3D turret_matrix,barrel_matrix,barrel_working;
        if(turret_cached && (!barrel_loaded || barrel_cached)) {
            turret_matrix=main_matrix;
            barrel_matrix=main_matrix;
        } else {
            main_matrix.Translate_X((float)(type->TurretOffset*unknown_geometry_c));
            main_matrix.Rotate_Z((float)(Unit_Direction_Radians32_006ff690(unknown_2f8.Current())-
                                        Unit_Direction_Radians32_006ff690(unknown_2e0.Current())));
            barrel_working=main_matrix;
            Vector3 translation=Unit_Matrix_Translation_006ff690(main_matrix);
            barrel_working.Translate(Vector3(-translation.X, -translation.Y, -translation.Z));
            barrel_working.Rotate_Y((float)-Unit_Direction_Radians32_006ff690(unknown_2c8.Current()));
            barrel_working.Translate(translation);
            turret_matrix=Unknown_00717F90()*main_matrix;
            barrel_matrix=Unknown_00717F90()*barrel_working;
        }
        bool barrel_above;
        if(unknown_6d83e0()) {
            if(Unit_Dir_To_4_006ff690(unknown_2f8.Current())>0 && Unit_Dir_To_4_006ff690(unknown_2f8.Current())<3) barrel_above=true;
            else {
                barrel_above=false;
                if(barrel_loaded) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_c8[turret_index],frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if(barrel_above && barrel_loaded) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        } else {
            if(Unit_Dir_To_4_006ff690(unknown_2f8.Current())>0 && Unit_Dir_To_4_006ff690(unknown_2f8.Current())<3) barrel_above=true;
            else {
                barrel_above=false;
                if(Unit_Voxel_Loaded_006ff690(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_b8,frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if(barrel_above && Unit_Voxel_Loaded_006ff690(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        }
    }
    rect=window;
    if(unknown_322>0) rect=Intersect(rect,Rect(0,0,unknown_int_839638,unknown_322-unknown_tactical_83996c->TacPixelY));
    else if(unknown_325) unknown_322=(short)(point.Y+unknown_tactical_83996c->TacPixelY+unknown_unit_rect_ad0490.Height+unknown_unit_rect_ad0490.Y-128);
    int apparent_brightness=unknown_slot254(brightness);
    if(unknown_slot203()) apparent_brightness=unknown_6d86f0(apparent_brightness);
    unknown_slot312(*old_surface,point,rect,apparent_brightness);
    Rect saved_composite=unknown_unit_rect_ad0490;
    Surface *composite=unknown_surface_83995c;
    unknown_surface_83995c=old_surface;
    unknown_5bb=false;
    if(!unknown_325) {
        Point2D shadow_point=point;
        if(Type->BalloonHover || Type->SpeedType==1 || Type->SpeedType==2) key=shadow_facing;
        else key=(shadow_facing<<5)|((Locomotor.Checked()->Shadow_Point().Y-16)&31);
        Matrix3D shadow_result;
        main_matrix=*Locomotor.Checked()->Shadow_Matrix(&shadow_result,&key);
        if(type->HunterSeeker) {
            CoordStruct coord=Position_Coord_004f3f40();
            coord.Z=Map.Get_Height_GL(coord);
            unknown_tactical_83996c->Coord_To_Pixel(coord,shadow_point);
            shadow_point+=Locomotor.Checked()->Shadow_Point();
        }
        if(unknown_37c) shadow_point.Y-=14;
        int height=unknown_slot108();
        CellClass *cell=&Map[Position_Coord_004f3f40()];
        if(unknown_37c && !unknown_79 && height>=unknown_geometry_j) {
            if((cell->unknown_140&0x100) && (((cell->unknown_140&0x800) && (cell->Adjacent_Cell((unsigned)0).unknown_140&0x100)) ||
               (!(cell->unknown_140&0x800) && (cell->Adjacent_Cell((unsigned)6).unknown_140&0x100)))) shadow_point.Y-=60;
        }
        Surface *shadow_surface;
        if(unknown_slot21()) shadow_surface=0;
        else {
            shadow_surface=composite;
        if(Type->BalloonHover || Type->SpeedType==1 || Type->SpeedType==2) {
            if(unknown_slot108()<=0) key|=0x80000000u;
            else key=-1;
        }
        }
        if(unknown_slot108()>0) {
            unknown_4c9a40(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_point,
                          Unknown_00717F90()*main_matrix,true,shadow_surface,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y+1);
        } else {
            unknown_4c9a40(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_point,
                          Unknown_00717F90()*main_matrix,true,shadow_surface,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y);
        }
    }
    composite->Fill_Rect(saved_composite,0);
}
"""
