// logic.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/logic.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/logic.hpp"
#include "shared/main_loop.hpp"
#include "shared/queue.hpp"
#include "shared/rules.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/map_view.hpp"
#include "shared/foot.hpp"
#include "shared/scenario_tags.hpp"
#include "shared/tag.hpp"
#include "shared/anim.hpp"
#include "shared/building_light.hpp"
#include "shared/bullet.hpp"
#include "shared/infantry.hpp"
#include "shared/particle_system.hpp"
#include "shared/terrain.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/wave.hpp"
#include "shared/aircraft.hpp"
#include "shared/saveload_lists.hpp"
#include "shared/kamikaze_tracker.hpp"
#include "shared/tiberium.hpp"
#include "shared/spot_light.hpp"
#include "shared/laser_draw.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/radsite.hpp"
#include "shared/light_source.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/vector_class.hpp"
#include "shared/abstract.hpp"
#include "shared/empulse_class.hpp"
#include "shared/alpha_shape.hpp"
#include "shared/tactical.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/house.hpp"
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#include "shared/object.hpp"


// Header-static dynamic initializers of the unit at 0x0053D630: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0053D630-0x0053D811); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0053D650, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D680, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D6A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D6C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D6E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D700, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D720, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D760, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D790, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D7C0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D7F0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


// Dynamic initializer (0x0053D800, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0053d630.cpp; no source of its own.


extern unsigned int unknown_detail_frame_rate_7dfb34;   // 0x007DFB34 (set by 0x0053D820)
extern unsigned int unknown_detail_buffer_zone_7dfb38;  // 0x007DFB38 (set by 0x0053D830)
extern bool unknown_detail_reduced_a6eb28;              // 0x00A6EB28
bool Unknown_0051FC70();                                // 0x0051FC70 (dominator state 2; hypothesis)
void Unknown_00521A70();                                // 0x00521A70

unsigned int GetMinFrameRate()
{
    if (unknown_detail_reduced_a6eb28) {
        if (FPSCounter_CurrentFrameRate >= unknown_detail_frame_rate_7dfb34 + unknown_detail_buffer_zone_7dfb38) {
            unknown_detail_reduced_a6eb28 = false;
            return unknown_detail_frame_rate_7dfb34;
        }
        return unknown_detail_frame_rate_7dfb34 + unknown_detail_buffer_zone_7dfb38;
    }
    if (FPSCounter_CurrentFrameRate < unknown_detail_frame_rate_7dfb34) {
        unknown_detail_reduced_a6eb28 = true;
        return unknown_detail_frame_rate_7dfb34 + unknown_detail_buffer_zone_7dfb38;
    }
    return unknown_detail_frame_rate_7dfb34;
}


void LogicClass::AI()
{
    int index;

    FramesThisSecond++;

    for (LogicTriggerID = 0; LogicTriggerID < LogicTags.Count(); LogicTriggerID++) {
        TagClass *trig = LogicTags[LogicTriggerID];

        if (Scen->IsCrateBeenPickedUp) {
            if (trig->Spring(0x32, 0, unknown_geometry_k)) continue;
        }
        if (Scen->IsGlobalChanged) {
            if (trig->Spring(0x1B, 0, unknown_geometry_k)) continue;
            if (trig->Spring(0x1C, 0, unknown_geometry_k)) continue;
            if (trig->Spring(0x24, 0, unknown_geometry_k)) continue;
            if (trig->Spring(0x25, 0, unknown_geometry_k)) continue;
        }
        if (Scen->IsAmbientLightChanged) {
            if (trig->Spring(0x2D, 0, unknown_geometry_k)) continue;
            if (trig->Spring(0x2E, 0, unknown_geometry_k)) continue;
        }
        if (trig->Spring(0x0D, 0, unknown_geometry_k)) continue;
        if (trig->Spring(0x33, 0, unknown_geometry_k)) continue;

        if (Scen->MissionTimer.Is_Active() && Scen->MissionTimer == 0) {
            if (trig->Spring(0x0E, 0, unknown_geometry_k)) continue;
        }
    }

    if (Scen->MissionTimer.Is_Active() && Scen->MissionTimer == 0) {
        Scen->MissionTimer.Stop();
        Map.Flag_To_Redraw(2);
    }
    Scen->IsGlobalChanged = false;
    Scen->IsBridgeChanged = false;
    Scen->IsAmbientLightChanged = false;
    Scen->IsCrateBeenPickedUp = false;

    if (Rules->ShroudGrow && Rules->ShroudRate != 0 && Scen->ShroudTimer == 0) {
        Scen->ShroudTimer = 900 * Rules->ShroudRate;
        Map.Encroach_Shadow();
    }
    if (Frame % 120 == 0) {
        Map.Refresh_Shroud();
    }
    if (((SpecialClass *)Scen)->FogOfWar && Rules->FogRate != 0 && Scen->FogTimer == 0) {
        Scen->FogTimer = 900 * Rules->FogRate;
        Map.Encroach_Fog();
    }

    if (Scen->DesiredAmbientLight != Scen->CurrentAmbientLight && Rules->AmbientChangeRate != 0 && Scen->AmbientChangeTimer == 0) {
        if (NukeFlash_IsFadingIn() || Unknown_0051FC70()) {
            Scen->AmbientChangeTimer = Scen->NukeAmbientChangeRate;
        } else {
            Scen->AmbientChangeTimer = 900 * Rules->AmbientChangeRate;
        }
        Scen->DesiredAmbientLight = MAX(Scen->DesiredAmbientLight, 0);
        int value = 100.0 * Rules->AmbientChangeStep;
        if (Scen->DesiredAmbientLight < Scen->CurrentAmbientLight) {
            Scen->CurrentAmbientLight -= value;
            Scen->CurrentAmbientLight = MAX(Scen->CurrentAmbientLight, Scen->DesiredAmbientLight);
        } else {
            Scen->CurrentAmbientLight += value;
            Scen->CurrentAmbientLight = MIN(Scen->CurrentAmbientLight, Scen->DesiredAmbientLight);
        }
        Scen->IsAmbientLightChanged = true;
        Map.Unknown_0049E5E0();
        Map.Flag_To_Redraw(1);
    }

    TiberiumClass::Growth_AI();
    TiberiumClass::Spread_AI();
    BombList.Unknown_004361F0();
    KamikazeTracker.AI();

    DynamicVectorClass<TeamClass *> teams;
    for (index = 0; index < Teams.Count(); index++) {
        teams.Add(Teams[index]);
    }
    for (index = 0; index < teams.Count(); index++) {
        teams[index]->AI();
    }

    SpotLightClass::Update_All();
    LaserDrawClass::Update_All();
    Update_Storm_00520200();
    for (index = unknown_RadSiteClass_list_00ab6620.Count() - 1; index >= 0; index--) {
        unknown_RadSiteClass_list_00ab6620[index]->AI();
    }
    LightSourceClass::Unknown_00537830(6, false);
    EMPulseClass::Expire_004B4DF0();

    for (index = 0; index < Count(); index++) {
        ObjectClass *obj = (*this)[index];
        obj->AI();
    }

    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        for (index = 0; index < unknown_anims_a35e60.Count(); index++) {
            unknown_anims_a35e60[index]->AI();
        }
    }

    Unknown_00521A70();
    AlphaShapeClass::Update_All();

    Map.unknown_slot27();
    TacticalMap->AI();

    for (index = 0; index < Factories.Count(); index++) {
        Factories[index]->AI();
    }

    for (index = 0; index < Houses.Count(); index++) {
        HouseClass *hptr = Houses[index];
        if (hptr) {
            hptr->AI();
        }
    }

    if (Map.Unknown_0049EC40() != 0) {
        CoordStruct coord = Map.Unknown_0049EC40()->Coord;
        TacticalMap->Set_Tactical_Position_006a66b0(coord);
    }
}


void LogicClass::Environment_AI()
{
    if (Rules->ShroudGrow && Rules->ShroudRate != 0) {
        if (Scen->ShroudTimer == 0) {
            Scen->ShroudTimer = 900 * Rules->ShroudRate;
            Map.Encroach_Shadow();
        }
    }
    if (((SpecialClass *)Scen)->FogOfWar && Rules->FogRate != 0) {
        if (Scen->FogTimer == 0) {
            Scen->FogTimer = 900 * Rules->FogRate;
            Map.Encroach_Fog();
        }
    }
    for (int index = 0; index < Count(); index++) {
        FootClass *foot = (FootClass *)(*this)[index];
        bool is_foot;
        if (foot != 0) is_foot = foot->AbstractFlags.IsFoot;
        if (foot != 0 && is_foot && foot->Locomotor != 0 && !foot->unknown_325) {
            foot->Locomotor.Checked()->Process();
        }
    }
}


void LogicClass::Detach(AbstractClass const *target, bool all)
{
    if (target != 0 && target->Kind_Of() == 0x2C) {
        for (int index = 0; index < LogicTags.Count(); index++) {
            if (target == (AbstractClass *)LogicTags[index]) {
                LogicTags.Delete(index);
                index--;
            }
        }
    }
}


void AI_Submitted_Objects()
{
    int index;
    for (index = Anims.Count() - 1; index >= 0; index--) {
        AnimClass *anim = Anims[index];
        if (anim->IsSubmittedToLayer) anim->AI();
    }
    for (index = BuildingLights.Count() - 1; index >= 0; index--) {
        BuildingLightClass *blight = BuildingLights[index];
        if (blight->IsSubmittedToLayer) blight->AI();
    }
    for (index = unknown_BulletClass_list_00a40ce8.Count() - 1; index >= 0; index--) {
        BulletClass *bullet = unknown_BulletClass_list_00a40ce8[index];
        if (bullet->IsSubmittedToLayer) bullet->AI();
    }
    for (index = unknown_BuildingClass_list_00a40af8.Count() - 1; index >= 0; index--) {
        BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
        if (building->IsSubmittedToLayer) building->AI();
    }
    for (index = Aircraft.Count() - 1; index >= 0; index--) {
        AircraftClass *aircraft = Aircraft[index];
        if (aircraft->IsSubmittedToLayer) aircraft->AI();
    }
    for (index = Infantry.Count() - 1; index >= 0; index--) {
        InfantryClass *infantry = Infantry[index];
        if (infantry->IsSubmittedToLayer) infantry->AI();
    }
    for (index = Units.Count() - 1; index >= 0; index--) {
        UnitClass *unit = Units[index];
        if (unit->IsSubmittedToLayer) unit->AI();
    }
    for (index = ParticleSystems.Count() - 1; index >= 0; index--) {
        ParticleSystemClass *partsys = ParticleSystems[index];
        if (partsys->IsSubmittedToLayer) partsys->AI();
    }
    for (index = Terrains.Count() - 1; index >= 0; index--) {
        TerrainClass *terrain = Terrains[index];
        if (terrain->IsSubmittedToLayer) terrain->AI();
    }
    for (index = VoxelAnims.Count() - 1; index >= 0; index--) {
        VoxelAnimClass *voxelanim = VoxelAnims[index];
        if (voxelanim->IsSubmittedToLayer) voxelanim->AI();
    }
    for (index = Waves.Count() - 1; index >= 0; index--) {
        WaveClass *wave = Waves[index];
        if (wave->IsSubmittedToLayer) wave->AI();
    }
}


// bool LogicClass::Submit(ObjectClass *object, bool sort) (0x0053E330; vtable 0x0079A8EC slot 7)
// Westwood LOGIC.CPP: an object already in the logic layer (ObjectClass +0x84) is accepted
// at once; otherwise the layer adds it and the object is marked as submitted.

bool LogicClass::Submit(ObjectClass *object, bool sort)
{
    if (object->IsSubmittedToLayer) {
        return true;
    }
    if (LayerClass::Submit(object, sort)) {
        object->IsSubmittedToLayer = true;
        return true;
    }
    return false;
}


void LogicClass::Remove(ObjectClass *object)
{
    if (object->IsSubmittedToLayer) {
        Delete(object);
        object->IsSubmittedToLayer = false;
    }
}


class Leaf_53d820 { public: void get(); };
void Leaf_53d820::get() { unsigned int this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; *(unsigned int*)(0x7dfb34) = this_address; }


class Leaf_53d830 { public: void get(); };
void Leaf_53d830::get() { unsigned int this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; *(unsigned int*)(0x7dfb38) = this_address; }


// VectorClass<TeamClass *> constructor (0x0053E3D0)
// The out-of-line copy of the template constructor (shared/vector_class.hpp) that the local team
// list of LogicClass::AI (0x0053D890) above calls; no source of its own.
