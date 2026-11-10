// int __stdcall WinMain(HINSTANCE instance, HINSTANCE, char *command_line, int command_show) (0x0068E280)
// Scout1146 (Scout1144 r1 + TibSun REGISTER_CLASS block; lineage Startup226/232, Scout1130-1144). Flags: see the round README.
#include "shared/iunknown_ptr.hpp"
#include "shared/debug_printf.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
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
// @ref ?main_thread_00b25f78@@3KA=00b25f78
// @ref ??_H@YGXPAXIHP6EX0@Z@Z=0040aed0
// @ref ??_I@YGXPAXIHP6EX0@Z@Z=0040af00
// @ref ??0INIClass@@QAE@XZ=0051ad50
// @ref ??_GGenericNode@@UAEPAXI@Z=0040e1d0
// @ref ??_GGenericList@@UAEPAXI@Z=0040e220
// @ref ??_GINIClass@@UAEPAXI@Z=004deef0
// @ref ??_G?$List@PAUINISection@INIClass@@@@UAEPAXI@Z=0040e300
// @ref ?Add@?$DynamicVectorClass@K@@QAE_NABK@Z=00698620
// @ref ??_7INIClass@@6B@=007a2b34
// @ref ??_7?$List@PAUINISection@INIClass@@@@6B@=0079aaec
// @ref ??_7GenericList@@6B@=0079aaf4
// @ref ??_7GenericNode@@6B@=0079aafc
// @ref ??_C@_01FCOA@?5?$AA@=007e613c
// @ref ??_C@_01PJCK@?4?$AA@=007dbe44
// @ref ??_C@_02JDLK@?$CFx?$AA@=007db4b4
// @ref ??_C@_04NHBG@Play?$AA@=007db8c0
// @ref ??_C@_05HAHE@Video?$AA@=007e8860
// @ref ??_C@_05MDNJ@Intro?$AA@=007d9d98
// @ref ??_C@_06BHPO@Socket?$AA@=007e871c
// @ref ??_C@_07HKHC@Network?$AA@=007e8760
// @ref ??_C@_07IBND@RA2?4INI?$AA@=007db8b8
// @ref ??_C@_07OPOK@DestNet?$AA@=007e870c
// @ref ??_C@_0BA@EOHF@AllowModeToggle?$AA@=007e87dc
// @ref ??_C@_0BA@GICH@TXT_SHORT_TITLE?$AA@=007d33cc
// @ref ??_C@_0M@ECB@Red?5Alert?52?$AA@=007f4f84
// @ref ??_C@_0M@KHJK@ScreenWidth?$AA@=007e8868
// @ref ??_C@_0N@PIKM@ScreenHeight?$AA@=007e8850
// @ref ?Main_Game@@YIHHQAPAD@Z=00481bf0
// @ref ?Handle_Exception@@YIHIPAU_EXCEPTION_POINTERS@@@Z=004b87f0
// @ref ??_C@_0L@IPGI@Main_Game?6?$AA@=007f4e34
// @ref ?CopyProtection_Check_Already_Running_004918f0@@YA_NXZ=004918f0
// @ref ?Query_Free_Disk_Kilobytes_00482c80@@YAIXZ=00482c80
// @ref ?Wide_Message_Box@@YIHPAUHWND__@@PBG1I@Z=006fa040
// @ref ?Parse_Command_Line@@YI_NHQAPAD@Z=00515110
// @ref ?Set_FPU_Control@@YAXII@Z=00785049
// @ref ?Unknown_0077F0F4@@YAXXZ=0077f0f4
// @ref _video_back_buffer_7f4cc4=007f4cc4
// @ref ?unknown_screen_width_a40b38@@3HA=00a40b38
// @ref ?unknown_screen_height_a40b3c@@3HA=00a40b3c
// @ref ??_C@_0BA@KCAE@VideoBackBuffer?$AA@=007f5060
// @ref ??_C@_0BD@CMK@TXT_CRITICALLY_LOW?$AA@=007f5038
// @ref ??_C@_0BD@GILP@D?3?2Ra2?2Startup?4CPP?$AA@=007f504c
// @ref ??_C@_0BH@EFMA@Create?5AppMutex?5okay?4?6?$AA@=007f5110
// @ref ??_C@_0BH@HKLN@Create?5AutoPlayMutex?4?6?$AA@=007f508c
// @ref ??_C@_0BJ@HBMF@Got?5AutoPlayMutex?5okay?4?6?$AA@=007f5070
// @ref ??_C@_0BP@OILI@Waiting?5for?5Autoplay?5to?5quit?$CB?6?$AA@=007f50c8
// @ref ??_C@_0BP@PKEA@Launcher?5not?5running?4?4?4?4Bail?$CB?6?$AA@=007f5174
// @ref ??_C@_0CC@DDHA@Failed?5waiting?5for?5AutoPlayMutex@=007f50a4
// @ref ??_C@_0CD@GIPM@TibSun?5is?5already?5running?4?4?4Bail@=007f5128
// @ref ??_C@_0CF@CAEB@29e3bb2a?92f36?911d3?9a72c?90090272f@=007f514c
// @ref ??_C@_0CF@GGBC@01AF9993?93492?911d3?98F6F?90060089C@=007f50e8
// @ref ??_C@_06LGNF@?$CFd?9?$CFd?6?$AA@=007f5194

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
#include "shared/com_registration.hpp"
#include "shared/class_factory.hpp"
#include "shared/cd.hpp"
#include "shared/ram_file_class.hpp"
#include "shared/wwmouse.hpp"
#include "shared/exit.hpp"
#include "shared/pkey.hpp"
class MouseClass;
extern MouseClass Map;
// @ref $T=007ab990
// @ref ??_C@_0BA@POEB@TXT_DLL_INVALID?$AA@=007f4e88
// @ref ??_C@_0BC@CHMN@Init_Keys?5?9?5Load?6?$AA@=007f4fc4
// @ref ??_C@_0BH@NGDK@Init?5Encryption?5Keys?4?6?$AA@=007f4ff4
// @ref ??_C@_0BK@CLJH@Init_Keys?5?9?5declarations?6?$AA@=007f4fd8
// @ref ??_C@_0BL@HKMO@Init_Keys?5?9?5Init?5fast?5key?6?$AA@=007f4fa8
// @ref ??_C@_0N@FDEG@comctl32?4dll?$AA@=007f4e98
// @ref ??_C@_0N@FKEK@LANGUAGE?4MIX?$AA@=007f4f98
// @ref __imp__RegisterActiveObject@16=0079a364
// @ref __except_handler3=007852d4
// @ref ??_C@_0L@HOJK@Main_Game?6?$AA@=007f4e34
// @ref _exit=00784edc
// @ref _atexit=0078298a
// @ref ?Load_String_File@@YIHPBD@Z=006f8e30
// @ref ?Unknown_00495590@@YI_N_N@Z=00495590
// @ref ?Query_DLL_Version@@YIIPBD@Z=0049ee00
// @ref _Keys=0079aa70
// @ref _unknown_string_7f4ea8=007f4ea8
// @ref ??0IUnknownPtr@@QAE@XZ=006983e0
// @ref ??1IUnknownPtr@@QAE@XZ=006983e0
// @ref _register_dlls_7ab988=007ab988
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
// @ref ??_C@_07MHCP@ra2?4str?$AA@=007f4f90
// @ref ??_C@_0BA@BALO@TXT_INVALIDMODE?$AA@=007f4e5c
// @ref ??_C@_0BA@POIP@TXT_VIDEO_ERROR?$AA@=007f4e6c
// @ref ??_C@_0BC@GLC@DllRegisterServer?$AA@=007f5024
// @ref ??_C@_0BD@JFP@TXT_MOUSE_REQUIRED?$AA@=007f4e20
// @ref ??_C@_0BG@FPEE@TXT_PREPARECOM_FAILED?$AA@=007f500c
// @ref ??_C@_0BL@EIFG@Failed?5to?5notify?5launcher?6?$AA@=007f4e40
// @ref ??_C@_0M@KFPM@session?4ipb?$AA@=007f4e7c
// @ref ?Audio_File_Open_Stream@@YIXPBD@Z=00522cb0
// @ref ?Audio_File_Record@@YIXPBD@Z=00522c50
// @ref ?unknown_audio_mode_a41664@@3HA=00a41664
// @ref ?unknown_depth_8523e8@@3HA=008523e8
// @ref ?unknown_no_exception_handler_a4168c@@3_NA=00a4168c
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
// @ref ?TibSunFactory@@3VTiberianSunClassFactory@@A=00a4088c

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
        AlphaBuffer = new ABufferSwapTmp(Rect(unknown_int_839630, unknown_int_839634, 480, 480 - unknown_int_839634));

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
    Map.unknown_slot50();
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
