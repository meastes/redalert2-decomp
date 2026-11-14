// Not exact (RA2_NON_MATCHING): VoxelAnimClass::VoxelAnimClass at 0x0070C800, best score 99.19, 2026-11-13; differs: same instruction count (371), shape 99.19: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
#define RA2_BOUNCE_DECLARE_MEMBERS
#define RA2_RADAR_ADD_THREADED

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
// Adapted from TibSun vanim.cpp VoxelAnimClass constructor for the exact RA2 target.
// See the upstream LICENSE.md for applicable additional terms and warranty disclaimers.
#include "shared/voxel_anim.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/particle_system.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crt_memory.hpp"

VoxelAnimClass::VoxelAnimClass(VoxelAnimTypeClass *type, CoordStruct const &coord, HouseClass *house) :
    unknown_e8(-1), Class(type), unknown_f0(0), unknown_f4(house), unknown_f8(false),
    unknown_124(false), unknown_128(0)
{
    AbstractClass::Create_ID();
    unknown_fc.Init();
    unknown_110.Init();
    unknown_128=Class->Duration;
    unknown_VoxelAnimClass_list_008399d0.Add(this);
    if (!Class->IsMeteor) {
        ObjectClass::Unlimbo(&coord,0);
        CoordStruct center=Center_Coord()+CoordStruct(0,0,10);
        int r1=unknown_id_source_a3d290->RandomNumber;
        int r2=unknown_id_source_a3d290->RandomNumber;
        int r3=unknown_id_source_a3d290->RandomNumber;
        int r4=unknown_id_source_a3d290->RandomNumber;
        float y=(abs(r3)%int(Class->MaxXYVel+Class->MaxXYVel))-Class->MaxXYVel;
        float z=(abs(r2)%int(Class->MaxZVel-Class->MinZVel+1.0))+Class->MinZVel;
        float x=(abs(r4)%int(Class->MaxXYVel+Class->MaxXYVel))-Class->MaxXYVel;
        Vector3 velocity(x,y,z);
        double rotation=(abs(r1)%int(Class->MaxAngularVelocity-Class->MinAngularVelocity+1.0))+Class->MinAngularVelocity;
        BounceClass::Init(center,Class->Elasticity,1.4f,0.0,velocity,rotation);
    } else {
        Vector3 velocity((abs(unknown_id_source_a3d290->RandomNumber)%int(Class->MaxXYVel+Class->MaxXYVel))-Class->MaxXYVel,
            (abs(unknown_id_source_a3d290->RandomNumber)%int(Class->MaxXYVel+Class->MaxXYVel))-Class->MaxXYVel,Class->MinZVel);
        if (velocity.X < -velocity.Y) {
            velocity.X=-velocity.X;
            velocity.Y=-velocity.Y;
        }
        unknown_128-=abs(unknown_id_source_a3d290->RandomNumber%20);
        int x=coord.X-unknown_128*velocity.X;
        int y=coord.Y-unknown_128*velocity.Y;
        int z=coord.Z-unknown_128*velocity.Z;
        CoordStruct origin(x,y,z);
        ObjectClass::Unlimbo(&origin,0);
        int random=unknown_id_source_a3d290->RandomNumber;
        BounceClass::Init(Center_Coord(),Class->Elasticity,1.4f,0.0,velocity,
            (abs(random)%int(Class->MaxAngularVelocity-Class->MinAngularVelocity+1.0))+Class->MinAngularVelocity);
    }
    if (Class->StartSound != -1) {
        Voc_Play_At(Class->StartSound,coord,&unknown_fc);
    } else {
        unknown_fc.Unknown_00406070();
    }
    unknown_110.Unknown_00405FE0();
    if (Class->AttachedSystem) {
        unknown_f0=new ParticleSystemClass(Class->AttachedSystem,coord,&Map[coord],this,unknown_geometry_l);
    }
}

#undef RA2_BOUNCE_DECLARE_MEMBERS
#undef RA2_RADAR_ADD_THREADED
#endif
