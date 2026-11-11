// NON-EXACT DRAFT 0x70C800 FUN_0070c800
// entry 0x70C800; exact-match score 99.19 (misses_index best); plan "draft as-is"
// origin lane: Scout931 (build/class-work/Scout931/w/t2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// VoxelAnimClass::VoxelAnimClass(VoxelAnimTypeClass *type, CoordStruct const &coord, HouseClass *owner) (0x0070C800)
// A voxel animation of the type at the coordinate: joins the list, then
// either flies off from 10 leptons above the coordinate with a random
// velocity and spin, or (a meteor) starts far enough back along a random
// velocity to land on the coordinate when its time runs out; plays the start
// sound and attaches the type's particle system.
#include "shared/voxel_anim.hpp"
#include "shared/anim.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/particle_system.hpp"
#include "shared/building_light.hpp"
// @ref ??0ObjectClass@@QAE@XZ=005d22e0
// @ref ??0UnknownQuaternion@@QAE@MMMM@Z=00622160
// @ref ?Create_ID@AbstractClass@@UAGXXZ=0040ffd0
// @ref ?Init@UnknownAudioController@@QAEXXZ=00405bf0
// @ref ?unknown_VoxelAnimClass_list_008399d0@@3V?$DynamicVectorClass@PAVVoxelAnimClass@@@@A=008399d0
// @ref ??_7VoxelAnimClass@@6BIPersistStream@@@=007ae428
// @ref ??_7VoxelAnimClass@@6BIRTTITypeInfo@@@=007ae40c
// @ref ??_7VoxelAnimClass@@6BINoticeSink@@@=007ae404
// @ref ??_7VoxelAnimClass@@6BINoticeSource@@@=007ae3fc
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?Init@BounceClass@@QAEXABUCoordStruct@@NNNABUVector3@@N@Z=00436dc0

VoxelAnimClass::VoxelAnimClass(VoxelAnimTypeClass *type, CoordStruct const &coord, HouseClass *owner) :
    unknown_e8(-1),
    Class(type),
    unknown_f0(0),
    unknown_f4(owner),
    unknown_f8(false),
    unknown_124(false),
    unknown_128(0)
{
    AbstractClass::Create_ID();
    unknown_fc.Init();
    unknown_110.Init();
    unknown_128 = Class->Duration;
    unknown_VoxelAnimClass_list_008399d0.Add(this);
    if (!Class->IsMeteor) {
        ObjectClass::Unlimbo(&coord, 0);
        CoordStruct center = ObjectClass::Center_Coord() + CoordStruct(0, 0, 10);
        int spin_roll = unknown_id_source_a3d290->Random()();
        int z_roll = unknown_id_source_a3d290->Random()();
        int y_roll = unknown_id_source_a3d290->Random()();
        int x_roll = unknown_id_source_a3d290->Random()();
        float y = abs(y_roll) % (int)(Class->MaxXYVel + Class->MaxXYVel) - Class->MaxXYVel;
        float z = abs(z_roll) % (int)(Class->MaxZVel - Class->MinZVel + 1.0) + Class->MinZVel;
        float x = abs(x_roll) % (int)(Class->MaxXYVel + Class->MaxXYVel) - Class->MaxXYVel;
        Vector3 velocity(x, y, z);
        double rotation = abs(spin_roll) % (int)(Class->MaxAngularVelocity - Class->MinAngularVelocity + 1.0) + Class->MinAngularVelocity;
        BounceClass::Init(center, Class->Elasticity, 1.4f, 0, velocity, rotation);
    } else {
        int y_roll = unknown_id_source_a3d290->Random()();
        int x_roll = unknown_id_source_a3d290->Random()();
        Vector3 velocity(abs(x_roll) % (int)(Class->MaxXYVel + Class->MaxXYVel) - Class->MaxXYVel,
                         abs(y_roll) % (int)(Class->MaxXYVel + Class->MaxXYVel) - Class->MaxXYVel,
                         Class->MinZVel);
        if (velocity.X < -velocity.Y) {
            velocity.X = -velocity.X;
            velocity.Y = -velocity.Y;
        }
        unknown_128 -= abs(unknown_id_source_a3d290->Random()() % 20);
        int x = coord.X - unknown_128 * velocity.X;
        int y = coord.Y - unknown_128 * velocity.Y;
        int z = coord.Z - unknown_128 * velocity.Z;
        CoordStruct start(x, y, z);
        ObjectClass::Unlimbo(&start, 0);
        int spin_roll = unknown_id_source_a3d290->Random()();
        BounceClass::Init(ObjectClass::Center_Coord(), Class->Elasticity, 1.4f, 0, velocity,
            abs(spin_roll) % (int)(Class->MaxAngularVelocity - Class->MinAngularVelocity + 1.0) + Class->MinAngularVelocity);
    }
    if (Class->StartSound != -1) {
        Voc_Play_At(Class->StartSound, coord, &unknown_fc);
    } else {
        unknown_fc.Unknown_00406070();
    }
    unknown_110.Unknown_00405FE0();
    if (Class->AttachedSystem != 0) {
        unknown_f0 = new ParticleSystemClass(Class->AttachedSystem, coord, &Map[coord], (TechnoClass *)this, unknown_geometry_l);
    }
}
