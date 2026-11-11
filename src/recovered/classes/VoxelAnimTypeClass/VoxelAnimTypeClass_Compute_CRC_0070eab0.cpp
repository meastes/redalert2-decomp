// void VoxelAnimTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x0070EAB0)
#include "shared/voxel_anim_type.hpp"
// @ref ?Compute_CRC@AbstractTypeClass@@UBEXAAVWWCRCEngine@@@Z=00410960
// @ref ??RWWCRCEngine@@QAEX_N@Z=00493e80
// @ref ??RWWCRCEngine@@QAEXJ@Z=00493f30
// @ref ??RWWCRCEngine@@QAEXN@Z=00493f70

void VoxelAnimTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectTypeClass::Compute_CRC(crc);
    crc(Normalized);
    crc(Translucent);
    crc(unknown_266);
    crc((long)VoxelIndex);
    crc((long)Duration);
    crc(Elasticity);
    crc(MinAngularVelocity);
    crc(MaxAngularVelocity);
    crc(MinZVel);
    crc(MaxZVel);
    crc(MaxXYVel);
    crc(IsMeteor);
    crc((long)SpawnCount);
    crc((long)StartSound);
    crc((long)StopSound);
    crc((long)Damage);
    crc((long)DamageRadius);
    crc(IsTiberium);
}
