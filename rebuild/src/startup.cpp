/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /counterstrike/STARTUP.CPP 6     3/15/97 7:18p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : STARTUP.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 3, 1994                                              *
 *                                                                                             *
 *                  Last Update : September 30, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Prog_End -- Cleans up library systems in prep for game exit.                              *
 *   main -- Initial startup routine (preps library systems).                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "rect.h"
#include "geometry.h"

#include "_alpha.h"
#include "_command.h"
#include "_convert.h"
#include "_font.h"
#include "_keyboar.h"
#include "_mixfile.h"
#include "_pk.h"
#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "_winfix.h"
#include "_zbuffer.h"
#include "abuffer.h"
#include "aircraft.h"
#include "airctype.h"
#include "aitrig.h"
#include "alphashp.h"
#include "anim.h"
#include "animtype.h"
#include "app.h"
#include "assert.h"
#include "audfile.h"
#include "audio.h"
#include "blight.h"
#include "bomb.h"
#include "brain.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "campaign.h"
#include "capmgr.h"
#include "cd.h"
#include "cdcntrl.h"
#include "cell.h"
#include "classfactory.h"
#include "command.h"
#include "conquer.h"
#include "copyprotection.h"
#include "cstream.h"
#include "data.h"
#include "dbgprint.h"
#include "dllver.h"
#include "drive.h"
#include "droppod.h"
#include "dsaudio.h"
#include "dsurface.h"
#include "empulse.h"
#include "except.h"
#include "factory.h"
#include "fly.h"
#include "fog.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "hover.h"
#include "iblowfish.h"
#include "infantry.h"
#include "infatype.h"
#include "init.h"
#include "ionblast.h"
#include "ipxmgr.h"
#include "isotype.h"
#include "jumpjet.h"
#include "language\language.h"
#include "ldprgmgr.h"
#include "levitate.h"
#include "light.h"
#include "lightcon.h"
#include "mech.h"
#include "mixfile.h"
#include "mono.h"
#include "mouse.h"
#include "movie.h"
#include "msgloop.h"
#include "multiplayer.h"
#include "netdlg.h" // for Shutdown_Network.
#include "netdlg2.h"
#include "overlay.h"
#include "overtype.h"
#include "ovrlight.h"
#include "ownrdraw.h"
#include "palette.h"
#include "parasite.h"
#include "particle.h"
#include "partsys.h"
#include "planning.h"
#include "playcd.h"
#include "psystype.h"
#include "ptype.h"
#include "radsite.h"
#include "ramfile.h"
#include "revent.h"
#include "rocket.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "script.h"
#include "session.h"
#include "shapeset.h"
#include "ship.h"
#include "side.h"
#include "sidebar.h"
#include "smudge.h"
#include "smudtype.h"
#include "stringtable.h"
#include "sun.h"
#include "spawnmanager.h"
#include "super.h"
#include "suprtype.h"
#include "surface.h"
#include "tactical.h"
#include "taction.h"
#include "tag.h"
#include "tagtype.h"
#include "taskforc.h"
#include "team.h"
#include "teamtype.h"
#include "temporal.h"
#include "teleport.h"
#include "terrain.h"
#include "terrtype.h"
#include "tevent.h"
#include "theme.h"
#include "tiberium.h"
#include "trigger.h"
#include "trigtype.h"
#include "trim.h"
#include "tube.h"
#include "tunnel.h"
#include "txtlabel2.h"
#include "uicmnds.h"
#include "unit.h"
#include "unittype.h"
#include "vanim.h"
#include "vanimtype.h"
#include "vector.h"
#include "voc.h"
#include "vox.h"
#include "walk.h"
#include "warhead.h"
#include "wave.h"
#include "waypoint.h"
#include "weapon.h"
#include "win.h"
#include "winstub.h"
#include "wolopt.h"
#include "wwfont.h"
#include "wwmouse.h"
#include "zbuffer.h"
#include "bittext.h"

#include <conio.h>
#include <io.h>
#include <cfloat>







#define APP_GUID "29e3bb2a-2f36-11d3-a72c-0090272fa661"
#define AUTOPLAY_GUID "01AF9993-3492-11d3-8F6F-0060089C05B1" // RA2


bool VideoBackBufferAllowed = true;


#ifndef NO_BLOWFISH_DLL
const struct RegStruct {
	const GUID *clsid;
	const char *name;
} RegisterTheseDLLs[] = {
	{ &CLSID_BlowfishObject, "blowfish.dll" }
};
#endif

HANDLE AppMutex;
HANDLE AutoPlayMutex;

DWORD dwRegisterTiberianSun;
DWORD dwRegisterMap = -1;

DynamicVectorClass<DWORD> RegisteredClasses;

//WinTimerClass * WinTimer;

/// MSDN sample code

/// <summary>
/// Sets the default value of a registry key.
/// This is the low level routine that the server registration code uses to create a key
/// beneath HKEY_CLASSES_ROOT and give it a string value.
/// </summary>
/// <param name="pszKey">The key to create, relative to HKEY_CLASSES_ROOT.</param>
/// <param name="pszSubkey">The subkey to create beneath the key, or NULL for none.</param>
/// <param name="pszValue">The string to store as the key's default value, or NULL to
/// leave the value alone.</param>
/// <returns>bool; Was the key created and its value stored?</returns>
bool SetRegKeyValue(const LPCSTR pszKey, const LPCSTR pszSubkey, const LPCSTR pszValue)
{
	bool bOk = FALSE;
	LONG ec;
	HKEY hKey;
	TCHAR szKey[260];

	lstrcpy(szKey, pszKey);

	if (NULL != pszSubkey)
	{
		lstrcat(szKey, TEXT("\\"));
		lstrcat(szKey, pszSubkey);
	}

	ec = RegCreateKeyEx(HKEY_CLASSES_ROOT, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

	if (ERROR_SUCCESS == ec)
	{
		if (NULL != pszValue)
		{
			ec = RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)pszValue, (lstrlen(pszValue) + 1) * sizeof(TCHAR));
		}
		if (ERROR_SUCCESS == ec)
			bOk = TRUE;
		RegCloseKey(hKey);
	}

	return(bOk);
}


/// MSDN sample code

/// <summary>
/// Registers the game as a COM server.
/// This routine writes the ProgID, the version independent ProgID and the CLSID keys
/// that let OLE find the game by name, and loads the type library so that the interfaces
/// the game exposes are known to the system.
/// </summary>
/// <returns>Returns with the result code; this routine always reports success.</returns>
HRESULT RegisterServer(void)
{
	HRESULT hr = NOERROR;
	TCHAR szID[39 + 1];
	TCHAR szCLSID[48];
	TCHAR szLibID[39 + 1];
	WCHAR temp[39 + 1];
	TCHAR szModulePath[MAX_PATH];

	GetModuleFileName(ProgramInstance, szModulePath, sizeof(szModulePath) / sizeof(TCHAR));
	StringFromGUID2(CLSID_TiberianSun, temp, (sizeof(temp) / sizeof(WCHAR)) - 1);
	WideCharToMultiByte(0, 0, temp, -1, szID, (sizeof(temp) / sizeof(WCHAR)) - 1, 0, 0);
	StringFromGUID2(LIBID_TiberianSun, temp, (sizeof(temp) / sizeof(WCHAR)) - 1);
	WideCharToMultiByte(0, 0, temp, -1, szLibID, (sizeof(temp) / sizeof(WCHAR)) - 1, 0, 0);

	/// Create some base key strings.
	lstrcpy(szCLSID, TEXT("CLSID\\"));
	lstrcat(szCLSID, szID);

	/*
	 * Create ProgID keys.
	 */
	SetRegKeyValue(TEXT(GAME_PROGID), NULL, GAME_VERNAME);
	SetRegKeyValue(TEXT(GAME_PROGID), TEXT("CLSID"), szID);

	/*
	 * Create VersionIndependentProgID keys.
	 */
	SetRegKeyValue(TEXT(GAME_VPROGID), NULL, GAME_VERNAME);
	SetRegKeyValue(TEXT(GAME_VPROGID), TEXT("CurVer"), TEXT(GAME_PROGID));
	SetRegKeyValue(TEXT(GAME_VPROGID), TEXT("CLSID"), szID);

	/*
	 * Create entries under CLSID.
	 */
	SetRegKeyValue(szCLSID, NULL, GAME_VERNAME);
	SetRegKeyValue(szCLSID, TEXT("ProgID"), TEXT(GAME_PROGID));
	SetRegKeyValue(szCLSID, TEXT("VersionIndependentProgID"), TEXT(GAME_VPROGID));
	SetRegKeyValue(szCLSID, TEXT("NotInsertable"), NULL);
	SetRegKeyValue(szCLSID, TEXT("InprocServer32"), szModulePath);
	SetRegKeyValue(szCLSID, TEXT("Typelib"), szLibID);

	ITypeLib * pITypeLib = NULL;
	LoadTypeLib(L"RA2.TLB", &pITypeLib);
	if (pITypeLib != NULL) {
		pITypeLib->Release();
	}

	return(hr);
}


/// MSDN sample code

/// <summary>
/// Removes the game's registration from the system.
/// This routine undoes the work of RegisterServer, deleting the ProgID, the version
/// independent ProgID and the CLSID keys that the game claims for itself.
/// </summary>
/// <returns>Returns with the result code; this routine always reports success.</returns>
HRESULT UnregisterServer(void)
{
	HRESULT hr = NOERROR;
	TCHAR szCLSID[39 + 1 + 32];
	TCHAR szID[39 + 1];
	TCHAR szTemp[300];
	WCHAR temp[39 + 1];

	/// Create some base key strings.
	StringFromGUID2(CLSID_TiberianSun, temp, (sizeof(temp) / sizeof(WCHAR)) - 1);
	WideCharToMultiByte(0, WC_SEPCHARS, temp, -1, szID, (sizeof(temp) / sizeof(WCHAR)) - 1, 0, 0);
	lstrcpy(szCLSID, TEXT("CLSID\\"));
	lstrcat(szCLSID, szID);

	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT(GAME_CURVER));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT(GAME_VCLSID));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT(GAME_VPROGID));

	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT(GAME_CLSID));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT(GAME_PROGID));

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, TEXT("ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, TEXT("VersionIndependentProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, TEXT("NotInsertable"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, TEXT("InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, TEXT("Typelib"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);

	RegDeleteKey(HKEY_CLASSES_ROOT, szCLSID);

	return(hr);
}





/// <summary>
/// Destroys the drawing surfaces and drops the video mode.
/// This routine is used on the way out of the game so that the display is handed back to
/// Windows in a sane state, rather than left sitting in the game's own video mode.
/// </summary>
/// <remarks>It is safe to call this routine more than once; only the first call does any
/// work, since several shutdown paths lead here.</remarks>
void Reset_Surfaces(void)
{
	static bool surfaces_reset = false;
	if (UseDirect3D) {
		Release_Direct3D();
	}
	if (!surfaces_reset) {
		if (HiddenSurface != 0) {
			delete HiddenSurface;
			HiddenSurface = 0;
		}
		if (AlternateSurface != 0) {
			delete AlternateSurface;
			AlternateSurface = 0;
		}
		if (TileSurface != 0) {
			delete TileSurface;
			TileSurface = 0;
		}
		if (SidebarSurface != 0) {
			delete SidebarSurface;
			SidebarSurface = 0;
		}
		if (CompositeSurface != 0) {
			delete CompositeSurface;
			CompositeSurface = 0;
		}
		if (VisibleSurface != 0) {
			delete VisibleSurface;
			VisibleSurface = 0;
		}
		Reset_Video_Mode();
		surfaces_reset = true;
	}
}



/// <summary>
/// Handles any exception that escapes the game.
/// This routine is installed as the top level exception filter for the process, so that
/// a crash is reported by the game's own exception handler rather than by Windows.
/// </summary>
/// <returns>Returns with the disposition that the exception handler decided upon.</returns>
LONG Top_Level_Exception_Filter(EXCEPTION_POINTERS *e_info)
{
	return(Exception_Handler(e_info->ExceptionRecord->ExceptionCode, e_info));
}













/// <summary>
/// Registers the game's COM classes with OLE.
/// This routine is called during startup, before anything that lives in the object
/// database can be created. It first ensures the support DLLs are present, asking any
/// that OLE cannot yet instantiate to register themselves, and then publishes a class
/// factory for every persistent game class so that objects can be created by CLSID. The
/// player is told by way of a message box if a support DLL could not be prepared.
/// </summary>
/// <returns>bool; Did the preparation fail? Note the sense -- true means trouble.</returns>
#ifdef RA2_NON_MATCHING // not exact: 7223 bytes differ (frame layout, and the original calls the first factory's constructor and INIClass() out of line where this expands them); RegisterClasses and Init_Keys are expanded into it
static bool RegisterClasses(void)
{

	bool failed = false;
#ifndef NO_BLOWFISH_DLL
	for (int i = 0; i < ARRAY_SIZE(RegisterTheseDLLs); i++) {
		IUnknownPtr ptr;
		HRESULT result = ptr.CreateInstance(*RegisterTheseDLLs[i].clsid, NULL, CLSCTX_ALL);
		failed = FAILED(result);
		if (failed) {
			failed = false;
			HINSTANCE hModule = LoadLibrary(RegisterTheseDLLs[i].name);
			if (hModule != NULL) {
				FARPROC fprocDllReg = (FARPROC)GetProcAddress(hModule, "DllRegisterServer");
				if (!fprocDllReg || (fprocDllReg(), FAILED(ptr.CreateInstance(*RegisterTheseDLLs[i].clsid, NULL, CLSCTX_ALL)))) {
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
#endif

	RegisterServer();

	DWORD dwRegister;
	IClassFactory *t;

	/// Handy macros to easily register the class factories.

	/// Register a class-object with OLE.
	#define REGISTER_CLASS(_class, _clsid) \
		{ \
			t = new TClassFactory<_class>; \
			CoRegisterClassObject(_clsid, t, CLSCTX_INPROC_SERVER, REGCLS_MULTIPLEUSE, &dwRegister); \
			RegisteredClasses.Add(dwRegister); \
		} \

	/// Register a class-object with OLE (precreated factory).
	#define REGISTER_FACT_CLASS(_class, _fact, _clsid) \
		{ \
			t = _fact; \
			CoRegisterClassObject(_clsid, t, CLSCTX_INPROC_SERVER, REGCLS_MULTIPLEUSE, &dwRegister); \
			RegisteredClasses.Add(dwRegister); \
		}

	REGISTER_FACT_CLASS(TiberianSunClassFactory, &TibSunFactory, CLSID_TiberianSun);

	REGISTER_CLASS(CStreamClass, CLSID_CompressStream);
	REGISTER_CLASS(WaveClass, CLSID_WaveClass);
	REGISTER_CLASS(TerrainTypeClass, CLSID_TerrainTypeClass);
	REGISTER_CLASS(TerrainClass, CLSID_TerrainClass);
	REGISTER_CLASS(SuperWeaponTypeClass, CLSID_SuperWeaponTypeClass);
	REGISTER_CLASS(SuperClass, CLSID_SuperWeaponClass);
	REGISTER_CLASS(Tactical, CLSID_TacticalMapClass);
	REGISTER_CLASS(CellClass, CLSID_CellClass);
	REGISTER_CLASS(EMPulseClass, CLSID_EMPulseClass);
	REGISTER_CLASS(LightSourceClass, CLSID_LightSource);
	REGISTER_CLASS(SideClass, CLSID_SideClass);
	REGISTER_CLASS(TiberiumClass, CLSID_TiberiumClass);
	REGISTER_CLASS(TubeClass, CLSID_TubeClass);
	REGISTER_CLASS(CampaignClass, CLSID_CampaignClass);
	REGISTER_CLASS(BuildingLightClass, CLSID_BuildingLightClass);
	REGISTER_CLASS(WaypointPathClass, CLSID_WaypointPath);
	REGISTER_CLASS(TemporalClass, CLSID_TemporalClass); // RA2: the eight classes from here to RadSiteClass are new
	REGISTER_CLASS(SpawnManagerClass, CLSID_SpawnManagerClass);
	REGISTER_CLASS(CaptureManagerClass, CLSID_CaptureManagerClass);
	REGISTER_CLASS(ParasiteClass, CLSID_ParasiteClass);
	REGISTER_CLASS(BombClass, CLSID_BombClass);
	REGISTER_CLASS(RadSiteClass, CLSID_RadSiteClass);
	REGISTER_CLASS(TEventClass, CLSID_EventClass);
	REGISTER_CLASS(VoxelAnimTypeClass, CLSID_VoxelAnimTypeClass);
	REGISTER_CLASS(VoxelAnimClass, CLSID_VoxelAnimClass);
	REGISTER_CLASS(TActionClass, CLSID_ActionClass);
	REGISTER_CLASS(TriggerClass, CLSID_TriggerClass);
	REGISTER_CLASS(TriggerTypeClass, CLSID_TriggerTypeClass);
	REGISTER_CLASS(ScriptClass, CLSID_ScriptClass);
	REGISTER_CLASS(ScriptTypeClass, CLSID_ScriptTypeClass);
	REGISTER_CLASS(TagClass, CLSID_TagClass);
	REGISTER_CLASS(TagTypeClass, CLSID_TagTypeClass);
	REGISTER_CLASS(TeamClass, CLSID_TeamClass);
	REGISTER_CLASS(TeamTypeClass, CLSID_TeamTypeClass);
	REGISTER_CLASS(TaskForceClass, CLSID_TaskForceClass);
	REGISTER_CLASS(UnitTypeClass, CLSID_UnitTypeClass);
	REGISTER_CLASS(BuildingTypeClass, CLSID_BuildingTypeClass);
	REGISTER_CLASS(AircraftTypeClass, CLSID_AircraftTypeClass);
	REGISTER_CLASS(InfantryTypeClass, CLSID_InfantryTypeClass);
	REGISTER_CLASS(BulletTypeClass, CLSID_BulletTypeClass);
	REGISTER_CLASS(IsometricTileTypeClass, CLSID_IsometricTileTypeClass);
	REGISTER_CLASS(OverlayTypeClass, CLSID_OverlayTypeClass);
	REGISTER_CLASS(SmudgeTypeClass, CLSID_SmudgeTypeClass);
	REGISTER_CLASS(UnitClass, CLSID_UnitClass);
	REGISTER_CLASS(BuildingClass, CLSID_BuildingClass);
	REGISTER_CLASS(AircraftClass, CLSID_AircraftClass);
	REGISTER_CLASS(InfantryClass, CLSID_InfantryClass);
	REGISTER_CLASS(AnimClass, CLSID_AnimClass);
	REGISTER_CLASS(AnimTypeClass, CLSID_AnimTypeClass);
	REGISTER_CLASS(HouseTypeClass, CLSID_HouseTypeClass);
	REGISTER_CLASS(HouseClass, CLSID_HouseClass);
	REGISTER_CLASS(DriveLocomotionClass, CLSID_DriveLocomotion);
	REGISTER_CLASS(JumpjetLocomotionClass, CLSID_JumpjetLocomotion);
	REGISTER_CLASS(HoverLocomotionClass, CLSID_HoverLocomotion);
	REGISTER_CLASS(RocketLocomotionClass, CLSID_RocketLocomotion); // RA2: new
	REGISTER_CLASS(TunnelLocomotionClass, CLSID_TunnelLocomotion);
	REGISTER_CLASS(WalkLocomotionClass, CLSID_WalkLocomotion);
	REGISTER_CLASS(DropPodLocomotionClass, CLSID_BallisticLocomotion);
	REGISTER_CLASS(FlyLocomotionClass, CLSID_FlyerLocomotion);
	REGISTER_CLASS(TeleportLocomotionClass, CLSID_TeleportLocomotion);
	REGISTER_CLASS(MechLocomotionClass, CLSID_MechLocomotion);
	REGISTER_CLASS(ShipLocomotionClass, CLSID_ShipLocomotion); // RA2: in place of LevitateLocomotionClass
	REGISTER_CLASS(BulletClass, CLSID_BulletClass);
	REGISTER_CLASS(FactoryClass, CLSID_FactoryClass);
	REGISTER_CLASS(WarheadTypeClass, CLSID_WarheadTypeClass);
	REGISTER_CLASS(WeaponTypeClass, CLSID_WeaponTypeClass);
	REGISTER_CLASS(ParticleClass, CLSID_ParticleClass);
	REGISTER_CLASS(ParticleTypeClass, CLSID_ParticleTypeClass);
	REGISTER_CLASS(ParticleSystemClass, CLSID_ParticleSystemClass);
	REGISTER_CLASS(ParticleSystemTypeClass, CLSID_ParticleSystemTypeClass);
	REGISTER_CLASS(AITriggerTypeClass, CLSID_AITriggerTypeClass);
	REGISTER_CLASS(NeuronClass, CLSID_NeuronClass);
	REGISTER_CLASS(FoggedObjectClass, CLSID_FoggedObjectClass);
	REGISTER_CLASS(AlphaShapeClass, CLSID_AlphaShapeClass);

	RegisterActiveObject((IUnknown *)&ApplicationInstance, CLSID_TiberianSun, ACTIVEOBJECT_WEAK, &dwRegisterTiberianSun);

	if (failed) {
		Message_Box_Wide(NULL, StringTable::LoadString("TXT_PREPARECOM_FAILED", NULL, "D:\\Ra2\\Startup.CPP", 1518), StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 1518), MB_ICONEXCLAMATION); // RA2: the wide message box and the string table
	}

	return(failed);

}


/***********************************************************************************************
 * Init_Keys -- Initialize the cryptographic keys.                                             *
 *                                                                                             *
 *    This routine will initialize the fast cryptographic key. It will also initialize the     *
 *    slow one if this is a scenario editor version of the game.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static void Init_Keys(void) // RA2: moved here from init.cpp; WinMain calls it before the language archive is opened
{
	DebugString("Init_Keys - declarations\n");
	RAMFileClass file((void *)Keys, strlen((char *)Keys));
	INIClass ini;
	DebugString("Init_Keys - Load\n");
	ini.Load(file, false);
	DebugString("Init_Keys - Init fast key\n");
	FastKey = ini.Get_PKey(true);
}



/***********************************************************************************************
 * main -- Initial startup routine (preps library systems).                                    *
 *                                                                                             *
 *    This is the routine that is first called when the program starts up. It basically        *
 *    handles the command line parsing and setting up library systems.                         *
 *                                                                                             *
 * INPUT:   argc  -- Number of command line arguments.                                         *
 *                                                                                             *
 *          argv  -- Pointer to array of command line argument strings.                        *
 *                                                                                             *
 * OUTPUT:  Returns with execution failure code (if any).                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int CALLBACK WinMain ( HINSTANCE instance , HINSTANCE , char * command_line , int command_show )
{
	int		argc;       //Command line argument count
	char *	argv[20];   //Pointers to command line arguments
	char	path_to_exe[132];
	wchar_t buffer[512]; // RA2: wide, for the string table's text

	ProgramInstance = instance;

	MainThread = (HANDLE)GetCurrentThreadId();

	DebugString("%d-%d\n", 1560, sizeof(CrateData)); // RA2: the line numbers are those of the original's startup.cpp
	DebugString("%d-%d\n", 1561, sizeof(CrateShares));
	DebugString("%d-%d\n", 1562, sizeof(CrateAnims));
	DebugString("%d-%d\n", 1563, 0x13); // RA2: a new entry; the object of 19 bytes is not identified
	DebugString("%d-%d\n", 1564, sizeof(Tactical));
	DebugString("%d-%d\n", 1565, sizeof(AircraftClass));
	DebugString("%d-%d\n", 1566, sizeof(AircraftTypeClass));
	DebugString("%d-%d\n", 1567, sizeof(AITriggerTypeClass));
	DebugString("%d-%d\n", 1568, sizeof(AnimClass));
	DebugString("%d-%d\n", 1569, sizeof(AnimTypeClass));
	DebugString("%d-%d\n", 1570, sizeof(BaseClass));
	DebugString("%d-%d\n", 1571, sizeof(BuildingClass));
	DebugString("%d-%d\n", 1572, sizeof(BuildingTypeClass));
	DebugString("%d-%d\n", 1573, sizeof(BulletClass));
	DebugString("%d-%d\n", 1574, sizeof(BulletTypeClass));
	DebugString("%d-%d\n", 1575, sizeof(CellClass));
	DebugString("%d-%d\n", 1576, sizeof(FactoryClass));
	DebugString("%d-%d\n", 1577, sizeof(HouseClass));
	DebugString("%d-%d\n", 1578, sizeof(HouseTypeClass));
	DebugString("%d-%d\n", 1579, sizeof(InfantryClass));
	DebugString("%d-%d\n", 1580, sizeof(InfantryTypeClass));
	DebugString("%d-%d\n", 1581, sizeof(LayerClass));
	DebugString("%d-%d\n", 1582, sizeof(MouseClass));
	DebugString("%d-%d\n", 1583, 0xE18);		/// Presumed to be MapEditTactical.
	DebugString("%d-%d\n", 1584, sizeof(OverlayClass));
	DebugString("%d-%d\n", 1585, sizeof(OverlayTypeClass));
	DebugString("%d-%d\n", 1586, sizeof(SmudgeClass));
	DebugString("%d-%d\n", 1587, sizeof(SmudgeTypeClass));
	DebugString("%d-%d\n", 1588, sizeof(TaskForceClass));
	DebugString("%d-%d\n", 1589, sizeof(TeamClass));
	DebugString("%d-%d\n", 1590, sizeof(TeamTypeClass));
	DebugString("%d-%d\n", 1591, sizeof(TerrainClass));
	DebugString("%d-%d\n", 1592, sizeof(TerrainTypeClass));
	DebugString("%d-%d\n", 1593, sizeof(TriggerClass));
	DebugString("%d-%d\n", 1594, sizeof(TriggerTypeClass));
	DebugString("%d-%d\n", 1595, sizeof(TagTypeClass));
	DebugString("%d-%d\n", 1596, sizeof(TagClass));
	DebugString("%d-%d\n", 1597, sizeof(UnitClass));
	DebugString("%d-%d\n", 1598, sizeof(UnitTypeClass));
	DebugString("%d-%d\n", 1599, sizeof(ScenarioClass));
	DebugString("%d-%d\n", 1600, sizeof(SuperWeaponTypeClass));
	DebugString("%d-%d\n", 1601, sizeof(SuperClass));
	DebugString("%d-%d\n", 1602, sizeof(Ground));
	DebugString("%d-%d\n", 1603, sizeof(RulesClass));
	DebugString("%d-%d\n", 1604, sizeof(MissionControl));
	DebugString("%d-%d\n", 1605, 1);
	DebugString("%d-%d\n", 1606, sizeof(TechnoClass));
	DebugString("%d-%d\n", 1607, sizeof(RadioClass));
	DebugString("%d-%d\n", 1608, sizeof(MissionClass));
	DebugString("%d-%d\n", 1609, 8);
	DebugString("%d-%d\n", 1610, sizeof(StageClass));
	DebugString("%d-%d\n", 1611, 8);
	DebugString("%d-%d\n", 1612, 8);
	DebugString("%d-%d\n", 1613, sizeof(StorageClass));
	DebugString("%d-%d\n", 1614, sizeof(DelayTimerClass));
	DebugString("%d-%d\n", 1615, sizeof(FacingClass));

	if (!CopyProtection::IsLauncherRunning()) { // RA2: the game runs only under its launcher
		DebugString("Launcher not running....Bail!\n");
		return(EXIT_SUCCESS);
	}

	/*
	 * Create a mutex with a unique name to TibSun in order to determine if
	 * our app is already running.
	 *
	 * WARNING: DO NOT use this number for any other application except TibSun
	 */
	AppMutex = ::CreateMutex (NULL, FALSE, APP_GUID);

	//
	// Is there already an instance of this app somewhere?
	//
	if (::GetLastError () == ERROR_ALREADY_EXISTS) {
		//
		// Find the previous instance
		//
		HWND main_wnd = ::FindWindow (APP_GUID, NULL);
		if (main_wnd != NULL) {
			::SetForegroundWindow (main_wnd);
			::ShowWindow (main_wnd, SW_RESTORE);
		}
		if (AppMutex != NULL) {
			CloseHandle(AppMutex);
			AppMutex = NULL;
		}
		DebugString("TibSun is already running...Bail!\n");
		return(EXIT_SUCCESS);
	}

	DebugString("Create AppMutex okay.\n");

	//
	// Obtain the mutex unique to the Renegade AutoPlay application.
	//
	// WARNING: DO NOT use this number for any other application except Renegade AutoPlay
	//
	do
	{
		//
		// Attempt to open the mutex
		//
		AutoPlayMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, AUTOPLAY_GUID);
		if (AutoPlayMutex != NULL) {
			DebugString("Waiting for Autoplay to quit!\n");
			if (::WaitForSingleObject(AutoPlayMutex, 30000) == WAIT_FAILED) {
				DebugString("Failed waiting for AutoPlayMutex\n");
				::CloseHandle(AutoPlayMutex);
				AutoPlayMutex = NULL;
			}
		}

		/*
		 * Create a mutex with a name unique to the TibSun AutoPlay application.
		 * This prevents the autoplay from running since it cannot get the mutex.
		 * TibSun needs both of these mutexs before it is allowed to run.
		 */
		if (AutoPlayMutex == NULL) {
			AutoPlayMutex = CreateMutex(NULL, FALSE, AUTOPLAY_GUID);
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				CloseHandle(AutoPlayMutex);
				AutoPlayMutex = NULL;
				Sleep(2500);
			} else {
				DebugString("Create AutoPlayMutex.\n");
			}
		}
	} while (AutoPlayMutex == NULL);

	DebugString("Got AutoPlayMutex okay.\n");

	_controlfp(_RC_CHOP, MCW_RC);
	set_control_word();

	/*
	**	Get the full path to the .EXE
	*/
	GetModuleFileName (instance, &path_to_exe[0], sizeof(path_to_exe));

	/*
	**	First argument is supposed to be a pointer to the .EXE that is running
	**
	*/
	argc=1;                     //Set argument count to 1
	argv[0]=&path_to_exe[0];    //Set 1st command line argument to point to full path

	/*
	**	Get pointers to command line arguments just like if we were in DOS
	**
	*/
	char *token = strtok(command_line, " ");
	while (argc < ARRAY_SIZE(argv) && token != NULL) {
		argv[argc++] = strtrim(token);
		token = strtok(NULL, " ");
	}

	/*
	**	Change directory to the where the executable is located. Handle the
	**	case where there is no path attached to argv[0].
	*/
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char dir[_MAX_DIR];
	_splitpath(argv[0], drive, dir, NULL, NULL);
	_makepath(path, drive, dir, NULL, NULL);
	SetCurrentDirectory(path);

	int error_code = EXIT_FAILURE;

	if (Parse_Command_Line(argc, argv)) {

		RawFileClass *cfile = new RawFileClass(CONFIG_FILE_NAME);

		ConfigINI.Load(*cfile, false);
		VideoBackBufferAllowed = ConfigINI.Get_Bool("Video", "VideoBackBuffer", true);
		Debug_IngameModeChange = ConfigINI.Get_Bool("Video", "AllowModeToggle", false);
		Options.ScreenWidth = ConfigINI.Get_Int("Video", "ScreenWidth", Options.ScreenWidth);
		Options.ScreenHeight = ConfigINI.Get_Int("Video", "ScreenHeight", Options.ScreenHeight);

		int socket = ConfigINI.Get_Int("Network", "Socket", 0);
		if (socket > 0) {
			socket += 0x4000;
			if (socket >= 0x4000 && socket < 0x8000) {
				Ipx.Set_Socket(socket);
			}
		}

		char netbuf[INIClass::MAX_LINE_LENGTH];
		memset(netbuf, 0, sizeof(netbuf));
		char * netptr = netbuf; // RA2: the scan starts at the beginning of the entry
		bool has_destination_network = ConfigINI.Get_String("Network", "DestNet", 0, netbuf, sizeof(netbuf)) ? true : false;
		if (has_destination_network && netptr && strlen(netbuf)) {

			NetNumType net;
			NetNodeType node;

			/*
			**	Scan thestring, pulling off each address piece
			*/
			int i = 0;
			char * p = strtok(netbuf, ".");
			int x;
			while (p) {
				sscanf(p, "%x", &x);			// convert from hex string to int
				if (i < 4) {
					net[i] = (char)x;			// fill NetNum
				} else {
					node[i-4] = (char)x;		// fill NetNode
				}
				i++;
				p = strtok(NULL, ".");
			}

			/*
			**	If all the address components were successfully read, fill in the
			**	BridgeNet with a broadcast address to the network across the bridge.
			*/
			if (i >= 4) {
				Session.IsBridge = 1;
				memset(node, 0xff, 6);
				Session.BridgeNet = IPXAddressClass(net, node, 0);
			}
		}

		Keyboard = new KeyboardClass();

		/*
		**	If there is not enough disk space free, don't allow the product to run.
		*/
		if (Disk_Space_Available() < INIT_FREE_DISK_SPACE) {
			swprintf(buffer, StringTable::LoadString("TXT_CRITICALLY_LOW", NULL, "D:\\Ra2\\Startup.CPP", 1831), (INIT_FREE_DISK_SPACE) / (1024 * 1024));
			int reply = Message_Box_Wide(NULL, buffer, StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 1832), MB_ICONQUESTION | MB_YESNO);
			if (reply == IDNO) {
				OleUninitialize();
				return(EXIT_FAILURE);
			}
		}

		OleInitialize(NULL); // RA2: OLE and the class factories come after the configuration file

		if (RegisterClasses()) {
			exit(EXIT_FAILURE);
		}

		/*
		**	Initialize the encryption keys.
		*/
		DebugString("Init Encryption Keys.\n"); // RA2: moved here from Init_Game
		Init_Keys();

		DiskID cd = CD::Get_Required_Disk(); // RA2: the language archive is opened from the local files
		CD::Set_Required_Disk(DISK_LOCAL);
		LanguageMix = new MFCD("LANGUAGE.MIX", &FastKey);
		CD::Set_Required_Disk(cd);

		atexit(Prog_End);

		Start_Mouse_Thread(); // RA2

		if (!StringTable::LoadFile("ra2.str")) { // RA2: the string table, in the three languages of the release when it cannot be read
			MessageBox(NULL, "Unable to initialize 'ra2.csf', please reinstall Red Alert.\nKeine Initialisierung von 'ra2.csf' m\xf6glich. Bitte installieren Sie Red Alert erneut.\nInitialisation de 'ra2.csf' impossible. Veuillez r\xe9installer Red Alert.", "Red Alert 2", MB_ICONERROR);
			return(EXIT_SUCCESS);
		}

		if (!Init_Language_Resources(true)) {
			return(EXIT_SUCCESS);
		}

		if (!GetSystemMetrics(SM_MOUSEPRESENT) || !GetSystemMetrics(SM_CMOUSEBUTTONS)) {
			Message_Box_Wide(NULL, StringTable::LoadString("TXT_MOUSE_REQUIRED", NULL, "D:\\Ra2\\Startup.CPP", 1895), StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 1895), MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		if (GetDllVersion("comctl32.dll") < PACKVERSION(4, 70)) {
			swprintf(buffer, StringTable::LoadString("TXT_DLL_INVALID", NULL, "D:\\Ra2\\Startup.CPP", 1905), "comctl32.dll", 4, 70, "comctl32.dll");
			Message_Box_Wide(NULL, buffer, StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 1906), MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		RegisterActiveObject((IUnknown *)&Map, CLSID_TiberianSun, ACTIVEOBJECT_WEAK, &dwRegisterMap);

		if (Options.ScreenWidth == -1 || Options.ScreenHeight == -1) {
			Options.ScreenWidth = 640;

			if (WinVersion.Is_WinNTx() || GetVideoMemory() > 0x200000) {
				Options.ScreenHeight = 480;
			} else {
				Options.ScreenHeight = 400;
			}
		}

		BOOL video_success = FALSE;

		if (Debug_IngameModeChange) {
			Create_Main_Window(instance, command_show, 640, 400);
			Prep_Direct_Draw();
			if (WindowedMode) {
				VisibleRect = Rect(0,0,640,400);
				VideoModeWidth = 640;
				VideoModeHeight = 400;
				VideoModeBitsPerPixel = HicolorFlag ? 16 : 8;
			} else {
				/*
				** Set 640x400 video mode. If its not available then try for 640x480
				*/
				VisibleRect = Rect(0,0,640,400);
				video_success = Set_Video_Mode(MainWindow, 640, 400, HicolorFlag ? 16 : 8);
				if (!video_success) {
					video_success = Set_Video_Mode(MainWindow, 640, 480, HicolorFlag ? 16 : 8);
					VisibleRect = Rect(0,0,640,480);
					if (!video_success) {
						Destroy_Direct_Draw();
						Message_Box_Wide(MainWindow, StringTable::LoadString("TXT_VIDEO_ERROR", NULL, "D:\\Ra2\\Startup.CPP", 2049), StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 2050), MB_ICONEXCLAMATION);
						exit(EXIT_FAILURE);
					}
				}
			}
		} else {
			Create_Main_Window(instance, command_show, Options.ScreenWidth, Options.ScreenHeight);
			Prep_Direct_Draw();
			if (WindowedMode) {
				VisibleRect = Rect(0,0,Options.ScreenWidth,Options.ScreenHeight);
				VideoModeWidth = Options.ScreenWidth;
				VideoModeHeight = Options.ScreenHeight;
				VideoModeBitsPerPixel = HicolorFlag ? 16 : 8;
			} else {
				/*
				 * Set desired video mode. If its not available then try for 640x480
				 */
				video_success = Set_Video_Mode(MainWindow, Options.ScreenWidth, Options.ScreenHeight, HicolorFlag ? 16 : 8);
				VisibleRect = Rect(0,0,Options.ScreenWidth,Options.ScreenHeight);
				if (!video_success) {
					if (Options.ScreenWidth != 640 || Options.ScreenHeight != 480) {
						Options.ScreenWidth = 640;
						Options.ScreenHeight = 480;
						video_success = Set_Video_Mode(MainWindow, Options.ScreenWidth, Options.ScreenHeight, HicolorFlag ? 16 : 8);
						VisibleRect = Rect(0,0,Options.ScreenWidth,Options.ScreenHeight);
					}
					if (!video_success) {
						Destroy_Direct_Draw();
						Message_Box_Wide(MainWindow, StringTable::LoadString("TXT_VIDEO_ERROR", NULL, "D:\\Ra2\\Startup.CPP", 2088), StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 2089), MB_ICONEXCLAMATION);
						exit(EXIT_FAILURE);
					}
				}
			}
		}

		if (RecordPlayRequest && Audio_File_Init(1000, 0x19000)) { // RA2: -record and -play keep the session's audio in session.ipb
			if (RecordPlayRequest == 1) {
				Audio_File_Create_Stream("session.ipb");
			} else if (RecordPlayRequest == 2) {
				Audio_File_Open_Stream("session.ipb");
			}
		}

		VisibleSurface = DSurface::Create_Primary();

		int visible_bpp = VisibleSurface->Bytes_Per_Pixel();
		HicolorFlag = visible_bpp == 2;
		if (!HicolorFlag) {
			Message_Box_Wide(MainWindow, StringTable::LoadString("TXT_INVALIDMODE", NULL, "D:\\Ra2\\Startup.CPP", 2118), StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Startup.CPP", 2118), MB_ICONEXCLAMATION);
			Emergency_Exit();
		}

		if (!CopyProtection::NotifyLauncher()) { // RA2
			DebugString("Failed to notify launcher\n");
			exit(EXIT_FAILURE);
		}

		do {
			Windows_Message_Handler();
		}
		while (!GameInFocus);

		SurfacesRestored = false;

		if (!WindowedMode) {
			VisibleSurface->Fill(0);
		}

		Mono_Window_Init(ProgramInstance, MainWindow, WindowedMode); // RA2
		Planning_Reset(); // RA2

		Rect sidebar_rect(0, 0, SidebarWidth, VisibleRect.Height);
		Rect tile_rect(0,0,VisibleRect.Width-sidebar_rect.Width, sidebar_rect.Height);
		Rect composite_rect(0,0,VisibleRect.Width-sidebar_rect.Width, sidebar_rect.Height);

		Allocate_Surfaces(VisibleRect, composite_rect, tile_rect, sidebar_rect, false);
		if (UseDirect3D) { // RA2
			Init_Direct3D();
		}
		LogicalSurface = HiddenSurface;
		Update_Visible_Surface(true, HiddenSurface);

		DepthBuffer = new ZBuffer(Rect(TacticalRect.X, TacticalRect.Y, 480, 480 - TacticalRect.Y));
		DepthBuffer->Set_Scroll(ZBUFFER_MAX);

		AlphaBuffer = new ABuffer(Rect(TacticalRect.X, TacticalRect.Y, 480, 480 - TacticalRect.Y));

		MouseCursor = new WWMouseClass(VisibleSurface, MainWindow);
		MouseCursor->Hide_Mouse(); // RA2: hidden until the game shows it

		CDFileClass::Set_CD_Drive (CDList.Get_First_CD_Drive());

		/*
		**	Check for forced intro movie run disabling. If the conquer
		**	configuration file says "no", then don't run the intro.
		*/
		int disk = CD::Get_Current_Disk(60); // RA2: any disc; the registry options can set EscapeMovies
		Get_Performance_Profile();
		if (!Special.IsFromInstall && disk > -1) {
			Special.IsFromInstall = ConfigINI.Get_Bool("Intro", "Play", true);
		}

		WOLOptionsClass options;
		options.Load();
		if (!options.Is_Bit0_Clear() && !options.Is_Bit2_Clear() && !options.Is_Bit1_Clear()) {
			Special.IsEscapeMovies = true;
		}

		/*
		**	Regardless of whether we should run it or not, here we're
		**	gonna change it to say "no" in the future.
		*/
		if (Special.IsFromInstall == true) {
			if (disk > -1) {
				Special.IsFromInstall = ConfigINI.Put_Bool("Intro", "Play", false);
			}
			cfile->Close();
			cfile->Open();
			ConfigINI.Save(*cfile, false);
		}

		cfile->Close();
		delete cfile;

		if (NoExceptionHandler) {

			Main_Game(argc, argv);

		} else {

			/*
			**	The __try/__except construct is part of the WIN32 interface. This is not the same
			**	as C++ exception handling. There is no additional code overhead required
			**	with this interface and the compilers exception handling options should be disabled
			**	to prevent the compiler generating additional stack unwinding code.
			*/
			__try {

				DebugString("Main_Game\n");

				Main_Game(argc, argv);

			} __except(Exception_Handler(GetExceptionCode(), GetExceptionInformation())) {};

			SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) &Top_Level_Exception_Filter);

		}

		MouseCursor->Hide_Mouse(); // RA2
		HiddenSurface->Fill(0);
		Update_Visible_Surface(true, HiddenSurface);

		/*
		**	Flag that this is a clean shutdown (not killed with Ctrl-Alt-Del)
		*/
		ReadyToQuit = 1;

		Theme.Free_Themes(); // RA2: the themes and the sound go before the window
		Audio_Shutdown();

		/*
		**	Post a message to our message handler to tell it to clean up.
		*/
		PostMessage(MainWindow, WM_DESTROY, 0, 0);

		/*
		**	Wait until the message handler has dealt with the message
		*/
		do
		{
			Windows_Message_Handler();
		}while (ReadyToQuit == 1);

		error_code = EXIT_SUCCESS;
	}

	Free_Game_Modes(); // RA2: the RA2 systems free what they hold
	RadarEventClass::Clear();
	Map.DisposeOfArt();
	SchemeLibrary_Destroy();
	Mono_Window_Shutdown();
	ObjectTypeClass::Free_One_Time();
	OwnerDraw::Shutdown();
	Planning_Reset_Wrapper();
	Free_All_Art();
	Free_Type_Select_List();
	LoadProgressManager::Destroy();
	Free_Mouse_Shapes();
	Net2Shutdown_Network();

	if (dwRegisterMap != -1) {
		RevokeActiveObject(dwRegisterMap, NULL);
		dwRegisterMap = -1;
	}

	CoDisconnectObject((LPUNKNOWN)&ApplicationInstance, 0);
	OleUninitialize();

	return(error_code);
}
#endif



/***********************************************************************************************
 * Prog_End -- Cleans up library systems in prep for game exit.                                *
 *                                                                                             *
 *    This routine should be called before the game terminates. It handles cleaning up         *
 *    library systems so that a graceful return to the host operating system is achieved.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void __cdecl Prog_End(void)
{
	int i;

	GameActive = false;

	if (EndGameGraphicsShape != NULL) { // RA2: the end game graphic, its palette conversion and its palette
		delete (void *)EndGameGraphicsShape;
		EndGameGraphicsShape = NULL;
	}
	if (EndGameGraphicsConvert != NULL) {
		delete EndGameGraphicsConvert;
		EndGameGraphicsConvert = NULL;
	}
	if (EndGameGraphicsPalette != NULL) {
		delete EndGameGraphicsPalette;
		EndGameGraphicsPalette = NULL;
	}

	Session.Free_Scenario_Descriptions();

	while (ColorSchemes.Count()) {
		delete ColorSchemes[0];
	}
	ColorSchemes.Clear();

	Delete_All_Objects(); // RA2: no tutorial text

	while (Movies.Count()) {
		free((void *)Movies[0]);
		Movies.Delete_Index(0);
	}
	Movies.Clear();

	for (i = 0; i < AllCommands.Count(); i++) {
		delete AllCommands[i];
	}
	AllCommands.Clear();

	if (RuleINI != NULL) { // RA2: the themes are freed at the end
		delete RuleINI;
		RuleINI = NULL;
	}
	AIINI.Clear();
	ArtINI.Clear();
	UIINI.Clear(); // RA2: in place of the Firestorm databases
	EditorINI.Clear();
	ConfigINI.Clear();
	ConfigINI.Clear();
	SpotLightClass::Clear_All();
	IonBlastClass::Clear_All();

	if (MouseCursor) {
		delete MouseCursor;
		MouseCursor = NULL;
	}

	Map.Free_Cells();
	Map.Clear_Radar();

	Reset_Surfaces(); // RA2

	if (DepthBuffer != NULL) {
		delete DepthBuffer;
		DepthBuffer = NULL;
	}

	if (AlphaBuffer != NULL) {
		delete AlphaBuffer;
		AlphaBuffer = NULL;
	}

	if (BitText::Instance != NULL) { // RA2: the game font in place of the TibSun fonts
		delete BitText::Instance;
		BitText::Instance = NULL;
	}

	if (TerrainDrawer != NULL) {
		delete TerrainDrawer;
		TerrainDrawer = NULL;
	}

	if (AnimDrawer != NULL) {
		delete AnimDrawer;
		AnimDrawer = NULL;
	}

	if (NormalDrawer != NULL) {
		delete NormalDrawer;
		NormalDrawer = NULL;
	}

	if (VoxelDrawer != NULL) {
		delete VoxelDrawer;
		VoxelDrawer = NULL;
	}

	if (MouseDrawer != NULL) {
		delete MouseDrawer;
		MouseDrawer = NULL;
	}

	if (SidebarDrawer != NULL) {
		delete SidebarDrawer;
		SidebarDrawer = NULL;
	}

	if (CameoDrawer != NULL) {
		delete CameoDrawer;
		CameoDrawer = NULL;
	}

	if (EightBitDrawer != NULL) {
		delete EightBitDrawer;
		EightBitDrawer = NULL;
	}

	if (EightBitSurface != NULL) {
		delete EightBitSurface;
		EightBitSurface = NULL;
	}

	if (CloakingSurface != NULL) {
		delete (Surface *)CloakingSurface;
		CloakingSurface = NULL;
	}

	if (BuildingTypeClass::BuildingZShape != NULL) {
		delete (void *)BuildingTypeClass::BuildingZShape;
		BuildingTypeClass::BuildingZShape = NULL;
	}

	Map.Shutdown();

	for (i = 0; i < TileDrawers.Count(); i++) {
		delete TileDrawers[i];
	}
	TileDrawers.Clear();

	if (TheaterData) {
		delete TheaterData;
		TheaterData = NULL;
	}

	if (TheaterDat) {
		delete TheaterDat;
		TheaterDat = NULL;
	}

	if (GenericMix) { // RA2
		delete GenericMix;
		GenericMix = NULL;
	}

	if (IsometricTheaterData) {
		delete IsometricTheaterData;
		IsometricTheaterData = NULL;
	}

	if (IsoGenericMix) { // RA2
		delete IsoGenericMix;
		IsoGenericMix = NULL;
	}

	if (IsometricTileTypeClass::CellShadowShapes != NULL) {
		delete IsometricTileTypeClass::CellShadowShapes;
		IsometricTileTypeClass::CellShadowShapes = NULL;
	}

	if (SlopeZShapes[0] != NULL) {
		delete (void *)SlopeZShapes[0];
		SlopeZShapes[0] = NULL;
	}

	if (SlopeZShapes[1] != NULL) {
		delete (void *)SlopeZShapes[1];
		SlopeZShapes[1] = NULL;
	}

	if (SlopeZShapes[2] != NULL) {
		delete (void *)SlopeZShapes[2];
		SlopeZShapes[2] = NULL;
	}

	if (SlopeZShapes[3] != NULL) {
		delete (void *)SlopeZShapes[3];
		SlopeZShapes[3] = NULL;
	}

	if (PreviewSurface != NULL) {
		delete PreviewSurface;
		PreviewSurface = NULL;
	}

	if (MoviesMix != NULL) {
		delete MoviesMix;
		MoviesMix = NULL;
	}

	if (Scores01Mix != NULL) {
		delete Scores01Mix;
		Scores01Mix = NULL;
	}

	if (MainMix != NULL) {
		delete MainMix;
		MainMix = NULL;
	}

	if (ConquerMix != NULL) {
		delete ConquerMix;
		ConquerMix = NULL;
	}

	if (CameoMix != NULL) { // RA2
		delete CameoMix;
		CameoMix = NULL;
	}

	while (ExpandMix.Count() > 0) {
		delete ExpandMix[0];
		ExpandMix.Delete_Index(0);
	}

	if (CacheMix != NULL) {
		delete CacheMix;
		CacheMix = NULL;
	}

	if (LocalMix != NULL) {
		delete LocalMix;
		LocalMix = NULL;
	}

	if (NeutralMix != NULL) { // RA2
		delete NeutralMix;
		NeutralMix = NULL;
	}

	if (SideCDMix != NULL) {
		delete SideCDMix;
		SideCDMix = NULL;
	}

	if (Sounds01Mix != NULL) {
		delete Sounds01Mix;
		Sounds01Mix = NULL;
	}

	while (ScenarioMixFiles.Count() > 0) { // RA2
		delete ScenarioMixFiles[0];
		ScenarioMixFiles.Delete_Index(0);
	}

	if (MapsMix != NULL) {
		delete MapsMix;
		MapsMix = NULL;
	}

	if (MultiMix != NULL) {
		delete MultiMix;
		MultiMix = NULL;
	}

	if (SideCMix != NULL) {
		delete SideCMix;
		SideCMix = NULL;
	}

	if (SideNCMix != NULL) {
		delete SideNCMix;
		SideNCMix = NULL;
	}

	if (GameMix != NULL) {
		delete GameMix;
		GameMix = NULL;
	}

	if (LanguageMix != NULL) { // RA2
		delete LanguageMix;
		LanguageMix = NULL;
	}

	if (TacticalMap != NULL) {
		delete TacticalMap;
		TacticalMap = NULL;
	}

	RadarEventClass::Clear(); // RA2: the RA2 systems free what they hold
	Map.DisposeOfArt();
	SchemeLibrary_Destroy();
	Mono_Window_Shutdown();
	ObjectTypeClass::Free_One_Time();
	OwnerDraw::Shutdown();
	Planning_Reset_Wrapper();
	Free_All_Art();
	Free_Type_Select_List();
	LoadProgressManager::Destroy();
	Free_Mouse_Shapes();
	Net2Shutdown_Network();

	Free_Vocs();
	Speech_Free_All(); // RA2: in place of the speech buffer
	CDFileClass::Clear_Search_Drives();

	if (Keyboard != NULL) {
		delete Keyboard;
		Keyboard = NULL;
	}

	Shutdown_Network();

	if (UnkBuffer != NULL) {
		delete UnkBuffer;
		UnkBuffer = NULL;
	}

	if (Rule != NULL) {
		delete Rule;
		Rule = NULL;
	}

	if (Scen != NULL) {
		delete Scen;
		Scen = NULL;
	}

	for (i = 0; i < RegisteredClasses.Count(); i++) {
		CoRevokeClassObject((DWORD)RegisteredClasses[i]);
	}
	RegisteredClasses.Clear();

	RevokeActiveObject(dwRegisterTiberianSun, NULL);

	if (LanguageResources) {
		FreeLibrary(LanguageResources);
	}

	if (AutoPlayMutex != NULL) {
		CloseHandle(AutoPlayMutex);
		AutoPlayMutex = NULL;
	}
	if (AppMutex != NULL) {
		CloseHandle(AppMutex);
		AppMutex = NULL;
	}

	Audio_File_Shutdown(); // RA2
	Theme.Free_Themes();
	Audio_Shutdown();
	StringTable::Unload();
	CopyProtection::ShutDown();
	Kill_Mouse_Thread();
}



/// <summary>
/// Ends the process with the exit code given.
/// </summary>
void Exit_Process(int code)
{
	exit(code);
}


/***********************************************************************************************
 * Emergency_Exit -- Function to call when we want to exit unexpectedly.                       *
 *                   Use this function instead of exit(n) so everything is properly cleaned up.*
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Code to return to the OS                                                          *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    3/13/97 1:32AM ST : Created                                                              *
 *=============================================================================================*/
void Emergency_Exit(void)
{

	ReadyToQuit = 1;

	/*
	**	Post a message to our message handler to tell it to clean up.
	*/
	PostMessage(MainWindow, WM_DESTROY, 0, 0);

	while (MainWindow) {
		Windows_Message_Handler();
		if (ReadyToQuit != 1) {
			break;
		}
	}

	if (dwRegisterMap != -1) {
		RevokeActiveObject(dwRegisterMap, NULL);
		dwRegisterMap = -1;
	}

	CoDisconnectObject((LPUNKNOWN)&ApplicationInstance, 0);

	OleUninitialize();

	if (MouseCursor) {
		MouseCursor->Release_Mouse();
		delete MouseCursor;
	}
	MouseCursor = NULL;

	if (MouseShapesAllocated) {
		delete MouseShapes;
		MouseShapesAllocated = false;
	}

	PostQuitMessage(EXIT_SUCCESS);

	Shutdown_Network();

	Prog_End();
}


/*
**	The class factories, one for each class registered with OLE, in the order of registration
**	(ra2-decomp's startup.cpp instantiates them the same way).
*/
template class TClassFactory<CStreamClass>;
template class TClassFactory<WaveClass>;
template class TClassFactory<TerrainTypeClass>;
template class TClassFactory<TerrainClass>;
template class TClassFactory<SuperWeaponTypeClass>;
template class TClassFactory<SuperClass>;
template class TClassFactory<Tactical>;
template class TClassFactory<CellClass>;
template class TClassFactory<EMPulseClass>;
template class TClassFactory<LightSourceClass>;
template class TClassFactory<SideClass>;
template class TClassFactory<TiberiumClass>;
template class TClassFactory<TubeClass>;
template class TClassFactory<CampaignClass>;
template class TClassFactory<BuildingLightClass>;
template class TClassFactory<WaypointPathClass>;
template class TClassFactory<TemporalClass>;
template class TClassFactory<SpawnManagerClass>;
template class TClassFactory<CaptureManagerClass>;
template class TClassFactory<ParasiteClass>;
template class TClassFactory<BombClass>;
template class TClassFactory<RadSiteClass>;
template class TClassFactory<TEventClass>;
template class TClassFactory<VoxelAnimTypeClass>;
template class TClassFactory<VoxelAnimClass>;
template class TClassFactory<TActionClass>;
template class TClassFactory<TriggerClass>;
template class TClassFactory<TriggerTypeClass>;
template class TClassFactory<ScriptClass>;
template class TClassFactory<ScriptTypeClass>;
template class TClassFactory<TagClass>;
template class TClassFactory<TagTypeClass>;
template class TClassFactory<TeamClass>;
template class TClassFactory<TeamTypeClass>;
template class TClassFactory<TaskForceClass>;
template class TClassFactory<UnitTypeClass>;
template class TClassFactory<BuildingTypeClass>;
template class TClassFactory<AircraftTypeClass>;
template class TClassFactory<InfantryTypeClass>;
template class TClassFactory<BulletTypeClass>;
template class TClassFactory<IsometricTileTypeClass>;
template class TClassFactory<OverlayTypeClass>;
template class TClassFactory<SmudgeTypeClass>;
template class TClassFactory<UnitClass>;
template class TClassFactory<BuildingClass>;
template class TClassFactory<AircraftClass>;
template class TClassFactory<InfantryClass>;
template class TClassFactory<AnimClass>;
template class TClassFactory<AnimTypeClass>;
template class TClassFactory<HouseTypeClass>;
template class TClassFactory<HouseClass>;
template class TClassFactory<DriveLocomotionClass>;
template class TClassFactory<JumpjetLocomotionClass>;
template class TClassFactory<HoverLocomotionClass>;
template class TClassFactory<RocketLocomotionClass>;
template class TClassFactory<TunnelLocomotionClass>;
template class TClassFactory<WalkLocomotionClass>;
template class TClassFactory<DropPodLocomotionClass>;
template class TClassFactory<FlyLocomotionClass>;
template class TClassFactory<TeleportLocomotionClass>;
template class TClassFactory<MechLocomotionClass>;
template class TClassFactory<ShipLocomotionClass>;
template class TClassFactory<BulletClass>;
template class TClassFactory<FactoryClass>;
template class TClassFactory<WarheadTypeClass>;
template class TClassFactory<WeaponTypeClass>;
template class TClassFactory<ParticleClass>;
template class TClassFactory<ParticleTypeClass>;
template class TClassFactory<ParticleSystemClass>;
template class TClassFactory<ParticleSystemTypeClass>;
template class TClassFactory<AITriggerTypeClass>;
template class TClassFactory<NeuronClass>;
template class TClassFactory<FoggedObjectClass>;
template class TClassFactory<AlphaShapeClass>;
