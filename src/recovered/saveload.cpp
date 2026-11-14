// saveload.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/saveload.json carries the addresses and the rows that
// stay in their own files.


#define MAP_VIEW_DECLARE_SAVELOAD
#define BOMB_LIST_DECLARE_LOAD
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define COM_PTR_IMPLEMENT_RELEASE
#define COM_PTR_RELEASE_IN_DESTRUCTOR
#include "shared/com_persist.hpp"
#include "shared/abstract.hpp"
#include "shared/cell.hpp"
#include "shared/building_class.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/aircraft.hpp"
#include "shared/house_type.hpp"
#include "shared/house.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/building_type_list.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/ai_trigger_type.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/task_force.hpp"
#include "shared/team_type.hpp"
#include "shared/team.hpp"
#include "shared/script.hpp"
#include "shared/tag_type.hpp"
#include "shared/tag.hpp"
#include "shared/trigger_type.hpp"
#include "shared/trigger.hpp"
#include "shared/taction.hpp"
#include "shared/tevent.hpp"
#include "shared/factory_class.hpp"
#include "shared/voxel_anim_type.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/warhead_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/particle.hpp"
#include "shared/bullet_type.hpp"
#include "shared/bullet.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/smudge_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/light_source.hpp"
#include "shared/building_light.hpp"
#include "shared/tube.hpp"
#include "shared/side.hpp"
#include "shared/tiberium.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/vector_class.hpp"
#include "shared/empulse_class.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/super.hpp"
#include "shared/terrain_type.hpp"
#include "shared/terrain.hpp"
#include "shared/object.hpp"
#include "shared/saveload_game.hpp"
#include "shared/com_ptr.hpp"
#include "shared/debug_printf.hpp"
#include "shared/abstract_type.hpp"
#include "shared/id_source.hpp"
#include "shared/load_dlg.hpp"
#include "shared/saveload_lists.hpp"
#include "shared/fogged_object.hpp"
#include "shared/alpha_shape.hpp"
#include "shared/alpha_shape_list.hpp"
#include "shared/wave.hpp"
#include "shared/veinhole.hpp"
#include "shared/capture_manager.hpp"
#include "shared/parasite.hpp"
#include "shared/temporal.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/spawn_manager_list.hpp"
#include "shared/radsite.hpp"
#include "shared/foot.hpp"
#include "shared/kamikaze_tracker.hpp"
#include "shared/scenario_checkpoint.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#define LOBBY_STATE_SAVELOAD
#include "shared/lobby.hpp"
#define THEME_SAVELOAD
#include "shared/theme.hpp"
#include "shared/save_vector.hpp"
#include "shared/swizzle.hpp"
#include "shared/progress_screen.hpp"
#include "shared/main_opt.hpp"
#include "shared/main_loop.hpp"
#include "shared/cd_timer.hpp"
#include "shared/ground_type.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/scenario_tags.hpp"
#include "shared/mission.hpp"
#include "shared/savegame_info.hpp"


// The save/load unit's guarded declarations (shared/map_view.hpp, shared/object.hpp) are
// enabled before the unit first includes those headers.
// Header-static dynamic initializers of the unit at 0x00654A50: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00654A50-0x00654C31); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00654A70, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654AA0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654AC0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654AE0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654B00, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654B20, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654B40, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654B80, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654BB0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654BE0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654C10, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// Dynamic initializer (0x00654C20, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00654a50.cpp; no source of its own.


// HRESULT __fastcall Skip_Objects(IStream *stream) (0x00654C40; name hypothesis)
// Reads a count, then loads that many OLE objects and drops them (each one
// leaks: the original never releases them).
// SaveLoad.cpp expands the smart pointers' Release_Held inline (shared/com_ptr.hpp).


HRESULT __fastcall Skip_Objects(IStream *stream)
{
    int count;
    IUnknown *object;
    HRESULT result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = OleLoadFromStream(stream, IID_IUnknown_7afce0, (void **)&object);
        if (FAILED(result)) {
            return result;
        }
    }
    return S_OK;
}


// WWCRCEngine __fastcall Game_CRC() (0x00654C90; name hypothesis)
// The game-state CRC: the scenario, every object and type list (each
// object's Compute_CRC, AbstractClass slot 13), the player's house index,
// the frame and the count of the 0x00A40C60 list.

WWCRCEngine __fastcall Game_CRC()
{
    WWCRCEngine crc;
    int index;
    unknown_id_source_a3d290->Compute_CRC(crc);
    for (index = 0; index < HouseTypes.Count(); index++) {
        HouseTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Houses.Count(); index++) {
        Houses[index]->Compute_CRC(crc);
    }
    for (index = 0; index < UnitTypes.Count(); index++) {
        UnitTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Units.Count(); index++) {
        Units[index]->Compute_CRC(crc);
    }
    for (index = 0; index < InfantryTypes.Count(); index++) {
        InfantryTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Infantry.Count(); index++) {
        Infantry[index]->Compute_CRC(crc);
    }
    for (index = 0; index < BuildingTypes.Count(); index++) {
        BuildingTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
        unknown_BuildingClass_list_00a40af8[index]->Compute_CRC(crc);
    }
    for (index = 0; index < AircraftTypes.Count(); index++) {
        AircraftTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Aircraft.Count(); index++) {
        Aircraft[index]->Compute_CRC(crc);
    }
    for (index = 0; index < AITriggerTypes.Count(); index++) {
        AITriggerTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Anims.Count(); index++) {
        Anims[index]->Compute_CRC(crc);
    }
    for (index = 0; index < AnimTypes.Count(); index++) {
        AnimTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < TaskForces.Count(); index++) {
        TaskForces[index]->Compute_CRC(crc);
    }
    for (index = 0; index < TeamTypes.Count(); index++) {
        TeamTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Teams.Count(); index++) {
        Teams[index]->Compute_CRC(crc);
    }
    for (index = 0; index < ScriptTypes.Count(); index++) {
        ScriptTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Scripts.Count(); index++) {
        Scripts[index]->Compute_CRC(crc);
    }
    for (index = 0; index < TagTypes.Count(); index++) {
        TagTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Tags.Count(); index++) {
        Tags[index]->Compute_CRC(crc);
    }
    for (index = 0; index < TriggerTypes.Count(); index++) {
        TriggerTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Triggers.Count(); index++) {
        Triggers[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_TActionClass_list_00abfd88.Count(); index++) {
        unknown_TActionClass_list_00abfd88[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_TEventClass_list_00ac0808.Count(); index++) {
        unknown_TEventClass_list_00ac0808[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Factories.Count(); index++) {
        Factories[index]->Compute_CRC(crc);
    }
    for (index = 0; index < VoxelAnimTypes.Count(); index++) {
        VoxelAnimTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_VoxelAnimClass_list_008399d0.Count(); index++) {
        unknown_VoxelAnimClass_list_008399d0[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        unknown_WarheadTypeClass_list_00839b00[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_WeaponTypeClass_list_00839b90.Count(); index++) {
        unknown_WeaponTypeClass_list_00839b90[index]->Compute_CRC(crc);
    }
    for (index = 0; index < ParticleTypes.Count(); index++) {
        ParticleTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_ParticleClass_list_00a35e28.Count(); index++) {
        unknown_ParticleClass_list_00a35e28[index]->Compute_CRC(crc);
    }
    for (index = 0; index < ParticleSystems.Count(); index++) {
        ParticleSystems[index]->Compute_CRC(crc);
    }
    for (index = 0; index < ParticleSystemTypes.Count(); index++) {
        ParticleSystemTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < BulletTypes.Count(); index++) {
        BulletTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_BulletClass_list_00a40ce8.Count(); index++) {
        unknown_BulletClass_list_00a40ce8[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_WaypointPathClass_list_00b25590.Count(); index++) {
        unknown_WaypointPathClass_list_00b25590[index]->Compute_CRC(crc);
    }
    for (index = 0; index < SmudgeTypes.Count(); index++) {
        SmudgeTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < OverlayTypes.Count(); index++) {
        OverlayTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_LightSourceClass_list_00a6e818.Count(); index++) {
        unknown_LightSourceClass_list_00a6e818[index]->Compute_CRC(crc);
    }
    for (index = 0; index < BuildingLights.Count(); index++) {
        BuildingLights[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_TubeClass_list_008661c8.Count(); index++) {
        unknown_TubeClass_list_008661c8[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Sides.Count(); index++) {
        Sides[index]->Compute_CRC(crc);
    }
    for (index = 0; index < unknown_TiberiumClass_list_00ac0b30.Count(); index++) {
        unknown_TiberiumClass_list_00ac0b30[index]->Compute_CRC(crc);
    }
    for (index = 0; index < EMPulseClass::Array.Count(); index++) {
        EMPulseClass::Array[index]->Compute_CRC(crc);
    }
    for (index = 0; index < SuperWeaponTypes.Count(); index++) {
        SuperWeaponTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Supers.Count(); index++) {
        Supers[index]->Compute_CRC(crc);
    }
    for (index = 0; index < TerrainTypes.Count(); index++) {
        TerrainTypes[index]->Compute_CRC(crc);
    }
    for (index = 0; index < Terrains.Count(); index++) {
        Terrains[index]->Compute_CRC(crc);
    }
    if (unknown_player_a35db4 != 0) {
        crc((long)unknown_player_a35db4->ArrayIndex);
    }
    crc((long)Frame);
    crc((long)unknown_objects_a40c60.Count());
    return crc;
}


// bool __fastcall Unknown_006554A0(char const *filename, wchar_t const *description, int unknown) (0x006554A0;
// TibSun saveload.cpp Save_Game)
// Creates the save file, writes its summary (SaveVersionInfo 0x00659530), then writes everything
// (Put_All 0x006558B0) to its CONTENTS stream through a compressing link stream and commits the file.
// The DebugString lines (release-build empty inlines, shared/saveload_lists.hpp) are TibSun's up to the
// content stream; with them the inline budget is the original's (hypothesis: RA2 drops the later
// ones and prints the link failure through Debug_Printf).

bool __fastcall Put_All(IStream *stream, int save_net);

bool __fastcall Unknown_006554A0(char const *filename, wchar_t const *description, int unknown)
{
    wchar_t name[64];
    Debug_Printf("\nSAVING GAME [%s - %s]\n", filename, description);
    DebugString("Creating DocFile\n");
    UnknownComPtr<IStorage, &unknown_uuid_IStorage> storage;
    MultiByteToWideChar(0, 0, filename, -1, name, 64);
    if (FAILED(StgCreateDocfile(name, 0x1012, 0, &storage))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }

    SaveVersionInfo info;
    info.Set_Internal_Version(unknown_savegame_version_7f1b18);
    info.Set_Scenario_Description(description);
    info.Set_Version(1);
    info.Set_Player_House(unknown_player_a35db4->Type->UIName);
    info.Set_Campaign_Number(unknown_id_source_a3d290->unknown_224c);
    info.Set_Scenario_Number(unknown_id_source_a3d290->TechLevel);
    info.Set_Executable_Name("SUN.EXE");
    info.Set_Game_Type(unknown_game_mode_a3d298);
    FILETIME time;
    CoFileTimeNow(&time);
    info.Set_Last_Time(time);
    info.Set_Start_Time(time);
    info.Set_Play_Time(time);
    DebugString("Saving version information\n");
    info.Save(storage);

    DebugString("Creating content stream\n");
    UnknownComPtr<IStream, &unknown_uuid_IStream> content;
    if (FAILED(storage.Checked()->CreateStream(L"CONTENTS", 0x1012, 0, 0, &content))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }

    UnknownComPtr<ILinkStream, &unknown_uuid_ILinkStream_7f1b50> link;
    link.CreateInstance(CLSID_CStreamClass, 0, 7);
    if (FAILED(link.Checked()->Link_Stream(content))) {
        Debug_Printf("\t***** FAILED!\n");
        return false;
    }
    UnknownComPtr<IStream, &unknown_uuid_IStream_7d5988> stream(link);

    bool result = Put_All(stream, 0);

    if (FAILED(link.Checked()->Unlink_Stream(0))) {
        return false;
    }
    content.Release();
    if (FAILED(((IFullStorage *)storage.Checked())->Commit(0))) {
        return false;
    }
    Debug_Printf("SAVING GAME [%s] - Complete\n\n", filename, description);
    return result;
}


// bool __fastcall Put_All(IStream *stream, int save_net) (0x006558B0; TibSun saveload.cpp Put_All)
// Writes the scenario, the checkpoint, the rules, every object and type list, the
// misc values, the logic and tactical map, the trackers, the skirmish options and
// the theme. Lists go through Save_Vector (shared/save_vector.hpp): the inline
// budget expands the first 23 and keeps the rest as out-of-line COMDATs
// (0x006580C0-0x00659140); the pointer list 0x00A35DB8 is written as raw pointers;
// the AITriggerTypes and Actions results are not tested. DebugString is the
// release build's empty inline (TibSun dbgprint.h): its calls emit nothing, but
// their IL sets the caller's inline budget (the "Saving" lines of the non-list
// steps and the FAILED lines; which ones the original kept is a hypothesis
// bounded by the budget: 6 to ~20 "Saving" lines give the original's 23 inline
// copies). The skirmish lines call Debug_Printf 0x004068F0.

bool __fastcall Put_All(IStream *stream, int save_net)
{
    unknown_id_source_a3d290->Save(stream);
    unknown_scenario_checkpoint_00855a90.Write_004B5C70(stream);
    Rules->Save(stream);
    if (FAILED(Save_Vector(stream, AnimTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Map\n");
    Map.Save(stream);
    if (FAILED(Save_Vector(stream, unknown_TubeClass_list_008661c8))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Misc. Values\n");
    if (FAILED(Save_Misc_Values(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Logic\n");
    Logic.Save(stream);
    DebugString("Saving TacticalMap\n");
    if (FAILED(OleSaveToStream(unknown_tactical_83996c, stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, HouseTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Houses))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Units))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, UnitTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, InfantryTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Infantry))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, BuildingTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_BuildingClass_list_00a40af8))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, AircraftTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Aircraft))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Anims))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, TaskForces))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, TeamTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Teams))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, ScriptTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Scripts))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, TagTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Tags))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, TriggerTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Triggers))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    Save_Vector(stream, AITriggerTypes);
    DebugString("\t***** FAILED!\n");
    Save_Vector(stream, unknown_TActionClass_list_00abfd88);
    DebugString("\t***** FAILED!\n");
    if (FAILED(Save_Vector(stream, unknown_TEventClass_list_00ac0808))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Factories))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, VoxelAnimTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_VoxelAnimClass_list_008399d0))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_WarheadTypeClass_list_00839b00))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_WeaponTypeClass_list_00839b90))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, ParticleTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_ParticleClass_list_00a35e28))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, ParticleSystemTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, ParticleSystems))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, BulletTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_BulletClass_list_00a40ce8))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_WaypointPathClass_list_00b25590))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, SmudgeTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, OverlayTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_LightSourceClass_list_00a6e818))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, BuildingLights))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Sides))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_TiberiumClass_list_00ac0b30))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, EMPulseClass::Array))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, SuperWeaponTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Supers))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    {
        int count = unknown_supers_a35db8.Count();
        HRESULT result = stream->Write(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            for (int index = 0; index < count; index++) {
                SuperClass *super = unknown_supers_a35db8[index];
                result = stream->Write(&super, sizeof(super), 0);
                if (FAILED(result)) {
                    break;
                }
            }
        }
        if (FAILED(result)) {
            return false;
        }
    }
    if (FAILED(Save_Vector(stream, TerrainTypes))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Terrains))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, FoggedObjectClass::Array))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_AlphaShapeClass_list_0083c648))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, Waves))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving VeinholeMonster\n");
    if (!VeinholeMonsterClass::Save_All(stream)) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving RadarEvents\n");
    if (!RadarEventClass_unknown_save_all(stream)) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_CaptureManagerClass_list_008505a0))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_ParasiteClass_list_00a76430))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_TemporalClass_list_00ac02e0))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_SpawnManagerClass_list_00abd028))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Unit Tracker\n");
    if (FAILED(unknown_tracker_839ea0.Save_00413590(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving KamikazeTracker\n");
    if (FAILED(KamikazeTracker.Save(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving BombList\n");
    if (FAILED(BombList.Save(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_BombClass_list_0084eb78))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Save_Vector(stream, unknown_RadSiteClass_list_00ab6620))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (unknown_game_mode_a3d298 == 5) {
        Debug_Printf("Writing Skirmish Session.Options\n");
        if (!unknown_lobby_state_a3d2b0.Save(stream)) {
            Debug_Printf("\t***** FAILED!\n");
            return false;
        }
    }
    DebugString("Saving Static Sounds\n");
    if (FAILED(Voc_Static_Save(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Speech\n");
    if (FAILED(Speech_Save(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    DebugString("Saving Theme\n");
    if (FAILED(Theme.Save(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    return true;
}


// bool __fastcall Unknown_00656890(char const *filename) (0x00656890; TibSun saveload.cpp Load_Game)
// Reads the saved game's summary (Get_Savefile_Info 0x00657FA0, expanded here) for its game mode,
// opens the file's CONTENTS stream through a decompressing link stream and reads everything back
// (Get_All 0x00656B80), then rebuilds the derived state of the map and the sidebar. The two
// DebugString lines (release-build empty inlines, shared/saveload_lists.hpp) mirror TibSun's
// Save_Game ones; their sites give the original's inline budget (hypothesis: the text is unknown).

bool __fastcall Get_All(IStream *stream, bool save_net);
void __fastcall Post_Load_Game_0065d2c0();             // 0x0065D2C0

bool __fastcall Unknown_00656890(char const *filename)
{
    wchar_t name[64];
    Debug_Printf("\nLOADING GAME [%s]\n", filename);
    SaveVersionInfo info;
    if (!Get_Savefile_Info(filename, info)) {
        return false;
    }
    unknown_game_mode_a3d298 = info.Get_Game_Type();
    Swizzler.Reset();

    UnknownComPtr<IStorage, &unknown_uuid_IStorage> storage;
    MultiByteToWideChar(0, 0, filename, -1, name, 64);
    if (FAILED(StgOpenStorage(name, 0, 0x20, 0, 0, &storage))) {
        return false;
    }

    UnknownComPtr<IStream, &unknown_uuid_IStream> content;
    if (FAILED(storage.Checked()->OpenStream(L"CONTENTS", 0, 0x10, 0, &content))) {
        return false;
    }

    UnknownComPtr<ILinkStream, &unknown_uuid_ILinkStream_7f1b50> link;
    link.CreateInstance(CLSID_CStreamClass, 0, 7);
    link.Checked()->Link_Stream(content);
    UnknownComPtr<IStream, &unknown_uuid_IStream_7d5988> stream(link);

    ProgressScreen.Init(100.0, 0, 0);
    DebugString("Calling Get_All()\n");
    Get_All(stream, false);
    DebugString("Unlinking content stream from decompressor\n");
    link.Checked()->Unlink_Stream(0);
    Swizzler.Reset();

    Post_Load_Game_0065d2c0();
    Map.Init_IO();
    Map.unknown_slot54(1);
    Map.Reposition_Sidebar();
    TiberiumClass::Init_Growth();
    TiberiumClass::Init_Spread();
    Map.Complete_Radar_Refresh();
    unknown_flag_a40d04 = true;
    unknown_flag_a40380 = true;
    Unknown_Free_Art_Set_006F30F0();
    Debug_Printf("LOADING GAME [%s] - Complete\n\n", filename);
    return true;
}


// bool __fastcall Get_All(IStream *stream, bool save_net) (0x00656B80; TibSun saveload.cpp Get_All)
// Clears the scenario, reads the scenario, prepares the side, reallocates the surfaces
// for the tactical view, then reads back everything Put_All 0x006558B0 wrote. Each
// object list is recreated through Skip_Objects 0x00654C40 (TibSun's Load_Vector: the
// objects re-add themselves); the inline budget expands the early calls.

bool __fastcall Get_All(IStream *stream, bool save_net)
{
    Clear_Scenario_0065d370();
    unknown_id_source_a3d290->Load(stream);
    if (!Unknown_0051a330(unknown_id_source_a3d290->unknown_2239 ? 0 : 1)) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    Rect temp = Get_Title_Screen_Area_006f1f30();
    {
        Rect sidebar(0, 0, unknown_int_7add80, unknown_window_839640.Height);
        Rect tile(0, 0, temp.Width, unknown_window_839640.Height);
        Rect composite(0, 0, temp.Width, unknown_window_839640.Height);
        Allocate_Surfaces(&unknown_window_839640, &composite, &tile, &sidebar, false);
    }
    Map.unknown_slot42(temp);
    unknown_scenario_checkpoint_00855a90.Read_004B5C50(stream);
    Init_Theater_00519e70(unknown_id_source_a3d290->Theater);
    Rules->Load(stream);
    if (FAILED(Skip_Objects(stream))) {    // AnimTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    Map.Load(stream);
    if (FAILED(Skip_Objects(stream))) {    // Tubes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Load_Misc_Values(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    Map.Reset_All_Subzones();
    Logic.Load(stream);
    if (unknown_tactical_83996c != 0) {
        delete unknown_tactical_83996c;
        unknown_tactical_83996c = 0;
    }
    Tactical *old_tactical;
    if (FAILED(OleLoadFromStream(stream, IID_IUnknown_7afce0, (void **)&old_tactical))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // HouseTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Houses
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Units
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // UnitTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // InfantryTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Infantry
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // BuildingTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Buildings
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // AircraftTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Aircraft
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Anims
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // TaskForces
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // TeamTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Teams
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // ScriptTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Scripts
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // TagTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Tags
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // TriggerTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Triggers
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // AITriggerTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Actions
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Events
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Factories
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // VoxelAnimTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // VoxelAnims
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Warheads
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Weapons
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // ParticleTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Particles
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // ParticleSystemTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // ParticleSystems
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // BulletTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Bullets
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // WaypointPaths
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // SmudgeTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // OverlayTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // LightSources
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // BuildingLights
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Sides
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Tiberiums
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // EMPulses
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // SuperWeaponTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // SuperWeapons
        DebugString("\t***** FAILED!\n");
        return false;
    }
    unknown_supers_a35db8.Clear();
    {
        int count;
        SuperClass *super;
        HRESULT result = stream->Read(&count, sizeof(count), 0);
        if (SUCCEEDED(result) && count > 0) {
            int index;
            for (index = 0; index < count; index++) {
                result = stream->Read(&super, sizeof(super), 0);
                if (FAILED(result)) {
                    break;
                }
                unknown_supers_a35db8.Add(super);
            }
            for (index = 0; index < count; index++) {
                Swizzler.Swizzle((void **)&unknown_supers_a35db8[index]);
            }
        }
        if (FAILED(result)) {
            DebugString("\t***** FAILED!\n");
            return false;
        }
    }
    if (FAILED(Skip_Objects(stream))) {    // TerrainTypes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Terrains
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // FoggedObjects
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // AlphaShapes
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Waves
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (!VeinholeMonsterClass::Load_All(stream)) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (!RadarEventClass_unknown_load_all(stream)) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // CaptureManagers
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Parasites
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Temporals
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // SpawnManagers
        DebugString("\t***** FAILED!\n");
        return false;
    }
    unknown_tracker_839ea0.Clear_00413540();
    if (FAILED(unknown_tracker_839ea0.Load_00413400(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    KamikazeTracker.Clear();
    if (FAILED(KamikazeTracker.Load(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    BombList.Clear();
    if (FAILED(BombList.Load(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // Bombs
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Skip_Objects(stream))) {    // RadSites
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (unknown_game_mode_a3d298 == 5) {
        Debug_Printf("Reading Skirmish Session.Options\n");
        if (!unknown_lobby_state_a3d2b0.Load(stream)) {
            Debug_Printf("\t***** FAILED!\n");
            return false;
        }
    }
    if (FAILED(Voc_Static_Load(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Speech_Load(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    if (FAILED(Theme.Load(stream))) {
        DebugString("\t***** FAILED!\n");
        return false;
    }
    Map.Flag_To_Redraw(2);
    if (unknown_game_mode_a3d298 == 0) {
        unknown_flag_a6ebd0 = true;
    }
    return true;
}


// HRESULT __fastcall Save_Misc_Values(IStream *stream) (0x00657A60; TibSun saveload.cpp)
// Writes the gas system, the player's house, the frame, the selected objects,
// the land types, the lightning storm, the two scenario tag lists, the crate
// tables, the mission table and 0x00AB9174.

HRESULT __fastcall Save_Misc_Values(IStream *stream)
{
    int i;
    int count;
    HRESULT res;

    res = stream->Write(&unknown_gas_cloud_system_a40d20, sizeof(unknown_gas_cloud_system_a40d20), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&unknown_player_a35db4, sizeof(unknown_player_a35db4), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&Frame, sizeof(Frame), 0);
    if (FAILED(res)) {
        return res;
    }
    count = unknown_objects_a40c60.Count();
    res = stream->Write(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Write(&unknown_objects_a40c60[i], sizeof(unknown_objects_a40c60[i]), 0);
        if (FAILED(res)) {
            return res;
        }
    }
    res = stream->Write(&Ground, sizeof(Ground), 0);
    if (FAILED(res)) {
        return res;
    }
    Save_Storm_0051F3F0(stream);
    count = unknown_tags_866160.Count();
    res = stream->Write(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Write(&unknown_tags_866160[i], sizeof(unknown_tags_866160[i]), 0);
        if (FAILED(res)) {
            return res;
        }
    }
    count = unknown_tags_866238.Count();
    res = stream->Write(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Write(&unknown_tags_866238[i], sizeof(unknown_tags_866238[i]), 0);
        if (FAILED(res)) {
            return res;
        }
    }
    res = stream->Write(&CrateShares, sizeof(CrateShares), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&CrateAnims, sizeof(CrateAnims), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&CrateData, sizeof(CrateData), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&CrateWater, sizeof(CrateWater), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&MissionControl, sizeof(MissionControl), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Write(&unknown_ab9174, sizeof(unknown_ab9174), 0);
    if (FAILED(res)) {
        return res;
    }
    return S_OK;
}


// HRESULT __fastcall Load_Misc_Values(IStream *stream) (0x00657C40; TibSun saveload.cpp)
// Reads what Save_Misc_Values 0x00657A60 writes, re-adding the saved selected
// objects and tags and swizzling every saved pointer.

HRESULT __fastcall Load_Misc_Values(IStream *stream)
{
    int i;
    int count;
    void *ptr;
    HRESULT res;

    res = stream->Read(&unknown_gas_cloud_system_a40d20, sizeof(unknown_gas_cloud_system_a40d20), 0);
    if (FAILED(res)) {
        return res;
    }
    Swizzler.Swizzle((void **)&unknown_gas_cloud_system_a40d20);
    res = stream->Read(&unknown_player_a35db4, sizeof(unknown_player_a35db4), 0);
    if (FAILED(res)) {
        return res;
    }
    Swizzler.Swizzle((void **)&unknown_player_a35db4);
    res = stream->Read(&Frame, sizeof(Frame), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Read(&ptr, sizeof(ptr), 0);
        if (FAILED(res)) {
            return res;
        }
        unknown_objects_a40c60.Add(*(ObjectClass **)&ptr);
    }
    for (i = 0; i < unknown_objects_a40c60.Count(); i++) {
        Swizzler.Swizzle((void **)&unknown_objects_a40c60[i]);
    }
    res = stream->Read(&Ground, sizeof(Ground), 0);
    if (FAILED(res)) {
        return res;
    }
    Load_Storm_0051F5E0(stream);
    res = stream->Read(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Read(&ptr, sizeof(ptr), 0);
        if (FAILED(res)) {
            return res;
        }
        unknown_tags_866160.Add(*(TagClass **)&ptr);
    }
    for (i = 0; i < unknown_tags_866160.Count(); i++) {
        Swizzler.Swizzle((void **)&unknown_tags_866160[i]);
    }
    res = stream->Read(&count, sizeof(count), 0);
    if (FAILED(res)) {
        return res;
    }
    for (i = 0; i < count; i++) {
        res = stream->Read(&ptr, sizeof(ptr), 0);
        if (FAILED(res)) {
            return res;
        }
        unknown_tags_866238.Add(*(TagClass **)&ptr);
    }
    for (i = 0; i < unknown_tags_866238.Count(); i++) {
        Swizzler.Swizzle((void **)&unknown_tags_866238[i]);
    }
    res = stream->Read(&CrateShares, sizeof(CrateShares), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&CrateAnims, sizeof(CrateAnims), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&CrateData, sizeof(CrateData), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&CrateWater, sizeof(CrateWater), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&MissionControl, sizeof(MissionControl), 0);
    if (FAILED(res)) {
        return res;
    }
    res = stream->Read(&unknown_ab9174, sizeof(unknown_ab9174), 0);
    if (FAILED(res)) {
        return res;
    }
    return S_OK;
}


// bool __fastcall Get_Savefile_Info(char const *filename, SaveVersionInfo &info) (0x00657FA0; name hypothesis)
// Opens a saved game's storage and reads its summary.

bool __fastcall Get_Savefile_Info(char const *filename, SaveVersionInfo &info)
{
    wchar_t wide[64];
    UnknownComPtr<IStorage, &unknown_uuid_IStorage> storage;
    MultiByteToWideChar(0, 0, filename, -1, wide, 64);
    if (FAILED(StgOpenStorage(wide, 0, 0x12, 0, 0, &storage))) {
        return false;
    }
    if (FAILED(info.Load(storage))) {
        return false;
    }
    return true;
}


// 0x00658040: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_00658040(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00658040 {

void  unknown_empty_00658040(void)
{
    return;
}

}  // namespace seed_00658040


// void UnknownComPtr<IStorage>::Release_Held() (0x00658050)
// comip's _Release, emitted out of line by Save_Game 0x006554A0 and Load_Game 0x00656890
// (shared/com_ptr.hpp).


// void UnknownComPtr<ILinkStream>::Release_Held() (0x00658060)
// comip's _Release, emitted out of line by the link pointers' CreateInstance in Save_Game
// 0x006554A0 and Load_Game 0x00656890 (shared/com_ptr.hpp).


// bool DynamicVectorClass<SuperClass *>::Add(SuperClass * const &object) (0x00658070)
// The out-of-line copy Get_All 0x00656B80 calls for the pointer list 0x00A35DB8 (shared/vector_class.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TActionClass *> const &list) (0x006580C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TEventClass *> const &list) (0x00658140)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<FactoryClass *> const &list) (0x006581C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<VoxelAnimTypeClass *> const &list) (0x00658240)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<VoxelAnimClass *> const &list) (0x006582C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<WarheadTypeClass *> const &list) (0x00658340)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<WeaponTypeClass *> const &list) (0x006583C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<ParticleTypeClass *> const &list) (0x00658440)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<ParticleClass *> const &list) (0x006584C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<ParticleSystemTypeClass *> const &list) (0x00658540)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<ParticleSystemClass *> const &list) (0x006585C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<BulletTypeClass *> const &list) (0x00658640)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<BulletClass *> const &list) (0x006586C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<WaypointPathClass *> const &list) (0x00658740)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<SmudgeTypeClass *> const &list) (0x006587C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<OverlayTypeClass *> const &list) (0x00658840)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<LightSourceClass *> const &list) (0x006588C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<BuildingLightClass *> const &list) (0x00658940)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<SideClass *> const &list) (0x006589C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TiberiumClass *> const &list) (0x00658A40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<EMPulseClass *> const &list) (0x00658AC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<SuperWeaponTypeClass *> const &list) (0x00658B40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<SuperClass *> const &list) (0x00658BC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TerrainTypeClass *> const &list) (0x00658C40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TerrainClass *> const &list) (0x00658CC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<FoggedObjectClass *> const &list) (0x00658D40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<AlphaShapeClass *> const &list) (0x00658DC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<WaveClass *> const &list) (0x00658E40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<CaptureManagerClass *> const &list) (0x00658EC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<ParasiteClass *> const &list) (0x00658F40)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<TemporalClass *> const &list) (0x00658FC0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<SpawnManagerClass *> const &list) (0x00659040)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<BombClass *> const &list) (0x006590C0)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT __fastcall Save_Vector(IStream *stream, DynamicVectorClass<RadSiteClass *> const &list) (0x00659140)
// The out-of-line copy Put_All 0x006558B0 emits (shared/save_vector.hpp).


// HRESULT UnknownComPtr<IStream>::Query_From(UnknownComPtr<ILinkStream> const &link) (0x006591C0)
// comip's _QueryInterface from another smart pointer, emitted out of line by the stream pointers
// Save_Game 0x006554A0 and Load_Game 0x00656890 build from their link pointer (shared/com_ptr.hpp).
