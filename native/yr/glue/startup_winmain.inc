
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y2): WinMain, YR 0x006BB9A0 (10,261 bytes, unmatched in ra2-decomp).
// Appended to the generated startup unit (native/yr/fixups.json "append"), so it sees that unit's
// includes and declarations. Written from the RA2 tree's WinMain (src/recovered/startup.cpp,
// 0x0068E280, TibSun startup.cpp's shape) and gamemd.exe 1.001's own code at 0x006BB9A0 read
// instruction by instruction for YR's changes: RA2MD.INI, the 77 class factories registered in
// line, Init_Keys through a CCINIClass, LANGMD.MIX before LANGUAGE.MIX, ra2md.str, the shell size
// (Options.ShellWidth/Height) for the window, the audio recorder (session.ipb), the Direct3D probe
// and the WOL options' movie flag. The debug prints of type sizes at the start are left out.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
// startup.cpp declares StringTable as a namespace (its Unload); the class's statics mangle the same
// (units that include windows.h first call LoadString as LoadStringA: native/yr/glue/uicmnds2_native.inc).
namespace StringTable {
wchar_t const *LoadString(char const *label, char **extra, char const *file, int line);
int LoadFile(char const *filename);
}
void FUN_007B84F0(void);
bool FUN_004A3BF0(void);
int __fastcall FUN_00734FE0(HWND window, wchar_t const *text, wchar_t const *caption, UINT type);   // message box
extern int DAT_00A8F784;                          // 0x00A8F784 the audio recorder's mode (1 record, 2 play)
int __fastcall FUN_0053DE00(int count, int size);
int __fastcall FUN_0053E4F0(char const *name);
int __fastcall FUN_0053E550(char const *name);
void __fastcall FUN_005BC180(HINSTANCE instance, HWND window, bool enabled);
void __fastcall FUN_00637070(void);
void FUN_0052CB80(void);
int FUN_004C1600(void);
int FUN_004C16A0(void);
void FUN_004C19B0(void);
int FUN_005355D0(void);
void FUN_005BC300(void);
void FUN_005F77F0(void);
void FUN_0060D440(void);
void __fastcall FUN_006370A0(void);
void FUN_0072AC40(void);
void FUN_00730970(void);
extern bool DAT_008A0DF0;
extern int SidebarWidth;                          // 0x007F5BF8
void Free_Game_Modes(void);
void SchemeLibrary_Destroy(void);
void Free_Mouse_Shapes(void);
void Net2Shutdown_Network(void);
// The mouse (0x00887640): gamemd.exe's WWMouseClass (wwmous.h; this unit sees TibSun's), made and
// hidden through native/yr/glue/wwmous_native.inc in the wwmous unit.
VirtualObject *yr_new_wwmouse(DSurface *surface, HWND window);
void yr_wwmouse_hide(VirtualObject *mouse);
#include "revent.h"
#include "ldprgmgr.h"
#include "wolopt.h"
#include "ramfile.h"
#include "_pk.h"
namespace CopyProtection {
bool IsLauncherRunning(void);
bool NotifyLauncher(void);
}
extern "C" const CLSID CLSID_AirstrikeClass;
extern "C" const CLSID CLSID_SpawnManagerClass;
extern "C" const CLSID CLSID_SlaveManagerClass;
extern "C" const CLSID CLSID_CaptureManagerClass;
extern const CLSID CLSID_DiskLaserClass;
extern "C" const CLSID CLSID_BombClass;
extern bool DAT_00840A6C;                         // 0x00840A6C [Video] VideoBackBuffer
extern int DAT_00A8D5FC;                          // 0x00A8D5FC a destination network was read
extern IPXAddressClass DAT_00A8D600;              // 0x00A8D600 [Network] DestNet's address
extern int DAT_00A8E950;                          // 0x00A8E950 CDList's drive count
extern int DAT_00A8E954;                          // 0x00A8E954 CDList's next index
extern int DAT_00A8E8E8;                          // 0x00A8E8E8 CDList's first drive
extern unsigned char DAT_00A8F7AC;                // 0x00A8F7AC no exception handler around Main_Game
extern char DAT_007E1A80[];                       // 0x007E1A80 the encryption keys' INI text
extern char DAT_00840C58[];                       // 0x00840C58 "ra2md.str" failure message
extern const void *DAT_007F3710[2];               // 0x007F3710 the COM server DLL: its CLSID, its file name
extern DynamicVectorClass<unsigned long> RegisteredClasses;   // 0x00B0BC88
extern unsigned long dwRegisterMap;               // 0x00840A74
extern unsigned long DAT_00B0BCEC;                // 0x00B0BCEC
extern unsigned char DAT_00887625;                // 0x00887625 WinVersion's Win9x flag
extern int DAT_00887598;                          // 0x00887598 WinVersion's major version
extern "C" unsigned int _controlfp(unsigned int value, unsigned int mask);
extern "C" HRESULT __stdcall RegisterActiveObject(IUnknown *unknown, const GUID &clsid, DWORD flags, DWORD *registration);
extern "C" HRESULT __stdcall RevokeActiveObject(DWORD registration, void *reserved);
void Prog_End(void);
int Exception_Handler(int exception_code, EXCEPTION_POINTERS *e_info);

// Why WinMain stopped early, on the native log (the original shows a message box or nothing).
static int yr_winmain_stop(int code, const char *why)
{
	fprintf(stderr, "yr-native: WinMain stops (%s)\n", why);
	return code;
}

#define YR_REGISTER_CLASS(_class, _clsid) \
	{ \
		IClassFactory *factory = new TClassFactory<_class>; \
		CoRegisterClassObject(_clsid, factory, 1, 1, &registration); \
		RegisteredClasses.Add(registration); \
	}

extern "C" int __stdcall WinMain(HINSTANCE instance, HINSTANCE, char *command_line, int command_show)
{
	char *argv[20];
	char path_to_exe[132];
	wchar_t buffer[512];

	ProgramInstance = instance;
	MainThread = (HANDLE)(uintptr_t)GetCurrentThreadId();

	if (!CopyProtection::IsLauncherRunning()) {
		return yr_winmain_stop(0, "the launcher is not running");
	}

	Unsorted::AppMutex = CreateMutexA(0, 0, "29e3bb2a-2f36-11d3-a72c-0090272fa661");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		HWND window = FindWindowA("29e3bb2a-2f36-11d3-a72c-0090272fa661", 0);
		if (window != 0) {
			SetForegroundWindow(window);
			ShowWindow(window, SW_RESTORE);
		}
		if (Unsorted::AppMutex != 0) {
			CloseHandle(Unsorted::AppMutex);
			Unsorted::AppMutex = 0;
		}
		return yr_winmain_stop(0, "another copy is running");
	}

	do {
		DAT_00B0BCE8 = OpenMutexA(0x1F0001, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
		if (DAT_00B0BCE8 != 0) {
			if (WaitForSingleObject(DAT_00B0BCE8, 30000) == WAIT_FAILED) {
				CloseHandle(DAT_00B0BCE8);
				DAT_00B0BCE8 = 0;
			}
		}
		if (DAT_00B0BCE8 == 0) {
			DAT_00B0BCE8 = CreateMutexA(0, 0, "01AF9993-3492-11d3-8F6F-0060089C05B1");
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				CloseHandle(DAT_00B0BCE8);
				DAT_00B0BCE8 = 0;
				Sleep(2500);
			}
		}
	} while (DAT_00B0BCE8 == 0);

	_controlfp(0x300, 0x300);

	GetModuleFileNameA(instance, path_to_exe, sizeof(path_to_exe));
	int argc = 1;
	argv[0] = path_to_exe;
	for (char *token = strtok(command_line, " "); token != 0 && argc < 20; token = strtok(0, " ")) {
		argv[argc++] = strtrim(token);
	}
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char path[_MAX_PATH];
	_splitpath(argv[0], drive, dir, 0, 0);
	_makepath(path, drive, dir, 0, 0);
	SetCurrentDirectoryA(path);

	int retval = 1;
	if (Parse_Command_Line(argc, argv)) {
		RawFileClass *cfile = new RawFileClass("RA2MD.INI");
		INI_RA2MD.ReadCCFile(*cfile, false, false);
		DAT_00840A6C = INI_RA2MD.Get_Bool("Video", "VideoBackBuffer", true);
		Debug_IngameModeChange = INI_RA2MD.Get_Bool("Video", "AllowModeToggle", false);
		Options.ScreenWidth = INI_RA2MD.Get_Int("Video", "ScreenWidth", Options.ScreenWidth);
		Options.ScreenHeight = INI_RA2MD.Get_Int("Video", "ScreenHeight", Options.ScreenHeight);
		int socket = INI_RA2MD.Get_Int("Network", "Socket", 0);
		if (socket > 0) {
			socket += 0x4000;
			if (socket >= 0x4000 && socket < 0x8000) {
				Ipx.Set_Socket((unsigned short)socket);
			}
		}

		// [Network] DestNet: the network number (and node) of a destination, dotted hex.
		{
			char netbuf[512];
			memset(netbuf, 0, sizeof(netbuf));
			bool found = INI_RA2MD.Get_String("Network", "DestNet", 0, netbuf, sizeof(netbuf)) != 0;
			if (found && strlen(netbuf)) {
				NetNumType net;
				NetNodeType node;
				int i = 0;
				for (char *p = strtok(netbuf, "."); p != 0; p = strtok(0, ".")) {
					int x;
					sscanf(p, "%x", &x);
					if (i < 4) {
						net[i] = (char)x;
					} else {
						node[i - 4] = (char)x;
					}
					i++;
				}
				if (i >= 4) {
					DAT_00A8D5FC = 1;
					memset(node, 0xFF, 6);
					DAT_00A8D600 = IPXAddressClass(net, node, 0);
				}
			}
		}

		Keyboard = new KeyboardClass();

		if (Disk_Space_Available() < 0x2000) {
			swprintf(buffer, StringTable::LoadString("TXT_CRITICALLY_LOW", 0, "D:\\ra2mdpost\\Startup.CPP", 0x78b), 0x2000);
			if (FUN_00734FE0(0, buffer, StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x78c), 0x24) == IDNO) {
				OleUninitialize();
				return yr_winmain_stop(1, "the disk is critically low");
			}
		}
		OleInitialize(0);

		// The COM server DLL (Blowfish): registered from its file when it cannot be made.
		bool failed = false;
		{
			IUnknownPtr ptr;
			const GUID &clsid = *(const GUID *)DAT_007F3710[0];
			failed = FAILED(ptr.CreateInstance(clsid, 0, 7));
			if (failed) {
				failed = false;
				HMODULE module = LoadLibraryA((const char *)DAT_007F3710[1]);
				if (module != 0) {
					typedef HRESULT (__stdcall *DllRegister)(void);
					DllRegister reg = (DllRegister)GetProcAddress(module, "DllRegisterServer");
					if (!reg || (reg(), FAILED(ptr.CreateInstance(clsid, 0, 7)))) {
						failed = true;
					}
					FreeLibrary(module);
				} else {
					failed = true;
				}
			}
		}
		RegisterServer();
		unsigned long registration;
		CoRegisterClassObject(CLSID_TiberianSun, (IUnknown *)&TibSunFactory, 1, 1, &registration);
		RegisteredClasses.Add(registration);
		YR_REGISTER_CLASS(CStreamClass, CLSID_CompressStream)
		YR_REGISTER_CLASS(WaveClass, CLSID_WaveClass)
		YR_REGISTER_CLASS(TerrainTypeClass, CLSID_TerrainTypeClass)
		YR_REGISTER_CLASS(TerrainClass, CLSID_TerrainClass)
		YR_REGISTER_CLASS(SuperWeaponTypeClass, CLSID_SuperWeaponTypeClass)
		YR_REGISTER_CLASS(SuperClass, CLSID_SuperWeaponClass)
		YR_REGISTER_CLASS(Tactical, CLSID_TacticalMapClass)
		YR_REGISTER_CLASS(CellClass, CLSID_CellClass)
		YR_REGISTER_CLASS(EMPulseClass, CLSID_EMPulseClass)
		YR_REGISTER_CLASS(LightSourceClass, CLSID_LightSource)
		YR_REGISTER_CLASS(SideClass, CLSID_SideClass)
		YR_REGISTER_CLASS(TiberiumClass, CLSID_TiberiumClass)
		YR_REGISTER_CLASS(TubeClass, CLSID_TubeClass)
		YR_REGISTER_CLASS(CampaignClass, CLSID_CampaignClass)
		YR_REGISTER_CLASS(BuildingLightClass, CLSID_BuildingLightClass)
		YR_REGISTER_CLASS(WaypointPathClass, CLSID_WaypointPath)
		YR_REGISTER_CLASS(TemporalClass, CLSID_TemporalClass)
		YR_REGISTER_CLASS(AirstrikeClass, CLSID_AirstrikeClass)
		YR_REGISTER_CLASS(SpawnManagerClass, CLSID_SpawnManagerClass)
		YR_REGISTER_CLASS(SlaveManagerClass, CLSID_SlaveManagerClass)
		YR_REGISTER_CLASS(CaptureManagerClass, CLSID_CaptureManagerClass)
		YR_REGISTER_CLASS(DiskLaserClass, CLSID_DiskLaserClass)
		YR_REGISTER_CLASS(ParasiteClass, CLSID_ParasiteClass)
		YR_REGISTER_CLASS(BombClass, CLSID_BombClass)
		YR_REGISTER_CLASS(RadSiteClass, CLSID_RadSiteClass)
		YR_REGISTER_CLASS(TEventClass, CLSID_EventClass)
		YR_REGISTER_CLASS(VoxelAnimTypeClass, CLSID_VoxelAnimTypeClass)
		YR_REGISTER_CLASS(VoxelAnimClass, CLSID_VoxelAnimClass)
		YR_REGISTER_CLASS(TActionClass, CLSID_ActionClass)
		YR_REGISTER_CLASS(TriggerClass, CLSID_TriggerClass)
		YR_REGISTER_CLASS(TriggerTypeClass, CLSID_TriggerTypeClass)
		YR_REGISTER_CLASS(ScriptClass, CLSID_ScriptClass)
		YR_REGISTER_CLASS(ScriptTypeClass, CLSID_ScriptTypeClass)
		YR_REGISTER_CLASS(TagClass, CLSID_TagClass)
		YR_REGISTER_CLASS(TagTypeClass, CLSID_TagTypeClass)
		YR_REGISTER_CLASS(TeamClass, CLSID_TeamClass)
		YR_REGISTER_CLASS(TeamTypeClass, CLSID_TeamTypeClass)
		YR_REGISTER_CLASS(TaskForceClass, CLSID_TaskForceClass)
		YR_REGISTER_CLASS(UnitTypeClass, CLSID_UnitTypeClass)
		YR_REGISTER_CLASS(BuildingTypeClass, CLSID_BuildingTypeClass)
		YR_REGISTER_CLASS(AircraftTypeClass, CLSID_AircraftTypeClass)
		YR_REGISTER_CLASS(InfantryTypeClass, CLSID_InfantryTypeClass)
		YR_REGISTER_CLASS(BulletTypeClass, CLSID_BulletTypeClass)
		YR_REGISTER_CLASS(IsometricTileTypeClass, CLSID_IsometricTileTypeClass)
		YR_REGISTER_CLASS(OverlayTypeClass, CLSID_OverlayTypeClass)
		YR_REGISTER_CLASS(SmudgeTypeClass, CLSID_SmudgeTypeClass)
		YR_REGISTER_CLASS(UnitClass, CLSID_UnitClass)
		YR_REGISTER_CLASS(BuildingClass, CLSID_BuildingClass)
		YR_REGISTER_CLASS(AircraftClass, CLSID_AircraftClass)
		YR_REGISTER_CLASS(InfantryClass, CLSID_InfantryClass)
		YR_REGISTER_CLASS(AnimClass, CLSID_AnimClass)
		YR_REGISTER_CLASS(AnimTypeClass, CLSID_AnimTypeClass)
		YR_REGISTER_CLASS(HouseTypeClass, CLSID_HouseTypeClass)
		YR_REGISTER_CLASS(HouseClass, CLSID_HouseClass)
		YR_REGISTER_CLASS(DriveLocomotionClass, CLSID_DriveLocomotion)
		YR_REGISTER_CLASS(JumpjetLocomotionClass, CLSID_JumpjetLocomotion)
		YR_REGISTER_CLASS(HoverLocomotionClass, LocomotionClass::CLSIDs::Hover)
		YR_REGISTER_CLASS(RocketLocomotionClass, LocomotionClass::CLSIDs::Rocket)
		YR_REGISTER_CLASS(TunnelLocomotionClass, CLSID_TunnelLocomotion)
		YR_REGISTER_CLASS(WalkLocomotionClass, CLSID_WalkLocomotion)
		YR_REGISTER_CLASS(DropPodLocomotionClass, LocomotionClass::CLSIDs::Droppod)
		YR_REGISTER_CLASS(FlyLocomotionClass, CLSID_FlyerLocomotion)
		YR_REGISTER_CLASS(TeleportLocomotionClass, LocomotionClass::CLSIDs::Teleport)
		YR_REGISTER_CLASS(MechLocomotionClass, CLSID_MechLocomotion)
		YR_REGISTER_CLASS(ShipLocomotionClass, CLSID_ShipLocomotion)
		YR_REGISTER_CLASS(BulletClass, CLSID_BulletClass)
		YR_REGISTER_CLASS(FactoryClass, CLSID_FactoryClass)
		YR_REGISTER_CLASS(WarheadTypeClass, CLSID_WarheadTypeClass)
		YR_REGISTER_CLASS(WeaponTypeClass, CLSID_WeaponTypeClass)
		YR_REGISTER_CLASS(ParticleClass, CLSID_ParticleClass)
		YR_REGISTER_CLASS(ParticleTypeClass, CLSID_ParticleTypeClass)
		YR_REGISTER_CLASS(ParticleSystemClass, CLSID_ParticleSystemClass)
		YR_REGISTER_CLASS(ParticleSystemTypeClass, CLSID_ParticleSystemTypeClass)
		YR_REGISTER_CLASS(AITriggerTypeClass, CLSID_AITriggerTypeClass)
		YR_REGISTER_CLASS(NeuronClass, CLSID_NeuronClass)
		YR_REGISTER_CLASS(FoggedObjectClass, CLSID_FoggedObjectClass)
		YR_REGISTER_CLASS(AlphaShapeClass, CLSID_AlphaShapeClass)
		RegisterActiveObject((IUnknown *)&ApplicationInstance, CLSID_TiberianSun, 1, &DAT_00B0BCEC);
		if (failed) {
			FUN_00734FE0(0, StringTable::LoadString("TXT_PREPARECOM_FAILED", 0, "D:\\ra2mdpost\\Startup.CPP", 0x652),
				StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x652), 0x30);
			exit(1);
		}

		// Init_Keys.
		{
			RAMFileClass file((void *)DAT_007E1A80, strlen(DAT_007E1A80));
			CCINIClass ini;
			ini.INIClass::Load(file, false);
			FastKey = ini.Get_PKey(true);
		}

		// The language files (Init_Language), with no CD required.
		{
			DiskID cur_cd = CD::Get_Required_Disk();
			CD::Set_Required_Disk((DiskID)-2);
			DAT_00884E04 = (VirtualObject *)new MixFileClass("LANGMD.MIX", &FastKey);
			DAT_00884E00 = (VirtualObject *)new MixFileClass("LANGUAGE.MIX", &FastKey);
			CD::Set_Required_Disk(cur_cd);
		}
		atexit(Prog_End);
		FUN_007B84F0();

		if (!StringTable::LoadFile("ra2md.str")) {
			MessageBoxA(0, DAT_00840C58, "Red Alert 2", MB_ICONSTOP);
			return yr_winmain_stop(0, "ra2md.str did not load");
		}
		if (!FUN_004A3BF0()) {
			return yr_winmain_stop(0, "FUN_004A3BF0 (the game data check) failed");
		}
		if (GetSystemMetrics(SM_MOUSEPRESENT) == 0 || GetSystemMetrics(SM_CMOUSEBUTTONS) == 0) {
			FUN_00734FE0(0, StringTable::LoadString("TXT_MOUSE_REQUIRED", 0, "D:\\ra2mdpost\\Startup.CPP", 0x7cf),
				StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x7cf), 0x10);
			exit(1);
		}
		if (GetDllVersion("comctl32.dll") < 0x40046) {
			swprintf(buffer, StringTable::LoadString("TXT_DLL_INVALID", 0, "D:\\ra2mdpost\\Startup.CPP", 0x7d9), "comctl32.dll", 4, 0x46, "comctl32.dll");
			FUN_00734FE0(0, buffer, StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x7da), 0x10);
			exit(1);
		}
		RegisterActiveObject((IUnknown *)&Map, CLSID_TiberianSun, 1, &dwRegisterMap);

		if (Options.ScreenWidth == -1 || Options.ScreenHeight == -1) {
			bool win9x = DAT_00887625 && DAT_00887598 < 5;
			if (win9x || GetVideoMemory() <= 0x200000) {
				Options.ScreenWidth = 640;
				Options.ScreenHeight = 480;
			} else {
				Options.ScreenWidth = 800;
				Options.ScreenHeight = 600;
			}
		}

		bool video_success;
		if (Debug_IngameModeChange) {
			Create_Main_Window(instance, command_show, 640, 480);
			Prep_Direct_Draw();
			if (WindowedMode) {
				VisibleRect = Rect(0, 0, 640, 480);
				VideoModeWidth = 640;
				VideoModeHeight = 480;
				VideoModeBitsPerPixel = HicolorFlag ? 16 : 8;
			} else {
				video_success = Set_Video_Mode(MainWindow, 640, 480, HicolorFlag ? 16 : 8);
				VisibleRect = Rect(0, 0, 640, 480);
				if (!video_success) {
					Destroy_Direct_Draw();
					FUN_00734FE0(MainWindow, StringTable::LoadString("TXT_VIDEO_ERROR", 0, "D:\\ra2mdpost\\Startup.CPP", 0x86f),
						StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x870), 0x30);
					exit(1);
				}
			}
		} else {
			Create_Main_Window(instance, command_show, Options.ShellWidth, Options.ShellHeight);
			Prep_Direct_Draw();
			if (WindowedMode) {
				VisibleRect = Rect(0, 0, Options.ShellWidth, Options.ShellHeight);
				VideoModeWidth = Options.ShellWidth;
				VideoModeHeight = Options.ShellHeight;
				VideoModeBitsPerPixel = HicolorFlag ? 16 : 8;
			} else {
				video_success = Set_Video_Mode(MainWindow, Options.ShellWidth, Options.ShellHeight, HicolorFlag ? 16 : 8);
				VisibleRect = Rect(0, 0, Options.ShellWidth, Options.ShellHeight);
				if (!video_success) {
					if (Options.ScreenWidth != 640 || Options.ScreenHeight != 480) {
						Options.ScreenWidth = 640;
						Options.ScreenHeight = 480;
						video_success = Set_Video_Mode(MainWindow, 640, 480, HicolorFlag ? 16 : 8);
						VisibleRect = Rect(0, 0, Options.ScreenWidth, Options.ScreenHeight);
					}
					if (!video_success) {
						Destroy_Direct_Draw();
						FUN_00734FE0(MainWindow, StringTable::LoadString("TXT_VIDEO_ERROR", 0, "D:\\ra2mdpost\\Startup.CPP", 0x8a7),
							StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x8a8), 0x30);
						exit(1);
					}
				}
			}
		}

		// The audio recorder (session.ipb), when the command line asked for it.
		if (DAT_00A8F784 != 0 && FUN_0053DE00(1000, 0x19000)) {
			if (DAT_00A8F784 == 1) {
				FUN_0053E4F0("session.ipb");
			} else if (DAT_00A8F784 == 2) {
				FUN_0053E550("session.ipb");
			}
		}

		VisibleSurface = DSurface::Create_Primary(0);
		HicolorFlag = VisibleSurface->Bytes_Per_Pixel() == 2;
		if (!HicolorFlag) {
			FUN_00734FE0(MainWindow, StringTable::LoadString("TXT_INVALIDMODE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x8c5),
				StringTable::LoadString("TXT_SHORT_TITLE", 0, "D:\\ra2mdpost\\Startup.CPP", 0x8c5), 0x30);
			Emergency_Exit();
		}
		do {
			Windows_Message_Handler();
		} while (!GameInFocus);
		if (!CopyProtection::NotifyLauncher()) {
			exit(1);
		}
		do {
			Windows_Message_Handler();
		} while (!GameInFocus);

		SurfacesRestored = false;
		if (!WindowedMode) {
			VisibleSurface->Fill(0);
		}
		FUN_005BC180(ProgramInstance, MainWindow, WindowedMode);
		FUN_00637070();
		FUN_0052CB80();
		if (!DirectDrawWrap::DoSmth) {
			DAT_008A0DF0 = false;
		} else if (FUN_004C1600() != 0) {
			FUN_004C19B0();
			DirectDrawWrap::DoSmth = false;
			DAT_008A0DF0 = false;
		}
		{
			Rect side_rect(0, 0, SidebarWidth, VisibleRect.Height);
			Rect tile_rect(0, 0, VisibleRect.Width - SidebarWidth, VisibleRect.Height);
			Rect composite_rect(0, 0, VisibleRect.Width - SidebarWidth, VisibleRect.Height);
			Allocate_Surfaces(VisibleRect, composite_rect, tile_rect, side_rect, false);
		}
		if (DirectDrawWrap::DoSmth && FUN_004C16A0() != 0) {
			FUN_004C19B0();
			DirectDrawWrap::DoSmth = false;
			DAT_008A0DF0 = false;
		}
		LogicalSurface = HiddenSurface;
		Update_Visible_Surface(true, HiddenSurface, 0);

		DepthBuffer = new ZBuffer(Rect(TacticalRect.X, TacticalRect.Y, 480, 480 - TacticalRect.Y));
		DepthBuffer->Set_Scroll(0x8000);
		AlphaBuffer = new ABuffer(Rect(TacticalRect.X, TacticalRect.Y, 480, 480 - TacticalRect.Y));

		DAT_00887640 = yr_new_wwmouse((DSurface *)VisibleSurface, MainWindow);
		yr_wwmouse_hide(DAT_00887640);

		DAT_00A8E954 = 0;
		int cd_drive = -1;
		if (DAT_00A8E950 != 0) {
			cd_drive = DAT_00A8E8E8;
			DAT_00A8E954 = 1;
		}
		CDFileClass::Set_CD_Drive(cd_drive);
		FUN_005355D0();

		Special.IsFromInstall = INI_RA2MD.Get_Bool("Intro", "Play", true);
		WOLOptionsClass wolopt;
		if (!wolopt.Is_Bit0_Clear() && !wolopt.Is_Bit2_Clear() && !wolopt.Is_Bit1_Clear()) {
			Special.Bit3 = true;
		}
		if (Special.IsFromInstall) {
			INI_RA2MD.Put_Bool("Intro", "Play", false);
			cfile->Close();
			cfile->Open(FileClass::READ);
			INI_RA2MD.Save(*cfile, false);
		}
		cfile->Close();
		delete cfile;

		Main_Game(argc, argv);
		if (!DAT_00A8F7AC) {
			SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)Top_Level_Exception_Filter);
		}

		yr_wwmouse_hide(DAT_00887640);
		HiddenSurface->Fill(0);
		Update_Visible_Surface(true, HiddenSurface, 0);
		ReadyToQuit = 1;
		Theme.Free_Themes();
		Audio_Shutdown();
		PostMessageA(MainWindow, WM_DESTROY, 0, 0);
		do {
			Windows_Message_Handler();
		} while (ReadyToQuit == 1);
		retval = 0;
	}

	Free_Game_Modes();
	RadarEventClass::Clear();
	Map.DisposeOfArt();
	SchemeLibrary_Destroy();
	FUN_005BC300();
	FUN_005F77F0();
	FUN_0060D440();
	FUN_006370A0();
	FUN_0072AC40();
	FUN_00730970();
	LoadProgressManager::Destroy();
	Free_Mouse_Shapes();
	Net2Shutdown_Network();
	if (dwRegisterMap != (unsigned long)-1) {
		RevokeActiveObject(dwRegisterMap, 0);
		dwRegisterMap = (unsigned long)-1;
	}
	CoDisconnectObject((IUnknown *)&ApplicationInstance, 0);
	OleUninitialize();
	return retval;
}
