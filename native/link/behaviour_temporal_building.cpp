// Behaviour-written rows for the temporal weapon on a building (Scout1242, 64/32 audit).
// NOT EXACT; never original code or bytes.
//
// TemporalClass::Unknown_006E27B0 0x006E27B0 (a building is warped) and Let_Go_Of_Target 0x006E2500 /
// Unknown_006E2610 0x006E2610 (released) call two small BuildingClass members, 0x0044D320 and
// 0x0044D380, whose only sources are seeds that take the building as an int and write its fields at
// their VC6 offsets (+0x530, the animation slots at +0x454, the type's Anims[i].Powered at
// +0xCD4 + i * 0x44 + 0x40, the animations' +0x101..+0x104 and the stage at +0x98). Natively the
// int cut the 64-bit address: the first warp of a building by a Chrono Legionnaire faulted. These
// rows do the same with the real members (port/fixups.json routes the three calls here).
#define RA2_VEIN_STAGE_ACCESSORS
#include "shared/building_class.hpp"
#include "shared/anim.hpp"

// 0x0044D320: the building stops (unknown_530 clear) and its powered animations pause, each keeping
// its current stage in +0x104.
void Native_Building_Temporal_Freeze_0044D320(TechnoClass *techno)
{
    BuildingClass *building = static_cast<BuildingClass *>(techno);
    building->unknown_530 = false;
    for (int i = 0; i < 18; i++) {
        AnimClass *anim = static_cast<AnimClass *>(building->unknown_454[i]);
        if (building->Type->Anims[i].Powered && anim != 0) {
            anim->unknown_102 = true;
            anim->unknown_103 = false;
            anim->unknown_104 = anim->Fetch_Stage();
            anim->unknown_101 = true;
        }
    }
}

// 0x0044D380: the reverse (unknown_530 set, the powered animations run again).
void Native_Building_Temporal_Thaw_0044D380(TechnoClass *techno)
{
    BuildingClass *building = static_cast<BuildingClass *>(techno);
    building->unknown_530 = true;
    for (int i = 0; i < 18; i++) {
        AnimClass *anim = static_cast<AnimClass *>(building->unknown_454[i]);
        if (building->Type->Anims[i].Powered && anim != 0) {
            anim->unknown_102 = false;
            anim->unknown_103 = true;
            anim->unknown_101 = false;
        }
    }
}
