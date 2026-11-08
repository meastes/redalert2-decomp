// NON-EXACT DRAFT 0x68E280 WinMain
// entry 0x68E280; exact-match score 98.55 (misses_index best); plan "draft, adapted"
// origin lane: codex4 (residue_batch_96/immediate.cpp, 98.55); native adaptation Scout1058 (build/class-work/Scout1058/draft/winmain_68e280_native.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// native adaptation: codex4's private shared/startup_*.hpp inlined; RA2_STARTUP_* header additions spelled locally; SEH __try/__except around Main_Game dropped (no SEH natively)
// NATIVE ADAPTATION of codex4's WinMain 0x0068E280 draft (NON-EXACT, best 98.55,
// C:/ra2/w/codex4/build/class-work/codex4/residue_batch_96/immediate.cpp, 2026-11-07).
// Its private headers (shared/startup_*.hpp of that batch) are inlined below;
// header-only additions it relied on (RA2_STARTUP_* macros) are spelled locally.
// Text adapted from TibSun/code/startup.cpp (GPL-3.0-or-later, EA section 7 terms).
// RA2_STARTUP_*: the header additions the draft batch made (spelled locally below).
#define RA2_SHIP_MOVE_TO_VALUE
#include "shared/iunknown_ptr.hpp"
#include "shared/win32_api.hpp"
#include "shared/class_factory.hpp"
#include "shared/com_registration.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/string_table.hpp"
#include "shared/ccini_class.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/options.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/keyboard_game.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/win32_window.hpp"
#include "shared/debug_printf.hpp"
#include "shared/pkey.hpp"
#include "shared/ram_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cd.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/exit.hpp"
#include "shared/windows_version.hpp"
#include "shared/win_stub.hpp"
#include "shared/dsurface.hpp"
#include "shared/audio_file.hpp"
#include "shared/house.hpp"
#include "shared/cell_class.hpp"
#include "shared/building_class.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/aircraft.hpp"
#include "shared/ai_trigger_type.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/alpha_shape.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/building_light.hpp"
#include "shared/building_type_class.hpp"
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/campaign.hpp"
#include "shared/capture_manager.hpp"
#include "shared/cstream.hpp"
#include "shared/drive_locomotion.hpp"
#include "shared/droppod_locomotion.hpp"
#include "shared/empulse_class.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/fly_locomotion.hpp"
#include "shared/fogged_object.hpp"
#include "shared/house_type.hpp"
#include "shared/hover_locomotion.hpp"
#include "shared/infantry_type.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/jumpjet_locomotion.hpp"
#include "shared/light_source.hpp"
#include "shared/mech_locomotion.hpp"
#include "shared/neuron.hpp"
#include "shared/object.hpp"
#include "shared/overlay_type.hpp"
#include "shared/parasite.hpp"
#include "shared/particle.hpp"
#include "shared/particle_system.hpp"
#include "shared/radsite.hpp"
#include "shared/rocket_locomotion.hpp"
#include "shared/script.hpp"
#include "shared/ship_locomotion.hpp"
#include "shared/side.hpp"
#include "shared/smudge_type.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/tactical.hpp"
#include "shared/taction.hpp"
#include "shared/tag.hpp"
#include "shared/tag_type.hpp"
#include "shared/task_force.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/teleport_locomotion.hpp"
#include "shared/temporal.hpp"
#include "shared/terrain.hpp"
#include "shared/terrain_type.hpp"
#include "shared/tevent.hpp"
#include "shared/tiberium.hpp"
#include "shared/trigger.hpp"
#include "shared/trigger_type.hpp"
#include "shared/tube.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/unit_type.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/voxel_anim_type.hpp"
#include "shared/walk_locomotion.hpp"
#include "shared/warhead_type.hpp"
#include "shared/wave.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/weapon_type.hpp"
#include "shared/wwmouse.hpp"
#include "shared/blit.hpp"
#include "shared/get_cd.hpp"
#include "shared/cd_file_class.hpp"
#include "shared/special.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/theme.hpp"
struct RegStruct { GUID const *clsid; char const *name; };
extern "C" RegStruct const RegisterTheseDLLs[1]; // original readonly eight-byte table007AB988
// Addressable first-vptr field of the precreated factory; its complete class is not yet declared.
class TiberianSunClassFactory;
extern TiberianSunClassFactory unknown_class_factory_a4088c;   // NATIVE: the tree's name for 0x00A4088C
extern char startup_source_file_7f504c[];
typedef HRESULT (__stdcall *StartupDllRegisterFunction)();
extern "C" __declspec(dllimport) HINSTANCE __stdcall LoadLibraryA(char const *name);
extern "C" __declspec(dllimport) BOOL __stdcall FreeLibrary(HINSTANCE module);
extern "C" __declspec(dllimport) StartupDllRegisterFunction __stdcall GetProcAddress(HINSTANCE module, char const *name);
extern "C" __declspec(dllimport) HRESULT __stdcall CoRegisterClassObject(GUID const &clsid, IUnknown *factory, DWORD context, DWORD flags, DWORD *registration);
extern "C" __declspec(dllimport) HRESULT __stdcall RegisterActiveObject(IUnknown *object, GUID const &clsid, DWORD flags, DWORD *registration);
extern "C" __declspec(noreturn) void __cdecl exit(int result);
int __fastcall Unknown_006FA040(HWND window, unsigned short const *text, unsigned short const *caption, unsigned int type);
extern "C" GUID const CLSID_CStreamClass;
extern "C" GUID const CLSID_WaveClass;
extern "C" GUID const CLSID_TerrainTypeClass;
extern "C" GUID const CLSID_TerrainClass;
extern "C" GUID const CLSID_SuperWeaponTypeClass;
extern "C" GUID const CLSID_SuperClass;
extern "C" GUID const Startup_CLSID_Tactical;
extern "C" GUID const Startup_CLSID_CellClass;
extern "C" GUID const Startup_CLSID_EMPulseClass;
extern "C" GUID const Startup_CLSID_LightSourceClass;
extern "C" GUID const CLSID_SideClass;
extern "C" GUID const Startup_CLSID_TiberiumClass;
extern "C" GUID const CLSID_TubeClass;
extern "C" GUID const CLSID_CampaignClass;
extern "C" GUID const CLSID_BuildingLightClass;
extern "C" GUID const CLSID_WaypointPathClass;
extern "C" GUID const CLSID_TemporalClass;
extern "C" GUID const CLSID_SpawnManagerClass;
extern "C" GUID const CLSID_CaptureManagerClass;
extern "C" GUID const CLSID_ParasiteClass;
extern "C" GUID const Startup_CLSID_BombClass;
extern "C" GUID const CLSID_RadSiteClass;
extern "C" GUID const CLSID_TEventClass;
extern "C" GUID const CLSID_VoxelAnimTypeClass;
extern "C" GUID const CLSID_VoxelAnimClass;
extern "C" GUID const CLSID_TActionClass;
extern "C" GUID const CLSID_TriggerClass;
extern "C" GUID const CLSID_TriggerTypeClass;
extern "C" GUID const CLSID_ScriptClass;
extern "C" GUID const CLSID_ScriptTypeClass;
extern "C" GUID const CLSID_TagClass;
extern "C" GUID const CLSID_TagTypeClass;
extern "C" GUID const CLSID_TeamClass;
extern "C" GUID const CLSID_TeamTypeClass;
extern "C" GUID const CLSID_TaskForceClass;
extern "C" GUID const Startup_CLSID_UnitTypeClass;
extern "C" GUID const Startup_CLSID_BuildingTypeClass;
extern "C" GUID const Startup_CLSID_AircraftTypeClass;
extern "C" GUID const Startup_CLSID_InfantryTypeClass;
extern "C" GUID const CLSID_BulletTypeClass;
extern "C" GUID const CLSID_IsometricTileTypeClass;
extern "C" GUID const CLSID_OverlayTypeClass;
extern "C" GUID const CLSID_SmudgeTypeClass;
extern "C" GUID const CLSID_UnitClass;
extern "C" GUID const CLSID_BuildingClass;
extern "C" GUID const CLSID_AircraftClass;
extern "C" GUID const CLSID_InfantryClass;
extern "C" GUID const CLSID_AnimClass;
extern "C" GUID const CLSID_AnimTypeClass;
extern "C" GUID const CLSID_HouseTypeClass;
extern "C" GUID const CLSID_HouseClass;
extern "C" GUID const CLSID_DriveLocomotionClass;
extern "C" GUID const CLSID_JumpjetLocomotionClass;
extern "C" GUID const CLSID_HoverLocomotionClass;
extern "C" GUID const CLSID_RocketLocomotionClass;
extern "C" GUID const CLSID_TunnelLocomotionClass;
extern "C" GUID const CLSID_WalkLocomotionClass;
extern "C" GUID const CLSID_DropPodLocomotionClass;
extern "C" GUID const CLSID_FlyLocomotionClass;
extern "C" GUID const CLSID_TeleportLocomotionClass;
extern "C" GUID const CLSID_MechLocomotionClass;
extern "C" GUID const CLSID_ShipLocomotionClass;
extern "C" GUID const CLSID_BulletClass;
extern "C" GUID const CLSID_FactoryClass;
extern "C" GUID const CLSID_WarheadTypeClass;
extern "C" GUID const CLSID_WeaponTypeClass;
extern "C" GUID const CLSID_ParticleClass;
extern "C" GUID const CLSID_ParticleTypeClass;
extern "C" GUID const CLSID_ParticleSystemClass;
extern "C" GUID const CLSID_ParticleSystemTypeClass;
extern "C" GUID const CLSID_AITriggerTypeClass;
extern "C" GUID const CLSID_NeuronClass;
extern "C" GUID const CLSID_FoggedObjectClass;
extern "C" GUID const CLSID_AlphaShapeClass;
#define RA2_STARTUP_KEYBOARD_CTOR
extern bool unknown_tile_sysmem_7f4cc4;
unsigned int __cdecl Query_Free_Disk_Kilobytes_00482c80();
extern "C" int __cdecl sscanf(char const *text, char const *format, ...);
WIN32_IMPORT HRESULT WINAPI OleInitialize(void *reserved);
extern DWORD main_thread_00b25f78;
WIN32_IMPORT DWORD WINAPI GetCurrentThreadId();
WIN32_IMPORT HANDLE WINAPI OpenMutexA(DWORD access, BOOL inherit, LPCSTR name);
WIN32_IMPORT HWND WINAPI FindWindowA(LPCSTR class_name, LPCSTR window_name);
extern "C" unsigned int __cdecl _controlfp(unsigned int control, unsigned int mask);
extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *name, char const *extension);
void __cdecl set_control_word(); // Westwood math entry0077F0F4, not a synthesized compiler helper
char *__fastcall strtrim(char *text);
bool __cdecl CopyProtection_Check_Already_Running_004918f0();
bool __fastcall Parse_Command_Line(int argc, char *argv[]);
extern char startup_keys_79aa70[];
extern "C" int __cdecl atexit(void (__cdecl *function)());
void __cdecl Prog_End();
void __cdecl Start_Mouse_Thread();
int __fastcall String_Table_Init_006f8e30(char const *name);
bool __fastcall Init_Language_Resources_00495590(bool value);
unsigned int __fastcall Query_DLL_Version_0049ee00(char const *name);
WIN32_IMPORT int WINAPI GetSystemMetrics(int index);
// MessageBoxA is already declared by the existing winsock interface included through Map.
extern bool unknown_hicolor_7ce008;
extern int direct_draw_depth_008523e8;
extern int unknown_record_mode_a41664;
DWORD __cdecl Get_Video_Memory();
void __cdecl Prep_Direct_Draw();
void __cdecl Release_DirectDraw_00495a60();
bool __fastcall Set_Video_Mode(HWND window, int width, int height, int bits);
int __fastcall Unknown_Audio_File_00522c50(char const *name);
int __fastcall Audio_File_Open_Stream(char const *name);
// Actual registered-class definitions for Startup, following the reference's full-type context.
// FlyLocomotionClass declares its COM-by-value Move_To argument.
// Canonical coord.hpp COM value: three ordered signed DWORDs.

extern bool unknown_redraw_8523dc;
void __cdecl Windows_Message_Handler();
void __fastcall Mono_Window_Init(void *, HWND, bool);
void __cdecl Planning_Reset();
bool __fastcall Allocate_Surfaces(Rect *, Rect *, Rect *, Rect *, bool);
long __cdecl Init_Direct3D();
bool __cdecl Notify_Launcher_00491930();
void __cdecl Unknown_00691320();
int __fastcall Unknown_0046E4C0(int);
int __fastcall Get_Performance_Profile();
void __fastcall Main_Game(int argc, char *argv[]);
void __cdecl Audio_Shutdown();
void __cdecl RadarEventClass_unknown_delete_all_63b890();
void __fastcall Unknown_Free_All_Art_006F1D20();
void __cdecl Unknown_0059D900();
void __cdecl Shutdown_Network_005bc4d0();
extern bool startup_no_exception_handler_a4168c;
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.

static void Init_Keys()
{
    // Init_Keys from TibSun init.cpp, with RA2's observed local lifetimes.
    Debug_Printf("Init_Keys - declarations\n");
    RAMFileClass file((void *)startup_keys_79aa70, strlen(startup_keys_79aa70));
    INIClass ini;
    Debug_Printf("Init_Keys - Load\n");
    ini.Load(file, false);
    Debug_Printf("Init_Keys - Init fast key\n");
    FastKey = ini.Get_PKey(true);
}
static bool RegisterClasses()
{
IClassFactory *factory;
bool failed = false;
for (int index = 0; index < 1; ++index) {
    IUnknownPtr ptr;
    failed = FAILED(ptr.CreateInstance(*RegisterTheseDLLs[index].clsid, 0, CLSCTX_ALL));
    if (failed) {
        failed = false;
        HINSTANCE module = LoadLibraryA(RegisterTheseDLLs[index].name);
        if (module != 0) {
            StartupDllRegisterFunction register_server = GetProcAddress(module, "DllRegisterServer");
            if (!register_server || (register_server(), FAILED(ptr.CreateInstance(*RegisterTheseDLLs[index].clsid, 0, CLSCTX_ALL)))) {
                failed = true;
            }
            FreeLibrary(module);
        } else {
            failed = true;
        }
    }
    if (failed) break;
    ptr.Release();
}
Register_Com_Server();
DWORD registration;
// The precreated-factory macro in TibSun startup.cpp owns this block.
{
    factory = (IClassFactory *)&unknown_class_factory_a4088c;
    CoRegisterClassObject(CLSID_TiberianSun, factory, CLSCTX_INPROC_SERVER, 1, &registration);
    unknown_unsigned_long_list_00abd428.Add(registration);
}
#define REGISTER_STARTUP_CLASS(Class, Clsid) \
    { \
        factory = new TClassFactory<Class>; \
        CoRegisterClassObject(Clsid, factory, CLSCTX_INPROC_SERVER, 1, &registration); \
        unknown_unsigned_long_list_00abd428.Add(registration); \
    }
REGISTER_STARTUP_CLASS(CStreamClass, CLSID_CStreamClass);
REGISTER_STARTUP_CLASS(WaveClass, CLSID_WaveClass);
REGISTER_STARTUP_CLASS(TerrainTypeClass, CLSID_TerrainTypeClass);
REGISTER_STARTUP_CLASS(TerrainClass, CLSID_TerrainClass);
REGISTER_STARTUP_CLASS(SuperWeaponTypeClass, CLSID_SuperWeaponTypeClass);
REGISTER_STARTUP_CLASS(SuperClass, CLSID_SuperClass);
REGISTER_STARTUP_CLASS(Tactical, Startup_CLSID_Tactical);
REGISTER_STARTUP_CLASS(CellClass, Startup_CLSID_CellClass);
REGISTER_STARTUP_CLASS(EMPulseClass, Startup_CLSID_EMPulseClass);
REGISTER_STARTUP_CLASS(LightSourceClass, Startup_CLSID_LightSourceClass);
REGISTER_STARTUP_CLASS(SideClass, CLSID_SideClass);
REGISTER_STARTUP_CLASS(TiberiumClass, Startup_CLSID_TiberiumClass);
REGISTER_STARTUP_CLASS(TubeClass, CLSID_TubeClass);
REGISTER_STARTUP_CLASS(CampaignClass, CLSID_CampaignClass);
REGISTER_STARTUP_CLASS(BuildingLightClass, CLSID_BuildingLightClass);
REGISTER_STARTUP_CLASS(WaypointPathClass, CLSID_WaypointPathClass);
REGISTER_STARTUP_CLASS(TemporalClass, CLSID_TemporalClass);
REGISTER_STARTUP_CLASS(SpawnManagerClass, CLSID_SpawnManagerClass);
REGISTER_STARTUP_CLASS(CaptureManagerClass, CLSID_CaptureManagerClass);
REGISTER_STARTUP_CLASS(ParasiteClass, CLSID_ParasiteClass);
REGISTER_STARTUP_CLASS(BombClass, Startup_CLSID_BombClass);
REGISTER_STARTUP_CLASS(RadSiteClass, CLSID_RadSiteClass);
REGISTER_STARTUP_CLASS(TEventClass, CLSID_TEventClass);
REGISTER_STARTUP_CLASS(VoxelAnimTypeClass, CLSID_VoxelAnimTypeClass);
REGISTER_STARTUP_CLASS(VoxelAnimClass, CLSID_VoxelAnimClass);
REGISTER_STARTUP_CLASS(TActionClass, CLSID_TActionClass);
REGISTER_STARTUP_CLASS(TriggerClass, CLSID_TriggerClass);
REGISTER_STARTUP_CLASS(TriggerTypeClass, CLSID_TriggerTypeClass);
REGISTER_STARTUP_CLASS(ScriptClass, CLSID_ScriptClass);
REGISTER_STARTUP_CLASS(ScriptTypeClass, CLSID_ScriptTypeClass);
REGISTER_STARTUP_CLASS(TagClass, CLSID_TagClass);
REGISTER_STARTUP_CLASS(TagTypeClass, CLSID_TagTypeClass);
REGISTER_STARTUP_CLASS(TeamClass, CLSID_TeamClass);
REGISTER_STARTUP_CLASS(TeamTypeClass, CLSID_TeamTypeClass);
REGISTER_STARTUP_CLASS(TaskForceClass, CLSID_TaskForceClass);
REGISTER_STARTUP_CLASS(UnitTypeClass, Startup_CLSID_UnitTypeClass);
REGISTER_STARTUP_CLASS(BuildingTypeClass, Startup_CLSID_BuildingTypeClass);
REGISTER_STARTUP_CLASS(AircraftTypeClass, Startup_CLSID_AircraftTypeClass);
REGISTER_STARTUP_CLASS(InfantryTypeClass, Startup_CLSID_InfantryTypeClass);
REGISTER_STARTUP_CLASS(BulletTypeClass, CLSID_BulletTypeClass);
REGISTER_STARTUP_CLASS(IsometricTileTypeClass, CLSID_IsometricTileTypeClass);
REGISTER_STARTUP_CLASS(OverlayTypeClass, CLSID_OverlayTypeClass);
REGISTER_STARTUP_CLASS(SmudgeTypeClass, CLSID_SmudgeTypeClass);
REGISTER_STARTUP_CLASS(UnitClass, CLSID_UnitClass);
REGISTER_STARTUP_CLASS(BuildingClass, CLSID_BuildingClass);
REGISTER_STARTUP_CLASS(AircraftClass, CLSID_AircraftClass);
REGISTER_STARTUP_CLASS(InfantryClass, CLSID_InfantryClass);
REGISTER_STARTUP_CLASS(AnimClass, CLSID_AnimClass);
REGISTER_STARTUP_CLASS(AnimTypeClass, CLSID_AnimTypeClass);
REGISTER_STARTUP_CLASS(HouseTypeClass, CLSID_HouseTypeClass);
REGISTER_STARTUP_CLASS(HouseClass, CLSID_HouseClass);
REGISTER_STARTUP_CLASS(DriveLocomotionClass, CLSID_DriveLocomotionClass);
REGISTER_STARTUP_CLASS(JumpjetLocomotionClass, CLSID_JumpjetLocomotionClass);
REGISTER_STARTUP_CLASS(HoverLocomotionClass, CLSID_HoverLocomotionClass);
REGISTER_STARTUP_CLASS(RocketLocomotionClass, CLSID_RocketLocomotionClass);
REGISTER_STARTUP_CLASS(TunnelLocomotionClass, CLSID_TunnelLocomotionClass);
REGISTER_STARTUP_CLASS(WalkLocomotionClass, CLSID_WalkLocomotionClass);
REGISTER_STARTUP_CLASS(DropPodLocomotionClass, CLSID_DropPodLocomotionClass);
REGISTER_STARTUP_CLASS(FlyLocomotionClass, CLSID_FlyLocomotionClass);
REGISTER_STARTUP_CLASS(TeleportLocomotionClass, CLSID_TeleportLocomotionClass);
REGISTER_STARTUP_CLASS(MechLocomotionClass, CLSID_MechLocomotionClass);
REGISTER_STARTUP_CLASS(ShipLocomotionClass, CLSID_ShipLocomotionClass);
REGISTER_STARTUP_CLASS(BulletClass, CLSID_BulletClass);
REGISTER_STARTUP_CLASS(FactoryClass, CLSID_FactoryClass);
REGISTER_STARTUP_CLASS(WarheadTypeClass, CLSID_WarheadTypeClass);
REGISTER_STARTUP_CLASS(WeaponTypeClass, CLSID_WeaponTypeClass);
REGISTER_STARTUP_CLASS(ParticleClass, CLSID_ParticleClass);
REGISTER_STARTUP_CLASS(ParticleTypeClass, CLSID_ParticleTypeClass);
REGISTER_STARTUP_CLASS(ParticleSystemClass, CLSID_ParticleSystemClass);
REGISTER_STARTUP_CLASS(ParticleSystemTypeClass, CLSID_ParticleSystemTypeClass);
REGISTER_STARTUP_CLASS(AITriggerTypeClass, CLSID_AITriggerTypeClass);
REGISTER_STARTUP_CLASS(NeuronClass, CLSID_NeuronClass);
REGISTER_STARTUP_CLASS(FoggedObjectClass, CLSID_FoggedObjectClass);
REGISTER_STARTUP_CLASS(AlphaShapeClass, CLSID_AlphaShapeClass);
#undef REGISTER_STARTUP_CLASS
RegisterActiveObject((IUnknown *)&unknown_application_84c7dc, CLSID_TiberianSun, 1, &unknown_active_object_abd484);
if (failed) {
    Unknown_006FA040(0,
        StringTable::Load_String("TXT_PREPARECOM_FAILED", 0, startup_source_file_7f504c, 0x5ee),
        StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x5ee), 0x30);
}

return failed;
}
extern "C" int __stdcall WinMain(HINSTANCE instance, HINSTANCE previous, char *command_line, int command_show)
{
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.
// Staged WinMain entry setup0068E2A9..0068E95F; not independently accepted.
int argc;
char *argv[20];
char path_to_exe[132];
wchar_t buffer[512];
ProgramInstance = instance;
main_thread_00b25f78 = GetCurrentThreadId();
// Target-verified diagnostic sizeof-report values; class associations remain hypotheses.
Debug_Printf("%d-%d\n", 0x618, 0x98);
Debug_Printf("%d-%d\n", 0x619, 0x4c);
Debug_Printf("%d-%d\n", 0x61a, 0x4c);
Debug_Printf("%d-%d\n", 0x61b, 0x13);
Debug_Printf("%d-%d\n", 0x61c, 0xe18);
Debug_Printf("%d-%d\n", 0x61d, 0x5c0);
Debug_Printf("%d-%d\n", 0x61e, 0xba8);
Debug_Printf("%d-%d\n", 0x61f, 0x110);
Debug_Printf("%d-%d\n", 0x620, 0x1a8);
Debug_Printf("%d-%d\n", 0x621, 0x330);
Debug_Printf("%d-%d\n", 0x622, 0x78);
Debug_Printf("%d-%d\n", 0x623, 0x5e0);
Debug_Printf("%d-%d\n", 0x624, 0x1418);
Debug_Printf("%d-%d\n", 0x625, 0x148);
Debug_Printf("%d-%d\n", 0x626, 0x2c8);
Debug_Printf("%d-%d\n", 0x627, 0x148);
Debug_Printf("%d-%d\n", 0x628, 0x70);
Debug_Printf("%d-%d\n", 0x629, 0x15f90);
Debug_Printf("%d-%d\n", 0x62a, 0x1b0);
Debug_Printf("%d-%d\n", 0x62b, 0x5d8);
Debug_Printf("%d-%d\n", 0x62c, 0xc18);
Debug_Printf("%d-%d\n", 0x62d, 0x18);
Debug_Printf("%d-%d\n", 0x62e, 0x4f3c);
Debug_Printf("%d-%d\n", 0x62f, 0xe18);
Debug_Printf("%d-%d\n", 0x630, 0x9c);
Debug_Printf("%d-%d\n", 0x631, 0x288);
Debug_Printf("%d-%d\n", 0x632, 0x9c);
Debug_Printf("%d-%d\n", 0x633, 0x274);
Debug_Printf("%d-%d\n", 0x634, 0xd4);
Debug_Printf("%d-%d\n", 0x635, 0x9c);
Debug_Printf("%d-%d\n", 0x636, 0xf0);
Debug_Printf("%d-%d\n", 0x637, 0xc8);
Debug_Printf("%d-%d\n", 0x638, 0x28c);
Debug_Printf("%d-%d\n", 0x639, 0x44);
Debug_Printf("%d-%d\n", 0x63a, 0xb4);
Debug_Printf("%d-%d\n", 0x63b, 0xa4);
Debug_Printf("%d-%d\n", 0x63c, 0x38);
Debug_Printf("%d-%d\n", 0x63d, 0x7d0);
Debug_Printf("%d-%d\n", 0x63e, 0xc10);
Debug_Printf("%d-%d\n", 0x63f, 0x2460);
Debug_Printf("%d-%d\n", 0x640, 0xec);
Debug_Printf("%d-%d\n", 0x641, 0x68);
Debug_Printf("%d-%d\n", 0x642, 0x1b0);
Debug_Printf("%d-%d\n", 0x643, 0x14c0);
Debug_Printf("%d-%d\n", 0x644, 0x3c0);
Debug_Printf("%d-%d\n", 0x645, 0x1);
Debug_Printf("%d-%d\n", 0x646, 0x418);
Debug_Printf("%d-%d\n", 0x647, 0xd8);
Debug_Printf("%d-%d\n", 0x648, 0xbc);
Debug_Printf("%d-%d\n", 0x649, 0x8);
Debug_Printf("%d-%d\n", 0x64a, 0x18);
Debug_Printf("%d-%d\n", 0x64b, 0x8);
Debug_Printf("%d-%d\n", 0x64c, 0x8);
Debug_Printf("%d-%d\n", 0x64d, 0x10);
Debug_Printf("%d-%d\n", 0x64e, 0x20);
Debug_Printf("%d-%d\n", 0x64f, 0x18);
if (!CopyProtection_Check_Already_Running_004918f0()) {
    Debug_Printf("Launcher not running....Bail!\n");
    return 0;
}
unknown_handle_abd47c = CreateMutexA(0, 0, "29e3bb2a-2f36-11d3-a72c-0090272fa661");
if (GetLastError() == 183) {
    HWND main_window = FindWindowA("29e3bb2a-2f36-11d3-a72c-0090272fa661", 0);
    if (main_window) {
        SetForegroundWindow(main_window);
        ShowWindow(main_window, 9);
    }
    if (unknown_handle_abd47c) {
        CloseHandle(unknown_handle_abd47c);
        unknown_handle_abd47c = 0;
    }
    Debug_Printf("TibSun is already running...Bail!\n");
    return 0;
} else {
    Debug_Printf("Create AppMutex okay.\n");
    do {
        unknown_handle_abd480 = OpenMutexA(0x1f0001, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
        if (unknown_handle_abd480) {
            Debug_Printf("Waiting for Autoplay to quit!\n");
            if (WaitForSingleObject(unknown_handle_abd480, 30000) == 0xffffffff) {
                Debug_Printf("Failed waiting for AutoPlayMutex\n");
                CloseHandle(unknown_handle_abd480);
                unknown_handle_abd480 = 0;
            }
        }
        if (!unknown_handle_abd480) {
            unknown_handle_abd480 = CreateMutexA(0, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
            if (GetLastError() == 183) {
                CloseHandle(unknown_handle_abd480);
                unknown_handle_abd480 = 0;
                Sleep(2500);
            } else {
                Debug_Printf("Create AutoPlayMutex.\n");
            }
        }
    } while (!unknown_handle_abd480);
    Debug_Printf("Got AutoPlayMutex okay.\n");
}
_controlfp(0x300, 0x300);
set_control_word();
GetModuleFileNameA(instance, path_to_exe, sizeof(path_to_exe));
argc = 1;
argv[0] = path_to_exe;
{
char *token = strtok(command_line, " ");
while (argc < 20 && token) {
    argv[argc++] = strtrim(token);
    token = strtok(0, " ");
}
}
char drive[4];
char path[260];
char dir[260];
_splitpath(argv[0], drive, dir, 0, 0);
_makepath(path, drive, dir, 0, 0);
SetCurrentDirectoryA(path);
int error_code = 1;
// Next: Parse_Command_Line(argc, argv) guards the actual main setup.

if (Parse_Command_Line(argc, argv)) {
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.
// Staged actual WinMain configuration0068E976..0068EC1F; follows successful Parse_Command_Line.
RawFileClass *cfile = new RawFileClass("RA2.INI");
unknown_ini_839740.Load(*cfile, false, false);
unknown_tile_sysmem_7f4cc4 = unknown_ini_839740.Get_Bool("Video", "VideoBackBuffer", true);
unknown_allow_mode_toggle_a40d0b = unknown_ini_839740.Get_Bool("Video", "AllowModeToggle", false);
Options.ScreenWidth = unknown_ini_839740.Get_Int("Video", "ScreenWidth", Options.ScreenWidth);
Options.ScreenHeight = unknown_ini_839740.Get_Int("Video", "ScreenHeight", Options.ScreenHeight);
int socket = unknown_ini_839740.Get_Int("Network", "Socket", 0);
if (socket > 0) {
    socket += 0x4000;
    if (socket >= 0x4000 && socket < 0x8000) unknown_ipx_manager_a40978.Set_Socket((unsigned short)socket);
}
{
char netbuf[512];
memset(netbuf, 0, sizeof(netbuf));
bool has_destination_network = unknown_ini_839740.Get_String("Network", "DestNet", 0, netbuf, sizeof(netbuf)) ? true : false;
// Retain the reference buffer guard: the original tests this address at 68EA94..68EA9C.
if (has_destination_network && netbuf && strlen(netbuf)) {
    NetNumType net;
    NetNodeType node;
    int index = 0;
    char *part = strtok(netbuf, ".");
    while (part) {
        unsigned int value;
        sscanf(part, "%x", &value);
        if (index < 4) net[index] = (unsigned char)value;
        else node[index - 4] = (unsigned char)value;
        ++index;
        part = strtok(0, ".");
    }
    if (index >= 4) {
        unknown_dest_net_set_a3f604 = 1;
        memset(node, 0xff, sizeof(node));
        unknown_dest_net_a3f608 = IPXAddressClass(net, node, 0);
    }
}
}
WWKeyboard = new KeyboardClass;
if (Query_Free_Disk_Kilobytes_00482c80() < 0x2000u) {
    // The original's integer conversion of its free-space constant produces the observed zero argument.
    swprintf(buffer, StringTable::Load_String("TXT_CRITICALLY_LOW", 0, startup_source_file_7f504c, 0x727), 0x2000 / (1024 * 1024));
    int reply = Unknown_006FA040(0, buffer, StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x728), 0x24);
    if (reply == 7) {
        OleUninitialize();
        return 1;
    }
}
OleInitialize(0);

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.
// Staged real WinMain section0068EC1F..0068FF43; no standalone acceptance entry.
// RegisterClasses is inlined by VC6 into this original WinMain extent.
if (RegisterClasses()) exit(1);
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.
// Actual WinMain key/language/mouse/DLL section0068FF43..0069013D.
Debug_Printf("Init Encryption Keys.\n");
Init_Keys();
int required_cd = CD::RequiredCD;
CD::Set_Required_CD(-2);
unknown_language_mix_8374bc = new MixFileClass("LANGUAGE.MIX", &FastKey);
CD::Set_Required_CD(required_cd);
atexit(Prog_End);
Start_Mouse_Thread();
if (!String_Table_Init_006f8e30("ra2.str")) {
    MessageBoxA(0, "Unable to initialize 'ra2.csf', please reinstall Red Alert.\nKeine Initialisierung von 'ra2.csf' m\366glich. Bitte installieren Sie Red Alert erneut.\nInitialisation de 'ra2.csf' impossible. Veuillez r\351installer Red Alert.", "Red Alert 2", 0x10);
    return 0;
}
if (!Init_Language_Resources_00495590(true)) return 0;
if (!GetSystemMetrics(0x13) || !GetSystemMetrics(0x2b)) {
    Unknown_006FA040(0,
        StringTable::Load_String("TXT_MOUSE_REQUIRED", 0, startup_source_file_7f504c, 0x767),
        StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x767), 0x10);
    exit(1);
}
if (Query_DLL_Version_0049ee00("comctl32.dll") < 0x40046u) {
    swprintf(buffer, StringTable::Load_String("TXT_DLL_INVALID", 0, startup_source_file_7f504c, 0x771), "comctl32.dll", 4, 70, "comctl32.dll");
    Unknown_006FA040(0, buffer,
        StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x772), 0x10);
    exit(1);
}
RegisterActiveObject((IUnknown *)&Map, CLSID_TiberianSun, 1, &unknown_active_object_7f4ccc);

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Adapted from TibSun/code/startup.cpp. EA GPLv3 Section 7 additional terms
// and supplemental warranty disclaimers apply; see the reference LICENSE.md.
// Target class identities/order/constructor calls independently verified on RA2.
// Actual WinMain video defaults and mode selection0069013D..006904D5.
int screen_height;
if (Options.ScreenWidth == -1 || (screen_height = Options.ScreenHeight) == -1) {
    Options.ScreenWidth = 640;
    BOOL is_nt4 = WinVersion.IsWinNT && WinVersion.MajorVersion < 5;
    if ((unsigned char)is_nt4 || Get_Video_Memory() > 0x200000u) {
        screen_height = 480;
        Options.ScreenHeight = screen_height;
    } else {
        screen_height = 400;
        Options.ScreenHeight = screen_height;
    }
}
BOOL video_success = 0;
if (unknown_allow_mode_toggle_a40d0b) {
    Create_Main_Window(instance, command_show, 640, 400);
    Prep_Direct_Draw();
    if (unknown_flag_851cb0) {
        unknown_window_839640 = Rect(0, 0, 640, 400);
        unknown_8523e0 = 640;
        unknown_8523e4 = 400;
        direct_draw_depth_008523e8 = unknown_hicolor_7ce008 ? 16 : 8;
    } else {
        unknown_window_839640 = Rect(0, 0, 640, 400);
        video_success = Set_Video_Mode(MainWindow, 640, 400, unknown_hicolor_7ce008 ? 16 : 8);
        if (!video_success) {
            video_success = Set_Video_Mode(MainWindow, 640, 480, unknown_hicolor_7ce008 ? 16 : 8);
            unknown_window_839640 = Rect(0, 0, 640, 480);
            if (!video_success) {
                Release_DirectDraw_00495a60();
                Unknown_006FA040(MainWindow,
                    StringTable::Load_String("TXT_VIDEO_ERROR", 0, startup_source_file_7f504c, 0x801),
                    StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x802), 0x30);
                exit(1);
            }
        }
    }
} else {
    Create_Main_Window(instance, command_show, Options.ScreenWidth, screen_height);
    Prep_Direct_Draw();
    if (unknown_flag_851cb0) {
        unknown_window_839640 = Rect(0, 0, Options.ScreenWidth, Options.ScreenHeight);
        unknown_8523e0 = Options.ScreenWidth;
        unknown_8523e4 = Options.ScreenHeight;
        direct_draw_depth_008523e8 = unknown_hicolor_7ce008 ? 16 : 8;
    } else {
        video_success = Set_Video_Mode(MainWindow, Options.ScreenWidth, Options.ScreenHeight, unknown_hicolor_7ce008 ? 16 : 8);
        unknown_window_839640 = Rect(0, 0, Options.ScreenWidth, Options.ScreenHeight);
        if (!video_success) {
            if (Options.ScreenWidth != 640 || Options.ScreenHeight != 480) {
                Options.ScreenWidth = 640;
                Options.ScreenHeight = 480;
                video_success = Set_Video_Mode(MainWindow, 640, 480, unknown_hicolor_7ce008 ? 16 : 8);
                unknown_window_839640 = Rect(0, 0, Options.ScreenWidth, Options.ScreenHeight);
            }
            if (!video_success) {
                Release_DirectDraw_00495a60();
                Unknown_006FA040(MainWindow,
                    StringTable::Load_String("TXT_VIDEO_ERROR", 0, startup_source_file_7f504c, 0x828),
                    StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x829), 0x30);
                exit(1);
            }
        }
    }
}
if (unknown_record_mode_a41664 && Audio_File_Init(1000, 0x19000)) {
    if (unknown_record_mode_a41664 == 1) Unknown_Audio_File_00522c50("session.ipb");
    else if (unknown_record_mode_a41664 == 2) Audio_File_Open_Stream("session.ipb");
}

unknown_surface_839950 = DSurface::Create_Primary(0);
int visible_bpp = unknown_surface_839950->Bytes_Per_Pixel();
unknown_hicolor_7ce008 = visible_bpp == 2;
if (!unknown_hicolor_7ce008) {
    Unknown_006FA040(MainWindow, StringTable::Load_String("TXT_INVALIDMODE", 0, startup_source_file_7f504c, 0x846), StringTable::Load_String("TXT_SHORT_TITLE", 0, startup_source_file_7f504c, 0x846), 0x30);
    Unknown_00691320();
}
if (!Notify_Launcher_00491930()) {
    Debug_Printf("Failed to notify launcher\012");
    exit(1);
}
do { Windows_Message_Handler(); } while (!unknown_flag_a40d28);
unknown_redraw_8523dc = false;
if (!unknown_flag_851cb0) unknown_surface_839950->Fill(0);
Mono_Window_Init(ProgramInstance, MainWindow, unknown_flag_851cb0);
Planning_Reset();
Rect sidebar(0, 0, unknown_int_7add80, unknown_window_839640.Height);
Rect tile(0, 0, unknown_window_839640.Width - sidebar.Width, sidebar.Height);
Rect composite(0, 0, unknown_window_839640.Width - sidebar.Width, sidebar.Height);
Allocate_Surfaces(&unknown_window_839640, &composite, &tile, &sidebar, false);
if (unknown_flag_852fd7) Init_Direct3D();
unknown_surface_83995c = unknown_surface_839954;
GScreenClass::Blit(true, unknown_surface_839954, 0);
ABufferPtr = new ABuffer(Rect(unknown_rect_839630.X, unknown_rect_839630.Y, 480, 480 - unknown_rect_839630.Y));
ABufferPtr->MaxValue = 0x8000;   // RA2_STARTUP_ABUFFER_SCROLL: TibSun abuffer.h Set_Scroll stores +0x24
ZBufferPtr = new ZBuffer(Rect(unknown_rect_839630.X, unknown_rect_839630.Y, 480, 480 - unknown_rect_839630.Y));
WWMouse = new WWMouseClass(unknown_surface_839950, MainWindow);
WWMouse->Hide_Mouse();
{
    // RA2_STARTUP_CD_ACCESSORS: TibSun playcd.h Get_First_CD_Drive / Get_Next_CD_Drive.
    int first_cd = -1;
    CDList.Cursor = 0;
    if (CDList.Count) {
        if (CDList.Cursor == CDList.Count) CDList.Cursor = 0;
        first_cd = CDList.Drives[CDList.Cursor++];
    }
    CDFileClass::Set_CD_Drive(first_cd);
}
int disk = Unknown_0046E4C0(60);
Get_Performance_Profile();
if (!unknown_special_a40920.IsFromInstall && disk > -1)
    unknown_special_a40920.IsFromInstall = unknown_ini_839740.Get_Bool("Intro", "Play", true);
    WolRegistryOptions registry_options;
    registry_options.Read_004ff200();
    if (!registry_options.unknown_004ff2b0() && !registry_options.unknown_004ff2d0() && !registry_options.unknown_004ff2c0())
        unknown_special_a40920.EscapeMovies = true;


if (unknown_special_a40920.IsFromInstall == true) {
    if (disk > -1) unknown_special_a40920.IsFromInstall = unknown_ini_839740.Put_Bool("Intro", "Play", false);
    cfile->Close();
    cfile->Open();
    unknown_ini_839740.Save(*cfile, false);
}
cfile->Close();
delete cfile;
// NATIVE: no structured exception handling on arm64 macOS; both branches call
// Main_Game (the original's __try/__except(Handle_Exception) wraps the second).
Debug_Printf("Main_Game\012");
Main_Game(argc, argv);
WWMouse->Hide_Mouse();
unknown_surface_839954->Fill(0);
GScreenClass::Blit(true, unknown_surface_839954, 0);
ReadyToQuit = 1;
Theme.Unknown_006E7A70();
Audio_Shutdown();
PostMessageA(MainWindow, 2, 0, 0);
do { Windows_Message_Handler(); } while (ReadyToQuit == 1);
error_code = 0;

}
MultiplayerGameMode::unknown_delete_modes_5b8d40();
RadarEventClass_unknown_delete_all_63b890();
Map.TabClass::unknown_slot50();
Unknown_00603FA0();
Unknown_0059D900();
Unknown_005D5F40();
Unknown_005EAC20();
Unknown_00613E40();
Unknown_Free_All_Art_006F1D20();
Unknown_006F57E0();
Unknown_00535970();
MouseClass::Unknown_0059FEF0();
Shutdown_Network_005bc4d0();
if (unknown_active_object_7f4ccc != (unsigned long)-1) {
    RevokeActiveObject(unknown_active_object_7f4ccc, 0);
    unknown_active_object_7f4ccc = (unsigned long)-1;
}
CoDisconnectObject((IUnknown *)&unknown_application_84c7dc, 0);
OleUninitialize();
return error_code;
}
