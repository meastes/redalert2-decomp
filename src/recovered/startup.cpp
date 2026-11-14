// startup.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/startup.json carries the addresses and the rows that
// stay in their own files.


#define RA2_IUNKNOWN_PTR_LIFETIME  // startup unit (WinMain 0x68E280) uses IUnknownPtr locals; see shared/iunknown_ptr.hpp
// Header-static dynamic initializers of the unit at 0x0068D950 and its
// list globals (unsigned_long); the other drafts of this plan are emitted by this file.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/com_registration.hpp"


// The unit's list of unsigned long (0x00ABD428; name is a hypothesis).
DynamicVectorClass<unsigned long> unknown_unsigned_long_list_00abd428;


// Dynamic initializer (0x0068D970, _$E5) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068D9A0, _$E8) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068D9C0, _$E11) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068D9E0, _$E14) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DA00, _$E17) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DA20, _$E20) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DA40, _$E23) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DA80, _$E26) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DAB0, _$E29) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DAE0, _$E32) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DB10, _$E35) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DB20, _$E38) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// Dynamic initializer (0x0068DB40, _$E43) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// atexit destructor (0x0068DB80, _$E41) emitted by HeaderStatics_empty_rect_init_0068d950.cpp; no source of its own.


// bool __fastcall Set_Class_Root_Key(char const *key, char const *subkey, char const *value) (0x0068DBC0)
// Creates HKEY_CLASSES_ROOT\key (\subkey when given) and sets its default
// value to value when one is given; false when a registry call fails.
// The constant-first tests (`0 != subkey`) show in the copies Register_Com_Server
// inlines: `xor reg, reg; cmp reg, offset string`.
#include "shared/win32_api.hpp"

bool __fastcall Set_Class_Root_Key(char const *key, char const *subkey, char const *value)
{
    HKEY handle;
    char path[260];
    bool result = false;

    lstrcpyA(path, key);
    if (0 != subkey) {
        lstrcatA(path, "\\");
        lstrcatA(path, subkey);
    }
    if (RegCreateKeyExA(HKEY_CLASSES_ROOT, path, 0, 0, 0, KEY_ALL_ACCESS, 0, &handle, 0) == 0) {
        if (0 == value || RegSetValueExA(handle, 0, 0, REG_SZ, (BYTE const *)value, lstrlenA(value) + 1) == 0) {
            result = true;
        }
        RegCloseKey(handle);
    }
    return result;
}


// int __fastcall Register_Com_Server() (0x0068DC70)
// Registers the game's COM server under HKEY_CLASSES_ROOT: the ProgIDs
// WW.TiberianSun.1 and WW.TiberianSun (with CurVer and CLSID), and the class
// key with its ProgIDs, module path and type library; then loads RA2.TLB.
// The first five keys inline Set_Class_Root_Key; the inline budget leaves
// the class key calls out of line.
#include "shared/win32_window.hpp"

static void Guid_To_String(GUID const &guid, char *string, int length)
{
    wchar_t wide[40];

    StringFromGUID2(guid, wide, 39);
    WideCharToMultiByte(0, 0, wide, -1, string, length, 0, 0);
}

int __fastcall Register_Com_Server()
{
    char module_path[260];
    char libid_string[40];
    char class_key[48];
    char clsid_string[40];
    IUnknown *library;

    GetModuleFileNameA(ProgramInstance, module_path, sizeof(module_path));
    Guid_To_String(CLSID_TiberianSun, clsid_string, 39);
    Guid_To_String(LIBID_TiberianSun, libid_string, 39);
    lstrcpyA(class_key, "CLSID\\");
    lstrcatA(class_key, clsid_string);

    Set_Class_Root_Key("WW.TiberianSun.1", 0, "Tiberian Sun");
    Set_Class_Root_Key("WW.TiberianSun.1", "CLSID", clsid_string);
    Set_Class_Root_Key("WW.TiberianSun", 0, "Tiberian Sun");
    Set_Class_Root_Key("WW.TiberianSun", "CurVer", "WW.TiberianSun.1");
    Set_Class_Root_Key("WW.TiberianSun", "CLSID", clsid_string);
    Set_Class_Root_Key(class_key, 0, "Tiberian Sun");
    Set_Class_Root_Key(class_key, "ProgID", "WW.TiberianSun.1");
    Set_Class_Root_Key(class_key, "VersionIndependentProgID", "WW.TiberianSun");
    Set_Class_Root_Key(class_key, "NotInsertable", 0);
    Set_Class_Root_Key(class_key, "InprocServer32", module_path);
    Set_Class_Root_Key(class_key, "Typelib", libid_string);

    library = 0;
    LoadTypeLib(L"RA2.TLB", &library);
    if (library != 0) {
        library->Release();
    }
    return 0;
}


// int __fastcall Unregister_Com_Server() (0x0068E040)
// Deletes the keys Register_Com_Server writes: the two ProgIDs (subkeys
// first), the class key's subkeys and the class key itself.

int __fastcall Unregister_Com_Server()
{
    char path[300];
    wchar_t wide[40];
    char clsid_string[40];
    char class_key[72];

    StringFromGUID2(CLSID_TiberianSun, wide, 39);
    WideCharToMultiByte(0, 0x20, wide, -1, clsid_string, 39, 0, 0);
    lstrcpyA(class_key, "CLSID\\");
    lstrcatA(class_key, clsid_string);

    RegDeleteKeyA(HKEY_CLASSES_ROOT, "WW.TiberianSun\\CurVer");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "WW.TiberianSun\\CLSID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "WW.TiberianSun");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "WW.TiberianSun.1\\CLSID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "WW.TiberianSun.1");
    wsprintfA(path, "%s\\%s", class_key, "ProgID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, path);
    wsprintfA(path, "%s\\%s", class_key, "VersionIndependentProgID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, path);
    wsprintfA(path, "%s\\%s", class_key, "NotInsertable");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, path);
    wsprintfA(path, "%s\\%s", class_key, "InprocServer32");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, path);
    wsprintfA(path, "%s\\%s", class_key, "Typelib");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, path);
    RegDeleteKeyA(HKEY_CLASSES_ROOT, class_key);
    return 0;
}


// int __stdcall WinMain(HINSTANCE instance, HINSTANCE, char *command_line, int command_show) (0x0068E280)
// Scout1146 (Scout1144 r1 + TibSun REGISTER_CLASS block; lineage Startup226/232, Scout1130-1144). Flags: see the round README.
#include "shared/iunknown_ptr.hpp"
#include "shared/debug_printf.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/ccini_class.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/ipx_address.hpp"
#include "shared/keyboard.hpp"
#include "shared/string_table.hpp"
#include "shared/crt_string.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/ini_class.hpp"

extern unsigned long main_thread_00b25f78;
struct _EXCEPTION_POINTERS;
int __fastcall Main_Game(int argc, char *argv[]);
int __fastcall Handle_Exception(unsigned code, _EXCEPTION_POINTERS *info);
extern "C" unsigned long __cdecl _exception_code();
extern "C" void *__cdecl _exception_info();
bool CopyProtection_Check_Already_Running_004918f0();
unsigned Query_Free_Disk_Kilobytes_00482c80();
int __fastcall Wide_Message_Box(HWND window, wchar_t const *text, wchar_t const *title, unsigned style);
bool __fastcall Parse_Command_Line(int argc, char *argv[]);
void Set_FPU_Control(unsigned value, unsigned mask);
void Unknown_0077F0F4();
extern "C" bool video_back_buffer_7f4cc4;
extern int unknown_screen_width_a40b38;
extern int unknown_screen_height_a40b3c;
extern bool unknown_allow_mode_toggle_a40d0b;
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
extern "C" __declspec(dllimport) HWND __stdcall FindWindowA(char const *class_name, char const *window_name);
extern "C" __declspec(dllimport) int __stdcall SetForegroundWindow(HWND window);
extern "C" __declspec(dllimport) HANDLE __stdcall OpenMutexA(unsigned long access, int inherit, char const *name);
extern "C" __declspec(dllimport) int __stdcall SetCurrentDirectoryA(char const *path);
extern "C" __declspec(dllimport) long __stdcall OleInitialize(void *reserved);
extern "C" __declspec(dllimport) void __stdcall OleUninitialize();

#include "shared/com_api.hpp"
#include "shared/class_factory.hpp"
#include "shared/cd.hpp"
#include "shared/ram_file_class.hpp"
#include "shared/wwmouse.hpp"
#include "shared/exit.hpp"
#include "shared/pkey.hpp"
class MouseClass;
extern MouseClass Map;
extern "C" void __cdecl exit(int code);
extern "C" int __cdecl atexit(void (__cdecl *function)());
int __fastcall Load_String_File(char const *name);
bool __fastcall Unknown_00495590(bool flag);
unsigned __fastcall Query_DLL_Version(char const *name);
extern "C" char const Keys[];
extern "C" char unknown_string_7f4ea8[];
extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int index);
extern "C" __declspec(dllimport) HRESULT __stdcall CoRegisterClassObject(GUID const &clsid, IUnknown *object, unsigned long context, unsigned long flags, unsigned long *cookie);
extern "C" __declspec(dllimport) HRESULT __stdcall RegisterActiveObject(IUnknown *object, GUID const &clsid, unsigned long flags, unsigned long *cookie);
extern "C" __declspec(dllimport) HINSTANCE __stdcall LoadLibraryA(char const *name);
typedef int (__stdcall *DllProc)();
extern "C" __declspec(dllimport) DllProc __stdcall GetProcAddress(HINSTANCE module, char const *name);
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(HINSTANCE module);


struct RegisterDLLType {
    GUID const *clsid;
    char const *name;
};
extern "C" RegisterDLLType const register_dlls_7ab988[1];

// generated from Startup232 out/regs.txt (registration order)
class CStreamClass; extern "C" GUID const CLSID_CompressStream; // @ref _CLSID_CompressStream=007a1ab0
class WaveClass; extern "C" GUID const CLSID_WaveClass; // @ref _CLSID_WaveClass=007a1c90
class TerrainTypeClass; extern "C" GUID const CLSID_TerrainTypeClass; // @ref _CLSID_TerrainTypeClass=007a1b50
class TerrainClass; extern "C" GUID const CLSID_TerrainClass; // @ref _CLSID_TerrainClass=007a1cb0
class SuperWeaponTypeClass; extern "C" GUID const CLSID_SuperWeaponTypeClass; // @ref _CLSID_SuperWeaponTypeClass=007a1ae0
class SuperClass; extern "C" GUID const CLSID_SuperClass; // @ref _CLSID_SuperClass=007a1af0
class Tactical; extern "C" GUID const CLSID_TacticalMapClass; // @ref _CLSID_TacticalMapClass=007a1e90
class CellClass; extern "C" GUID const CLSID_CellClass; // @ref _CLSID_CellClass=007a1e70
class EMPulseClass; extern "C" GUID const CLSID_EMPulseClass; // @ref _CLSID_EMPulseClass=007a1e80
class LightSourceClass; extern "C" GUID const CLSID_LightSource; // @ref _CLSID_LightSource=007a1e30
class SideClass; extern "C" GUID const CLSID_SideClass; // @ref _CLSID_SideClass=007a1e50
class TiberiumClass; extern "C" GUID const CLSID_TiberiumClass; // @ref _CLSID_TiberiumClass=007a1e60
class TubeClass; extern "C" GUID const CLSID_TubeClass; // @ref _CLSID_TubeClass=007a1cc0
class CampaignClass; extern "C" GUID const CLSID_CampaignClass; // @ref _CLSID_CampaignClass=007a1e40
class BuildingLightClass; extern "C" GUID const CLSID_BuildingLightClass; // @ref _CLSID_BuildingLightClass=007a1ca0
class WaypointPathClass; extern "C" GUID const CLSID_WaypointPath; // @ref _CLSID_WaypointPath=007a1dc0
class TemporalClass; extern "C" GUID const CLSID_TemporalClass; // @ref _CLSID_TemporalClass=007a1e20
class SpawnManagerClass; extern "C" GUID const CLSID_SpawnManagerClass; // @ref _CLSID_SpawnManagerClass=007a1dd0
class CaptureManagerClass; extern "C" GUID const CLSID_CaptureManagerClass; // @ref _CLSID_CaptureManagerClass=007a1de0
class ParasiteClass; extern "C" GUID const CLSID_ParasiteClass; // @ref _CLSID_ParasiteClass=007a1df0
class BombClass; extern "C" GUID const CLSID_BombClass; // @ref _CLSID_BombClass=007a1e00
class RadSiteClass; extern "C" GUID const CLSID_RadSiteClass; // @ref _CLSID_RadSiteClass=007a1e10
class TEventClass; extern "C" GUID const CLSID_TEventClass; // @ref _CLSID_TEventClass=007a1d80
class VoxelAnimTypeClass; extern "C" GUID const CLSID_VoxelAnimTypeClass; // @ref _CLSID_VoxelAnimTypeClass=007a1d00
class VoxelAnimClass; extern "C" GUID const CLSID_VoxelAnimClass; // @ref _CLSID_VoxelAnimClass=007a1bc0
class TActionClass; extern "C" GUID const CLSID_TActionClass; // @ref _CLSID_TActionClass=007a1d70
class TriggerClass; extern "C" GUID const CLSID_TriggerClass; // @ref _CLSID_TriggerClass=007a1d50
class TriggerTypeClass; extern "C" GUID const CLSID_TriggerTypeClass; // @ref _CLSID_TriggerTypeClass=007a1d60
class ScriptClass; extern "C" GUID const CLSID_ScriptClass; // @ref _CLSID_ScriptClass=007a1d10
class ScriptTypeClass; extern "C" GUID const CLSID_ScriptTypeClass; // @ref _CLSID_ScriptTypeClass=007a1d20
class TagClass; extern "C" GUID const CLSID_TagClass; // @ref _CLSID_TagClass=007a1d30
class TagTypeClass; extern "C" GUID const CLSID_TagTypeClass; // @ref _CLSID_TagTypeClass=007a1d40
class TeamClass; extern "C" GUID const CLSID_TeamClass; // @ref _CLSID_TeamClass=007a1cd0
class TeamTypeClass; extern "C" GUID const CLSID_TeamTypeClass; // @ref _CLSID_TeamTypeClass=007a1cf0
class TaskForceClass; extern "C" GUID const CLSID_TaskForceClass; // @ref _CLSID_TaskForceClass=007a1ce0
class UnitTypeClass; extern "C" GUID const CLSID_UnitTypeClass; // @ref _CLSID_UnitTypeClass=007a1b00
class BuildingTypeClass; extern "C" GUID const CLSID_BuildingTypeClass; // @ref _CLSID_BuildingTypeClass=007a1b30
class AircraftTypeClass; extern "C" GUID const CLSID_AircraftTypeClass; // @ref _CLSID_AircraftTypeClass=007a1b20
class InfantryTypeClass; extern "C" GUID const CLSID_InfantryTypeClass; // @ref _CLSID_InfantryTypeClass=007a1b10
class BulletTypeClass; extern "C" GUID const CLSID_BulletTypeClass; // @ref _CLSID_BulletTypeClass=007a1b40
class IsometricTileTypeClass; extern "C" GUID const CLSID_IsometricTileTypeClass; // @ref _CLSID_IsometricTileTypeClass=007a1b60
class OverlayTypeClass; extern "C" GUID const CLSID_OverlayTypeClass; // @ref _CLSID_OverlayTypeClass=007a1b70
class SmudgeTypeClass; extern "C" GUID const CLSID_SmudgeTypeClass; // @ref _CLSID_SmudgeTypeClass=007a1b80
class UnitClass; extern "C" GUID const CLSID_UnitClass; // @ref _CLSID_UnitClass=007a1c60
class BuildingClass; extern "C" GUID const CLSID_BuildingClass; // @ref _CLSID_BuildingClass=007a1c10
class AircraftClass; extern "C" GUID const CLSID_AircraftClass; // @ref _CLSID_AircraftClass=007a1bd0
class InfantryClass; extern "C" GUID const CLSID_InfantryClass; // @ref _CLSID_InfantryClass=007a1bf0
class AnimClass; extern "C" GUID const CLSID_AnimClass; // @ref _CLSID_AnimClass=007a1be0
class AnimTypeClass; extern "C" GUID const CLSID_AnimTypeClass; // @ref _CLSID_AnimTypeClass=007a1b90
class HouseTypeClass; extern "C" GUID const CLSID_HouseTypeClass; // @ref _CLSID_HouseTypeClass=007a1ba0
class HouseClass; extern "C" GUID const CLSID_HouseClass; // @ref _CLSID_HouseClass=007a1ad0
class DriveLocomotionClass; extern "C" GUID const CLSID_DriveLocomotionClass; // @ref _CLSID_DriveLocomotionClass=007a1f70
class JumpjetLocomotionClass; extern "C" GUID const CLSID_JumpjetLocomotionClass; // @ref _CLSID_JumpjetLocomotionClass=007a2000
class HoverLocomotionClass; extern "C" GUID const CLSID_HoverLocomotionClass; // @ref _CLSID_HoverLocomotionClass=007a1f80
class RocketLocomotionClass; extern "C" GUID const CLSID_RocketLocomotionClass; // @ref _CLSID_RocketLocomotionClass=007a2010
class TunnelLocomotionClass; extern "C" GUID const CLSID_TunnelLocomotionClass; // @ref _CLSID_TunnelLocomotionClass=007a1f90
class WalkLocomotionClass; extern "C" GUID const CLSID_WalkLocomotionClass; // @ref _CLSID_WalkLocomotionClass=007a1fa0
class DropPodLocomotionClass; extern "C" GUID const CLSID_DropPodLocomotionClass; // @ref _CLSID_DropPodLocomotionClass=007a1fb0
class FlyLocomotionClass; extern "C" GUID const CLSID_FlyLocomotionClass; // @ref _CLSID_FlyLocomotionClass=007a1fc0
class TeleportLocomotionClass; extern "C" GUID const CLSID_TeleportLocomotion; // @ref _CLSID_TeleportLocomotion=007a1fd0
class MechLocomotionClass; extern "C" GUID const CLSID_MechLocomotionClass; // @ref _CLSID_MechLocomotionClass=007a1fe0
class ShipLocomotionClass; extern "C" GUID const CLSID_ShipLocomotionClass; // @ref _CLSID_ShipLocomotionClass=007a1ff0
class BulletClass; extern "C" GUID const CLSID_BulletClass; // @ref _CLSID_BulletClass=007a1c50
class FactoryClass; extern "C" GUID const CLSID_FactoryClass; // @ref _CLSID_FactoryClass=007a1d90
class WarheadTypeClass; extern "C" GUID const CLSID_WarheadTypeClass; // @ref _CLSID_WarheadTypeClass=007a1db0
class WeaponTypeClass; extern "C" GUID const CLSID_WeaponTypeClass; // @ref _CLSID_WeaponTypeClass=007a1da0
class ParticleClass; extern "C" GUID const CLSID_ParticleClass; // @ref _CLSID_ParticleClass=007a1c70
class ParticleTypeClass; extern "C" GUID const CLSID_ParticleTypeClass; // @ref _CLSID_ParticleTypeClass=007a1c80
class ParticleSystemClass; extern "C" GUID const CLSID_ParticleSystemClass; // @ref _CLSID_ParticleSystemClass=007a1c30
class ParticleSystemTypeClass; extern "C" GUID const CLSID_ParticleSystemTypeClass; // @ref _CLSID_ParticleSystemTypeClass=007a1c40
class AITriggerTypeClass; extern "C" GUID const CLSID_AITriggerTypeClass; // @ref _CLSID_AITriggerTypeClass=007a1ea0
class NeuronClass; extern "C" GUID const CLSID_NeuronClass; // @ref _CLSID_NeuronClass=007a1ec0
class FoggedObjectClass; extern "C" GUID const CLSID_FoggedObjectClass; // @ref _CLSID_FoggedObjectClass=007a1ed0
class AlphaShapeClass; extern "C" GUID const CLSID_AlphaShapeClass; // @ref _CLSID_AlphaShapeClass=007a1ee0
#include "shared/windows_version.hpp"
#include "shared/win_stub.hpp"
#include "shared/main_opt.hpp"
#include "shared/dsurface.hpp"
#include "shared/mono_window.hpp"
#include "shared/gscreen.hpp"
#include "shared/blit.hpp"
#include "shared/mouse.hpp"
#include "shared/cd_file_class.hpp"
#include "shared/get_cd.hpp"
#include "shared/special.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/theme.hpp"
#include "shared/startup_exceptions.hpp"
#include "shared/mouse_class.hpp"
#include "shared/multiplayer_game_mode.hpp"
extern bool unknown_hicolor_7ce008;
extern int unknown_depth_8523e8;
extern int unknown_audio_mode_a41664;
extern bool unknown_no_exception_handler_a4168c;
extern bool unknown_redraw_8523dc;
unsigned long Get_Video_Memory();
void Prep_Direct_Draw();
void Release_DirectDraw_00495a60();
int __fastcall Audio_File_Init(int count, int size);
void __fastcall Audio_File_Record(char const *name);
void __fastcall Audio_File_Open_Stream(char const *name);
bool Notify_Launcher_00491930();
void Windows_Message_Handler();
void Planning_Reset();
long Init_Direct3D();
int __fastcall Get_Performance_Profile();
void Audio_Shutdown();
void RadarEventClass_unknown_delete_all_63b890();
void Unknown_005D5F40();
void Unknown_005EAC20();
void __fastcall Unknown_Free_All_Art_006F1D20();
void Shutdown_Network_005bc4d0();
#define REGISTER_LIST \
    REGISTER_CLASS(CStreamClass, CLSID_CompressStream) \
    REGISTER_CLASS(WaveClass, CLSID_WaveClass) \
    REGISTER_CLASS(TerrainTypeClass, CLSID_TerrainTypeClass) \
    REGISTER_CLASS(TerrainClass, CLSID_TerrainClass) \
    REGISTER_CLASS(SuperWeaponTypeClass, CLSID_SuperWeaponTypeClass) \
    REGISTER_CLASS(SuperClass, CLSID_SuperClass) \
    REGISTER_CLASS(Tactical, CLSID_TacticalMapClass) \
    REGISTER_CLASS(CellClass, CLSID_CellClass) \
    REGISTER_CLASS(EMPulseClass, CLSID_EMPulseClass) \
    REGISTER_CLASS(LightSourceClass, CLSID_LightSource) \
    REGISTER_CLASS(SideClass, CLSID_SideClass) \
    REGISTER_CLASS(TiberiumClass, CLSID_TiberiumClass) \
    REGISTER_CLASS(TubeClass, CLSID_TubeClass) \
    REGISTER_CLASS(CampaignClass, CLSID_CampaignClass) \
    REGISTER_CLASS(BuildingLightClass, CLSID_BuildingLightClass) \
    REGISTER_CLASS(WaypointPathClass, CLSID_WaypointPath) \
    REGISTER_CLASS(TemporalClass, CLSID_TemporalClass) \
    REGISTER_CLASS(SpawnManagerClass, CLSID_SpawnManagerClass) \
    REGISTER_CLASS(CaptureManagerClass, CLSID_CaptureManagerClass) \
    REGISTER_CLASS(ParasiteClass, CLSID_ParasiteClass) \
    REGISTER_CLASS(BombClass, CLSID_BombClass) \
    REGISTER_CLASS(RadSiteClass, CLSID_RadSiteClass) \
    REGISTER_CLASS(TEventClass, CLSID_TEventClass) \
    REGISTER_CLASS(VoxelAnimTypeClass, CLSID_VoxelAnimTypeClass) \
    REGISTER_CLASS(VoxelAnimClass, CLSID_VoxelAnimClass) \
    REGISTER_CLASS(TActionClass, CLSID_TActionClass) \
    REGISTER_CLASS(TriggerClass, CLSID_TriggerClass) \
    REGISTER_CLASS(TriggerTypeClass, CLSID_TriggerTypeClass) \
    REGISTER_CLASS(ScriptClass, CLSID_ScriptClass) \
    REGISTER_CLASS(ScriptTypeClass, CLSID_ScriptTypeClass) \
    REGISTER_CLASS(TagClass, CLSID_TagClass) \
    REGISTER_CLASS(TagTypeClass, CLSID_TagTypeClass) \
    REGISTER_CLASS(TeamClass, CLSID_TeamClass) \
    REGISTER_CLASS(TeamTypeClass, CLSID_TeamTypeClass) \
    REGISTER_CLASS(TaskForceClass, CLSID_TaskForceClass) \
    REGISTER_CLASS(UnitTypeClass, CLSID_UnitTypeClass) \
    REGISTER_CLASS(BuildingTypeClass, CLSID_BuildingTypeClass) \
    REGISTER_CLASS(AircraftTypeClass, CLSID_AircraftTypeClass) \
    REGISTER_CLASS(InfantryTypeClass, CLSID_InfantryTypeClass) \
    REGISTER_CLASS(BulletTypeClass, CLSID_BulletTypeClass) \
    REGISTER_CLASS(IsometricTileTypeClass, CLSID_IsometricTileTypeClass) \
    REGISTER_CLASS(OverlayTypeClass, CLSID_OverlayTypeClass) \
    REGISTER_CLASS(SmudgeTypeClass, CLSID_SmudgeTypeClass) \
    REGISTER_CLASS(UnitClass, CLSID_UnitClass) \
    REGISTER_CLASS(BuildingClass, CLSID_BuildingClass) \
    REGISTER_CLASS(AircraftClass, CLSID_AircraftClass) \
    REGISTER_CLASS(InfantryClass, CLSID_InfantryClass) \
    REGISTER_CLASS(AnimClass, CLSID_AnimClass) \
    REGISTER_CLASS(AnimTypeClass, CLSID_AnimTypeClass) \
    REGISTER_CLASS(HouseTypeClass, CLSID_HouseTypeClass) \
    REGISTER_CLASS(HouseClass, CLSID_HouseClass) \
    REGISTER_CLASS(DriveLocomotionClass, CLSID_DriveLocomotionClass) \
    REGISTER_CLASS(JumpjetLocomotionClass, CLSID_JumpjetLocomotionClass) \
    REGISTER_CLASS(HoverLocomotionClass, CLSID_HoverLocomotionClass) \
    REGISTER_CLASS(RocketLocomotionClass, CLSID_RocketLocomotionClass) \
    REGISTER_CLASS(TunnelLocomotionClass, CLSID_TunnelLocomotionClass) \
    REGISTER_CLASS(WalkLocomotionClass, CLSID_WalkLocomotionClass) \
    REGISTER_CLASS(DropPodLocomotionClass, CLSID_DropPodLocomotionClass) \
    REGISTER_CLASS(FlyLocomotionClass, CLSID_FlyLocomotionClass) \
    REGISTER_CLASS(TeleportLocomotionClass, CLSID_TeleportLocomotion) \
    REGISTER_CLASS(MechLocomotionClass, CLSID_MechLocomotionClass) \
    REGISTER_CLASS(ShipLocomotionClass, CLSID_ShipLocomotionClass) \
    REGISTER_CLASS(BulletClass, CLSID_BulletClass) \
    REGISTER_CLASS(FactoryClass, CLSID_FactoryClass) \
    REGISTER_CLASS(WarheadTypeClass, CLSID_WarheadTypeClass) \
    REGISTER_CLASS(WeaponTypeClass, CLSID_WeaponTypeClass) \
    REGISTER_CLASS(ParticleClass, CLSID_ParticleClass) \
    REGISTER_CLASS(ParticleTypeClass, CLSID_ParticleTypeClass) \
    REGISTER_CLASS(ParticleSystemClass, CLSID_ParticleSystemClass) \
    REGISTER_CLASS(ParticleSystemTypeClass, CLSID_ParticleSystemTypeClass) \
    REGISTER_CLASS(AITriggerTypeClass, CLSID_AITriggerTypeClass) \
    REGISTER_CLASS(NeuronClass, CLSID_NeuronClass) \
    REGISTER_CLASS(FoggedObjectClass, CLSID_FoggedObjectClass) \
    REGISTER_CLASS(AlphaShapeClass, CLSID_AlphaShapeClass) \



// TibSun startup.cpp: each registration is its own block (the block's IL keeps INIClass() 0x51AD50
// out of line inside Init_Keys).
#define REGISTER_CLASS(_class, _clsid) \
    { \
        factory = new TClassFactory<_class>; \
        CoRegisterClassObject(_clsid, factory, 1, 1, &dwRegister); \
        unknown_unsigned_long_list_00abd428.Add(dwRegister); \
    }

class TiberianSunClassFactory;
extern TiberianSunClassFactory TibSunFactory;

struct KeyboardClass : public WWKeyboardClass {
    KeyboardClass() : IsLibrary(true) {}
    unsigned IsLibrary;
};
#define INIT_FREE_DISK_SPACE 0x2000

inline bool RegisterClasses()
{
    bool failed = false;
    for (int i = 0; i < 1; i++) {
        IUnknownPtr ptr;
        HRESULT result = ptr.CreateInstance(*register_dlls_7ab988[i].clsid, 0, 7);
        failed = FAILED(result);
        if (failed) {
            failed = false;
            HINSTANCE hModule = LoadLibraryA(register_dlls_7ab988[i].name);
            if (hModule != 0) {
                DllProc fprocDllReg = GetProcAddress(hModule, "DllRegisterServer");
                if (!fprocDllReg || (fprocDllReg(), FAILED(ptr.CreateInstance(*register_dlls_7ab988[i].clsid, 0, 7)))) {
                    failed = true;
                }
                FreeLibrary(hModule);
            } else {
                failed = true;
            }
        }
        if (failed) {
            break;
        }
        ptr.Release();
    }

    Register_Com_Server();

    unsigned long dwRegister;
    IClassFactory *factory;

    CoRegisterClassObject(CLSID_TiberianSun, (IUnknown *)&TibSunFactory, 1, 1, &dwRegister);
    unknown_unsigned_long_list_00abd428.Add(dwRegister);
    REGISTER_LIST

    RegisterActiveObject((IUnknown *)&unknown_application_84c7dc, CLSID_TiberianSun, 1, &unknown_active_object_abd484);
    if (failed) {
        Wide_Message_Box(0, StringTable::Load_String("TXT_PREPARECOM_FAILED", 0, "D:\\Ra2\\Startup.CPP", 0x5ee),
                         StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x5ee), 0x30);
    }
    return failed;
}

inline void Init_Keys()
{
    Debug_Printf("Init_Keys - declarations\n");
    RAMFileClass file((void *)Keys, strlen(Keys));
    INIClass ini;
    Debug_Printf("Init_Keys - Load\n");
    ini.Load(file, false);
    Debug_Printf("Init_Keys - Init fast key\n");
    FastKey = ini.Get_PKey(true);
}

inline void Init_Mouse()
{
    WWMouse = new WWMouseClass(unknown_surface_839950, MainWindow);
}

inline void Init_Language()
{
    unknown_language_mix_8374bc = new MixFileClass("LANGUAGE.MIX", &FastKey);
}

inline void Read_Destination_Network()
{
    char netbuf[512];
    memset(netbuf, 0, sizeof(netbuf));
    char *net_ptr = netbuf;
    bool found = unknown_ini_839740.Get_String("Network", "DestNet", 0, netbuf, sizeof(netbuf));

    if (found && net_ptr != 0 && strlen(netbuf)) {
        NetNumType net;
        NetNodeType node;

        int i = 0;
        char *p = strtok(netbuf, ".");
        int x;
        while (p != 0) {
            sscanf(p, "%x", &x);
            if (i < 4) {
                net[i] = (char)x;
            } else {
                node[i - 4] = (char)x;
            }
            i++;
            p = strtok(0, ".");
        }

        if (i >= 4) {
            unknown_dest_net_set_a3f604 = 1;
            memset(node, 0xFF, 6);
            unknown_dest_net_a3f608 = IPXAddressClass(net, node, 0);
        }
    }
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE, char *command_line, int command_show)
{
    int argc;
    char *argv[20];
    char path_to_exe[132];
    wchar_t buffer[512];

    ProgramInstance = instance;
    main_thread_00b25f78 = GetCurrentThreadId();
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
        HWND window = FindWindowA("29e3bb2a-2f36-11d3-a72c-0090272fa661", 0);
        if (window != 0) {
            SetForegroundWindow(window);
            ShowWindow(window, 9);
        }
        if (unknown_handle_abd47c != 0) {
            CloseHandle(unknown_handle_abd47c);
            unknown_handle_abd47c = 0;
        }
        Debug_Printf("TibSun is already running...Bail!\n");
        return 0;
    }
    Debug_Printf("Create AppMutex okay.\n");

    do {
        unknown_handle_abd480 = OpenMutexA(0x1F0001, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
        if (unknown_handle_abd480 != 0) {
            Debug_Printf("Waiting for Autoplay to quit!\n");
            if (WaitForSingleObject(unknown_handle_abd480, 30000) == -1) {
                Debug_Printf("Failed waiting for AutoPlayMutex\n");
                CloseHandle(unknown_handle_abd480);
                unknown_handle_abd480 = 0;
            }
        }
        if (unknown_handle_abd480 == 0) {
            unknown_handle_abd480 = CreateMutexA(0, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
            if (GetLastError() == 183) {
                CloseHandle(unknown_handle_abd480);
                unknown_handle_abd480 = 0;
                Sleep(2500);
            } else {
                Debug_Printf("Create AutoPlayMutex.\n");
            }
        }
    } while (unknown_handle_abd480 == 0);
    Debug_Printf("Got AutoPlayMutex okay.\n");

    Set_FPU_Control(0x300, 0x300);
    Unknown_0077F0F4();

    GetModuleFileNameA(instance, path_to_exe, sizeof(path_to_exe));
    argc = 1;
    argv[0] = path_to_exe;
    char *token = strtok(command_line, " ");
    while (argc < 20 && token != 0) {
        argv[argc++] = strtrim(token);
        token = strtok(0, " ");
    }
    char drive[3];
    char path[260];
    char dir[256];
    _splitpath(argv[0], drive, dir, 0, 0);
    _makepath(path, drive, dir, 0, 0);
    SetCurrentDirectoryA(path);

    int retval = 1;
    if (Parse_Command_Line(argc, argv)) {
        RawFileClass *cfile = new RawFileClass("RA2.INI");
        unknown_ini_839740.Load(*cfile, false, false);
        video_back_buffer_7f4cc4 = unknown_ini_839740.Get_Bool("Video", "VideoBackBuffer", true);
        unknown_allow_mode_toggle_a40d0b = unknown_ini_839740.Get_Bool("Video", "AllowModeToggle", false);
        unknown_screen_width_a40b38 = unknown_ini_839740.Get_Int("Video", "ScreenWidth", unknown_screen_width_a40b38);
        unknown_screen_height_a40b3c = unknown_ini_839740.Get_Int("Video", "ScreenHeight", unknown_screen_height_a40b3c);
        int socket = unknown_ini_839740.Get_Int("Network", "Socket", 0);
        if (socket > 0) {
            socket += 0x4000;
            if (socket >= 0x4000 && socket < 0x8000) {
                unknown_ipx_manager_a40978.Set_Socket((unsigned short)socket);
            }
        }
        Read_Destination_Network();

        WWKeyboard = new KeyboardClass();

        if (Query_Free_Disk_Kilobytes_00482c80() < INIT_FREE_DISK_SPACE) {
            swprintf(buffer, StringTable::Load_String("TXT_CRITICALLY_LOW", 0, "D:\\Ra2\\Startup.CPP", 0x727), INIT_FREE_DISK_SPACE / (1024 * 1024));
            int reply = Wide_Message_Box(0, buffer, StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x728), 0x24);
            if (reply == 7) {
                OleUninitialize();
                return 1;
            }
        }
        OleInitialize(0);
        if (RegisterClasses()) {
            exit(1);
        }

        Debug_Printf("Init Encryption Keys.\n");
        Init_Keys();

        int cur_cd = CD::Get_Required_CD();
        CD::Set_Required_CD(-2);
        Init_Language();
        CD::Set_Required_CD(cur_cd);
        atexit(Prog_End);
        Start_Mouse_Thread();

        if (!Load_String_File("ra2.str")) {
            MessageBoxA(0, unknown_string_7f4ea8, "Red Alert 2", 0x10);
            return 0;
        }
        if (!Unknown_00495590(true)) {
            return 0;
        }
        if (GetSystemMetrics(0x13) == 0 || GetSystemMetrics(0x2B) == 0) {
            Wide_Message_Box(0, StringTable::Load_String("TXT_MOUSE_REQUIRED", 0, "D:\\Ra2\\Startup.CPP", 0x767),
                             StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x767), 0x10);
            exit(1);
        }
        if (Query_DLL_Version("comctl32.dll") < 0x40046) {
            swprintf(buffer, StringTable::Load_String("TXT_DLL_INVALID", 0, "D:\\Ra2\\Startup.CPP", 0x771), "comctl32.dll", 4, 0x46, "comctl32.dll");
            Wide_Message_Box(0, buffer, StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x772), 0x10);
            exit(1);
        }
        RegisterActiveObject((IUnknown *)&Map, CLSID_TiberianSun, 1, &unknown_active_object_7f4ccc);

        if (unknown_screen_width_a40b38 == -1 || unknown_screen_height_a40b3c == -1) {
            unknown_screen_width_a40b38 = 640;
            if (WinVersion.Is_WinNTx() || Get_Video_Memory() > 0x200000) {
                unknown_screen_height_a40b3c = 480;
            } else {
                unknown_screen_height_a40b3c = 400;
            }
        }

        BOOL video_success = FALSE;
        if (unknown_allow_mode_toggle_a40d0b) {
            Create_Main_Window(instance, command_show, 640, 400);
            Prep_Direct_Draw();
            if (unknown_flag_851cb0) {
                Rect rect1(0, 0, 640, 400);
                unknown_window_839640 = rect1;
                unknown_8523e0 = 640;
                unknown_8523e4 = 400;
                unknown_depth_8523e8 = unknown_hicolor_7ce008 ? 16 : 8;
            } else {
                Rect rect2(0, 0, 640, 400);
                unknown_window_839640 = rect2;
                video_success = Set_Video_Mode(MainWindow, 640, 400, unknown_hicolor_7ce008 ? 16 : 8);
                if (!video_success) {
                    video_success = Set_Video_Mode(MainWindow, 640, 480, unknown_hicolor_7ce008 ? 16 : 8);
                    Rect rect3(0, 0, 640, 480);
                    unknown_window_839640 = rect3;
                    if (!video_success) {
                        Release_DirectDraw_00495a60();
                        Wide_Message_Box(MainWindow, StringTable::Load_String("TXT_VIDEO_ERROR", 0, "D:\\Ra2\\Startup.CPP", 0x801),
                                         StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x802), 0x30);
                        exit(1);
                    }
                }
            }
        } else {
            Create_Main_Window(instance, command_show, unknown_screen_width_a40b38, unknown_screen_height_a40b3c);
            Prep_Direct_Draw();
            if (unknown_flag_851cb0) {
                Rect rect4(0, 0, unknown_screen_width_a40b38, unknown_screen_height_a40b3c);
                unknown_window_839640 = rect4;
                unknown_8523e0 = unknown_screen_width_a40b38;
                unknown_8523e4 = unknown_screen_height_a40b3c;
                unknown_depth_8523e8 = unknown_hicolor_7ce008 ? 16 : 8;
            } else {
                video_success = Set_Video_Mode(MainWindow, unknown_screen_width_a40b38, unknown_screen_height_a40b3c, unknown_hicolor_7ce008 ? 16 : 8);
                Rect rect5(0, 0, unknown_screen_width_a40b38, unknown_screen_height_a40b3c);
                unknown_window_839640 = rect5;
                if (!video_success) {
                    if (unknown_screen_width_a40b38 != 640 || unknown_screen_height_a40b3c != 480) {
                        unknown_screen_width_a40b38 = 640;
                        unknown_screen_height_a40b3c = 480;
                        video_success = Set_Video_Mode(MainWindow, unknown_screen_width_a40b38, unknown_screen_height_a40b3c, unknown_hicolor_7ce008 ? 16 : 8);
                        Rect rect6(0, 0, unknown_screen_width_a40b38, unknown_screen_height_a40b3c);
                        unknown_window_839640 = rect6;
                    }
                    if (!video_success) {
                        Release_DirectDraw_00495a60();
                        Wide_Message_Box(MainWindow, StringTable::Load_String("TXT_VIDEO_ERROR", 0, "D:\\Ra2\\Startup.CPP", 0x828),
                                         StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x829), 0x30);
                        exit(1);
                    }
                }
            }
        }

        if (unknown_audio_mode_a41664 != 0 && Audio_File_Init(1000, 0x19000)) {
            if (unknown_audio_mode_a41664 == 1) {
                Audio_File_Record("session.ipb");
            } else if (unknown_audio_mode_a41664 == 2) {
                Audio_File_Open_Stream("session.ipb");
            }
        }

        unknown_surface_839950 = DSurface::Create_Primary(0);
        int bpp = unknown_surface_839950->Bytes_Per_Pixel();
        unknown_hicolor_7ce008 = bpp == 2;
        if (!unknown_hicolor_7ce008) {
            Wide_Message_Box(MainWindow, StringTable::Load_String("TXT_INVALIDMODE", 0, "D:\\Ra2\\Startup.CPP", 0x846),
                             StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Startup.CPP", 0x846), 0x30);
            Unknown_00691320();
        }
        if (!Notify_Launcher_00491930()) {
            Debug_Printf("Failed to notify launcher\n");
            exit(1);
        }

        do {
            Windows_Message_Handler();
        } while (!unknown_flag_a40d28);

        unknown_redraw_8523dc = false;
        if (!unknown_flag_851cb0) {
            unknown_surface_839950->Fill(0);
        }
        Mono_Window_Init(ProgramInstance, MainWindow, unknown_flag_851cb0);
        Planning_Reset();

        {
        int sidebar_width = unknown_int_7add80;
        int screen_height = unknown_window_839640.Height;
        Rect side_rect(0, 0, sidebar_width, screen_height);
        int width = unknown_window_839640.Width;
        Rect tile_rect(0, 0, width - side_rect.Width, side_rect.Height);
        Rect composite_rect(0, 0, width - side_rect.Width, side_rect.Height);
        Allocate_Surfaces(&unknown_window_839640, &composite_rect, &tile_rect, &side_rect, false);
        }
        if (unknown_flag_852fd7) {
            Init_Direct3D();
        }
        unknown_surface_83995c = unknown_surface_839954;
        GScreenClass::Blit(true, unknown_surface_839954);

        DepthBuffer = new ZBuffer(Rect(unknown_int_839630, unknown_int_839634, 480, 480 - unknown_int_839634));
        DepthBuffer->Set_Scroll(0x8000);
        AlphaBuffer = new ABuffer(Rect(unknown_int_839630, unknown_int_839634, 480, 480 - unknown_int_839634));

        Init_Mouse();
        WWMouse->Hide_Mouse();

        CDFileClass::Set_CD_Drive(CDList.Get_First_CD_Drive());

        int disk_number = Unknown_0046E4C0(60);
        Get_Performance_Profile();
        if (!unknown_special_a40920.IsFromInstall && disk_number > -1) {
            unknown_special_a40920.IsFromInstall = unknown_ini_839740.Get_Bool("Intro", "Play", true);
        }

        WolRegistryOptions wolopt;
        wolopt.Read_004ff200();
        if (!wolopt.unknown_004ff2b0() && !wolopt.unknown_004ff2d0() && !wolopt.unknown_004ff2c0()) {
            unknown_special_a40920.EscapeMovies = true;
        }

        if (unknown_special_a40920.IsFromInstall == true) {
            if (disk_number > -1) {
                unknown_special_a40920.IsFromInstall = unknown_ini_839740.Put_Bool("Intro", "Play", false);
            }
            cfile->Close();
            cfile->Open(1);
            unknown_ini_839740.Save(*cfile, false);
        }
        cfile->Close();
        delete cfile;

        if (unknown_no_exception_handler_a4168c) {
            Main_Game(argc, argv);
        } else {
            __try {
                Debug_Printf("Main_Game\n");
                Main_Game(argc, argv);
            } __except (Handle_Exception_004b87f0(_exception_code(), (_EXCEPTION_POINTERS *)_exception_info())) {
            }
            SetUnhandledExceptionFilter((StartupTopLevelFilter)Top_Level_Exception_Filter_0068e270);
        }

        WWMouse->Hide_Mouse();
        unknown_surface_839954->Fill(0);
        GScreenClass::Blit(true, unknown_surface_839954);

        ReadyToQuit = 1;
        Theme.Unknown_006E7A70();
        Audio_Shutdown();
        PostMessageA(MainWindow, 2, 0, 0);
        do {
            Windows_Message_Handler();
        } while (ReadyToQuit == 1);

        retval = 0;
    }

    MultiplayerGameMode::unknown_delete_modes_5b8d40();
    RadarEventClass_unknown_delete_all_63b890();
    Map.DisposeOfArt();
    Unknown_00603FA0();
    Mono_Window_Shutdown();
    Unknown_005D5F40();
    Unknown_005EAC20();
    Unknown_00613E40();
    Unknown_Free_All_Art_006F1D20();
    Unknown_006F57E0();
    Unknown_00535970();
    MouseClass::Unknown_0059FEF0();
    Shutdown_Network_005bc4d0();

    if (unknown_active_object_7f4ccc != -1) {
        RevokeActiveObject(unknown_active_object_7f4ccc, 0);
        unknown_active_object_7f4ccc = -1;
    }
    CoDisconnectObject((IUnknown *)&unknown_application_84c7dc, 0);
    OleUninitialize();
    return retval;

}


// void __cdecl Prog_End() (0x006909E0): the program's exit cleanup, registered with atexit by
// WinMain and reached from the shutdown path 0x00691320. Frees the palettes, converters,
// surfaces, mixfiles, INI databases, lists and libraries the start-up made, then stops the
// mouse thread.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/session_state.hpp"
#include "shared/color_scheme.hpp"
#include "shared/movie_list.hpp"
#include "shared/command.hpp"
#include "shared/radar.hpp"
#include "shared/tab.hpp"
#include "shared/map_view.hpp"
#include "shared/text_print.hpp"
#include "shared/convert.hpp"
#include "shared/light_convert.hpp"
#include "shared/gadget.hpp"
#include "shared/unit_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/surface.hpp"
#include "shared/tactical.hpp"
#include "shared/buffer.hpp"
#include "shared/rules.hpp"
#include "shared/id_source.hpp"
#include "shared/lobby.hpp"
#include "shared/crt_memory.hpp"

// Template copies the unit emits (ScenarioClass's lists; radar.hpp's and map.hpp's list members),
// bound to the kept copies.


// (Query_DLL_Version 0x0049EE00 declares it with a strict HMODULE.)
WIN32_IMPORT BOOL WINAPI FreeLibrary(HANDLE module);

void Unknown_Free_Screen_Surfaces_0068E1C0();
void __fastcall Delete_All_Objects_00519960();
void RadarEventClass_unknown_delete_all_63b890();
void __fastcall Unknown_Free_All_Art_006F1D20();
void Speech_Free_All();
void Audio_Shutdown();
void Unknown_006F9D90();
void __cdecl CopyProtection_Shutdown_00491b00();

void __cdecl Prog_End()
{
    unknown_flag_a40958 = false;
    if (unknown_grfxtxt_shape_a41674) {
        delete unknown_grfxtxt_shape_a41674;
        unknown_grfxtxt_shape_a41674 = 0;
    }
    if (unknown_grfxtxt_drawer_a41678) {
        delete unknown_grfxtxt_drawer_a41678;
        unknown_grfxtxt_drawer_a41678 = 0;
    }
    if (unknown_grfxtxt_palette_a41670) {
        delete unknown_grfxtxt_palette_a41670;
        unknown_grfxtxt_palette_a41670 = 0;
    }
    unknown_session_a3d298.Free_Scenario_Descriptions();

    while (ColorSchemes.Count()) {
        delete ColorSchemes[0];
    }
    ColorSchemes.Clear();

    Delete_All_Objects_00519960();

    while (unknown_names_a71050.Count()) {
        free((void *)unknown_names_a71050[0]);
        unknown_names_a71050.Delete(0);
    }
    unknown_names_a71050.Clear();

    for (int index = 0; index < unknown_commands_832358.Count(); index++) {
        delete unknown_commands_832358[index];
    }
    unknown_commands_832358.Clear();

    if (unknown_ini_8396d0) {
        delete unknown_ini_8396d0;
        unknown_ini_8396d0 = 0;
    }
    unknown_ini_8397a0.Clear();
    ArtINI.Clear();
    unknown_ini_839860.Clear();
    unknown_ini_8396e0.Clear();
    unknown_ini_839740.Clear();
    unknown_ini_839740.Clear();
    Unknown_005DDB40();
    Unknown_00521CB0();

    if (WWMouse) {
        delete WWMouse;
        WWMouse = 0;
    }
    Map.unknown_slot23();
    Map.Clear_Radar();
    Unknown_Free_Screen_Surfaces_0068E1C0();
    if (DepthBuffer) {
        delete DepthBuffer;
        DepthBuffer = 0;
    }
    if (AlphaBuffer) {
        delete AlphaBuffer;
        AlphaBuffer = 0;
    }
    if (BitText) {
        delete BitText;
        BitText = 0;
    }
    if (unknown_drawer_8323b8) {
        delete unknown_drawer_8323b8;
        unknown_drawer_8323b8 = 0;
    }
    if (Unit_Draw_Convert_008323C0) {
        delete Unit_Draw_Convert_008323C0;
        Unit_Draw_Convert_008323C0 = 0;
    }
    if (unknown_drawer_8323c4) {
        delete unknown_drawer_8323c4;
        unknown_drawer_8323c4 = 0;
    }
    if (unknown_drawer_8323b4) {
        delete unknown_drawer_8323b4;
        unknown_drawer_8323b4 = 0;
    }
    if (unknown_convert_8323c8) {
        delete (ConvertClass *)unknown_convert_8323c8;
        unknown_convert_8323c8 = 0;
    }
    if (unknown_drawer_8323cc) {
        delete unknown_drawer_8323cc;
        unknown_drawer_8323cc = 0;
    }
    if (unknown_drawer_8323b0) {
        delete unknown_drawer_8323b0;
        unknown_drawer_8323b0 = 0;
    }
    if (unknown_drawer_ad0608) {
        delete unknown_drawer_ad0608;
        unknown_drawer_ad0608 = 0;
    }
    if (unknown_surface_ad0604) {
        delete unknown_surface_ad0604;
        unknown_surface_ad0604 = 0;
    }
    if (unknown_cloak_surface_8502a0) {
        delete unknown_cloak_surface_8502a0;
        unknown_cloak_surface_8502a0 = 0;
    }
    if (unknown_building_depth_shape_85029c) {
        delete unknown_building_depth_shape_85029c;
        unknown_building_depth_shape_85029c = 0;
    }
    Map.Unknown_00562EA0();

    for (int light = 0; light < unknown_LightConvertClass_list_00832398.Count(); light++) {
        delete unknown_LightConvertClass_list_00832398[light];
    }
    unknown_LightConvertClass_list_00832398.Clear();

    if (unknown_theater_mix_8374c0) {
        delete unknown_theater_mix_8374c0;
        unknown_theater_mix_8374c0 = 0;
    }
    if (unknown_theater_mix_8374c4) {
        delete unknown_theater_mix_8374c4;
        unknown_theater_mix_8374c4 = 0;
    }
    if (unknown_theater_mix_8374c8) {
        delete unknown_theater_mix_8374c8;
        unknown_theater_mix_8374c8 = 0;
    }
    if (unknown_theater_mix_8374cc) {
        delete unknown_theater_mix_8374cc;
        unknown_theater_mix_8374cc = 0;
    }
    if (unknown_theater_mix_8374d0) {
        delete unknown_theater_mix_8374d0;
        unknown_theater_mix_8374d0 = 0;
    }
    if (unknown_block_a6e38c) {
        delete unknown_block_a6e38c;
        unknown_block_a6e38c = 0;
    }
    if (unknown_block_a52eb0) {
        delete unknown_block_a52eb0;
        unknown_block_a52eb0 = 0;
    }
    if (unknown_block_a52eb4) {
        delete unknown_block_a52eb4;
        unknown_block_a52eb4 = 0;
    }
    if (unknown_block_a52eb8) {
        delete unknown_block_a52eb8;
        unknown_block_a52eb8 = 0;
    }
    if (unknown_block_a52ebc) {
        delete unknown_block_a52ebc;
        unknown_block_a52ebc = 0;
    }
    if (unknown_surface_839968) {
        delete unknown_surface_839968;
        unknown_surface_839968 = 0;
    }
    if (MoviesMix) {
        delete MoviesMix;
        MoviesMix = 0;
    }
    if (unknown_mix_8374d8) {
        delete unknown_mix_8374d8;
        unknown_mix_8374d8 = 0;
    }
    if (MainMix) {
        delete MainMix;
        MainMix = 0;
    }
    if (ConquerMix) {
        delete ConquerMix;
        ConquerMix = 0;
    }
    if (CameoMix) {
        delete CameoMix;
        CameoMix = 0;
    }
    while (unknown_expansion_mixes_837450.Count() > 0) {
        delete unknown_expansion_mixes_837450[0];
        unknown_expansion_mixes_837450.Delete(0);
    }
    if (unknown_cache_mix_8374e8) {
        delete unknown_cache_mix_8374e8;
        unknown_cache_mix_8374e8 = 0;
    }
    if (unknown_local_mix_8374ec) {
        delete unknown_local_mix_8374ec;
        unknown_local_mix_8374ec = 0;
    }
    if (unknown_mix_8374f0) {
        delete unknown_mix_8374f0;
        unknown_mix_8374f0 = 0;
    }
    if (unknown_mix_8374f8) {
        delete unknown_mix_8374f8;
        unknown_mix_8374f8 = 0;
    }
    if (unknown_mix_8374fc) {
        delete unknown_mix_8374fc;
        unknown_mix_8374fc = 0;
    }
    while (unknown_mixes_837480.Count() > 0) {
        delete unknown_mixes_837480[0];
        unknown_mixes_837480.Delete(0);
    }
    if (MapsMix) {
        delete MapsMix;
        MapsMix = 0;
    }
    if (MultiMix) {
        delete MultiMix;
        MultiMix = 0;
    }
    if (unknown_side_mix_837500) {
        delete unknown_side_mix_837500;
        unknown_side_mix_837500 = 0;
    }
    if (unknown_side_nc_mix_837504) {
        delete unknown_side_nc_mix_837504;
        unknown_side_nc_mix_837504 = 0;
    }
    if (unknown_ra2_mix_8374b8) {
        delete unknown_ra2_mix_8374b8;
        unknown_ra2_mix_8374b8 = 0;
    }
    if (unknown_language_mix_8374bc) {
        delete unknown_language_mix_8374bc;
        unknown_language_mix_8374bc = 0;
    }
    if (unknown_tactical_83996c) {
        delete unknown_tactical_83996c;
        unknown_tactical_83996c = 0;
    }
    RadarEventClass_unknown_delete_all_63b890();
    Map.DisposeOfArt();
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
    Unknown_00714860();
    Speech_Free_All();
    CDFileClass::Clear_Search_Drives();
    if (WWKeyboard) {
        delete WWKeyboard;
        WWKeyboard = 0;
    }
    Unknown_005BB5A0();
    if (unknown_buffer_a35e20) {
        delete unknown_buffer_a35e20;
        unknown_buffer_a35e20 = 0;
    }
    if (Rules) {
        delete Rules;
        Rules = 0;
    }
    if (unknown_id_source_a3d290) {
        delete unknown_id_source_a3d290;
        unknown_id_source_a3d290 = 0;
    }

    for (int com = 0; com < unknown_unsigned_long_list_00abd428.Count(); com++) {
        CoRevokeClassObject(unknown_unsigned_long_list_00abd428[com]);
    }
    unknown_unsigned_long_list_00abd428.Clear();
    RevokeActiveObject(unknown_active_object_abd484, 0);
    if (unknown_instance_00851ca8) {
        FreeLibrary(unknown_instance_00851ca8);
    }
    if (unknown_handle_abd480) {
        CloseHandle(unknown_handle_abd480);
        unknown_handle_abd480 = 0;
    }
    if (unknown_handle_abd47c) {
        CloseHandle(unknown_handle_abd47c);
        unknown_handle_abd47c = 0;
    }
    Unknown_00522930();
    Theme.Unknown_006E7A70();
    Audio_Shutdown();
    Unknown_006F9D90();
    CopyProtection_Shutdown_00491b00();
    Kill_Mouse_Thread();
}


#include "shared/queue.hpp"


// void __fastcall Emergency_Exit(int code) (0x00691310): ends the program with an exit code; its
// callers run the orderly shutdown 0x00691320 first (Red Alert's Emergency_Exit, split in two).

void __fastcall Emergency_Exit(int code)
{
    exit(code);
}


// void Unknown_00691320() (0x00691320): the orderly shutdown before Emergency_Exit. Sets ReadyToQuit,
// posts WM_DESTROY to the game window and pumps messages until the window procedure has handled it,
// revokes and disconnects the application object, uninitializes OLE, removes the mouse, frees a
// pending block, posts WM_QUIT and ends with Prog_End.

void Unknown_00691320()
{
    ReadyToQuit = 1;
    PostMessageA(MainWindow, 0x0002, 0, 0);  // WM_DESTROY
    while (MainWindow) {
        Unknown_005B5EE0();
        if (ReadyToQuit != 1) {
            break;
        }
    }
    if (unknown_active_object_7f4ccc != -1) {
        RevokeActiveObject(unknown_active_object_7f4ccc, 0);
        unknown_active_object_7f4ccc = -1;
    }
    CoDisconnectObject((IUnknown *)&unknown_application_84c7dc, 0);
    OleUninitialize();
    if (WWMouse) {
        WWMouse->Release_Mouse();
        delete WWMouse;
    }
    WWMouse = 0;
    if (unknown_flag_abd48c) {
        delete unknown_block_abd488;
        unknown_flag_abd48c = false;
    }
    PostQuitMessage(0);
    Unknown_005BB5A0();
    Prog_End();
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<unsigned long>::Clear (0x006913F0; DynamicVectorClass<unsigned long> vtable 0x007ab9a0 slot 3).
// DynamicVectorClass<unsigned long>::ID_pointer (0x00691420; DynamicVectorClass<unsigned long> vtable 0x007ab9a0 slot 5).
// VectorClass<unsigned long>::Fetch (0x00691440; VectorClass<unsigned long> vtable 0x007ab9c0 slot 6).


// HRESULT IUnknownPtr::CreateInstance(GUID const &clsid, IUnknown *outer, unsigned long context) (0x00691450)
// comip.h's _com_ptr_t::CreateInstance for IUnknown: releases the held
// interface, then creates the object (a local or remote server is created
// as IUnknown, run and queried).

HRESULT IUnknownPtr::CreateInstance(GUID const &clsid, IUnknown *outer, unsigned long context)
{
    HRESULT result;

    _Release();
    if (context & (CLSCTX_LOCAL_SERVER | 0x10)) {
        IUnknown *unknown;
        result = CoCreateInstance(clsid, outer, context, IID_IUnknown, (void **)&unknown);
        if (FAILED(result)) {
            return result;
        }
        result = OleRun(unknown);
        if (SUCCEEDED(result)) {
            result = unknown->QueryInterface(IID_IUnknown, (void **)&Interface);
        }
        unknown->Release();
    } else {
        result = CoCreateInstance(clsid, outer, context, IID_IUnknown, (void **)&Interface);
    }
    return result;
}


// HRESULT __stdcall TClassFactory<CStreamClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x006915B0
// Purpose: the COM class factory for CStreamClass: validates the output pointer,
// refuses aggregation, constructs a CStreamClass (0x2C bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00494800 stores the CStreamClass vtables (RTTI). Only the members the factory touches are declared;
// the virtual destructor follows the eleven IStream methods
// (slot 14 of the primary vtable).

class CStreamClass {
public:
    CStreamClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void Read();    // IStream slot 3 (signature not needed here)
    virtual void Write();    // IStream slot 4 (signature not needed here)
    virtual void Seek();    // IStream slot 5 (signature not needed here)
    virtual void SetSize();    // IStream slot 6 (signature not needed here)
    virtual void CopyTo();    // IStream slot 7 (signature not needed here)
    virtual void Commit();    // IStream slot 8 (signature not needed here)
    virtual void Revert();    // IStream slot 9 (signature not needed here)
    virtual void LockRegion();    // IStream slot 10 (signature not needed here)
    virtual void UnlockRegion();    // IStream slot 11 (signature not needed here)
    virtual void Stat();    // IStream slot 12 (signature not needed here)
    virtual void Clone();    // IStream slot 13 (signature not needed here)
    virtual ~CStreamClass();

    unsigned char unknown_04[0x2C - 4];
};

template <>
HRESULT __stdcall TClassFactory<CStreamClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    CStreamClass *instance = new CStreamClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// HRESULT __stdcall TClassFactory<WaveClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00691730
// Purpose: the COM class factory for WaveClass: validates the output pointer,
// refuses aggregation, constructs a WaveClass (0x208 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x007216D0 builds on ObjectClass and owns a VectorClass<CellClass *>; WaveClass (size 0x208) is inferred, not RTTI-certified for this writer. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class WaveClass {
public:
    WaveClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~WaveClass();

    unsigned char unknown_04[0x208 - 4];
};

template <>
HRESULT __stdcall TClassFactory<WaveClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    WaveClass *instance = new WaveClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<TerrainTypeClass> COM creation, original 006918b0.
// Light TerrainTypeClass interface: allocation size 652 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TerrainTypeClass {
public:
    TerrainTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TerrainTypeClass();
    unsigned char unknown_04[0x28C - 4];
};

template <>
HRESULT __stdcall TClassFactory<TerrainTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TerrainTypeClass *instance = new TerrainTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<TerrainClass> COM creation, original 00691a30.
// Light TerrainClass interface: allocation size 200 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TerrainClass {
public:
    TerrainClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TerrainClass();
    unsigned char unknown_04[0xC8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TerrainClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TerrainClass *instance = new TerrainClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<SuperWeaponTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00691BB0
// Purpose: the COM class factory for SuperWeaponTypeClass: validates the output pointer,
// refuses aggregation, constructs a SuperWeaponTypeClass (0xEC bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x0069F000 is the recovered SuperWeaponTypeClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class SuperWeaponTypeClass {
public:
    SuperWeaponTypeClass(char const *ini_name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~SuperWeaponTypeClass();

    unsigned char unknown_04[0xEC - 4];
};

template <>
HRESULT __stdcall TClassFactory<SuperWeaponTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    SuperWeaponTypeClass *instance = new SuperWeaponTypeClass(0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<SuperClass> COM creation, original 00691d30.
// Light SuperClass interface: allocation size 104 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class SuperClass {
public:
    SuperClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~SuperClass();
    unsigned char unknown_04[0x68 - 4];
};

template <>
HRESULT __stdcall TClassFactory<SuperClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    SuperClass *instance = new SuperClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<CellClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00692030
// Purpose: the COM class factory for CellClass: validates the output pointer,
// refuses aggregation, constructs a CellClass (0x148 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00470F10 is the recovered CellClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class CellClass {
public:
    CellClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~CellClass();

    unsigned char unknown_04[0x148 - 4];
};

template <>
HRESULT __stdcall TClassFactory<CellClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    CellClass *instance = new CellClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<EMPulseClass> COM creation, original 006921b0.
// Light EMPulseClass interface: allocation size 52 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class EMPulseClass {
public:
    EMPulseClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~EMPulseClass();
    unsigned char unknown_04[0x34 - 4];
};

template <>
HRESULT __stdcall TClassFactory<EMPulseClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    EMPulseClass *instance = new EMPulseClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<LightSourceClass> COM creation, original 00692330.
// Light LightSourceClass interface: allocation size 76 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class LightSourceClass {
public:
    LightSourceClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~LightSourceClass();
    unsigned char unknown_04[0x4C - 4];
};

template <>
HRESULT __stdcall TClassFactory<LightSourceClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    LightSourceClass *instance = new LightSourceClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<SideClass> COM creation, original 006924b0.
// Light SideClass interface: allocation size 180 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class SideClass {
public:
    SideClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~SideClass();
    unsigned char unknown_04[0xB4 - 4];
};

template <>
HRESULT __stdcall TClassFactory<SideClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    SideClass *instance = new SideClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<TiberiumClass> COM creation, original 00692630.
// Light TiberiumClass interface: allocation size 296 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TiberiumClass {
public:
    TiberiumClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TiberiumClass();
    unsigned char unknown_04[0x128 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TiberiumClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TiberiumClass *instance = new TiberiumClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<TubeClass> COM creation, original 006927b0.
// Light TubeClass interface: allocation size 452 and QI/deleting destructor slots
// are independently present in the accepted original factory body.
class Cell;
extern "C" Cell const data_6927b0_0;

class TubeClass {
public:
    TubeClass(Cell const &cell, int value);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TubeClass();
    unsigned char unknown_04[0x1C4 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TubeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TubeClass *instance = new TubeClass(data_6927b0_0, 0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<CampaignClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00692930
// Purpose: the COM class factory for CampaignClass: validates the output pointer,
// refuses aggregation, constructs a CampaignClass (0x3A0 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00465450 is the recovered CampaignClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class CampaignClass {
public:
    CampaignClass(char const *ini_name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~CampaignClass();

    unsigned char unknown_04[0x3A0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<CampaignClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    CampaignClass *instance = new CampaignClass(0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<BuildingLightClass> COM creation, original 00692ab0.
// Light BuildingLightClass interface: allocation size 208 and QI/deleting destructor slots
// are independently present in the accepted original factory body.
class TechnoClass;

class BuildingLightClass {
public:
    BuildingLightClass(TechnoClass *owner);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~BuildingLightClass();
    unsigned char unknown_04[0xD0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<BuildingLightClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    BuildingLightClass *instance = new BuildingLightClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<WaypointPathClass> COM creation, original 00692c30.
// Light WaypointPathClass interface: allocation size 64 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class WaypointPathClass {
public:
    WaypointPathClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~WaypointPathClass();
    unsigned char unknown_04[0x40 - 4];
};

template <>
HRESULT __stdcall TClassFactory<WaypointPathClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    WaypointPathClass *instance = new WaypointPathClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<TemporalClass> COM creation, original 00692db0.
// Light TemporalClass interface: allocation size 80 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TemporalClass {
public:
    TemporalClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TemporalClass();
    unsigned char unknown_04[0x50 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TemporalClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TemporalClass *instance = new TemporalClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<SpawnManagerClass> COM creation, original 00692f30.
// Light SpawnManagerClass interface: allocation size 116 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class SpawnManagerClass {
public:
    SpawnManagerClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~SpawnManagerClass();
    unsigned char unknown_04[0x74 - 4];
};

template <>
HRESULT __stdcall TClassFactory<SpawnManagerClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    SpawnManagerClass *instance = new SpawnManagerClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<CaptureManagerClass> COM creation, original 006930b0.
// Light CaptureManagerClass interface: allocation size 48 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class CaptureManagerClass {
public:
    CaptureManagerClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~CaptureManagerClass();
    unsigned char unknown_04[0x30 - 4];
};

template <>
HRESULT __stdcall TClassFactory<CaptureManagerClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    CaptureManagerClass *instance = new CaptureManagerClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<ParasiteClass> COM creation, original 00693230.
// Light ParasiteClass interface: allocation size 88 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ParasiteClass {
public:
    ParasiteClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ParasiteClass();
    unsigned char unknown_04[0x58 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ParasiteClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ParasiteClass *instance = new ParasiteClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<RadSiteClass> COM creation, original 00693530.
// Light RadSiteClass interface: allocation size 116 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class RadSiteClass {
public:
    RadSiteClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~RadSiteClass();
    unsigned char unknown_04[0x74 - 4];
};

template <>
HRESULT __stdcall TClassFactory<RadSiteClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    RadSiteClass *instance = new RadSiteClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<VoxelAnimTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00693830
// Purpose: the COM class factory for VoxelAnimTypeClass: validates the output pointer,
// refuses aggregation, constructs a VoxelAnimTypeClass (0x2D8 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x0070E1A0 is the recovered VoxelAnimTypeClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class VoxelAnimTypeClass {
public:
    VoxelAnimTypeClass(char const *ini_name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~VoxelAnimTypeClass();

    unsigned char unknown_04[0x2D8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<VoxelAnimTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    VoxelAnimTypeClass *instance = new VoxelAnimTypeClass(0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<VoxelAnimClass> COM creation, original 006939b0.
// Light VoxelAnimClass interface: allocation size 304 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class VoxelAnimClass {
public:
    VoxelAnimClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~VoxelAnimClass();
    unsigned char unknown_04[0x130 - 4];
};

template <>
HRESULT __stdcall TClassFactory<VoxelAnimClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    VoxelAnimClass *instance = new VoxelAnimClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<ScriptClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00693FB0
// Purpose: the COM class factory for ScriptClass: validates the output pointer,
// refuses aggregation, constructs a ScriptClass (0x30 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00668730 is the recovered ScriptClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class ScriptTypeClass;

class ScriptClass {
public:
    ScriptClass(ScriptTypeClass *type);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ScriptClass();

    unsigned char unknown_04[0x30 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ScriptClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    ScriptClass *instance = new ScriptClass(0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<ScriptTypeClass> COM creation, original 00694130.
// Light ScriptTypeClass interface: allocation size 564 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ScriptTypeClass {
public:
    ScriptTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ScriptTypeClass();
    unsigned char unknown_04[0x234 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ScriptTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ScriptTypeClass *instance = new ScriptTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<TeamClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x006945B0
// Purpose: the COM class factory for TeamClass: validates the output pointer,
// refuses aggregation, constructs a TeamClass (0x9C bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x006B7430 stores the TeamClass vtables (RTTI); its
// parameters (type, house, a flag) are assumed from the three zero arguments. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class TeamTypeClass;
class HouseClass;

class TeamClass {
public:
    TeamClass(TeamTypeClass *type, HouseClass *house, int unknown_flag);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TeamClass();

    unsigned char unknown_04[0x9C - 4];
};

template <>
HRESULT __stdcall TClassFactory<TeamClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    TeamClass *instance = new TeamClass(0, 0, 0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<TeamTypeClass> COM creation, original 00694730.
// Light TeamTypeClass interface: allocation size 240 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TeamTypeClass {
public:
    TeamTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TeamTypeClass();
    unsigned char unknown_04[0xF0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TeamTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TeamTypeClass *instance = new TeamTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<TaskForceClass> COM creation, original 006948b0.
// Light TaskForceClass interface: allocation size 212 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class TaskForceClass {
public:
    TaskForceClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TaskForceClass();
    unsigned char unknown_04[0xD4 - 4];
};

template <>
HRESULT __stdcall TClassFactory<TaskForceClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    TaskForceClass *instance = new TaskForceClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<AircraftTypeClass> COM creation, original 00694d30.
// Light AircraftTypeClass interface: allocation size 2984 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class AircraftTypeClass {
public:
    AircraftTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AircraftTypeClass();
    unsigned char unknown_04[0xBA8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AircraftTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    AircraftTypeClass *instance = new AircraftTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<InfantryTypeClass> COM creation, original 00694eb0.
// Light InfantryTypeClass interface: allocation size 3096 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class InfantryTypeClass {
public:
    InfantryTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~InfantryTypeClass();
    unsigned char unknown_04[0xC18 - 4];
};

template <>
HRESULT __stdcall TClassFactory<InfantryTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    InfantryTypeClass *instance = new InfantryTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<IsometricTileTypeClass> COM creation, original 006951b0.
// Light IsometricTileTypeClass interface: allocation size 732 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class IsometricTileTypeClass {
public:
    IsometricTileTypeClass(int type, int value, unsigned char mode, char const *name, bool skip);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~IsometricTileTypeClass();
    unsigned char unknown_04[0x2DC - 4];
};

template <>
HRESULT __stdcall TClassFactory<IsometricTileTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    IsometricTileTypeClass *instance = new IsometricTileTypeClass(0, 0, 0, 0, false);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<OverlayTypeClass> COM creation, original 00695330.
// Light OverlayTypeClass interface: allocation size 648 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class OverlayTypeClass {
public:
    OverlayTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~OverlayTypeClass();
    unsigned char unknown_04[0x288 - 4];
};

template <>
HRESULT __stdcall TClassFactory<OverlayTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    OverlayTypeClass *instance = new OverlayTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<SmudgeTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x006954B0
// Purpose: the COM class factory for SmudgeTypeClass: validates the output pointer,
// refuses aggregation, constructs a SmudgeTypeClass (0x274 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00687DA0 stores the SmudgeTypeClass vtables (RTTI); its parameter type is assumed (INI name, as for the other type classes). Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class SmudgeTypeClass {
public:
    SmudgeTypeClass(char const *ini_name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~SmudgeTypeClass();

    unsigned char unknown_04[0x274 - 4];
};

template <>
HRESULT __stdcall TClassFactory<SmudgeTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    SmudgeTypeClass *instance = new SmudgeTypeClass(0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<BuildingClass> COM creation, original 006957b0.
// Light BuildingClass interface: allocation size 1504 and QI/deleting destructor slots
// are independently present in the accepted original factory body.
class BuildingTypeClass;
class HouseClass;

class BuildingClass {
public:
    BuildingClass(BuildingTypeClass *type, HouseClass *house);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~BuildingClass();
    unsigned char unknown_04[0x5E0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<BuildingClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    BuildingClass *instance = new BuildingClass(0, 0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<AircraftClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00695930
// Purpose: the COM class factory for AircraftClass: validates the output pointer,
// refuses aggregation, constructs a AircraftClass (0x5C0 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00413A40 is the recovered AircraftClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class AircraftTypeClass;
class HouseClass;

class AircraftClass {
public:
    AircraftClass(AircraftTypeClass *type, HouseClass *house);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AircraftClass();

    unsigned char unknown_04[0x5C0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AircraftClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    AircraftClass *instance = new AircraftClass(0, 0);
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<InfantryClass> COM creation, original 00695ab0.
// Light InfantryClass interface: allocation size 1496 and QI/deleting destructor slots
// are independently present in the accepted original factory body.
class InfantryTypeClass;
class HouseClass;

class InfantryClass {
public:
    InfantryClass(InfantryTypeClass *type, HouseClass *house);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~InfantryClass();
    unsigned char unknown_04[0x5D8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<InfantryClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    InfantryClass *instance = new InfantryClass(0, 0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<AnimClass> COM creation, original 00695c30.
// Light AnimClass interface: allocation size 424 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class AnimClass {
public:
    AnimClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AnimClass();
    unsigned char unknown_04[0x1A8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AnimClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    AnimClass *instance = new AnimClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<AnimTypeClass> COM creation, original 00695db0.
// Light AnimTypeClass interface: allocation size 816 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class AnimTypeClass {
public:
    AnimTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AnimTypeClass();
    unsigned char unknown_04[0x330 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AnimTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    AnimTypeClass *instance = new AnimTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<DriveLocomotionClass> COM creation, original 00696230.
// Light DriveLocomotionClass interface: allocation size 112 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class DriveLocomotionClass {
public:
    DriveLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~DriveLocomotionClass();
    unsigned char unknown_04[0x70 - 4];
};

template <>
HRESULT __stdcall TClassFactory<DriveLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    DriveLocomotionClass *instance = new DriveLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<JumpjetLocomotionClass> COM creation, original 006963b0.
// Light JumpjetLocomotionClass interface: allocation size 144 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class JumpjetLocomotionClass {
public:
    JumpjetLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~JumpjetLocomotionClass();
    unsigned char unknown_04[0x90 - 4];
};

template <>
HRESULT __stdcall TClassFactory<JumpjetLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    JumpjetLocomotionClass *instance = new JumpjetLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<HoverLocomotionClass> COM creation, original 00696530.
// Light HoverLocomotionClass interface: allocation size 120 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class HoverLocomotionClass {
public:
    HoverLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~HoverLocomotionClass();
    unsigned char unknown_04[0x78 - 4];
};

template <>
HRESULT __stdcall TClassFactory<HoverLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    HoverLocomotionClass *instance = new HoverLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<RocketLocomotionClass> COM creation, original 006966b0.
// Light RocketLocomotionClass interface: allocation size 88 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class RocketLocomotionClass {
public:
    RocketLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~RocketLocomotionClass();
    unsigned char unknown_04[0x58 - 4];
};

template <>
HRESULT __stdcall TClassFactory<RocketLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    RocketLocomotionClass *instance = new RocketLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<TunnelLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00696830
// Purpose: the COM class factory for TunnelLocomotionClass: validates the output pointer,
// refuses aggregation, constructs a TunnelLocomotionClass (0x3C bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x006EFBE0 is the recovered TunnelLocomotionClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class TunnelLocomotionClass {
public:
    TunnelLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TunnelLocomotionClass();

    unsigned char unknown_04[0x3C - 4];
};

template <>
HRESULT __stdcall TClassFactory<TunnelLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    TunnelLocomotionClass *instance = new TunnelLocomotionClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<WalkLocomotionClass> COM creation, original 006969b0.
// Light WalkLocomotionClass interface: allocation size 60 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class WalkLocomotionClass {
public:
    WalkLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~WalkLocomotionClass();
    unsigned char unknown_04[0x3C - 4];
};

template <>
HRESULT __stdcall TClassFactory<WalkLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    WalkLocomotionClass *instance = new WalkLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<DropPodLocomotionClass> COM creation, original 00696b30.
// Light DropPodLocomotionClass interface: allocation size 48 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class DropPodLocomotionClass {
public:
    DropPodLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~DropPodLocomotionClass();
    unsigned char unknown_04[0x30 - 4];
};

template <>
HRESULT __stdcall TClassFactory<DropPodLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    DropPodLocomotionClass *instance = new DropPodLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<FlyLocomotionClass> COM creation, original 00696cb0.
// Light FlyLocomotionClass interface: allocation size 96 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class FlyLocomotionClass {
public:
    FlyLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~FlyLocomotionClass();
    unsigned char unknown_04[0x60 - 4];
};

template <>
HRESULT __stdcall TClassFactory<FlyLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    FlyLocomotionClass *instance = new FlyLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<TeleportLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00696E30
// Purpose: the COM class factory for TeleportLocomotionClass: validates the output pointer,
// refuses aggregation, constructs a TeleportLocomotionClass (0x4C bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x006DFCA0 is the recovered TeleportLocomotionClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class TeleportLocomotionClass {
public:
    TeleportLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~TeleportLocomotionClass();

    unsigned char unknown_04[0x4C - 4];
};

template <>
HRESULT __stdcall TClassFactory<TeleportLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    TeleportLocomotionClass *instance = new TeleportLocomotionClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<MechLocomotionClass> COM creation, original 00696fb0.
// Light MechLocomotionClass interface: allocation size 52 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class MechLocomotionClass {
public:
    MechLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~MechLocomotionClass();
    unsigned char unknown_04[0x34 - 4];
};

template <>
HRESULT __stdcall TClassFactory<MechLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    MechLocomotionClass *instance = new MechLocomotionClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<ShipLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00697130
// Purpose: the COM class factory for ShipLocomotionClass: validates the output pointer,
// refuses aggregation, constructs a ShipLocomotionClass (0x70 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x00675600 stores the ShipLocomotionClass vtables (RTTI). Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class ShipLocomotionClass {
public:
    ShipLocomotionClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ShipLocomotionClass();

    unsigned char unknown_04[0x70 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ShipLocomotionClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    ShipLocomotionClass *instance = new ShipLocomotionClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// TClassFactory<BulletClass> COM creation, original 006972b0.
// Light BulletClass interface: allocation size 328 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class BulletClass {
public:
    BulletClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~BulletClass();
    unsigned char unknown_04[0x148 - 4];
};

template <>
HRESULT __stdcall TClassFactory<BulletClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    BulletClass *instance = new BulletClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<FactoryClass> COM creation, original 00697430.
// Light FactoryClass interface: allocation size 112 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class FactoryClass {
public:
    FactoryClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~FactoryClass();
    unsigned char unknown_04[0x70 - 4];
};

template <>
HRESULT __stdcall TClassFactory<FactoryClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    FactoryClass *instance = new FactoryClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<ParticleClass> COM creation, original 006978b0.
// Light ParticleClass interface: allocation size 296 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ParticleClass {
public:
    ParticleClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ParticleClass();
    unsigned char unknown_04[0x128 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ParticleClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ParticleClass *instance = new ParticleClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<ParticleTypeClass> COM creation, original 00697a30.
// Light ParticleTypeClass interface: allocation size 744 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ParticleTypeClass {
public:
    ParticleTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ParticleTypeClass();
    unsigned char unknown_04[0x2E8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ParticleTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ParticleTypeClass *instance = new ParticleTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<ParticleSystemClass> COM creation, original 00697bb0.
// Light ParticleSystemClass interface: allocation size 232 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ParticleSystemClass {
public:
    ParticleSystemClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ParticleSystemClass();
    unsigned char unknown_04[0xE8 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ParticleSystemClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ParticleSystemClass *instance = new ParticleSystemClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<ParticleSystemTypeClass> COM creation, original 00697d30.
// Light ParticleSystemTypeClass interface: allocation size 736 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class ParticleSystemTypeClass {
public:
    ParticleSystemTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~ParticleSystemTypeClass();
    unsigned char unknown_04[0x2E0 - 4];
};

template <>
HRESULT __stdcall TClassFactory<ParticleSystemTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    ParticleSystemTypeClass *instance = new ParticleSystemTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<AITriggerTypeClass> COM creation, original 00697eb0.
// Light AITriggerTypeClass interface: allocation size 272 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class AITriggerTypeClass {
public:
    AITriggerTypeClass(char const *name);
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AITriggerTypeClass();
    unsigned char unknown_04[0x110 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AITriggerTypeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    AITriggerTypeClass *instance = new AITriggerTypeClass(0);
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<NeuronClass> COM creation, original 00698030.
// Light NeuronClass interface: allocation size 56 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class NeuronClass {
public:
    NeuronClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~NeuronClass();
    unsigned char unknown_04[0x38 - 4];
};

template <>
HRESULT __stdcall TClassFactory<NeuronClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    NeuronClass *instance = new NeuronClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// TClassFactory<FoggedObjectClass> COM creation, original 006981b0.
// Light FoggedObjectClass interface: allocation size 120 and QI/deleting destructor slots
// are independently present in the accepted original factory body.

class FoggedObjectClass {
public:
    FoggedObjectClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~FoggedObjectClass();
    unsigned char unknown_04[0x78 - 4];
};

template <>
HRESULT __stdcall TClassFactory<FoggedObjectClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) return E_INVALIDARG;
    *object = 0;
    if (outer != 0) return CLASS_E_NOAGGREGATION;
    FoggedObjectClass *instance = new FoggedObjectClass();
    if (instance == 0) return E_OUTOFMEMORY;
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) delete instance;
    return result;
}


// HRESULT __stdcall TClassFactory<AlphaShapeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00698330
// Purpose: the COM class factory for AlphaShapeClass: validates the output pointer,
// refuses aggregation, constructs a AlphaShapeClass (0x40 bytes) and queries it
// for iid, destroying it again when the query fails.
// Assumptions: the constructor at 0x0041ED70 is the recovered AlphaShapeClass constructor. Only the members the factory touches are declared;
// the virtual destructor occupies slot 8 of the primary vtable.

class AlphaShapeClass {
public:
    AlphaShapeClass();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual void unknown_slot3();
    virtual void unknown_slot4();
    virtual void unknown_slot5();
    virtual void unknown_slot6();
    virtual void unknown_slot7();
    virtual ~AlphaShapeClass();

    unsigned char unknown_04[0x40 - 4];
};

template <>
HRESULT __stdcall TClassFactory<AlphaShapeClass>::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    AlphaShapeClass *instance = new AlphaShapeClass();
    if (instance == 0) {
        return E_OUTOFMEMORY;
    }
    HRESULT result = instance->QueryInterface(iid, object);
    if (FAILED(result)) {
        delete instance;
    }
    return result;
}


// Class factory members this unit emits by its own use of TClassFactory<T> (bodies: shared/class_factory.hpp):
// 006914e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<CStreamClass>::QueryInterface(GUID const &iid, void **object) (0x006914F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<CStreamClass>::AddRef (0x00691560); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<CStreamClass>::Release (0x00691580); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<CStreamClass>::LockServer (0x00691630); body: shared/class_factory.hpp
// 00691660: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<WaveClass>::QueryInterface(GUID const &iid, void **object) (0x00691670)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<WaveClass>::AddRef (0x006916E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<WaveClass>::Release (0x00691700); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<WaveClass>::LockServer (0x006917B0); body: shared/class_factory.hpp
// 006917e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TerrainTypeClass>::QueryInterface(GUID const &iid, void **object) (0x006917F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TerrainTypeClass>::AddRef (0x00691860); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TerrainTypeClass>::Release (0x00691880); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TerrainTypeClass>::LockServer (0x00691930); body: shared/class_factory.hpp
// 00691960: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TerrainClass>::QueryInterface(GUID const &iid, void **object) (0x00691970)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TerrainClass>::AddRef (0x006919E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TerrainClass>::Release (0x00691A00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TerrainClass>::LockServer (0x00691AB0); body: shared/class_factory.hpp
// 00691ae0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<SuperWeaponTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00691AF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<SuperWeaponTypeClass>::AddRef (0x00691B60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<SuperWeaponTypeClass>::Release (0x00691B80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<SuperWeaponTypeClass>::LockServer (0x00691C30); body: shared/class_factory.hpp
// 00691c60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<SuperClass>::QueryInterface(GUID const &iid, void **object) (0x00691C70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<SuperClass>::AddRef (0x00691CE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<SuperClass>::Release (0x00691D00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<SuperClass>::LockServer (0x00691DB0); body: shared/class_factory.hpp
// 00691de0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<Tactical>::QueryInterface(GUID const &iid, void **object) (0x00691DF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<Tactical>::AddRef (0x00691E60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<Tactical>::Release (0x00691E80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<Tactical>::LockServer (0x00691F30); body: shared/class_factory.hpp
// 00691f60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<CellClass>::QueryInterface(GUID const &iid, void **object) (0x00691F70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<CellClass>::AddRef (0x00691FE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<CellClass>::Release (0x00692000); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<CellClass>::LockServer (0x006920B0); body: shared/class_factory.hpp
// 006920e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<EMPulseClass>::QueryInterface(GUID const &iid, void **object) (0x006920F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<EMPulseClass>::AddRef (0x00692160); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<EMPulseClass>::Release (0x00692180); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<EMPulseClass>::LockServer (0x00692230); body: shared/class_factory.hpp
// 00692260: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<LightSourceClass>::QueryInterface(GUID const &iid, void **object) (0x00692270)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<LightSourceClass>::AddRef (0x006922E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<LightSourceClass>::Release (0x00692300); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<LightSourceClass>::LockServer (0x006923B0); body: shared/class_factory.hpp
// 006923e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<SideClass>::QueryInterface(GUID const &iid, void **object) (0x006923F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<SideClass>::AddRef (0x00692460); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<SideClass>::Release (0x00692480); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<SideClass>::LockServer (0x00692530); body: shared/class_factory.hpp
// 00692560: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TiberiumClass>::QueryInterface(GUID const &iid, void **object) (0x00692570)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TiberiumClass>::AddRef (0x006925E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TiberiumClass>::Release (0x00692600); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TiberiumClass>::LockServer (0x006926B0); body: shared/class_factory.hpp
// 006926e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TubeClass>::QueryInterface(GUID const &iid, void **object) (0x006926F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TubeClass>::AddRef (0x00692760); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TubeClass>::Release (0x00692780); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TubeClass>::LockServer (0x00692830); body: shared/class_factory.hpp
// 00692860: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<CampaignClass>::QueryInterface(GUID const &iid, void **object) (0x00692870)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<CampaignClass>::AddRef (0x006928E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<CampaignClass>::Release (0x00692900); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<CampaignClass>::LockServer (0x006929B0); body: shared/class_factory.hpp
// 006929e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BuildingLightClass>::QueryInterface(GUID const &iid, void **object) (0x006929F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BuildingLightClass>::AddRef (0x00692A60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BuildingLightClass>::Release (0x00692A80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BuildingLightClass>::LockServer (0x00692B30); body: shared/class_factory.hpp
// 00692b60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<WaypointPathClass>::QueryInterface(GUID const &iid, void **object) (0x00692B70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<WaypointPathClass>::AddRef (0x00692BE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<WaypointPathClass>::Release (0x00692C00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<WaypointPathClass>::LockServer (0x00692CB0); body: shared/class_factory.hpp
// 00692ce0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TemporalClass>::QueryInterface(GUID const &iid, void **object) (0x00692CF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TemporalClass>::AddRef (0x00692D60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TemporalClass>::Release (0x00692D80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TemporalClass>::LockServer (0x00692E30); body: shared/class_factory.hpp
// 00692e60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<SpawnManagerClass>::QueryInterface(GUID const &iid, void **object) (0x00692E70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<SpawnManagerClass>::AddRef (0x00692EE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<SpawnManagerClass>::Release (0x00692F00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<SpawnManagerClass>::LockServer (0x00692FB0); body: shared/class_factory.hpp
// 00692fe0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<CaptureManagerClass>::QueryInterface(GUID const &iid, void **object) (0x00692FF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<CaptureManagerClass>::AddRef (0x00693060); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<CaptureManagerClass>::Release (0x00693080); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<CaptureManagerClass>::LockServer (0x00693130); body: shared/class_factory.hpp
// 00693160: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ParasiteClass>::QueryInterface(GUID const &iid, void **object) (0x00693170)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ParasiteClass>::AddRef (0x006931E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ParasiteClass>::Release (0x00693200); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ParasiteClass>::LockServer (0x006932B0); body: shared/class_factory.hpp
// 006932e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BombClass>::QueryInterface(GUID const &iid, void **object) (0x006932F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BombClass>::AddRef (0x00693360); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BombClass>::Release (0x00693380); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BombClass>::LockServer (0x00693430); body: shared/class_factory.hpp
// 00693460: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<RadSiteClass>::QueryInterface(GUID const &iid, void **object) (0x00693470)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<RadSiteClass>::AddRef (0x006934E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<RadSiteClass>::Release (0x00693500); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<RadSiteClass>::LockServer (0x006935B0); body: shared/class_factory.hpp
// 006935e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TEventClass>::QueryInterface(GUID const &iid, void **object) (0x006935F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TEventClass>::AddRef (0x00693660); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TEventClass>::Release (0x00693680); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TEventClass>::LockServer (0x00693730); body: shared/class_factory.hpp
// 00693760: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<VoxelAnimTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00693770)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<VoxelAnimTypeClass>::AddRef (0x006937E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<VoxelAnimTypeClass>::Release (0x00693800); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<VoxelAnimTypeClass>::LockServer (0x006938B0); body: shared/class_factory.hpp
// 006938e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<VoxelAnimClass>::QueryInterface(GUID const &iid, void **object) (0x006938F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<VoxelAnimClass>::AddRef (0x00693960); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<VoxelAnimClass>::Release (0x00693980); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<VoxelAnimClass>::LockServer (0x00693A30); body: shared/class_factory.hpp
// 00693a60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TActionClass>::QueryInterface(GUID const &iid, void **object) (0x00693A70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TActionClass>::AddRef (0x00693AE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TActionClass>::Release (0x00693B00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TActionClass>::LockServer (0x00693BB0); body: shared/class_factory.hpp
// 00693be0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TriggerClass>::QueryInterface(GUID const &iid, void **object) (0x00693BF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TriggerClass>::AddRef (0x00693C60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TriggerClass>::Release (0x00693C80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TriggerClass>::LockServer (0x00693D30); body: shared/class_factory.hpp
// 00693d60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TriggerTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00693D70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TriggerTypeClass>::AddRef (0x00693DE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TriggerTypeClass>::Release (0x00693E00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TriggerTypeClass>::LockServer (0x00693EB0); body: shared/class_factory.hpp
// 00693ee0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ScriptClass>::QueryInterface(GUID const &iid, void **object) (0x00693EF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ScriptClass>::AddRef (0x00693F60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ScriptClass>::Release (0x00693F80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ScriptClass>::LockServer (0x00694030); body: shared/class_factory.hpp
// 00694060: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ScriptTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694070)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ScriptTypeClass>::AddRef (0x006940E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ScriptTypeClass>::Release (0x00694100); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ScriptTypeClass>::LockServer (0x006941B0); body: shared/class_factory.hpp
// 006941e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TagClass>::QueryInterface(GUID const &iid, void **object) (0x006941F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TagClass>::AddRef (0x00694260); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TagClass>::Release (0x00694280); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TagClass>::LockServer (0x00694330); body: shared/class_factory.hpp
// 00694360: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TagTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694370)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TagTypeClass>::AddRef (0x006943E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TagTypeClass>::Release (0x00694400); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TagTypeClass>::LockServer (0x006944B0); body: shared/class_factory.hpp
// 006944e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TeamClass>::QueryInterface(GUID const &iid, void **object) (0x006944F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TeamClass>::AddRef (0x00694560); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TeamClass>::Release (0x00694580); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TeamClass>::LockServer (0x00694630); body: shared/class_factory.hpp
// 00694660: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TeamTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694670)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TeamTypeClass>::AddRef (0x006946E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TeamTypeClass>::Release (0x00694700); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TeamTypeClass>::LockServer (0x006947B0); body: shared/class_factory.hpp
// 006947e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TaskForceClass>::QueryInterface(GUID const &iid, void **object) (0x006947F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TaskForceClass>::AddRef (0x00694860); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TaskForceClass>::Release (0x00694880); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TaskForceClass>::LockServer (0x00694930); body: shared/class_factory.hpp
// 00694960: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<UnitTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694970)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<UnitTypeClass>::AddRef (0x006949E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<UnitTypeClass>::Release (0x00694A00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<UnitTypeClass>::LockServer (0x00694AB0); body: shared/class_factory.hpp
// 00694ae0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BuildingTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694AF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BuildingTypeClass>::AddRef (0x00694B60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BuildingTypeClass>::Release (0x00694B80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BuildingTypeClass>::LockServer (0x00694C30); body: shared/class_factory.hpp
// 00694c60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AircraftTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694C70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AircraftTypeClass>::AddRef (0x00694CE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AircraftTypeClass>::Release (0x00694D00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AircraftTypeClass>::LockServer (0x00694DB0); body: shared/class_factory.hpp
// 00694de0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<InfantryTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694DF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<InfantryTypeClass>::AddRef (0x00694E60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<InfantryTypeClass>::Release (0x00694E80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<InfantryTypeClass>::LockServer (0x00694F30); body: shared/class_factory.hpp
// 00694f60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BulletTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00694F70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BulletTypeClass>::AddRef (0x00694FE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BulletTypeClass>::Release (0x00695000); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BulletTypeClass>::LockServer (0x006950B0); body: shared/class_factory.hpp
// 006950e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<IsometricTileTypeClass>::QueryInterface(GUID const &iid, void **object) (0x006950F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<IsometricTileTypeClass>::AddRef (0x00695160); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<IsometricTileTypeClass>::Release (0x00695180); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<IsometricTileTypeClass>::LockServer (0x00695230); body: shared/class_factory.hpp
// 00695260: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<OverlayTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00695270)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<OverlayTypeClass>::AddRef (0x006952E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<OverlayTypeClass>::Release (0x00695300); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<OverlayTypeClass>::LockServer (0x006953B0); body: shared/class_factory.hpp
// 006953e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<SmudgeTypeClass>::QueryInterface(GUID const &iid, void **object) (0x006953F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<SmudgeTypeClass>::AddRef (0x00695460); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<SmudgeTypeClass>::Release (0x00695480); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<SmudgeTypeClass>::LockServer (0x00695530); body: shared/class_factory.hpp
// 00695560: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<UnitClass>::QueryInterface(GUID const &iid, void **object) (0x00695570)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<UnitClass>::AddRef (0x006955E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<UnitClass>::Release (0x00695600); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<UnitClass>::LockServer (0x006956B0); body: shared/class_factory.hpp
// 006956e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BuildingClass>::QueryInterface(GUID const &iid, void **object) (0x006956F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BuildingClass>::AddRef (0x00695760); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BuildingClass>::Release (0x00695780); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BuildingClass>::LockServer (0x00695830); body: shared/class_factory.hpp
// 00695860: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AircraftClass>::QueryInterface(GUID const &iid, void **object) (0x00695870)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AircraftClass>::AddRef (0x006958E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AircraftClass>::Release (0x00695900); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AircraftClass>::LockServer (0x006959B0); body: shared/class_factory.hpp
// 006959e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<InfantryClass>::QueryInterface(GUID const &iid, void **object) (0x006959F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<InfantryClass>::AddRef (0x00695A60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<InfantryClass>::Release (0x00695A80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<InfantryClass>::LockServer (0x00695B30); body: shared/class_factory.hpp
// 00695b60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AnimClass>::QueryInterface(GUID const &iid, void **object) (0x00695B70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AnimClass>::AddRef (0x00695BE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AnimClass>::Release (0x00695C00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AnimClass>::LockServer (0x00695CB0); body: shared/class_factory.hpp
// 00695ce0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AnimTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00695CF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AnimTypeClass>::AddRef (0x00695D60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AnimTypeClass>::Release (0x00695D80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AnimTypeClass>::LockServer (0x00695E30); body: shared/class_factory.hpp
// 00695e60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<HouseTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00695E70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<HouseTypeClass>::AddRef (0x00695EE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<HouseTypeClass>::Release (0x00695F00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<HouseTypeClass>::LockServer (0x00695FB0); body: shared/class_factory.hpp
// 00695fe0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<HouseClass>::QueryInterface(GUID const &iid, void **object) (0x00695FF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<HouseClass>::AddRef (0x00696060); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<HouseClass>::Release (0x00696080); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<HouseClass>::LockServer (0x00696130); body: shared/class_factory.hpp
// 00696160: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<DriveLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696170)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<DriveLocomotionClass>::AddRef (0x006961E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<DriveLocomotionClass>::Release (0x00696200); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<DriveLocomotionClass>::LockServer (0x006962B0); body: shared/class_factory.hpp
// 006962e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<JumpjetLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x006962F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<JumpjetLocomotionClass>::AddRef (0x00696360); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<JumpjetLocomotionClass>::Release (0x00696380); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<JumpjetLocomotionClass>::LockServer (0x00696430); body: shared/class_factory.hpp
// 00696460: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<HoverLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696470)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<HoverLocomotionClass>::AddRef (0x006964E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<HoverLocomotionClass>::Release (0x00696500); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<HoverLocomotionClass>::LockServer (0x006965B0); body: shared/class_factory.hpp
// 006965e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<RocketLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x006965F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<RocketLocomotionClass>::AddRef (0x00696660); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<RocketLocomotionClass>::Release (0x00696680); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<RocketLocomotionClass>::LockServer (0x00696730); body: shared/class_factory.hpp
// 00696760: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TunnelLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696770)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TunnelLocomotionClass>::AddRef (0x006967E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TunnelLocomotionClass>::Release (0x00696800); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TunnelLocomotionClass>::LockServer (0x006968B0); body: shared/class_factory.hpp
// 006968e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<WalkLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x006968F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<WalkLocomotionClass>::AddRef (0x00696960); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<WalkLocomotionClass>::Release (0x00696980); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<WalkLocomotionClass>::LockServer (0x00696A30); body: shared/class_factory.hpp
// 00696a60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<DropPodLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696A70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<DropPodLocomotionClass>::AddRef (0x00696AE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<DropPodLocomotionClass>::Release (0x00696B00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<DropPodLocomotionClass>::LockServer (0x00696BB0); body: shared/class_factory.hpp
// 00696be0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<FlyLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696BF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<FlyLocomotionClass>::AddRef (0x00696C60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<FlyLocomotionClass>::Release (0x00696C80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<FlyLocomotionClass>::LockServer (0x00696D30); body: shared/class_factory.hpp
// 00696d60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<TeleportLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696D70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<TeleportLocomotionClass>::AddRef (0x00696DE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<TeleportLocomotionClass>::Release (0x00696E00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<TeleportLocomotionClass>::LockServer (0x00696EB0); body: shared/class_factory.hpp
// 00696ee0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<MechLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00696EF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<MechLocomotionClass>::AddRef (0x00696F60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<MechLocomotionClass>::Release (0x00696F80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<MechLocomotionClass>::LockServer (0x00697030); body: shared/class_factory.hpp
// 00697060: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ShipLocomotionClass>::QueryInterface(GUID const &iid, void **object) (0x00697070)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ShipLocomotionClass>::AddRef (0x006970E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ShipLocomotionClass>::Release (0x00697100); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ShipLocomotionClass>::LockServer (0x006971B0); body: shared/class_factory.hpp
// 006971e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<BulletClass>::QueryInterface(GUID const &iid, void **object) (0x006971F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<BulletClass>::AddRef (0x00697260); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<BulletClass>::Release (0x00697280); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<BulletClass>::LockServer (0x00697330); body: shared/class_factory.hpp
// 00697360: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<FactoryClass>::QueryInterface(GUID const &iid, void **object) (0x00697370)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<FactoryClass>::AddRef (0x006973E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<FactoryClass>::Release (0x00697400); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<FactoryClass>::LockServer (0x006974B0); body: shared/class_factory.hpp
// 006974e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<WarheadTypeClass>::QueryInterface(GUID const &iid, void **object) (0x006974F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<WarheadTypeClass>::AddRef (0x00697560); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<WarheadTypeClass>::Release (0x00697580); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<WarheadTypeClass>::LockServer (0x00697630); body: shared/class_factory.hpp
// 00697660: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<WeaponTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00697670)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<WeaponTypeClass>::AddRef (0x006976E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<WeaponTypeClass>::Release (0x00697700); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<WeaponTypeClass>::LockServer (0x006977B0); body: shared/class_factory.hpp
// 006977e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ParticleClass>::QueryInterface(GUID const &iid, void **object) (0x006977F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ParticleClass>::AddRef (0x00697860); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ParticleClass>::Release (0x00697880); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ParticleClass>::LockServer (0x00697930); body: shared/class_factory.hpp
// 00697960: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ParticleTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00697970)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ParticleTypeClass>::AddRef (0x006979E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ParticleTypeClass>::Release (0x00697A00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ParticleTypeClass>::LockServer (0x00697AB0); body: shared/class_factory.hpp
// 00697ae0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ParticleSystemClass>::QueryInterface(GUID const &iid, void **object) (0x00697AF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ParticleSystemClass>::AddRef (0x00697B60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ParticleSystemClass>::Release (0x00697B80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ParticleSystemClass>::LockServer (0x00697C30); body: shared/class_factory.hpp
// 00697c60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<ParticleSystemTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00697C70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<ParticleSystemTypeClass>::AddRef (0x00697CE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<ParticleSystemTypeClass>::Release (0x00697D00); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<ParticleSystemTypeClass>::LockServer (0x00697DB0); body: shared/class_factory.hpp
// 00697de0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AITriggerTypeClass>::QueryInterface(GUID const &iid, void **object) (0x00697DF0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AITriggerTypeClass>::AddRef (0x00697E60); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AITriggerTypeClass>::Release (0x00697E80); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AITriggerTypeClass>::LockServer (0x00697F30); body: shared/class_factory.hpp
// 00697f60: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<NeuronClass>::QueryInterface(GUID const &iid, void **object) (0x00697F70)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<NeuronClass>::AddRef (0x00697FE0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<NeuronClass>::Release (0x00698000); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<NeuronClass>::LockServer (0x006980B0); body: shared/class_factory.hpp
// 006980e0: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<FoggedObjectClass>::QueryInterface(GUID const &iid, void **object) (0x006980F0)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<FoggedObjectClass>::AddRef (0x00698160); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<FoggedObjectClass>::Release (0x00698180); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<FoggedObjectClass>::LockServer (0x00698230); body: shared/class_factory.hpp
// 00698260: TClassFactory<T>::TClassFactory, emitted by this unit's use of TClassFactory<T> (the constructor)
// HRESULT __stdcall TClassFactory<AlphaShapeClass>::QueryInterface(GUID const &iid, void **object) (0x00698270)
// IUnknown and IClassFactory give the factory itself (body: shared/class_factory.hpp).
// unsigned long __stdcall TClassFactory<AlphaShapeClass>::AddRef (0x006982E0); body: shared/class_factory.hpp
// unsigned long __stdcall TClassFactory<AlphaShapeClass>::Release (0x00698300); body: shared/class_factory.hpp
// HRESULT __stdcall TClassFactory<AlphaShapeClass>::LockServer (0x006983B0); body: shared/class_factory.hpp


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<unsigned long>::operator equal (0x006983F0; VectorClass<unsigned long> vtable 0x007ab9c0 slot 1).
// VectorClass<unsigned long>::Resize (0x00698430; VectorClass<unsigned long> vtable 0x007ab9c0 slot 2).
// VectorClass<unsigned long>::Clear (0x006984E0; VectorClass<unsigned long> vtable 0x007ab9c0 slot 3).
// VectorClass<unsigned long>::ID_object (0x00698510; VectorClass<unsigned long> vtable 0x007ab9c0 slot 4).
// VectorClass<unsigned long>::ID_pointer (0x00698550; VectorClass<unsigned long> vtable 0x007ab9c0 slot 5).
// DynamicVectorClass<unsigned long>::Resize (0x00698570; DynamicVectorClass<unsigned long> vtable 0x007ab9a0 slot 2).
// Original00698620: appends CoRegisterClassObject registration cookies.
// DynamicVectorClass<unsigned long>::ID_object (0x00698670; DynamicVectorClass<unsigned long> vtable 0x007ab9a0 slot 4).


extern "C" unsigned char vtable_6986a0[]; extern "C" void __cdecl release_buffer_6986a0(void*); extern "C" void __cdecl release_object_6986a0(void*);
class Leaf_6986a0 { public: void* get(unsigned int flags); };
void* Leaf_6986a0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6986a0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6986a0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6986a0(this); return this; }


extern "C" unsigned char vtable_6986f0[]; extern "C" void __cdecl release_buffer_6986f0(void*); extern "C" void __cdecl release_object_6986f0(void*);
class Leaf_6986f0 { public: void* get(unsigned int flags); };
void* Leaf_6986f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6986f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6986f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6986f0(this); return this; }
