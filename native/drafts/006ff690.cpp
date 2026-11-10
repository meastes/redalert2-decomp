// NON-EXACT DRAFT 0x6FF690 FUN_006ff690
// entry 0x6FF690; exact-match score 86.76 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout974 (build/class-work/Scout974/best_8676.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// The shadow mask surface (cache_key) is a Surface * since Scout1102 r2 (shadow draw 0x004C9A40 takes Surface *mask).
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// From TibSun code/unit.cpp (codex4/codex2 drafts); Is_Cached, Is_Barrel_Ahead and the shadow-key
// goto form from ra2-decomp 0x0073B470 (UnitClass::Unit_Draw_Voxel). Adapted to the exact RA2 006FF690 body.
// Contains material derived from Electronic Arts source code; EA GPLv3 Section 7 terms apply.
#include "shared/unit.hpp"
#include "shared/surface.hpp"
#include "shared/matrix3d.hpp"
#include "shared/weapon_type.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
Matrix3D __fastcall Unit_Isometric_View_00717f90();
inline Vector3 operator-(Vector3 const &vector);
inline Vector3 Unit_Matrix_Translation_007005b0(Matrix3D const &matrix);
inline int Unit_Dir_To_4_007005b0(UnknownFacingDir const &direction);
inline double Unit_Direction_Radians32_007005b0(UnknownFacingDir const &direction);
inline bool Unit_Voxel_Loaded_007005b0(UnknownObjectPair const &voxel);
extern bool unknown_use_voxel_cache_839a58;
extern HouseClass *unknown_player_house_a35db4;
extern int unknown_bridge_height_ad057c;
extern void *unknown_cache_empty_b2b624;
// @ref ?Unit_Isometric_View_00717f90@@=00717f90
// @ref ?unknown_use_voxel_cache_839a58@@=00839a58
// @ref ?unknown_player_house_a35db4@@=00a35db4
// @ref ?unknown_bridge_height_ad057c@@=00ad057c
// @ref ?unknown_cache_empty_b2b624@@=00b2b624
// TibSun face.h direction accessors (twin: Current().As_Radian32())
inline unsigned UnknownFacingDir::Round_To_32() const
{
    return ((((*(unsigned const *)this)>>10)+1)>>1);
}
inline Dir32 UnknownFacingDir::As_Dir32() const
{
    return (Dir32)(Round_To_32()%32U);
}
inline double UnknownFacingDir::As_Radian32() const
{
    return (double)((As_Dir32()-8)*-0.19634954084936207);
}
inline void * const &UnknownBuffer::operator[](int const &id) const
{
    if (Is_Present_0070a350(id)) return ((NodeElement const *)unknown_10)->Data;
    return unknown_cache_empty_b2b624;
}
// ra2-decomp unit.cpp Is_Cached shape (twin-shaped helper)
static inline bool Unit_Is_Cached_006ff690(UnknownBuffer *cache,int key)
{
    if (key!=-1 && cache!=0) {
        return((*cache)[key]!=0);
    }
    return(false);
}
static inline bool Unit_Is_Barrel_Ahead_006ff690(FacingClass const &facing)
{
    return(Unit_Dir_To_4_007005b0(facing.Current())>0 && Unit_Dir_To_4_007005b0(facing.Current())<3);
}
void UnitClass::unknown_slot310(Point2D point,Rect window,int brightness)
{
    Point2D drawpoint=point;
    TechnoTypeClass *type=Type;
    if (!unknown_slot245(unknown_player_house_a35db4)) type=static_cast<TechnoTypeClass *>(unknown_slot50(true));
    int frame=unknown_430 % (int)type->unknown_b0.unknown_04->unknown_frame_count;
    Rect cliprect=window;
    Surface *old_surface=unknown_surface_83995c;
    int key=0;
    Matrix3D main_matrix;
    unknown_surface_83995c=unknown_surface_ad0604;
    drawpoint=Point2D(128,128);
    cliprect=unknown_surface_83995c->Get_Rect();
    unknown_5bb=true;
    key=unknown_use_voxel_cache_839a58 ? 0 : -1;
    {
        Matrix3D draw_result;
        Matrix3D converted(*Locomotor.Checked()->Draw_Matrix(&draw_result,&key));
        main_matrix=converted;
    }
    if (key!=-1) key=(key<<5)|(frame&31);
    int hull_facing=(key>>5)&0x3ff;
    Rect rect=cliprect;
    unknown_unit_rect_ad0490=unknown_static_rect;
    if (Type->unknown_1b8 && SpawnManager->Docked_Count()==0) {
        key|=0x10000;
        unknown_slot246(type->unknown_b8,frame,key,&type->unknown_214,rect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0x2800);
    } else {
        unknown_slot246(type->unknown_b0,frame,key,&type->unknown_214,rect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0x2800);
    }
    int turret_key=key;
    if (turret_key!=-1) {
        turret_key=(turret_key&~31)|unknown_2f8.Current().As_Dir32();
        if (unknown_slot32()->TurretOffset==0) turret_key&=0xfffffc1f;
    }
    if (!type->Turret && (!unknown_6d83e0() || !unknown_6d8460())) {
        if (Unit_Voxel_Loaded_007005b0(type->unknown_c0)) {
            main_matrix.Rotate_Y((float)-(unknown_2c8.Current().As_Radian32()-main_matrix.Get_Y_Rotation()));
            main_matrix.Translate((float)(unknown_slot231(0)->FLH.X*unknown_geometry_c),0.0f,(float)(unknown_slot231(0)->FLH.Z*unknown_geometry_c));
            unknown_slot246(type->unknown_c0,frame,turret_key,&type->unknown_250,cliprect,drawpoint,Unit_Isometric_View_00717f90()*main_matrix,brightness,0);
        }
    } else {
        int turret_index=0;
        bool has_barrel;
        if (unknown_6d83e0()) {
            turret_index=unknown_6d8470();
            turret_key|=turret_index<<16;
            has_barrel=Unit_Voxel_Loaded_007005b0(type->unknown_140[turret_index]);
        } else has_barrel=Unit_Voxel_Loaded_007005b0(type->unknown_c0);
        bool cached_turret=Unit_Is_Cached_006ff690(&type->unknown_228,turret_key);
        bool cached_barrel=has_barrel ? Unit_Is_Cached_006ff690(&type->unknown_250,turret_key) : false;
        Matrix3D turret_matrix;
        Matrix3D barrel_matrix;
        if (!cached_turret || (has_barrel && !cached_barrel)) {
            main_matrix.Translate_X((float)(type->TurretOffset*unknown_geometry_c));
            main_matrix.Rotate_Z((float)(unknown_2f8.Current().As_Radian32()-unknown_2e0.Current().As_Radian32()));
            Matrix3D barrel;
            barrel=main_matrix;
            Vector3 translation=Unit_Matrix_Translation_007005b0(main_matrix);
            barrel.Translate(-translation);
            barrel.Rotate_Y((float)-unknown_2c8.Current().As_Radian32());
            barrel.Translate(translation);
            turret_matrix=Unit_Isometric_View_00717f90()*main_matrix;
            barrel_matrix=Unit_Isometric_View_00717f90()*barrel;
        } else {
            turret_matrix=main_matrix;
            barrel_matrix=main_matrix;
        }
        bool barrel_above;
        if (unknown_6d83e0()) {
            if (Unit_Is_Barrel_Ahead_006ff690(unknown_2f8)) barrel_above=true;
            else {
                barrel_above=false;
                if (has_barrel) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_c8[turret_index],frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if (barrel_above && has_barrel) unknown_slot246(type->unknown_140[turret_index],0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        } else {
            if (Unit_Is_Barrel_Ahead_006ff690(unknown_2f8)) barrel_above=true;
            else {
                barrel_above=false;
                if (Unit_Voxel_Loaded_007005b0(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
            }
            unknown_slot246(type->unknown_b8,frame,turret_key,&type->unknown_228,cliprect,drawpoint,turret_matrix,brightness,0x2800);
            if (barrel_above && Unit_Voxel_Loaded_007005b0(type->unknown_c0)) unknown_slot246(type->unknown_c0,0,turret_key,&type->unknown_250,cliprect,drawpoint,barrel_matrix,brightness,0x2800);
        }
    }
    rect=window;
    if (unknown_322>0) rect=Intersect(rect,Rect(0,0,unknown_int_839638,unknown_322-unknown_tactical_83996c->TacPixelY));
    else if (unknown_325) unknown_322=(short)(unknown_tactical_83996c->TacPixelY+point.Y+unknown_unit_rect_ad0490.Y+unknown_unit_rect_ad0490.Height-128);
    int apparent_brightness=unknown_slot254(brightness);
    if (unknown_slot203()) apparent_brightness=unknown_6d86f0(apparent_brightness);
    unknown_slot312(*old_surface,point,rect,apparent_brightness);
    Rect dirtyrect=unknown_unit_rect_ad0490;
    Surface *composite_surface=unknown_surface_83995c;
    unknown_surface_83995c=old_surface;
    unknown_5bb=false;
    if (!unknown_325) {
        Point2D shadow_drawpoint=point;
        if (Type->BalloonHover) goto plain;
        if (Type->SpeedType==1) goto plain;
        if (Type->SpeedType==2) {
plain:
            key=hull_facing;
        } else {
            key=hull_facing<<5;
            key|=(Locomotor.Checked()->Shadow_Point().Y-16)&31;
        }
        {
            Matrix3D shadow_result;
            Matrix3D converted(*Locomotor.Checked()->Shadow_Matrix(&shadow_result,&key));
            main_matrix=converted;
        }
        if (type->HunterSeeker) {
            CoordStruct coord=Position_Coord_004f3f40();
            coord.Z=Map.Get_Height_GL(coord);
            unknown_tactical_83996c->Coord_To_Pixel(coord,shadow_drawpoint);
            shadow_drawpoint+=Locomotor.Checked()->Shadow_Point();
        }
        if (unknown_37c) shadow_drawpoint.Y-=14;
        int height=unknown_slot108();
        CellClass *cell= &Map[Position_Coord_004f3f40()];
        if (unknown_37c && !unknown_79 && height>=unknown_bridge_height_ad057c && (cell->unknown_140&0x100) &&
            (((cell->unknown_140&0x800) && (cell->Adjacent_Cell(0).unknown_140&0x100)) ||
             (!(cell->unknown_140&0x800) && (cell->Adjacent_Cell(6).unknown_140&0x100)))) shadow_drawpoint.Y-=60;
        Surface *cache_key;   // the shadow mask surface
        if (unknown_slot21()) cache_key=0;
        else {
            cache_key=composite_surface;
            if (Type->BalloonHover || Type->SpeedType==1 || Type->SpeedType==2) {
                if (unknown_slot108()<=0) key|=0x80000000;
                else key=-1;
            }
        }
        if (unknown_slot108()>0) {
            unknown_4c9a40(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_drawpoint,Unit_Isometric_View_00717f90()*main_matrix,true,cache_key,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y+1);
        } else {
            unknown_4c9a40(type->unknown_b0,type->ShadowIndex,key,&type->unknown_23c,window,shadow_drawpoint,Unit_Isometric_View_00717f90()*main_matrix,true,cache_key,Locomotor.Checked()->Shadow_Point().X,Locomotor.Checked()->Shadow_Point().Y);
        }
    }
    composite_surface->Fill_Rect(dirtyrect,0);
}
