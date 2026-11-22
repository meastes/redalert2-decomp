/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/INIT.CPP 8     3/14/97 5:15p Joe_b $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : INIT.CPP                                                     *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : January 20, 1992                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Anim_Init -- Initialize the VQ animation control structure.                               *
 *   Bootstrap -- Perform the initial bootstrap procedure.                                     *
 *   Calculate_CRC -- Calculates a one-way hash from a data block.                             *
 *   Init_Authorization -- Verifies that the player is authorized to play the game.            *
 *   Init_Bootstrap_Mixfiles -- Registers and caches any mixfiles needed for bootstrapping.    *
 *   Init_Bulk_Data -- Initialize the time-consuming mixfile caching.                          *
 *   Init_CDROM_Access -- Initialize the CD-ROM access handler.                                *
 *   Init_Color_Remaps -- Initialize the text remap tables.                                    *
 *   Init_Expansion_Files -- Fetch any override expansion mixfiles.                            *
 *   Init_Fonts -- Initialize all the game font pointers.                                      *
 *   Init_Game -- Main game initialization routine.                                            *
 *   Init_Heaps -- Initialize the game heaps and buffers.                                      *
 *   Init_Keys -- Initialize the cryptographic keys.                                           *
 *   Init_Mouse -- Initialize the mouse system.                                                *
 *   Init_One_Time_Systems -- Initialize internal pointers to the bulk data.                   *
 *   Init_Random -- Initializes the random-number generator                                    *
 *   Init_Secondary_Mixfiles -- Register and cache secondary mixfiles.                         *
 *   Load_Recording_Values -- Loads recording values from recording file                       *
 *   Load_Title_Page -- Load the background art for the title page.                            *
 *   Obfuscate -- Sufficiently transform parameter to thwart casual hackers.                   *
 *   Parse_Command_Line -- Parses the command line parameters.                                 *
 *   Parse_INI_File -- Parses CONQUER.INI for special options                                  *
 *   Play_Intro -- plays the introduction & logo movies                                        *
 *   Save_Recording_Values -- Saves recording values to a recording file                       *
 *   Select_Game -- The game's main menu                                                       *
 *   Load_Prolog_Page -- Loads the special pre-prolog "please wait" page.                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include "init.h"
#include "bitfont.h" // RA2: Draw_Loading_Screen measures its text with a BitFont
#include "_keyboar.h"
#include "_palette.h"
#include "_pk.h"
#include "_rand.h"
#include "_surface.h"
#include "airctype.h"
#include "anim.h"
#include "builtype.h"
#include "bullettype.h"
#include "cd.h"
#include "cell.h"
#include "conquer.h"
#include "dbgprint.h"
#include "audtimer.h" // RA2: AudioGetTime, for the loading screen hold time
#include "draw.h" // RA2: Draw_Shape, for the loading screen picture
#include "msgloop.h" // RA2: Get_Input_Record_Mode, which Init_Random asks
#include "shapeset.h" // RA2: the loading screen picture is a ShapeSet
#include "stringtable.h" // RA2: StringTable::LoadString, with RA2's source file and line
#include "dialog.h"
#include "dsurface.h"
#include "gamedlg.h"
#include "getcpu.h"
#include "infatype.h"
#include "ionblast.h"
#include "lightcon.h"
#include "mixfile.h"
#include "msgbox.h"
#include "newmenu.h"
#include "nullmgr.h"
#include "overtype.h"
#include "ovrlight.h"
#include "ownrdraw.h"
#include "pcx.h"
#include "revent.h"
#include "rndstraw.h"
#include "rules.h"
#include "scenario.h"
#include "smudtype.h"
#include "theme.h"
#include "unittype.h"
#include "vein.h"
#include "campaign.h" // RA2: CampaignClass::From_Name, Campaigns
#include "voc.h" // RA2: VocClass::PlayGlobal
#include "audioctl.h" // RA2: AudioController
#include "queue.h" // RA2: DoList and OutList
#include "_tooltip.h" // RA2: ToolTips
#include "cctooltip.h" // RA2: CCToolTip::Activate
#include "movie.h" // RA2: Movies, Play_Movie
#include "enviro.h" // RA2: Environment
#include "misc.h" // RA2: Set_Palette
#include "skirmish.h" // RA2: Skirmish_Mode_Dialog
#include "modemhst.h" // RA2: ModemHost
#include "modemgst.h" // RA2: ModemGuest
#include "wspipx.h" // RA2: IPXInterfaceClass
#include "netdlg2.h" // RA2: Net2Init_Network
#include "_timer.h" // RA2: TickCount
#include "mainopt.h" // RA2: Main_Options_Dialog
#include "vox.h" // RA2: Is_Speaking
#include "planning.h" // RA2: Planning_Reset_State
#include "vqoption.h" // RA2: Anim_Init sets the VQ option
#include "loaddlg.h" // RA2: LoadOptionsClass::Files_Present
#include "wspudp.h" // RA2: Start_Network_Session creates the UDP interface
#include "_WSProto.h" // RA2: PacketTransport
#include "ownertalk.h" // RA2: OwnerTalk.Reset
#include "netdlg.h" // RA2: Init_Network
#include "_rules.h" // RA2: ConfigINI, RuleINI
#include "houstype.h" // RA2: HouseTypeClass::Read_INI
#include "winstub.h"
#include "_xmouse.h"
#include "worlddom.h"
#include "ipxmgr.h" // RA2: Ipx.Set_Socket
#include "obscure.h" // RA2: Obfuscate
#include "wsproto.h" // RA2: Set_Packet_Loss_Percent, for -DROP=
#include "firewall.h" // RA2: FirewallHelper.Port
#include "command.h" // RA2: the keyboard commands Init_Commands creates
#include "_command.h" // RA2: AllCommands, HotkeyCommands
#include "tactical.h" // RA2: Tactical
#include "_tactica.h" // RA2: TacticalMap
#include "building.h" // RA2: the tab commands place the building they are given
#include "unit.h" // RA2: Center_Base_View looks through Units
#include "tracker.h" // RA2: Delete_All_Objects: ObjectsToDelete, TargetTracker, Process_Deferred_Deletion
#include "bullet.h" // RA2: as above
#include "trigger.h" // RA2: as above
#include "tube.h" // RA2: as above
#include "blight.h" // RA2: as above
#include "overlay.h" // RA2: as above
#include "partsys.h" // RA2: as above
#include "wave.h" // RA2: as above
#include "factory.h" // RA2: as above
#include "side.h" // RA2: as above
#include "_script.h" // RA2: as above
#include "script.h" // RA2: as above
#include "terrain.h" // RA2: as above
#include "alphashp.h" // RA2: as above
#include "fog.h" // RA2: as above
#include "empulse.h" // RA2: as above
#include "parasite.h" // RA2: as above
#include "temporal.h" // RA2: as above
#include "spawnmanager.h" // RA2: as above
#include "radsite.h" // RA2: as above
#include "capmgr.h" // RA2: as above
#include "bomb.h" // RA2: as above
#include "ebolt.h" // RA2: as above
#include "laser.h" // RA2: as above
#include "beacon.h" // RA2: as above
#include "_theater.h" // RA2: Init_Theater: Theaters
#include "scheme.h" // RA2: as above: ColorSchemes
#include "mpu.h" // RA2: Get_Performance_Profile: Get_RDTSC_CPU_Speed
#include "mono.h" // RA2: Bootstrap clears mono page 4
#include "bittext.h" // RA2: Bootstrap creates the BitText
#include "volinfo.h" // RA2: Init_CDROM_Access: VolumeCheck
#include "wolopt.h" // RA2: Init_CDROM_Access: WOLOptionsClass
#include "voxdrsys.h" // RA2: Bootstrap: VoxelDrawSystem::Load_VPL_File
#include "_voxel.h" // RA2: Bootstrap: Set_Voxel_Light_Angle
#include "voxel.h" // RA2: Bootstrap: Init_Voxel_Matrices
#include "_convert.h" // RA2: Bootstrap: the drawers
#include "convert.h" // RA2: as above


#include <conio.h>
#include <ctime>
#include <dos.h>
#include "_mixfile.h"
#include "audio.h"
#include "egos.h"
#include "globals.h"
#include "mouse.h"
#include "multiplayer.h"
#include "txtlabel2.h"
#include "win.h"



#define	PARM_INSTALL		0xD95C68A2		//	"FROMINSTALL"


/*
**	RA2: the values Main_Menu and the menu dialogs store for Select_Game. The numbers are what the
**	executable compares; the names say what the code around them does.
*/
enum MainMenuChoice { // RA2: as above
	MENU_TIMEOUT = -1,			// RA2: Main_Menu ran out of time: the attract mode may play a recording
	MENU_SINGLE_PLAYER = 1,		// RA2: the button that opens the single player dialog 0x100
	MENU_INTERNET = 2,			// RA2: Session.Type = GAME_INTERNET
	MENU_NETWORK = 3,			// RA2: IDC_NETWORK, Session.Type = GAME_IPX
	MENU_EXTRAS = 4,			// RA2: the button that opens the movies and credits dialog 0x101
	MENU_OPTIONS = 5,			// RA2: Main_Options_Dialog
	MENU_EXIT_ASK = 6,			// RA2: "GUI:ExitAreYouSure"
	MENU_EXIT = 7, // RA2: as above
	MENU_NEW_CAMPAIGN = 8,		// RA2: the single player dialog's first button: Choose_Campaign
	MENU_LOAD_GAME = 9,			// RA2: its second button, enabled while saved games exist
	MENU_SINGLE_AGAIN = 10,		// RA2: its third button; Select_Game shows the single player dialog again
	MENU_SKIRMISH = 11,			// RA2: its fourth button: Session.Type = GAME_SKIRMISH
	MENU_TRAILER = 13,			// RA2: movies dialog: plays the trailer
	MENU_MOVIES = 14,			// RA2: movies dialog: the list of movies
	MENU_CREDITS = 15,			// RA2: movies dialog: Show_Credits
	MENU_MULTIPLAYER = 0x10,	// RA2: back from a multiplayer game
	MENU_INTERNET_RETURN = 0x11,	// RA2: back from an Internet game
	MENU_NONE = 0x12			// RA2: nothing chosen yet
};

/*
**	RA2: an entry of the movie list that the movies dialog shows (OptionsClass::Fill_Movie_List fills it).
*/
struct MovieListEntry { // RA2: as above
	char const * Name;		// RA2: +0x00, the movie file
	char const * Label;		// RA2: +0x04, the string table entry shown in the list
	int CD;					// RA2: +0x08, the disc the movie is on
};


bool Init_Secondary_Mixfiles(void); // RA2: 0x00515E70, defined below
bool Init_Bulk_Data(void); // RA2: 0x00516900, defined below
bool Init_Bootstrap_Mixfiles(void); // RA2: 0x00515C90, defined below
bool __cdecl Init_Expansion_Files(void); // RA2: 0x00515AF0, defined below
void Draw_Loading_Screen(void); // RA2: 0x00516560, defined below
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined below
void Init_Random(void); // RA2: 0x00515710, defined below
void Init_Commands(void); // RA2: 0x005173C0, defined below
bool Init_Rules(void); // RA2: 0x005129C0, defined below
BOOL CALLBACK Rules_Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x00513080, defined below

static bool Bootstrap(void); // RA2: Bootstrap, Init_Mouse and Init_CDROM_Access are expanded inline in Init_Game (0x005116A0), as in TibSun
static void Init_Mouse(void); // RA2: as above
static bool Init_CDROM_Access(void); // RA2: as above

/***********************************************************************************************
 * Init_Game -- Main game initialization routine.                                              *
 *                                                                                             *
 *    RA2 has Bootstrap, Init_Mouse and Init_CDROM_Access expanded inline in here.             *
 *=============================================================================================*/
int Init_Game(int, char *[]) // RA2: 0x005116A0
{
	DebugString("Init Game\n"); // RA2: as above
	Scen = new ScenarioClass; // RA2: as above

	if (Scen == NULL) { // RA2: as above
		DebugString("Failed to instantiate Scenario!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	Rule = new RulesClass; // RA2: as above

	if (Rule == NULL) { // RA2: as above
		DebugString("Failed to instantiate Rules!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	DebugString("Bootstrap....."); // RA2: as above

	if (!Bootstrap()) { // RA2: as above
		DebugStringNoPrefix(" ..Failed to bootstrap!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	DebugStringNoPrefix(" ...OK\n"); // RA2: as above

	DebugString("Init Mouse\n"); // RA2: as above

	Init_Mouse(); // RA2: as above

	DebugString("Init CDROM\n"); // RA2: as above

	if (!Init_CDROM_Access()) { // RA2: as above
		DebugString("Failed to initialize CDROM access!\n"); // RA2: as above
		return(1); // RA2: as above
	}

	DebugString("Init Secondary Mixfiles....."); // RA2: as above

	if (!Init_Secondary_Mixfiles()) { // RA2: as above
		DebugStringNoPrefix(" ...Failed!!!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	DebugStringNoPrefix(" ...OK\n"); // RA2: as above

	DebugString("Init Campaigns\n"); // RA2: as above
	Init_Campaigns(); // RA2: as above

	DebugString("Init Heaps\n"); // RA2: no benchmarks and no Init_Keys at the start, and Init_Heaps and Init_Threads call nothing: only their DebugString lines remain
	DebugString("Init Threads\n"); // RA2: as above

	DebugString("Reading Game Settings\n"); // RA2: as above
	Options.Load_Settings(); // RA2: as above

	DebugString("Init Anim System\n"); // RA2: as above
	Initialize_Options(); // RA2: the executable calls Initialize_Options and Set_Option(0) (vqoption.cpp) here, and plays the movies later, after Init_Rules
	Set_Option(0); // RA2: as above

	DebugString("Init Color Remap Tables\n"); // RA2: as above
	GadgetClass::Set_Color_Scheme(DEFAULT_GADGET_SCHEME); // RA2: the executable stores the colour scheme name "Grey" (no Init_Color_Remaps call)

	DebugString("Creating TacticalMap\n"); // RA2: as above

	delete TacticalMap; // RA2: as above
	TacticalMap = new Tactical; // RA2: as above

	if (TacticalMap == NULL) { // RA2: as above
		DebugString("Failed to create TacticalMap!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	LogicalSurface = HiddenSurface; // RA2: as above

	DebugString("Reading SOUND.INI\n"); // RA2: SOUND.INI, EVA.INI and THEME.INI are read here, each into a local CCINIClass
	CCFileClass voc_file("SOUND.INI"); // RA2: as above
	if (voc_file.Is_Available() == false) { // RA2: as above
		DebugString("Failed to find SOUND.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	CCINIClass voc_ini; // RA2: as above
	if (!voc_ini.Load(voc_file, false)) { // RA2: as above
		DebugString("Failed to load SOUND.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	Free_Vocs(); // RA2: as above
	Init_Vocs(voc_ini); // RA2: 0x00714540 reads the sound list from SOUND.INI

	DebugString("Reading EVA.INI\n"); // RA2: as above
	CCFileClass eva_file("EVA.INI"); // RA2: as above
	if (eva_file.Is_Available() == false) { // RA2: as above
		DebugString("Failed to find EVA.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	CCINIClass eva_ini; // RA2: as above
	if (!eva_ini.Load(eva_file, false)) { // RA2: as above
		DebugString("Failed to load EVA.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	Speech_Free_All(); // RA2: 0x00716590 frees the speech, then VoxClass::Process (0x007163F0) reads EVA.INI
	VoxClass::Process(eva_ini); // RA2: as above

	DebugString("Reading THEME.INI\n"); // RA2: as above
	CCFileClass theme_file("THEME.INI"); // RA2: as above
	if (theme_file.Is_Available() == false) { // RA2: as above
		DebugString("Failed to find THEME.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	CCINIClass theme_ini; // RA2: as above
	if (!theme_ini.Load(theme_file, false)) { // RA2: as above
		DebugString("Failed to load THEME.INI!\n"); // RA2: as above
		return(-1); // RA2: as above
	}
	Theme.Free_Themes(); // RA2: as above
	Theme.Init_Themes(theme_ini); // RA2: as above
	Theme.Scan(); // RA2: as above

	DebugString("Init Bulk Data\n"); // RA2: as above
	if (!Init_Bulk_Data()) { // RA2: as above
		return(-1); // RA2: as above
	}

	DebugString("Init Rules\n"); // RA2: as above
	if (!Init_Rules()) { // RA2: as above
		DebugString("Failed to initialize Rules!\n"); // RA2: as above
		return(-1); // RA2: as above
	}

	while (AudioGetTime() < LoadingScreenHoldUntil) { // RA2: waits until LoadingScreenHoldUntil (0x00A41668, set by Draw_Loading_Screen) has passed
	}

	if (Special.IsFromInstall == true) { // RA2: WESTLOGO and A00_F00E play at the first start (the 6-argument Play_Movie 0x005A02D0 when EscapeMovies is set), otherwise WESTLOGO alone
		DebugString("Playing first time intro sequence.\n"); // RA2: as above
		if (Special.IsEscapeMovies) { // RA2: as above
			Play_Movie("WESTLOGO", THEME_NONE, true, true, true, false); // RA2: as above
			Play_Movie("A00_F00E", THEME_NONE, false, true, true, false); // RA2: as above
		} else {
			Play_Movie("WESTLOGO", THEME_NONE, true, true, true); // RA2: as above
			Play_Movie("A00_F00E", THEME_NONE, false, true, true); // RA2: as above
		}
	} else {
		DebugString("Playing startup movie.\n"); // RA2: as above
		Play_Movie("WESTLOGO", THEME_NONE, true, true, true, false); // RA2: as above
	}
	Darken_Surface(NULL, true); // RA2: as above
	Call_Back(); // RA2: as above

	Session.MaxPlayers = Rule->MaxPlayers; // RA2: the session copies MaxPlayers from the rules (+0x1138 to 0x00A3D558)
	GadgetClass::Set_Color_Scheme(DEFAULT_GADGET_SCHEME); // RA2: the executable stores the colour scheme name "Grey" (no Init_Color_Remaps call)

	Session.GamesPlayed = 0; // RA2: the score table starts empty: every count is -1
	Session.NumScores = 0; // RA2: as above
	Session.CurGame = 0; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		Session.Score[i].Name[0] = 0; // RA2: as above
		Session.Score[i].Wins = 0; // RA2: as above
		for (int j = 0; j < 4; j++) { // RA2: as above
			Session.Score[i].Kills[j] = -1; // RA2: as above
			Session.Score[i].Lost[j] = -1; // RA2: as above
			Session.Score[i].Built[j] = -1; // RA2: as above
			Session.Score[i].Score[j] = -1; // RA2: as above
		}
	}
	memset(Session.PlayerChatEnabled, true, sizeof(Session.PlayerChatEnabled)); // RA2: the eight chat flags at 0x00A3F118 are set

	Init_Random(); // RA2: ends with Init_Random, Init_Commands and Init_Game_Modes
	DebugString("Init Commands\n"); // RA2: as above
	Init_Commands(); // RA2: as above
	DebugString("Game Init Completed.\n"); // RA2: as above
	Init_Game_Modes(); // RA2: as above
	return(0); // RA2: as above
}


static bool Bootstrap(void) // RA2: as above
{
	int index; // RA2: as above

	if (!HicolorFlag) { // RA2: as above
		Set_Palette(BlackPalette); // RA2: as above
	}

	if (CD::Is_Override_Swap() == true) { // RA2: as above
		CD::Set_Required_Disk(DISK_LOCAL); // RA2: as above
	}

	do { // RA2: as above
		Keyboard->Check(); // RA2: as above
	} while (!GameInFocus); // RA2: as above

	SurfacesRestored = false; // RA2: as above
	MonoClass::Get_Page(4)->Clear(); // RA2: clears mono page 4 (0x0059DA50) where TibSun calls Mono_Clear_Screen

	if (!Init_Bootstrap_Mixfiles()) { // RA2: as above
		DebugString("Failed to initialize bootstrap mixfiles!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	BitText::Instance = new BitText; // RA2: no Init_Fonts; the executable creates the BitText, starts the owner draw system, loads the interface art, starts the audio and draws the loading screen
	OwnerDraw::Initialize(); // RA2: as above
	Load_All_Art(); // RA2: as above
	Audio_Init(AudioAllowed); // RA2: as above
	Draw_Loading_Screen(); // RA2: as above
	Keyboard->Clear(); // RA2: as above

	memmove(&SchemePalette[0], MFCD::Retrieve("UNITSNO.PAL"), sizeof(SchemePalette)); // RA2: as above

	for (index = 0; index < 256; index++) { // RA2: as above
		SchemePalette[index] = RGBClass( // RA2: as above
				(unsigned char)(SchemePalette[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(SchemePalette[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(SchemePalette[index].Get_Blue()<<2)); // RA2: as above
	}

	memmove(&GamePalette[0], MFCD::Retrieve("TEMPERAT.PAL"), sizeof(GamePalette)); // RA2: as above

	for (index = 0; index < 256; index++) { // RA2: as above
		GamePalette[index] = RGBClass( // RA2: as above
				(unsigned char)(GamePalette[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(GamePalette[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(GamePalette[index].Get_Blue()<<2)); // RA2: as above
	}

	OriginalPalette = GamePalette; // RA2: as above
	CCPalette = GamePalette; // RA2: as above
	WhitePalette[0] = BlackPalette[0]; // RA2: as above

	memmove(&WaypointPalette[0], MFCD::Retrieve("WAYPOINT.PAL"), sizeof(WaypointPalette)); // RA2: as above

	for (index = 0; index < 256; index++) { // RA2: as above
		WaypointPalette[index] = RGBClass( // RA2: as above
				(unsigned char)(WaypointPalette[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(WaypointPalette[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(WaypointPalette[index].Get_Blue()<<2)); // RA2: as above
	}

	CCFileClass vplfile("voxels.vpl"); // RA2: the result of Load_VPL_File is not checked
	VoxelDrawSystem::Load_VPL_File(vplfile); // RA2: as above

	for (int i = 0; i < ARRAY_SIZE(VoxelRGBColors); i++) { // RA2: as above
		VoxelPalette[i] = RGBClass(VoxelRGBColors[i].Red, VoxelRGBColors[i].Green, VoxelRGBColors[i].Blue); // RA2: as above
	}

	UseVoxelCache = true; // RA2: as above
	Set_Voxel_Camera_Angle(DefaultCameraAngle); // RA2: as above
	Set_Voxel_Light_Angle(DefaultLightAngle); // RA2: as above
	VoxelDrawSystem::Enable_Lighting(); // RA2: as above
	VoxelDrawSystem::Disable_ZBuffer(); // RA2: as above

	Init_Voxel_Matrices(); // RA2: as above

	TerrainDrawer = new ConvertClass(GamePalette, GamePalette, *VisibleSurface, 0x35, false); // RA2: every drawer is built with 0x35 intensity levels and no blitter tables

	PaletteClass pal; // RA2: as above

	memmove(&pal[0], MFCD::Retrieve("ANIM.PAL"), sizeof(pal)); // RA2: as above
	for (index = 0; index < 256; index++) { // RA2: as above
		pal[index] = RGBClass( // RA2: as above
				(unsigned char)(pal[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Blue()<<2)); // RA2: as above
	}

	AnimDrawer = new ConvertClass(pal, GamePalette, *VisibleSurface, 0x35, false); // RA2: as above

	memmove(&pal[0], MFCD::Retrieve("PALETTE.PAL"), sizeof(pal)); // RA2: as above
	for (index = 0; index < 256; index++) { // RA2: as above
		pal[index] = RGBClass( // RA2: as above
				(unsigned char)(pal[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Blue()<<2)); // RA2: as above
	}

	NormalDrawer = new ConvertClass(pal, GamePalette, *VisibleSurface, 0x35, false); // RA2: as above

	memmove(&pal[0], MFCD::Retrieve("UNITSNO.PAL"), sizeof(pal)); // RA2: as above
	for (index = 0; index < 256; index++) { // RA2: as above
		pal[index] = RGBClass( // RA2: as above
				(unsigned char)(pal[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Blue()<<2)); // RA2: as above
	}

	VoxelDrawer = new ConvertClass(pal, GamePalette, *VisibleSurface, 0x35, false); // RA2: as above

	memmove(&pal[0], MFCD::Retrieve("CAMEO.PAL"), sizeof(pal)); // RA2: as above
	for (index = 0; index < 256; index++) { // RA2: as above
		pal[index] = RGBClass( // RA2: as above
				(unsigned char)(pal[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Blue()<<2)); // RA2: as above
	}
	CameoDrawer = new ConvertClass(pal, GamePalette, *VisibleSurface, 0x35, false); // RA2: as above

	memmove(&pal[0], MFCD::Retrieve("MOUSEPAL.PAL"), sizeof(pal)); // RA2: as above
	for (index = 0; index < 256; index++) { // RA2: as above
		pal[index] = RGBClass( // RA2: as above
				(unsigned char)(pal[index].Get_Red()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Green()<<2), // RA2: as above
				(unsigned char)(pal[index].Get_Blue()<<2)); // RA2: as above
	}
	MouseDrawer = new ConvertClass(pal, GamePalette, *VisibleSurface, 1, false); // RA2: as above

	TiberiumDrawer = VoxelDrawer; // RA2: as above

	CCFileClass palfile("GRFXTXT.PAL"); // RA2: GRFXTXT.PAL, GRFXTXT.SHP and their ConvertClass (0x00A41670-0x00A41678) are loaded here
	if (palfile.Is_Available()) { // RA2: as above
		EndGameGraphicsPalette = Load_Alloc_Data(palfile); // RA2: as above
		palfile.Close(); // RA2: as above
		for (index = 0; index < 256; index++) { // RA2: as above
			(*(PaletteClass *)EndGameGraphicsPalette)[index] = RGBClass( // RA2: as above
					(unsigned char)((*(PaletteClass *)EndGameGraphicsPalette)[index].Get_Red()<<2), // RA2: as above
					(unsigned char)((*(PaletteClass *)EndGameGraphicsPalette)[index].Get_Green()<<2), // RA2: as above
					(unsigned char)((*(PaletteClass *)EndGameGraphicsPalette)[index].Get_Blue()<<2)); // RA2: as above
		}
	}
	if (EndGameGraphicsPalette != NULL) { // RA2: as above
		EndGameGraphicsConvert = new ConvertClass(*(PaletteClass *)EndGameGraphicsPalette, GamePalette, *VisibleSurface, 1, false); // RA2: as above
	}

	CCFileClass shpfile("GRFXTXT.SHP"); // RA2: as above
	if (shpfile.Is_Available()) { // RA2: as above
		EndGameGraphicsShape = (ShapeSet const *)Load_Alloc_Data(shpfile); // RA2: as above
		shpfile.Close(); // RA2: as above
	}

	Init_Expansion_Files(); // RA2: as above

	return(true); // RA2: as above
}


static void Init_Mouse(void) // RA2: as above
{
	MouseShapes = (void *)Load_Alloc_Data("MOUSE.SHP", MouseShapesAllocated); // RA2: MOUSE.SHP is loaded by name (0x00495270) into 0x00ABD488 and handed to the mouse
	if (MouseShapes != NULL) { // RA2: as above
		Hide_Mouse(); // RA2: as above
		Point2D pt = Point2D(0, 0); // RA2: as above
		Set_Mouse_Cursor(pt, (ShapeSet const *)MouseShapes, 0); // RA2: as above
	}
	Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: as above
	Call_Back(); // RA2: as above
}


static bool Init_CDROM_Access(void) // RA2: as above
{
	if (!CDFileClass::Is_There_Search_Drives()) { // RA2: as above
		DebugString("Calling Force_CD_Available\n"); // RA2: as above
		if (!CD().Force_Available(DISK_ANY)) { // RA2: as above
			DebugString("Failed to find disk (user cancel)!\n"); // RA2: as above
			return(false); // RA2: as above
		}

		int error; // RA2: as above
		do { // RA2: as above
			error = CDFileClass::Set_Search_Drives("?:\\"); // RA2: every CD drive is searched ("?:\\"); a message box explains the two error cases
			switch (error) { // RA2: as above
				case 1: // RA2: as above
					Set_Palette(GamePalette); // RA2: as above
					WWMessageBox().Process(StringTable::LoadString("TXT_CD_ERROR1", NULL, "D:\\Ra2\\Init.CPP", 0x104D), // RA2: as above
						StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Init.CPP", 0x104D), NULL, NULL, false); // RA2: as above
					return(false); // RA2: as above

				case 2: // RA2: as above
					Set_Palette(GamePalette); // RA2: as above
					if (WWMessageBox().Process(StringTable::LoadString("TXT_CD_DIALOG_1", NULL, "D:\\Ra2\\Init.CPP", 0x1056), // RA2: as above
							StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Init.CPP", 0x1056), // RA2: as above
							StringTable::LoadString("TXT_CANCEL", NULL, "D:\\Ra2\\Init.CPP", 0x1056), NULL, false) == 1) { // RA2: as above
						DebugString("Failed to find disk (user cancel)!\n"); // RA2: as above
						return(false); // RA2: as above
					}
					break; // RA2: as above

				default: // RA2: as above
					if (!CD::Force_Available()) { // RA2: as above
						DebugString("Failed to find disk (user cancel)!\n"); // RA2: as above
						return(false); // RA2: as above
					}
					break; // RA2: as above
			}
		} while (error); // RA2: as above

		CD::Set_Required_Disk(DISK_ANY); // RA2: as above
		if (!VolumeCheck()) { // RA2: as above
			DebugString("VolumeCheck FAILED!!\n"); // RA2: as above
			WWMessageBox().Process(StringTable::LoadString("TXT_CD_ERROR1", NULL, "D:\\Ra2\\Init.CPP", 0x106F), // RA2: as above
				StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Init.CPP", 0x106F), NULL, NULL, false); // RA2: as above
			return(false); // RA2: as above
		}

		int disk = CD::Get_Current_Disk(60); // RA2: the disc found decides whether the first-time intro plays; the registry options can set EscapeMovies
		if (!Special.IsFromInstall && disk > -1) { // RA2: as above
			Special.IsFromInstall = ConfigINI.Get_Bool("Intro", "Play", true); // RA2: as above
		}

		WOLOptionsClass options; // RA2: the constructor clears the two members and Load (0x004FF200) is called explicitly: the executable does so inline
		options.Load(); // RA2: as above
		if (!options.Is_Bit0_Clear() && !options.Is_Bit2_Clear() && !options.Is_Bit1_Clear()) { // RA2: as above
			Special.IsEscapeMovies = true; // RA2: as above
		}

		if (Special.IsFromInstall == true) { // RA2: WESTLOGO and A00_F00E play at the first start (the 6-argument Play_Movie 0x005A02D0 when EscapeMovies is set), otherwise WESTLOGO alone
			CCFileClass ra2_ini("RA2.INI"); // RA2: as above
			if (disk > -1) { // RA2: as above
				Special.IsFromInstall = ConfigINI.Put_Bool("Intro", "Play", false); // RA2: as above
			}
			ConfigINI.Save(ra2_ini, false); // RA2: as above
		}
	} else {
		DebugString("-CD parameter was specified. No CD init - all files must be local\n"); // RA2: as above
		CD::Set_Required_Disk(DISK_LOCAL); // RA2: as above
	}

	return(true); // RA2: as above
}


void __fastcall Init_Campaigns(void) // RA2: 0x005127F0
{
	bool found = false; // RA2: as above
	WIN32_FIND_DATA data; // RA2: as above
	HANDLE handle = FindFirstFile("BATTLE*.INI", &data); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) { // RA2: as above
		do { // RA2: as above
			if ((data.dwFileAttributes & 0x116) == 0) { // RA2: as above
				char const * name = data.cAlternateFileName; // RA2: as above
				if (strlen(name) == 0) { // RA2: as above
					name = data.cFileName; // RA2: as above
				}
				CCFileClass file(name); // RA2: as above
				CCINIClass * ini = new CCINIClass; // RA2: as above
				ini->Load(file, false, false); // RA2: as above
				if (stricmp(name, "BATTLE.INI") == 0) { // RA2: as above
					found = true; // RA2: as above
				}
				Read_Battle_INI(*ini); // RA2: as above
				delete ini; // RA2: as above
			}
		} while (FindNextFile(handle, &data)); // RA2: as above
		FindClose(handle); // RA2: as above
	}
	if (!found) { // RA2: as above
		CCFileClass file("BATTLE.INI"); // RA2: as above
		CCINIClass * ini = new CCINIClass; // RA2: as above
		if (ini != NULL) { // RA2: as above
			ini->Load(file, false, false); // RA2: as above
			Read_Battle_INI(*ini); // RA2: as above
			delete ini; // RA2: as above
		}
	}
}


bool __fastcall Init_Rules(void) // RA2: 0x005129C0
{
	DynamicVectorClass<CCINIClass *> files; // RA2: as above
	bool found = false; // RA2: as above
	WIN32_FIND_DATA data; // RA2: as above
	HANDLE handle = FindFirstFile("RULE*.INI", &data); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) { // RA2: as above
		do { // RA2: as above
			if ((data.dwFileAttributes & 0x116) == 0) { // RA2: as above
				char const * name = data.cAlternateFileName; // RA2: as above
				if (data.cAlternateFileName[0] == 0) { // RA2: as above
					name = data.cFileName; // RA2: as above
				}
				CCFileClass file(name); // RA2: as above
				CCINIClass * ini = new CCINIClass; // RA2: as above
				ini->Load(file, false, false); // RA2: as above
				if (stricmp(name, "RULES.INI") == 0) { // RA2: as above
					found = true; // RA2: as above
					files.Add_Head(ini); // RA2: as above
				} else {
					files.Add(ini); // RA2: as above
				}
			}
		} while (FindNextFile(handle, &data)); // RA2: as above
		FindClose(handle); // RA2: as above
	}
	if (!found) { // RA2: as above
		CCFileClass file("RULES.INI"); // RA2: as above
		CCINIClass * ini = new CCINIClass; // RA2: as above
		ini->Load(file, false, false); // RA2: as above
		files.Add_Head(ini); // RA2: as above
	}
	if (files.Count() <= 0) { // RA2: as above
		return(false); // RA2: as above
	}

	CCFileClass artfile("ART.INI"); // RA2: as above
	if (ArtINI.Load(artfile, false, false) == 0) { // RA2: as above
		DebugString("Failed to load ART.INI!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	CCINIClass * rules; // RA2: as above
	if (files.Count() == 1) { // RA2: as above
		rules = files[0]; // RA2: as above
	} else {
		int choice = DialogBoxParam(ProgramInstance, MAKEINTRESOURCE(0x93), MainWindow, Rules_Choice_Dialog_Proc, (LPARAM)&files); // RA2: as above
		if (choice == -1) { // RA2: as above
			choice = 0; // RA2: as above
		}
		rules = files[choice]; // RA2: as above
	}
	RuleINI = rules; // RA2: as above
	Rule->Color_Schemes(*rules); // RA2: as above
	Rule->Do_Movies(ArtINI); // RA2: as above
	Rule->Audio_Visual_Rules(*RuleINI); // RA2: as above
	Rule->MPlayer(*RuleINI); // RA2: as above

	Session.Options.Credits = Rule->MPMoney; // RA2: as above
	Session.Options.UnitCount = Rule->MPUnitCount; // RA2: as above
	BuildLevel = Rule->MPBuildLevel; // RA2: as above
	Session.Options.GameSpeed = Rule->GameSpeed; // RA2: as above
	Session.Options.AIDifficulty = (DiffType)Rule->AIDifficultyStruct; // RA2: as above
	Session.Options.AIPlayers = Rule->AIPlayers; // RA2: as above
	Session.Options.BridgeDestruction = Rule->IsMPBridgeDestruction; // RA2: as above
	Session.Options.Bases = Rule->IsMPBasesOn; // RA2: as above
	Session.Options.Goodies = Rule->IsMPCrates; // RA2: as above
	Session.Options.CTF = Rule->IsMPCaptureTheFlag; // RA2: as above
	Session.Options.HarvTruce = Rule->HarvesterTruce; // RA2: as above
	Session.Options.CrapEngineers = Rule->MultiEngineer; // RA2: as above
	Session.Options.AlliesAllowed = Rule->AlliesAllowed; // RA2: as above
	Session.Options.ShortGame = Rule->ShortGame; // RA2: as above
	Session.Options.SWAllowed = Rule->SuperWeaponsAllowed; // RA2: as above
	Session.Options.BuildOffAlly = Rule->BuildOffAlly; // RA2: as above
	Session.Options.FogOfWar = Rule->FogOfWar; // RA2: as above
	Session.Options.MCVRedeploy = Rule->MCVRedeploys; // RA2: as above

	CCFileClass langfile("LANGRULE.INI"); // RA2: as above
	if (langfile.Is_Available() == true) { // RA2: as above
		CCINIClass lang; // RA2: as above
		if (lang.Load(langfile, true, false) > 1) { // RA2: as above
			return(false); // RA2: as above
		}
		Rule->Addition(lang); // RA2: as above
	}

	for (int index = 0; index < files.Count(); index++) { // RA2: as above
		if (files[index] != RuleINI) { // RA2: as above
			delete files[index]; // RA2: as above
		}
	}

	CCFileClass aifile("AI.INI"); // RA2: as above
	AIINI.Load(aifile, true, false); // RA2: as above
	return(true); // RA2: as above
}


/***********************************************************************************************
 * Rules_Choice_Dialog_Proc -- Message handler of the dialog that chooses among RULE*.INI files.*
 *                                                                                             *
 *    Init_Rules opens this dialog when more than one rules file was found. The dialog's list  *
 *    shows each file's [General] Name; the position of the chosen entry is the result.        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this dialog; it is not in TibSun.                                                 *
 *=============================================================================================*/
BOOL CALLBACK Rules_Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00513080, not in TibSun
{
	char buffer[128]; // RA2: the executable reserves 128 bytes of locals

	switch (message) { // RA2: as above
		case WM_CONTEXTMENU: // RA2: as above
			On_WM_CONTEXTMENU(wparam); // RA2: as above
			return(0); // RA2: as above

		case WM_HELP: // RA2: as above
			On_WM_HELP(lparam); // RA2: as above
			return(0); // RA2: as above

		case WM_INITDIALOG: // RA2: as above
		{
			Center_Window_Within_Window(window); // RA2: as above
			HWND list = GetDlgItem(window, IDC_RULES_LIST); // RA2: the list box
			DynamicVectorClass<CCINIClass *> * files = (DynamicVectorClass<CCINIClass *> *)lparam; // RA2: Init_Rules passes its file list as the dialog parameter
			for (int index = 0; index < files->Count(); index++) { // RA2: as above
				(*files)[index]->Get_String("General", "Name", "", buffer, sizeof(buffer)); // RA2: as above
				SendMessage(list, LB_ADDSTRING, 0, (LPARAM)buffer); // RA2: as above
			}
			SendMessage(list, LB_SETCURSEL, 0, 0); // RA2: as above
			return(0); // RA2: as above
		}

		case WM_MOVING: // RA2: as above
			return(On_WM_MOVING(window, wparam, lparam)); // RA2: as above

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) { // RA2: as above
				case IDCANCEL: // RA2: as above
				case IDC_RULES_OK: // RA2: the OK button
					if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
						HWND list = GetDlgItem(window, IDC_RULES_LIST); // RA2: as above
						EndDialog(window, SendMessage(list, LB_GETCURSEL, 0, 0)); // RA2: as above
						DestroyWindow(window); // RA2: as above
					}
					break; // RA2: as above
			}
			break; // RA2: as above
	}
	return(0); // RA2: as above
}


/***********************************************************************************************
 * Two_Button_Dialog_Proc -- Message handler of a dialog with two result buttons.              *
 *                                                                                             *
 *    The executable never references this procedure. Cancel ends the dialog with 0, control   *
 *    0x53A with 1 and control 0x53B with 2.                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this procedure; it is not in TibSun.                                              *
 *=============================================================================================*/
BOOL CALLBACK Two_Button_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM) // RA2: 0x00513200, not in TibSun
{
	if (message == WM_COMMAND) { // RA2: as above
		switch (wparam) { // RA2: as above
			case IDCANCEL: // RA2: as above
				EndDialog(window, 0); // RA2: as above
				return(0); // RA2: as above

			case 0x53A: // RA2: as above
				EndDialog(window, 1); // RA2: as above
				return(0); // RA2: as above

			case 0x53B: // RA2: as above
				EndDialog(window, 2); // RA2: as above
				return(0); // RA2: as above
		}
	}
	return(0); // RA2: as above
}


/*
**	RA2: three empty functions that nothing in the executable references (0x00513260, 0x00513270 and 0x00513280).
*/
void Init_Unused_Stub_1(void) // RA2: 0x00513260, not in TibSun
{
}

void Init_Unused_Stub_2(void) // RA2: 0x00513270, not in TibSun
{
}

void Init_Unused_Stub_3(void) // RA2: 0x00513280, not in TibSun
{
}


/***********************************************************************************************
 * Single_Player_Dialog_Proc -- Message handler of the single player dialog.                   *
 *                                                                                             *
 *    RA2's main menu has a button that opens this dialog; its buttons start a campaign,       *
 *    load a game or start a skirmish. TibSun has no such dialog.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this dialog; it is not in TibSun.                                                 *
 *=============================================================================================*/
BOOL CALLBACK Single_Player_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x00513290, not in TibSun
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (rc) { // RA2: as above
		return(rc); // RA2: as above
	} // RA2: as above

	int * choice = (int *)GetWindowLong(window, DWL_USER); // RA2: as above

	switch (message) { // RA2: as above
		case WM_PAINT: // RA2: as above
		{
			HWND control = GetDlgItem(window, 0x71A); // RA2: as above
			SendMessage(control, WM_USER + 240, 0, 0); // RA2: as above
			break; // RA2: as above
		}

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) { // RA2: as above
				case 0x688: // RA2: as above
					*choice = MENU_NEW_CAMPAIGN; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x689: // RA2: as above
					*choice = MENU_LOAD_GAME; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x68A: // RA2: as above
					*choice = MENU_SINGLE_AGAIN; // RA2: as above
					return(FALSE); // RA2: as above

				case IDC_SKIRMISH: // RA2: as above
					*choice = MENU_SKIRMISH; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x686: // RA2: as above
					*choice = MENU_NONE; // RA2: as above
					return(FALSE); // RA2: as above
			}
			break; // RA2: as above

		case OD_SUBCLASSED: // RA2: as above
		{
			HWND control = GetDlgItem(window, 0x689); // RA2: as above
			if (control) { // RA2: as above
				bool saved = LoadOptionsClass().Files_Present(); // RA2: as above
				EnableWindow(control, saved); // RA2: as above
			} // RA2: as above
			break; // RA2: as above
		}
	}

	return(FALSE); // RA2: as above
}


/***********************************************************************************************
 * Extras_Dialog_Proc -- Message handler of the movies and credits dialog.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this dialog; it is not in TibSun.                                                 *
 *=============================================================================================*/
BOOL CALLBACK Extras_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005133E0, not in TibSun
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (rc) { // RA2: as above
		return(rc); // RA2: as above
	} // RA2: as above

	int * choice = (int *)GetWindowLong(window, DWL_USER); // RA2: as above

	switch (message) { // RA2: as above
		case WM_PAINT: // RA2: as above
		{
			HWND control = GetDlgItem(window, 0x71A); // RA2: as above
			SendMessage(control, WM_USER + 240, 0, 0); // RA2: as above
			break; // RA2: as above
		}

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) { // RA2: as above
				case 0x68D: // RA2: as above
					*choice = MENU_TRAILER; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x68E: // RA2: as above
					*choice = MENU_MOVIES; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x68F: // RA2: as above
					*choice = MENU_CREDITS; // RA2: as above
					return(FALSE); // RA2: as above

				case 0x686: // RA2: as above
					*choice = MENU_NONE; // RA2: as above
					return(FALSE); // RA2: as above
			}
			break; // RA2: as above
	}

	return(FALSE); // RA2: as above
}


/***********************************************************************************************
 * Movie_List_Dialog_Proc -- Message handler of the list of movies.                            *
 *                                                                                             *
 *    The dialog lists the movies the player has unlocked; the choice is the list entry the    *
 *    player picked, or -1.                                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this dialog; it is not in TibSun.                                                 *
 *=============================================================================================*/
BOOL CALLBACK Movie_List_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005134C0, not in TibSun
{
	bool chosen = false; // RA2: as above
	HWND list = GetDlgItem(window, 0x744); // RA2: as above

	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (rc) { // RA2: as above
		return(rc); // RA2: as above
	} // RA2: as above

	int * choice = (int *)GetWindowLong(window, DWL_USER); // RA2: as above

	switch (message) { // RA2: as above
		case OD_SUBCLASSED: // RA2: as above
			if (list != NULL) { // RA2: as above
				Options.Fill_Movie_List(list); // RA2: as above
				if (SendMessage(list, LB_SETCURSEL, MovieListChoice, 0) == LB_ERR) { // RA2: as above
					SendMessage(list, LB_SETCURSEL, 0, 0); // RA2: as above
				} // RA2: as above
			} else { // RA2: as above
				if (choice != NULL) { // RA2: as above
					*choice = -1; // RA2: as above
				} // RA2: as above
				return(FALSE); // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) { // RA2: as above
				case 0x686: // RA2: as above
					if (choice != NULL) { // RA2: as above
						*choice = -1; // RA2: as above
					} // RA2: as above
					break; // RA2: as above

				case 0x744: // RA2: as above
					if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
						chosen = true; // RA2: as above
					} // RA2: as above
					break; // RA2: as above

				case 0x745: // RA2: as above
					chosen = true; // RA2: as above
					break; // RA2: as above
			}
			break; // RA2: as above
	}

	if (list != NULL && chosen) { // RA2: as above
		int index = SendMessage(list, LB_GETCURSEL, 0, 0); // RA2: as above
		MovieListChoice = index; // RA2: as above
		if (index != LB_ERR) { // RA2: as above
			int movie = SendMessage(list, LB_GETITEMDATA, index, 0); // RA2: as above
			if (movie != 0 && choice != NULL) { // RA2: as above
				*choice = movie; // RA2: as above
			} // RA2: as above
		} // RA2: as above
	} // RA2: as above

	return(FALSE); // RA2: as above
}


struct ChooseCampaignStruct {
	CampaignType ChosenCampaign;
	bool ChoiceMade;
};

int PendingSideVoc = -1; // RA2: 0x007DB4F8, the side sound to play when the choice is made (-1 none)

struct CheatEntryStruct {
	bool * State;
	char const * CheatString;
	char const * VersionSuffix;	/// Suffix appended to version string
	bool IsAllowedInMP;
};

static CheatEntryStruct CheatEntries[] = {
	{ &VisceroidsAsSnoBees,    "PENGO",    " PG", false },
	{ &Just4Fun,               "THETEAM",  NULL,  false },
	{ &Options.AllowHiResModes,"HIRES",    " RS", false },
	{ &Debug_IngameModeChange, "TOGGLE",   NULL,  true  },
};
char * CampaignStartNames[3] = { "all1", "sov1", "tut1" }; // RA2: 0x007DB540, the first scenario of the Allied, Soviet and boot camp campaigns
int SovietSelectVoc = -1; // RA2: 0x007DB54C
int AlliedSelectVoc = -1; // RA2: 0x007DB550
int BootCampSelectVoc = -1; // RA2: 0x007DB554
int MovieListChoice = -1; // RA2: 0x007DB558, the list entry that was chosen last

BOOL CALLBACK Campaign_Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: 0x005147A0, after Select_Game
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, defined below
void Init_Random(void); // RA2: 0x00515710, defined below
bool Load_Recording_Values(CCFileClass & file); // RA2: 0x00516BA0, defined below
bool Save_Recording_Values(CCFileClass & file); // RA2: as above
bool Start_Network_Session(void); // RA2: 0x00514EE0, defined below
int WolGameResult = -1; // RA2: 0x007DB55C, 2 when a game is ready after the Internet dialogs, -1 when not

/***********************************************************************************************
 * Cheat_Disable -- Clears the cheats that are not allowed in a multiplayer game.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has this as a routine of its own; TibSun walks the table where it needs it.           *
 *=============================================================================================*/
static void Cheat_Disable(void)
{
	for (int code = 0; code < ARRAY_SIZE(CheatEntries); code++) { // RA2: as above
		if (!CheatEntries[code].IsAllowedInMP) { // RA2: as above
			*CheatEntries[code].State = false;
		}
	}
}


/// <summary>
/// Asks the player which campaign to play.
/// This routine reads the campaign list first if that has not already happened, and then
/// runs the campaign dialog until the player either commits or backs out.
/// </summary>
/// <returns>Returns with the campaign chosen, or CAMPAIGN_NONE if the player backed out.</returns>
static CampaignType Choose_Campaign(void)
{
	HWND dialog;
	struct ChooseCampaignStruct state;

	state.ChoiceMade = false;
	state.ChosenCampaign = CAMPAIGN_NONE;

	if (Campaigns.Count() == 0) {
		Init_Campaigns();

		if (Campaigns.Count() == 0) {
			return(CAMPAIGN_NONE);
		}
	}

	Load_Side_Animation_Art(); // RA2: not in TibSun: the animation the side buttons show
	SideSound.Init(); // RA2: as above
	SideSoundTime = 0; // RA2: as above
	HoveredSideButton = 0; // RA2: as above

	dialog = OwnerDraw::Begin_Dialog(IDD_CAMPAIGN, Campaign_Choice_Dialog_Proc, 0); // RA2: the campaign dialog (IDD_CAMPAIGN in TibSun)
	if (dialog != NULL) {
		SetWindowLong(dialog, DWL_USER, (LONG)&state); // RA2: as above
		Center_Window_Within_Window(dialog, MainWindow); // RA2: as above
		OwnerDraw::Display_Dialog(dialog);

		while (!state.ChoiceMade && OwnerDraw::Dialog_Message_Handler() != true) { // RA2: as above
			if (PendingSideVoc != VOC_NONE && SideSoundTime < AudioGetTime()) { // RA2: the hovered side's sound plays once the mouse has rested on the button
				SideSound.Stop(); // RA2: as above
				VocClass::PlayGlobal(PendingSideVoc, 1.0f, 0x2000, &SideSound); // RA2: as above
				PendingSideVoc = VOC_NONE; // RA2: as above
			}
			if (SurfacesRestored) { // RA2: as above
				SurfacesRestored = false; // RA2: as above
				Darken_Surface(NULL, true); // RA2: as above
				Update_Visible_Surface(false, HiddenSurface, NULL); // RA2: as above
			}
		}
		OwnerDraw::End_Dialog(dialog);
	}

	SideSoundTime = AudioGetTime() + 3000; // RA2: lets the side's sound play out
	while (AudioControllerGetEvent(&SideSound) != NULL && SideSoundTime > AudioGetTime()) { // RA2: as above
		Call_Back(); // RA2: as above
	}
	Free_Side_Animation_Art(); // RA2: as above

	return(state.ChosenCampaign);
}


#define Max(a, b) ((a) < (b) ? (b) : (a)) // RA2: the executable tests the first value against the second, not as the MAX macro does

/***********************************************************************************************
 * Select_Game -- The game's main menu                                                         *
 *                                                                                             *
 *    This routine runs the main menu and its dialogs until a game has been chosen, then       *
 *    starts the scenario. A failed start returns to the menu.                                 *
 *                                                                                             *
 * INPUT:   fade  -- unused                                                                    *
 *                                                                                             *
 * OUTPUT:  bool; Is a game going to be played?                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2's form of TibSun's routine.                                                           *
 *=============================================================================================*/
bool Select_Game(bool) // RA2: as above
{
	bool gameloaded;				// Has the game been loaded from the menu?
	int selection;					// the default selection
	bool process;					// false = break out of while loop

	Theme.Play_Song(Theme.From_Name("INTRO")); // RA2: the main menu theme is looked up by name; TibSun has this commented out

restart:
	gameloaded = false;
	process = true;

	/*
	**	[Re]set any globals that need it, in preparation for a new scenario
	*/
	GameActive = true;
	PlayerPtr = NULL;
	DoList.Init();
	OutList.Init();
	Frame = 0;
	Scen->MissionTimer = 0;
	Scen->MissionTimer.Stop();
	Scen->CDifficulty = DIFF_NORMAL;
	Scen->Difficulty = DIFF_NORMAL;
	PlayerWins = false;
	PlayerLoses = false;
	PlayerRestarts = false;
	PlayerAborts = false;
	Session.MultiplayerObserver = false; // RA2: as above
	Map.Set_Cursor_Shape(NULL);
	Map.PendingObjectPtr = 0;
	Map.PendingObject = 0;
	Map.PendingHouse = HOUSE_NONE;

	Session.ProcessTicks = 0;
	Session.ProcessFrames = 0;
	Session.DesiredFrameRate = 30;
	NewMaxAheadFrame1 = 0;
	NewMaxAheadFrame2 = 0;

	/*
	**	Init multiplayer game scores.  Let Wins accumulate; just init the current
	**	Kills for this game.  Kills of -1 means this player didn't play this round.
	*/
	for (int i = 0; i < MAX_MULTI_GAMES; i++) {
		Session.Score[i].Kills[Session.CurGame] = -1;
	}

	/*
	**	Set default mouse shape
	*/
	Map.Set_Default_Mouse(MOUSE_NORMAL, false);

	if (ToolTips != NULL) {
		ToolTips->Activate(false);
	}

	/*
	**	If the last game we played was a multiplayer game, jump right to that
	**	menu by pre-setting 'selection'.
	*/
	if (Session.Type == GAME_NORMAL) {
		selection = MENU_NONE; // RA2: as above
	} else if (Session.Type == GAME_INTERNET) {
		selection = MENU_INTERNET_RETURN; // RA2: as above
	} else {
		selection = MENU_MULTIPLAYER; // RA2: as above
	}

	/*
	**	Main menu processing; only do this if we're not in editor mode.
	*/
	if (!Debug_Map) {

		/*
		**	If we're playing back a recording, load all pertinent values & skip
		**	the menu loop.  Hide the now-useless mouse pointer.
		*/
		if (Session.Play && Session.RecordFile.Is_Available()) {
			if (Session.RecordFile.Open(FileClass::READ)) {
				Load_Recording_Values(Session.RecordFile);
				process = false;
				Theme.Stop(true);
			} else {
				Session.Play = false;
			}
		}

		while (process) {

			/*
			**	Display menu and fetch selection from player.
			*/
			if (PacketTransport != NULL) {
				if (Session.ViaPacketRouter) { // RA2: as above
					if (PacketRouterConnection != NULL) { // RA2: as above
						OwnerTalk.Send_Shutdown(); // RA2: as above
					}
					Session.ViaPacketRouter = false; // RA2: as above
				}
				delete PacketTransport;
				PacketTransport = NULL;
				Start_Network_Session(); // RA2: as above
			}

			if ((selection == MENU_NONE) && !Debug_ForceScenario) { // RA2: as above
				selection = Main_Menu(3600); // RA2: as above
			}

			if (Debug_ForceScenario) {
				selection = MENU_NEW_CAMPAIGN; // RA2: as above
			}

			if (selection != MENU_EXIT) { // RA2: as above
				Theme.Play_Song(Theme.From_Name("INTRO")); // RA2: as above
			}

			Call_Back();

			switch (selection) {

				case MENU_TIMEOUT: // RA2: as above
					if (Session.Attract && Session.RecordFile.Is_Available()) {
						Session.Play = true;
						if (Session.RecordFile.Open(FileClass::READ)) {
							Load_Recording_Values(Session.RecordFile);
							process = false;
							Theme.Stop(true);
							break;
						}
						Session.Play = false;
						selection = MENU_NONE; // RA2: as above
					} else {
						selection = MENU_NONE; // RA2: as above
					}
					break;

				case MENU_SINGLE_PLAYER: // RA2: as above
					ObserverMode = 0; // RA2: as above
					selection = Dialog_Box(0x100, Single_Player_Dialog_Proc, true); // RA2: as above
					break;

				case MENU_INTERNET: // RA2: as above
					selection = MENU_MULTIPLAYER; // RA2: as above
					Session.Type = GAME_INTERNET;
					Set_Address_Protocol(2); // RA2: as above
					break;

				case MENU_NETWORK: // RA2: as above
					selection = MENU_MULTIPLAYER; // RA2: as above
					Session.Type = GAME_IPX;
					Set_Address_Protocol(1); // RA2: as above
					break;

				case MENU_EXTRAS: // RA2: as above
					selection = Dialog_Box(0x101, Extras_Dialog_Proc, true); // RA2: as above
					break;

				case MENU_OPTIONS: // RA2: as above
					selection = MENU_NONE; // RA2: as above
					Main_Options_Dialog();
					break;

				case MENU_EXIT_ASK: { // RA2: as above
					wchar_t const * text = StringTable::LoadString("GUI:ExitAreYouSure", NULL, "D:\\Ra2\\Init.CPP", 2411); // RA2: as above
					wchar_t const * ok = StringTable::LoadString("GUI:Ok", NULL, "D:\\Ra2\\Init.CPP", 2412); // RA2: as above
					wchar_t const * cancel = StringTable::LoadString("GUI:Cancel", NULL, "D:\\Ra2\\Init.CPP", 2413); // RA2: as above
					Darken_Surface(NULL, true); // RA2: as above
					if (WWMessageBox(L" ").Process(text, ok, cancel, NULL, false) == 0) { // RA2: as above
						selection = MENU_EXIT; // RA2: as above
						Options.Save_Settings(); // RA2: as above
					} else {
						Darken_Surface(NULL, true); // RA2: as above
						selection = MENU_NONE; // RA2: as above
					}
					break;
				}

				case MENU_EXIT: { // RA2: as above
					Theme.Stop(true);
					long timeout = TickCount + 3000; // RA2: as above
					while (Theme.Still_Playing() && Is_Speaking() && timeout > TickCount) { // RA2: as above
						Call_Back();
					}
					Theme.Stop(false); // RA2: as above
					if (!HicolorFlag) {
						Set_Palette(BlackPalette, 30, NULL); // RA2: as above
					}
					return(false);
				}

				case MENU_TRAILER: // RA2: as above
					selection = MENU_EXTRAS; // RA2: as above
					if (MovieListShown) { // RA2: as above
						if (TrailerMovieIndex <= 0) { // RA2: as above
							TrailerMovieIndex = Movies.Count(); // RA2: as above
						}
						if (TrailerMovieIndex == Movies.Count()) { // RA2: as above
							TrailerMovieIndex--; // RA2: as above
						}
						Play_Movie((VQType)TrailerMovieIndex--, THEME_NONE, true, true, true); // RA2: as above
						Darken_Surface(NULL, true); // RA2: as above
					} else {
						Play_Movie("EMPRTRLR.BIK", THEME_NONE, true, true, true, false); // RA2: as above
						Darken_Surface(NULL, true); // RA2: as above
					}
					break;

				case MENU_MOVIES: { // RA2: as above
					MovieListEntry * movie = (MovieListEntry *)Dialog_Box(0x129, Movie_List_Dialog_Proc, true); // RA2: as above
					if (movie != (MovieListEntry *)-1 && movie != NULL) { // RA2: as above
						if (!CD().Force_Available((DiskID)movie->CD)) { // RA2: as above
							break;
						}
						Play_Movie(movie->Name, THEME_NONE, true, true, true, false); // RA2: as above
						Darken_Surface(NULL, true); // RA2: as above
					} else {
						selection = MENU_EXTRAS; // RA2: as above
					}
					break;
				}

				case MENU_CREDITS: // RA2: as above
					selection = MENU_EXTRAS; // RA2: as above
					Show_Credits(); // RA2: as above
					Theme.Queue_Song(Theme.From_Name("INTRO")); // RA2: as above
					Darken_Surface(NULL, true); // RA2: as above
					break;

				case MENU_NEW_CAMPAIGN: { // RA2: as above
					new (&Environment) EnvironmentClass;

					Scen->Campaign = Choose_Campaign();
					if (Scen->Campaign == CAMPAIGN_NONE) {
						process = true;
						selection = MENU_SINGLE_PLAYER; // RA2: as above
						break;
					}

					switch (Options.Difficulty) {
						case 0:
							Scen->CDifficulty = DIFF_HARD;
							Scen->Difficulty = DIFF_EASY;
							break;

						case 1:
							Scen->CDifficulty = DIFF_HARD;
							Scen->Difficulty = DIFF_NORMAL;
							break;

						case 2:
							Scen->CDifficulty = DIFF_NORMAL;
							Scen->Difficulty = DIFF_NORMAL;
							break;

						case 3:
							Scen->CDifficulty = DIFF_EASY;
							Scen->Difficulty = DIFF_NORMAL;
							break;

						case 4:
							Scen->CDifficulty = DIFF_EASY;
							Scen->Difficulty = DIFF_HARD;
							break;
					}

					Session.Type = GAME_NORMAL;
					process = false;
					break;
				}

				/*
				**	Load a saved game.
				*/
				case MENU_LOAD_GAME: // RA2: as above
					ScenarioInit++;
					Init_Campaigns();
					ScenarioInit--;
					if (LoadOptionsClass().Load()) {
						gameloaded = true;
						process = false;
					} else {
						selection = MENU_SINGLE_PLAYER; // RA2: as above
					}
					break;

				case MENU_SINGLE_AGAIN: // RA2: as above
					selection = MENU_SINGLE_PLAYER; // RA2: as above
					break;

				case MENU_SKIRMISH: // RA2: as above
					Session.Type = GAME_SKIRMISH;
					// fall through

				/*
				**	MENU_MULTIPLAYER: set 'Session.Type' to NULL-modem, modem, or
				**	network play.
				*/
				case MENU_MULTIPLAYER: // RA2: as above
				case MENU_INTERNET_RETURN: { // RA2: as above
					Session.Read_MultiPlayer_Settings();

					for (int house = 0; house < HouseTypes.Count(); house++) {
						HouseTypes[house]->Read_INI(*RuleINI);
					}

					Session.Suspended = 0;

					switch (Session.Type) {
						case GAME_NORMAL:
							selection = MENU_NONE; // RA2: as above
							continue;

						case GAME_SKIRMISH:
							if (!Skirmish_Mode_Dialog()) {
								Session.Type = GAME_NORMAL;
								selection = MENU_SINGLE_PLAYER; // RA2: as above
							}
							break;

						case GAME_NULL_MODEM:
						case GAME_MODEM: { // RA2: as above
							Cheat_Disable();
							Session.NetOpen = true;

							if (NullModem.Num_Connections()) {
								NullModem.Init_Send_Queue();
								ModemGuest guest; // RA2: as above

								if ((Session.Type == GAME_NULL_MODEM && Session.ModemType == MODEM_NULL_HOST) || (Session.Type == GAME_MODEM && Session.ModemType == MODEM_DIALER)) {
									if (!ModemHost().Dialog()) {
										Session.Type = GAME_NORMAL;
										selection = MENU_NONE; // RA2: as above
									}
								} else {
									if (!guest.Dialog()) { // RA2: as above
										Session.Type = GAME_NORMAL;
										selection = MENU_NONE; // RA2: as above
									}
								}
							} else {
								selection = MENU_NONE; // RA2: as above
								Session.Type = GAME_NORMAL;
								continue;
							}
							break;
						}

						/*
						**	Handle being spawned from WChat. Internet play based on IPX code.
						*/
						case GAME_INTERNET: {
							Cheat_Disable();
							DebugString("Init.cpp - GAME_INTERNET\n"); // RA2: as above
							memset(Session.WolPlayerState, 0, sizeof(Session.WolPlayerState)); // RA2: as above
							Session.RouterProbePort2 = 0; // RA2: as above
							bool success = Start_Network_Session(); // RA2: as above
							int result = Wonline_Main(selection == MENU_INTERNET_RETURN); // RA2: as above
							selection = MENU_MULTIPLAYER; // RA2: as above
							Session.NetOpen = false;

							switch (result) { // RA2: as above
								case -2: // RA2: as above
								case 0:
									delete PacketTransport;
									PacketTransport = NULL;
									selection = MENU_NONE; // RA2: as above
									Session.Type = GAME_NORMAL;
									Theme.Play_Song(Theme.From_Name("INTRO")); // RA2: as above
									break;

								case -1: // RA2: as above
									Theme.Stop(true);
									if (!HicolorFlag) {
										Set_Palette(BlackPalette, 30, NULL); // RA2: as above
									}
									delete PacketTransport;
									PacketTransport = NULL;
									return(false);

								case 1:
									DebugString("Return from WOL to play a %d player game\n", Session.Players.Count());
									if (success) {
										process = false;
										WolGameResult = 2; // RA2: as above
									} else {
										Session.Type = GAME_NORMAL;
										selection = MENU_NONE; // RA2: as above
										delete PacketTransport;
										PacketTransport = NULL;
										WolGameResult = -1; // RA2: as above
									}
									break;

								case 2:
									delete PacketTransport;
									PacketTransport = NULL;
									Session.Type = GAME_NORMAL;
									selection = MENU_SKIRMISH; // RA2: as above
									break;
							}
							break;
						}
					}

					switch (Session.Type) {
						case GAME_IPX: {
							Cheat_Disable();
							Session.Read_MultiPlayer_Settings();
							if (PacketTransport != NULL) {
								delete PacketTransport;
								PacketTransport = NULL;
							}

							for (int house = 0; house < HouseTypes.Count(); house++) {
								HouseTypes[house]->Read_INI(*RuleINI);
							}

							Session.Type = GAME_IPX;
							Session.CommProtocol = COMM_PROTOCOL_MULTI_E_COMP;
							if (PacketTransport == NULL) {
								PacketTransport = new IPXInterfaceClass;
								PacketTransport->Init(); // RA2: as above
								Darken_Surface(NULL, true); // RA2: as above
								if (Net2Init_Network() && Net_Join_Dialog()) { // RA2: as above
									process = false;
								} else {
									Session.Type = GAME_NORMAL;
									selection = MENU_NONE; // RA2: as above
									delete PacketTransport;
									PacketTransport = NULL;
								}
							}
							break;
						}

						case GAME_MODEM:
						case GAME_NULL_MODEM:
						case GAME_INTERNET:
						case GAME_SKIRMISH:
							process = false;
							break;
					}
					break;
				}
			}
		}
	}

	Keyboard->Clear();
	Init_Random();
	Planning_Reset_State(); // RA2: as above

	if (!gameloaded && !Session.LoadGame) {
		DebugString("About to load a %d player game.\n", Session.Players.Count()); // RA2: as above
		MouseCursor->Hide_Mouse(); // RA2: as above
		if (selection != MENU_NEW_CAMPAIGN) { // RA2: as above
			if (!HicolorFlag) {
				Set_Palette(BlackPalette, 15, Call_Back); // RA2: as above
			}
			HiddenSurface->Fill(0);
			Update_Visible_Surface(true, HiddenSurface, NULL); // RA2: as above
		}
		MouseCursor->Show_Mouse(); // RA2: as above

		if (Session.Type == GAME_INTERNET) {
			for (int player = 0; player < 8; player++) { // RA2: as above
				Session.PlayerLastTime[player] = TickCount; // RA2: as above
			}
		}

		if (Session.Type == GAME_NORMAL && !Debug_ForceScenario && !Session.Play) { // RA2: as above
			if (!Start_Scenario(Campaigns[Scen->Campaign]->ScenarioName, true, Scen->Campaign)) {
				if (Debug_Map) {
					return(false);
				}
				goto restart;
			}
		} else if (!Start_Scenario(Scen->ScenarioName, true, CAMPAIGN_NONE)) { // RA2: as above
			if (Debug_Map) {
				return(false);
			}
			goto restart;
		}

		if (Session.Record) {
			if (Session.RecordFile.Open(FileClass::WRITE)) {
				Save_Recording_Values(Session.RecordFile);
			} else {
				Session.Record = false;
			}
		}
		Special.IsFromInstall = false;
	}

	DebugString("IsTGrowth = %d\n", Special.IsTGrowth);
	DebugString("IsTSpread = %d\n", Special.IsTSpread);

	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session.Play) {
		Session.Create_Connections();
		if (Session.Type == GAME_IPX) {
			Ipx.Set_Timing(Max(Ipx.Global_Response_Time() + 2, 15), (unsigned int)-1, 600, true); // RA2: as above
			Ipx.Set_External_Timing(Max(Ipx.Global_Response_Time() + 2, 60), (unsigned int)-1, 600); // RA2: as above
		} else if (Session.Type == GAME_INTERNET && !Session.PrecalcDesiredFrameRate) { // RA2: as above
			Ipx.Set_Timing(Max(Ipx.Global_Response_Time() + 2, 60), (unsigned int)-1, 600, true); // RA2: as above
		}
	}

	Call_Back();
	MouseCursor->Hide_Mouse(); // RA2: as above
	if (!HicolorFlag) {
		Set_Palette(BlackPalette, 15, Call_Back); // RA2: as above
	}
	HiddenSurface->Fill(0);
	Update_Visible_Surface(true, HiddenSurface, NULL); // RA2: as above
	MouseCursor->Show_Mouse(); // RA2: as above
	LogicalSurface = HiddenSurface;
	Map.Override_Mouse_Shape(MOUSE_NO_MOVE, false); // RA2: as above
	Map.Revert_Mouse_Shape();
	if (!Debug_Map) {
		Map.Activate(1);
	}
	Map.Flag_To_Redraw();
	Call_Back();
	MouseCursor->Hide_Mouse(); // RA2: as above
	return(true);
}

#undef Max // RA2: as above


/***********************************************************************************************
 * Campaign_Choice_Dialog_Proc -- Message handler of the campaign choice dialog.               *
 *                                                                                             *
 *    The dialog has a button for each side's campaign (Allied 0x6EA, Soviet 0x6EC and boot    *
 *    camp 0x6EB), which highlights under the mouse and plays the side's sound when chosen;    *
 *    the list control 0x455 holds the campaigns and the slider 0x50F the difficulty.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   RA2 has the side buttons; TibSun's dialog only has the list.                              *
 *=============================================================================================*/
BOOL CALLBACK Campaign_Choice_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x005147A0
{
	HWND item;
	struct ChooseCampaignStruct * state;

	int rc;
	rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);

	if (rc) {
		return(rc);
	}

	switch (message) {

		case WM_HSCROLL: // RA2: the difficulty label follows the slider
			if ((HWND)lparam == GetDlgItem(window, IDC_DIFFICULTY_SLIDER)) { // RA2: as above
				char const * label = Difficulty_Labels[wparam >> 16]; // RA2: as above
				item = GetDlgItem(window, IDC_DIFFICULTY_LABEL); // RA2: as above
				SendMessage(item, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString(label, NULL, "D:\\Ra2\\Init.CPP", 1193)); // RA2: as above
				return(FALSE); // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case 0x40E: // RA2: the OK button
					if (HIWORD(wparam) == BN_CLICKED) {
						state = (ChooseCampaignStruct *)GetWindowLong(window, DWL_USER);

						if (state != NULL) {
							item = GetDlgItem(window, IDC_LIST); // RA2: the campaign list
							if (item != NULL) {
								state->ChosenCampaign = (CampaignType)SendMessage(item, LB_GETCURSEL, 0, 0); // RA2: the list holds the available campaigns in order
								state->ChoiceMade = true;
							}
						}

						item = GetDlgItem(window, IDC_DIFFICULTY_SLIDER); // RA2: the difficulty slider

						if (item != NULL) {
							Options.Difficulty = SendMessage(item, TBM_GETPOS, 0, 0); // RA2: as above
							return(FALSE); // RA2: as above
						}
					}
					break;

				case 0x686: // RA2: back
					state = (ChooseCampaignStruct *)GetWindowLong(window, DWL_USER);

					if (state != NULL) {
						state->ChosenCampaign = CAMPAIGN_NONE;
						state->ChoiceMade = true;
						return(FALSE); // RA2: as above
					}
					break;
			}
			break;

		case WM_NCHITTEST: // RA2: highlights the side button under the mouse; not in TibSun
			if (PressedSideButton == NULL) { // RA2: as above
				POINT point; // RA2: as above
				point.x = LOWORD(lparam); // RA2: as above
				point.y = HIWORD(lparam); // RA2: as above
				RECT rect; // RA2: as above
				GetWindowRect(window, &rect); // RA2: as above
				point.x -= rect.left; // RA2: as above
				point.y -= rect.top; // RA2: as above
				HWND child = ChildWindowFromPointEx(window, point, CWP_SKIPINVISIBLE); // RA2: as above
				if (child != NULL) { // RA2: as above
					int id = GetDlgCtrlID(child); // RA2: as above
					if (HoveredSideButton != id) { // RA2: as above
						SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
						SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
						SideSoundTime = AudioGetTime() + 500; // RA2: as above
						PendingSideVoc = VOC_NONE; // RA2: as above
						HoveredSideButton = 0; // RA2: as above
					} // RA2: as above
					if (id == 0x6EA) { // RA2: as above
						if (HoveredSideButton != id) { // RA2: as above
							PendingSideVoc = AlliedSelectVoc; // RA2: as above
						} // RA2: as above
						SendDlgItemMessage(window, 0x6EA, WM_USER + 211, 0, 0); // RA2: as above
						HoveredSideButton = 0x6EA; // RA2: as above
						return(FALSE); // RA2: as above
					} // RA2: as above
					if (id == 0x6EC) { // RA2: as above
						if (HoveredSideButton != id) { // RA2: as above
							PendingSideVoc = SovietSelectVoc; // RA2: as above
						} // RA2: as above
						SendDlgItemMessage(window, 0x6EC, WM_USER + 211, 0, 0); // RA2: as above
						HoveredSideButton = 0x6EC; // RA2: as above
						return(FALSE); // RA2: as above
					} // RA2: as above
					if (id == 0x6EB) { // RA2: as above
						if (HoveredSideButton != id) { // RA2: as above
							PendingSideVoc = BootCampSelectVoc; // RA2: as above
						} // RA2: as above
						SendDlgItemMessage(window, 0x6EB, WM_USER + 211, 0, 0); // RA2: as above
						HoveredSideButton = 0x6EB; // RA2: as above
						return(FALSE); // RA2: as above
					} // RA2: as above
				} else if (HoveredSideButton != 0) { // RA2: as above
					SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
					SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
					return(FALSE); // RA2: as above
				} // RA2: as above
			} // RA2: as above
			break; // RA2: as above

		case WM_LBUTTONDOWN: // RA2: remembers the side button the mouse went down on
		{
			POINT point; // RA2: as above
			point.x = LOWORD(lparam); // RA2: as above
			point.y = HIWORD(lparam); // RA2: as above
			RECT rect; // RA2: as above
			GetWindowRect(window, &rect); // RA2: as above
			point.x -= rect.left; // RA2: as above
			point.y -= rect.top; // RA2: as above
			HWND child = ChildWindowFromPointEx(window, point, CWP_SKIPINVISIBLE); // RA2: as above
			if (child != NULL) { // RA2: as above
				int id = GetDlgCtrlID(child); // RA2: as above
				if (id == 0x6EA || id == 0x6EC || id == 0x6EB) { // RA2: as above
					PressedSideButton = child; // RA2: as above
					SetCapture(window); // RA2: as above
					return(FALSE); // RA2: as above
				} // RA2: as above
			} // RA2: as above
			break; // RA2: as above
		}

		case WM_LBUTTONUP: // RA2: chooses the side's first campaign when the button is released over the one that was pressed
		{
			POINT point; // RA2: as above
			point.x = LOWORD(lparam); // RA2: as above
			point.y = HIWORD(lparam); // RA2: as above
			RECT rect; // RA2: as above
			GetWindowRect(window, &rect); // RA2: as above
			point.x -= rect.left; // RA2: as above
			point.y -= rect.top; // RA2: as above
			HWND child = ChildWindowFromPointEx(window, point, CWP_SKIPINVISIBLE); // RA2: as above
			if (child != NULL) { // RA2: as above
				int id = GetDlgCtrlID(child); // RA2: as above
				if (child == PressedSideButton) { // RA2: as above
					state = (ChooseCampaignStruct *)GetWindowLong(window, DWL_USER); // RA2: as above
					if (state != NULL) { // RA2: as above
						if (id == 0x6EA) { // RA2: as above
							state->ChosenCampaign = CampaignClass::From_Name(CampaignStartNames[0]); // RA2: as above
							state->ChoiceMade = true; // RA2: as above
						} else if (id == 0x6EC) { // RA2: as above
							state->ChosenCampaign = CampaignClass::From_Name(CampaignStartNames[1]); // RA2: as above
							state->ChoiceMade = true; // RA2: as above
						} else if (id == 0x6EB) { // RA2: as above
							state->ChosenCampaign = CampaignClass::From_Name(CampaignStartNames[2]); // RA2: as above
							state->ChoiceMade = true; // RA2: as above
						} // RA2: as above
						if (state->ChoiceMade) { // RA2: as above
							ScenarioSaveNameSet = false; // RA2: as above
							SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
							SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
							item = GetDlgItem(window, IDC_DIFFICULTY_SLIDER); // RA2: as above
							if (item != NULL) { // RA2: as above
								Options.Difficulty = SendMessage(item, TBM_GETPOS, 0, 0); // RA2: as above
							} // RA2: as above
							if (PendingSideVoc != VOC_NONE) { // RA2: as above
								SideSound.Stop(); // RA2: as above
								VocClass::PlayGlobal(PendingSideVoc, 1.0f, 0x2000, &SideSound); // RA2: as above
								PendingSideVoc = VOC_NONE; // RA2: as above
							} // RA2: as above
						} // RA2: as above
					} // RA2: as above
				} else if (id == 0x6EA || id == 0x6EC || id == 0x6EB) { // RA2: as above
					if (HoveredSideButton != 0) { // RA2: as above
						SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
						SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
					} // RA2: as above
					SendDlgItemMessage(window, id, WM_USER + 213, 0, 0); // RA2: as above
					SendDlgItemMessage(window, id, WM_USER + 212, 0, 0); // RA2: as above
					HoveredSideButton = id; // RA2: as above
				} else if (HoveredSideButton != 0) { // RA2: as above
					SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
					SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
					HoveredSideButton = 0; // RA2: as above
				} // RA2: as above
			} else if (HoveredSideButton != 0) { // RA2: as above
				SendDlgItemMessage(window, HoveredSideButton, WM_USER + 213, 0, 0); // RA2: as above
				SendDlgItemMessage(window, HoveredSideButton, WM_USER + 212, 0, 0); // RA2: as above
				HoveredSideButton = 0; // RA2: as above
			} // RA2: as above
			if (GetCapture() == window) { // RA2: as above
				ReleaseCapture(); // RA2: as above
			} // RA2: as above
			PressedSideButton = NULL; // RA2: as above
			break; // RA2: as above
		}

		case OD_SUBCLASSED: // RA2: the dialog is ready
		{
			SovietSelectVoc = VocClass::From_Name("SovietCampaignSelect"); // RA2: as above
			AlliedSelectVoc = VocClass::From_Name("AlliedCampaignSelect"); // RA2: as above
			BootCampSelectVoc = VocClass::From_Name("BootCampSelect"); // RA2: as above
			item = GetDlgItem(window, IDC_LIST); // RA2: the campaign list
			if (CampaignListShown) { // RA2: as above
				if (item != NULL) {
					for (int index = 0; index < Campaigns.Count(); index++) {
						SendMessage(item, OD_ADDSTRINGW, 0, (LPARAM)Campaigns[index]->Description); // RA2: wide text
					}
					SendMessage(item, LB_SETCURSEL, 0, 0); // RA2: as above
				}
			} else { // RA2: as above
				ShowWindow(item, SW_HIDE); // RA2: as above
				HWND button = GetDlgItem(window, 0x40E); // RA2: as above
				ShowWindow(button, SW_HIDE); // RA2: as above
				EnableWindow(button, FALSE); // RA2: as above
			} // RA2: as above

			item = GetDlgItem(window, IDC_DIFFICULTY_SLIDER); // RA2: the difficulty slider
			if (item != NULL) {
				PostMessage(item, OD_TRACKNUMBERS, 0, 0); // RA2: as above
				PostMessage(item, TBM_SETRANGE, TRUE, MAKELONG(0, 2)); // RA2: as above
				PostMessage(item, TBM_SETPOS, TRUE, Options.Difficulty); // RA2: as above
			}

			SendDlgItemMessage(window, 0x6EA, WM_USER + 213, 0, 0); // RA2: as above
			SendDlgItemMessage(window, 0x6EA, WM_USER + 212, 0, 0); // RA2: as above
			SendDlgItemMessage(window, 0x6EC, WM_USER + 213, 0, 0); // RA2: as above
			SendDlgItemMessage(window, 0x6EC, WM_USER + 212, 0, 0); // RA2: as above
			SendDlgItemMessage(window, 0x6EB, WM_USER + 213, 0, 0); // RA2: as above
			SendDlgItemMessage(window, 0x6EB, WM_USER + 212, 0, 0); // RA2: as above
			return(FALSE); // RA2: as above
		}
	}

	return(FALSE);
}



/***********************************************************************************************
 * Start_Network_Session -- Opens the UDP network for a multiplayer game.                      *
 *                                                                                             *
 *    RA2 has this routine, TibSun does not. It reads every house's rules, creates the UDP     *
 *    interface and its socket, initializes the network code, clears the per-player port and   *
 *    connection state of the session, picks the base port for the game and writes the port    *
 *    base and pool to RA2.INI.                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Did Init_Network succeed?                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *=============================================================================================*/
bool Start_Network_Session(void) // RA2: 0x00514EE0, not in TibSun
{
	if (PacketTransport != NULL) { // RA2: as above
		delete PacketTransport; // RA2: as above
		PacketTransport = NULL; // RA2: as above
	}

	for (int house = 0; house < HouseTypes.Count(); house++) { // RA2: as above
		HouseTypes[house]->Read_INI(*RuleINI); // RA2: as above
	}

	if (PacketTransport == NULL) { // RA2: as above
		PacketTransport = new UDPInterfaceClass; // RA2: as above
	}

	Session.CommProtocol = COMM_PROTOCOL_MULTI_E_COMP; // RA2: as above
	PacketTransport->Open_Socket(0); // RA2: as above
	Ipx.Set_Timing(30, (unsigned int)-1, 600, true); // RA2: as above
	DebugString("About to initialise the network code\n"); // RA2: as above
	bool success = Init_Network(); // RA2: as above
	PacketTransport->Start_Listening(); // RA2: as above

	memset(Session.PlayerPorts, 0, sizeof(Session.PlayerPorts)); // RA2: as above
	Session.RouterProbeActive = false; // RA2: as above
	Session.PortProbeDue = false; // RA2: as above
	Session.PacketRouterPort = 0; // RA2: as above
	Session.RouterProbePort = 0; // RA2: as above
	Session.SlowestResponse = 0; // RA2: as above
	Session.LastTimingID = -1; // RA2: as above
	Session.RouterResponse = 20; // RA2: as above
	Session.ExitEventPending = 0; // RA2: as above
	Session.ExitEventFrame = 0; // RA2: as above
	Session.ExitEventPlayer = -1; // RA2: as above
	memset(Session.PeerProbeData, 0, sizeof(Session.PeerProbeData)); // RA2: as above
	memset(Session.ConnectionResults, -1, sizeof(Session.ConnectionResults)); // RA2: as above
	memset(Session.PeerPorts, 0, sizeof(Session.PeerPorts)); // RA2: as above
	memset(Session.PlayerPortBase, 0, sizeof(Session.PlayerPortBase)); // RA2: as above
	Session.ConnectionResultCount = 0; // RA2: as above

	if (Session.ForcePortBase) { // RA2: as above
		Session.PortBase = Session.ForcePortBase; // RA2: as above
	} else if (Session.RotatePortBase) { // RA2: as above
		Session.PortBase += 5; // RA2: as above
		if (Session.PortBase >= 1750) { // RA2: as above
			Session.PortBase = 1250; // RA2: as above
		} // RA2: as above
		Session.RotatePortBase = false; // RA2: as above
	} // RA2: as above

	RawFileClass file("RA2.INI"); // RA2: as above
	ConfigINI.Put_Int("MultiPlayer", "PortBase", Session.PortBase); // RA2: as above
	ConfigINI.Put_Int("MultiPlayer", "PortPool", FirewallHelper.Port); // RA2: as above
	ConfigINI.Save(file, false); // RA2: as above
	OwnerTalk.Reset(); // RA2: as above
	Session.NetOpen = true; // RA2: as above
	return(success); // RA2: as above
}


/***********************************************************************************************
 * Anim_Init -- Initialize the VQ animation control structure.                                 *
 *                                                                                             *
 *    VQ animations are controlled by a structure passed to the VQ player. This routine        *
 *    initializes the structure to values required by C&C.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only need to call this routine once at the beginning of the game.               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Anim_Init(void)
{
	Initialize_Options();
	Set_Option(OPTION_PLAY_FROM_MIXFILE);
}



/***********************************************************************************************
 * Parse_Command_Line -- Parses the command line parameters.                                   *
 *                                                                                             *
 *    This routine should be called before the graphic mode is initialized. It examines the    *
 *    command line parameters and sets the appropriate globals. If there is an error, then     *
 *    it outputs a command summary and then returns false.                                     *
 *                                                                                             *
 * INPUT:   argc  -- The number of command line arguments.                                     *
 *                                                                                             *
 *          argv  -- Pointer to character string array that holds the individual arguments.    *
 *                                                                                             *
 * OUTPUT:  bool; Was the command line parsed successfully?                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Parse_Command_Line(int argc, char * argv[])
{
	/*
	**	Parse the command line and set globals to reflect the parameters
	**	passed in.
	*/
	//Whom = HOUSE_GOOD;
	Special.Init();

	Debug_Map = false;

	for (int index = 1; index < argc; index++) {
		char * string;		// Pointer to argument.
		string = strupr(argv[index]);

		if (stricmp("-jabber", string) == 0) { // RA2: -jabber, -str, -nostr, -noaudio, -record and -play are not in TibSun; the executable tests them first
			Set_Jabber_Option(1); // RA2: as above
			continue; // RA2: as above
		}
		if (stricmp("-str", string) == 0) { // RA2: as above
			Set_Str_Option(1); // RA2: as above
			continue; // RA2: as above
		}
		if (stricmp("-nostr", string) == 0) { // RA2: as above
			Set_Str_Option(0); // RA2: as above
			continue; // RA2: as above
		}
		if (stricmp("-noaudio", string) == 0) { // RA2: as above
			AudioAllowed = 0; // RA2: as above
			continue; // RA2: as above
		}
		if (stricmp("-record", string) == 0) { // RA2: as above
			RecordPlayRequest = 1; // RA2: as above
			continue; // RA2: as above
		}
		if (stricmp("-play", string) == 0) { // RA2: as above
			RecordPlayRequest = 2; // RA2: as above
			continue; // RA2: as above
		}

		/*
		**	Print usage text only if requested.
		*/
		if (stricmp("/?", string) == 0 || stricmp("-?", string) == 0 || stricmp("-h", string) == 0 || stricmp("/h", string) == 0) {
			/*
			**	Unrecognized command line parameter... Display usage
			**	and then exit.
			*/
			_putws(StringTable::LoadString("TXT_OPTION_HELP_01", NULL, "D:\\Ra2\\Init.CPP", 3342)); // RA2: wide text, with the executable's file name and line
			_putws(StringTable::LoadString("TXT_OPTION_HELP_02", NULL, "D:\\Ra2\\Init.CPP", 3343)); // RA2: as above
			_putws(StringTable::LoadString("TXT_OPTION_HELP_03", NULL, "D:\\Ra2\\Init.CPP", 3344)); // RA2: as above
			_putws(StringTable::LoadString("TXT_OPTION_HELP_04", NULL, "D:\\Ra2\\Init.CPP", 3345)); // RA2: as above
			_putws(StringTable::LoadString("TXT_OPTION_HELP_05", NULL, "D:\\Ra2\\Init.CPP", 3346)); // RA2: as above
			_putws(StringTable::LoadString("TXT_OPTION_HELP_06", NULL, "D:\\Ra2\\Init.CPP", 3347)); // RA2: as above
			return(false);
		}


		bool processed = true;
		int ob = Obfuscate(string);

		switch (ob) {

			/*
			**	Special flag - is C&C being run from the install program?
			*/
			case PARM_INSTALL:
				Special.IsFromInstall = true;
// If uncommented, will disable the <ESC> key during the first movie run.
//				BreakoutAllowed = false;
				break;

			default:
				processed = false;
				break;
		}
		if (processed) continue;

		/*
		**	File search path override.
		*/
		if (strstr(string, "-CD")) { // RA2: in every build, not only the debug ones
			CD::Override_Swap(true); // RA2: as above
			CDFileClass::Set_Search_Drives(&string[3]); // RA2: the executable calls CDFileClass's
			continue; // RA2: as above
		}

		/*
		**	Specify destination connection for network play
		*/
		if (strstr(string, "-DESTNET")) {
			NetNumType net;
			NetNodeType node;

			/*
			**	Scan the command-line string, pulling off each address piece
			*/
			int i = 0;
			char * p = strtok(string + 8, ".");

			while (p) {
				int x;

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
				Session.BridgeNet = IPXAddressClass(net, node, 0); // RA2: IPXAddressClass has only the (net, node, port) constructor
			}
			continue;
		}

		/*
		**	Specify socket ID, as an offset from 0x4000.
		*/
		if (strstr(string, "-SOCKET")) {
			unsigned short socket;

			socket = (unsigned short)(atoi(string + strlen("SOCKET")));
			socket += (unsigned short)0x4000;
			if (socket >= 0x4000 && socket < 0x8000) {
				Ipx.Set_Socket(socket);
			}
			continue;
		}

		Set_Packet_Loss_Percent(0); // RA2: not in TibSun (it has -TIME= here): every other argument first resets the packet loss

		if (strstr(string, "-MPDEBUG")) {
			ExtraSyncInfo = true; // RA2: TibSun sets Session.ShowInternetDebug
			Session.EnableMultiplayerDebug = true; // RA2: as above
		}

		if (strstr(string, "-DROP=")) { // RA2: not in TibSun: drop that percentage of received packets
			Set_Packet_Loss_Percent((unsigned short)atoi(string + strlen("-DROP="))); // RA2: as above
			continue; // RA2: as above
		}

		if (strstr(string, "-SPEEDCONTROL")) { // RA2: not in TibSun: keeps the game speed sliders visible
			GameControlsShown = true; // RA2: as above
		}
		if (strstr(string, "-DLINK1")) { // RA2: not in TibSun: router type 1
			RouterType = 1; // RA2: as above
		}
		if (strstr(string, "-NETGEAR")) { // RA2: not in TibSun: router type 2
			RouterType = 2; // RA2: as above
		}
		if (strstr(string, "-NOROUTER")) { // RA2: not in TibSun: do not allow for a router
			Session.UseRouter = false; // RA2: as above
		}

		/*
		**	Set the Net Stealth option
		*/
		if (strstr(string, "-STEALTH")) {
			Session.NetStealth = true;
			continue;
		}

		/*
		**	Set the Net Protection option
		*/
		if (strstr(string, "-MESSAGES")) {
			Session.NetProtect = false;
			continue;
		}

		/*
		**	Allow "attract" mode
		*/
		if (strstr(string, "-ATTRACT")) {
			Session.Attract = true;
			continue;
		}

		if (isdigit(string[1])) {
			sscanf(string, "-%dX%d", &Options.ScreenWidth, &Options.ScreenHeight);
			continue;
		}

		/*
		**	Set screen to 640x480 instead of 640x400
		*/
		if (strcmp(string, "-480") == 0) {
			Options.ScreenHeight = 480;
			continue;
		}

		if (strcmp(string, "-16") == 0) {
			HicolorFlag = true;
			continue;
		}

		if (stricmp(string, "-WIN") == 0) {
			WindowedMode = true;
			continue;
		}

		/*
		**	Special command line control parsing.
		*/
		if (strnicmp(string, "-X", strlen("-O")) == 0) {
			string += strlen("-X");
			while (*string) {
				char code = *string++;
				switch (toupper(code)) {

					/*
					**	Quiet mode
					*/
					case 'Q':
						Debug_Quiet = true;
						break;

					default:
						_putws(StringTable::LoadString("TXT_INVALID", NULL, "D:\\Ra2\\Init.CPP", 3660)); // RA2: wide text, with the executable's file name and line
						return(false);
				}
			}
			continue;
		}
	}

	return(true);
}

/***************************************************************************
 * Init_Random -- Initializes the random-number generator                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/04/1995 BRR : Created.                                             *
 *=========================================================================*/
void Init_Random(void)
{
	DebugString("Init random number\n");
	/*
	**	Do nothing if we've loaded a multiplayer game, or we're playing back
	**	a recording; the random number generator is initialized by loading
	**	the game.
	*/
	if (Session.LoadGame || Session.Play || Get_Input_Record_Mode()) { // RA2: a recording that is being made or played back (0x00522E80 Get_Input_Record_Mode) also keeps the seed as it is
		Scen->RandomNumber = Seed;
		NonCriticalRandomNumber = Seed;
		DebugString("Seed is %08x\n", Seed);
		return;
	}

	/*
	**	Initialize the random number Seed.  For multiplayer, this will have been done
	**	in the connection dialogs.  For single-player games, AND if we're not playing
	**	back a recording, init the Seed to a random value.
	*/
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {

		/*
		**	Gather some "random" bits from the system timer. Actually, only the
		**	low order millisecond bits are secure. The other bits could be
		**	easily guessed from the system clock (most clocks are fairly accurate
		**	and thus predictable).
		*/
		SYSTEMTIME t;
		GetSystemTime(&t);
		CryptRandom.Seed_Byte(t.wMilliseconds);
		CryptRandom.Seed_Bit(t.wSecond);
		CryptRandom.Seed_Bit(t.wSecond>>1);
		CryptRandom.Seed_Bit(t.wSecond>>2);
		CryptRandom.Seed_Bit(t.wSecond>>3);
		CryptRandom.Seed_Bit(t.wSecond>>4);
		CryptRandom.Seed_Bit(t.wMinute);
		CryptRandom.Seed_Bit(t.wMinute>>1);
		CryptRandom.Seed_Bit(t.wMinute>>2);
		CryptRandom.Seed_Bit(t.wMinute>>3);
		CryptRandom.Seed_Bit(t.wMinute>>4);
		CryptRandom.Seed_Bit(t.wHour);
		CryptRandom.Seed_Bit(t.wDay);
		CryptRandom.Seed_Bit(t.wDayOfWeek);
		CryptRandom.Seed_Bit(t.wMonth);
		CryptRandom.Seed_Bit(t.wYear);

		/*
		**	Set the optional user-specified seed
		*/
		if (CustomSeed != 0) {
			Seed = CustomSeed;
		} else {
			CryptRandom.Get(&Seed, sizeof(Seed));
			Seed = GetTickCount();
			//srand(time(NULL));
			//Seed = rand();
		}
	}

	/*
	**	Initialize the random-number generators
	*/
	DebugString("Seed is %08x\n", Seed);
	Scen->RandomNumber = Seed;
	NonCriticalRandomNumber = Seed;
}


/// <summary>
/// Dims a surface and shows it.
/// This routine is used to darken the screen behind a dialog box. A null surface means the
/// hidden surface.
/// </summary>
/// <param name="surface">The surface to darken, or NULL for the hidden surface.</param>
/// <param name="update">Should the result be copied to the visible surface?</param>
void Darken_Surface(Surface * surface, bool update) // RA2: 0x005159B0, not in TibSun: dims the screen behind a dialog; update copies the result to the visible surface
{
	if (surface == NULL) { // RA2: as above
		surface = HiddenSurface; // RA2: as above
	}
	surface->Fill(0); // RA2: as above
	int width = surface->Get_Width(); // RA2: as above
	int height = surface->Get_Height(); // RA2: as above
	Rect rect(0, 0, width, height); // RA2: as above
	BSurface * work = new BSurface(width, height, 2); // RA2: as above
	Draw_Interface_Screen(work, rect, false); // RA2: as above
	surface->Blit_From(*work, false, true); // RA2: as above
	delete work; // RA2: as above
	if (update) { // RA2: as above
		Update_Visible_Surface(false, surface, NULL); // RA2: as above
	}
}



/***********************************************************************************************
 * Load_Title_Page -- Load the background art for the title page.                              *
 *                                                                                             *
 *    This routine will load the background art in a machine independent format. There is      *
 *    different art required for the hi-res and lo-res versions of the game.                   *
 *                                                                                             *
 * INPUT:   visible  -- Should the title page art be copied to the visible page by this        *
 *                      routine?                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure the mouse is hidden if the image is to be copied to the visible page.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void Load_Title_Page(const char * name, bool visible)
{
	HiddenSurface->Fill(0);
	Load_Title_Screen(name, HiddenSurface, &CCPalette);
	if (visible)
		Update_Visible_Surface(true, HiddenSurface);
}

/// <summary>
/// Clears the hidden surface to black.
/// </summary>
/// <param name="visible">Should the cleared surface be copied to the visible one as well?</param>
void Blank_Hidden_Surface(bool visible) // RA2: 0x00515AC0, not in TibSun: clears the hidden surface and, if visible, copies it to the screen (YR: Blank_Hidden_Surface)
{
	HiddenSurface->Fill(0); // RA2: as above
	if (visible) { // RA2: as above
		Update_Visible_Surface(true, HiddenSurface); // RA2: as above
	}
}



/***********************************************************************************************
 * Init_Expansion_Files -- Fetch any override expansion mixfiles.                              *
 *                                                                                             *
 *    This routine will search for and register/cache any override mixfiles found.             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool __cdecl Init_Expansion_Files(void) // RA2: 0x00515AF0 searches ECACHE*.MIX and ELOCAL*.MIX in every build (TibSun only in the demo); a plain C function, not static, as Init_Game is not part of this file yet
{
	WIN32_FIND_DATA data; // RA2: as above
	HANDLE handle = FindFirstFile("ECACHE*.MIX", &data); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) {
		do { // RA2: as above
			if ((data.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) { // RA2: as above
				MFCD * mix = new MFCD(data.cFileName, &FastKey); // RA2: as above
				if (mix != NULL) { // RA2: as above
					ExpandMix.Add(mix); // RA2: as above
					mix->Cache(); // RA2: as above
				}
			}
		} while (FindNextFile(handle, &data)); // RA2: as above
		FindClose(handle);
	}
	handle = FindFirstFile("ELOCAL*.MIX", &data); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) {
		do { // RA2: as above
			if ((data.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) { // RA2: as above
				MFCD * mix = new MFCD(data.cFileName, &FastKey); // RA2: as above
				if (mix != NULL) { // RA2: as above
					ExpandMix.Add(mix); // RA2: as above
				}
			}
		} while (FindNextFile(handle, &data)); // RA2: as above
		FindClose(handle);
	}
	return(true);
}



/***********************************************************************************************
 * Init_Bootstrap_Mixfiles -- Registers and caches any mixfiles needed for bootstrapping.      *
 *                                                                                             *
 *    This routine will register the initial mixfiles that are required to display error       *
 *    messages and get input from the player.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure to call this routine before any dialogs would be displayed to the       *
 *             player.                                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Init_Bootstrap_Mixfiles(void) // RA2: 0x00515C90 is not static (Init_Game, 0x005116A0, is not in this part); RA2 registers EXPAND99..00.MIX, RA2.MIX, CACHE.MIX and LOCAL.MIX only
{
	DiskID temp = CD::Get_Required_Disk(); // RA2: as above
	CD::Set_Required_Disk(DISK_LOCAL);

	for (int index = 99; index >= 0; index--) { // RA2: as above
		char name[64]; // RA2: as above
		sprintf(name, "EXPAND%02d.MIX", index); // RA2: as above
		if (RawFileClass(name).Is_Available()) { // RA2: as above
			MFCD * expand = new MFCD(name, &FastKey); // RA2: as above
			if (expand != NULL) { // RA2: as above
				ExpandMix.Add(expand); // RA2: as above
				DebugStringNoPrefix(" %s", name); // RA2: as above
			}
		}
	}

	GameMix = new MFCD("RA2.MIX", &FastKey); // RA2: as above
	if (GameMix == NULL) {
		return(false);
	}

	DebugStringNoPrefix(" CACHE.MIX");

	CacheMix = new MFCD("CACHE.MIX", &FastKey);
	if ((CacheMix == NULL) || (MFCD::Cache("CACHE.MIX") == false)) {
		return(false);
	}

	DebugStringNoPrefix(" CACHE.MIX");

	LocalMix = new MFCD("LOCAL.MIX", &FastKey);
	if (LocalMix == NULL) {
		return(false);
	}

	DebugStringNoPrefix(" LOCAL.MIX");

	CD::Set_Required_Disk(temp);

	return(true);
}



/***********************************************************************************************
 * Init_Secondary_Mixfiles -- Register and cache secondary mixfiles.                           *
 *                                                                                             *
 *    This routine is used to register the mixfiles that are needed for main menu processing.  *
 *    Call this routine before the main menu is display and processed.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Init_Secondary_Mixfiles(void) // RA2: 0x00515E70 is not static; RA2 registers CONQUER.MIX, CAMEO.MIX, MAPS, MULTI, THEME (ScoresMix) and MOVIES, with the further MAPS*/MOVIES* files kept in every build
{
	char name[_MAX_PATH];

	if (CCFileClass("CONQUER.MIX").Is_Available()) {
		ConquerMix = new MFCD("CONQUER.MIX", &FastKey);
	}
	DebugStringNoPrefix(" CONQUER.MIX");
	if (ConquerMix == NULL) {
		return(false);
	}

	if (CCFileClass("CAMEO.MIX").Is_Available()) { // RA2: as above
		CameoMix = new MFCD("CAMEO.MIX", &FastKey); // RA2: as above
	}
	DebugStringNoPrefix(" CAMEO.MIX"); // RA2: as above
	if (CameoMix == NULL) { // RA2: as above
		return(false);
	}

	int id = ((int)CD::Get_Current_Disk(60) + 1); // RA2: as above
	if (CD::Is_Override_Swap() == true) {
		strcpy(name, "MAPS*.MIX");
		if (CDFileClass::Find_First_File(name) == true) {
			DebugStringNoPrefix(" %s", name);
			MapsMix = new MFCD(name, &FastKey);
			while (CDFileClass::Find_Next_File(name) == true) {
				DebugStringNoPrefix(" %s", name);
				MFCD * mix = new MFCD(name, &FastKey); // RA2: as above
				if (mix != NULL) {
					MapsMixLocal.Add(mix);
				}
			}
			CDFileClass::Find_Close();
		}
	} else {
		sprintf(name, "MAPS%02d.MIX", id);
		if (CCFileClass(name).Is_Available()) {
			MapsMix = new MFCD(name, &FastKey);
		}
		DebugStringNoPrefix(" %s", name);
	}
	if (MapsMix == NULL) {
		return(false);
	}

	if (CCFileClass("MULTI.MIX").Is_Available()) {
		MultiMix = new MFCD("MULTI.MIX", &FastKey);
	}
	DebugStringNoPrefix(" MULTI.MIX");
	if (MultiMix == NULL) {
		return(false);
	}

	if (CCFileClass("THEME.MIX").Is_Available()) { // RA2: as above
		ScoresMix = new MFCD("THEME.MIX", &FastKey); // RA2: as above
	}
	DebugStringNoPrefix(" THEME.MIX"); // RA2: as above
	ScoresPresent = true;
	Theme.Scan();

	if (CD::Is_Override_Swap() == true) {
		strcpy(name, "u:\\mixfiles\\MOVIES*.MIX"); // RA2: as above
		if (RawFileClass("MOVIES01.MIX").Is_Available()) { // RA2: as above
			strcpy(name, "MOVIES*.MIX");
		}
		if (RawFileClass("MIXFILES\\MOVIES01.MIX").Is_Available()) { // RA2: as above
			strcpy(name, "MIXFILES\\MOVIES*.MIX"); // RA2: as above
		}
		if (CDFileClass::Find_First_File(name) == true) {
			DebugStringNoPrefix(" %s", name);
			MoviesMix = new MFCD(name, &FastKey);
			while (CDFileClass::Find_Next_File(name) == true) {
				DebugStringNoPrefix(" %s", name);
				MFCD * mix = new MFCD(name, &FastKey); // RA2: as above
				if (mix != NULL) {
					MoviesMixLocal.Add(mix);
				}
			}
			CDFileClass::Find_Close();
		}
	} else {
		sprintf(name, "MOVIES%02d.MIX", id);
		if (CCFileClass(name).Is_Available()) {
			MoviesMix = new MFCD(name, &FastKey);
		}
		DebugStringNoPrefix(" %s", name);
	}
	if (MoviesMix == NULL) {
		return(false);
	}
	return(true);
}




/***********************************************************************************************
 * Init_One_Time_Systems -- Initialize internal pointers to the bulk data.                     *
 *                                                                                             *
 *    This performs the one-time processing required after the bulk data has been cached but   *
 *    before the game actually starts. Typically, this routine extracts pointers to all the    *
 *    embedded data sub-files within the main game data mixfile. This routine must be called   *
 *    AFTER the bulk data has been cached.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine AFTER the bulk data has been cached.                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
static bool Init_One_Time_Systems(void)
{
	Call_Back();

	Map.One_Time();
	Options.One_Time();
	Session.One_Time();
	ObjectTypeClass::One_Time();
	BuildingTypeClass::One_Time();
	BulletTypeClass::One_Time();
	OverlayTypeClass::One_Time();
	SmudgeTypeClass::One_Time();
	UnitTypeClass::One_Time();
	InfantryTypeClass::One_Time();
	AnimTypeClass::One_Time();
	AircraftTypeClass::One_Time();
	HouseClass::One_Time();
	SpotLightClass::One_Time();
	IonBlastClass::One_Time();

	CCFileClass dropvxl("DPOD.VXL");
	DropPodVoxel.VoxLib = new VoxelLibrary(dropvxl);

	if (DropPodVoxel.VoxLib == NULL) {
		DebugString("Failed to create VoxLib!\n");
		return(false);
	}

	CCFileClass dropmot("DPOD.HVA");
	DropPodVoxel.MotLib = new MotionLibrary(dropmot);

	if (DropPodVoxel.MotLib == NULL) {
		DebugString("Failed to create MotLib!\n");
		return(false);
	}

	return(true);
}


/// <summary>
/// Draws the loading screen.
/// This routine puts the loading picture, the copyright text and the loading text up on the
/// screen, and notes the time before which Init_Game keeps it there.
/// </summary>
void Draw_Loading_Screen(void) // RA2: 0x00516560, not in TibSun: GLS.PAL, GLSS.SHP or GLSL.SHP and the copyright and loading texts on the hidden surface; Init_Game waits until LoadingScreenHoldUntil
{
	CCFileClass file("GLS.PAL"); // RA2: as above
	ShapeSet * shape = NULL; // RA2: as above
	PaletteClass * palette = NULL; // RA2: as above
	ConvertClass * drawer = NULL; // RA2: as above
	if (file.Is_Available()) { // RA2: as above
		palette = (PaletteClass *)Load_Alloc_Data(file); // RA2: as above
		file.Close(); // RA2: as above
		for (int index = 0; index < PaletteClass::COLOR_COUNT; index++) { // RA2: as above
			RGBClass & color = (*palette)[index]; // RA2: as above
			color = RGBClass(color.Get_Red() << 2, color.Get_Green() << 2, color.Get_Blue() << 2); // RA2: as above
		}
		if (palette != NULL) { // RA2: as above
			drawer = new ConvertClass(*palette, GamePalette, *VisibleSurface, 1, false); // RA2: as above
			if (drawer != NULL) { // RA2: as above
				CCFileClass shapefile(VideoModeWidth == 640 ? "GLSS.SHP" : "GLSL.SHP"); // RA2: as above
				if (shapefile.Is_Available()) { // RA2: as above
					shape = (ShapeSet *)Load_Alloc_Data(shapefile); // RA2: as above
					shapefile.Close(); // RA2: as above
				}
			}
		}
	}

	RECT client; // RA2: as above
	GetClientRect(MainWindow, &client); // RA2: as above
	Rect area(0, 0, client.right - client.left, client.bottom - client.top); // RA2: as above
	if (shape != NULL) { // RA2: as above
		Point2D xy((area.Width - shape->Get_Width()) / 2, (area.Height - shape->Get_Height()) / 2); // RA2: as above
		Draw_Shape(*HiddenSurface, *drawer, shape, 0, xy, area, SHAPE_NORMAL, NULL, 0, ZGRAD_GROUND, 1000, NULL, 0, Point2D(0, 0)); // RA2: as above
	}

	wchar_t const * text = StringTable::LoadString("TXT_COPYRIGHT", NULL, "D:\\Ra2\\Init.CPP", 4700); // RA2: as above
	BitFont * font = Font_From_TPF(TPF_TEXT); // RA2: as above
	Point2D xy(0, area.Height - 60); // RA2: as above
	if (font != NULL) { // RA2: as above
		xy.X = area.Width - font->String_Pixel_Width(text, 0) - 50; // RA2: as above
	}
	Fancy_Text_Print(text, *HiddenSurface, area, xy, 0xFFFFU, 0U, TPF_TEXT); // RA2: as above

	text = StringTable::LoadString("GUI:WWBrand", NULL, "D:\\Ra2\\Init.CPP", 4711); // RA2: as above
	if (font != NULL) { // RA2: as above
		xy.X = area.Width - font->String_Pixel_Width(text, 0) - 50; // RA2: as above
		xy.Y += font->LineHeight + 3; // RA2: as above
	}
	Fancy_Text_Print(text, *HiddenSurface, area, xy, 0xFFFFU, 0U, TPF_TEXT); // RA2: as above

	text = StringTable::LoadString("GUI:LoadingEx", NULL, "D:\\Ra2\\Init.CPP", 4721); // RA2: as above
	xy.X = 10; // RA2: as above
	xy.Y = 10; // RA2: as above
	Fancy_Text_Print(text, *HiddenSurface, area, xy, 0xFFFFU, 0U, TPF_TEXT); // RA2: as above

	Update_Visible_Surface(true, HiddenSurface); // RA2: as above
	LoadingScreenHoldUntil = AudioGetTime() + 5000; // RA2: as above
	if (palette != NULL) { // RA2: as above
		operator delete(palette); // RA2: as above
	}
	if (shape != NULL) { // RA2: as above
		operator delete(shape); // RA2: as above
	}
	delete drawer; // RA2: as above
}



/***********************************************************************************************
 * Init_Bulk_Data -- Initialize the time-consuming mixfile caching.                            *
 *                                                                                             *
 *    This routine is called to handle the time consuming process of game initialization.      *
 *    The title page will be displayed when this routine is called.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will take a very long time.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Init_Bulk_Data(void) // RA2: 0x00516900 is not static; RA2 caches CONQUER.MIX and CAMEO.MIX only, there is no sound cache and no TUTORIAL.INI
{
	if (ConquerMix == NULL || !ConquerMix->Cache()) {
		return(false);
	}
	if (CameoMix == NULL || !CameoMix->Cache()) { // RA2: as above
		return(false);
	}
	Call_Back();
	return(Init_One_Time_Systems());
}



/***************************************************************************
 * Save_Recording_Values -- Saves multiplayer-specific values              *
 *                                                                         *
 * This routine saves multiplayer values that need to be restored for a    *
 * save game.  In addition to saving the random # seed for this scenario,  *
 * it saves the contents of the actual random number generator; this       *
 * ensures that the random # sequencer will pick up where it left off when *
 * the game was saved.                                                     *
 * This routine also saves the header for a Recording file, so it must     *
 * save some data not needed specifically by a save-game file (ie Seed).   *
 *                                                                         *
 * INPUT:                                                                  *
 *      file      file to save to                                          *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = success, false = failure                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/28/1995 BRR : Created.                                             *
 *=========================================================================*/
bool Save_Recording_Values(CCFileClass & file)
{
	DebugString("Saving recording values for scenario : %s\n", Scen->ScenarioName);
	file.Write(&BuildLevel, sizeof(BuildLevel));
	file.Write(&Seed, sizeof(Seed));
	file.Write(&Scen->Scenario, sizeof(Scen->Scenario));
	file.Write(Scen->ScenarioName, sizeof(Scen->ScenarioName));
	file.Write(&Whom, sizeof(Whom));
	file.Write(&Special, sizeof(SpecialClass));
	file.Write(&Options, sizeof(OptionsClass));
	return(true);
}



/***************************************************************************
 * Load_Recording_Values -- Loads multiplayer-specific values              *
 *                                                                         *
 * INPUT:                                                                  *
 *      file         file to load from                                     *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = success, false = failure                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/28/1995 BRR : Created.                                             *
 *=========================================================================*/
bool Load_Recording_Values(CCFileClass & file)
{
	file.Read(&BuildLevel, sizeof(BuildLevel));
	file.Read(&Seed, sizeof(Seed));
	file.Read(&Scen->Scenario, sizeof(Scen->Scenario));
	file.Read(Scen->ScenarioName, sizeof(Scen->ScenarioName));
	file.Read(&Whom, sizeof(Whom));
	file.Read(&Special, sizeof(SpecialClass));
	file.Read(&Options, sizeof(OptionsClass));
	DebugString("Loaded recording values for scenario : %s\n", Scen->ScenarioName);
	return(true);
}

BOOL CALLBACK Main_Menu_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam); // RA2: Main_Menu names its dialog procedure



/// <summary>
/// Feeds a typed character to the cheat code recognizer.
/// This routine gathers up alphanumeric keystrokes and toggles a cheat the moment its code
/// appears in what has been typed so far. Anything else the player types wipes the slate.
/// </summary>
/// <param name="chr">The character that the player just typed.</param>
/// <returns>bool; Was a cheat toggled by this character?</returns>
static bool Cheat_Key_Process(char chr) // RA2: static: RA2 has no out-of-line copy, the code is inlined into Main_Menu (0x00517081-0x00517164)
{
	static char _buffer[32] = "";

	if (!isalnum(chr) || chr == '~') {
		memset(_buffer, 0, sizeof(_buffer));
		return(false);
	}

	char tmp[2];
	tmp[0] = chr;
	tmp[1] = 0;
	DebugStringNoPrefix(tmp);

	int len = strlen(_buffer);
	if (len >= ARRAY_SIZE(_buffer)-1) {
		len = 0;
		_buffer[0] = 0;
	}
	_buffer[len] = toupper(chr);

	for (int c = 0; c < ARRAY_SIZE(CheatEntries); c++) {
		if (strstr(_buffer, CheatEntries[c].CheatString) != NULL) {
			*CheatEntries[c].State = !(*CheatEntries[c].State);
			memset(_buffer, 0, sizeof(_buffer));
			return(true);
		}
	}
	return(false);
}

/// <summary>
/// Places the flag picture of a menu dialog.
/// The flag is moved to the corner of the dialog area on screens larger than 800 by 600,
/// and is given the small flag picture on a 640 pixel wide screen.
/// </summary>
/// <param name="dialog">The dialog whose flag control is to be placed.</param>
void Show_Dialog_With_Flag(HWND dialog) // RA2: 0x005115F0, not in TibSun: puts the flag picture (control 0x71A) at the dialog corner of a 800x600 or larger screen
{
	HWND item = GetDlgItem(dialog, 0x71A); // RA2: as above
	if (item != NULL) { // RA2: as above
		int x = VideoModeWidth <= 800 ? 0 : (VideoModeWidth - 800) / 2; // RA2: as above
		int y = VideoModeHeight <= 600 ? 0 : (VideoModeHeight - 600) / 2; // RA2: as above
		SetWindowPos(item, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW); // RA2: as above
		SendMessage(item, WM_USER + 227, 1, 0); // RA2: as above
		if (VideoModeWidth == 640) { // RA2: as above
			SendMessage(item, WM_USER + 228, 0, (LPARAM)"Ra2ts_s"); // RA2: as above
		} else { // RA2: as above
			SendMessage(item, WM_USER + 228, 0, (LPARAM)"Ra2ts_l"); // RA2: as above
		}
	}
}

char ProfileText[128]; // RA2: 0x00A4127C, the summary text Get_Performance_Profile builds
char ProfileCPUText[128]; // RA2: 0x00A411FC
char ProfileRAMText[128]; // RA2: 0x00A41388
char ProfileVRAMText[128]; // RA2: 0x00A41308
char ProfileBlitText[128]; // RA2: 0x00A41408



/// <summary>
/// Handles the messages for the version information dialog.
/// This routine fills the list box with the game's title, its version numbers, the build
/// stamp, and a description of the processor it finds itself running upon. It is the
/// first thing to ask for when a player reports a problem.
/// </summary>
BOOL CALLBACK Version_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	char buffer[256];
	char time[128]; // RA2: 0x00516C40 uses RA2's dialog messages (WM_USER + 151 after creation, WM_USER + 200 adds a line to the list, + 205 sets its title) and its profile texts
	char machine[128]; // RA2: as above
	char user[128]; // RA2: as above
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam); // RA2: as above
	if (result != 0) { // RA2: as above
		return(result); // RA2: as above
	}
	int * choice = (int *)GetWindowLong(window, DWL_USER); // RA2: as above
	switch (message) {
		case OD_SUBCLASSED: // RA2: as above
		{
			HWND item = GetDlgItem(window, IDC_VERSION_INFO); // RA2: as above
			SendMessage(item, OD_ADDSTRINGW, 0, (LPARAM)StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Init.CPP", 5428)); // RA2: as above
			sprintf(buffer, "Version %s", Version_Name());
			SendMessage(item, WM_USER + 200, 0, (LPARAM)buffer); // RA2: as above
			sprintf(buffer, "Internal Version %s", VerNum.Version_Name());
			SendMessage(item, WM_USER + 200, 0, (LPARAM)buffer); // RA2: as above
			time[0] = 0; // RA2: as above
			Build_Date_String(time, sizeof(time)); // RA2: as above
			sprintf(buffer, "Release Build: %s by %s - %s", Build_Number_String(machine, sizeof(machine)), // RA2: as above
				Build_By_String(user, sizeof(user)), time); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)buffer); // RA2: as above
			int family = 5; // RA2: as above
			bool mmx = false; // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)ProfileCPUText); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)ProfileRAMText); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)ProfileVRAMText); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)ProfileBlitText); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)ProfileText); // RA2: as above
			Get_CPU_Type(family, mmx, NULL, 0); // RA2: as above
			sprintf(buffer, "MMX %s, Vendor: %s", mmx ? "Yes" : "No", &VendorID); // RA2: as above
			SendMessage(item, WM_USER + 200, 0, (LPARAM)buffer); // RA2: as above
			Get_Language_Version(buffer);
			SendMessage(item, WM_USER + 200, 0, (LPARAM)buffer); // RA2: as above
		}
		return(FALSE);
	case WM_COMMAND:
		if ((int)(wparam & 0xFFFF) > 0 && (int)(wparam & 0xFFFF) <= 2) { // RA2: as above
			*choice = wparam & 0xFFFF; // RA2: as above
		}
		break;
	}
	return(FALSE);
}



/// <summary>
/// Displays the version information dialog.
/// This routine does not return until the player dismisses the dialog, and keeps the
/// title screen alive behind it while it waits.
/// </summary>
void Version_Dialog(void)
{
	int res = 0;

	HWND dialog = OwnerDraw::Begin_Dialog(IDD_VERSION, (DLGPROC)Version_Dialog_Proc); // RA2: 0x00516E70 is not static; opens dialog 212 with the procedure above
	if (dialog != NULL) {
		SetWindowLong(dialog, DWL_USER, (LONG)&res);
		OwnerDraw::Display_Dialog(dialog);
		while (res == 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				break;
			}
			Title_Screen_Restore();
		}
		OwnerDraw::End_Dialog(dialog);
	}
}



/***************************************************************************
 * Main_Menu -- Menu processing                                            *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      index of item selected, -1 if time out                             *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/17/1995 BRR : Created.                                             *
 *=========================================================================*/
int Main_Menu(unsigned int timeout)
{
	int retval = MENU_NONE; // RA2: 0x00516F00: RA2's menu returns MENU_* values, centres its dialog on the main window and shows the flag; the cheat check is inlined

	CD::Set_Required_Disk(DISK_ANY);
	if (CD::Force_Available() == false) {
		return(MENU_EXIT); // RA2: as above
	}

	HWND dialog = OwnerDraw::Begin_Dialog(IDD_MAIN_MENU, (DLGPROC)Main_Menu_Dialog_Proc); // RA2: as above
	if (dialog != NULL) {
		SetWindowLong(dialog, DWL_USER, (LONG)&retval);
		Center_Window_Within_Window(dialog, MainWindow); // RA2: as above
		OwnerDraw::Display_Dialog(dialog);
		SetFocus(MainWindow);
		Show_Dialog_With_Flag(dialog); // RA2: as above

		do {
			if (OwnerDraw::Dialog_Message_Handler()) { // RA2: as above
				retval = MENU_EXIT; // RA2: as above
			}
			Title_Screen_Restore();

			if (Keyboard->Check()) {
				int input = Keyboard->Get(); // RA2: as above
				if ((input & KN_RLSE_BIT) == 0) { // RA2: as above
					if (Cheat_Key_Process((char)input) == true) {
						VocClass::PlayGlobal(Rule->OptionsChanged, 1.0f, 0x2000); // RA2: as above
						Title_Screen_Restore(true);
					}
				}
			}
		} while (retval == MENU_NONE); // RA2: as above

		OwnerDraw::End_Dialog(dialog);

		SYSTEMTIME t;
		GetSystemTime(&t);
		CryptRandom.Seed_Byte(t.wMilliseconds);
	} else {
		retval = MENU_EXIT; // RA2: as above
	}

	SetFocus(MainWindow);
	return(retval);
}



/// <summary>
/// Handles the messages for the main menu dialog.
/// This routine records the button the player pressed into the result that Main_Menu is
/// waiting upon, and greys out the load button when there is nothing to load.
/// </summary>
BOOL CALLBACK Main_Menu_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc) {
		return(rc);
	}

	int * res = (int *)GetWindowLong(window, DWL_USER); // RA2: 0x005171D0: RA2's buttons are 0x578 (IDC_NETWORK), 0x55C, IDC_EXIT_GAME, 0x683, 0x686 and 0x684; control 0x71D shows the version

	switch (message) {
		case OD_SUBCLASSED: // RA2: as above
		{
			HWND control = GetDlgItem(window, 0x71D); // RA2: as above
			wchar_t text[128]; // RA2: as above
			swprintf(text, L"%s %s", StringTable::LoadString("GUI:Version", NULL, "D:\\Ra2\\Init.CPP", 5723), Narrow_To_Wide(VerNum.Version_Name())); // RA2: as above
			SendMessage(control, OD_SETTEXTW, 0, (LPARAM)text); // RA2: as above
			return(FALSE);
		}

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) {
				case IDC_NETWORK: // RA2: as above
					*res = MENU_NETWORK; // RA2: as above
					return(FALSE);

				case IDC_OPTIONS: // RA2: as above
					*res = MENU_OPTIONS; // RA2: as above
					return(FALSE);

				case IDC_EXIT_GAME:
					*res = MENU_EXIT_ASK; // RA2: as above
					return(FALSE);

				case 0x683: // RA2: as above
					*res = MENU_SINGLE_PLAYER; // RA2: as above
					return(FALSE);

				case 0x686: // RA2: as above
					*res = MENU_EXTRAS; // RA2: as above
					return(FALSE);

				case 0x684: // RA2: as above
					*res = MENU_INTERNET; // RA2: as above
					return(FALSE);
			}
			break;

		case WM_PAINT: // RA2: as above
		{
			HWND control = GetDlgItem(window, 0x71A); // RA2: as above
			SendMessage(control, WM_USER + 240, 0, 0); // RA2: as above
			break;
		}
	}

	return(FALSE);
}



/// <summary>
/// Redraws the title screen if the display surfaces have been lost.
/// The menu dialogs call this routine from their message loops, so that the background
/// comes back after the display has been taken away and handed back to the game.
/// </summary>
/// <param name="force">Should the title screen be redrawn even if nothing was lost?</param>
void Title_Screen_Restore(bool force)
{
	if (SurfacesRestored || force) { // RA2: 0x00517370 darkens the title screen with Darken_Surface instead of loading the menu background
		SurfacesRestored = false;
		Darken_Surface(NULL, true); // RA2: as above
		Update_Visible_Surface(false, HiddenSurface);
	}
}

/// <summary>
/// Does nothing.
/// Leave_Game calls this routine first.
/// </summary>
void Leave_Game_Hook(void) // RA2: 0x005173A0, not in TibSun: an empty function that Leave_Game calls first (session.cpp)
{
}


/*
**	RA2: the keyboard commands. TibSun's file has such classes too, but RA2's differ: their names, categories and descriptions are
**	wide strings that come from the string table with this file's name and line, there are more of them (the team, taunt, sidebar,
**	tab and selection commands), and Execute takes the key. The executable's copies of their methods lie after the last function of
**	this file, in the order of the classes below.
*/
static char CommandNameBuffer[160]; // RA2: 0x00A4149C; the unique name of a team command
static wchar_t CommandTextBuffer[128]; // RA2: 0x00A4153C; the display name and description of a team command
bool CursorCheat; // RA2: 0x00A416BC; the cursor position command toggles it
static int LastTauntTime = -1; // RA2: 0x007DB560; when the last taunt was sent


class CreateTeamCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051AEA0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		CreateTeamCommandClass(int team) : Team(team) {} // RA2: as above

		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			sprintf(CommandNameBuffer, "TeamCreate_%d", Team); // RA2: as above
			return CommandNameBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_CREATE_TEAM", NULL, "D:\\Ra2\\Init.CPP", 5910), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 5912); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_CREATE_TEAM_DESC", NULL, "D:\\Ra2\\Init.CPP", 5914), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Group_Assign(Team); // RA2: as above
		} // RA2: as above

	private: // RA2: as above
		int Team; // RA2: as above
}; // RA2: as above

class SetStructureTabCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051AFC0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "StructureTab"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_STRUCTURE_TAB", NULL, "D:\\Ra2\\Init.CPP", 5931); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 5933); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_STRUCTURE_TAB_DESC", NULL, "D:\\Ra2\\Init.CPP", 5935); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Map.Column[0].Has_Buildables()) { // RA2: as above
				Map.Select_Tab(0); // RA2: as above
				BuildingClass * building = Structure_Tab_Building(); // RA2: as above
				if (building != NULL) { // RA2: as above
					PlayerPtr->Manual_Place(building->Who_Can_Build_Me(false, false), building); // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetDefenseTabCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B080; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "DefenseTab"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_DEFENSE_TAB", NULL, "D:\\Ra2\\Init.CPP", 5957); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 5959); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_DEFENSE_TAB_DESC", NULL, "D:\\Ra2\\Init.CPP", 5961); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Map.Column[1].Has_Buildables()) { // RA2: as above
				Map.Select_Tab(1); // RA2: as above
				BuildingClass * building = Defense_Tab_Building(); // RA2: as above
				if (building != NULL) { // RA2: as above
					PlayerPtr->Manual_Place(building->Who_Can_Build_Me(false, false), building); // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetInfantryTabCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B140; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "InfantryTab"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INFANTRY_TAB", NULL, "D:\\Ra2\\Init.CPP", 5983); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 5985); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INFANTRY_TAB_DESC", NULL, "D:\\Ra2\\Init.CPP", 5987); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Map.Column[2].Has_Buildables()) { // RA2: as above
				Map.Select_Tab(2); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetUnitTabCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B1D0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "UnitTab"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_UNIT_TAB", NULL, "D:\\Ra2\\Init.CPP", 6004); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 6006); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_UNIT_TAB_DESC", NULL, "D:\\Ra2\\Init.CPP", 6008); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Map.Column[3].Has_Buildables()) { // RA2: as above
				Map.Select_Tab(3); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SelectTeamCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B260; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		SelectTeamCommandClass(int team) : Team(team) {} // RA2: as above

		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			sprintf(CommandNameBuffer, "TeamSelect_%d", Team); // RA2: as above
			return CommandNameBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_SELECT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6037), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6039); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_SELECT_TEAM_DESC", NULL, "D:\\Ra2\\Init.CPP", 6041), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Group_Select(Team); // RA2: as above
		} // RA2: as above

	private: // RA2: as above
		int Team; // RA2: as above
}; // RA2: as above

class AddTeamCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B350; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		AddTeamCommandClass(int team) : Team(team) {} // RA2: as above

		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			sprintf(CommandNameBuffer, "TeamAddSelect_%d", Team); // RA2: as above
			return CommandNameBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_ADD_SELECT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6071), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6073); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_ADD_SELECT_TEAM_DESC", NULL, "D:\\Ra2\\Init.CPP", 6075), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Group_Select_Members(Team); // RA2: as above
		} // RA2: as above

	private: // RA2: as above
		int Team; // RA2: as above
}; // RA2: as above

class CenterTeamCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B440; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		CenterTeamCommandClass(int team) : Team(team) {} // RA2: as above

		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			sprintf(CommandNameBuffer, "TeamCenter_%d", Team); // RA2: as above
			return CommandNameBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_CENTER_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6106), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_TEAM", NULL, "D:\\Ra2\\Init.CPP", 6108); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_CENTER_TEAM_DESC", NULL, "D:\\Ra2\\Init.CPP", 6110), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Group_Reselect(Team); // RA2: as above
		} // RA2: as above

	private: // RA2: as above
		int Team; // RA2: as above
}; // RA2: as above

class TauntCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B530; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		TauntCommandClass(int team) : Team(team) {} // RA2: as above

		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			sprintf(CommandNameBuffer, "Taunt_%d", Team); // RA2: as above
			return CommandNameBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_TAUNT_NUMBER", NULL, "D:\\Ra2\\Init.CPP", 6145), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_TAUNT", NULL, "D:\\Ra2\\Init.CPP", 6150); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			swprintf(CommandTextBuffer, StringTable::LoadString("TXT_TAUNT_DESC", NULL, "D:\\Ra2\\Init.CPP", 6154), Team); // RA2: as above
			return CommandTextBuffer; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: as above
				if (!MuteSWLaunches) { // RA2: as above
					unsigned long now = timeGetTime(); // RA2: as above
					if (LastTauntTime == -1 || (int)(now - LastTauntTime) > 5000) { // RA2: as above
						if ((Session.LANTaunts && Session.Type == GAME_IPX) || (Session.WOLTaunts && Session.Type == GAME_INTERNET)) { // RA2: as above
							LastTauntTime = now; // RA2: as above
							GlobalPacketType packet; // RA2: as above
							packet.Command = NetCommandType(29); // RA2: as above
							strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above
							packet.Taunt.Color = Session.PrefColor; // RA2: as above
							packet.Taunt.NameCRC = Compute_Name_CRC(Session.GameName); // RA2: as above
							unsigned char house = Session.Players[0]->Player.House << 4; // RA2: as above
							packet.Taunt.ID = (unsigned char)(house ^ ((house ^ Team) & 0x0F)); // RA2: as above
							Play_Taunt(packet.Taunt.ID); // RA2: as above
							packet.Taunt.Recipients = 0; // RA2: as above
							int house_count = Houses.Count(); // RA2: as above
							for (int i = 0; i < house_count; i++) { // RA2: as above
								HouseClass * house = Houses[i]; // RA2: as above
								if (house != NULL && house->Class->IsMultiplay && Session.PlayerChatEnabled[i]) { // RA2: as above
									int count = Ipx.Num_Connections(); // RA2: as above
									for (int j = 0; j < count; j++) { // RA2: as above
										int id = Ipx.Connection_ID(j); // RA2: as above
										if (wcscmp(house->UIName, Ipx.Connection_Name(id)) == 0) { // RA2: as above
											IPXAddressClass * address = Ipx.Connection_Address(id); // RA2: as above
											if (Session.ViaPacketRouter) { // RA2: as above
												packet.Taunt.Recipients |= OwnerTalk.Mask_From_ID(id); // RA2: as above
											} else { // RA2: as above
												Ipx.Send_Global_Message(&packet, 455, 1, address); // RA2: as above
												Ipx.Service(); // RA2: as above
											} // RA2: as above
										} // RA2: as above
									} // RA2: as above
								} // RA2: as above
							} // RA2: as above
							if (Session.ViaPacketRouter && packet.Taunt.Recipients) { // RA2: as above
								Ipx.Send_Global_Message(&packet, 455, 1, &Session.Players[0]->Address); // RA2: as above
								Ipx.Service(); // RA2: as above
							} // RA2: as above
						} // RA2: as above
					} // RA2: as above
				} // RA2: as above
			} // RA2: as above
		} // RA2: as above

	private: // RA2: as above
		int Team; // RA2: as above
}; // RA2: as above

class NextObjectCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B840; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "NextObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_NEXT_OBJECT", NULL, "D:\\Ra2\\Init.CPP", 6271); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6273); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_NEXT_OBJECT_DESC", NULL, "D:\\Ra2\\Init.CPP", 6275); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Power_Mode_Control(0); // RA2: as above
			Map.Waypoint_Mode_Control(0, false); // RA2: as above
			Map.Repair_Mode_Control(0); // RA2: as above
			Map.Sell_Mode_Control(0); // RA2: as above
			ObjectClass * object = Map.Next_Object(CurrentObject.Count() ? CurrentObject[0] : NULL); // RA2: as above
			if (object != NULL) { // RA2: as above
				Unselect_All(); // RA2: as above
				object->Select(); // RA2: as above
				Map.Center_Map(); // RA2: as above
				Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class PlanningModeCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051B940; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "PlanningMode"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:PlanningMode", NULL, "D:\\Ra2\\Init.CPP", 6325); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6327); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:PlanningModeDesc", NULL, "D:\\Ra2\\Init.CPP", 6329); // RA2: as above
		} // RA2: as above
		virtual bool PreventCombinationOverride(int key) const // RA2: as above
		{ // RA2: as above
			return (key & KN_SHIFT_BIT) || (key & KN_CTRL_BIT) || (key & KN_ALT_BIT); // RA2: as above
		} // RA2: as above
		virtual bool ExtraTriggerCondition(int key) const // RA2: as above
		{ // RA2: as above
			return true; // RA2: as above
		} // RA2: as above
		virtual bool Unknown1C(int key) const // RA2: as above
		{ // RA2: as above
			return !(key & KN_RLSE_BIT); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (key & KN_RLSE_BIT) { // RA2: as above
				Command_Planning_Mode_Off(false); // RA2: as above
			} else { // RA2: as above
				Command_Planning_Mode_On(false); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class CombatantSelectCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BA10; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "CombatantSelect"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:CombatantSelect", NULL, "D:\\Ra2\\Init.CPP", 6409); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6411); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:CombatantSelectDesc", NULL, "D:\\Ra2\\Init.CPP", 6413); // RA2: as above
		} // RA2: as above
		virtual bool PreventCombinationOverride(int key) const // RA2: as above
		{ // RA2: as above
			return (key & KN_SHIFT_BIT) != 0; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Select_All_Combatants((key & KN_SHIFT_BIT) == 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class TypeSelectCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BAB0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "TypeSelect"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:TypeSelect", NULL, "D:\\Ra2\\Init.CPP", 6435); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6437); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:TypeSelectDesc", NULL, "D:\\Ra2\\Init.CPP", 6439); // RA2: as above
		} // RA2: as above
		virtual bool PreventCombinationOverride(int key) const // RA2: as above
		{ // RA2: as above
			return (key & KN_SHIFT_BIT) != 0; // RA2: as above
		} // RA2: as above
		virtual bool ExtraTriggerCondition(int key) const // RA2: as above
		{ // RA2: as above
			return true; // RA2: as above
		} // RA2: as above
		virtual bool Unknown1C(int key) const // RA2: as above
		{ // RA2: as above
			return !(key & KN_RLSE_BIT); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (key & KN_RLSE_BIT) { // RA2: as above
				Type_Select_Release(); // RA2: as above
			} else { // RA2: as above
				Type_Select_Press(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class HealthNavCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BB70; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "HealthNav"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:HealthNavigation", NULL, "D:\\Ra2\\Init.CPP", 6473); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6475); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:HealthNavigationDesc", NULL, "D:\\Ra2\\Init.CPP", 6477); // RA2: as above
		} // RA2: as above
		virtual bool PreventCombinationOverride(int key) const // RA2: as above
		{ // RA2: as above
			return (key & KN_SHIFT_BIT) != 0; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Apply_State_Command((key & KN_SHIFT_BIT) == 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class VeterancyNavCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BC10; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "VeterancyNav"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:VetNavigation", NULL, "D:\\Ra2\\Init.CPP", 6499); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6501); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:VetNavigationDesc", NULL, "D:\\Ra2\\Init.CPP", 6503); // RA2: as above
		} // RA2: as above
		virtual bool PreventCombinationOverride(int key) const // RA2: as above
		{ // RA2: as above
			return (key & KN_SHIFT_BIT) != 0; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Apply_Rank_Command((key & KN_SHIFT_BIT) == 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class PrevObjectCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BCB0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "PreviousObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PREV_OBJECT", NULL, "D:\\Ra2\\Init.CPP", 6562); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6564); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PREV_OBJECT_DESC", NULL, "D:\\Ra2\\Init.CPP", 6566); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Power_Mode_Control(0); // RA2: as above
			Map.Waypoint_Mode_Control(0, false); // RA2: as above
			Map.Repair_Mode_Control(0); // RA2: as above
			Map.Sell_Mode_Control(0); // RA2: as above
			ObjectClass * object = Map.Prev_Object(CurrentObject.Count() ? CurrentObject[0] : NULL); // RA2: as above
			if (object != NULL) { // RA2: as above
				Unselect_All(); // RA2: as above
				object->Select(); // RA2: as above
				Map.Center_Map(); // RA2: as above
				Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class StopCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BDB0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "StopObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_STOP_OBJECT", NULL, "D:\\Ra2\\Init.CPP", 6603); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6605); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_STOP_OBJECT_DESC", NULL, "D:\\Ra2\\Init.CPP", 6607); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Stop(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class AllToCheerCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BE30; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "AllToCheer"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:AllCheer", NULL, "D:\\Ra2\\Init.CPP", 6621); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6623); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:AllCheerDesc", NULL, "D:\\Ra2\\Init.CPP", 6625); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Cheer(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class DeployCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BEB0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "DeployObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_DEPLOY_OBJECT", NULL, "D:\\Ra2\\Init.CPP", 6648); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6650); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_DEPLOY_OBJECT_DESC", NULL, "D:\\Ra2\\Init.CPP", 6652); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Deploy(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class GuardCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BF30; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "GuardObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_GUARD", NULL, "D:\\Ra2\\Init.CPP", 6676); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6678); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_GUARD_DESC", NULL, "D:\\Ra2\\Init.CPP", 6680); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Guard(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class ScatterCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051BFB0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ScatterObject"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SCATTER", NULL, "D:\\Ra2\\Init.CPP", 6705); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6707); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SCATTER_DESC", NULL, "D:\\Ra2\\Init.CPP", 6709); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Scatter(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class CenterViewCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C030; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "CenterView"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CENTER_VIEW", NULL, "D:\\Ra2\\Init.CPP", 6732); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6734); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CENTER_VIEW_DESC", NULL, "D:\\Ra2\\Init.CPP", 6736); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (CurrentObject.Count() > 0) { // RA2: as above
				Map.Center_Map(); // RA2: as above
				Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class CenterBaseCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C0D0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "CenterBase"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CENTER_BASE", NULL, "D:\\Ra2\\Init.CPP", 6763); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELECTION", NULL, "D:\\Ra2\\Init.CPP", 6765); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CENTER_BASE_DESC", NULL, "D:\\Ra2\\Init.CPP", 6767); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Coord conyard_coord = COORD_NONE; // RA2: as above
			Coord any_building_coord = COORD_NONE; // RA2: as above
			int index = 0; // RA2: as above
			bool none = true; // RA2: as above

			if (PlayerPtr->CurBuildings) { // RA2: as above
				for (index = 0; index < Buildings.Count(); index++) { // RA2: as above
					none = false; // RA2: as above
					BuildingClass * building = Buildings[index]; // RA2: as above

					if (building != NULL && !building->IsInLimbo && building->House->Is_Player_Control()) { // RA2: as above
						if (building->Class->IsConstructionYard) { // RA2: as above
							conyard_coord = building->Center_Coord(); // RA2: as above
							if (building->IsLeader) { // RA2: as above
								break; // RA2: as above
							} // RA2: as above
						} else if (any_building_coord == COORD_NONE) { // RA2: as above
							any_building_coord = building->Center_Coord(); // RA2: as above
						} // RA2: as above
					} // RA2: as above
				} // RA2: as above
			} // RA2: as above

			if (!none) { // RA2: as above
				if (any_building_coord == COORD_NONE && conyard_coord == COORD_NONE) { // RA2: as above
					none = true; // RA2: as above
				} // RA2: as above
			} // RA2: as above

			if (none) { // RA2: as above
				if (PlayerPtr->CurUnits) { // RA2: as above
					for (index = 0; index < Units.Count(); index++) { // RA2: as above
						UnitClass * unit = Units[index]; // RA2: as above
						if (unit != NULL && !unit->IsInLimbo && unit->House->Is_Player_Control() && unit->Class == PlayerPtr->Get_First_Ownable(Rule->BaseUnit)) { // RA2: as above
							conyard_coord = unit->Center_Coord(); // RA2: as above
							break; // RA2: as above
						} // RA2: as above
					} // RA2: as above
				} // RA2: as above
			} // RA2: as above

			if (conyard_coord != COORD_NONE) { // RA2: as above
				TacticalMap->Set_Tactical_Position(conyard_coord); // RA2: as above
			} else if (any_building_coord != COORD_NONE) { // RA2: as above
				TacticalMap->Set_Tactical_Position(any_building_coord); // RA2: as above
			} // RA2: as above

			if (Map.PendingObject) { // RA2: as above
				Map.Set_Cursor_Pos(); // RA2: as above
			} // RA2: as above

			Map.Break_Follow_Mode(); // RA2: as above
			Map.Flag_To_Redraw(GS_REDRAW_TACTICAL); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class AllianceCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C3C0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ToggleAlliance"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_ALLIANCE", NULL, "D:\\Ra2\\Init.CPP", 6848); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_CONTROL", NULL, "D:\\Ra2\\Init.CPP", 6850); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_ALLIANCE_DESC", NULL, "D:\\Ra2\\Init.CPP", 6852); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Ally(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class ToggleRepairCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C440; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ToggleRepair"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_REPAIR_MODE", NULL, "D:\\Ra2\\Init.CPP", 6906); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 6908); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_REPAIR_MODE_DESC", NULL, "D:\\Ra2\\Init.CPP", 6910); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Repair_Mode_Control(-1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class ToggleSellCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C4C0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ToggleSell"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELL_MODE", NULL, "D:\\Ra2\\Init.CPP", 6932); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 6934); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SELL_MODE_DESC", NULL, "D:\\Ra2\\Init.CPP", 6936); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Sell_Mode_Control(-1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class BeaconPlacementCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C540; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "PlaceBeacon"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PLACE_BEACON", NULL, "D:\\Ra2\\Init.CPP", 6975); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 6977); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PLACE_BEACON_DESC", NULL, "D:\\Ra2\\Init.CPP", 6979); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) { // RA2: as above
				Command_Beacon(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class CenterREventCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C5D0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "CenterOnRadarEvent"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RADAR_EVENT", NULL, "D:\\Ra2\\Init.CPP", 6998); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7000); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RADAR_EVENT_DESC", NULL, "D:\\Ra2\\Init.CPP", 7002); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Jump_To_Radar_Event(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SidebarUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C650; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SidebarUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_UP", NULL, "D:\\Ra2\\Init.CPP", 7052); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7054); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_UP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7056); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(true, -1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class LSidebarUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C6E0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "LeftSidebarUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_UP", NULL, "D:\\Ra2\\Init.CPP", 7078); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7080); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_UP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7082); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(true, 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class RSidebarUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C770; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "RightSidebarUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_UP", NULL, "D:\\Ra2\\Init.CPP", 7105); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7107); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_UP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7109); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(true, 1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SidebarPageUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C800; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SidebarPageUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_PGUP", NULL, "D:\\Ra2\\Init.CPP", 7131); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7133); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_PGUP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7135); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(true, -1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class LSidebarPageUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C890; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "LeftSidebarPageUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_PGUP", NULL, "D:\\Ra2\\Init.CPP", 7149); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7151); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_PGUP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7153); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(true, 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class RSidebarPageUpCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C920; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "RightSidebarPageUp"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_PGUP", NULL, "D:\\Ra2\\Init.CPP", 7167); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7169); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_PGUP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7171); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(true, 1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SidebarDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051C9B0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SidebarDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_DOWN", NULL, "D:\\Ra2\\Init.CPP", 7193); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7195); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_DOWN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7197); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(false, -1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class LSidebarDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CA40; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "LeftSidebarDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_DOWN", NULL, "D:\\Ra2\\Init.CPP", 7219); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7221); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_DOWN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7223); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(false, 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class RSidebarDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CAD0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "RightSidebarDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_DOWN", NULL, "D:\\Ra2\\Init.CPP", 7246); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7248); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_DOWN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7250); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Scroll(false, 1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SidebarPageDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CB60; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SidebarPageDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_PGDN", NULL, "D:\\Ra2\\Init.CPP", 7272); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7274); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SIDEBAR_PGDN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7276); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(false, -1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class LSidebarPageDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CBF0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "LeftSidebarPageDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_PGDN", NULL, "D:\\Ra2\\Init.CPP", 7290); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7292); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_LSIDEBAR_PGDN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7294); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(false, 0); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class RSidebarPageDownCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CC80; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "RightSidebarPageDown"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_PGDN", NULL, "D:\\Ra2\\Init.CPP", 7308); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7310); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_RSIDEBAR_PGDN_DESC", NULL, "D:\\Ra2\\Init.CPP", 7312); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Map.Page(false, 1); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class OptionsCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CD10; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "Options"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_OPTIONS", NULL, "D:\\Ra2\\Init.CPP", 7335); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7337); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_OPTIONS_DESC", NULL, "D:\\Ra2\\Init.CPP", 7339); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Queue_Options(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class View1CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CD90; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "View1"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK1", NULL, "D:\\Ra2\\Init.CPP", 7512); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7514); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK1_DESC", NULL, "D:\\Ra2\\Init.CPP", 7516); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Coord coord(Scen->Views[0]); // RA2: as above
			coord.Z = Map.Get_Height_GL(coord); // RA2: as above
			TacticalMap->Set_Tactical_Position(coord); // RA2: as above
			if (Map.PendingObject) { // RA2: as above
				Map.Set_Cursor_Pos(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class View2CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CE80; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "View2"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK2", NULL, "D:\\Ra2\\Init.CPP", 7548); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7550); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK2_DESC", NULL, "D:\\Ra2\\Init.CPP", 7552); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Coord coord(Scen->Views[1]); // RA2: as above
			coord.Z = Map.Get_Height_GL(coord); // RA2: as above
			TacticalMap->Set_Tactical_Position(coord); // RA2: as above
			if (Map.PendingObject) { // RA2: as above
				Map.Set_Cursor_Pos(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class View3CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051CF70; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "View3"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK3", NULL, "D:\\Ra2\\Init.CPP", 7584); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7586); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK3_DESC", NULL, "D:\\Ra2\\Init.CPP", 7588); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Coord coord(Scen->Views[2]); // RA2: as above
			coord.Z = Map.Get_Height_GL(coord); // RA2: as above
			TacticalMap->Set_Tactical_Position(coord); // RA2: as above
			if (Map.PendingObject) { // RA2: as above
				Map.Set_Cursor_Pos(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class View4CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D060; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "View4"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK4", NULL, "D:\\Ra2\\Init.CPP", 7620); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7622); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_VIEW_BOOKMARK4_DESC", NULL, "D:\\Ra2\\Init.CPP", 7624); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Coord coord(Scen->Views[3]); // RA2: as above
			coord.Z = Map.Get_Height_GL(coord); // RA2: as above
			TacticalMap->Set_Tactical_Position(coord); // RA2: as above
			if (Map.PendingObject) { // RA2: as above
				Map.Set_Cursor_Pos(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetView1CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D150; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SetView1"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK1", NULL, "D:\\Ra2\\Init.CPP", 7656); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7658); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK1_DESC", NULL, "D:\\Ra2\\Init.CPP", 7660); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Point2D pixel = TacticalMap->Get_Relative_Tactical_Position(); // RA2: as above
			Scen->Views[0] = TacticalMap->Pixel_To_Cell(pixel); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetView2CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D210; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SetView2"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK2", NULL, "D:\\Ra2\\Init.CPP", 7683); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7685); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK2_DESC", NULL, "D:\\Ra2\\Init.CPP", 7687); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Point2D pixel = TacticalMap->Get_Relative_Tactical_Position(); // RA2: as above
			Scen->Views[1] = TacticalMap->Pixel_To_Cell(pixel); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetView3CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D2D0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SetView3"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK3", NULL, "D:\\Ra2\\Init.CPP", 7710); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7712); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK3_DESC", NULL, "D:\\Ra2\\Init.CPP", 7714); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Point2D pixel = TacticalMap->Get_Relative_Tactical_Position(); // RA2: as above
			Scen->Views[2] = TacticalMap->Pixel_To_Cell(pixel); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class SetView4CommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D390; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "SetView4"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK4", NULL, "D:\\Ra2\\Init.CPP", 7737); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7739); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SET_BOOKMARK4_DESC", NULL, "D:\\Ra2\\Init.CPP", 7741); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Point2D pixel = TacticalMap->Get_Relative_Tactical_Position(); // RA2: as above
			Scen->Views[3] = TacticalMap->Pixel_To_Cell(pixel); // RA2: as above
		} // RA2: as above
}; // RA2: as above

class FollowCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D450; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "Follow"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_FOLLOW", NULL, "D:\\Ra2\\Init.CPP", 7764); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7766); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_FOLLOW_DESC", NULL, "D:\\Ra2\\Init.CPP", 7768); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (CurrentObject.Count() != 0 && Map.Object_To_Follow() == NULL) { // RA2: as above
				Map.Set_To_Follow(CurrentObject[0]); // RA2: as above
			} else { // RA2: as above
				Map.Break_Follow_Mode(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class ScreenCaptureCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D500; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ScreenCapture"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7876); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SCRNCAP", NULL, "D:\\Ra2\\Init.CPP", 7877); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_SCRNCAP_DESC", NULL, "D:\\Ra2\\Init.CPP", 7878); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			RECT win_rect; // RA2: as above
			
			if (GetClientRect(MainWindow, &win_rect)) { // RA2: as above
				POINT upper_left; // RA2: as above
				upper_left.x = win_rect.left; // RA2: as above
				upper_left.y = win_rect.top; // RA2: as above
				if (!ClientToScreen(MainWindow, &upper_left)) { // RA2: as above
					return; // RA2: as above
				} // RA2: as above
				POINT lower_right; // RA2: as above
				lower_right.x = win_rect.right; // RA2: as above
				lower_right.y = win_rect.bottom; // RA2: as above
				if (!ClientToScreen(MainWindow, &lower_right)) { // RA2: as above
					return; // RA2: as above
				} // RA2: as above
				Rect dest_rect(upper_left.x, upper_left.y, win_rect.right + 1, win_rect.bottom + 1); // RA2: as above
				dest_rect.Width = MIN(dest_rect.Width, HiddenSurface->Get_Width()); // RA2: as above
				dest_rect.Height = MIN(dest_rect.Height, HiddenSurface->Get_Height()); // RA2: as above
				MouseCursor->Hide_Mouse(); // RA2: as above
				HiddenSurface->Blit_From(Rect(0, 0, HiddenSurface->Get_Width(), HiddenSurface->Get_Height()), *VisibleSurface, dest_rect); // RA2: as above
				MouseCursor->Show_Mouse(); // RA2: as above
				char fname[128]; // RA2: as above
				int index = -1; // RA2: as above
				do { // RA2: as above
					index++; // RA2: as above
					sprintf(fname, "SCRN%04d.pcx", index); // RA2: as above
				} while (CCFileClass(fname).Is_Available()); // RA2: as above
				CCFileClass file(fname); // RA2: as above
				Write_PCX_File(file, *HiddenSurface, &GamePalette); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class MultiplayerDebugCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D780; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "MultiplayerDebug"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return L"Debug"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return L"Multiplayer debug info"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return L"Turns multiplayer debug on or off."; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Session.EnableMultiplayerDebug) { // RA2: as above
				Session.DrawMPDebugStats = !Session.DrawMPDebugStats; // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class MultiplayerSyncCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D7E0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "ExtraSyncChecking"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return L"Debug"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return L"Game sync logging"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return L"Turns extra game sync logging on or off."; // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			ExtraSyncInfo = !ExtraSyncInfo; // RA2: as above
			if (ExtraSyncInfo) { // RA2: as above
				Session.Messages.Add_Message(NULL, 0, Narrow_To_Wide("Extra sync info ON"), 3, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW | TPF_USE_GRAD_PAL), 150, false); // RA2: as above
			} else { // RA2: as above
				Session.Messages.Add_Message(NULL, 0, Narrow_To_Wide("Extra sync info OFF"), 3, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW | TPF_USE_GRAD_PAL), 150, false); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class PageUserCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D870; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "PageUser"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PAGEUSER", NULL, "D:\\Ra2\\Init.CPP", 7368); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 7370); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_PAGEUSER_DESC", NULL, "D:\\Ra2\\Init.CPP", 7372); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			if (Session.Type == GAME_INTERNET) { // RA2: as above
				Queue_PageUser(); // RA2: as above
			} // RA2: as above
		} // RA2: as above
}; // RA2: as above

class CursorPositionCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D910; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "CursorCheat"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("GUI:CursorCheat", NULL, "D:\\Ra2\\Init.CPP", 8040); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("GUI:Debug", NULL, "D:\\Ra2\\Init.CPP", 8041); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("DESC:CursorCheat", NULL, "D:\\Ra2\\Init.CPP", 8042); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			CursorCheat = !CursorCheat; // RA2: as above
		} // RA2: as above
}; // RA2: as above

class DeleteCommandClass : public CommandClass // RA2: the executable's methods of this class start at 0x0051D9B0; not in TibSun's text
{ // RA2: as above
	public: // RA2: as above
		virtual char const * Get_Unique_Name(void) const // RA2: as above
		{ // RA2: as above
			return "Delete"; // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Display_Name(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:Delete", NULL, "D:\\Ra2\\Init.CPP", 6304); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Category(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("TXT_INTERFACE", NULL, "D:\\Ra2\\Init.CPP", 6306); // RA2: as above
		} // RA2: as above
		virtual wchar_t const * Get_Description(void) const // RA2: as above
		{ // RA2: as above
			return StringTable::LoadString("Cmnd:DeleteDesc", NULL, "D:\\Ra2\\Init.CPP", 6308); // RA2: as above
		} // RA2: as above
		virtual void Execute(int key) const // RA2: as above
		{ // RA2: as above
			Command_Delete_Plan_Or_Beacon(); // RA2: as above
		} // RA2: as above
}; // RA2: as above

/// <summary>
/// An empty function. Nothing in the executable calls it.
/// </summary>
void Unused_Init_Hook(void)
{
}

bool Init_Hotkeys(void); // RA2: as above

void Init_Commands(void) // RA2: 0x005173C0; not static; the list is RA2's: the multiplayer debug commands first, the sidebar commands in left and right variants, the team, taunt, screen capture, page user, cursor position and delete commands
{
	if (Session.EnableMultiplayerDebug) { // RA2: as above
		AllCommands.Add(new MultiplayerDebugCommandClass); // RA2: as above
		AllCommands.Add(new MultiplayerSyncCommandClass); // RA2: as above
	}
	AllCommands.Add(new FollowCommandClass);
	AllCommands.Add(new View1CommandClass);
	AllCommands.Add(new View2CommandClass);
	AllCommands.Add(new View3CommandClass);
	AllCommands.Add(new View4CommandClass);
	AllCommands.Add(new SetView1CommandClass);
	AllCommands.Add(new SetView2CommandClass);
	AllCommands.Add(new SetView3CommandClass);
	AllCommands.Add(new SetView4CommandClass);
	const CommandClass * optcmd = new OptionsCommandClass;
	AllCommands.Add(optcmd);
	AllCommands.Add(new SidebarUpCommandClass);
	AllCommands.Add(new LSidebarUpCommandClass);
	AllCommands.Add(new RSidebarUpCommandClass);
	AllCommands.Add(new SidebarDownCommandClass);
	AllCommands.Add(new LSidebarDownCommandClass);
	AllCommands.Add(new RSidebarDownCommandClass);
	AllCommands.Add(new SidebarPageUpCommandClass);
	AllCommands.Add(new LSidebarPageUpCommandClass);
	AllCommands.Add(new RSidebarPageUpCommandClass);
	AllCommands.Add(new SidebarPageDownCommandClass);
	AllCommands.Add(new LSidebarPageDownCommandClass);
	AllCommands.Add(new RSidebarPageDownCommandClass);
	const CommandClass * reventcmd = new CenterREventCommandClass; // RA2: as above
	AllCommands.Add(reventcmd); // RA2: as above
	AllCommands.Add(new BeaconPlacementCommandClass); // RA2: as above
	AllCommands.Add(new ToggleSellCommandClass);
	AllCommands.Add(new ToggleRepairCommandClass);
	AllCommands.Add(new AllianceCommandClass);
	AllCommands.Add(new CenterBaseCommandClass);
	AllCommands.Add(new CenterViewCommandClass);
	AllCommands.Add(new ScatterCommandClass);
	AllCommands.Add(new GuardCommandClass);
	AllCommands.Add(new StopCommandClass);
	AllCommands.Add(new AllToCheerCommandClass); // RA2: as above
	AllCommands.Add(new DeployCommandClass);
	AllCommands.Add(new PrevObjectCommandClass);
	AllCommands.Add(new NextObjectCommandClass);
	AllCommands.Add(new PlanningModeCommandClass); // RA2: as above
	AllCommands.Add(new CombatantSelectCommandClass); // RA2: as above
	AllCommands.Add(new TypeSelectCommandClass); // RA2: as above
	AllCommands.Add(new HealthNavCommandClass); // RA2: as above
	AllCommands.Add(new VeterancyNavCommandClass); // RA2: as above
	AllCommands.Add(new SetStructureTabCommandClass); // RA2: as above
	AllCommands.Add(new SetDefenseTabCommandClass); // RA2: as above
	AllCommands.Add(new SetUnitTabCommandClass); // RA2: as above
	AllCommands.Add(new SetInfantryTabCommandClass); // RA2: as above
	AllCommands.Add(new CreateTeamCommandClass(1));
	AllCommands.Add(new CreateTeamCommandClass(2));
	AllCommands.Add(new CreateTeamCommandClass(3));
	AllCommands.Add(new CreateTeamCommandClass(4));
	AllCommands.Add(new CreateTeamCommandClass(5));
	AllCommands.Add(new CreateTeamCommandClass(6));
	AllCommands.Add(new CreateTeamCommandClass(7));
	AllCommands.Add(new CreateTeamCommandClass(8));
	AllCommands.Add(new CreateTeamCommandClass(9));
	AllCommands.Add(new CreateTeamCommandClass(10));
	AllCommands.Add(new SelectTeamCommandClass(1));
	AllCommands.Add(new SelectTeamCommandClass(2));
	AllCommands.Add(new SelectTeamCommandClass(3));
	AllCommands.Add(new SelectTeamCommandClass(4));
	AllCommands.Add(new SelectTeamCommandClass(5));
	AllCommands.Add(new SelectTeamCommandClass(6));
	AllCommands.Add(new SelectTeamCommandClass(7));
	AllCommands.Add(new SelectTeamCommandClass(8));
	AllCommands.Add(new SelectTeamCommandClass(9));
	AllCommands.Add(new SelectTeamCommandClass(10));
	AllCommands.Add(new AddTeamCommandClass(1));
	AllCommands.Add(new AddTeamCommandClass(2));
	AllCommands.Add(new AddTeamCommandClass(3));
	AllCommands.Add(new AddTeamCommandClass(4));
	AllCommands.Add(new AddTeamCommandClass(5));
	AllCommands.Add(new AddTeamCommandClass(6));
	AllCommands.Add(new AddTeamCommandClass(7));
	AllCommands.Add(new AddTeamCommandClass(8));
	AllCommands.Add(new AddTeamCommandClass(9));
	AllCommands.Add(new AddTeamCommandClass(10));
	AllCommands.Add(new CenterTeamCommandClass(1));
	AllCommands.Add(new CenterTeamCommandClass(2));
	AllCommands.Add(new CenterTeamCommandClass(3));
	AllCommands.Add(new CenterTeamCommandClass(4));
	AllCommands.Add(new CenterTeamCommandClass(5));
	AllCommands.Add(new CenterTeamCommandClass(6));
	AllCommands.Add(new CenterTeamCommandClass(7));
	AllCommands.Add(new CenterTeamCommandClass(8));
	AllCommands.Add(new CenterTeamCommandClass(9));
	AllCommands.Add(new CenterTeamCommandClass(10));
	AllCommands.Add(new TauntCommandClass(1)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(2)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(3)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(4)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(5)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(6)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(7)); // RA2: as above
	AllCommands.Add(new TauntCommandClass(8)); // RA2: as above
	AllCommands.Add(new ScreenCaptureCommandClass);
	AllCommands.Add(new PageUserCommandClass);
	AllCommands.Add(new CursorPositionCommandClass); // RA2: as above
	const CommandClass * delcmd = new DeleteCommandClass; // RA2: as above
	AllCommands.Add(delcmd); // RA2: as above
	Init_Hotkeys();
	if (HotkeyCommands.Is_Present(KN_DELETE)) {
		HotkeyCommands.Remove_Index(KN_DELETE);
	}
	HotkeyCommands.Add_Index(KN_DELETE, delcmd); // RA2: as above
	if (HotkeyCommands.Is_Present(KN_ESC)) {
		HotkeyCommands.Remove_Index(KN_ESC);
	}
	HotkeyCommands.Add_Index(KN_ESC, optcmd);
	if (HotkeyCommands.Is_Present(KN_SPACE)) { // RA2: as above
		HotkeyCommands.Remove_Index(KN_SPACE); // RA2: as above
	}
	HotkeyCommands.Add_Index(KN_SPACE, reventcmd); // RA2: as above
}


/// <summary>
/// Binds the game commands to the keys named in KEYBOARD.INI.
/// This routine throws the current key assignments away and rebuilds them from the
/// player's keyboard file, matching each entry against a command's unique name.
/// </summary>
/// <returns>bool; Was the keyboard file loaded?</returns>
/// <remarks>The command list must already be built before calling this routine.</remarks>
bool Init_Hotkeys(void) // RA2: 0x00519240; KEYBOARD.INI is read with Load(file, false) and a command is found by its unique name
{
	CCINIClass ini;
	CCFileClass file("KEYBOARD.INI");

	if (ini.Load(file, false)) {
		HotkeyCommands.Clear();
		for (int index = 0; index < ini.Entry_Count("Hotkey"); index++) {
			const char * entry = ini.Get_Entry("Hotkey", index); // RA2: as above
			int id = ini.Get_Int("Hotkey", entry, 0);
			const CommandClass * command = NULL; // RA2: as above

			for (int cindex = 0; cindex < AllCommands.Count(); cindex++) {
				if (strcmp(AllCommands[cindex]->Get_Unique_Name(), entry) == 0) {
					command = AllCommands[cindex];
					break;
				}
			}

			if ((command != NULL) && (id != 0)) {
				HotkeyCommands.Add_Index(id, command);
			}
		}
		return(true);
	}

	DebugString("Unable to load KEYBOARD.INI\n");
	return(false);
}

/// <summary>
/// Executes the command that goes by the specified unique name.
/// Use this routine where a command must be triggered by name rather than by the key it
/// happens to be bound to. A name that matches no command is quietly ignored.
/// </summary>
/// <param name="name">The unique name of the command to execute.</param>
void Execute_Command(char const * name) // RA2: 0x00519480; the command takes a key, 0 here
{
	for (int command = 0; command < AllCommands.Count(); command++) {
		if (strcmp(AllCommands[command]->Get_Unique_Name(), name) == 0) {
			AllCommands[command]->Execute(0); // RA2: as above
			break;
		}
	}
}



/*
**	RA2: globals this part of the file uses (addresses from the executable; the headers do not declare them yet).
*/


/// <summary>
/// Allocates the game's drawing surfaces.
/// This routine releases whatever surfaces are already in hand and builds a fresh set to
/// the dimensions given. A rectangle that is not valid means that surface is not wanted.
/// The composite and tile surfaces must share the same kind of memory as each other, so
/// both are pushed into system memory if the video card cannot hold the pair.
/// </summary>
/// <param name="hidden_rect">The dimensions for the hidden and alternate surfaces.</param>
/// <param name="composite_rect">The dimensions for the composite surface.</param>
/// <param name="tile_rect">The dimensions for the tile surface.</param>
/// <param name="sidebar_rect">The dimensions for the sidebar surface.</param>
/// <param name="hidden_first">Should the hidden surface get first claim on video memory?</param>
/// <returns>bool; Were the surfaces allocated?</returns>
#ifdef RA2_NON_MATCHING // not exact: 1015 bytes differ (adopted text)
bool Allocate_Surfaces(const Rect & hidden_rect, const Rect & composite_rect, const Rect & tile_rect, const Rect & sidebar_rect, bool hidden_first) // RA2: 0x00519500; the composite and tile surfaces follow VideoBackBuffer, the sidebar surface AllowVRAMSidebar; compiles to the executable's 1120 bytes, but with the hidden and composite rectangles in the other two registers and the memory flags on the stack, so it is excepted
{
	DebugString("Allocating new surfaces\n");

	if (AlternateSurface != NULL) {
		DebugString("Deleting AlternateSurface\n");
		delete AlternateSurface;
		AlternateSurface = NULL;
	}

	if (HiddenSurface != NULL) {
		DebugString("Deleting HiddenSurface\n");
		delete HiddenSurface;
		HiddenSurface = NULL;
	}

	if (CompositeSurface != NULL) {
		DebugString("Deleting CompositeSurface\n");
		delete CompositeSurface;
		CompositeSurface = NULL;
	}

	if (TileSurface != NULL) {
		DebugString("Deleting TileSurface\n");
		delete TileSurface;
		TileSurface = NULL;
	}

	if (SidebarSurface != NULL) {
		DebugString("Deleting SidebarSurface\n");
		delete SidebarSurface;
		SidebarSurface = NULL;
	}

	bool tile_in_system = !VideoBackBufferAllowed; // RA2: the composite and tile surfaces go to system memory unless the video back buffer is allowed
	bool sidebar_in_system = !Options.AllowVRAMSidebar; // RA2: as above for the sidebar surface

	if (hidden_first && hidden_rect.Is_Valid()) {
		HiddenSurface = new DSurface(hidden_rect.Width, hidden_rect.Height, false); // RA2: as above
		HiddenSurface->Fill(0);

		DebugString("HiddenSurface (%dx%d) %s\n", hidden_rect.Width, hidden_rect.Height,
					((DSurface *)HiddenSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	if (composite_rect.Is_Valid()) {
		CompositeSurface = new DSurface(composite_rect.Width, composite_rect.Height, tile_in_system); // RA2: as above
		CompositeSurface->Fill(0);

		DebugString("CompositeSurface (%dx%d) %s\n", composite_rect.Width, composite_rect.Height,
					((DSurface *)CompositeSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	if (tile_rect.Is_Valid()) {
		TileSurface = new DSurface(tile_rect.Width, tile_rect.Height, tile_in_system); // RA2: as above
		TileSurface->Fill(0);

		DebugString("TileSurface (%dx%d) %s\n", tile_rect.Width, tile_rect.Height,
					((DSurface *)TileSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	if ((CompositeSurface != NULL) && (TileSurface != NULL)) {
		bool is_composite_surf_in_vram = ((DSurface *)CompositeSurface)->In_Video_Ram();
		bool is_tile_surf_in_vram = ((DSurface *)TileSurface)->In_Video_Ram();

		if (is_composite_surf_in_vram ^ is_tile_surf_in_vram) {
			delete CompositeSurface;
			delete TileSurface;
			DebugString("Moving TileSurface and CompositeSurface into system memory\n");
			CompositeSurface = new DSurface(composite_rect.Width, composite_rect.Height, true);
			TileSurface = new DSurface(tile_rect.Width, tile_rect.Height, true);
		}
	}

	if (sidebar_rect.Is_Valid()) {
		SidebarSurface = new DSurface(sidebar_rect.Width, sidebar_rect.Height, sidebar_in_system); // RA2: as above
		SidebarSurface->Fill(0);

		DebugString("SidebarSurface (%dx%d) %s\n", sidebar_rect.Width, sidebar_rect.Height,
					((DSurface *)SidebarSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	if (!hidden_first && hidden_rect.Is_Valid()) {
		HiddenSurface = new DSurface(hidden_rect.Width, hidden_rect.Height, false); // RA2: as above
		HiddenSurface->Fill(0);

		DebugString("HiddenSurface (%dx%d) %s\n", hidden_rect.Width, hidden_rect.Height,
					((DSurface *)HiddenSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	if (hidden_rect.Is_Valid()) {
		AlternateSurface = new DSurface(hidden_rect.Width, hidden_rect.Height, true);
		AlternateSurface->Fill(0);

		DebugString("AlternateSurface (%dx%d) %s\n", hidden_rect.Width, hidden_rect.Height,
					((DSurface *)AlternateSurface)->In_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
	}

	return(true);
}
#endif


/// <summary>
/// Deletes every game object and object type in existence.
/// This routine is used when tearing a scenario down so that the next one may start from a
/// clean slate. Everything from bullets through houses to the tactical map itself is
/// released, and the cell array goes with them.
/// </summary>
/// <remarks>The game logic must not be running -- there is nothing left for it to run
/// upon.</remarks>
void Delete_All_Objects(void) // RA2: 0x00519960; RA2 has more object lists to empty, no veinhole monsters, and no checks at the end
{
	ScenarioInit++;

	ObjectsToDelete.Clear();

	while (TargetTracker.Count()) {
		AbstractClass * ptr = TargetTracker.Fetch_By_Position(0);
		delete ptr;
	}
	Process_Deferred_Deletion();

	while (Bullets.Count()) {
		Bullets[0]->Release();
	}
	Process_Deferred_Deletion();

	while (Objects.Count()) {
		delete Objects[0];
	}
	Process_Deferred_Deletion();

	while (Tags.Count()) {
		delete Tags[0];
	}
	Process_Deferred_Deletion();

	while (Triggers.Count()) {
		delete Triggers[0];
	}
	Process_Deferred_Deletion();

	while (Tubes.Count()) {
		delete Tubes[0];
	}
	Process_Deferred_Deletion();

	while (BuildingLights.Count()) {
		delete BuildingLights[0];
	}
	Process_Deferred_Deletion();

	while (Overlays.Count()) {
		delete Overlays[0];
	}
	Process_Deferred_Deletion();

	while (ParticleSystems.Count()) {
		delete ParticleSystems[0];
	}
	Process_Deferred_Deletion();

	while (Waves.Count()) {
		delete Waves[0];
	}
	Process_Deferred_Deletion();

	while (Factories.Count()) {
		delete Factories[0];
	}
	Process_Deferred_Deletion();

	while (Sides.Count()) {
		delete Sides[0];
	}
	Process_Deferred_Deletion();

	while (Teams.Count()) {
		delete Teams[0];
	}
	Process_Deferred_Deletion();

	while (Houses.Count()) {
		delete Houses[0];
	}
	Process_Deferred_Deletion();

	while (Anims.Count()) {
		delete Anims[0];
	}
	Process_Deferred_Deletion();

	while (Scripts.Count()) {
		delete Scripts[0];
	}
	Process_Deferred_Deletion();

	while (RadSites.Count()) { // RA2: as above
		delete RadSites[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (LightSources.Count()) {
		delete LightSources[0];
	}
	Process_Deferred_Deletion();

	while (EMPulseClass::EMPulses.Count()) {
		delete EMPulseClass::EMPulses[0];
	}
	Process_Deferred_Deletion();

	while (CaptureManagerArray.Count()) { // RA2: as above
		delete CaptureManagerArray[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (Parasites.Count()) { // RA2: as above
		delete Parasites[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (Temporals.Count()) { // RA2: as above
		delete Temporals[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (SpawnManagers.Count()) { // RA2: as above
		delete SpawnManagers[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (Bombs.Count()) { // RA2: as above
		delete Bombs[0]; // RA2: as above
	}
	Process_Deferred_Deletion();

	while (SpotLights.Count()) {
		delete SpotLights[0];
	}
	Process_Deferred_Deletion();

	while (FoggedObjectClass::FoggyObjects.Count()) {
		delete FoggedObjectClass::FoggyObjects[0];
	}
	Process_Deferred_Deletion();

	while (AlphaShapes.Count()) {
		delete AlphaShapes[0];
	}
	Process_Deferred_Deletion();

	while (Terrains.Count()) {
		delete Terrains[0];
	}
	Process_Deferred_Deletion();

	EBolt::Delete_All(); // RA2: as above
	Delete_All_Line_Trails(); // RA2: as above
	LaserDrawClass::All_Clear();

	while (AbstractTypes.Count()) {
		delete AbstractTypes[0];
	}
	Process_Deferred_Deletion();

	delete TacticalMap;
	TacticalMap = NULL;
	Map.Free_Cells();
	Process_Deferred_Deletion();

	BeaconManager.Clear_Slots(); // RA2: as above
	ScenarioInit--;
}


/***********************************************************************************************
 * DisplayClass::Init_Theater -- Performs theater-specific initialization (mixfiles, etc)      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      theater         new theater                                                            *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/17/1995 BRR : Created.                                                                 *
 *   05/07/1996 JLB : Added translucent tables.                                                *
 *=============================================================================================*/
void Init_Theater(TheaterType theater) // RA2: 0x00519E70; RA2 also keeps GENERIC.MIX and ISOGEN.MIX, loads the UNIT palette by theater name and switches the colour scheme table
{
	char fullname[16];
	char shortname[16];
	char isofullname[16];

	wsprintf(fullname, "%s.MIX", Theaters[theater].Root);
	wsprintf(isofullname, "%s.MIX", Theaters[theater].IsoRoot);
	wsprintf(shortname, "%s.MIX", Theaters[theater].Suffix);
	DebugString("Init theater %s\n", Theaters[theater].Name);

	Scen->Theater = theater;
	Session.Update_Progress(8);

	if (Scen->Theater != THEATER_SNOW && (LastTheater == THEATER_SNOW || LastTheater == THEATER_NONE)) { // RA2: as above
		if (GenericMix != NULL) { // RA2: as above
			delete GenericMix; // RA2: as above
		}
		GenericMix = new MFCD("GENERIC.MIX", &FastKey); // RA2: as above
		GenericMix->Cache(); // RA2: as above
	}

	if (Scen->Theater != LastTheater) {
		if (TheaterData != NULL) {
			delete TheaterData;
		}
		TheaterData = new MFCD(fullname, &FastKey);

		if (TheaterDat != NULL) {
			delete TheaterDat;
		}
		TheaterDat = new MFCD(shortname, &FastKey);
		TheaterDat->Cache();
		TheaterData->Cache();
		Session.Update_Progress(6);

		if (IsometricTheaterData != NULL) {
			delete IsometricTheaterData;
		}
		IsometricTheaterData = new MFCD(isofullname, &FastKey);

		if (IsoGenericMix != NULL) { // RA2: as above
			delete IsoGenericMix; // RA2: as above
		}
		IsoGenericMix = new MFCD("ISOGEN.MIX", &FastKey); // RA2: as above
		Session.Update_Progress(12);

		wsprintf(fullname, "%s.PAL", Theaters[theater].Root);
		unsigned char * ptr = (unsigned char *)MFCD::Retrieve(fullname);
		if (ptr != NULL) {
			for (int color = 0; color < PaletteClass::COLOR_COUNT; color++) {
				unsigned char r = (unsigned char)((*ptr++)<<2);
				unsigned char g = (unsigned char)((*ptr++)<<2);
				unsigned char b = (unsigned char)((*ptr++)<<2);
				GamePalette[color] = RGBClass(r, g, b);
			}
		} else {
			for (int color = 0; color < PaletteClass::COLOR_COUNT; color++) {
				GamePalette[color] = RGBClass(color, 255 - color, ((color << 2) & 0xff));
			}
		}
		OriginalPalette = GamePalette;

		wsprintf(fullname, "UNIT%s.PAL", Theaters[theater].Suffix); // RA2: as above
		PaletteClass * unitpal = (PaletteClass *)MFCD::Retrieve(fullname); // RA2: as above
		Call_Back();
		if (unitpal != NULL) {
			SchemePalette = *unitpal;
		}
		for (int index = 0; index < 256; index++) {
			SchemePalette[index] = RGBClass(
					(unsigned char)(SchemePalette[index].Get_Red()<<2),
					(unsigned char)(SchemePalette[index].Get_Green()<<2),
					(unsigned char)(SchemePalette[index].Get_Blue()<<2));
		}

		int last_percent = 12;
		int prog_step = ColorSchemes.Count() / (25 - 12);
		for (int s = 0; s < ColorSchemes.Count(); s++) {
			ColorSchemes[s]->Build_Light_Converters(SchemePalette, GamePalette);
			int percent = (min(12 + (s / prog_step),25));
			if (percent != last_percent) {
				Session.Update_Progress(percent);
				last_percent = percent;
			}
			Call_Back();
		}
		Set_Scheme_Table_Theater(theater); // RA2: as above
		Load_Techno_Palettes(); // RA2: as above
		Session.Update_Progress(25);
	}
}


bool __cdecl Release_Neutral_Mix(void) // RA2: as above
{
	if (NeutralMix != NULL) { // RA2: as above
		DebugString("     Releasing %s\n", NeutralMix->Filename); // RA2: as above
		delete NeutralMix; // RA2: as above
		NeutralMix = NULL; // RA2: as above
	}
	return(true);
}


bool __cdecl Load_Neutral_Mix(void) // RA2: as above
{
	if (NeutralMix != NULL) { // RA2: as above
		DebugString("     Releasing %s\n", NeutralMix->Filename); // RA2: as above
		delete NeutralMix; // RA2: as above
		NeutralMix = NULL; // RA2: as above
	}
	NeutralMix = new MFCD("NEUTRAL.MIX", &FastKey); // RA2: as above
	if (NeutralMix == NULL) { // RA2: as above
		return(false);
	}
	DebugStringNoPrefix("LOADED NEUTRAL.MIX"); // RA2: as above
	return(true);
}


/// <summary>
/// Prepares the art and data mixfiles for the specified side.
/// This routine is called whenever the side being played changes. It releases the previous
/// side's archives, mounts the cached, uncached and CD archives belonging to the new one,
/// and then lets the map rebuild whatever it keeps on a per house basis.
/// </summary>
/// <param name="side">The side whose archives should be made available.</param>
/// <returns>bool; Were all of the required archives for the side found?</returns>
bool Prep_For_Side(SideType side) // RA2: 0x0051A330; RA2 knows no expansion mixes, loads SIDEC and SIDENC only and then UI.INI
{
	if (side == -1) { // RA2: as above
		int disk = DiskSwap::Get_Disk_ID(CDFileClass::Get_CD_Drive(), 60); // RA2: as above
		if (disk != 0 && disk != 1) { // RA2: as above
			DebugString("     FAILED!  This was not disk one or two!!!\n"); // RA2: as above
			return(false);
		}
		side = (SideType)disk; // RA2: as above
	}
	DebugString("Preparing Mixfiles for Side %02d.\n", side);

	if (SideCDMix != NULL) {
		DebugString("     Releasing %s\n", SideCDMix->Filename);
		delete SideCDMix;
		SideCDMix = NULL;
	}
	if (SideCMix != NULL) {
		DebugString("     Releasing %s\n", SideCMix->Filename);
		delete SideCMix;
		SideCMix = NULL;
	}
	if (SideNCMix != NULL) {
		DebugString("     Releasing %s\n", SideNCMix->Filename);
		delete SideNCMix;
		SideNCMix = NULL;
	}

	int id = (int)side + 1; // RA2: as above
	Speech_Set_Side(side); // RA2: as above

	char name[64];
	sprintf(name, "SIDEC%02d.MIX", id);
	DebugString("     Initilizing %s\n", name);
	if (CCFileClass(name).Is_Available()) {
		SideCMix = new MFCD(name, &FastKey);
	}
	if (SideCMix == NULL) {
		DebugString("     FAILED!\n");
		return(false);
	}
	SideCMix->Cache();

	sprintf(name, "SIDENC%02d.MIX", id);
	DebugString("     Initilizing %s\n", name);
	if (CCFileClass(name).Is_Available()) {
		SideNCMix = new MFCD(name, &FastKey);
	}

	Reload_Sidebar_Art(); // RA2: as above
	Select_Tooltip_Color(side); // RA2: as above

	CCFileClass file("UI.INI"); // RA2: as above
	if (UIINI.Load(file, false)) { // RA2: as above
		Rule->Read_AdvancedCommandBar(UIINI, Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH); // RA2: as above
		Map.Init_For_House();
		return(true);
	}
	DebugString("Failed to load UI.INI!\n"); // RA2: as above
	return(false);
}


/// <summary>
/// Fetches the most interesting object out of the current selection.
/// This routine is used wherever a single object must stand in for the whole selection.
/// An armed and mobile combatant outranks a defensive building, and anything that cannot
/// move under its own power ranks lowest of all.
/// </summary>
/// <returns>Returns with a pointer to the best object selected. Otherwise, NULL is
/// returned.</returns>
ObjectClass * Best_Selected_Object(Cell const * cell, ObjectClass const * object) // RA2: 0x0051A690; takes the cell or the object the answer is nearest to
{
	ObjectClass * best_obj = NULL; // RA2: as above
	int best_value = -1;
	int best_distance = 0x7FFFFFFF; // RA2: as above

	if (CurrentObject.Count() > 0) {
		for (int index = 0; index < CurrentObject.Count(); index++) {
			ObjectClass * obj = CurrentObject[index];

			int value = 0;

			bool is_techno = false; // RA2: as above
			if (obj != NULL) {
				is_techno = obj->AbstractFlags & ABSTRACT_TECHNO; // RA2: as above
			}
			if (is_techno) { // RA2: as above
				value = 2;
				TechnoClass * tech = (TechnoClass *)obj;
				if (tech->IsUnderEMP()) { // RA2: as above
					value = 1;
				} else if (tech->Is_Weapon_Equipped()) {
					value = 3;
					if (tech->RTTI != RTTI_BUILDING) {
						value = 4;
						if (tech->Combat_Damage() > 0) {
							value = 5;
						}
					}
				}
			}

			if (value >= best_value) { // RA2: as above
				int distance = 0x7FFFFFFF; // RA2: as above
				bool has_cell = cell != NULL; // RA2: as above
				if (has_cell || object != NULL) { // RA2: as above
					Coord here = obj->Center_Coord(); // RA2: as above
					Coord there; // RA2: as above
					if (has_cell) { // RA2: as above
						there = cell->As_Coord(); // RA2: as above
					} else {
						there = object->Center_Coord(); // RA2: as above
					}
					distance = here.Distance_To(there); // RA2: as above
				}
				if (value > best_value) {
					best_value = value;
					best_obj = obj;
					best_distance = distance; // RA2: as above
				} else if (distance < best_distance) { // RA2: as above
					best_obj = obj;
					best_distance = distance; // RA2: as above
				}
			}
		}
	}
	return(best_obj);
}


ThemeType Fetch_Main_Menu_Theme(void)
{
	return(Theme.From_Name("INTRO"));
}


static int PerformanceProfile = -1; // RA2: 0x007DB564; the overall rating of this machine, -1 until Get_Performance_Profile has measured it

int Get_Performance_Profile(void) // RA2: as above
{
	if (PerformanceProfile == -1) { // RA2: as above
		ProfileText[0] = 0; // RA2: as above
		ProfileCPUText[0] = 0; // RA2: as above
		ProfileRAMText[0] = 0; // RA2: as above
		ProfileVRAMText[0] = 0; // RA2: as above
		ProfileBlitText[0] = 0; // RA2: as above

		int type = Get_CPU_Type(); // RA2: as above
		int mhz = Get_RDTSC_CPU_Speed(); // RA2: as above
		int cpu = 5; // RA2: as above
		switch (type) { // RA2: as above
			case -1: // RA2: as above
				break;

			case 3:
			case 5: // RA2: as above
				cpu = mhz / 100 + 1; // RA2: as above
				break;

			case 4:
			case 7: // RA2: as above
				cpu = mhz / 100 + 2; // RA2: as above
				break;

			case 6: // RA2: as above
				cpu = mhz / 100 + 3; // RA2: as above
				break;

			default:
				cpu = 1; // RA2: as above
				break;
		}
		DebugString("Profile: CPU:%d (%dMhz %s)\n", cpu, mhz, Get_CPU_Type_Name()); // RA2: as above
		sprintf(ProfileCPUText, "CPU: %d Mhz %s", mhz, Get_CPU_Type_Name()); // RA2: as above

		MEMORYSTATUS status; // RA2: as above
		status.dwLength = sizeof(status); // RA2: as above
		GlobalMemoryStatus(&status); // RA2: as above
		int ram = 2; // RA2: as above
		unsigned int megabytes = (status.dwTotalPhys + 0xA0000) >> 20; // RA2: as above
		if (megabytes >= 64) { // RA2: as above
			ram = 5; // RA2: as above
		}
		if (megabytes >= 128) { // RA2: as above
			ram += 2; // RA2: as above
		}
		if (megabytes == 256) { // RA2: as above
			ram += 2; // RA2: as above
		} else if (megabytes > 256) { // RA2: as above
			ram = 10; // RA2: as above
		}
		DebugString("Profile: RAM:%d (%dMb)\n", ram, megabytes); // RA2: as above
		sprintf(ProfileRAMText, "RAM reported: %d Mb", megabytes); // RA2: as above

		megabytes = 4; // RA2: as above
		int vram = 5; // RA2: as above
		if (DirectDrawObject != NULL) { // RA2: as above
			DDCAPS caps; // RA2: as above
			caps.dwSize = sizeof(caps); // RA2: as above
			if (DirectDrawObject->GetCaps(&caps, NULL) == 0) { // RA2: as above
				unsigned int memory = caps.dwVidMemTotal > caps.dwVidMemFree ? caps.dwVidMemTotal : caps.dwVidMemFree; // RA2: as above
				megabytes = (memory + 0xFFFFF) >> 20; // RA2: as above
			}
		}
		if (megabytes >= 32) { // RA2: as above
			vram = 10; // RA2: as above
		} else if (megabytes >= 16) { // RA2: as above
			vram = 9; // RA2: as above
		} else if (megabytes >= 8) { // RA2: as above
			vram = 7; // RA2: as above
		} else if (megabytes >= 4) { // RA2: as above
			vram = 5; // RA2: as above
		} else if (megabytes <= 2) { // RA2: as above
			vram = 3; // RA2: as above
		}
		DebugString("Profile: VRAM:%d (%dMb)\n", vram, megabytes); // RA2: as above
		sprintf(ProfileVRAMText, "VRAM reported: %d Mb", megabytes); // RA2: as above

		int speed = 5; // RA2: as above
		int blits = 0; // RA2: as above
		if (TileSurface != NULL && ((DSurface *)TileSurface)->In_Video_Ram() && !((DSurface *)AlternateSurface)->In_Video_Ram()) { // RA2: as above
			Rect area = TileSurface->Get_Rect(); // RA2: as above
			Rect other = AlternateSurface->Get_Rect(); // RA2: as above
			if (area.Width >= other.Width) { // RA2: as above
				area.Width = other.Width; // RA2: as above
			}
			if (area.Height >= other.Height) { // RA2: as above
				area.Height = other.Height; // RA2: as above
			}
			if (area.Width >= 480) { // RA2: as above
				area.Width = 480; // RA2: as above
			}
			if (area.Height >= 400) { // RA2: as above
				area.Height = 400; // RA2: as above
			}
			for (int index = 0; index < 4; index++) { // RA2: as above
				TileSurface->Blit_From(area, *AlternateSurface, area, false, true); // RA2: as above
				AlternateSurface->Blit_From(area, *TileSurface, area, false, true); // RA2: as above
				Sleep(0); // RA2: as above
			}
			DWORD start = timeGetTime(); // RA2: as above
			if ((start & 0xFF000000) != 0xFF) { // RA2: as above
				TileSurface->Blit_From(area, *AlternateSurface, area, false, true); // RA2: as above
				AlternateSurface->Blit_From(area, *TileSurface, area, false, true); // RA2: as above
				blits++; // RA2: as above
				while (timeGetTime() - start <= 1000 && blits <= 5000) { // RA2: as above
					TileSurface->Blit_From(area, *AlternateSurface, area, false, true); // RA2: as above
					AlternateSurface->Blit_From(area, *TileSurface, area, false, true); // RA2: as above
					blits++; // RA2: as above
				}
				speed = blits / 4 < 1 ? 1 : blits / 4; // RA2: as above
			}
		}
		DebugString("Profile: VRAM speed:%d (%d blits per second)\n", speed, blits * 2); // RA2: as above
		sprintf(ProfileBlitText, "%d blits per second\n", blits * 2); // RA2: as above

		PerformanceProfile = cpu; // RA2: as above
		if (PerformanceProfile >= ram) { // RA2: as above
			PerformanceProfile = ram; // RA2: as above
		}
		if (PerformanceProfile >= vram) { // RA2: as above
			PerformanceProfile = vram; // RA2: as above
		}
		if (PerformanceProfile >= speed) { // RA2: as above
			PerformanceProfile = speed; // RA2: as above
		}
		DebugString("Overall performance profile = %d\n", PerformanceProfile); // RA2: as above
		sprintf(ProfileText, "Profile: %d\n", PerformanceProfile); // RA2: as above
	}
	return(PerformanceProfile); // RA2: as above
}

