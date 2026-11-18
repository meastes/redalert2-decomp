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

//Mono_Printf("%d %s\n",__LINE__,__FILE__);
/* $Header: /CounterStrike/SCENARIO.CPP 15    3/13/97 2:06p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SCENARIO.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : October 21, 1996 [JLB]                                       *
 *                                                                                             *
 * This module handles the scenario reading and writing. Scenario related                      *
 * code that is executed between scenario play can also be here.                               *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Assign_Houses -- Assigns multiplayer houses to various players                            *
 *   Clear_Flag_Spots -- Clears flag overlays off the map                                      *
 *   Clear_Scenario -- Clears all data in preparation for scenario load.                       *
 *   Clip_Move -- moves in given direction from given cell; clips to map                       *
 *   Clip_Scatter -- randomly scatters from given cell; won't fall off map                     *
 *   Create_Units -- Creates infantry & units, for non-base multiplayer                        *
 *   Do_Lose -- Display losing comments.                                                       *
 *   Do_Restart -- Handle the restart mission process.                                         *
 *   Do_Win -- Display winning congratulations.                                                *
 *   Fill_In_Data -- Recreate all data that is not loaded with scenario.                       *
 *   Post_Load_Game -- Fill in an inferred data from the game state.                           *
 *   Read_Scenario -- Reads a scenario from disk.                                              *
 *   Read_Scenario_INI -- Read specified scenario INI file.                                    *
 *   Remove_AI_Players -- Removes the computer AI houses & their units                         *
 *   Restate_Mission -- Handles restating the mission objective.                               *
 *   Scan_Place_Object -- places an object >near< the given cell                               *
 *   ScenarioClass::ScenarioClass -- Constructor for the scenario control object.              *
 *   ScenarioClass::Set_Global_To -- Set scenario global to value specified.                   *
 *   Set_Scenario_Name -- Creates the INI scenario name string.                                *
 *   Start_Scenario -- Starts the scenario.                                                    *
 *   Write_Scenario_INI -- Write the scenario INI file.                                        *
 *   ScenarioClass::Do_BW_Fade -- Cause the palette to temporarily shift to B/W.               *
 *   ScenarioClass::Do_Fade_AI -- Process the palette fading effect.                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"

#include "scenario.h"

#include "_bench.h"
#include "_keyboar.h"
#include "_logic.h"
#include "_map.h"
#include "_palette.h"
#include "_rect.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "_timer.h"
#include "_tooltip.h"
#include "_wsproto.h"
#include "addon.h"
#include "aircraft.h"
#include "aitrig.h"
#include "anim.h"
#include "astar.h"
#include "bench.h"
#include "building.h"
#include "builtype.h"
#include "campaign.h"
#include "ccrand.h"
#include "cctooltip.h"
#include "cd.h"
#include "cell.h"
#include "conquer.h"
#include "convert.h"
#include "crc.h"
#include "data.h"
#include "dbgprint.h"
#include "egos.h"
#include "empulse.h"
#include "enviro.h"
#include "getcpu.h"
#include "globals.h"
#include "goptions.h"
#include "houstype.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "init.h"
#include "inline.h"
#include "ion.h"
#include "isotype.h"
#include "language\language.h"
#include "light.h"
#include "logic.h"
#include "mainopt.h"
#include "mapgen.h"
#include "mapsel.h"
#include "misc.h"
#include "mouse.h"
#include "movie.h"
#include "mpu.h"
#include "msgbox.h"
#include "netdlg.h"
#include "newmenu.h"
#include "overlay.h"
#include "overtype.h"
#include "partsys.h"
#include "preview.h"
#include "progress.h"
#include "psystype.h"
#include "queue.h"
#include "revent.h"
#include "rules.h"
#include "scheme.h"
#include "score.h"
#include "script.h"
#include "session.h"
#include "smudge.h"
#include "surface.h"
#include "swizzle.h"
#include "tactical.h"
#include "tag.h"
#include "tagtype.h"
#include "taskforc.h"
#include "teamtype.h"
#include "terrain.h"
#include "theme.h"
#include "tiberium.h"
#include "tracker.h"
#include "trigger.h"
#include "trigtype.h"
#include "tube.h"
#include "unit.h"
#include "unittype.h"
#include "vein.h"
#include "vox.h"
#include "voc.h"
#include "stringtable.h"
#include "wspipx.h"
#include "planning.h"
#include "ownertalk.h"
#include "ipxmgr.h"
#include "netdlg2.h"
#include "multiplayer.h"
#include "mainloop.h"
#include "kamikaze.h"
#include "bomblist.h"
#include "beacon.h"
#include "audlevel.h"
#include "auddsnd.h"
#include "audattr.h"
#include "wave.h"
#include "waypoint.h"
#include "wincomm.h"
#include "wonline.h"
#include "wsproto.h"
#include "ldprgmgr.h" // RA2: the load screen manager (0x535910), which this file declared as a placeholder class before

#include "bench.hh"
#include "wstring.h"
#include "mpscore.h"

void Audio_Pause_All(void); // RA2
void Audio_Resume_All(void); // RA2
bool Wait_For_Players_To_Load(void); // RA2: defined after Read_Scenario
void Reset_Group_Select(void); // RA2: 0x6F89E0, clears the control group last selected (GroupSelectGroup -1, GroupSelectFrame -100)
void Clear_Sidebar_Tab_Object(TechnoClass * techno); // RA2: 0x6F8A20, forgets techno in the two sidebar tab slots (all of them for NULL); YR: TechnoClass::ClearSidebarTabObject
void __fastcall AudioLevelMute(AudioLevel * level); // RA2: 0x406FA0
void __fastcall AudioLevelWaitFade(AudioLevel * level); // RA2: 0x406FE0
char * Wide_To_Narrow(wchar_t const * text); // RA2: 0x6FA120, static buffer (wonline.cpp declares it the same way)
Wstring Wstring_Narrow(WideWstring const & text); // RA2: 0x00771280 (wonline.cpp declares it the same way)
void __fastcall Set_Frame_Rate_Limit(int value); // RA2: 0x53D820 (logic.cpp)
void __fastcall Set_Buffer_Zone_Width(int value); // RA2: 0x53D830 (logic.cpp)
ConvertClass * Get_Splash_Bar_Drawer(void); // RA2: 0x6F2490
static inline int Player_Side(void); // RA2: defined below, with the other RA2 helpers
void Process_All_Task_Forces(void); // RA2: 0x6C0220, calls TeamTypeClass::Process_Task_Force (0x6C0180, YR name) for every team type
 // RA2: the multiplayer score screen (YR: MPScoreScreen, ctor 0x5AAA20); 0x5AAA40 shows it (placeholder name)
void Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0 (init.cpp declares it the same way)
class UnknownScore_0069A640 { public: UnknownScore_0069A640(void); void Presentation_0069A650(void); }; // RA2: the single player score screen (ctor 0x69A640, Presentation 0x69A650; names_proposed: UnknownScore_0069A640::Presentation_0069A650)
void Clear_Radar_Events(void); // RA2: 0x0063C640, clears the radar events (revent.cpp)
void Show_Credits(void); // RA2: 0x4B37C0 (init.cpp declares it the same way; TibSun: Show_Who_Was_Responsible)
void __fastcall Free_Interface_Art(void); // RA2: 0x006F30F0, frees the interface art (txtlabel2.cpp defines it, saveload.cpp declares it the same way)
void Reset_Radar_Event_Cells(void); // RA2: 0x63C790, clears the radar event cell table (YR name)

namespace Game { extern HWND hWnd; bool __cdecl CallBack(void); }
namespace CopyProtection { bool CheckProtectedData(void); }


CDTimerClass<SystemTimerClass> ScenUnusedTimer;

static void Remove_AI_Players(void);
static void Create_Units(bool official);
static Cell const Clip_Scatter(Cell const & cell, int maxdist);
static Cell const Clip_Move(Cell const & cell, FacingType facing, int dist);

static void Multiplayer_Last_Minute_Fixups(bool official = true);
static char const * Pick_Load_Background_Name(Point2D & text_pos);


/***********************************************************************************************
 * ScenarioClass::ScenarioClass -- Constructor for the scenario control object.                *
 *                                                                                             *
 *    This constructs the default scenario control object. Normally, all the default values    *
 *    are meaningless since the act of starting a scenario will fill in all of the values with *
 *    settings retrieved from the scenario control file.                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ScenarioClass::ScenarioClass(void)
{
	Reset();
	IsReadingScenario = false;
	IsRandom = false;
	Special.Init();

	Scenario = 1;
	Campaign = CAMPAIGN_NONE;
	NextScenarioName[0] = '\0';
	AltNextScenarioName[0] = '\0';

	int i;
	for (i = 0; i < ARRAY_SIZE(Waypoint); i++) {
		Waypoint[i] = CELL_NONE;
	}
	wcscpy(Description, L""); // RA2: wide
	strcpy(ScenarioName, "");
	strcpy(BriefingCSF, ""); // RA2: the briefing's CSF label (+0x11D8) is cleared here, between the file name and the UI name
	strcpy(UIName, ""); // RA2: the UI name (+0x99A) is cleared before the briefing text
	wcscpy(BriefingText, L""); // RA2: wide
	for (i = 0; i < ARRAY_SIZE(GlobalFlags); i++) {
		GlobalFlags[i].VariableName[0] = '\0';
		Set_Global_To(i, false);
	}
	for (i = 0; i < ARRAY_SIZE(LocalFlags); i++) {
		LocalFlags[i].VariableName[0] = '\0';
		Set_Local_To(i, false);
	}
	memset(Views, '\0', sizeof(Views));
}


/// <summary>
/// Constructs a scenario object without initializing it.
/// This constructor is used by the load process, which is about to overwrite the object
/// with the data it reads from the save game stream.
/// </summary>
ScenarioClass::ScenarioClass(NoInitClass const & x) :
	ElapsedTimer(x), // RA2: the elapsed timer is left alone (no system timer read for it)
	MissionTimer(x),
	MissionTimerText(x), // RA2: the saved label is kept (no clearing)
	ShroudTimer(x),
	FogTimer(x),
	IceGrowthTimer(x),
	VeinGrowthTimer(x),
	AmbientChangeTimer(x),
	FadeTimer(x)
{
	if (MissionTimerText[0] != '\0') { // RA2: the timer's label is loaded again (the string table is not saved)
		MissionTimerTextCSF = StringTable::LoadString(MissionTimerText, NULL, "D:\\Ra2\\Scenario.CPP", 340); // RA2: as above
	} else {
		MissionTimerTextCSF = L""; // RA2: as above
	}
}


/// <summary>
/// Resets the scenario data back to its defaults.
/// This routine runs before a scenario is read, so that any setting the mission neglects
/// to mention falls back on a sensible value rather than a leftover from the last game.
/// </summary>
void ScenarioClass::Reset(void)
{
	Home = WAYPT_HOME;
	AltHome = WAYPT_HOME;
	UniqueID = 1000 * 1000;
	Difficulty = DIFF_NORMAL;
	CDifficulty = DIFF_NORMAL;
	ElapsedTimer = 0;
	ElapsedTimer.Stop();
	PauseTimer = 0; // RA2: the pause timer (+0x620) and its count (+0x62C) are reset here
	PauseTimer.Stop(); // RA2: as above
	Unknown_62C = 0; // RA2: as above
	IsGamePaused = false; // RA2: as above
	MissionTimer = 0;
	MissionTimer.Stop();
	MissionTimerTextCSF = L""; // RA2: the timer's label starts empty
	MissionTimerText = TStringID<31>(""); // RA2: as above
	ShroudTimer = 0;
	FogTimer = 0;
	IceGrowthTimer = 0;
	VeinGrowthTimer = 0;
	AmbientChangeTimer = 0;
	Theater = THEATER_NONE;
	IntroMovie = VQ_NONE;
	BriefMovie = VQ_NONE;
	WinMovie = VQ_NONE;
	LoseMovie = VQ_NONE;
	ActionMovie = VQ_NONE;
	PostScoreMovie = VQ_NONE;
	PreMapSelectMovie = VQ_NONE;
	TransitTheme = THEME_NONE;
	PlayerHouse = HOUSE_FIRST;
	CarryOverPercent = 0;
	CarryOverCap = 0;
	Percent = 0;
	BridgeCount = 0;
	IsFreeRadar = false;
	IsTrainCargo = false;
	IsTibGrowth = true;
	IsVeinGrowth = true;
	IsIceGrowth = true;
	IsBridgeChanged = false;
	IsGlobalChanged = false;
	IsAmbientLightChanged = false;
	IsEndOfGame = false;
	IsInheritTimer = false;
	IsSkipScore = false;
	IsOneTimeOnly = false;
	IsNoMapSel = false;
	IsTruckCrate = false;
	IsMoneyTiberium = false;
	IsIgnoreGlobalAITriggers = false;
	IsGDI = true;
	IsMultiplayerOnly = false;
	IsCrateBeenPickedUp = false;
	FadeTimer = 0;
	StartingDropships = 0;
	AmbientLight = 100;
	CurrentAmbientLight = 100;
	DesiredAmbientLight = 100;
	RedTint = 100;
	GreenTint = 100;
	BlueTint = 100;
	GroundLight = 100; // RA2: the ground and level light are plain numbers here
	LevelLight = 16; // RA2: as above
	IonAmbientLight = 100;
	IonRedTint = 0;
	IonGreenTint = 0;
	IonBlueTint = 0;
	IonGroundLight = 100; // RA2: as above
	IonLevelLight = 16; // RA2: as above
	NukeAmbient = 200; // RA2: the nuclear flash light set (not in TibSun)
	NukeRedTint = 175; // RA2: as above
	NukeGreenTint = 150; // RA2: as above
	NukeBlueTint = 125; // RA2: as above
	NukeGroundLight = 100; // RA2: as above
	NukeLevelLight = 100; // RA2: as above
	NukeAmbientChangeRate = 1; // RA2: as above
	InitTime = 10000;
	Stage = 0;
	IsInputLocked = false;
	IsTiberiumDeathToVisceroid = true;
	ParTimeEasy = 3600; // RA2: the par times and the score and load screen texts are reset here
	ParTimeMedium = 3600; // RA2: as above
	ParTimeDifficult = 3600; // RA2: as above
	UnderParTitle[0] = '\0'; // RA2: as above
	UnderParMessage[0] = '\0'; // RA2: as above
	OverParTitle[0] = '\0'; // RA2: as above
	OverParMessage[0] = '\0'; // RA2: as above
	LSLoadMessage[0] = '\0'; // RA2: as above
	LSBrief[0] = '\0'; // RA2: as above
	LS640BriefLocX = 0; // RA2: as above
	LS640BriefLocY = 0; // RA2: as above
	LS800BriefLocX = 0; // RA2: as above
	LS800BriefLocY = 0; // RA2: as above
	LS640BkgdName[0] = '\0'; // RA2: as above
	LS800BkgdName[0] = '\0'; // RA2: as above

	AllowableUnits.Clear();
	AllowableUnitMaximums.Clear();
	AllowableUnitCounts.Clear();

	if (Session.Type == GAME_SKIRMISH) { // RA2: a skirmish game is named from the string table
		wcscpy(Scen->Description, StringTable::LoadString("GUI:SkirmishGame", NULL, "D:\\Ra2\\Scenario.CPP", 486)); // RA2: as above
	}
} // RA2: no SpeechSide, RequiredAddOn (not in RA2's layout)


/***********************************************************************************************
 * Start_Scenario -- Starts the scenario.                                                      *
 *                                                                                             *
 *    This routine will start the scenario. In addition to loading the scenario data, it will  *
 *    play the briefing and action movies.                                                     *
 *                                                                                             *
 * INPUT:   root     -- Pointer to the filename root for this scenario (e.g., "SCG01EA").      *
 *                                                                                             *
 *          briefing -- Should the briefing be played? Normally this is true except when the   *
 *                      scenario is restarting.                                                *
 *                                                                                             *
 * OUTPUT:  Was the scenario started without error?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Start_Scenario(char const * name, bool briefing, CampaignType campaign)
{
	if ((name == NULL || strlen(name) == 0) && campaign != CAMPAIGN_NONE) {
		if (Debug_ForceScenario == true) {
			name = Debug_ScenarioName;
		} else {
			name = Campaigns[campaign]->ScenarioName;
		}
	}

	Scen->Campaign = campaign;

	DebugString("\n----- Starting scnenario: %s -----\n", name);
	DebugString("Player Count: %d\n", Session.Players.Count());

	strcpy(Scen->ScenarioName, name);
	strupr(Scen->ScenarioName);

	CD::Set_Required_Disk(DISK_ANY); // RA2: no Theme.Stop() here and no _DEBUG swap override test

	if (Session.Type == GAME_NORMAL) { // RA2: as above
		if (Scen->Campaign != CAMPAIGN_NONE) { // RA2: as above
			CD::Set_Required_Disk((DiskID)Campaigns[Scen->Campaign]->CDNumber); // RA2: as above
		}
	} else {
		if (Session.Options.ScenarioIndex != -1) { // RA2: as above
			MultiMission * multi = Session.Scenarios[Session.Options.ScenarioIndex]; // RA2: as above
			if (!multi->Is_On_CD(CD::Get_Current_Disk(60))) { // RA2: the multiplayer map names the disc it is on
				CD::Set_Required_Disk(multi->On_Which_CD()); // RA2: as above
			}
		}
	}

	Session.Suspended++; // RA2: as above

	if (!CD::Force_Available()) { // RA2: as above
		return(false); // RA2: the suspend count stays raised when the disc does not come
	}
	Session.Suspended--; // RA2: as above

	Hide_Mouse(); // RA2: the mouse is hidden while the movies and the scenario load

	{ // RA2: the scenario's own intro (or briefing) movie plays before it is read
		CCINIClass ini; // RA2: as above
		CCFileClass file(name); // RA2: as above
		if (ini.Load(file, true, false)) { // RA2: as above
			VQType movie = ini.Get_VQType("Basic", "Intro", VQ_NONE); // RA2: as above
			if (movie == VQ_NONE) { // RA2: as above
				movie = ini.Get_VQType("Basic", "Brief", movie); // RA2: as above
			}
			if (movie != VQ_NONE) { // RA2: as above
				Theme.Stop(false); // RA2: as above
				Play_Movie(movie, THEME_NONE, true, true, true); // RA2: as above
			}
		}
	}

	DebugString("Reading scenario: %s\n", name);

	Theme.Play_Song(Theme.From_Name("LOADING")); // RA2: the loading score plays while the scenario is read

	if (!Read_Scenario(name)) {
		Show_Mouse(); // RA2: the mouse comes back when the scenario cannot be read
		return(false);
	}

	/*
	**	If there's no briefing movie, restate the mission at the beginning.
	*/
	char buffer[28]; // RA2: as above

	if (Scen->BriefMovie != VQ_NONE) {
		wsprintf(buffer, "%s.VQA", Movies[Scen->BriefMovie]);
	}

	if (Scen->StartingDropships > 0) {
		Dropship_Screen();
	}

	if (briefing) {
		Play_Movie(Scen->ActionMovie, THEME_NONE, true, true, true); // RA2: the action movie only, with no score
	}

	Free_Interface_Art(); // RA2: the interface art is freed once the scenario is read (0x6F30F0)

	/*
	**	Set the options values, since the palette has been initialized by Read_Scenario
	*/
	Options.Set();

	HiddenSurface->Fill(0);
	Update_Visible_Surface(true, HiddenSurface, NULL); // RA2: as above

	Toggle_Display_Mode(true);
	Scen->ElapsedTimer.Start();

	if (Scen->TransitTheme != THEME_NONE) { // RA2: the scenario's score is queued, or the music stops
		Theme.Queue_Song(Scen->TransitTheme); // RA2: as above
	} else {
		Theme.Stop(true); // RA2: as above
	}

	ScenarioActive = true;
	TacticalActive = true;

	return(true);
}


/// <summary>
/// Pauses the scenario in progress.
/// The mission clock is stopped for solo play, any in-game movie is suspended, and the
/// tactical map is redrawn with the plain cursor so that the game is left looking tidy
/// while the player is away from it.
/// </summary>
void Pause_Scenario(void)
{
	Audio_Pause_All(); // RA2: the sound stops with the game
	SavedSpeechVolume = AudioLevelGetVal(&AudioGroupAttribs1->VolumeLevel); // RA2: EVA's volume is kept and set to AUDIO_VOLUME_MAX
	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, AUDIO_VOLUME_MAX); // RA2: as above

	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		DebugString("Paused ElapsedTimer = %ld\n", (int)Scen->ElapsedTimer); // RA2: the timer is passed as its value
		Scen->ElapsedTimer.Stop();
	}

	Pause_Ingame_Movie(true);

	if (ToolTips != NULL) {
		ToolTips->Activate(false);
	}

	Map.Abort_Drag_Select();

	Map.Override_Mouse_Shape(MOUSE_NORMAL, false);

	Hide_Mouse();
	Map.Flag_To_Redraw(GS_REDRAW_ALL);
	Map.Render();
	Show_Mouse();
}


/// <summary>
/// Resumes a scenario that was paused.
/// The mission clock starts again, the mouse shape is put back the way the player left it,
/// and any in-game movie picks up where it stopped.
/// </summary>
void Resume_Scenario(void)
{
	Map.Revert_Mouse_Shape();

	Scen->ElapsedTimer.Start();
	DebugString("Resume ElapsedTimer = %ld\n", (int)Scen->ElapsedTimer); // RA2: as above

	if (ToolTips != NULL) {
		ToolTips->Activate(Options.ToolTips);
	}

	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, SavedSpeechVolume); // RA2: EVA's volume comes back
	SavedSpeechVolume = AUDIO_VOLUME_MAX; // RA2: as above

	Pause_Ingame_Movie(false);
	Audio_Resume_All(); // RA2: the sound goes on again
}








/// <summary>
/// RA2: holds the scenario (for seconds, when given): the sound and the mission clock stop and the input is
/// locked, counted so that each Release_Scenario undoes one hold.
/// </summary>
void Hold_Scenario(int seconds) // RA2: at 0x65C1F0
{
	if (seconds) {
		Scen->PauseTimer = seconds * 60;
		if (Scen->IsGamePaused) {
			return;
		}
		Scen->IsGamePaused = true;
	}

	if (Scen->Unknown_62C == 0) {
		Audio_Pause_All();

		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Scen->ElapsedTimer.Stop();
		}

		InputLockedBeforeHold = Scen->IsInputLocked;
		if (!InputLockedBeforeHold) {
			Map.Abort_Drag_Select();
			if (!Get_Mouse_State()) {
				Hide_Mouse();
			}
			Scen->IsInputLocked = true;
			IgnoreInput = true;
			Keyboard->Clear();
			UnlockInputFrame = 0;
		}
	}
	Scen->Unknown_62C++;
}


/// <summary>
/// RA2: undoes one Hold_Scenario; the last one gives back the sound, the mission clock and the input (unless the
/// input was locked before the hold).
/// </summary>
void Release_Scenario(void) // RA2: at 0x65C310
{
	if (Scen->Unknown_62C == 0) {
		return;
	}
	Scen->Unknown_62C--;
	if (Scen->Unknown_62C != 0) {
		return;
	}

	if (!InputLockedBeforeHold) {
		if (Scen->IsInputLocked == true) {
			while (Get_Mouse_State()) {
				Show_Mouse();
			}
		}
		Scen->IsInputLocked = false;
		IgnoreInput = false;
		Keyboard->Clear();
		UnlockInputFrame = 0;
	}

	Scen->ElapsedTimer.Start();
	Audio_Resume_All();
}


/// <summary>
/// Takes control of the game away from the player.
/// This routine is used while a scripted sequence plays out, so that the player cannot
/// interfere with it. The mouse is hidden and all input is discarded until
/// Unlock_Scenario_Input is called.
/// </summary>
void Lock_Scenario_Input(void)
{
	Map.Abort_Drag_Select();

	if (!Get_Mouse_State()) { // RA2: as YR
		Hide_Mouse();
	}

	Scen->IsInputLocked = true;
	IgnoreInput = true;
	Keyboard->Clear(); // RA2
	UnlockInputFrame = 0; // RA2
}


/// <summary>
/// Returns control of the game to the player.
/// This routine undoes Lock_Scenario_Input. The mouse comes back and anything the player
/// typed while input was being ignored is thrown away.
/// </summary>
void Unlock_Scenario_Input(void)
{
	if (Scen->IsInputLocked == true) { // RA2: as YR
		while (Get_Mouse_State()) { // RA2: as YR
			Show_Mouse(); // RA2: Unlock_Scenario_Input at 0x65C420 matches only so
		}
	}

	Scen->IsInputLocked = false;
	IgnoreInput = false;
	Keyboard->Clear(); // RA2
	UnlockInputFrame = 0; // RA2
}


/// <summary>
/// Switches the display between the menu and the in-game resolutions.
/// The menus and movies are authored for a low resolution while the game itself runs at
/// whatever the player selected, so the screen mode is flipped as play starts and stops.
/// </summary>
/// <param name="ingame">Should the display switch to the in-game resolution?</param>
void Toggle_Display_Mode(bool ingame)
{
	if (!Debug_IngameModeChange) {
		return;
	}

	if (ingame) {
		if (VisibleRect.Width != Options.ScreenWidth || VisibleRect.Height != Options.ScreenHeight) {
			DebugString("Toggle display mode to %d X %d\n", Options.ScreenWidth, Options.ScreenHeight);
			Change_Display_Mode(Options.ScreenWidth, Options.ScreenHeight);
		}
	} else {
		if (VisibleRect.Width != 640 && VisibleRect.Height > 480) {
			DebugString("Toggling display mode back to 640 X 480 / 400\n");

			if (Change_Display_Mode(640, 400) == false) {
				Change_Display_Mode(640, 480);
			}
		}
	}
}


/// <summary>
/// Waits for the other players to finish loading the scenario.
/// The network is kept alive and the progress display updated until every machine reports
/// that it is ready. A player who stops making progress is dropped rather than being
/// allowed to hold the game up forever. Solo and skirmish games have nobody to wait for.
/// </summary>
/// <returns>bool; Did everyone make it into the game?</returns>
bool Wait_For_Players_To_Load(void)
{
	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		return(true);
	}

	CDTimerClass<SystemTimerClass> wait_timeout;
	CDTimerClass<SystemTimerClass> timer = TIMER_SECOND * 5;

	wait_timeout = 60 * TIMER_SECOND;
	double last_progress = Progress.Get_Current_Progress();

	for (;;) {
		Keyboard->Check();
		Call_Back();

		if (Session.Type == GAME_MODEM && SerialPort) {
			int modemstatus = SerialPort->Get_Modem_Status();
			if ((modemstatus & WinModemClass::CD_SET) == 0) {
				DebugString("Carrier Detect is false. Aborting load wait\n");
				return(false);
			}
		}

		if (wait_timeout <= 0) {
			for (int i = 1, slot = 1; i < Session.Players.Count(); i++, slot++) { // RA2: slot and i step together except after a drop
				if (Progress.Get_Current_Progress(slot) < 0.999) { // RA2: below 0.999 instead of not exactly 1.0
					wchar_t const * name = Session.Players[i]->Name; // RA2: read once for both names
					DebugString("Player %s failed to make it into the game. Dropping %s\n", Wide_To_Narrow(name), Wide_To_Narrow(name)); // RA2: the wide names
					int id = Session.Players[i]->Player.ID; // RA2: the connection's id is kept for the master test
					Destroy_Connection(id, 1); // RA2: as above
					if (Session.Am_I_Master(Houses[id])) { // RA2: a dropped master is forgotten (0x66F150)
						Session.MasterPlayerID = -1; // RA2: as above
						memset(Session.MasterPlayerName, 0, sizeof(Session.MasterPlayerName)); // RA2: as above
					}
					i--;
				}
			}
			return(false);
		}

		double current_progress = Progress.Get_Current_Progress();
		if (current_progress >= 0.9995) {
			Session.Update_Progress(100 + (Scen->IsRandom ? 100 : 0));
			break;
		}

		if (current_progress != last_progress) {
			wait_timeout = 60 * TIMER_SECOND;
			last_progress = current_progress;
		}

		if (timer <= 0) {
			timer = TIMER_SECOND;
			Session.Update_Progress(100 + (Scen->IsRandom ? 100 : 0));
		}
	}
	return(true);
}



/// <summary>
/// Fetches the artwork to display while a scenario loads.
/// The picture is chosen to suit the player's side and the current screen resolution, and
/// one of the pair available is taken at random so that the loading screen is not always
/// the same one.
/// </summary>
/// <param name="pos">Filled in with the position the loading text belongs at.</param>
/// <returns>Returns with the filename of the backdrop picture to display.</returns>
char const * Pick_Load_Background_Name(Point2D & pos)
{
	static char _backgrounds[12][13] = {
		"LOAD400C.PCX",
		"LOAD400D.PCX",
		"LOAD400A.PCX",
		"LOAD400B.PCX",
		"LOAD480C.PCX",
		"LOAD480D.PCX",
		"LOAD480A.PCX",
		"LOAD480B.PCX",
		"LOAD600C.PCX",
		"LOAD600D.PCX",
		"LOAD600A.PCX",
		"LOAD600B.PCX"
	};

	int player = 0;

	if (Session.Type == GAME_NORMAL) {
		if (Scen->Campaign != CAMPAIGN_NONE) {
			CampaignClass * campaign = Campaigns[Scen->Campaign];
			player = campaign->CDNumber;

			if (player > 1) {
				player = 0;
				if (strstr(campaign->ScenarioName, "GDI") == 0) {
					player = 1;
				}
			}
		}
	} else {
		player = Session.Players[player]->Player.House;
	}

	int choice = (player << 1) + Random_Pick(0, 1);

	if (VisibleRect.Width == 640) {
		if (VisibleRect.Height == 480) {
			if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
				pos = Point2D(440, 189);
			} else {
				pos = Point2D(570, 180);
			}
			if (player == 1) {
				pos += Point2D(-4, 10);
			} else {
				pos += Point2D(-4, 3);
			}

			return(_backgrounds[choice + 4]);
		}
	}

	if (VisibleRect.Width >= 800 && VisibleRect.Height >= 600) {
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			pos = Point2D(550, 236);
		} else {
			pos = Point2D(715, 230);
		}

		int x_adjust = (VisibleRect.Width - 800) / 2;
		int y_adjust = (VisibleRect.Height- 600) / 2;

		pos += Point2D(x_adjust, y_adjust);

		if (player == 1) {
			pos += Point2D(-4, 10);
		} else {
			pos += Point2D(-4, 3);
		}

		return(_backgrounds[choice + 8]);
	}

	if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
		pos = Point2D(440, 158);
	} else {
		pos = Point2D(570, 155);
	}

	if (player == 1) {
		pos += Point2D(-4, 10);
	} else {
		pos += Point2D(-4, 3);
	}

	return(_backgrounds[choice]);
}


/***********************************************************************************************
 * Read_Scenario -- Reads a scenario from disk.                                                *
 *                                                                                             *
 *    This will read a scenario from disk. Use this to begin a scenario.                       *
 *    It doesn't perform any rendering, it merely sets up the system                           *
 *    with the proper data. Setting of the right game state will start                         *
 *    the scenario running.                                                                    *
 *                                                                                             *
 * INPUT:   root     -- Scenario root filename                                                 *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   You must clear out the system variables before calling                          *
 *             this function. Use the Clear_Scenario() function.                               *
 *               It is assumed that Scenario is set to the current scenario number.            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1991     : Created.                                                                 *
 *   02/03/1992 JLB : Uses house identification.                                               *
 *=============================================================================================*/
bool Read_Scenario(char const * fname)
{
	bool read_ok = true;
	char name[_MAX_PATH];

	strcpy(name, fname);

	Frame = 0;

	Scen->IsReadingScenario = true;
	ScenarioInit++;

	char * ext = name + strlen(name) - 4;

	if (stricmp((ext), ".SED") == 0) {
		Scen->IsRandom = true;
		DebugString("Scen->IsRandom = true\n");
	} else {
		Scen->IsRandom = false;
		DebugString("Scen->IsRandom = false\n");
	}

	LoadProgressManager * loader = NULL; // RA2: the load screen's manager (0x535910)

	if (!Debug_Map) {

		int players = 1;

		if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
			players = Session.Players.Count();
		}

		Progress.Initialize(100.0, players, NULL); // RA2: as above

		wchar_t const * prog_msg = NULL; // RA2: the Internet tournament game id is shown on the load screen
		wchar_t prog_msg_buffer[130]; // RA2: as above

		if (Session.Type == GAME_INTERNET && WestwoodOnline_Tournament) {
			swprintf(prog_msg_buffer, StringTable::LoadString("TXT_GAME_ID", NULL, "D:\\Ra2\\Scenario.CPP", 1303), GameID); // RA2: as above
			prog_msg = prog_msg_buffer; // RA2: as above
		}

		loader = LoadProgressManager::Create(); // RA2: the load screen is set up here (0x65C903)
		loader->Compute_Rects(); // RA2: as above
		Progress.Unknown_Virtual_0((int)loader); // RA2: the manager is told of the progress (0x6203E0)

		int side = Player_Side(); // RA2: as above
		Scen->IsGDI = (side == 0); // RA2: as above
		Progress.Set_Side(side); // RA2: as above

		LoadProgressManager::Create()->Load_Mixes(); // RA2: as above

		if (Session.Type == GAME_NORMAL) { // RA2: a campaign shows the splash bar, a multiplayer game the bars of its players
			Progress.Set_Graphic_Data("SPLDBR.SHP", NULL, Get_Splash_Bar_Drawer()); // RA2: as above
		} else {
			Progress.Set_Graphic_Data("PROGBARM.SHP", NULL, NULL); // RA2: as above
		}
		bool campaign = (Session.Type == GAME_NORMAL); // RA2: as above
		Progress.Set_Layout(loader->Text_Origin(), prog_msg, !campaign, !campaign); // RA2: as above
		Progress.Set_Row_Width(loader->Get_Row_Width()); // RA2: as above

		if (PacketTransport != NULL && Session.Type == GAME_INTERNET && Session.Players.Count() > 1) {
			DebugString("Setting addresses for UDP broadcast\n");
			PacketTransport->Clear_Broadcast_Addresses();

			char address[32] = {"xxx.xxx.xxx.xxx"};
			NetNumType net;
			NetNodeType node;

			for (int p = 1; p < Session.Players.Count(); p++) {
				Session.Players[p]->Address.Get_Address(net, node);
				sprintf(address, "%d.%d.%d.%d", node[0], node[1], node[2], node[3]);
				DebugString("Adding broadcast address %s\n", address);
				PacketTransport->Set_Broadcast_Address(address, Session.PlayerPorts[p]); // RA2: the address and its port
			}
		}
	}

	if (Scen->IsRandom) {
		read_ok = RandomMapGen.SeedData.Load(name);

		if (read_ok) {
			RandomMapGen.Generate_Random_Map(false, NULL);
			Multiplayer_Last_Minute_Fixups(true); // RA2: as an official map
		}
		strcpy(Scen->ScenarioName, name);
	} else {
		read_ok = Read_Scenario_INI(name);
	}

	if (!Rule->Shroud) { // RA2: a scenario without shroud is revealed (0x65CB9B)
		Map.Reveal_The_Map(); // RA2: as above
	}

	if (!read_ok) {
		DebugString("Error - Unable to read scenario: %s\n", name);
		Set_Palette(GamePalette, FADE_PALETTE_FAST, Call_Back);
		WWMessageBox(L" ").Process(StringTable::LoadString("TXT_UNABLE_READ_SCENARIO", NULL, "D:\\Ra2\\Scenario.CPP", 1394), StringTable::LoadString("TXT_OK", NULL, "D:\\Ra2\\Scenario.CPP", 1394), NULL, NULL, false); // RA2: the texts are loaded by name

		ScenarioInit--;
		Scen->IsReadingScenario = false;
		if (loader != NULL) { // RA2: the manager is given up
			Progress.Unknown_Virtual_0(0); // RA2: as above
			delete loader; // RA2: as above
		}
		return(false);
	}

	/*
	**	For multiplayer games, initialize the inter-player message system.
	**	Do this after loading the scenario, so the map's upper-left corner is
	**	properly set.
	*/
	Session.Messages.Init(
		TacticalRect.X + 3, TacticalRect.Y,	// x,y for messages // RA2: the message list starts 3 pixels in and is 6 narrower (0x65CC5D, 0x65CC77)
		6, 										// max # msgs
		MAX_MESSAGE_LENGTH - 14,			// max msg length
		7 * 2,									// font height in pixels
		-1, -1, 									// x,y for edit line (appears above msgs)
		0,//BG		1,							// enable edit overflow
		20,										// min,
		MAX_MESSAGE_LENGTH - 14,			// max for trimming overflow
		TacticalRect.Width - 6);			// Width in pixels of buffer // RA2: the message list starts 3 pixels in and is 6 narrower (0x65CC5D, 0x65CC77)

	Fill_In_Data();

	if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) { // RA2: the load is nearly done
		Session.Update_Progress(99); // RA2: as above
		if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) { // RA2: a game played through the packet router waits for it to shut down (0x65CCAD)
			OwnerTalk.Wait_For_Router_Shutdown(15000, true); // RA2: as above
		}
	}

	Session.Update_Progress(100 + (Scen->IsRandom ? 100 : 0));

	Session.SawGameCompletion = false;
	Session.OutOfSync = false;

	bool all_loaded = Wait_For_Players_To_Load();

	if (Session.Type == GAME_INTERNET) {
		Session.Init_Fixed_Alliances();
	}

	ScenarioInit--;

	if (!Debug_Map) {
		if (all_loaded) {

			Call_Back();
			double progress = Progress.Get_Current_Progress();

			while (progress < 1.0) {
				for (int i = 0; i < Session.Players.Count(); i++) {
					Progress.Set_Progress_Percent(i, 100);
				}
				progress = Progress.Get_Current_Progress();
			}
		}
	}

	Scen->IsReadingScenario = false;

	Progress.End();

	if (loader != NULL) { // RA2: the manager is released
		Progress.Unknown_Virtual_0(0); // RA2: as above
		loader->Release(); // RA2: as above
	}

	return(true);
}



/***********************************************************************************************
 * Fill_In_Data -- Recreate all data that is not loaded with scenario.                         *
 *                                                                                             *
 *    This routine is called after the INI file for the scenario has been processed. It will   *
 *    infer the game state from the scenario INI data.                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 3 bytes differ (adopted text)
void Fill_In_Data(void)
{
	/*
	**	The basic scenario data load does not contain the full set of
	**	game data. We now must fill in the missing pieces.
	*/
	ScenarioInit++;
	int index;
	for (index = 0; index < Buildings.Count(); index++) {
		Buildings[index]->Update_Buildables();
	}

	Map.Flag_To_Redraw(GS_REDRAW_ALL);

	/*
	**	Since the sidebar starts up activated, adjust the home start position so that
	**	the right edge of the map will still be visible.
	*/
	if (!Debug_Map) {
		Map.TabClass::Activate(1); // RA2: the tab bar's own Activate (0x6A0DA0), not the sidebar's

		if (Session.Type == GAME_NORMAL) {
			bool flag = Environment.Globals[0];
			Cell view = Scen->Get_Waypoint_Cell(flag ? Scen->AltHome : Scen->Home);

			Scen->Views[0] = Scen->Views[1] = Scen->Views[2] = Scen->Views[3] = view;
			Coord coord = Coord(view);
			coord.Z = Map.Get_Height_GL(coord);
			Map.Set_Tactical_Position(coord);
		}
	}

	/*
	**	Handle any data resetting that can be safely inferred from the actual
	**	data that has been loaded.
	*/
	/*
	**	Distribute the trigger pointers to the appropriate working lists.
	*/
	for (index = 0; index < TagTypes.Count(); index++) {
		TagTypeClass * tp = TagTypes[index];

		if (tp->Attaches_To() & ATTACH_MAP) {
			MapTags.Add(Find_Or_Make(tp));
		}

		if (tp->Attaches_To() & ATTACH_GENERAL) {
			LogicTags.Add(Find_Or_Make(tp));
		}

		if (tp->Attaches_To() & ATTACH_HOUSE) {
			TagClass * tt = Find_Or_Make(tp);
			HouseClass * tagowner = House_From_HousesType(tt->Class->FirstTrigger->House->House); // RA2: the owner may be gone
			if (tagowner != NULL) { // RA2: as above
				tagowner->HouseTags.Add(tt); // RA2: as above
			}
		}
	}

	ScenarioInit--;

#ifdef DO_CARRYOVERS
	/*
	**	If inheriting from a previous scenario was indicated, then create the carry over
	**	objects at this time.
	*/
	if (Scen->IsToInherit) {
		CarryoverClass * cptr = Carryover;

		while (cptr != NULL) {
			cptr->Create();
			cptr = (CarryoverClass *)cptr->Get_Next();
		}
	}
#endif

	/*
	**	The "allow win" action is a special case that is handled here. The total number
	**	of triggers that have this action must be recorded.
	*/
	for (index = 0; index < TagTypes.Count(); index++) {
		TagTypeClass * tp = TagTypes[index];
		if (tp->Is_Allow_Win()) {
			HouseClass * blocked = House_From_HousesType((HousesType)tp->FirstTrigger->House->HeapID); // RA2: found through House_From_HousesType (0x4ED760)
			blocked->Blockage++; // RA2: as above
		}
	}

	/*
	**	Move available money to silos, if the scenario flag so indicates.
	*/
	if (Scen->IsMoneyTiberium) {
		for (int house = 0; house < Houses.Count(); house++) {
			HouseClass * hptr = Houses[house];

			if (hptr != NULL) {
				int money = hptr->Available_Money();
				TiberiumClass const * tib = Tiberiums[0];

				while (money > tib->CreditValue && hptr->Available_Storage() > 0) {
					hptr->Give_Tiberium(1, 0); // RA2: as above
					money -= tib->CreditValue;
				}
			}
		}
	}

	Map.Reveal_Technos(true, false); // RA2: 0x49DE00
	WaveClass::Init_Statics();

	/*
	**	Reset the movement zones according to the terrain passability.
	*/
	Map.Reset_Iterator();
	CellClass * cellptr = Map.Iterate();

	while (cellptr) {
		cellptr->Recalc_Attributes();
		cellptr = Map.Iterate();
	}

	Map.Compute_Zone_Connections();
	Map.Zone_Reset();
	Map.Reset_All_Subzones();
	Map.Set_WasUnderBridge_Flags();

	if (GasSystem == NULL) {
		GasSystem = new ParticleSystemClass(ParticleSystemTypes[ParticleSystemTypeClass::From_Name("GasCloudSys")], Cell(10, 10));
	}

	if (Rule->OreTwinkle != NULL) { // RA2: ore cells twinkle at random when the rules name an animation for it
		Map.Reset_Iterator(); // RA2: as above
		CellClass * orecell = Map.Iterate(); // RA2: as above
		while (orecell != NULL) { // RA2: as above
			if (orecell->Tiberium_Value() && Random_Pick(0, Rule->OreTwinkleChance - 1) == 0) { // RA2: as above
				new AnimClass(Rule->OreTwinkle, orecell->Center_Coord(), 0, 1); // RA2: as above
			}
			orecell = Map.Iterate(); // RA2: as above
		}
	}

	Set_Frame_Rate_Limit(Rule->DetailMinFrameRateNormal); // RA2: the detail limits of the rules are applied (0x53D820)
	Set_Buffer_Zone_Width(Rule->DetailBufferZoneWidth); // RA2: 0x53D830
}
#endif



/***********************************************************************************************
 * Post_Load_Game -- Fill in an inferred data from the game state.                             *
 *                                                                                             *
 *    This routine is typically called after a game has been loaded. Some working data lists   *
 *    can be rebuild from the game state. This working data is rebuilt rather than being       *
 *    stored with the game data file.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Although it is safe to call this routine whenever, it is only needed after a    *
 *             game load.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Post_Load_Game(void)
{
	BuildingTypeClass::Post_Load_Game();
	TiberiumClass::Post_Load_Game();
	VeinholeMonsterClass::Init(Scen->Theater);

	Map.Post_Load_Radar_Fixup();

	TechnoClass::ActionLineTimer.Stop();
	TechnoClass::ActionLineTimer.Start();

	Search.Update_Map_Dimensions(Map.PlayRect);

	IonStormClass::Apply_Secondary_Effect(); // RA2: takes no argument (0x520770)

	AnimClass::Post_Load_Game();

	Map.Flag_To_Redraw(GS_REDRAW_ALL);
	InvalidateRect(MainWindow, NULL, FALSE);

	Reset_Group_Select(); // RA2: 0x6F89E0, then 0x6F8A20 with NULL (clears both sidebar tab objects), then the tail call of 0x63C790
	Clear_Sidebar_Tab_Object(NULL); // RA2: as above
	Reset_Radar_Event_Cells(); // RA2: as above
}



/***********************************************************************************************
 * Clear_Scenario -- Clears all data in preparation for scenario load.                         *
 *                                                                                             *
 *    This routine will clear out all data specific to a scenario in                           *
 *    preparation for a subsequent scenario data load. This will free                          *
 *    all units, animations, and icon maps.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/22/1991     : Created.                                                                 *
 *   03/21/1992 JLB : Changed buffer allocations, so changes memset code.                      *
 *   07/13/1995 JLB : End count down moved here.                                               *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 3 bytes differ (adopted text)
void Clear_Scenario(void)
{
	int index;

	Scen->UniqueID = 1000 * 1000;
	Scen->RandomNumber.IsLocked = true; // RA2: the scenario's random numbers stay locked while it is cleared (0x65D386)
	SavedSpeechVolume = AUDIO_VOLUME_MAX; // RA2: 0x7F1DEC
	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, AUDIO_VOLUME_MAX); // RA2: EVA's volume goes back to full (0x407080)
	Sound_Clear_Static(); // RA2: 0x714410

	PlayerPtr = NULL;

	Scen->Reset();

	IonStormClass::Ion_Storm_End();

	for (index = AbstractTypes.Count() - 1; index >= 0; index--) {
		AbstractTypeClass * abstype = AbstractTypes[index];
		if (abstype->What_Am_I() == RTTI_ISOTILETYPE) {
			AbstractTypes.Delete_Index(index);
		}
	}
	Delete_All_Objects();
	for (index = 0; index < IsometricTileTypes.Count(); index++) {
		AbstractTypes.Add(IsometricTileTypes[index]);
	}

	Map.Free_Cells();

	Map.ZoneConnections.Clear();

	TagClass::Init();

	for (index = 0; index < ARRAY_SIZE(Scen->GlobalFlags); index++) {
		Scen->Set_Global_To(index, false);
	}

	delete TacticalMap;
	TacticalMap = new Tactical;
	TacticalMap->Reset_Dirty_Rectangles();

	LightSourceClass::Recalc = false;
	while (Objects.Count()) {
		if (Objects[0]->RTTI == RTTI_BULLET) {
			Objects[0]->Release();
		} else {
			delete Objects[0];
		}
	}

	LightSourceClass::Recalc = true;

	while (TagTypes.Count()) {
		delete TagTypes[0];
	}

	MapTags.Clear();
	LogicTags.Clear();


	Scen->ParTimeEasy = 3600; // RA2: one hour par time for every difficulty, no under/over par texts (0x65D627)
	Scen->ParTimeMedium = Scen->ParTimeEasy; // RA2: as above
	Scen->ParTimeDifficult = Scen->ParTimeEasy; // RA2: as above
	Scen->UnderParTitle[0] = '\0'; // RA2: as above
	Scen->UnderParMessage[0] = '\0'; // RA2: as above
	Scen->OverParTitle[0] = '\0'; // RA2: as above
	Scen->OverParMessage[0] = '\0'; // RA2: as above

	LightSourceClass::Reset();
	IonStormClass::Init();
	EMPulseClass::Reset();
	VeinholeMonsterClass::Reset();

	TiberiumClass::Deinit_Tiberium_Spread_System();
	TiberiumClass::Deinit_Tiberium_Growth_System();
	BombList.Clear(); // RA2: 0x436710
	Kamikazes.Clear(); // RA2: 0x5319E0
	AircraftTracker.Clear(); // RA2: 0x413540
	Planning_Reset_State(); // RA2: 0x613E50

	/*
	**	Call everyone's Init routine, except the Map's; for the Map, only call
	**	MapClass::Init, which clears the Cell array.  The Display::Init requires
	**	a Theater argument, and the theater is not known at this point; also, it
	**	would reload MixFiles, which isn't desired.  Display::Read_INI calls its
	**	own Init, which will Init the entire Map hierarchy.
	*/
	Map.PlayRect = Rect(0, 0, 0, 0);
	DebugString("Map.Init_Clear()\n");
	Map.Init_Clear();
	DebugString("Logic.Init()\n");
	Logic.Init();

	DebugString("CurrentObjects.Clear()\n");
	CurrentObject.Clear();

	DebugString("Scen->Clear_All_Waypoints()\n");
	Scen->Clear_All_Waypoints();

	DebugString("Init_Campaigns()\n");
	Init_Campaigns();

	if (GasSystem) {
		GasSystem->Clear_System();
		delete GasSystem;
		GasSystem = NULL;
	}

	Scen->RandomNumber.IsLocked = false; // RA2: the random numbers run again (0x65D7BA)
	Reset_Group_Select(); // RA2: 0x6F89E0
	Clear_Sidebar_Tab_Object(NULL); // RA2: 0x6F8A20
	Reset_Radar_Event_Cells(); // RA2: 0x63C790

	Scen->UniqueID = 1000 * 1000;
}
#endif



/***********************************************************************************************
 * Do_Win -- Display winning congratulations.                                                  *
 *                                                                                             *
 *    Perform the win the mission process. This will display any winning movies and the score  *
 *    screen. Followed by the map selection screen and then the load of the new scenario.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *   01/01/1995 JLB : Carries money forward into next scenario.                                *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 485 bytes differ (adopted text)
void Do_Win(void)
{
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session.Play) { // RA2: the queue is drained and the connections closed
		Wait_For_End_Of_Queue();
		Disconnect_Gracefully(); // RA2: 0x53F780
	}

	ScenarioActive = false;
	TacticalActive = false;

	Scen->ElapsedTimer.Stop();
	Stop_Ingame_Movie();

	MouseCursor->Erase_Mouse(HiddenSurface);
	MouseCursor->Release_Mouse(); // RA2: 0x65D870, virtual 0x14
	Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: second argument
	MouseCursor->Hide_Mouse(); // RA2: the cursor's own Hide_Mouse (virtual 0xC)

	AudioLevelMute(&AudioGroupAttribs1->VolumeLevel); // RA2: EVA's group is muted (0x406FA0) while the speech finishes
	AudioLevelWaitFade(&AudioGroupAttribs1->VolumeLevel); // RA2: 0x406FE0

	/*
	**	Announce win to player.
	*/
	int timeout = TickCount + 300; // RA2: 300 ticks

	while (Is_Speaking() && (timeout > TickCount)) {
		Call_Back();
	}

	Speech_Stop(true); // RA2: 0x715DE0
	Aux_Stream_Stop(); // RA2: 0x7160D0
	Audio_Stop_Events(); // RA2: 0x406DC0
	Sound_Clear_Static(); // RA2: 0x714410
	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, AUDIO_VOLUME_MAX); // RA2: the level is restored (0x407080)

	Toggle_Display_Mode(false);

	/*
	**	Stop here if this is a multiplayer game.
	*/
	bool next = false; // RA2: the game mode may ask for another round
	if (Session.Type != GAME_NORMAL) {
		/*
		**	If this is a multiplayer game, clear the game's name so we won't respond
		**	to game queries any more (in Call_Back)
		*/
		Session.GameName[0] = 0;

		if (!Session.Play) {
			Session.GamesPlayed++;
			if (Session.MPGameMode != NULL) { // RA2: a network game of a mode that does its own scoring is not scored here
				if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode->vt_entry_08()) { // RA2: as above
					Darken_Surface(NULL, true); // RA2: as above
				} else {
					MPScoreScreen().Presentation_005AAA40(); // RA2: 0x5AAA20, 0x5AAA40 (in place of Multi_Score_Presentation)
				}
			} else {
				MPScoreScreen().Presentation_005AAA40(); // RA2: as above
			}
			if (Session.MPGameMode != NULL) { // RA2: as above
				next = Session.MPGameMode->vt_entry_90(); // RA2: slot 0x90, whether another round is played
			}
			Session.CurGame++;

			if (Session.CurGame >= MAX_MULTI_GAMES) {
				Session.CurGame = MAX_MULTI_GAMES - 1;
			}
		}

		if (!next) { // RA2: as above
			if (Session.MPGameMode != NULL) { // RA2: as above
				Session.MPGameMode->vt_entry_8C(true); // RA2: slot 0x8C, the game mode is told the game was won
			}
			GameActive = false;
			return;
		}
		Session.MPGameMode->vt_entry_94(); // RA2: slot 0x94, a new round begins
	}

	Play_Movie(Scen->WinMovie, THEME_NONE, true, true, true); // RA2: one more argument

	/*
	**	Do the ending screens only if not playing back a recorded game.
	*/
	if (!Session.Play) {
		/*
		**	If the score presentation should be performed, then do
		**	so now.
		*/
		Keyboard->Clear();

		if (!Scen->IsSkipScore) {
			UnknownScore_0069A640().Presentation_0069A650(); // RA2: 0x69A640, 0x69A650 (in place of ScoreClass::Presentation)
		}

		if (Scen->PostScoreMovie != VQ_NONE) {
			Play_Movie(Scen->PostScoreMovie, THEME_NONE, true, true, true); // RA2: one more argument
		}
		if (Scen->PreMapSelectMovie != VQ_NONE) {
			Play_Movie(Scen->PreMapSelectMovie, THEME_NONE, true, true, true); // RA2: one more argument
		}

		if (!CopyProtection::CheckProtectedData() || Scen->IsOneTimeOnly) { // RA2: the copy protection is checked again here
			GameActive = false;
			MouseCursor->Show_Mouse(); // RA2: the cursor's own Show_Mouse (virtual 0x10)
			return;
		}

		/*
		**	If this scenario is flagged as ending the game then print the credits and exit.
		*/
		if (Scen->IsEndOfGame) {
			if (Scen->Campaign != CAMPAIGN_NONE) {
				Play_Movie(Campaigns[Scen->Campaign]->FinalMovie, THEME_NONE, true, true, true); // RA2: one more argument
			}
			Show_Credits(); // RA2: 0x4B37C0, the credits (TibSun: Show_Who_Was_Responsible)
			GameActive = false;
			MouseCursor->Show_Mouse(); // RA2: as above
			return;
		}

		/*
		** Hack section.  If it's allied scenario 10, variation A, then skip the
		** score and map selection, don't increment scenario, and set it to
		** variation B.
		*/
		if (!((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08())) { // RA2: a network round goes straight on
			if (Scen->IsNoMapSel) {
				if (Scen->GlobalFlags[1].Value) {
					Map_Select_Advance(Scen, Scen->AltNextScenarioName);
				} else {
					Map_Select_Advance(Scen, Scen->NextScenarioName);
				}
			} else {
				MouseCursor->Show_Mouse(); // RA2: as above
				Map_Selection(Scen);
				MouseCursor->Hide_Mouse(); // RA2: as above
			}
		}

		Keyboard->Clear();
	}

	MouseCursor->Show_Mouse(); // RA2: as above

	Environment.Store();

	Scen->Scenario++;

	if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08()) { // RA2: a network game starts again over a fresh connection
		if (Session.Type == GAME_IPX) { // RA2: as above
			PacketTransport->Close_Socket(); // RA2: as above
			delete PacketTransport; // RA2: as above
			PacketTransport = NULL; // RA2: as above
			PacketTransport = new IPXInterfaceClass; // RA2: as above
		}
		PacketTransport->Init(); // RA2: as above

		int ai_players = Session.Options.AIPlayers; // RA2: the computer players' setup survives
		int colors[8]; // RA2: as above
		int countries[8]; // RA2: as above
		int difficulties[8]; // RA2: as above
		memcpy(colors, Session.Options.AISlots.Colors, sizeof(colors)); // RA2: as above
		memcpy(countries, Session.Options.AISlots.Countries, sizeof(countries)); // RA2: as above
		memcpy(difficulties, Session.Options.AISlots.Difficulties, sizeof(difficulties)); // RA2: as above
		if (!Net2Init_Network()) { // RA2: as above
			if (PacketTransport != NULL) { // RA2: as above
				delete PacketTransport; // RA2: as above
				PacketTransport = NULL; // RA2: as above
			}
			GameActive = false;
			return;
		}
		Session.Options.AIPlayers = ai_players; // RA2: as above
		for (int index = 0; index < 8; index++) { // RA2: as above
			Session.Options.AISlots.Difficulties[index] = difficulties[index]; // RA2: as above
			Session.Options.AISlots.Countries[index] = countries[index]; // RA2: as above
			Session.Options.AISlots.Colors[index] = colors[index]; // RA2: as above
		}
		Keyboard->Clear();
	}

	/*
	**	Generate a new scenario filename
	*/
	GameActive = CopyProtection::CheckProtectedData(); // RA2: the game ends when the copy protection check fails
	if (!GameActive) { // RA2: as above
		MouseCursor->Show_Mouse(); // RA2: as above
		return;
	}
	Start_Scenario(Scen->ScenarioName, true, Scen->Campaign);

	if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08()) { // RA2: the connections are made again
		Session.Create_Connections(); // RA2: as above
		if (Session.Type == GAME_IPX) { // RA2: as above
			Ipx.Set_Timing((Ipx.Global_Response_Time() + 2 < 15) ? 15 : Ipx.Global_Response_Time() + 2, -1, 600, true); // RA2: the timing is at least 15 (the test is unsigned, the response time is read again)
			Ipx.Set_External_Timing((Ipx.Global_Response_Time() + 2 < 60) ? 60 : Ipx.Global_Response_Time() + 2, -1, 600); // RA2: as above
		} else if (Session.Type == GAME_INTERNET && Session.PrecalcDesiredFrameRate == 0) { // RA2: as above
			Ipx.Set_Timing((Ipx.Global_Response_Time() + 2 < 60) ? 60 : Ipx.Global_Response_Time() + 2, -1, 600, true); // RA2: as above
		}
	}

	Environment.Restore();

	if (ToolTips != NULL) { // RA2: the tooltips come back as the options have them
		ToolTips->Activate(Options.ToolTips); // RA2: as above
	}
	Map.Render();
	Set_Palette(GamePalette, FADE_PALETTE_FAST, Call_Back);
}
#endif



/***********************************************************************************************
 * Do_Lose -- Display losing comments.                                                         *
 *                                                                                             *
 *    Performs the lose mission processing. This will generally display a "would you like      *
 *    to replay" dialog and then either reload the scenario or set flags such that the main    *
 *    menu will appear.                                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/05/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
void Do_Lose(void)
{
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session.Play) { // RA2: the queue is drained and the connections closed
		Wait_For_End_Of_Queue();
		Disconnect_Gracefully(); // RA2: 0x53F780
	}

	ScenarioActive = false;
	TacticalActive = false;

	Scen->ElapsedTimer.Stop();
	Stop_Ingame_Movie();

	MouseCursor->Erase_Mouse(HiddenSurface);
	MouseCursor->Release_Mouse(); // RA2: 0x65DF40, virtual 0x14
	Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: second argument
	MouseCursor->Hide_Mouse(); // RA2: the cursor's own Hide_Mouse (virtual 0xC)

	Toggle_Display_Mode(false);

	/*
	**	If this is a multiplayer game, clear the game's name so we won't respond
	**	to game queries any more (in Call_Back)
	*/
	if (Session.Type != GAME_NORMAL) {
		Session.GameName[0] = 0;
	}

	AudioLevelMute(&AudioGroupAttribs1->VolumeLevel); // RA2: EVA's group is muted (0x406FA0) while the speech finishes
	AudioLevelWaitFade(&AudioGroupAttribs1->VolumeLevel); // RA2: 0x406FE0

	/*
	**	Announce loss to player.
	*/
	int timeout = TickCount + 300; // RA2: 300 ticks

	while (Is_Speaking() && (timeout > TickCount)) {
		Call_Back();
	}

	Speech_Stop(true); // RA2: 0x715DE0
	Aux_Stream_Stop(); // RA2: 0x7160D0
	Audio_Stop_Events(); // RA2: 0x406DC0
	Sound_Clear_Static(); // RA2: 0x714410
	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, AUDIO_VOLUME_MAX); // RA2: the level is restored (0x407080)

	bool replay = false; // RA2: the game mode may ask for another round
	if (Session.Type != GAME_NORMAL) {
		if (!Session.Play) {
			Session.GamesPlayed++;
			if (Session.MPGameMode != NULL) { // RA2: a network game of a mode that does its own scoring is not scored here
				if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode->vt_entry_08()) { // RA2: as above
					Darken_Surface(NULL, true); // RA2: as above
				} else {
					MPScoreScreen().Presentation_005AAA40(); // RA2: 0x5AAA20, 0x5AAA40 (in place of Multi_Score_Presentation)
				}
			} else {
				MPScoreScreen().Presentation_005AAA40(); // RA2: as above
			}
			if (Session.MPGameMode != NULL) { // RA2: as above
				replay = Session.MPGameMode->vt_entry_90(); // RA2: slot 0x90, whether another round is played
			}
			Session.CurGame++;

			if (Session.CurGame >= MAX_MULTI_GAMES) {
				Session.CurGame = MAX_MULTI_GAMES - 1;
			}
		}
		if (!replay) { // RA2: as above
			if (Session.MPGameMode != NULL) { // RA2: as above
				Session.MPGameMode->vt_entry_8C(false); // RA2: slot 0x8C, the game mode is told it is over
			}
			GameActive = false;
			return;
		}
		Session.MPGameMode->vt_entry_94(); // RA2: slot 0x94, a new round begins
	}

	Play_Movie(Scen->LoseMovie, THEME_NONE, true, true, true); // RA2: one more argument
	Darken_Surface(NULL, true); // RA2: 0x5159B0, in place of Draw_Menu_Background

	/*
	**	Start same scenario again
	*/
	if ((Session.Type == GAME_NORMAL && !Session.Play && WWMessageBox(L" ").Process(StringTable::LoadString("TXT_TO_REPLAY", NULL, "D:\\Ra2\\Scenario.CPP", 2612), StringTable::LoadString("TXT_YES", NULL, "D:\\Ra2\\Scenario.CPP", 2612), StringTable::LoadString("TXT_NO", NULL, "D:\\Ra2\\Scenario.CPP", 2612), NULL, false) == 0) || replay) { // RA2: the texts are loaded by name
		if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08()) { // RA2: a network game starts again over a fresh connection
			if (Session.Type == GAME_IPX) { // RA2: as above
				PacketTransport->Close_Socket(); // RA2: as above
				delete PacketTransport; // RA2: as above
				PacketTransport = NULL; // RA2: as above
				PacketTransport = new IPXInterfaceClass; // RA2: as above
			}
			PacketTransport->Init(); // RA2: as above

			int ai_players = Session.Options.AIPlayers; // RA2: the computer players' setup survives
			int colors[8]; // RA2: as above
			int countries[8]; // RA2: as above
			int difficulties[8]; // RA2: as above
			memcpy(colors, Session.Options.AISlots.Colors, sizeof(colors)); // RA2: as above
			memcpy(countries, Session.Options.AISlots.Countries, sizeof(countries)); // RA2: as above
			memcpy(difficulties, Session.Options.AISlots.Difficulties, sizeof(difficulties)); // RA2: as above
			if (!Net2Init_Network()) { // RA2: as above
				if (PacketTransport != NULL) { // RA2: as above
					delete PacketTransport; // RA2: as above
					PacketTransport = NULL; // RA2: as above
				}
				GameActive = false;
				return;
			}
			Session.Options.AIPlayers = ai_players; // RA2: as above
			for (int index = 0; index < 8; index++) { // RA2: as above
				Session.Options.AISlots.Difficulties[index] = difficulties[index]; // RA2: as above
				Session.Options.AISlots.Countries[index] = countries[index]; // RA2: as above
				Session.Options.AISlots.Colors[index] = colors[index]; // RA2: as above
			}
			Keyboard->Clear();
		}

		Start_Scenario(Scen->ScenarioName, false, Scen->Campaign);

		if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08()) { // RA2: the connections are made again
			Session.Create_Connections(); // RA2: as above
			if (Session.Type == GAME_IPX) { // RA2: as above
				Ipx.Set_Timing((Ipx.Global_Response_Time() + 2 < 15) ? 15 : Ipx.Global_Response_Time() + 2, -1, 600, true); // RA2: the timing is at least 15 (the test is unsigned, the response time is read again)
				Ipx.Set_External_Timing((Ipx.Global_Response_Time() + 2 < 60) ? 60 : Ipx.Global_Response_Time() + 2, -1, 600); // RA2: as above
			} else if (Session.Type == GAME_INTERNET && Session.PrecalcDesiredFrameRate == 0) { // RA2: as above
				Ipx.Set_Timing((Ipx.Global_Response_Time() + 2 < 60) ? 60 : Ipx.Global_Response_Time() + 2, -1, 600, true); // RA2: as above
			}
		}

		Environment.Restore();

		if (ToolTips != NULL) { // RA2: the tooltips come back as the options have them
			ToolTips->Activate(Options.ToolTips); // RA2: as above
		}
		Map.Render();
	} else {
		GameActive = false;
	}
}



/***********************************************************************************************
 * Do_Restart -- Handle the restart mission process.                                           *
 *                                                                                             *
 *    This routine is called in the main game loop when the mission must be restarted. This    *
 *    routine will throw away the current game and reload the appropriate mission. The         *
 *    game will "resume" at the start of the mission.                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/24/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Do_Restart(void)
{
	ScenarioActive = false;
	TacticalActive = false;

	if (ToolTips != NULL) { // RA2: the tooltips are switched off for the restart
		ToolTips->Activate(false); // RA2: as above
	}
	Scen->ElapsedTimer.Stop();
	Stop_Ingame_Movie();

	/*
	**	Start a timer going, before we restart the scenario
	*/
	CDTimerClass<SystemTimerClass> timer;
	timer = 60; // RA2: 60 ticks (0x65E4BA)
	Theme.Stop(true); // RA2: the music is stopped (0x6E81A0), not queued to quiet
	Audio_Stop_Events(); // RA2: 0x406DC0
	Sound_Clear_Static(); // RA2: 0x714410
	Speech_Stop(true); // RA2: 0x715DE0
	Aux_Stream_Stop(); // RA2: 0x7160D0

	Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: second argument
	Keyboard->Clear();

	Start_Scenario(Scen->ScenarioName, false, Scen->Campaign);
	Environment.Restore();

	/*
	**	Make sure the message stays displayed for at least 1 second
	*/
	while (timer > 0) {
		Call_Back();
	}
	Keyboard->Clear();

	if (ToolTips != NULL) { // RA2: the tooltips come back as the options have them
		ToolTips->Activate(Options.ToolTips); // RA2: as above
	}
	Map.Render();
}



/// <summary>
/// Handles the player abandoning the mission in progress.
/// The scenario is shut down and the exit acknowledgement is spoken. This routine waits
/// for that speech to finish before letting the game fall back out to the menus.
/// </summary>
void Do_Abort(void)
{
	ScenarioActive = false;
	TacticalActive = false;

	Scen->ElapsedTimer.Stop();
	Scen->Scenario = 1;

	Stop_Ingame_Movie();
	Keyboard->Clear();

	MouseCursor->Erase_Mouse(HiddenSurface);
	MouseCursor->Release_Mouse(); // RA2: 0x65E604, virtual 0x14
	Map.Set_Default_Mouse(MOUSE_NORMAL, false); // RA2: second argument
	Theme.Stop(true); // RA2: the music is stopped (0x6E81A0), not queued to quiet

	AudioLevelMute(&AudioGroupAttribs1->VolumeLevel); // RA2: EVA's group is muted (0x406FA0) while the line plays
	Queue_Speech("EVA_BattleControlTerminated", 2, -1); // RA2: by name (0x715B40), not Speak(VOX_CONTROL_EXIT)
	AudioLevelWaitFade(&AudioGroupAttribs1->VolumeLevel); // RA2: 0x406FE0

	int timeout = TickCount + 300; // RA2: 300 ticks

	while (Is_Speaking() && timeout > TickCount) {
		Call_Back();
	}

	Speech_Stop(true); // RA2: 0x715DE0
	Aux_Stream_Stop(); // RA2: 0x7160D0
	Audio_Stop_Events(); // RA2: 0x406DC0
	Sound_Clear_Static(); // RA2: 0x714410
	AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, AUDIO_VOLUME_MAX); // RA2: the level is restored (0x407080, 0x4000)
	Toggle_Display_Mode(false);
	GameActive = false;
}



/// <summary>
/// Sets the name of the scenario being played.
/// The name is what the save game system records and what the mission description and
/// briefing text are looked up by.
/// </summary>
/// <remarks>A NULL name is quietly ignored, leaving the previous name in place.</remarks>
void ScenarioClass::Set_Scenario_Name(char const * name)
{
	if (name != NULL) {
		strncpy(ScenarioName, name, sizeof(ScenarioName));
		ScenarioName[ARRAY_SIZE(ScenarioName) - 1] = '\0';
	}
}


/***********************************************************************************************
 * Read_Scenario_INI -- Read specified scenario INI file.                                      *
 *                                                                                             *
 *    Read in the scenario INI file. This routine only sets the game                           *
 *    globals with that data that is explicitly defined in the INI file.                       *
 *    The remaining necessary interpolated data is generated elsewhere.                        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *          root      root filename for scenario file to read                                  *
 *                                                                                             *
 *          fresh      true = should the current scenario be cleared?                          *
 *                                                                                             *
 * OUTPUT:  bool; Was the scenario read successful?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Read_Scenario_INI(char const * fname, bool)
{
	if (CD::Force_Available() == false) {
		return(false);
	}

	Frame = 0;
	// RA2: no TournamentTimer is started here (0x65E760 goes straight to the file)

	/*
	**	Create scenario filename and read the file.
	*/
	CCINIClass ini;
	CCFileClass file(fname);

	DebugString("Read_Scenario_INI - Filename is %s\n", fname);

	int result = ini.Load(file, true);

	if (result == 0) {
		DebugString("Scenario ini load failed!\n");
		return(false);
	}

	strcpy(Scen->ScenarioName, fname);

	bool ok = Read_Scenario_INI(ini);
	return(ok);
}



/***********************************************************************************************
 * Remove_AI_Players -- Removes the computer AI houses & their units                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static void Remove_AI_Players(void)
{
	for (int i = 0; i < Houses.Count(); i++) {
		int aicount = 0;
		HouseClass * housep = Houses[i];
		if (housep->IsHuman == false && !housep->Class->IsMultiplayPassive) {
			aicount++;
			if (aicount > Session.Options.AIPlayers) {
				housep->Clobber_All();
			}
		}
	}
}


/// <summary>
/// Performs the multiplayer adjustments that must wait for the map.
/// Surplus computer players are removed, each side's starting units and any random crates
/// are placed, and everyone is allied with the special house. None of this can be done
/// until every object in the scenario has been read, so it is left until the very end.
/// </summary>
/// <param name="official">Is this one of the maps that shipped with the game?</param>
void Multiplayer_Last_Minute_Fixups(bool official)
{
	/*
	**	If Ghosts are disabled and we're not editing, remove computer players
	**	(Must be done after all objects are read in from the INI)
	*/
	if ((Session.Options.AIPlayers + Session.Players.Count() < Rule->MaxPlayers) && !Debug_Map) {
		Remove_AI_Players();
	}
	Call_Back();

	/*
	**	Units must be created for each house.  If bases are ON, this routine
	**	will create an MCV along with the units; otherwise, it will just create
	**	a whole bunch of units.  Session.Options.UnitCount is the total # of units
	**	to create.
	*/
	if (!Debug_Map) {
		int save_init = ScenarioInit;			// turn ScenarioInit off
		ScenarioInit = 0;
		if (Session.MPGameMode != NULL) { // RA2: the game mode places the units
			Session.MPGameMode->StartingPositionsToHouseBaseCells(official); // RA2: 0x65E95B, vtable slot 0x6C (its char is official)
			Session.MPGameMode->GenerateUnits(); // RA2: 0x5B7CE0
		} else {
			Create_Units(official);
		}
		ScenarioInit = save_init;				// turn ScenarioInit back on
	}
	Session.Update_Progress(93);
	Call_Back();

	/*
	**	Place crates if random crates are enabled for
	**	this scenario.
	*/
	if (Session.Options.Goodies) {
		int count = MAX(Rule->CrateMinimum, Session.NumPlayers);
		count = MIN(count, Rule->CrateMaximum);
		for (int index = 0; index < count; index++) {
			Map.Place_Random_Crate();
		}
	}
	Call_Back();

	/*
	**	Compute my starting location as the average Coord of all my stuff.
	*/
	Map.Compute_Start_Pos();

	HouseClass * special_hptr = House_From_HousesType(HouseTypeClass::From_Name("Special"));

	for (int i = 0; i < Houses.Count(); i++) {
		Houses[i]->Base.House = Houses[i];

		if (!Houses[i]->IsHuman && !Houses[i]->Class->IsMultiplayPassive) {

			Houses[i]->PickEnemyTimer = Rule->AIHateDelays[Houses[i]->Difficulty];

			int available_money = Houses[i]->Available_Money();
			Houses[i]->Refund_Money((int)(available_money * (Rule->MultiplayerAICreditMultipliers[Houses[i]->Difficulty] * 0.01))); // RA2: times 0.01, not divided by 100.0
		}

		if (Houses[i]->Class->IsMultiplayPassive) {
			Houses[i]->Class->IsWallOwner = false;
		} else {
			Houses[i]->Class->IsWallOwner = true;
		}

		if (Houses[i] != special_hptr) {
			Houses[i]->Make_Ally(special_hptr, false); // RA2: with the announce flag
			special_hptr->Make_Ally(Houses[i], false); // RA2: as above
		}
	}

	if (Session.MPGameMode != NULL) { // RA2: the game mode is told (slots 0x70 and 0x74)
		Session.MPGameMode->vt_entry_70(); // RA2: as above
		Session.MPGameMode->OnGameStart(); // RA2: as above
	}
	memset(Session.PlayerChatEnabled, 1, sizeof(Session.PlayerChatEnabled)); // RA2: chat is enabled for every player (0x65EB1F)
}



/// <summary>
/// RA2
/// </summary>
static inline int Player_Side(void)
{
	int side = 0;
	if (Session.Type == GAME_NORMAL) {
		if (Scen->Campaign != CAMPAIGN_NONE) {
			side = Campaigns[Scen->Campaign]->CDNumber;
		}
	} else {
		int house = Session.Players[0]->Player.House;
		if (house == -3) {
			house = 0;
		}
		side = HouseTypes[house]->Side;
	}
	return(side);
}


/// <summary>
/// Reads a scenario from the database supplied.
/// This is the workhorse of scenario startup. Whatever scenario was in place is cleared,
/// the theater, side, and rules are prepared, and then every object type in the game is
/// given the chance to read itself in. The loading screen is kept moving throughout, since
/// this can take a while.
/// </summary>
/// <param name="is_mapgen">Is the scenario built by the random map generator?</param>
/// <returns>bool; Was the scenario read successfully?</returns>
bool Read_Scenario_INI(CCINIClass const & ini, bool is_mapgen)
{
	char buffer[32];

	ScenarioInit++;

	DebugString("Clearing old scenario\n");
	Clear_Scenario();

	if (Session.Type == GAME_NORMAL) {
		Scen->Difficulty = (DiffType)Options.Difficulty;
		Scen->CDifficulty = (DiffType)(DIFF_COUNT - 1 - Options.Difficulty);
		Scen->Special.IsFogOfWar = false;
		Special.IsFogOfWar = false;
	} else {
		Scen->Difficulty = (DiffType)Session.Options.AIDifficulty;
		Scen->CDifficulty = (DiffType)(DIFF_COUNT - 1 - Scen->Difficulty);
		Scen->Special.IsFogOfWar = Session.Options.FogOfWar;
		Special.IsFogOfWar = Session.Options.FogOfWar;
	}

	char const * const BASIC = "Basic";
	Scen->InitTime = ini.Get_Int(BASIC, "InitTime", 10000);
	ini.Get_Bool(BASIC, "Official", false); // RA2: read here, but never consulted

	CCINIClass mini; // RA2: declared here, the mission ini is read before the tactical map is made
	CCFileClass cfile; // RA2: as above

	if (Session.Type == GAME_NORMAL) { // RA2: the scenario's own ini and MISSION.INI are read first (0x65EC83)
		_splitpath(Scen->ScenarioName, NULL, NULL, buffer, NULL); // RA2: as above
		strcat(buffer, ".INI"); // RA2: as above
		cfile.Set_Name(buffer); // RA2: as above

		if (cfile.Is_Available(0)) { // RA2: as above
			mini.Load(cfile, false, false); // RA2: as above
			Rule->Addition(mini); // RA2: as above
		}

		cfile.Close(); // RA2: as above
		cfile.Set_Name("MISSION.INI"); // RA2: as above

		if (cfile.Is_Available(0)) { // RA2: as above
			mini.Load(cfile, false, false); // RA2: as above

			Scen->Description[0] = L'\0'; // RA2: the mission texts of this scenario
			if (mini.Is_Present(Scen->ScenarioName, "Name")) { // RA2: as above
				mini.Get_UnicodeString(Scen->ScenarioName, "Name", L"", Scen->Description, 45); // RA2: as above
			}

			Scen->BriefingText[0] = L'\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "Briefing")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "Briefing", "", Scen->BriefingCSF, 32); // RA2: as above
				if (strlen(Scen->BriefingCSF) != 0) { // RA2: as above
					wcsncpy(Scen->BriefingText, StringTable::LoadString(Scen->BriefingCSF, NULL, "D:\\Ra2\\Scenario.CPP", 3180), 1024); // RA2: as above
					Scen->BriefingText[1023] = L'\0'; // RA2: as above
				}
			}

			Scen->UIName[0] = '\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "UIName")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "UIName", "", Scen->UIName, 32); // RA2: as above
				if (strlen(Scen->UIName) != 0) { // RA2: as above
					wcsncpy(Scen->Description, StringTable::LoadString(Scen->UIName, NULL, "D:\\Ra2\\Scenario.CPP", 3192), 45); // RA2: as above
					Scen->Description[44] = L'\0'; // RA2: as above
				}
			}

			Scen->LSLoadMessage[0] = '\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "LSLoadMessage")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "LSLoadMessage", "", Scen->LSLoadMessage, 31); // RA2: as above
			}

			Scen->LSBrief[0] = '\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "LSLoadBriefing")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "LSLoadBriefing", "", Scen->LSBrief, 31); // RA2: as above
			}

			if (mini.Is_Present(Scen->ScenarioName, "LS640BriefLocX")) { // RA2: as above
				Scen->LS640BriefLocX = mini.Get_Int(Scen->ScenarioName, "LS640BriefLocX", Scen->LS640BriefLocX); // RA2: as above
			}
			if (mini.Is_Present(Scen->ScenarioName, "LS640BriefLocY")) { // RA2: as above
				Scen->LS640BriefLocY = mini.Get_Int(Scen->ScenarioName, "LS640BriefLocY", Scen->LS640BriefLocY); // RA2: as above
			}
			if (mini.Is_Present(Scen->ScenarioName, "LS800BriefLocX")) { // RA2: as above
				Scen->LS800BriefLocX = mini.Get_Int(Scen->ScenarioName, "LS800BriefLocX", Scen->LS800BriefLocX); // RA2: as above
			}
			if (mini.Is_Present(Scen->ScenarioName, "LS800BriefLocY")) { // RA2: as above
				Scen->LS800BriefLocY = mini.Get_Int(Scen->ScenarioName, "LS800BriefLocY", Scen->LS800BriefLocY); // RA2: as above
			}

			Scen->LS640BkgdName[0] = '\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "LS640BkgdName")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "LS640BkgdName", "", Scen->LS640BkgdName, 64); // RA2: as above
			}
			Scen->LS800BkgdName[0] = '\0'; // RA2: as above
			if (mini.Is_Present(Scen->ScenarioName, "LS800BkgdName")) { // RA2: as above
				mini.Get_String(Scen->ScenarioName, "LS800BkgdName", "", Scen->LS800BkgdName, 64); // RA2: as above
			}
		}
	}

	LoadProgressManager::Create()->Draw(); // RA2: the loading screen prepares its surfaces (0x535910, 0x535BF0)

	/*
	**
	*/
	Session.Update_Progress(3);
	Swizzler.Reset();

	/*
	**
	*/
	delete TacticalMap; // RA2: deleted without the test (0x65F2B3)

	TacticalMap = new Tactical;
	TacticalMap->Set_View_Dimensions(TacticalRect);

	/*
	**
	*/
	Scen->Theater = ini.Get_TheaterType("Map", "Theater", THEATER_TEMPERATE);
	Init_Theater(Scen->Theater);

	Session.Update_Progress(30);

	Rule->Read_AdvancedCommandBar(UIINI, Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH); // RA2: the command bar rules of UI.INI (0x64DD90), before the rules proper
	Session.Update_Progress(31); // RA2: as above

	Rule->Initialize(*RuleINI);
	Session.Update_Progress(35);
	Call_Back();

	Scen->Read_Global_INI(*RuleINI);
	Call_Back();

	Rule->Addition(const_cast<CCINIClass &>(ini)); // RA2: the scenario's own rules are added (0x65F401; the exe's ini is not const)
	Session.Update_Progress(45);

	/*
	**	Init the Scenario CRC value
	*/
	ScenarioCRC = 0;

	/*
	**	Which side the player is on: for a campaign the side of its CD, otherwise
	**	the side of the first player's country.
	*/
	int side = Player_Side(); // RA2: as above
	bool allied = (side == 0); // RA2: as above

	/*
	**	Read in the specific information for each of the house types.  This creates
	**	the houses of different types.
	*/
	if (Session.Type == GAME_NORMAL) {
		HouseClass::Read_All(ini);
		ini.Get_String(BASIC, "Player", "Americans", buffer, sizeof(buffer)); // RA2: the player's house decides the side (0x65F490)
		int house = House_Index_From_Name(buffer); // RA2: as above
		if (house != -1 && house <= Houses.Count()) { // RA2: as above
			allied = (Houses[house]->Class->Side == 0); // RA2: as above
		} else {
			allied = false; // RA2: as above
		}
		Scen->IsGDI = allied; // RA2: as above
		if (!Prep_For_Side(allied ? SIDE_GDI : SIDE_NOD)) { // RA2: as above
			ScenarioInit--; // RA2: as above
			return(false);
		}
	} else {
		Scen->IsGDI = allied; // RA2: as above
		if (!Prep_For_Side(allied ? SIDE_GDI : SIDE_NOD)) { // RA2: as above
			ScenarioInit--; // RA2: as above
			return(false);
		}
	}
	Session.Update_Progress(50);

	/*
	**
	*/
	if (Scen->Read_INI(ini) == false) {
		ScenarioInit--; // RA2: the nesting count is given back (0x65F598)
		return(false);
	}

	Session.Update_Progress(58);

	if (Session.Type == GAME_NORMAL) { // RA2: the player's house decides the side again, after the scenario's own Basic section
		char player[32]; // RA2: as above
		ini.Get_String(BASIC, "Player", "Americans", player, sizeof(player)); // RA2: as above
		int house = House_Index_From_Name(player); // RA2: as above
		if (house != -1 && house <= Houses.Count()) { // RA2: as above
			allied = (Houses[house]->Class->Side == 0); // RA2: as above
		} else {
			allied = false; // RA2: as above
		}
		if (!Prep_For_Side(allied ? SIDE_GDI : SIDE_NOD)) { // RA2: as above
			ScenarioInit--; // RA2: as above
			return(false);
		}
		Scen->IsGDI = allied; // RA2: as above
	} else {
		Scen->IsGDI = allied; // RA2: as above
		if (Session.MPGameMode != NULL) { // RA2: the game mode releases its alliances (slot 0x68)
			Session.MPGameMode->UnfixAlliances(); // RA2: as above
		}
	}

	/*
	**
	*/
	if (Session.Type != GAME_NORMAL && !Session.Options.BridgeDestruction) {
		Special.IsDestroyBridges = false; // RA2: as above
	}
	Special.Apply_To_Game();

	Call_Back();

	/*
	**	Read in the team-type data. The team types must be created before any
	**	triggers can be created.
	*/
	TeamTypeClass::Read_All(AIINI, SCOPE_GLOBAL);
	TeamTypeClass::Read_All(ini, SCOPE_LOCAL);

	/*
	**
	*/
	ScriptTypeClass::Read_All(AIINI, SCOPE_GLOBAL);
	ScriptTypeClass::Read_All(ini, SCOPE_LOCAL);

	/*
	**
	*/
	TaskForceClass::Read_All(AIINI, SCOPE_GLOBAL);
	TaskForceClass::Read_All(ini, SCOPE_LOCAL);

	/*
	**	Read in the trigger data. The triggers must be created before any other
	**	objects can be initialized.
	*/
	TriggerTypeClass::Read_All(ini);
	TagTypeClass::Read_All(ini);

	AITriggerTypeClass::Read_All(AIINI, SCOPE_GLOBAL);
	AITriggerTypeClass::Read_All(ini, SCOPE_LOCAL);

	Process_All_Task_Forces(); // RA2: every team type is given its task force's attributes (0x6C0220)

	Session.Update_Progress(60);

	/*
	**	Read in the map control values. This includes dimensions
	**	as well as theater information.
	*/
	Map.Read_INI(ini);
	Call_Back();

	/*
	**
	*/
	TubeClass::Read_INI(ini);

	BuildingTypeClass::Post_Read_Tile_Fixup();

	Map.Flag_To_Redraw(GS_REDRAW_ALL);

	Session.Update_Progress(70);

	Call_Back();

	/*
	**	Read in any normal overlay objects.
	*/
	OverlayClass::Read_INI(ini);
	Call_Back();

	/*
	**
	*/
	Map.Reset_Iterator();
	CellClass * cptr = Map.Iterate();
	while (cptr) {
		cptr->Recalc_Attributes();
		cptr = Map.Iterate();
	}

	/*
	**
	*/
	OverlayClass::Post_Read_Vein_Fixups();

	/*
	**	Read in and place the 3D terrain objects.
	*/
	TerrainClass::Read_INI(ini);
	Call_Back();

	/*
	**
	*/
	VeinholeMonsterClass::Init_Vein_Growth_System(true);
	TiberiumClass::Init_Tiberium_Growth_System();
	TiberiumClass::Init_Tiberium_Spread_System();
	Session.Update_Progress(72);

	/*
	**
	*/
	Map.Compute_Radar_Image();

	/*
	**	Read in and place the units (all sides).
	*/
	UnitClass::Read_INI(ini);
	Call_Back();
	Session.Update_Progress(74);

	AircraftClass::Read_INI(ini);
	Call_Back();

	/*
	**	Read in and place the infantry units (all sides).
	*/
	InfantryClass::Read_INI(ini);
	Call_Back();
	Session.Update_Progress(76);

	LightSourceClass::Recalc = false;

	/*
	**	Read in and place all the buildings on the map.
	*/
	BuildingClass::Read_INI(ini);
	Call_Back();
	Session.Update_Progress(78);

	LightSourceClass::Recalc = true;

	Call_Back();

	/*
	**	Read in any smudge overlays.
	*/
	SmudgeClass::Read_INI(ini);
	Call_Back();

	/*
	**
	*/
	if (Session.Type == GAME_SKIRMISH) {
		if (Just4Fun) {
			cfile.Close();
			cfile.Set_Name("TMCJ4F.INI");

			if (cfile.Is_Available(0) == true) { // RA2: with the explicit argument
				mini.Load(cfile, false, false); // RA2: as above
				Rule->Addition(mini); // RA2: as above
			}
		}
	}

	Session.Update_Progress(82);
	Call_Back();

	/*
	**	Perform a final overpass of the map. This handles smoothing of certain
	**	types of terrain (tiberium).
	*/
	Map.Overpass();

	Session.Update_Progress(86);
	Call_Back();

	BeaconManager.Load_Art(); // RA2: the beacon art is loaded here (0x42E1A0)

	Session.Update_Progress(90);
	Call_Back();

	/*
	**	Multi-player last-minute fixups:
	**	- If computer players are disabled, remove all computer-owned houses
	**	- If bases are disabled, create the scenario dynamically
	**	- Remove any flag spot overlays lying around
	**	- If capture-the-flag is enabled, assign flags to cells.
	*/
	if (Session.Type != GAME_NORMAL && !is_mapgen) {
		Multiplayer_Last_Minute_Fixups(ini.Get_Bool(BASIC, "Official", false));
	}

	Call_Back();

	Swizzler.Reset();

	Session.Update_Progress(96);
	Call_Back();

	Process_Deferred_Deletion();

	if (Session.Type != GAME_NORMAL) {
		Scen->Special = Special;
	}

	/*
	**	Return with flag saying that the scenario file was read.
	*/
	ScenarioInit--;

	int save_init = ScenarioInit;
	ScenarioInit = 0;
	BuildingClass::Init_All_Laser_Fences();
	ScenarioInit = save_init;

	Session.Update_Progress(98);
	Call_Back();

	Map.Clear_Background_Stack();

	if (Scen->Special.IsFogOfWar) {
		Map.Init_Fog_System();
	}

	Clear_Radar_Events(); // RA2: the executable calls the free copy at 0x0063C640 (revent.cpp), not RadarEventClass::Clear (0x0063B890, same code)

	Map.Complete_Radar_Refresh();

	return(true);
}



/***********************************************************************************************
 * Write_Scenario_INI -- Write the scenario INI file.                                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      root      root filename for the scenario                                               *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/11/1995 JLB : Updates movie data.                                                      *
 *=============================================================================================*/
void Write_Scenario_INI(char const * fname, bool mplayer)
{
	CCINIClass ini;

	if (Debug_Map) {
		if (CCFileClass(fname).Is_Available()) {
			CCFileClass cfile(fname);
			ini.Load(cfile, true);
		}
	}

	Scen->Write_INI(ini, mplayer);

	if (mplayer) {
		if (RandomMapGen.MapPreview != NULL && RandomMapGen.MapPreview->Get_Preview_Surface() != NULL) {
			RandomMapGen.MapPreview->Write_INI(ini);
		} else {
			MapPreviewClass map_preview;
			map_preview.Create_Preview();
			map_preview.Write_INI(ini);
		}
	} else {
		HouseClass::Write_All(ini);
	}
	Map.Write_INI(ini);
	TerrainClass::Write_INI(ini);
	UnitClass::Write_INI(ini);
	InfantryClass::Write_INI(ini);
	AircraftClass::Write_INI(ini);
	BuildingClass::Write_All(ini);
	OverlayClass::Write_INI(ini);
	SmudgeClass::Write_INI(ini);
	TubeClass::Write_INI(ini);

	if (!mplayer || Debug_Map) {
		ScriptTypeClass::Write_All(ini, SCOPE_LOCAL);
		TaskForceClass::Write_All(ini, SCOPE_LOCAL);
		TeamTypeClass::Write_All(ini, SCOPE_LOCAL);
		AITriggerTypeClass::Write_All(ini, SCOPE_LOCAL);
	}

	TriggerTypeClass::Write_All(ini);
	TagTypeClass::Write_All(ini);

	RawFileClass rawfile(fname);
	ini.Save(rawfile, true);
}


/***********************************************************************************************
 * Assign_Houses -- Assigns multiplayer houses to various players                              *
 *                                                                                             *
 * This routine assigns all players to a multiplayer house slot; it forms network connections  *
 * to each player.  The Connection ID used is the value for that player's HousesType.          *
 *                                                                                             *
 * PlayerPtr is also set here.                                                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      This routine assumes the 'Players' vector has been properly filled in with players'    *
 *      names, addresses, color, etc.                                                          *
 *      Also, it's assumed that the HouseClass's have all been created & initialized.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *   07/14/1995 JLB : Records name of player in house structure.                               *
 *=============================================================================================*/
void Assign_Houses(void)
{
	int const SCHEME_INTENSITY_LEVELS = 53; // RA2: the levels the two scheme lookups at the end ask for (0x65FF6E)
	int i;

	//------------------------------------------------------------------------
	// RA2: a player whose country is still "random observer" (-3) takes country 0
	//------------------------------------------------------------------------
	int count = Session.Players.Count(); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
	for (i = 0; i < count; i++) { // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
		if (Session.Players[i]->Slot.Country == -3) { // RA2: +0x4B
			Session.Players[i]->Set_Country(0); // RA2: 0x66E2D0
		}
	}

	//------------------------------------------------------------------------
	// Initialize
	//------------------------------------------------------------------------
	bool assigned[MAX_PLAYERS];
	for (i = 0; i < MAX_PLAYERS; i++) {
		assigned[i] = false;
	}
	ObserverHouse = NULL; // RA2: 0xA72D28, set below for the player that observes

	//------------------------------------------------------------------------
	// Assign each player in 'Players' to a multiplayer house.  Players will
	// be sorted by their chosen color value (this value must be unique among
	// all the players).
	//------------------------------------------------------------------------
	for (int houses = 0; houses < count; houses++) { // RA2: counted against the count taken above
		//.....................................................................
		// Find the player with the lowest color index
		//.....................................................................
		int index = -1;
		int lowest_color = -1;
		for (int j = 0; j < count; j++) { // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
			//..................................................................
			// If we've already assigned this house, skip it.
			//..................................................................
			if (!assigned[j] && (lowest_color == -1 || Session.Players[j]->Slot.ColorIdx < lowest_color)) { // RA2: +0x53
				lowest_color = Session.Players[index = j]->Slot.ColorIdx; // RA2: as above
			}
		}
		NodeNameType * player = Session.Players[index];
		assigned[index] = true;

		/*
		**	Assign the lowest-color'd player to the next available slot in the
		**	HouseClass array.
		*/
		HouseClass * housep = new HouseClass(HouseTypes[player->Slot.Country]); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once

		if (Session.Type == GAME_INTERNET) { // RA2: a WOL game names the house after the player
			Wstring name = Wstring_Narrow(player->Name); // RA2: as above
			housep->IniName = TStringID<HOUSE_NAME_MAX>(name.get()); // RA2: as above
		} else {
			housep->IniName = TStringID<HOUSE_NAME_MAX>("<human player>"); // RA2: as above
		}
		wcsncpy(housep->UIName, player->Name, 21); // RA2: as above
		housep->UIName[20] = L'\0'; // RA2: as above
		housep->IsHuman = true;
		housep->Init_Data(player->Slot.ColorIdx, (HousesType)player->Slot.Country, Session.Options.Credits); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once

		housep->Scheme = Session.Color_Index_To_Scheme(player->Slot.ColorIdx); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
		housep->Initialize_Radar_Color();
		housep->Update_Laser_Color(); // RA2: 0x4F5570

		if (index == 0) {
			PlayerPtr = housep;
			PlayerPtr->IsPlayerControl = true;
		}
		if (player->Slot.SpectatorFlag == -1) { // RA2: a player without a team observes
			ObserverHouse = housep; // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
		}

		/*
		**	Convert the build level into an actual tech level to assign to the house.
		**	There isn't a one-to-one correspondence.
		*/
		housep->Control.TechLevel = BuildLevel;

		housep->Assign_Handicap(DIFF_NORMAL);

		//.....................................................................
		// Record where we placed this player
		//.....................................................................
		player->Player.ID = housep->HeapID;
	}

	//------------------------------------------------------------------------
	// Now assign computer players to the remaining houses.
	//------------------------------------------------------------------------
	int computers = Session.Options.AIPlayers; // RA2: the computer players come from the per-slot setup, not from random picks
	int made = 0; // RA2: as above
	for (int slot = 0; slot < 8; slot++) { // RA2: as above
		if (made < computers && Session.Options.AISlots.Countries[slot] != -1 && Session.Options.AISlots.Countries[slot] != -3) { // RA2: unused slots are -1, random observers -3
			int country = Session.Options.AISlots.Countries[slot]; // RA2: as above
			int color = Session.Options.AISlots.Colors[slot]; // RA2: as above
			made++; // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once

			/*
			**	Set up the house
			*/
			HouseClass * housep = new HouseClass(HouseTypes[country]); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once

			housep->Control.TechLevel = BuildLevel;
			housep->IsHuman = false;

			housep->Init_Data(color, (HousesType)country, Session.Options.Credits); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
			housep->Scheme = Session.Color_Index_To_Scheme(color);
			housep->Initialize_Radar_Color();
			housep->Update_Laser_Color(); // RA2: 0x4F5570
			housep->IniName = TStringID<HOUSE_NAME_MAX>("Computer"); // RA2: as above
			wcscpy(housep->UIName, StringTable::LoadString("TXT_COMPUTER", NULL, "D:\\Ra2\\Scenario.CPP", 3947)); // RA2: as above

			if (Session.Type != GAME_NORMAL) {
				housep->IQ = Rule->MaxIQ;
			}

			int difficulty = Session.Options.AISlots.Difficulties[slot]; // RA2: each computer player has its own
			if (Session.Players.Count() > 1 && Rule->IsCompEasyBonus && difficulty > 0) { // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
				difficulty--; // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
			}
			housep->Assign_Handicap((DiffType)difficulty); // RA2: the slot fields of the player and its setup (Slot.Country, Slot.ColorIdx), the count read once
		}
	}

	HouseClass * neutral_house = new HouseClass(HouseTypes[HouseTypeClass::From_Name("Neutral")]);
	neutral_house->Scheme = Fetch_Scheme_Index_By_Name("LightGrey", SCHEME_INTENSITY_LEVELS); // RA2: 53 intensity levels, not NUM_INTENSITY_LEVELS (63)
	neutral_house->Initialize_Radar_Color();

	HouseClass * special_house = new HouseClass(HouseTypes[HouseTypeClass::From_Name("Special")]);
	special_house->Scheme = Fetch_Scheme_Index_By_Name("LightGrey", SCHEME_INTENSITY_LEVELS); // RA2: 53 intensity levels, not NUM_INTENSITY_LEVELS (63)
	special_house->Initialize_Radar_Color();
}


/// <summary>
/// Fetches the starting locations available to a multiplayer game.
/// The scenario's own waypoints are preferred, but a map that does not supply enough of
/// them for everyone playing has the shortfall made up with random spots on open ground.
/// </summary>
/// <returns>Returns with the list of cells that players may be started from.</returns>
#ifdef RA2_NON_MATCHING // not exact: 790 bytes differ (adopted text)
DynamicVectorClass<Cell> Build_Start_Waypoint_List(char) // RA2: 0x65FFF0; the argument is passed (MultiplayerGameMode 0x005B7C00 loads it) and not used
{
	DynamicVectorClass<Cell> waypts;

	int num_waypts = 0;
	for (int i = 0; i < 8; i++) {
		if (Scen->Is_Valid_Waypoint(i)) {
			num_waypts++;
		} else {
			break;
		}
	}

	int spectators = 0;
	for (int s = 0; s < Session.Players.Count(); s++) {
		if (Session.Players[s]->Slot.SpectatorFlag == -1) {
			spectators++;
		}
	}

	int look_for = MAX(num_waypts, Session.Players.Count() - spectators + Session.Options.AIPlayers);

	for (int waycount = 0; waycount < look_for; waycount++) {
		if (Scen->Is_Valid_Waypoint(waycount)) {
			waypts.Add(Scen->Get_Waypoint_Cell(waycount));
			DebugString("Multiplayer start waypoint found at cell %d,%d\n", Scen->Get_Waypoint_Cell(waycount).X, Scen->Get_Waypoint_Cell(waycount).Y);
		}
	}

	int deficiency = look_for - waypts.Count();
	if (deficiency > 0) {
		DebugString("Multiplayer start waypoint deficiency - looking for more start positions\n");

		while (waypts.Count() < look_for) {
			Cell trycell = Cell(Map.MapRect.X + Random_Pick(10, Map.MapRect.Width - 10), Map.MapRect.Y + 10 + Random_Pick(0, Map.MapRect.Height - 10));

			trycell = Map.Nearby_Location(trycell, SPEED_TRACK, -1, MZONE_NORMAL, false, Point2D(8, 8));
			if (trycell != CELL_NONE) {
				waypts.Add(trycell);
				DebugString("Random multiplayer start waypoint added at cell %d,%d\n", trycell.X, trycell.Y);
			}
		}
	}

	return(waypts);
}
#endif


/// <summary>
/// RA2
/// </summary>
static inline bool Is_Base_Unit(UnitTypeClass const * type)
{
	for (int i = 0; i < Rule->BaseUnit.Count(); i++) {
		if (Rule->BaseUnit[i] == type) {
			return true;
		}
	}
	return false;
}


/***********************************************************************************************
 * Create_Units -- Creates infantry & units, for non-base multiplayer                          *
 *                                                                                             *
 * This routine uses data tables to determine which units to create for either                 *
 * a GDI or NOD house, and how many of each.                                                   *
 *                                                                                             *
 * It also sets each house's FlagHome & FlagLocation to the Waypoint selected                  *
 * as that house's "home" cell.                                                                *
 *                                                                                             *
 * INPUT:   official -- Directs the placement logic to use the full set of waypoints rather    *
 *                      than biasing toward the first four.                                    *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/



void Create_Units(bool official) // RA2: YR's body, at 0x660320 (a list of base units)
{
	Cell centroid; // RA2: as YR's body
	DynamicVectorClass<TechnoClass *> deployed_list; // RA2: as YR's body
	int unit_count = Session.Options.UnitCount;

	if (Session.Options.Bases) {
		unit_count--;
	}

	DebugString("Creating %d units - Random seed is %08x\n", unit_count, Scen->RandomNumber);
	DebugString("UniqueID is %08x\n", Scen->UniqueID);

	int total_cost = 0;
	int total_objs = 0;
	for (int u = 0; u < UnitTypes.Count(); u++) {
		UnitTypeClass * utype = UnitTypes[u];
		if (utype->IsAllowedToStartInMultiplayer) {
			if (!Is_Base_Unit(utype)) { // RA2: as YR's body
				total_cost += utype->Raw_Cost();
				total_objs++;
			}
		}
	}

	for (int inf = 0; inf < InfantryTypes.Count(); inf++) {
		InfantryTypeClass * itype = InfantryTypes[inf];
		if (itype->IsAllowedToStartInMultiplayer) {
			total_cost += itype->Raw_Cost();
			total_objs++;
		}
	}

	int average_cost = total_cost / total_objs;
	int max_value = unit_count * average_cost;

	DynamicVectorClass<Cell> waypts = Build_Start_Waypoint_List(official); // RA2: the cells players may start from (0x65FFF0)
	bool taken[16];
	for (int index = 0; index < ARRAY_SIZE(taken); index++) {
		taken[index] = false;
	}

	int numtaken = 0;
	for (int house = 0; house < Houses.Count(); house++) {
		HouseClass * hptr = Houses[house];

		if (hptr->Class->IsMultiplayPassive) continue;

		DebugString("Generating units for house %d (%s)\n", house, (char const *)hptr->Class->IniName); // RA2: as YR's body

		DynamicVectorClass<UnitTypeClass *> units; // RA2: as YR's body
		DynamicVectorClass<InfantryTypeClass *> infantry; // RA2: as YR's body
		unsigned int mask = 1 << hptr->Class->HeapID;

		for (int unit = 0; unit < UnitTypes.Count(); unit++) {
			UnitTypeClass * utype = UnitTypes[unit];
			if (utype->IsAllowedToStartInMultiplayer) {
				if (utype->Level <= hptr->Control.TechLevel && (utype->Ownable & mask)) {
					if (!Is_Base_Unit(utype)) { // RA2: as YR's body
						units.Add(utype);
					}
				}
			}
		}

		for (int inf = 0; inf < InfantryTypes.Count(); inf++) {
			InfantryTypeClass * itype = InfantryTypes[inf];
			if (itype->IsAllowedToStartInMultiplayer) {
				if (itype->Level <= hptr->Control.TechLevel && (itype->Ownable & mask)) {
					infantry.Add(itype);
				}
			}
		}

		if (numtaken == 0) {
			int pick = Scen->RandomNumber(0, waypts.Count() - 1); // RA2: the first house takes a random start cell
			centroid = waypts[pick]; // RA2: as above
			taken[pick] = true; // RA2: as above
			numtaken++;
		} else {
			int score[26]; // RA2: each free start cell is scored by its distance to the taken ones
			memset(score, '\0', sizeof(score));

			for (int index = 0; index < waypts.Count(); index++) { // RA2: as above
				if (!taken[index]) { // RA2: as above
					for (int trypoint = 0; trypoint < waypts.Count(); trypoint++) { // RA2: as above
						if (taken[trypoint]) { // RA2: as above
							score[index] += Distance(waypts[index], waypts[trypoint]); // RA2: as above
						}
					}
				}
			}

			int best = 0;
			int bestvalue = 0;
			for (int searchindex = 0; searchindex < waypts.Count(); searchindex++) { // RA2: the farthest cell is taken
				if (score[searchindex] > bestvalue || bestvalue == 0) { // RA2: as above
					bestvalue = score[searchindex]; // RA2: as above
					best = searchindex; // RA2: as above
				}
			}

			centroid = waypts[best]; // RA2: as above
			taken[best] = true;
			numtaken++;
		}

		hptr->Center = Coord(centroid); // RA2: the house's base center is the chosen cell's center

		if (Session.Options.Bases) {
			TechnoClass * obj = new UnitClass(hptr->Get_First_Ownable(Rule->BaseUnit), hptr); // RA2: as YR's body
			if (!obj->Unlimbo(Coord(centroid), DIR_N)) { // RA2: as YR's body
				if (!Scan_Place_Object(obj, centroid, 1)) { // RA2: as YR's body
					delete obj;
					obj = NULL;
				}
			}
			if (obj != NULL) {
				hptr->FlagHome = CELL_NONE;
				hptr->FlagLocation = NULL;
				if (Scen->Special.IsCaptureTheFlag) {
					hptr->Flag_Attach((UnitClass *)obj, true);
				}
			}
		}

		int deployed_value = 0;
		deployed_list.Clear();

		while (deployed_value < max_value) {
			TechnoTypeClass const * tech = NULL;

			if (deployed_value < (max_value * 2) / 3 && units.Count() > 0) {
				tech = units[Scen->RandomNumber(0, units.Count() - 1)]; // RA2: as YR's body
			} else if (infantry.Count() > 0) {
				tech = infantry[Scen->RandomNumber(0, infantry.Count() - 1)]; // RA2: as YR's body
			}

			ObjectClass * obj = tech->Create_One_Of(hptr);
			TechnoClass * tobj = Dynamic_Cast<TechnoClass *>(obj);

			if (!Scan_Place_Object(obj, centroid, 4)) { // RA2: as YR's body
				delete obj;
			} else {
				DebugString("House %s deployed object %s\n", (char const *)hptr->Class->IniName, (char const *)tech->IniName);

				deployed_value += tech->Raw_Cost();
				deployed_list.Add(tobj);

				if (Scen->Special.IsInitialVeteran) {
					tobj->Veterancy.Set_Elite(true);
				}

				if (!hptr->Is_Human_Player()) { // RA2: computer players' units guard the area, human players' guard
					tobj->Set_Mission(MISSION_GUARD_AREA);
				} else {
					tobj->Set_Mission(MISSION_GUARD);
				}

			}
		}

		deployed_list.Clear();
	}
	DebugString("Finished unit generation. Random number is %d\n", Scen->RandomNumber(0, 65535)); // RA2: as YR's body
}



/***********************************************************************************************
 * Clip_Scatter -- randomly scatters from given cell; won't fall off map                       *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell      cell to scatter from                                                         *
 *      maxdist   max distance to scatter                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      new cell number                                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static Cell const Clip_Scatter(Cell const & cell, int maxdist)
{
	int x = cell.X;
	int y = cell.Y;

	int xmin = Map.MapRect.X;
	int xmax = xmin + Map.MapRect.Width - 1;
	int ymin = Map.MapRect.Y;
	int ymax = ymin + Map.MapRect.Height - 1;

	int xdist = Random_Pick(0, maxdist);
	if (Percent_Chance(50)) {
		x += xdist;
		if (x > xmax) {
			x = xmax;
		}
	} else {
		x -= xdist;
		if (x < xmin) {
			x = xmin;
		}
	}

	int ydist = Random_Pick(0, maxdist);
	if (Percent_Chance(50)) {
		y += ydist;
		if (y > ymax) {
			y = ymax;
		}
	} else {
		y -= ydist;
		if (y < ymin) {
			y = ymin;
		}
	}

	Cell result;
	result.X = x;
	result.Y = y;
	return(result);
}


/***********************************************************************************************
 * Clip_Move -- moves in given direction from given cell; clips to map                         *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      cell      cell to start from                                                           *
 *      facing   direction to move                                                             *
 *      dist      distance to move                                                             *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      new cell number                                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/30/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static Cell const Clip_Move(Cell const & cell, FacingType facing, int dist)
{
	int x = cell.X;
	int y = cell.Y;

	int xmin = Map.MapRect.X;
	int xmax = xmin + Map.MapRect.Width - 1;
	int ymin = Map.MapRect.Y;
	int ymax = ymin + Map.MapRect.Height - 1;

	switch (facing) {
		case FACING_N:
			y -= dist;
			break;

		case FACING_NE:
			x += dist;
			y -= dist;
			break;

		case FACING_E:
			x += dist;
			break;

		case FACING_SE:
			x += dist;
			y += dist;
			break;

		case FACING_S:
			y += dist;
			break;

		case FACING_SW:
			x -= dist;
			y += dist;
			break;

		case FACING_W:
			x -= dist;
			break;

		case FACING_NW:
			x -= dist;
			y -= dist;
			break;
	}

	if (x > xmax) {
		x = xmax;
	}
	if (x < xmin) {
		x = xmin;
	}

	if (y > ymax) {
		y = ymax;
	}
	if (y < ymin) {
		y = ymin;
	}

	return(Cell(x, y));
}


/***********************************************************************************************
 * Scan_Place_Object -- places an object >near< the given cell                                 *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      obj      ptr to object to Unlimbo                                                      *
 *      cell      center of search area                                                        *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      true = object was placed; false = it wasn't                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/09/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 913 bytes differ (adopted text)
int Scan_Place_Object(ObjectClass * obj, Cell const & cell, int start) // RA2: the locals are declared where they are first needed
{
	if (Map.In_Radar(cell)) { // RA2: In_Radar (0x54AAB0), not In_Local_Radar
		TechnoClass * techno = Map[cell].Cell_Techno(); // RA2: the locals are declared where they are first needed
		if (!techno || (techno->RTTI == RTTI_INFANTRY &&
			obj->RTTI == RTTI_INFANTRY)) {
			if (obj->Unlimbo(Coord(cell, Map.Get_Height_GL(Coord(cell))))) {
				return(true);
			}
		}
	}

	int tryval; // RA2: the locals are declared after the first try, in this order
	FacingType fcounter; // RA2: as above
	int skipit; // RA2: as above
	TechnoClass * techno; // RA2: as above
	Cell newcell; // RA2: as above
	int dist = start; // RA2: as above
	FacingType rot; // RA2: as above
	for (; dist < 32; dist++) { // RA2: the locals are declared where they are first needed
		rot = Random_Pick(FACING_N, FACING_NW);

		for (tryval = 0; tryval < 2; tryval++) {
			for (fcounter = FACING_N; fcounter <= FACING_NW; fcounter++) {

				skipit = false;

				newcell = Clip_Move(cell, rot, dist);

				if (tryval > 0) {
					newcell = Clip_Scatter(newcell, 1);
				}

				if (newcell == cell) {
					skipit = true;
				}

				if (!skipit) { // RA2: no radar test for the scanned cells
					techno = Map[newcell].Cell_Techno();
					if (techno == NULL || (techno->RTTI == RTTI_INFANTRY &&
						obj->RTTI == RTTI_INFANTRY)) {
						if (obj->Unlimbo(Coord(newcell, Map.Get_Height_GL(Coord(newcell))))) {
							return(true);
						}
					}
				}

				rot++;
				if (rot > FACING_NW) {
					rot = FACING_N;
				}
			}
		}
	}

	return(false);
}
#endif



/// <summary>
/// Writes the scenario object to the save game stream.
/// The elapsed mission clock is halted across the write so that the time recorded is the
/// one the player will be given back when the game is resumed.
/// </summary>
void ScenarioClass::Save(IStream * stream) const
{
	DebugString("Scenario Save: ElapsedTimer = %ld\n", ElapsedTimer);
	bool was_stopped = !ElapsedTimer.Is_Active(); // RA2: Save at 0x660F60 matches only so
	if (!was_stopped) { // RA2: as above
		ElapsedTimer.Stop(); // RA2: as above
	}

	stream->Write(this, sizeof(*this), NULL);

	AllowableUnits.Save(stream);
	AllowableUnitMaximums.Save(stream);
	AllowableUnitCounts.Save(stream);

	if (!was_stopped) { // RA2: as above
		ElapsedTimer.Start(); // RA2: as above
	}
}


/// <summary>
/// Reads the scenario object back from the save game stream.
/// Save games written by an older version of the game were laid out differently, so this
/// routine shuffles their contents into the current arrangement and supplies defaults for
/// the members those saves never carried.
/// </summary>
void ScenarioClass::Load(IStream * stream)
{
	ElapsedTimer.Stop();

	int size = sizeof(*this);

	stream->Read(this, size, NULL); // RA2: no old save game form (no RequiredAddOn, SpeechSide, delta in RA2's layout)

	new (this) ScenarioClass(NoInitClass());

	AllowableUnits.Load(stream);
	AllowableUnitMaximums.Load(stream);
	AllowableUnitCounts.Load(stream);

	for (int i = 0; i < AllowableUnits.Count(); i++) {
		Swizzle_Pointer(&AllowableUnits[i]);
	}

	if (MissionTimerText[0] != '\0') { // RA2: the timer's label is fetched again from its string table name (0x66125D)
		MissionTimerTextCSF = StringTable::LoadString(MissionTimerText, NULL, "D:\\Ra2\\Scenario.CPP", 6379); // RA2: as above
	} else {
		MissionTimerTextCSF = L""; // RA2: as above
	}

	ElapsedTimer.Start();
	DebugString("Scenario Load: ElapsedTimer = %ld\n", ElapsedTimer);
}



/***********************************************************************************************
 * ScenarioClass::Set_Global_To -- Set scenario global to value specified.                     *
 *                                                                                             *
 *    This routine will set the global flag to the falue (true/false) specified. It will       *
 *    also scan for and spring any triggers that are dependant upon that global.               *
 *                                                                                             *
 * INPUT:   global   -- The global flag to change.                                             *
 *                                                                                             *
 *          value    -- The value to change the global flag to.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the previous value of the flag.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/26/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
bool ScenarioClass::Set_Global_To(int global, bool value)
{
	if ((unsigned)global < ARRAY_SIZE(Scen->GlobalFlags)) {
		bool previous = GlobalFlags[global].Value;
		if (previous != value) {
			GlobalFlags[global].Value = value;
			IsGlobalChanged = true;
			TagClass::All_Timer_Global_Reset(global);
		}
		return(previous);
	}
	return(false);
}


/// <summary>
/// Sets a global scenario variable by name.
/// This routine is used by trigger and script code that refers to a global by the
/// designer's name rather than by its number.
/// </summary>
/// <returns>bool; What was the previous state of the global? Returns true if there is
/// no global by that name.</returns>
bool ScenarioClass::Set_Global_To(char const * variable_name, bool value)
{
	int index = Global_From_Name(variable_name);
	if (index != -1) {
		return(Set_Global_To(index, value));
	}
	return(true);
}


/// <summary>
/// Fetches the current state of a global scenario variable.
/// </summary>
/// <param name="value">Reference to be filled in with the state of the global.</param>
/// <returns>bool; Was the global index a legal one?</returns>
bool ScenarioClass::Fetch_Global_Value(int global, bool & value)
{
	if (global >= 0 && global < ARRAY_SIZE(GlobalFlags)) {
		value = GlobalFlags[global].Value;
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the current state of a global scenario variable by name.
/// </summary>
/// <param name="value">Reference to be filled in with the state of the global.</param>
/// <returns>bool; Was a global of that name found?</returns>
bool ScenarioClass::Fetch_Global_Value(char const * variable_name, bool & value)
{
	int index = Global_From_Name(variable_name);
	if (index != -1) {
		return(Fetch_Global_Value(index, value));
	}
	return(false);
}


/// <summary>
/// Finds the index of the global variable that carries the specified name.
/// The names are supplied by the rules database, so this is how designer-authored logic
/// reaches a global without knowing its number.
/// </summary>
/// <returns>Returns with the index of the global, or -1 if no global carries that
/// name.</returns>
int ScenarioClass::Global_From_Name(char const * variable_name)
{
	for (int i = 0; i < ARRAY_SIZE(GlobalFlags); i++) {
		if (!strcmp(variable_name, GlobalFlags[i].VariableName)) {
			return(i);
		}
	}
	return(-1);
}


/// <summary>
/// Reads the global variable names from an INI database.
/// This routine picks up the name assigned to each global flag so that triggers and
/// scripts can refer to the globals by name.
/// </summary>
bool ScenarioClass::Read_Global_INI(CCINIClass const & ini)
{
	char const * const SECTION = "VariableNames";
	int length = MIN(ini.Entry_Count(SECTION), ARRAY_SIZE(GlobalFlags));
	for (int i = 0; i < length; i++) {
		char const * entry = ini.Get_Entry(SECTION, i);
		int index = atoi(entry);

		ini.Get_String(SECTION, entry, NULL, GlobalFlags[index].VariableName, sizeof(GlobalFlags[index].VariableName));
	}
	return(true);
}


/// <summary>
/// Sets one of the scenario's local variables.
/// This routine notifies the trigger system whenever the flag actually changes, so that
/// any elapsed time event keyed to this local is restarted.
/// </summary>
/// <returns>bool; What was the previous state of the local? Returns false if the index
/// is out of range.</returns>
bool ScenarioClass::Set_Local_To(int index, bool value)
{
	if ((unsigned)index < ARRAY_SIZE(Scen->LocalFlags)) {
		bool previous = LocalFlags[index].Value;
		if (previous != value) {
			LocalFlags[index].Value = value;
			IsGlobalChanged = true;
			TagClass::All_Timer_Local_Reset(index);
		}
		return(previous);
	}
	return(false);
}


/// <summary>
/// Sets one of the scenario's local variables by name.
/// </summary>
/// <returns>bool; What was the previous state of the local? Returns true if there is no
/// local by that name.</returns>
bool ScenarioClass::Set_Local_To(char const * variable_name, bool value)
{
	int index = Local_From_Name(variable_name);
	if (index != -1) {
		return(Set_Local_To(index, value));
	}
	return(true);
}


/// <summary>
/// Fetches the current state of a scenario local variable.
/// </summary>
/// <param name="value">Reference to be filled in with the state of the local.</param>
/// <returns>bool; Was the local index a legal one?</returns>
bool ScenarioClass::Fetch_Local_Value(int index, bool & value)
{
	if (index >= 0 && index < ARRAY_SIZE(LocalFlags)) {
		value = LocalFlags[index].Value;
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the current state of a scenario local variable by name.
/// </summary>
/// <param name="value">Reference to be filled in with the state of the local.</param>
/// <returns>bool; Was a local of that name found?</returns>
bool ScenarioClass::Fetch_Local_Value(char const * variable_name, bool & value)
{
	int index = Local_From_Name(variable_name);
	if (index != -1) {
		return(Fetch_Local_Value(index, value));
	}
	return(false);
}


/// <summary>
/// Finds the index of the local variable that carries the specified name.
/// </summary>
/// <returns>Returns with the index of the local, or -1 if no local carries that
/// name.</returns>
int ScenarioClass::Local_From_Name(char const * variable_name)
{
	for (int i = 0; i < ARRAY_SIZE(LocalFlags); i++) {
		if (strcmp(variable_name, LocalFlags[i].VariableName) == 0) {
			return(i);
		}
	}
	return(-1);
}


/// <summary>
/// Reads the scenario's local variables from an INI database.
/// Every local is cleared first, then those the designer named are given their name and
/// their starting state.
/// </summary>
bool ScenarioClass::Read_Local_INI(CCINIClass const & ini)
{
	char buffer[128];
	int index;

	for (index = 0; index < ARRAY_SIZE(LocalFlags); index++) {
		LocalFlags[index].VariableName[0] = '\0';
	}

	char const * const SECTION = "VariableNames";
	int length = MIN(ini.Entry_Count(SECTION), ARRAY_SIZE(LocalFlags));
	for (index = 0; index < length; index++) {
		char const * entry = ini.Get_Entry(SECTION, index);
		int index = atoi(entry);

		ini.Get_String(SECTION, entry, NULL, buffer, sizeof(buffer));
		char * token = strtok(buffer, ",");
		strcpy(LocalFlags[index].VariableName, token);

		token = strtok(NULL, ",");
		if (token != NULL) {
			LocalFlags[index].Value = (atoi(token) == 0) ? false : true;
		}
	}

	return(true);
}


/// <summary>
/// Writes the scenario's named local variables to an INI database.
/// Locals that the designer never named are not recorded.
/// </summary>
bool ScenarioClass::Write_Local_INI(CCINIClass & ini) const
{
	static char const * const SECTION = "VariableNames";
	char index_buffer[10];
	char buffer[128];

	ini.Clear(SECTION);
	int length = ARRAY_SIZE(LocalFlags);
	for (int index = 0; index < length; index++) {
		if (LocalFlags[index].VariableName[0] != '\0') {
			wsprintf(index_buffer, "%d", index);
			wsprintf(buffer, "%s,%d", LocalFlags[index].VariableName, LocalFlags[index].Value ? 1 : 0);
			ini.Put_String(SECTION, index_buffer, buffer);
		}
	}
	return(true);
}


/// <summary>
/// Counts the local variables that the designer has named.
/// </summary>
/// <returns>Returns with the number of locals this scenario puts to use.</returns>
int ScenarioClass::Used_Local_Count(void)
{
	int count = 0;
	for (int i = 0; i < ARRAY_SIZE(LocalFlags); i++) {
		if (LocalFlags[i].VariableName[0] != '\0') {
			count++;
		}
	}
	return(count);
}


/// <summary>
/// Finds a local variable slot that has not been claimed.
/// This routine is used by the editor when the designer asks for another local.
/// </summary>
/// <returns>Returns with the index of the first free local, or -1 if all of them are
/// in use.</returns>
int ScenarioClass::First_Unused_Local(void) const
{
	for (int index = 0; index < ARRAY_SIZE(LocalFlags); index++) {
		if (LocalFlags[index].VariableName[0] == '\0') {
			return(index);
		}
	}
	return(-1);
}





/// <summary>
/// Reads the scenario's parameters from an INI database.
/// This routine picks up everything that describes the scenario itself -- the movies, the
/// theme, the carry over money rules, the growth switches and the ambient lighting. The
/// player's house is established here as well, since the objects created afterward attach
/// themselves to it.
/// </summary>
bool ScenarioClass::Read_INI(CCINIClass const & ini)
{
	char const * const BASIC = "Basic";

	int i;

	Special.Read_INI(ini);
	ini.Get_String(BASIC, "NextScenario", NextScenarioName, NextScenarioName, sizeof(NextScenarioName)); // RA2: no BriefingText, Description (TibSun's use does not compile against RA2's declaration)
	ini.Get_String(BASIC, "AltNextScenario", AltNextScenarioName, AltNextScenarioName, sizeof(AltNextScenarioName));
	IntroMovie = ini.Get_VQType(BASIC, "Intro", IntroMovie);
	BriefMovie = ini.Get_VQType(BASIC, "Brief", BriefMovie);
	WinMovie = ini.Get_VQType(BASIC, "Win", WinMovie);
	LoseMovie = ini.Get_VQType(BASIC, "Lose", LoseMovie);
	ActionMovie = ini.Get_VQType(BASIC, "Action", ActionMovie);
	PostScoreMovie = ini.Get_VQType(BASIC, "PostScore", PostScoreMovie);
	PreMapSelectMovie = ini.Get_VQType(BASIC, "PreMapSelect", PreMapSelectMovie);

	IsMultiplayerOnly = ini.Get_Bool(BASIC, "MultiplayerOnly", IsMultiplayerOnly);
	IsInheritTimer = ini.Get_Bool(BASIC, "TimerInherit", IsInheritTimer);
	IsEndOfGame = ini.Get_Bool(BASIC, "EndOfGame", IsEndOfGame);
	TransitTheme = ini.Get_ThemeType(BASIC, "Theme", TransitTheme);
	NewINIFormat = ini.Get_Int(BASIC, "NewINIFormat", 0);
	CarryOverPercent = ini.Get_Float(BASIC, "CarryOverMoney", CarryOverPercent);
	CarryOverPercent = MIN(CarryOverPercent, 1.0);
	CarryOverCap = ini.Get_Int(BASIC, "CarryOverCap", CarryOverCap);
	IsSkipScore = ini.Get_Bool(BASIC, "SkipScore", IsSkipScore);
	IsOneTimeOnly = ini.Get_Bool(BASIC, "OneTimeOnly", IsOneTimeOnly);
	IsNoMapSel = ini.Get_Bool(BASIC, "SkipMapSelect", IsNoMapSel);
	IsTruckCrate = ini.Get_Bool(BASIC, "TruckCrate", IsTruckCrate);
	IsTrainCargo = ini.Get_Bool(BASIC, "TrainCrate", IsTrainCargo);
	IsMoneyTiberium = ini.Get_Bool(BASIC, "FillSilos", IsMoneyTiberium);
	IsIgnoreGlobalAITriggers = ini.Get_Bool(BASIC, "IgnoreGlobalAITriggers", IsIgnoreGlobalAITriggers);
	Percent = ini.Get_Int(BASIC, "Percent", Percent);
	StartingDropships = ini.Get_Int(BASIC, "StartingDropships", StartingDropships);
	AllowableUnits = ini.Get_TechnoType_List(BASIC, "AllowableUnits", AllowableUnits);
	AllowableUnitMaximums = ini.Get_IntList(BASIC, "AllowableUnitMaximums", AllowableUnitMaximums);
	IsTibGrowth = ini.Get_Bool(BASIC, "TiberiumGrowthEnabled", IsTibGrowth);
	IsVeinGrowth = ini.Get_Bool(BASIC, "VeinGrowthEnabled", IsVeinGrowth);
	IsIceGrowth = ini.Get_Bool(BASIC, "IceGrowthEnabled", IsIceGrowth);
	IsTiberiumDeathToVisceroid = ini.Get_Bool(BASIC, "TiberiumDeathToVisceroid", IsTiberiumDeathToVisceroid);
	IsFreeRadar = ini.Get_Bool(BASIC, "FreeRadar", IsFreeRadar);
	Home = ini.Get_Int(BASIC, "HomeCell", Home);
	AltHome = ini.Get_Int(BASIC, "AltHomeCell", AltHome);

	ini.Get_Bool(BASIC, "CivEvac", false); // RA2: no RequiredAddOn (not in RA2's layout)

	for (i = 0; i < AllowableUnits.Count() - AllowableUnitMaximums.Count(); i++) {
		AllowableUnitMaximums.Add(-1);
	}

	for (i = 0; i < AllowableUnitMaximums.Count(); i++) {
		AllowableUnitCounts.Add(0);
	}

	Read_Local_INI(ini);
	Call_Back();

	char const * const RANKING = "Ranking"; // RA2: the par times and rank texts are read before the lighting

	ParTimeEasy = ini.Get_Time(RANKING, "ParTimeEasy", ParTimeEasy); // RA2: as above
	ParTimeMedium = ini.Get_Time(RANKING, "ParTimeMedium", ParTimeMedium); // RA2: as above
	ParTimeDifficult = ini.Get_Time(RANKING, "ParTimeHard", ParTimeDifficult); // RA2: as above
	ini.Get_String(RANKING, "UnderParTitle", "GUI:NoUnderParTitle", UnderParTitle, sizeof(UnderParTitle)); // RA2: as above
	ini.Get_String(RANKING, "UnderParMessage", "GUI:NoUnderParMessage", UnderParMessage, sizeof(UnderParMessage)); // RA2: as above
	ini.Get_String(RANKING, "OverParTitle", "GUI:NoOverParTitle", OverParTitle, sizeof(OverParTitle)); // RA2: as above
	ini.Get_String(RANKING, "OverParMessage", "GUI:NoOverParMessage", OverParMessage, sizeof(OverParMessage)); // RA2: as above

	char const * const LIGHTING = "Lighting";

	DesiredAmbientLight = CurrentAmbientLight = AmbientLight = (int)(ini.Get_Float(LIGHTING, "Ambient", AmbientLight * 0.01) * 100.0); // RA2: the lighting reads (and its defaults) default to the current value times 0.01, the ground and level ones to an integer division by 1000
	RedTint = (int)(ini.Get_Float(LIGHTING, "Red", RedTint * 0.01) * 100.0); // RA2: as above
	GreenTint = (int)(ini.Get_Float(LIGHTING, "Green", GreenTint * 0.01) * 100.0); // RA2: as above
	BlueTint = (int)(ini.Get_Float(LIGHTING, "Blue", BlueTint * 0.01) * 100.0); // RA2: as above
	GroundLight = (int)(ini.Get_Float(LIGHTING, "Ground", GroundLight / 1000) * 1000.0); // RA2: as above
	LevelLight = (int)(ini.Get_Float(LIGHTING, "Level", LevelLight / 1000) * 1000.0); // RA2: as above
	IonAmbientLight = (int)(ini.Get_Float(LIGHTING, "IonAmbient", AmbientLight * 0.01) * 100.0); // RA2: as above
	IonRedTint = (int)(ini.Get_Float(LIGHTING, "IonRed", RedTint * 0.01) * 100.0); // RA2: as above
	IonGreenTint = (int)(ini.Get_Float(LIGHTING, "IonGreen", GreenTint * 0.01) * 100.0); // RA2: as above
	IonBlueTint = (int)(ini.Get_Float(LIGHTING, "IonBlue", BlueTint * 0.01) * 100.0); // RA2: as above
	IonGroundLight = (int)(ini.Get_Float(LIGHTING, "IonGround", GroundLight / 1000) * 1000.0); // RA2: as above
	IonLevelLight = (int)(ini.Get_Float(LIGHTING, "IonLevel", LevelLight / 1000) * 1000.0); // RA2: as above
	NukeAmbientChangeRate = ini.Get_Float(LIGHTING, "NukeAmbientChangeRate", NukeAmbientChangeRate); // RA2: as above

	Session.Update_Progress(55);
	Call_Back();

	/*
	**	Assign PlayerPtr by reading the player's house from the INI;
	**	Must be done before any TechnoClass objects are created.
	*/
	if (Session.Type == GAME_NORMAL) {
		char player[20]; // RA2: the player's house is found by its name among the houses made, not by type
		ini.Get_String(BASIC, "Player", "", player, sizeof(player)); // RA2: as above
		int house = House_Index_From_Name(player); // RA2: as above
		if (house == -1) { // RA2: as above
			house = 0; // RA2: as above
		}
		PlayerPtr = Houses[house]; // RA2: as above
		PlayerHouse = PlayerPtr->Class->House; // RA2: as above
	} else {
		Assign_Houses();
	}

	PlayerPtr->IsHuman = true;
	PlayerPtr->IsPlayerControl = true;
	PlayerPtr->CurrentDropship = 0;

	Session.Update_Progress(58);
	Call_Back();

	if (Debug_Map) {
		IsometricTileTypeClass::Init_Drawers();
	}

	Session.Update_Progress(60);
	Call_Back();

	return(true);
}



/// <summary>
/// RA2
/// </summary>
static inline Point2D Header_Point(CellClass & cell)
{
	int px;
	int py;
	TacticalMap->Lepton_To_Map_Pixel(cell.Center_Coord().X, cell.Center_Coord().Y, px, py);
	return Point2D(px / 60, py / 30);
}


/// <summary>
/// Writes the scenario's parameters to an INI database.
/// This routine is used by the map editor when it saves a scenario back out.
/// </summary>
/// <param name="mplayer">Is the scenario being saved as a multiplayer map?</param>
bool ScenarioClass::Write_INI(CCINIClass & ini, bool mplayer) const
{
	static char const * VARIABLE_NAMES = "VariableNames"; // RA2: RA2's form
	static char const * BASIC = "Basic"; // RA2: as above
	ini.Clear(BASIC);
	ini.Clear(BASIC, "CivEvac");
	if (strlen(Scen->BriefingCSF) != 0) { // RA2: as above
		ini.Put_String(BASIC, "Briefing", Scen->BriefingCSF); // RA2: as above
	}
	if (strlen(Scen->UIName) != 0) { // RA2: as above
		ini.Put_String(BASIC, "UIName", Scen->UIName); // RA2: as above
	}
	Special.Write_INI(ini); // RA2: as above
	ini.Put_String(BASIC, "NextScenario", NextScenarioName); // RA2: as above
	ini.Put_String(BASIC, "AltNextScenario", AltNextScenarioName); // RA2: as above
	ini.Put_UnicodeString(BASIC, "Name", Description); // RA2: as above
	ini.Put_Int(BASIC, "NewINIFormat", 4); // RA2: as above
	ini.Put_Int(BASIC, "CarryOverCap", CarryOverCap / 100);
	ini.Put_Bool(BASIC, "EndOfGame", IsEndOfGame); // RA2: as above
	ini.Put_Bool(BASIC, "SkipScore", IsSkipScore); // RA2: as above
	ini.Put_Bool(BASIC, "OneTimeOnly", IsOneTimeOnly); // RA2: as above
	ini.Put_Bool(BASIC, "SkipMapSelect", IsNoMapSel); // RA2: as above
	ini.Put_Bool(BASIC, "Official", true);
	ini.Put_Bool(BASIC, "IgnoreGlobalAITriggers", IsIgnoreGlobalAITriggers);
	ini.Put_Bool(BASIC, "TruckCrate", IsTruckCrate); // RA2: as above
	ini.Put_Bool(BASIC, "TrainCrate", IsTrainCargo);
	ini.Put_Int(BASIC, "Percent", Percent);
	ini.Put_TechnoType_List(BASIC, "AllowableUnits", AllowableUnits);
	ini.Put_IntList(BASIC, "AllowableUnitMaximums", AllowableUnitMaximums);
	ini.Put_ThemeType(BASIC, "Theme", TransitTheme); // RA2: written under the key "Theme" (the string at 0x007D9D6C), the key Read_INI reads

	if (!mplayer) {
		char player[32]; // RA2: as above
		strcpy(player, PlayerPtr->IniName); // RA2: as above
		ini.Put_String(BASIC, "Player", player); // RA2: as above
		ini.Put_VQType(BASIC, "Intro", IntroMovie);
		ini.Put_VQType(BASIC, "Brief", BriefMovie);
		ini.Put_VQType(BASIC, "Win", WinMovie);
		ini.Put_VQType(BASIC, "Lose", LoseMovie);
		ini.Put_VQType(BASIC, "Action", ActionMovie);
		ini.Put_VQType(BASIC, "PostScore", PostScoreMovie);
		ini.Put_VQType(BASIC, "PreMapSelect", PreMapSelectMovie);
		ini.Put_Float(BASIC, "CarryOverMoney", CarryOverPercent); // RA2: as above
		ini.Put_Bool(BASIC, "TimerInherit", IsInheritTimer);
		ini.Put_Bool(BASIC, "FillSilos", IsMoneyTiberium); // RA2: as above
		ini.Put_Int(BASIC, "StartingDropships", StartingDropships);
		ini.Put_Int(BASIC, "HomeCell", Home); // RA2: as above
		ini.Put_Int(BASIC, "AltHomeCell", AltHome); // RA2: as above
	}
	ini.Put_Int(BASIC, "MultiplayerOnly", IsMultiplayerOnly); // RA2: as above
	ini.Put_Bool(BASIC, "TiberiumGrowthEnabled", IsTibGrowth);
	ini.Put_Bool(BASIC, "VeinGrowthEnabled", IsVeinGrowth);
	ini.Put_Bool(BASIC, "IceGrowthEnabled", IsIceGrowth);
	ini.Put_Bool(BASIC, "TiberiumDeathToVisceroid", IsTiberiumDeathToVisceroid); // RA2: as above
	ini.Put_Bool(BASIC, "FreeRadar", IsFreeRadar); // RA2: as above
	ini.Put_Int(BASIC, "InitTime", InitTime);

	ini.Clear(VARIABLE_NAMES); // RA2: as above
	for (int index = 0; index < 50; index++) { // RA2: as above
		ScenarioClass::ScenarioFlagType const &variable = LocalFlags[index]; // RA2: as above
		if (variable.VariableName[0] != 0) { // RA2: as above
			char entry[12]; // RA2: as above
			char value[128]; // RA2: as above
			wsprintfA(entry, "%d", index); // RA2: as above
			wsprintfA(value, "%s,%d", (char *)&variable, variable.Value != 0); // RA2: as above
			ini.Put_String(VARIABLE_NAMES, entry, value); // RA2: as above
		}
	}

	ini.Put_Time("Ranking", "ParTimeEasy", ParTimeEasy); // RA2: as above
	ini.Put_Time("Ranking", "ParTimeMedium", ParTimeMedium); // RA2: as above
	ini.Put_Time("Ranking", "ParTimeHard", ParTimeDifficult); // RA2: as above
	ini.Put_String("Ranking", "UnderParTitle", UnderParTitle); // RA2: as above
	ini.Put_String("Ranking", "UnderParMessage", UnderParMessage); // RA2: as above
	ini.Put_String("Ranking", "OverParTitle", OverParTitle); // RA2: as above
	ini.Put_String("Ranking", "OverParMessage", OverParMessage); // RA2: as above

	ini.Clear("Lighting"); // RA2: as above
	double ambient = AmbientLight * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "Ambient", ambient); // RA2: as above
	double red = RedTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "Red", red); // RA2: as above
	double green = GreenTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "Green", green); // RA2: as above
	double blue = BlueTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "Blue", blue); // RA2: as above
	double ground = GroundLight * 0.001; // RA2: as above
	ini.Put_Float("Lighting", "Ground", ground); // RA2: as above
	double level = LevelLight * 0.001; // RA2: as above
	ini.Put_Float("Lighting", "Level", level); // RA2: as above
	double ionambient = IonAmbientLight * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "IonAmbient", ionambient); // RA2: as above
	double ionred = IonRedTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "IonRed", ionred); // RA2: as above
	double iongreen = IonGreenTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "IonGreen", iongreen); // RA2: as above
	double ionblue = IonBlueTint * 0.01; // RA2: as above
	ini.Put_Float("Lighting", "IonBlue", ionblue); // RA2: as above
	double ionground = IonGroundLight * 0.001; // RA2: as above
	ini.Put_Float("Lighting", "IonGround", ionground); // RA2: as above
	double ionlevel = IonLevelLight * 0.001; // RA2: as above
	ini.Put_Float("Lighting", "IonLevel", ionlevel); // RA2: as above
	return(true);
}



/// <summary>
/// Adjusts a time value to suit the speed of this machine.
/// Use this routine where a delay should feel the same regardless of the processor it
/// runs on. The value is scaled against a normalized machine speed.
/// </summary>
/// <returns>Returns with the time value scaled for this machine.</returns>
int Adjust_To_CPU_Timing(int time)
{
	static const double P_SIX_TWEAK = .8;
	static const double TIME_SCALE = 200;

	int cpu_type;
	bool mmx;

	int speed = 0;

	Get_CPU_Type(cpu_type, mmx, NULL, 0);

	if (cpu_type > PROC_PENTIUM_PRO) {
		time = (int)(time * P_SIX_TWEAK);
	}

	double hiscale = ((__int64)2 << 31);

	unsigned int lo;
	unsigned int hi;
	lo = Get_CPU_Rate(hi);

	if (lo != 0 || hi != 0) {
		speed = (int)(((double)lo + ((double)hi * hiscale)) / (double)1000000);
	}

	time = (int)(time * TIME_SCALE / speed);
	return(time);
}


/// <summary>
/// Submits the scenario state to a CRC calculation.
/// This routine is used by the multiplayer sync check, which compares the CRC taken on
/// each machine in order to spot a game that has drifted out of step.
/// </summary>
void ScenarioClass::Compute_CRC(CRCEngine & crc) const
{
	crc(UIName); // RA2: RA2's form
	crc((long)IntroMovie); // RA2: as above
	crc((long)BriefMovie); // RA2: as above
	crc((long)WinMovie); // RA2: as above
	crc((long)LoseMovie); // RA2: as above
	crc((long)ActionMovie); // RA2: as above
	crc((long)PostScoreMovie); // RA2: as above
	crc((long)PreMapSelectMovie); // RA2: as above
	crc((long)PlayerHouse); // RA2: as above
	crc((long)Percent); // RA2: as above
	crc((long)TransitTheme); // RA2: as above
	crc(CarryOverPercent);
	crc(IsMultiplayerOnly);
	crc(IsInheritTimer);
	crc((long)(CarryOverCap / 100)); // RA2: as above
	crc((long)BridgeCount); // RA2: as above
	crc(IsEndOfGame);
	crc((long)Campaign); // RA2: as above
	crc(IsSkipScore);
	crc(IsOneTimeOnly);
	crc(IsNoMapSel);
	crc(IsMoneyTiberium);
	crc(IsIgnoreGlobalAITriggers);
	crc(IsTruckCrate);
	crc((long)Percent); // RA2: as above
	crc((long)StartingDropships); // RA2: as above
	crc((long)MissionTimer.Value()); // RA2: as above
	crc((long)ShroudTimer.Value()); // RA2: as above
	crc((long)FogTimer.Value()); // RA2: as above
	crc((long)IceGrowthTimer.Value()); // RA2: as above
	crc((long)VeinGrowthTimer.Value()); // RA2: as above
	crc((long)ElapsedTimer.Value()); // RA2: as above
	crc((long)Difficulty); // RA2: as above
	crc((long)CDifficulty); // RA2: as above
	crc((long)Theater); // RA2: as above
	crc((long)GroundLight); // RA2: as above
	crc((long)LevelLight); // RA2: as above
	crc(IsFreeRadar);
	crc((long)IonAmbientLight); // RA2: as above
	crc((long)IonRedTint); // RA2: as above
	crc((long)IonGreenTint); // RA2: as above
	crc((long)IonBlueTint); // RA2: as above
	crc((long)IonGroundLight); // RA2: as above
	crc((long)IonLevelLight); // RA2: as above
	crc((long)AmbientLight); // RA2: as above
	crc((long)CurrentAmbientLight); // RA2: as above
	crc((long)DesiredAmbientLight); // RA2: as above
}



/// <summary>
/// Fetches a unique identification number.
/// This routine hands out a number that nothing else in the scenario shares.
/// </summary>
/// <returns>Returns with the newly allocated identifier.</returns>
int ScenarioClass::Get_Unique_ID(void)
{
	UniqueID++;

	return(UniqueID);
}


/// <summary>
/// Fetches the cell that a waypoint is anchored to.
/// </summary>
/// <returns>Returns with the cell the waypoint occupies.</returns>
/// <remarks>Only call this routine for a waypoint that has been placed on the map.</remarks>
Cell ScenarioClass::Get_Waypoint_Cell(WAYPOINT waypoint) const
{
	return(Waypoint[waypoint]);
}


/// <summary>
/// Fetches the waypoint's cell as a target.
/// This routine is used where mission and trigger logic needs something it can be sent to
/// rather than a bare cell number.
/// </summary>
/// <returns>Returns with a pointer to the cell object the waypoint occupies.</returns>
/// <remarks>Only call this routine for a waypoint that has been placed on the map.</remarks>
AbstractClass * ScenarioClass::Get_Waypoint_Target(WAYPOINT waypoint) const
{
	return(&Map[Waypoint[waypoint]]);
}


/// <summary>
/// Fetches the coordinate at the center of a waypoint's cell.
/// A waypoint that sits under a bridge is lifted to the bridge deck, so that whatever is
/// sent there travels over the span rather than beneath it.
/// </summary>
/// <returns>Returns with the coordinate of the waypoint.</returns>
/// <remarks>Only call this routine for a waypoint that has been placed on the map.</remarks>
#ifdef RA2_NON_MATCHING // not exact: 3 bytes differ (adopted text)
Coord ScenarioClass::Get_Waypoint_Coord(WAYPOINT waypoint) const
{
	CellClass const * cell = &Map[Waypoint[waypoint]];
	Coord coord = cell->Center_Coord();

	if (cell->IsUnderBridge || cell->WasUnderBridge) {
		coord.Z += BRIDGE_LEPTON_HEIGHT;
	}
	return(coord);
}
#endif


/// <summary>
/// Removes every waypoint from the scenario.
/// This routine is used when the scenario is being torn down to make room for another.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 3 bytes differ (adopted text)
void ScenarioClass::Clear_All_Waypoints(void)
{
	for (int index = 0; index < ARRAY_SIZE(Waypoint); index++) {
		Waypoint[index] = CELL_NONE;
	}
}
#endif


/// <summary>
/// Has this waypoint been placed on the map?
/// </summary>
/// <returns>bool; Does the waypoint refer to a real cell?</returns>
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (adopted text)
bool ScenarioClass::Is_Valid_Waypoint(WAYPOINT waypoint) const
{
	if (waypoint < ARRAY_SIZE(Waypoint) &&
		waypoint >= 0 &&
		Waypoint[waypoint] != CELL_NONE) {

		return(true);
	}
	return(false);
}
#endif


/// <summary>
/// Reads the waypoint list from an INI database.
/// The cells named are also flagged as waypoints so that the editor can display them.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 9 bytes differ (adopted text)
void ScenarioClass::Read_Waypoints(CCINIClass const & ini)
{
	char buf[20];

	for (int i = 0; i < WAYPT_COUNT; i++) {
		wsprintf(buf, "%d", i);
		int val = ini.Get_Int("Waypoints", buf, 0);
		if (val == 0) {
			Waypoint[i] = CELL_NONE;
		} else {
			Waypoint[i] = Cell(val % 1000, val / 1000);
		}

		if (Is_Valid_Waypoint(i)) {
			Map[Waypoint[i]].IsWaypoint = 1; // RA2: Read_Waypoints at 0x663280 matches only so
		}
	}
}
#endif


/// <summary>
/// Writes the placed waypoints to an INI database.
/// Waypoints the designer never placed are not recorded.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 12 bytes differ (adopted text)
void ScenarioClass::Write_Waypoints(CCINIClass & ini) const
{
	static char const * const WAYNAME = "Waypoints";
	char entry[32];

	ini.Clear(WAYNAME);
	for (int i = 0; i < WAYPT_COUNT; i++) {
		if (Waypoint[i] != CELL_NONE) {
			wsprintf(entry, "%d", i);
			ini.Put_Int(WAYNAME, entry, Waypoint[i].Y * 1000 + Waypoint[i].X);
		}
	}
}
#endif


/// <summary>
/// Removes a single waypoint from the scenario.
/// </summary>
#ifdef RA2_NON_MATCHING // not exact: 3 bytes differ (adopted text)
void ScenarioClass::Clear_Waypoint(WAYPOINT waypoint)
{
	Waypoint[waypoint] = CELL_NONE;
}
#endif


/// <summary>
/// Anchors a waypoint to the specified cell.
/// </summary>
void ScenarioClass::Set_Waypoint(WAYPOINT waypoint, Cell cell)
{
	Waypoint[waypoint] = cell;
}


/// <summary>
/// Fetches the cell object that a waypoint is anchored to.
/// </summary>
/// <returns>Returns with a pointer to the cell the waypoint occupies.</returns>
/// <remarks>Only call this routine for a waypoint that has been placed on the map.</remarks>
CellClass * ScenarioClass::Get_Waypoint_CellClass(WAYPOINT waypoint) const
{
	return(&Map[Waypoint[waypoint]]);
}


/// <summary>
/// Finds the name of the first waypoint that has not been placed.
/// This routine is used by the editor when it must offer the designer a fresh waypoint.
/// </summary>
/// <returns>Returns with the name of the first unused waypoint, or an empty string if
/// every waypoint has been placed.</returns>
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (adopted text)
char const * ScenarioClass::First_Unused_Waypoint_Name(void) const
{
	for (int index = 0; index < ARRAY_SIZE(Waypoint); index++) {
		if (!Is_Valid_Waypoint(index)) {
			return(Waypoint_To_Name(index));
		}
	}
	return("");
}
#endif

