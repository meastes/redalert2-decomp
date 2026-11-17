// Header-static dynamic initializers of the unit at 0x004D2BA0 and its
// list globals (AITriggerTypeClass, AbstractTypeClass, AircraftClass, AircraftTypeClass, AnimClass, AnimTypeClass, BuildingClass, BuildingLightClass, BuildingTypeClass, BulletClass, BulletTypeClass, CampaignClass, FactoryClass, HouseClass, HouseTypeClass, InfantryClass, InfantryTypeClass, IsometricTileTypeClass, ObjectClass, OverlayClass, OverlayTypeClass, ParticleClass, ParticleSystemClass, ParticleSystemTypeClass, ParticleTypeClass, ScriptTypeClass, SideClass, SmudgeClass, SmudgeTypeClass, SuperClass, SuperWeaponTypeClass, TaskForceClass, TeamClass, TeamTypeClass, TechnoClass, TechnoTypeClass, TerrainClass, TerrainTypeClass, TriggerClass, TriggerTypeClass, TubeClass, UnitClass, UnitTypeClass, VoxelAnimTypeClass, WaveClass); the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class AITriggerTypeClass;
class TriggerTypeClass;
class TriggerClass;
class BuildingLightClass;
class ParticleSystemClass;
class TubeClass;
class WaveClass;
class BulletClass;
class OverlayClass;
class ParticleClass;
class SmudgeClass;
class TerrainClass;
class InfantryClass;
class BuildingClass;
class AircraftClass;
class UnitClass;
class HouseClass;
class AnimClass;
class TeamClass;
class FactoryClass;
class CampaignClass;
class SideClass;
class HouseTypeClass;
class TaskForceClass;
class TeamTypeClass;
class ScriptTypeClass;
class OverlayTypeClass;
class SmudgeTypeClass;
class TerrainTypeClass;
class BuildingTypeClass;
class AircraftTypeClass;
class UnitTypeClass;
class AnimTypeClass;
class InfantryTypeClass;
class BulletTypeClass;
class VoxelAnimTypeClass;
class IsometricTileTypeClass;
class ParticleTypeClass;
class ParticleSystemTypeClass;
class SuperWeaponTypeClass;
class SuperClass;
class TechnoClass;
class TechnoTypeClass;
class ObjectClass;
class AbstractTypeClass;

// The unit's list of AITriggerTypeClass (0x00A3D260; name is a hypothesis).
DynamicVectorClass<AITriggerTypeClass *> AITriggerTypes;
// The unit's list of TriggerTypeClass (0x00866208; name is a hypothesis).
DynamicVectorClass<TriggerTypeClass *> TriggerTypes;
// The unit's list of TriggerClass (0x00A40AA0; name is a hypothesis).
DynamicVectorClass<TriggerClass *> Triggers;
// The unit's list of BuildingLightClass (0x00866220; name is a hypothesis).
DynamicVectorClass<BuildingLightClass *> BuildingLights;
// The unit's list of ParticleSystemClass (0x00A32280; name is a hypothesis).
DynamicVectorClass<ParticleSystemClass *> ParticleSystems;
// The unit's list of TubeClass (0x008661C8; name is a hypothesis).
DynamicVectorClass<TubeClass *> Tubes;
// The unit's list of WaveClass (0x00A40BE8; name is a hypothesis).
DynamicVectorClass<WaveClass *> Waves;
// The unit's list of BulletClass (0x00A40CE8; name is a hypothesis).
DynamicVectorClass<BulletClass *> unknown_BulletClass_list_00a40ce8;
// The unit's list of OverlayClass (0x00A40C00; name is a hypothesis).
DynamicVectorClass<OverlayClass *> Overlays;
// The unit's list of ParticleClass (0x00A35E28; name is a hypothesis).
DynamicVectorClass<ParticleClass *> unknown_ParticleClass_list_00a35e28;
// The unit's list of SmudgeClass (0x00A3D240; name is a hypothesis).
DynamicVectorClass<SmudgeClass *> Smudges;
// The unit's list of TerrainClass (0x00A40940; name is a hypothesis).
DynamicVectorClass<TerrainClass *> Terrains;
// The unit's list of InfantryClass (0x00A35E48; name is a hypothesis).
DynamicVectorClass<InfantryClass *> Infantry;
// The unit's list of BuildingClass (0x00A40AF8; name is a hypothesis).
DynamicVectorClass<BuildingClass *> unknown_BuildingClass_list_00a40af8;
// The unit's list of AircraftClass (0x00A40398; name is a hypothesis).
DynamicVectorClass<AircraftClass *> Aircraft;
// The unit's list of UnitClass (0x00866198; name is a hypothesis).
DynamicVectorClass<UnitClass *> Units;
// The unit's list of HouseClass (0x00A32298; name is a hypothesis).
DynamicVectorClass<HouseClass *> Houses;
// The unit's list of AnimClass (0x00A40960; name is a hypothesis).
DynamicVectorClass<AnimClass *> Anims;
// The unit's list of TeamClass (0x00866178; name is a hypothesis).
DynamicVectorClass<TeamClass *> Teams;
// The unit's list of FactoryClass (0x00A35E90; name is a hypothesis).
DynamicVectorClass<FactoryClass *> Factories;
// The unit's list of CampaignClass (0x00A35D60; name is a hypothesis).
DynamicVectorClass<CampaignClass *> Campaigns;
// The unit's list of SideClass (0x008661B0; name is a hypothesis).
DynamicVectorClass<SideClass *> Sides;
// The unit's list of HouseTypeClass (0x00A35D08; name is a hypothesis).
DynamicVectorClass<HouseTypeClass *> HouseTypes;
// The unit's list of TaskForceClass (0x00A40890; name is a hypothesis).
DynamicVectorClass<TaskForceClass *> TaskForces;
// The unit's list of TeamTypeClass (0x00A40C48; name is a hypothesis).
DynamicVectorClass<TeamTypeClass *> TeamTypes;
// The unit's list of ScriptTypeClass (0x00866258; name is a hypothesis).
DynamicVectorClass<ScriptTypeClass *> ScriptTypes;
// The unit's list of OverlayTypeClass (0x00A35DE8; name is a hypothesis).
DynamicVectorClass<OverlayTypeClass *> OverlayTypes;
// The unit's list of SmudgeTypeClass (0x00A40BC8; name is a hypothesis).
DynamicVectorClass<SmudgeTypeClass *> SmudgeTypes;
// The unit's list of TerrainTypeClass (0x00A40320; name is a hypothesis).
DynamicVectorClass<TerrainTypeClass *> TerrainTypes;
// The unit's list of BuildingTypeClass (0x00A35CD8; name is a hypothesis).
DynamicVectorClass<BuildingTypeClass *> BuildingTypes;
// The unit's list of AircraftTypeClass (0x00A3D278; name is a hypothesis).
DynamicVectorClass<AircraftTypeClass *> AircraftTypes;
// The unit's list of UnitTypeClass (0x00A35D48; name is a hypothesis).
DynamicVectorClass<UnitTypeClass *> UnitTypes;
// The unit's list of AnimTypeClass (0x008661E0; name is a hypothesis).
DynamicVectorClass<AnimTypeClass *> unknown_AnimTypeClass_list_008661e0;
// The unit's list of InfantryTypeClass (0x00A40350; name is a hypothesis).
DynamicVectorClass<InfantryTypeClass *> InfantryTypes;
// The unit's list of BulletTypeClass (0x00A35CF0; name is a hypothesis).
DynamicVectorClass<BulletTypeClass *> BulletTypes;
// The unit's list of VoxelAnimTypeClass (0x00A40AE0; name is a hypothesis).
DynamicVectorClass<VoxelAnimTypeClass *> VoxelAnimTypes;
// The unit's list of IsometricTileTypeClass (0x00A40CD0; name is a hypothesis).
DynamicVectorClass<IsometricTileTypeClass *> IsoTileTypes;
// The unit's list of ParticleTypeClass (0x00A35E00; name is a hypothesis).
DynamicVectorClass<ParticleTypeClass *> ParticleTypes;
// The unit's list of ParticleSystemTypeClass (0x00A35DD0; name is a hypothesis).
DynamicVectorClass<ParticleSystemTypeClass *> ParticleSystemTypes;
// The unit's list of SuperWeaponTypeClass (0x00A40338; name is a hypothesis).
DynamicVectorClass<SuperWeaponTypeClass *> SuperWeaponTypes;
// The unit's list of SuperClass (0x00A35D28; name is a hypothesis).
DynamicVectorClass<SuperClass *> SuperWeapons;
// The unit's list of SuperClass (0x00A35DB8; name is a hypothesis).
DynamicVectorClass<SuperClass *> TimedSuperWeapons;
// The unit's list of TechnoClass (0x00A40C20; name is a hypothesis).
DynamicVectorClass<TechnoClass *> Technos;
// The unit's list of TechnoTypeClass (0x00A40AB8; name is a hypothesis).
DynamicVectorClass<TechnoTypeClass *> TechnoTypes;
// The unit's list of ObjectClass (0x00A40368; name is a hypothesis).
DynamicVectorClass<ObjectClass *> unknown_objects_a40368;
// The unit's list of AbstractTypeClass (0x00A40928; name is a hypothesis).
DynamicVectorClass<AbstractTypeClass *> AbstractTypes;
// The unit's list of AnimClass (0x00A35E60; name is a hypothesis).
DynamicVectorClass<AnimClass *> unknown_AnimClass_list_00a35e60;
// Later sources of this unit include headers that declare these lists extern (see RA2_GLOBAL_LISTS_DEFINED).
#define RA2_GLOBAL_LISTS_DEFINED


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TagClass *>::Clear (0x004D49A0; DynamicVectorClass<TagClass *> vtable 0x007a2ae4 slot 3).
// DynamicVectorClass<TagClass *>::ID_pointer (0x004D49D0; DynamicVectorClass<TagClass *> vtable 0x007a2ae4 slot 5).
// VectorClass<TagClass *>::Fetch (0x004D49F0; VectorClass<TagClass *> vtable 0x007a2b04 slot 6).
// DynamicVectorClass<BuildingClass *>::Clear (0x004D4A00; DynamicVectorClass<BuildingClass *> vtable 0x007a2364 slot 3).
// DynamicVectorClass<BuildingClass *>::ID_pointer (0x004D4A30; DynamicVectorClass<BuildingClass *> vtable 0x007a2364 slot 5).
// VectorClass<BuildingClass *>::Fetch (0x004D4A50; VectorClass<BuildingClass *> vtable 0x007a2384 slot 6).
// DynamicVectorClass<SuperClass *>::Clear (0x004D4A60; DynamicVectorClass<SuperClass *> vtable 0x007a2a24 slot 3).
// DynamicVectorClass<SuperClass *>::ID_pointer (0x004D4A90; DynamicVectorClass<SuperClass *> vtable 0x007a2a24 slot 5).
// VectorClass<SuperClass *>::Fetch (0x004D4AB0; VectorClass<SuperClass *> vtable 0x007a2a44 slot 6).
// DynamicVectorClass<AITriggerTypeClass *>::Clear (0x004D4AC0; DynamicVectorClass<AITriggerTypeClass *> vtable 0x007a20a4 slot 3).
// DynamicVectorClass<AITriggerTypeClass *>::ID_pointer (0x004D4AF0; DynamicVectorClass<AITriggerTypeClass *> vtable 0x007a20a4 slot 5).
// VectorClass<AITriggerTypeClass *>::Fetch (0x004D4B10; VectorClass<AITriggerTypeClass *> vtable 0x007a20c4 slot 6).
// DynamicVectorClass<BuildingLightClass *>::Clear (0x004D4B20; DynamicVectorClass<BuildingLightClass *> vtable 0x007a2164 slot 3).
// DynamicVectorClass<BuildingLightClass *>::ID_pointer (0x004D4B50; DynamicVectorClass<BuildingLightClass *> vtable 0x007a2164 slot 5).
// VectorClass<BuildingLightClass *>::Fetch (0x004D4B70; VectorClass<BuildingLightClass *> vtable 0x007a2184 slot 6).
// DynamicVectorClass<ParticleClass *>::Clear (0x004D4B80; DynamicVectorClass<ParticleClass *> vtable 0x007a22a4 slot 3).
// DynamicVectorClass<ParticleClass *>::ID_pointer (0x004D4BB0; DynamicVectorClass<ParticleClass *> vtable 0x007a22a4 slot 5).
// VectorClass<ParticleClass *>::Fetch (0x004D4BD0; VectorClass<ParticleClass *> vtable 0x007a22c4 slot 6).
// DynamicVectorClass<TeamClass *>::Clear (0x004D4BE0; DynamicVectorClass<TeamClass *> vtable 0x007a24a4 slot 3).
// DynamicVectorClass<TeamClass *>::ID_pointer (0x004D4C10; DynamicVectorClass<TeamClass *> vtable 0x007a24a4 slot 5).
// VectorClass<TeamClass *>::Fetch (0x004D4C30; VectorClass<TeamClass *> vtable 0x007a24c4 slot 6).
// DynamicVectorClass<TriggerClass *>::Clear (0x004D4C50; DynamicVectorClass<TriggerClass *> vtable 0x007a2124 slot 3).
// DynamicVectorClass<TriggerClass *>::ID_pointer (0x004D4C80; DynamicVectorClass<TriggerClass *> vtable 0x007a2124 slot 5).
// VectorClass<TriggerClass *>::Fetch (0x004D4CA0; VectorClass<TriggerClass *> vtable 0x007a2144 slot 6).
// DynamicVectorClass<TriggerTypeClass *>::Clear (0x004D4CB0; DynamicVectorClass<TriggerTypeClass *> vtable 0x007a20e4 slot 3).
// DynamicVectorClass<TriggerTypeClass *>::ID_pointer (0x004D4CE0; DynamicVectorClass<TriggerTypeClass *> vtable 0x007a20e4 slot 5).
// VectorClass<TriggerTypeClass *>::Fetch (0x004D4D00; VectorClass<TriggerTypeClass *> vtable 0x007a2104 slot 6).
// DynamicVectorClass<TubeClass *>::Clear (0x004D4D10; DynamicVectorClass<TubeClass *> vtable 0x007a21e4 slot 3).
// DynamicVectorClass<TubeClass *>::ID_pointer (0x004D4D40; DynamicVectorClass<TubeClass *> vtable 0x007a21e4 slot 5).
// VectorClass<TubeClass *>::Fetch (0x004D4D60; VectorClass<TubeClass *> vtable 0x007a2204 slot 6).
// DynamicVectorClass<ParticleSystemClass *>::Clear (0x004D4D70; DynamicVectorClass<ParticleSystemClass *> vtable 0x007a21a4 slot 3).
// DynamicVectorClass<ParticleSystemClass *>::ID_pointer (0x004D4DA0; DynamicVectorClass<ParticleSystemClass *> vtable 0x007a21a4 slot 5).
// VectorClass<ParticleSystemClass *>::Fetch (0x004D4DC0; VectorClass<ParticleSystemClass *> vtable 0x007a21c4 slot 6).
// DynamicVectorClass<WaveClass *>::Clear (0x004D4DD0; DynamicVectorClass<WaveClass *> vtable 0x007a2224 slot 3).
// DynamicVectorClass<WaveClass *>::ID_pointer (0x004D4E00; DynamicVectorClass<WaveClass *> vtable 0x007a2224 slot 5).
// VectorClass<WaveClass *>::Fetch (0x004D4E20; VectorClass<WaveClass *> vtable 0x007a2244 slot 6).
// DynamicVectorClass<OverlayClass *>::Clear (0x004D4E30; DynamicVectorClass<OverlayClass *> vtable 0x007a2264 slot 3).
// DynamicVectorClass<OverlayClass *>::ID_pointer (0x004D4E60; DynamicVectorClass<OverlayClass *> vtable 0x007a2264 slot 5).
// VectorClass<OverlayClass *>::Fetch (0x004D4E80; VectorClass<OverlayClass *> vtable 0x007a2284 slot 6).
// DynamicVectorClass<SmudgeClass *>::Clear (0x004D4E90; DynamicVectorClass<SmudgeClass *> vtable 0x007a22e4 slot 3).
// VectorClass<SmudgeClass *>::Fetch (0x004D4EE0; VectorClass<SmudgeClass *> vtable 0x007a2304 slot 6).
// DynamicVectorClass<TerrainClass *>::Clear (0x004D4EF0; DynamicVectorClass<TerrainClass *> vtable 0x007a2324 slot 3).
// DynamicVectorClass<AircraftClass *>::Clear (0x004D4F50; DynamicVectorClass<AircraftClass *> vtable 0x007a23a4 slot 3).
// VectorClass<AircraftClass *>::Fetch (0x004D4FA0; VectorClass<AircraftClass *> vtable 0x007a23c4 slot 6).
// DynamicVectorClass<UnitClass *>::Clear (0x004D4FB0; DynamicVectorClass<UnitClass *> vtable 0x007a23e4 slot 3).
// VectorClass<UnitClass *>::Fetch (0x004D5000; VectorClass<UnitClass *> vtable 0x007a2404 slot 6).
// DynamicVectorClass<HouseClass *>::Clear (0x004D5010; DynamicVectorClass<HouseClass *> vtable 0x007a2424 slot 3).
// VectorClass<HouseClass *>::Fetch (0x004D5060; VectorClass<HouseClass *> vtable 0x007a2444 slot 6).
// DynamicVectorClass<AnimClass *>::Clear (0x004D5070; DynamicVectorClass<AnimClass *> vtable 0x007a2464 slot 3).
// VectorClass<AnimClass *>::Fetch (0x004D50C0; VectorClass<AnimClass *> vtable 0x007a2484 slot 6).
// DynamicVectorClass<FactoryClass *>::Clear (0x004D50D0; DynamicVectorClass<FactoryClass *> vtable 0x007a24e4 slot 3).
// VectorClass<FactoryClass *>::Fetch (0x004D5120; VectorClass<FactoryClass *> vtable 0x007a2504 slot 6).
// DynamicVectorClass<CampaignClass *>::Clear (0x004D5130; DynamicVectorClass<CampaignClass *> vtable 0x007a2524 slot 3).
// VectorClass<CampaignClass *>::Fetch (0x004D5180; VectorClass<CampaignClass *> vtable 0x007a2544 slot 6).
// DynamicVectorClass<SideClass *>::Clear (0x004D5190; DynamicVectorClass<SideClass *> vtable 0x007a2564 slot 3).
// DynamicVectorClass<HouseTypeClass *>::Clear (0x004D51F0; DynamicVectorClass<HouseTypeClass *> vtable 0x007a25a4 slot 3).
// VectorClass<HouseTypeClass *>::Fetch (0x004D5240; VectorClass<HouseTypeClass *> vtable 0x007a25c4 slot 6).
// DynamicVectorClass<TaskForceClass *>::Clear (0x004D5250; DynamicVectorClass<TaskForceClass *> vtable 0x007a25e4 slot 3).
// VectorClass<TaskForceClass *>::Fetch (0x004D52A0; DynamicVectorClass<TaskForceClass *> vtable 0x007a25e4 slot 6).
// DynamicVectorClass<TeamTypeClass *>::Clear (0x004D52B0; DynamicVectorClass<TeamTypeClass *> vtable 0x007a2624 slot 3).
// VectorClass<TeamTypeClass *>::Fetch (0x004D5300; VectorClass<TeamTypeClass *> vtable 0x007a2644 slot 6).
// DynamicVectorClass<ScriptTypeClass *>::Clear (0x004D5310; DynamicVectorClass<ScriptTypeClass *> vtable 0x007a2664 slot 3).
// VectorClass<ScriptTypeClass *>::Fetch (0x004D5360; VectorClass<ScriptTypeClass *> vtable 0x007a2684 slot 6).
// DynamicVectorClass<OverlayTypeClass *>::Clear (0x004D5370; DynamicVectorClass<OverlayTypeClass *> vtable 0x007a26a4 slot 3).
// VectorClass<OverlayTypeClass *>::Fetch (0x004D53C0; VectorClass<OverlayTypeClass *> vtable 0x007a26c4 slot 6).
// DynamicVectorClass<SmudgeTypeClass *>::Clear (0x004D53D0; DynamicVectorClass<SmudgeTypeClass *> vtable 0x007a26e4 slot 3).
// VectorClass<SmudgeTypeClass *>::Fetch (0x004D5420; VectorClass<SmudgeTypeClass *> vtable 0x007a2704 slot 6).
// DynamicVectorClass<TerrainTypeClass *>::Clear (0x004D5430; DynamicVectorClass<TerrainTypeClass *> vtable 0x007a2724 slot 3).
// VectorClass<TerrainTypeClass *>::Fetch (0x004D5480; VectorClass<TerrainTypeClass *> vtable 0x007a2744 slot 6).
// DynamicVectorClass<BuildingTypeClass *>::Clear (0x004D5490; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2764 slot 3).
// VectorClass<BuildingTypeClass *>::Fetch (0x004D54E0; VectorClass<BuildingTypeClass *> vtable 0x007a2784 slot 6).
// DynamicVectorClass<AircraftTypeClass *>::Clear (0x004D54F0; DynamicVectorClass<AircraftTypeClass *> vtable 0x007a27a4 slot 3).
// VectorClass<AircraftTypeClass *>::Fetch (0x004D5540; VectorClass<AircraftTypeClass *> vtable 0x007a27c4 slot 6).
// DynamicVectorClass<UnitTypeClass *>::Clear (0x004D5550; DynamicVectorClass<UnitTypeClass *> vtable 0x007a27e4 slot 3).
// VectorClass<UnitTypeClass *>::Fetch (0x004D55A0; VectorClass<UnitTypeClass *> vtable 0x007a2804 slot 6).
// DynamicVectorClass<AnimTypeClass *>::Clear (0x004D55B0; DynamicVectorClass<AnimTypeClass *> vtable 0x007a2824 slot 3).
// VectorClass<AnimTypeClass *>::Fetch (0x004D5600; VectorClass<AnimTypeClass *> vtable 0x007a2844 slot 6).
// DynamicVectorClass<InfantryTypeClass *>::Clear (0x004D5610; DynamicVectorClass<InfantryTypeClass *> vtable 0x007a2864 slot 3).
// VectorClass<InfantryTypeClass *>::Fetch (0x004D5660; VectorClass<InfantryTypeClass *> vtable 0x007a2884 slot 6).
// DynamicVectorClass<BulletTypeClass *>::Clear (0x004D5670; DynamicVectorClass<BulletTypeClass *> vtable 0x007a28a4 slot 3).
// VectorClass<BulletTypeClass *>::Fetch (0x004D56C0; VectorClass<BulletTypeClass *> vtable 0x007a28c4 slot 6).
// DynamicVectorClass<VoxelAnimTypeClass *>::Clear (0x004D56D0; DynamicVectorClass<VoxelAnimTypeClass *> vtable 0x007a28e4 slot 3).
// VectorClass<VoxelAnimTypeClass *>::Fetch (0x004D5720; VectorClass<VoxelAnimTypeClass *> vtable 0x007a2904 slot 6).
// DynamicVectorClass<IsometricTileTypeClass *>::Clear (0x004D5730; DynamicVectorClass<IsometricTileTypeClass *> vtable 0x007a2924 slot 3).
// VectorClass<IsometricTileTypeClass *>::Fetch (0x004D5780; VectorClass<IsometricTileTypeClass *> vtable 0x007a2944 slot 6).
// DynamicVectorClass<ParticleTypeClass *>::Clear (0x004D5790; DynamicVectorClass<ParticleTypeClass *> vtable 0x007a2964 slot 3).
// VectorClass<ParticleTypeClass *>::Fetch (0x004D57E0; VectorClass<ParticleTypeClass *> vtable 0x007a2984 slot 6).
// DynamicVectorClass<ParticleSystemTypeClass *>::Clear (0x004D57F0; DynamicVectorClass<ParticleSystemTypeClass *> vtable 0x007a29a4 slot 3).
// VectorClass<ParticleSystemTypeClass *>::Fetch (0x004D5840; VectorClass<ParticleSystemTypeClass *> vtable 0x007a29c4 slot 6).
// DynamicVectorClass<SuperWeaponTypeClass *>::Clear (0x004D5850; DynamicVectorClass<SuperWeaponTypeClass *> vtable 0x007a29e4 slot 3).
// VectorClass<SuperWeaponTypeClass *>::Fetch (0x004D58A0; VectorClass<SuperWeaponTypeClass *> vtable 0x007a2a04 slot 6).
// DynamicVectorClass<AbstractTypeClass *>::Clear (0x004D58B0; DynamicVectorClass<AbstractTypeClass *> vtable 0x007a2a64 slot 3).
// VectorClass<AbstractTypeClass *>::Fetch (0x004D5900; VectorClass<AbstractTypeClass *> vtable 0x007a2a84 slot 6).
// VectorClass<TagClass *>::operator equal (0x004D5910; VectorClass<TagClass *> vtable 0x007a2b04 slot 1).
// VectorClass<TagClass *>::Resize (0x004D5950; VectorClass<TagClass *> vtable 0x007a2b04 slot 2).
// VectorClass<TagClass *>::Clear (0x004D5A00; VectorClass<TagClass *> vtable 0x007a2b04 slot 3).
// VectorClass<TagClass *>::ID_object (0x004D5A30; VectorClass<TagClass *> vtable 0x007a2b04 slot 4).
// DynamicVectorClass<TagClass *>::Resize (0x004D5A90; DynamicVectorClass<TagClass *> vtable 0x007a2ae4 slot 2).
// DynamicVectorClass<TagClass *>::ID_object (0x004D5B40; DynamicVectorClass<TagClass *> vtable 0x007a2ae4 slot 4).
// VectorClass<BuildingClass *>::operator equal (0x004D5B70; VectorClass<BuildingClass *> vtable 0x007a2384 slot 1).
// VectorClass<BuildingClass *>::Resize (0x004D5BB0; VectorClass<BuildingClass *> vtable 0x007a2384 slot 2).
// VectorClass<BuildingClass *>::Clear (0x004D5C60; VectorClass<BuildingClass *> vtable 0x007a2384 slot 3).
// VectorClass<BuildingClass *>::ID_object (0x004D5C90; VectorClass<BuildingClass *> vtable 0x007a2384 slot 4).
// DynamicVectorClass<BuildingClass *>::Resize (0x004D5CF0; DynamicVectorClass<BuildingClass *> vtable 0x007a2364 slot 2).
// DynamicVectorClass<BuildingClass *>::ID_object (0x004D5DA0; DynamicVectorClass<BuildingClass *> vtable 0x007a2364 slot 4).
// VectorClass<SuperClass *>::operator equal (0x004D5DD0; VectorClass<SuperClass *> vtable 0x007a2a44 slot 1).
// VectorClass<SuperClass *>::Resize (0x004D5E10; VectorClass<SuperClass *> vtable 0x007a2a44 slot 2).
// VectorClass<SuperClass *>::Clear (0x004D5EC0; VectorClass<SuperClass *> vtable 0x007a2a44 slot 3).
// VectorClass<SuperClass *>::ID_object (0x004D5EF0; VectorClass<SuperClass *> vtable 0x007a2a44 slot 4).
// DynamicVectorClass<SuperClass *>::Resize (0x004D5F50; DynamicVectorClass<SuperClass *> vtable 0x007a2a24 slot 2).
// DynamicVectorClass<SuperClass *>::ID_object (0x004D6000; DynamicVectorClass<SuperClass *> vtable 0x007a2a24 slot 4).
// VectorClass<AITriggerTypeClass *>::operator equal (0x004D6030; VectorClass<AITriggerTypeClass *> vtable 0x007a20c4 slot 1).
// VectorClass<AITriggerTypeClass *>::Resize (0x004D6070; VectorClass<AITriggerTypeClass *> vtable 0x007a20c4 slot 2).
// VectorClass<AITriggerTypeClass *>::Clear (0x004D6120; VectorClass<AITriggerTypeClass *> vtable 0x007a20c4 slot 3).
// VectorClass<AITriggerTypeClass *>::ID_object (0x004D6150; VectorClass<AITriggerTypeClass *> vtable 0x007a20c4 slot 4).
// DynamicVectorClass<AITriggerTypeClass *>::Resize (0x004D61B0; DynamicVectorClass<AITriggerTypeClass *> vtable 0x007a20a4 slot 2).
// DynamicVectorClass<AITriggerTypeClass *>::ID_object (0x004D6260; DynamicVectorClass<AITriggerTypeClass *> vtable 0x007a20a4 slot 4).
// VectorClass<BuildingLightClass *>::operator equal (0x004D6290; VectorClass<BuildingLightClass *> vtable 0x007a2184 slot 1).
// VectorClass<BuildingLightClass *>::Resize (0x004D62D0; VectorClass<BuildingLightClass *> vtable 0x007a2184 slot 2).
// VectorClass<BuildingLightClass *>::Clear (0x004D6380; VectorClass<BuildingLightClass *> vtable 0x007a2184 slot 3).
// VectorClass<BuildingLightClass *>::ID_object (0x004D63B0; VectorClass<BuildingLightClass *> vtable 0x007a2184 slot 4).
// DynamicVectorClass<BuildingLightClass *>::Resize (0x004D6410; DynamicVectorClass<BuildingLightClass *> vtable 0x007a2164 slot 2).
// DynamicVectorClass<BuildingLightClass *>::ID_object (0x004D64C0; DynamicVectorClass<BuildingLightClass *> vtable 0x007a2164 slot 4).
// VectorClass<ParticleClass *>::operator equal (0x004D64F0; VectorClass<ParticleClass *> vtable 0x007a22c4 slot 1).
// VectorClass<ParticleClass *>::Resize (0x004D6530; VectorClass<ParticleClass *> vtable 0x007a22c4 slot 2).
// VectorClass<ParticleClass *>::Clear (0x004D65E0; VectorClass<ParticleClass *> vtable 0x007a22c4 slot 3).
// VectorClass<ParticleClass *>::ID_object (0x004D6610; VectorClass<ParticleClass *> vtable 0x007a22c4 slot 4).
// DynamicVectorClass<ParticleClass *>::Resize (0x004D6670; DynamicVectorClass<ParticleClass *> vtable 0x007a22a4 slot 2).
// DynamicVectorClass<ParticleClass *>::ID_object (0x004D6720; DynamicVectorClass<ParticleClass *> vtable 0x007a22a4 slot 4).
// VectorClass<TeamClass *>::operator equal (0x004D6750; VectorClass<TeamClass *> vtable 0x007a24c4 slot 1).
// VectorClass<TeamClass *>::Resize (0x004D6790; VectorClass<TeamClass *> vtable 0x007a24c4 slot 2).
// VectorClass<TeamClass *>::Clear (0x004D6840; VectorClass<TeamClass *> vtable 0x007a24c4 slot 3).
// VectorClass<TeamClass *>::ID_object (0x004D6870; VectorClass<TeamClass *> vtable 0x007a24c4 slot 4).
// DynamicVectorClass<TeamClass *>::Resize (0x004D68D0; DynamicVectorClass<TeamClass *> vtable 0x007a24a4 slot 2).
// DynamicVectorClass<TeamClass *>::ID_object (0x004D6980; DynamicVectorClass<TeamClass *> vtable 0x007a24a4 slot 4).
// VectorClass<TriggerClass *>::operator equal (0x004D6B30; VectorClass<TriggerClass *> vtable 0x007a2144 slot 1).
// VectorClass<TriggerClass *>::Resize (0x004D6B70; VectorClass<TriggerClass *> vtable 0x007a2144 slot 2).
// VectorClass<TriggerClass *>::Clear (0x004D6C20; VectorClass<TriggerClass *> vtable 0x007a2144 slot 3).
// VectorClass<TriggerClass *>::ID_object (0x004D6C50; VectorClass<TriggerClass *> vtable 0x007a2144 slot 4).
// DynamicVectorClass<TriggerClass *>::Resize (0x004D6CB0; DynamicVectorClass<TriggerClass *> vtable 0x007a2124 slot 2).
// DynamicVectorClass<TriggerClass *>::ID_object (0x004D6D60; DynamicVectorClass<TriggerClass *> vtable 0x007a2124 slot 4).
// VectorClass<TriggerTypeClass *>::operator equal (0x004D6D90; VectorClass<TriggerTypeClass *> vtable 0x007a2104 slot 1).
// VectorClass<TriggerTypeClass *>::Resize (0x004D6DD0; VectorClass<TriggerTypeClass *> vtable 0x007a2104 slot 2).
// VectorClass<TriggerTypeClass *>::Clear (0x004D6E80; VectorClass<TriggerTypeClass *> vtable 0x007a2104 slot 3).
// VectorClass<TriggerTypeClass *>::ID_object (0x004D6EB0; VectorClass<TriggerTypeClass *> vtable 0x007a2104 slot 4).
// DynamicVectorClass<TriggerTypeClass *>::Resize (0x004D6F10; DynamicVectorClass<TriggerTypeClass *> vtable 0x007a20e4 slot 2).
// DynamicVectorClass<TriggerTypeClass *>::ID_object (0x004D6FC0; DynamicVectorClass<TriggerTypeClass *> vtable 0x007a20e4 slot 4).
// VectorClass<TubeClass *>::operator equal (0x004D6FF0; VectorClass<TubeClass *> vtable 0x007a2204 slot 1).
// VectorClass<TubeClass *>::Resize (0x004D7030; VectorClass<TubeClass *> vtable 0x007a2204 slot 2).
// VectorClass<TubeClass *>::Clear (0x004D70E0; VectorClass<TubeClass *> vtable 0x007a2204 slot 3).
// VectorClass<TubeClass *>::ID_object (0x004D7110; VectorClass<TubeClass *> vtable 0x007a2204 slot 4).
// DynamicVectorClass<TubeClass *>::Resize (0x004D7170; DynamicVectorClass<TubeClass *> vtable 0x007a21e4 slot 2).
// DynamicVectorClass<TubeClass *>::ID_object (0x004D7220; DynamicVectorClass<TubeClass *> vtable 0x007a21e4 slot 4).
// VectorClass<ParticleSystemClass *>::operator equal (0x004D7250; VectorClass<ParticleSystemClass *> vtable 0x007a21c4 slot 1).
// VectorClass<ParticleSystemClass *>::Resize (0x004D7290; VectorClass<ParticleSystemClass *> vtable 0x007a21c4 slot 2).
// VectorClass<ParticleSystemClass *>::Clear (0x004D7340; VectorClass<ParticleSystemClass *> vtable 0x007a21c4 slot 3).
// VectorClass<ParticleSystemClass *>::ID_object (0x004D7370; VectorClass<ParticleSystemClass *> vtable 0x007a21c4 slot 4).
// DynamicVectorClass<ParticleSystemClass *>::Resize (0x004D73D0; DynamicVectorClass<ParticleSystemClass *> vtable 0x007a21a4 slot 2).
// DynamicVectorClass<ParticleSystemClass *>::ID_object (0x004D7480; DynamicVectorClass<ParticleSystemClass *> vtable 0x007a21a4 slot 4).
// VectorClass<WaveClass *>::operator equal (0x004D74B0; VectorClass<WaveClass *> vtable 0x007a2244 slot 1).
// VectorClass<WaveClass *>::Resize (0x004D74F0; VectorClass<WaveClass *> vtable 0x007a2244 slot 2).
// VectorClass<WaveClass *>::Clear (0x004D75A0; VectorClass<WaveClass *> vtable 0x007a2244 slot 3).
// VectorClass<WaveClass *>::ID_object (0x004D75D0; VectorClass<WaveClass *> vtable 0x007a2244 slot 4).
// DynamicVectorClass<WaveClass *>::Resize (0x004D7630; DynamicVectorClass<WaveClass *> vtable 0x007a2224 slot 2).
// DynamicVectorClass<WaveClass *>::ID_object (0x004D76E0; DynamicVectorClass<WaveClass *> vtable 0x007a2224 slot 4).
// VectorClass<OverlayClass *>::operator equal (0x004D7710; VectorClass<OverlayClass *> vtable 0x007a2284 slot 1).
// VectorClass<OverlayClass *>::Resize (0x004D7750; VectorClass<OverlayClass *> vtable 0x007a2284 slot 2).
// VectorClass<OverlayClass *>::Clear (0x004D7800; VectorClass<OverlayClass *> vtable 0x007a2284 slot 3).
// VectorClass<OverlayClass *>::ID_object (0x004D7830; VectorClass<OverlayClass *> vtable 0x007a2284 slot 4).
// DynamicVectorClass<OverlayClass *>::Resize (0x004D7890; DynamicVectorClass<OverlayClass *> vtable 0x007a2264 slot 2).
// DynamicVectorClass<OverlayClass *>::ID_object (0x004D7940; DynamicVectorClass<OverlayClass *> vtable 0x007a2264 slot 4).
// VectorClass<SmudgeClass *>::operator equal (0x004D7970; VectorClass<SmudgeClass *> vtable 0x007a2304 slot 1).
// VectorClass<SmudgeClass *>::Resize (0x004D79B0; VectorClass<SmudgeClass *> vtable 0x007a2304 slot 2).
// VectorClass<SmudgeClass *>::Clear (0x004D7A60; VectorClass<SmudgeClass *> vtable 0x007a2304 slot 3).
// VectorClass<SmudgeClass *>::ID_object (0x004D7A90; VectorClass<SmudgeClass *> vtable 0x007a2304 slot 4).
// DynamicVectorClass<SmudgeClass *>::Resize (0x004D7AF0; DynamicVectorClass<SmudgeClass *> vtable 0x007a22e4 slot 2).
// DynamicVectorClass<SmudgeClass *>::ID_object (0x004D7BA0; DynamicVectorClass<SmudgeClass *> vtable 0x007a22e4 slot 4).
// VectorClass<TerrainClass *>::operator equal (0x004D7BD0; VectorClass<TerrainClass *> vtable 0x007a2344 slot 1).
// VectorClass<TerrainClass *>::Resize (0x004D7C10; VectorClass<TerrainClass *> vtable 0x007a2344 slot 2).
// VectorClass<TerrainClass *>::Clear (0x004D7CC0; VectorClass<TerrainClass *> vtable 0x007a2344 slot 3).
// VectorClass<TerrainClass *>::ID_object (0x004D7CF0; VectorClass<TerrainClass *> vtable 0x007a2344 slot 4).
// DynamicVectorClass<TerrainClass *>::Resize (0x004D7D50; DynamicVectorClass<TerrainClass *> vtable 0x007a2324 slot 2).
// DynamicVectorClass<TerrainClass *>::ID_object (0x004D7E00; DynamicVectorClass<TerrainClass *> vtable 0x007a2324 slot 4).
// VectorClass<AircraftClass *>::operator equal (0x004D7E30; VectorClass<AircraftClass *> vtable 0x007a23c4 slot 1).
// VectorClass<AircraftClass *>::Resize (0x004D7E70; VectorClass<AircraftClass *> vtable 0x007a23c4 slot 2).
// VectorClass<AircraftClass *>::Clear (0x004D7F20; VectorClass<AircraftClass *> vtable 0x007a23c4 slot 3).
// VectorClass<AircraftClass *>::ID_object (0x004D7F50; VectorClass<AircraftClass *> vtable 0x007a23c4 slot 4).
// DynamicVectorClass<AircraftClass *>::Resize (0x004D7FB0; DynamicVectorClass<AircraftClass *> vtable 0x007a23a4 slot 2).
// DynamicVectorClass<AircraftClass *>::ID_object (0x004D8060; DynamicVectorClass<AircraftClass *> vtable 0x007a23a4 slot 4).
// VectorClass<UnitClass *>::operator equal (0x004D8090; VectorClass<UnitClass *> vtable 0x007a2404 slot 1).
// VectorClass<UnitClass *>::Resize (0x004D80D0; VectorClass<UnitClass *> vtable 0x007a2404 slot 2).
// VectorClass<UnitClass *>::Clear (0x004D8180; VectorClass<UnitClass *> vtable 0x007a2404 slot 3).
// VectorClass<UnitClass *>::ID_object (0x004D81B0; VectorClass<UnitClass *> vtable 0x007a2404 slot 4).
// DynamicVectorClass<UnitClass *>::Resize (0x004D8210; DynamicVectorClass<UnitClass *> vtable 0x007a23e4 slot 2).
// DynamicVectorClass<UnitClass *>::ID_object (0x004D82C0; DynamicVectorClass<UnitClass *> vtable 0x007a23e4 slot 4).
// VectorClass<HouseClass *>::operator equal (0x004D82F0; VectorClass<HouseClass *> vtable 0x007a2444 slot 1).
// VectorClass<HouseClass *>::Resize (0x004D8330; VectorClass<HouseClass *> vtable 0x007a2444 slot 2).
// VectorClass<HouseClass *>::Clear (0x004D83E0; VectorClass<HouseClass *> vtable 0x007a2444 slot 3).
// VectorClass<HouseClass *>::ID_object (0x004D8410; VectorClass<HouseClass *> vtable 0x007a2444 slot 4).
// DynamicVectorClass<HouseClass *>::Resize (0x004D8470; DynamicVectorClass<HouseClass *> vtable 0x007a2424 slot 2).
// DynamicVectorClass<HouseClass *>::ID_object (0x004D8520; DynamicVectorClass<HouseClass *> vtable 0x007a2424 slot 4).
// VectorClass<AnimClass *>::operator equal (0x004D8550; VectorClass<AnimClass *> vtable 0x007a2484 slot 1).
// VectorClass<AnimClass *>::Resize (0x004D8590; VectorClass<AnimClass *> vtable 0x007a2484 slot 2).
// VectorClass<AnimClass *>::Clear (0x004D8640; VectorClass<AnimClass *> vtable 0x007a2484 slot 3).
// VectorClass<AnimClass *>::ID_object (0x004D8670; VectorClass<AnimClass *> vtable 0x007a2484 slot 4).
// DynamicVectorClass<AnimClass *>::Resize (0x004D86D0; DynamicVectorClass<AnimClass *> vtable 0x007a2464 slot 2).
// DynamicVectorClass<AnimClass *>::ID_object (0x004D8780; DynamicVectorClass<AnimClass *> vtable 0x007a2464 slot 4).
// VectorClass<FactoryClass *>::operator equal (0x004D87B0; VectorClass<FactoryClass *> vtable 0x007a2504 slot 1).
// VectorClass<FactoryClass *>::Resize (0x004D87F0; VectorClass<FactoryClass *> vtable 0x007a2504 slot 2).
// VectorClass<FactoryClass *>::Clear (0x004D88A0; VectorClass<FactoryClass *> vtable 0x007a2504 slot 3).
// VectorClass<FactoryClass *>::ID_object (0x004D88D0; VectorClass<FactoryClass *> vtable 0x007a2504 slot 4).
// DynamicVectorClass<FactoryClass *>::Resize (0x004D8930; DynamicVectorClass<FactoryClass *> vtable 0x007a24e4 slot 2).
// DynamicVectorClass<FactoryClass *>::ID_object (0x004D89E0; DynamicVectorClass<FactoryClass *> vtable 0x007a24e4 slot 4).
// VectorClass<CampaignClass *>::operator equal (0x004D8A10; VectorClass<CampaignClass *> vtable 0x007a2544 slot 1).
// VectorClass<CampaignClass *>::Resize (0x004D8A50; VectorClass<CampaignClass *> vtable 0x007a2544 slot 2).
// VectorClass<CampaignClass *>::Clear (0x004D8B00; VectorClass<CampaignClass *> vtable 0x007a2544 slot 3).
// VectorClass<CampaignClass *>::ID_object (0x004D8B30; VectorClass<CampaignClass *> vtable 0x007a2544 slot 4).
// DynamicVectorClass<CampaignClass *>::Resize (0x004D8B90; DynamicVectorClass<CampaignClass *> vtable 0x007a2524 slot 2).
// DynamicVectorClass<CampaignClass *>::ID_object (0x004D8C40; DynamicVectorClass<CampaignClass *> vtable 0x007a2524 slot 4).
// VectorClass<SideClass *>::operator equal (0x004D8C70; VectorClass<SideClass *> vtable 0x007a2584 slot 1).
// VectorClass<SideClass *>::Resize (0x004D8CB0; VectorClass<SideClass *> vtable 0x007a2584 slot 2).
// VectorClass<SideClass *>::Clear (0x004D8D60; VectorClass<SideClass *> vtable 0x007a2584 slot 3).
// DynamicVectorClass<SideClass *>::Resize (0x004D8DF0; DynamicVectorClass<SideClass *> vtable 0x007a2564 slot 2).
// DynamicVectorClass<SideClass *>::ID_object (0x004D8EA0; DynamicVectorClass<SideClass *> vtable 0x007a2564 slot 4).
// VectorClass<HouseTypeClass *>::operator equal (0x004D8ED0; VectorClass<HouseTypeClass *> vtable 0x007a25c4 slot 1).
// VectorClass<HouseTypeClass *>::Resize (0x004D8F10; VectorClass<HouseTypeClass *> vtable 0x007a25c4 slot 2).
// VectorClass<HouseTypeClass *>::Clear (0x004D8FC0; VectorClass<HouseTypeClass *> vtable 0x007a25c4 slot 3).
// VectorClass<HouseTypeClass *>::ID_object (0x004D8FF0; VectorClass<HouseTypeClass *> vtable 0x007a25c4 slot 4).
// DynamicVectorClass<HouseTypeClass *>::Resize (0x004D9050; DynamicVectorClass<HouseTypeClass *> vtable 0x007a25a4 slot 2).
// DynamicVectorClass<HouseTypeClass *>::ID_object (0x004D9100; DynamicVectorClass<HouseTypeClass *> vtable 0x007a25a4 slot 4).
// VectorClass<TaskForceClass *>::operator equal (0x004D9130; VectorClass<TaskForceClass *> vtable 0x007a2604 slot 1).
// VectorClass<TaskForceClass *>::Resize (0x004D9170; VectorClass<TaskForceClass *> vtable 0x007a2604 slot 2).
// VectorClass<TaskForceClass *>::Clear (0x004D9220; VectorClass<TaskForceClass *> vtable 0x007a2604 slot 3).
// VectorClass<TaskForceClass *>::ID_object (0x004D9250; VectorClass<TaskForceClass *> vtable 0x007a2604 slot 4).
// DynamicVectorClass<TaskForceClass *>::Resize (0x004D92B0; DynamicVectorClass<TaskForceClass *> vtable 0x007a25e4 slot 2).
// DynamicVectorClass<TaskForceClass *>::ID_object (0x004D9360; DynamicVectorClass<TaskForceClass *> vtable 0x007a25e4 slot 4).
// VectorClass<TeamTypeClass *>::operator equal (0x004D9390; VectorClass<TeamTypeClass *> vtable 0x007a2644 slot 1).
// VectorClass<TeamTypeClass *>::Resize (0x004D93D0; VectorClass<TeamTypeClass *> vtable 0x007a2644 slot 2).
// VectorClass<TeamTypeClass *>::Clear (0x004D9480; VectorClass<TeamTypeClass *> vtable 0x007a2644 slot 3).
// VectorClass<TeamTypeClass *>::ID_object (0x004D94B0; VectorClass<TeamTypeClass *> vtable 0x007a2644 slot 4).
// DynamicVectorClass<TeamTypeClass *>::Resize (0x004D9510; DynamicVectorClass<TeamTypeClass *> vtable 0x007a2624 slot 2).
// DynamicVectorClass<TeamTypeClass *>::ID_object (0x004D95C0; DynamicVectorClass<TeamTypeClass *> vtable 0x007a2624 slot 4).
// VectorClass<ScriptTypeClass *>::operator equal (0x004D95F0; VectorClass<ScriptTypeClass *> vtable 0x007a2684 slot 1).
// VectorClass<ScriptTypeClass *>::Resize (0x004D9630; VectorClass<ScriptTypeClass *> vtable 0x007a2684 slot 2).
// VectorClass<ScriptTypeClass *>::Clear (0x004D96E0; VectorClass<ScriptTypeClass *> vtable 0x007a2684 slot 3).
// VectorClass<ScriptTypeClass *>::ID_object (0x004D9710; VectorClass<ScriptTypeClass *> vtable 0x007a2684 slot 4).
// DynamicVectorClass<ScriptTypeClass *>::Resize (0x004D9770; DynamicVectorClass<ScriptTypeClass *> vtable 0x007a2664 slot 2).
// DynamicVectorClass<ScriptTypeClass *>::ID_object (0x004D9820; DynamicVectorClass<ScriptTypeClass *> vtable 0x007a2664 slot 4).
// VectorClass<OverlayTypeClass *>::operator equal (0x004D9850; VectorClass<OverlayTypeClass *> vtable 0x007a26c4 slot 1).
// VectorClass<OverlayTypeClass *>::Resize (0x004D9890; VectorClass<OverlayTypeClass *> vtable 0x007a26c4 slot 2).
// VectorClass<OverlayTypeClass *>::Clear (0x004D9940; VectorClass<OverlayTypeClass *> vtable 0x007a26c4 slot 3).
// VectorClass<OverlayTypeClass *>::ID_object (0x004D9970; VectorClass<OverlayTypeClass *> vtable 0x007a26c4 slot 4).
// DynamicVectorClass<OverlayTypeClass *>::Resize (0x004D99D0; DynamicVectorClass<OverlayTypeClass *> vtable 0x007a26a4 slot 2).
// DynamicVectorClass<OverlayTypeClass *>::ID_object (0x004D9A80; DynamicVectorClass<OverlayTypeClass *> vtable 0x007a26a4 slot 4).
// VectorClass<SmudgeTypeClass *>::operator equal (0x004D9AB0; VectorClass<SmudgeTypeClass *> vtable 0x007a2704 slot 1).
// VectorClass<SmudgeTypeClass *>::Resize (0x004D9AF0; VectorClass<SmudgeTypeClass *> vtable 0x007a2704 slot 2).
// VectorClass<SmudgeTypeClass *>::Clear (0x004D9BA0; VectorClass<SmudgeTypeClass *> vtable 0x007a2704 slot 3).
// VectorClass<SmudgeTypeClass *>::ID_object (0x004D9BD0; VectorClass<SmudgeTypeClass *> vtable 0x007a2704 slot 4).
// DynamicVectorClass<SmudgeTypeClass *>::Resize (0x004D9C30; DynamicVectorClass<SmudgeTypeClass *> vtable 0x007a26e4 slot 2).
// DynamicVectorClass<SmudgeTypeClass *>::ID_object (0x004D9CE0; DynamicVectorClass<SmudgeTypeClass *> vtable 0x007a26e4 slot 4).
// VectorClass<TerrainTypeClass *>::operator equal (0x004D9D10; VectorClass<TerrainTypeClass *> vtable 0x007a2744 slot 1).
// VectorClass<TerrainTypeClass *>::Resize (0x004D9D50; VectorClass<TerrainTypeClass *> vtable 0x007a2744 slot 2).
// VectorClass<TerrainTypeClass *>::Clear (0x004D9E00; VectorClass<TerrainTypeClass *> vtable 0x007a2744 slot 3).
// VectorClass<TerrainTypeClass *>::ID_object (0x004D9E30; VectorClass<TerrainTypeClass *> vtable 0x007a2744 slot 4).
// DynamicVectorClass<TerrainTypeClass *>::Resize (0x004D9E90; DynamicVectorClass<TerrainTypeClass *> vtable 0x007a2724 slot 2).
// DynamicVectorClass<TerrainTypeClass *>::ID_object (0x004D9F40; DynamicVectorClass<TerrainTypeClass *> vtable 0x007a2724 slot 4).
// VectorClass<BuildingTypeClass *>::operator equal (0x004D9F70; VectorClass<BuildingTypeClass *> vtable 0x007a2784 slot 1).
// VectorClass<BuildingTypeClass *>::Resize (0x004D9FB0; VectorClass<BuildingTypeClass *> vtable 0x007a2784 slot 2).
// VectorClass<BuildingTypeClass *>::Clear (0x004DA060; VectorClass<BuildingTypeClass *> vtable 0x007a2784 slot 3).
// VectorClass<BuildingTypeClass *>::ID_object (0x004DA090; VectorClass<BuildingTypeClass *> vtable 0x007a2784 slot 4).
// DynamicVectorClass<BuildingTypeClass *>::Resize (0x004DA0F0; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2764 slot 2).
// DynamicVectorClass<BuildingTypeClass *>::ID_object (0x004DA1A0; DynamicVectorClass<BuildingTypeClass *> vtable 0x007a2764 slot 4).
// VectorClass<AircraftTypeClass *>::operator equal (0x004DA1D0; VectorClass<AircraftTypeClass *> vtable 0x007a27c4 slot 1).
// VectorClass<AircraftTypeClass *>::Resize (0x004DA210; VectorClass<AircraftTypeClass *> vtable 0x007a27c4 slot 2).
// VectorClass<AircraftTypeClass *>::Clear (0x004DA2C0; VectorClass<AircraftTypeClass *> vtable 0x007a27c4 slot 3).
// VectorClass<AircraftTypeClass *>::ID_object (0x004DA2F0; VectorClass<AircraftTypeClass *> vtable 0x007a27c4 slot 4).
// DynamicVectorClass<AircraftTypeClass *>::Resize (0x004DA350; DynamicVectorClass<AircraftTypeClass *> vtable 0x007a27a4 slot 2).
// DynamicVectorClass<AircraftTypeClass *>::ID_object (0x004DA400; DynamicVectorClass<AircraftTypeClass *> vtable 0x007a27a4 slot 4).
// VectorClass<UnitTypeClass *>::operator equal (0x004DA430; VectorClass<UnitTypeClass *> vtable 0x007a2804 slot 1).
// VectorClass<UnitTypeClass *>::Resize (0x004DA470; VectorClass<UnitTypeClass *> vtable 0x007a2804 slot 2).
// VectorClass<UnitTypeClass *>::Clear (0x004DA520; VectorClass<UnitTypeClass *> vtable 0x007a2804 slot 3).
// VectorClass<UnitTypeClass *>::ID_object (0x004DA550; VectorClass<UnitTypeClass *> vtable 0x007a2804 slot 4).
// DynamicVectorClass<UnitTypeClass *>::Resize (0x004DA5B0; DynamicVectorClass<UnitTypeClass *> vtable 0x007a27e4 slot 2).
// DynamicVectorClass<UnitTypeClass *>::ID_object (0x004DA660; DynamicVectorClass<UnitTypeClass *> vtable 0x007a27e4 slot 4).
// VectorClass<AnimTypeClass *>::operator equal (0x004DA690; VectorClass<AnimTypeClass *> vtable 0x007a2844 slot 1).
// VectorClass<AnimTypeClass *>::Resize (0x004DA6D0; VectorClass<AnimTypeClass *> vtable 0x007a2844 slot 2).
// VectorClass<AnimTypeClass *>::Clear (0x004DA780; VectorClass<AnimTypeClass *> vtable 0x007a2844 slot 3).
// VectorClass<AnimTypeClass *>::ID_object (0x004DA7B0; VectorClass<AnimTypeClass *> vtable 0x007a2844 slot 4).
// DynamicVectorClass<AnimTypeClass *>::Resize (0x004DA810; DynamicVectorClass<AnimTypeClass *> vtable 0x007a2824 slot 2).
// DynamicVectorClass<AnimTypeClass *>::ID_object (0x004DA8C0; DynamicVectorClass<AnimTypeClass *> vtable 0x007a2824 slot 4).
// VectorClass<InfantryTypeClass *>::operator equal (0x004DA8F0; VectorClass<InfantryTypeClass *> vtable 0x007a2884 slot 1).
// VectorClass<InfantryTypeClass *>::Resize (0x004DA930; VectorClass<InfantryTypeClass *> vtable 0x007a2884 slot 2).
// VectorClass<InfantryTypeClass *>::Clear (0x004DA9E0; VectorClass<InfantryTypeClass *> vtable 0x007a2884 slot 3).
// VectorClass<InfantryTypeClass *>::ID_object (0x004DAA10; VectorClass<InfantryTypeClass *> vtable 0x007a2884 slot 4).
// DynamicVectorClass<InfantryTypeClass *>::Resize (0x004DAA70; DynamicVectorClass<InfantryTypeClass *> vtable 0x007a2864 slot 2).
// DynamicVectorClass<InfantryTypeClass *>::ID_object (0x004DAB20; DynamicVectorClass<InfantryTypeClass *> vtable 0x007a2864 slot 4).
// VectorClass<BulletTypeClass *>::operator equal (0x004DAB50; VectorClass<BulletTypeClass *> vtable 0x007a28c4 slot 1).
// VectorClass<BulletTypeClass *>::Resize (0x004DAB90; VectorClass<BulletTypeClass *> vtable 0x007a28c4 slot 2).
// VectorClass<BulletTypeClass *>::Clear (0x004DAC40; VectorClass<BulletTypeClass *> vtable 0x007a28c4 slot 3).
// VectorClass<BulletTypeClass *>::ID_object (0x004DAC70; VectorClass<BulletTypeClass *> vtable 0x007a28c4 slot 4).
// DynamicVectorClass<BulletTypeClass *>::Resize (0x004DACD0; DynamicVectorClass<BulletTypeClass *> vtable 0x007a28a4 slot 2).
// DynamicVectorClass<BulletTypeClass *>::ID_object (0x004DAD80; DynamicVectorClass<BulletTypeClass *> vtable 0x007a28a4 slot 4).
// VectorClass<VoxelAnimTypeClass *>::operator equal (0x004DADB0; VectorClass<VoxelAnimTypeClass *> vtable 0x007a2904 slot 1).
// VectorClass<VoxelAnimTypeClass *>::Resize (0x004DADF0; VectorClass<VoxelAnimTypeClass *> vtable 0x007a2904 slot 2).
// VectorClass<VoxelAnimTypeClass *>::Clear (0x004DAEA0; VectorClass<VoxelAnimTypeClass *> vtable 0x007a2904 slot 3).
// VectorClass<VoxelAnimTypeClass *>::ID_object (0x004DAED0; VectorClass<VoxelAnimTypeClass *> vtable 0x007a2904 slot 4).
// DynamicVectorClass<VoxelAnimTypeClass *>::Resize (0x004DAF30; DynamicVectorClass<VoxelAnimTypeClass *> vtable 0x007a28e4 slot 2).
// DynamicVectorClass<VoxelAnimTypeClass *>::ID_object (0x004DAFE0; DynamicVectorClass<VoxelAnimTypeClass *> vtable 0x007a28e4 slot 4).
// VectorClass<IsometricTileTypeClass *>::operator equal (0x004DB010; VectorClass<IsometricTileTypeClass *> vtable 0x007a2944 slot 1).
// VectorClass<IsometricTileTypeClass *>::Resize (0x004DB050; VectorClass<IsometricTileTypeClass *> vtable 0x007a2944 slot 2).
// VectorClass<IsometricTileTypeClass *>::Clear (0x004DB100; VectorClass<IsometricTileTypeClass *> vtable 0x007a2944 slot 3).
// VectorClass<IsometricTileTypeClass *>::ID_object (0x004DB130; VectorClass<IsometricTileTypeClass *> vtable 0x007a2944 slot 4).
// DynamicVectorClass<IsometricTileTypeClass *>::Resize (0x004DB190; DynamicVectorClass<IsometricTileTypeClass *> vtable 0x007a2924 slot 2).
// DynamicVectorClass<IsometricTileTypeClass *>::ID_object (0x004DB240; DynamicVectorClass<IsometricTileTypeClass *> vtable 0x007a2924 slot 4).
// VectorClass<ParticleTypeClass *>::operator equal (0x004DB270; VectorClass<ParticleTypeClass *> vtable 0x007a2984 slot 1).
// VectorClass<ParticleTypeClass *>::Resize (0x004DB2B0; VectorClass<ParticleTypeClass *> vtable 0x007a2984 slot 2).
// VectorClass<ParticleTypeClass *>::Clear (0x004DB360; VectorClass<ParticleTypeClass *> vtable 0x007a2984 slot 3).
// VectorClass<ParticleTypeClass *>::ID_object (0x004DB390; VectorClass<ParticleTypeClass *> vtable 0x007a2984 slot 4).
// DynamicVectorClass<ParticleTypeClass *>::Resize (0x004DB3F0; DynamicVectorClass<ParticleTypeClass *> vtable 0x007a2964 slot 2).
// DynamicVectorClass<ParticleTypeClass *>::ID_object (0x004DB4A0; DynamicVectorClass<ParticleTypeClass *> vtable 0x007a2964 slot 4).
// VectorClass<ParticleSystemTypeClass *>::operator equal (0x004DB4D0; VectorClass<ParticleSystemTypeClass *> vtable 0x007a29c4 slot 1).
// VectorClass<ParticleSystemTypeClass *>::Resize (0x004DB510; VectorClass<ParticleSystemTypeClass *> vtable 0x007a29c4 slot 2).
// VectorClass<ParticleSystemTypeClass *>::Clear (0x004DB5C0; VectorClass<ParticleSystemTypeClass *> vtable 0x007a29c4 slot 3).
// VectorClass<ParticleSystemTypeClass *>::ID_object (0x004DB5F0; VectorClass<ParticleSystemTypeClass *> vtable 0x007a29c4 slot 4).
// DynamicVectorClass<ParticleSystemTypeClass *>::Resize (0x004DB650; DynamicVectorClass<ParticleSystemTypeClass *> vtable 0x007a29a4 slot 2).
// DynamicVectorClass<ParticleSystemTypeClass *>::ID_object (0x004DB700; DynamicVectorClass<ParticleSystemTypeClass *> vtable 0x007a29a4 slot 4).
// VectorClass<SuperWeaponTypeClass *>::operator equal (0x004DB730; VectorClass<SuperWeaponTypeClass *> vtable 0x007a2a04 slot 1).
// VectorClass<SuperWeaponTypeClass *>::Resize (0x004DB770; VectorClass<SuperWeaponTypeClass *> vtable 0x007a2a04 slot 2).
// VectorClass<SuperWeaponTypeClass *>::Clear (0x004DB820; VectorClass<SuperWeaponTypeClass *> vtable 0x007a2a04 slot 3).
// VectorClass<SuperWeaponTypeClass *>::ID_object (0x004DB850; VectorClass<SuperWeaponTypeClass *> vtable 0x007a2a04 slot 4).
// DynamicVectorClass<SuperWeaponTypeClass *>::Resize (0x004DB8B0; DynamicVectorClass<SuperWeaponTypeClass *> vtable 0x007a29e4 slot 2).
// DynamicVectorClass<SuperWeaponTypeClass *>::ID_object (0x004DB960; DynamicVectorClass<SuperWeaponTypeClass *> vtable 0x007a29e4 slot 4).
// VectorClass<AbstractTypeClass *>::operator equal (0x004DB990; VectorClass<AbstractTypeClass *> vtable 0x007a2a84 slot 1).
// VectorClass<AbstractTypeClass *>::Resize (0x004DB9D0; VectorClass<AbstractTypeClass *> vtable 0x007a2a84 slot 2).
// VectorClass<AbstractTypeClass *>::Clear (0x004DBA80; VectorClass<AbstractTypeClass *> vtable 0x007a2a84 slot 3).
// VectorClass<AbstractTypeClass *>::ID_object (0x004DBAB0; VectorClass<AbstractTypeClass *> vtable 0x007a2a84 slot 4).
// DynamicVectorClass<AbstractTypeClass *>::Resize (0x004DBB10; DynamicVectorClass<AbstractTypeClass *> vtable 0x007a2a64 slot 2).
// DynamicVectorClass<AbstractTypeClass *>::ID_object (0x004DBBC0; DynamicVectorClass<AbstractTypeClass *> vtable 0x007a2a64 slot 4).
