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

/* $Header: /CounterStrike/CONQUER.CPP 6     3/13/97 2:05p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CONQUER.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 3, 1991                                                *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CC_Draw_Shape -- Custom draw shape handler.                                               *
 *   Call_Back -- Main game maintenance callback routine.                                      *
 *   Color_Cycle -- Handle the general palette color cycling.                                  *
 *   Crate_From_Name -- Given a crate name convert it to a crate type.                         *
 *   Disk_Space_Available -- returns bytes of free disk space                                  *
 *   Do_Record_Playback -- handles saving/loading map pos & current object                     *
 *   Fading_Table_Name -- Builds a theater specific fading table name.                         *
 *   Fetch_Techno_Type -- Convert type and ID into TechnoTypeClass pointer.                    *
 *   Force_CD_Available -- Ensures that specified CD is available.                             *
 *   Get_Radar_Icon -- Builds and alloc a radar icon from a shape file                         *
 *   Handle_Team -- Processes team selection command.                                          *
 *   Handle_View -- Either records or restores the tactical view.                              *
 *   KN_To_Facing -- Converts a keyboard input number into a facing value.                     *
 *   Keyboard_Process -- Processes the tactical map input codes.                               *
 *   Language_Name -- Build filename for current language.                                     *
 *   List_Copy -- Makes a copy of a cell offset list.                                          *
 *   Main_Game -- Main game startup routine.                                                   *
 *   Main_Loop -- This is the main game loop (as a single loop).                               *
 *   Map_Edit_Loop -- a mini-main loop for map edit mode only                                  *
 *   Message_Input -- allows inter-player message input processing                             *
 *   MixFileHandler -- Handles VQ file access.                                                 *
 *   Name_From_Source -- retrieves the name for the given SourceType                           *
 *   Owner_From_Name -- Convert an owner name into a bitfield.                                 *
 *   Play_Movie -- Plays a VQ movie.                                                           *
 *   Shake_The_Screen -- Dispatcher that shakes the screen.                                    *
 *   Shape_Dimensions -- Determine the minimum rectangle for the shape.                        *
 *   Source_From_Name -- Converts ASCII name into SourceType.                                  *
 *   Sync_Delay -- Forces the game into a 15 FPS rate.                                         *
 *   Theater_From_Name -- Converts ASCII name into a theater number.                           *
 *   Unselect_All -- Causes all selected objects to become unselected.                         *
 *   VQ_Call_Back -- Maintenance callback used for VQ movies.                                  *
 *   Game_Registry_Key -- Returns pointer to string containing the registry subkey for the game.
 *   Is_Counterstrike_Installed -- Function to determine the availability of the CS expansion.
 *   Is_Aftermath_Installed -- Function to determine the availability of the AM expansion.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"

#include "conquer.h"

#include "_keyboar.h"
#include "_map.h"
#include "_palette.h"
#include "_rules.h"
#include "_source.h"
#include "_surface.h"
#include "_tactica.h"
#include "_theater.h"
#include "_timer.h"	// RA2: needed by the functions below
#include "_tooltip.h"
#include "_WSProto.h"
#include "beacon.h"	// RA2: needed by the functions below
#include "cctooltip.h"
#include "data.h"
#include "dbgprint.h"
#include "dsaudio.h"
#include "except.h"
#include "gamedlg.h"
#include "globals.h"
#include "houstype.h"
#include "incdec.h"
#include "init.h"
#include "internet.h"
#include "ipxmgr.h"
#include "keyboard.h"
#include "language\language.h"
#include "logic.h"
#include "mainloop.h"
#include "movie.h"
#include "mplayer.h"
#include "msgloop.h"
#include "netdlg.h"
#include "netdlg2.h"
#include "netshare.h"
#include "nulldlg.h"
#include "nullmgr.h"
#include "ownertalk.h"	// RA2: needed by the functions below
#include "progress.h"
#include "queue.h"
#include "restate.h"	// RA2: needed by the functions below
#include "rules.h"
#include "scenario.h"
#include "setpal.h"
#include "sidebar.h"
#include "sounddlg.h"
#include "stats.h"
#include "stringtable.h" // RA2: the includes this file needs
#include "movies.h"	// RA2: needed by the functions below
#include "surface.h"
#include "tactical.h"
#include "theme.h"
#include "voc.h"
#include "vox.h"
#include "wolapi.h"  // RA2: main has the WOL API headers flat in include/
#include "wonline.h"
#include "wsproto.h"
#include "wspudp.h"	// RA2: needed by the functions below

#include "special.hh"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>


/****************************************
**	Function prototypes for this module **
*****************************************/
bool Main_Loop(void);
void Message_Input(KeyNumType &input);

void Print_MP_Stats(void);

//
// Special module globals for recording and playback
//

void __fastcall Reload_Art_Set(void);	// RA2: 0x006F31B0 (txtlabel2.cpp), frees and loads again the art the end of a game replaced
void __fastcall Diplomacy_Dialog(MouseClass * map);	// RA2: 0x00634A00 (radar.cpp), shows the diplomacy dialog (resource 115) and redraws the map; a member of RadarClass once radar.cpp lands (a declaration in radar.h would unmatch base.cpp)
void __fastcall Register_Game_Start_Time(void);	// RA2: 0x0069A130, notes when the game started; the sibling of Register_Game_End_Time in internet.h (a declaration there would risk unmatching other files)
void Game_Options_Dialog(void);	// RA2: 0x004DD7D0, the options menu that Ingame_Menu_Dialog opens (dialog 181, 3002 or 255 by game type)
void Set_Type_Select_Mode(unsigned mode);	// RA2: 0x006F6780 (uicmnds.cpp), the select-by-type mode; 0 leaves it
void Audio_Service(void);	// RA2: 0x00406EA0 (audio.cpp), the sound callback, theme and speech in one routine
void Queue_Ipx_Clear(void);	// RA2: 0x00623280 (queue.cpp)
void Stamp_Their_Timing(int index);	// RA2: 0x00623300 (queue.cpp)
char * Wide_To_Narrow(wchar_t const * text);	// RA2: 0x006FA120 (wonline.cpp), the narrow text in a static buffer
wchar_t * Narrow_To_Wide(char const * narrow);	// RA2: 0x006FA180 (wonline.cpp)
unsigned int __fastcall Compute_Name_CRC(wchar_t const * name);	// RA2: 0x005BBA50, the wide form of netdlg.h's
namespace OwnerDraw { // RA2: as above
	void __fastcall WideToCharString(char * dest, wchar_t const * src, int count);	// RA2: 0x006FA0F0
}

enum { // RA2: SpecialDialogType has no enumerator for dialog 9
	SDLG_RESTATE = 9	// RA2: dialog 9 restates the mission (0x0063B090)
};

/*
**	RA2: the global packet commands IPX_Call_Back handles that the enumeration lacks.
*/
enum { // RA2: NetCommandType lacks these four global packet commands
	NET_PLACE_BEACON = 32,	// RA2: a player placed a beacon
	NET_REMOVE_BEACON = 33,	// RA2: a player removed a beacon
	NET_BEACON_TEXT = 34,	// RA2: a player typed the text of a beacon
	NET_TAUNT = 29	// RA2: a player sent a taunt
};

static inline bool Same_Name(NodeNameType * a, NodeNameType * b)	// RA2: the inline compare of two lobby names (case insensitive, as narrow text)
{
	char * name = Wide_To_Narrow(a->Name); // RA2: the lobby names are wide; they are compared as narrow text
	return(stricmp(name, Wide_To_Narrow(b->Name)) == 0); // RA2: as above
}


/// <summary>
/// Handles the in-game menu dialogs.
/// This routine is called by the main loop whenever a dialog has been requested. The
/// scenario is paused for as long as the player stays in the menus, and one menu chains
/// into the next until the player leaves them behind. In a multiplayer game the menus are
/// refused unless the player has already been defeated.
/// </summary>
void Ingame_Menu_Dialog(void)
{
	if (SpecialDialog != SDLG_NONE) {
		if (Session.Type != GAME_NORMAL) {
			if (PlayerPtr->IsToLose || PlayerPtr->IsToWin || PlayerPtr->IsToDie) {
				SpecialDialog = SDLG_NONE;
				return;
			}
			if (!_special_dialog_flag) {
				if (PlayerPtr->IsDefeated) {
					_special_dialog_flag = true;
				} else {
					SpecialDialog = SDLG_NONE;
					return;
				}
			}
		}

		Pause_Scenario();
		MouseCursor->Release_Mouse();	// RA2: the mouse is released and shown until its state is clear, then the dialogs run
		while (MouseCursor->Get_Mouse_State()) {	// RA2: as above
			MouseCursor->Show_Mouse();	// RA2: as above
		}

		while (SpecialDialog != SDLG_NONE) {
			switch (SpecialDialog) {
				case SDLG_RESTATE:	// RA2: dialog 9 restates the mission (0x0063B090)
					Restate_Mission(Scen); // RA2: as above
					if (SpecialDialog == SDLG_RESTATE) { // RA2: as above
						SpecialDialog = SDLG_NONE;
					}
					break;

				case SDLG_SPECIAL:	// RA2: dialog 8 is the diplomacy dialog of the map (0x00634A00)
					Diplomacy_Dialog(&Map); // RA2: as above
					if (SpecialDialog == SDLG_SPECIAL) { // RA2: as above
						SpecialDialog = SDLG_NONE;
					}
					break;

				case SDLG_OPTIONS:
					Game_Options_Dialog();	// RA2: 0x004DD7D0, the options menu (the abort menu below is Abort_Dialog, 0x004DDF20)
					if (SpecialDialog != SDLG_OPTIONS) {
						break;
					}
					SpecialDialog = SDLG_NONE;
					break;

				case SDLG_SETTINGS:
					GameControlsClass().Dialog();
					if (SpecialDialog != SDLG_SETTINGS) {
						break;
					}
					SpecialDialog = SDLG_OPTIONS;
					break;

				case SDLG_SOUND:
					SoundControlsClass().Dialog();
					SpecialDialog = SDLG_SETTINGS;
					break;

				case SDLG_KEYBOARD:
					Options.Hotkey_Dialog();
					SpecialDialog = SDLG_SETTINGS;
					break;

				case SDLG_ABORT:
					switch (Abort_Dialog()) {	// RA2: the abort menu returns 2 (leave), 3 (leave and destruct), 4 (destruct) or 5 (restart)

						case 3:
							OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::DESTRUCT));	// RA2: queues the event inline, then leaves like case 2
							_special_dialog_flag = false;
						case 2:
							Queue_Exit();
							break;

						case 4: // RA2: the abort menu's third answer queues the destruct event without leaving
							OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::DESTRUCT));	// RA2: as above
							_special_dialog_flag = false;
							break;

						case 5: // RA2: its fourth answer restarts the mission
							PlayerRestarts = true;
							break;

					}
					SpecialDialog = SDLG_NONE;
					break;

				case SDLG_SURRENDER:
					if (ObserverMode) {	// RA2: an observer just leaves
						Queue_Exit();
					} else if (!PlayerPtr->IsDefeated && !PlayerPtr->IsToWin && !PlayerPtr->IsToLose && !PlayerPtr->IsToDie && Surrender_Dialog(StringTable::LoadString("TXT_SURRENDER", NULL, "D:\\Ra2\\Conquer.CPP", 487))) {	// RA2: the text is a string table entry, line 487 of the source
						OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::DESTRUCT));	// RA2: no Flag_To_Lose branch for single player
						_special_dialog_flag = false;
					}
					SpecialDialog = SDLG_NONE;
					break;

				case SDLG_WOL_OPTIONS:
					DoFindPage();
					SpecialDialog = SDLG_NONE;
					break;

				default:
					break;
			}
		}

		UpdateWindow(MainWindow);	// RA2: after the dialogs the window is redrawn, input is locked again if the scenario wants it, and the mouse is taken back
		if (Scen->IsInputLocked) {	// RA2: as above
			MouseCursor->Hide_Mouse();	// RA2: as above
		}
		MouseCursor->Capture_Mouse();	// RA2: as above
		Session.Resume();	// RA2: as above

		Resume_Scenario();
	}
}


/***********************************************************************************************
 * Main_Game -- Main game startup routine.                                                     *
 *                                                                                             *
 *    This is the first official routine of the game. It handles game initialization and       *
 *    the main game loop control.                                                              *
 *                                                                                             *
 *    Initialization:                                                                          *
 *    - Init_Game handles one-time-only inits                                                  *
 *    - Select_Game is responsible for initializations required for each new game played       *
 *      (these may be different depending on whether a multiplayer game is selected, and       *
 *      other parameters)                                                                      *
 *    - This routine performs any un-inits required, both for each game played, and one-time   *
 *                                                                                             *
 * INPUT:   argc  -- Number of command line arguments (including program name itself).         *
 *                                                                                             *
 *          argv  -- Array of command line argument pointers.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Main_Game(int argc, char * argv[])
{
	static bool fade = true;

	/*
	**	Perform one-time-only initializations
	*/
	int ret = Init_Game(argc, argv);
	if (ret) {
		if (ret < 0) {
			char text[5120];	// RA2: the message text and caption are converted to ANSI into buffers of 5120 and 2048 bytes
			char caption[2048];	// RA2: as above
			MSGBOXPARAMS params;
			WideCharToMultiByte(CP_ACP, 0, StringTable::LoadString("TXT_INITGAME_FAILED", NULL, "D:\\Ra2\\Conquer.CPP", 562), -1, text, sizeof(text), NULL, NULL);	// RA2: the text is a string table entry, line 562 of the source
			WideCharToMultiByte(CP_ACP, 0, StringTable::LoadString("TXT_SHORT_TITLE", NULL, "D:\\Ra2\\Conquer.CPP", 563), -1, caption, sizeof(caption), NULL, NULL);	// RA2: as above, line 563
			params.cbSize = sizeof(MSGBOXPARAMS);
			params.hwndOwner = MainWindow;
			params.hInstance = ProgramInstance;
			params.lpszText = text;	// RA2: the text points at the converted buffer
			params.lpszCaption = caption;	// RA2: as above
			params.dwStyle = (MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_TOPMOST);
			params.lpszIcon = NULL;
			params.dwContextHelpId = NULL;
			params.lpfnMsgBoxCallback = NULL;
			params.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
			MessageBoxIndirect(&params);
		}
		return;
	}

	__asm {
		mov ExceptionReturnStack, esp
		mov ExceptionReturnFrame, ebp
		lea eax, ret_addr
		mov ExceptionReturnAddress, eax
	}

ret_addr:
	DebugString("ExceptionReturnStack = %08X\n", ExceptionReturnStack);
	DebugString("ExceptionReturnAddress = %08X\n", ExceptionReturnAddress);

	/*
	**	Game processing loop:
	**	1) Select which game to play, or whether to exit (don't fade the palette
	**		on the first game selection, but fade it in on subsequent calls)
	**	2) Invoke either the main-loop routine, or the editor-loop routine,
	**		until they indicate that the user wants to exit the scenario.
	*/
	while (Select_Game(fade)) {
		fade = false;
		ScenarioInit = 0;		// Kludge.
		fade = true;

		/*
		**	Make the game screen visible, clear the keyboard buffer of spurious
		**	values, and then show the mouse.  This PRESUMES that Select_Game() has
		**	told the map to draw itself.
		*/
		Set_Palette(GamePalette, FADE_PALETTE_MEDIUM, NULL);
		Keyboard->Clear();
		/*
		**	Only show the mouse if we're not playing back a recording.
		*/
		if (Session.Play) {
			MouseCursor->Hide_Mouse();	// RA2: virtual call on MouseCursor, not the Hide_Mouse helper
			TeamEvent = 0;
			TeamNumber = 0;
		}

		if (Session.Type == GAME_INTERNET) {
			Register_Game_Start_Time();
			GameStatisticsPacketSent = false;
			PacketLater = NULL;
			ConnectionLost = false;
		}

		if (ToolTips != NULL) {
			ToolTips->Activate(Options.ToolTips);
		}

		IgnoreInput = Scen->IsInputLocked;	// RA2: no Show_Mouse after it

		SpecialDialog = SDLG_NONE;
		_special_dialog_flag = true;
		Session.Resume();	// RA2: the session resumes before the game loop starts

		/*
		**	Call the game's main loop
		*/
		for (;;) {	// RA2: no editor loop; the loop ends when the game is over, which Process_Game_End decides
			if (Main_Loop()) {	// RA2: as above
				if (Process_Game_End()) {	// RA2: as above
					break;
				}
			}

			/*
			**	If the SpecialDialog flag is set, invoke the given special dialog.
			**	This must be done outside the main loop, since the dialog will call
			**	Main_Loop(), allowing the game to run in the background.
			*/
			Ingame_Menu_Dialog();
			if (Process_Game_End()) {	// RA2: the end of the game is also checked after the dialog
				break;
			}
		}
		Reload_Art_Set();	// RA2: replaces Stop_Ingame_Movie, the tooltip deactivation and Hide_Mouse
		Session.Leave_Game();	// RA2: as above
		ScenarioActive = false;
		TacticalActive = false;

		DebugString("Game loop finished. Average FPS = %d\n", SecondsPassed ? TotalFrames / SecondsPassed : 0);

		Print_MP_Stats();

		/*
		**	Scenario is done; fade palette to black
		*/
		if (!HicolorFlag) Set_Palette(BlackPalette, FADE_PALETTE_SLOW, NULL);
		//VisiblePage.Clear();
		Title_Screen_Restore(true);

		/*
		**	Un-initialize whatever needs it, for each game played.
		**
		**	Shut down either the modem or network; they'll get re-initialized if
		**	the user selections those options again in Select_Game().  This
		**	"re-boots" the modem & network code, which I currently feel is safer
		**	than just letting it hang around.
		** (Skip this step if we're in playback mode; the modem or net won't have
		**	been initialized in that case.)
		*/
		if (Session.Record || Session.Play) {
			Session.RecordFile.Close();
		}

		if (Session.Type == GAME_NULL_MODEM || Session.Type == GAME_MODEM) {
			if (!Session.Play) {
				Modem_Signoff();
			}
		} else {
			if (Session.Type == GAME_IPX) {
				if (!Session.Play) {
					Shutdown_Network();
				}
			}
		}

		if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET || Session.Type == GAME_NULL_MODEM || Session.Type == GAME_MODEM) {	// RA2: the chat settings are written for every networked game
			if (!Session.Play) {	// RA2: as above
				Session.Write_Chat_Settings();	// RA2: as above
			}
		}

		/*
		**	If we're playing back, the mouse will be hidden; show it.
		**	Also, set all variables back to normal, to return to the main menu.
		*/
		if (Session.Play) {
			Session.Type = GAME_NORMAL;
			Session.Play = 0;
			MouseCursor->Show_Mouse();	// RA2: virtual call on MouseCursor
		}
	}

	if (MouseCursor != NULL) {
		MouseCursor->Release_Mouse();
	}

	/*
	**	Free the scenario description buffers
	*/
	//Session.Free_Scenario_Descriptions();
}


/// <summary>
/// Handles the modem maintenance for a serial game.
/// This routine services the null modem connection and, while a scenario is being read in,
/// picks up the loading progress the other player reports so that the progress bar can
/// follow along.
/// </summary>
void Serial_Call_Back(void)	// RA2: not static; RA2 keeps an out-of-line copy before Call_Back, which inlines the same text
{
	int packetlen;
	SerialPacketType packet;

	/*
	**	Serial game maintenance.
	*/
	if (Session.Type == GAME_NULL_MODEM || Session.ModemService) {
		NullModem.Service();
		if (Scen->IsReadingScenario) {
			while (NullModem.Get_Message(&packet, &packetlen) > 0) {
				if (packet.Command == SERIAL_PROGRESS_REPORT && packet.ID != Session.ModemType) {
					Progress.Set_Progress_Percent(1, packet.Progress.Percent);
				}
			}
		}
	}
}


/***********************************************************************************************
 * Call_Back -- Main game maintenance callback routine.                                        *
 *                                                                                             *
 *    This routine handles all the "real time" processing that needs to                        *
 *    occur. This includes palette fading and sound updating. It needs                         *
 *    to be called as often as possible.                                                       *
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
void Call_Back(void)
{
	/*
	**	Music and speech maintenance
	*/
	Audio_Service();	// RA2: one audio routine replaces the focus test, the sound callback, the theme and the speech
	if (IngameVQ.Count() > 0) {	// RA2: the movie playing in the game gets its waiting call
		MovieHandle * movie = IngameVQ[0];	// RA2: as above
		if (movie != NULL) {	// RA2: as above
			movie->Waiting();	// RA2: as above
		}
	}

	/*
	 * Network game maintenance.
	 */
	if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
		IPX_Call_Back();
	}

	/*
	**	Serial game maintenance.
	*/
	if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		Serial_Call_Back();
	}

	/*
	 * Internet game maintenance.
	 */
	if (Session.Type == GAME_INTERNET) {
		static int _last_pump_time = 0;
		if (!g_ConnectionLost && time(0) != _last_pump_time) {	// RA2: the pump also stops once the connection is lost
			if (g_pChat != NULL && !g_SuspendChatPump) {
				g_pChat->PumpMessages();
			}
			if (g_pNetUtil != NULL) {
				g_pNetUtil->PumpMessages();
			}
			_last_pump_time = time(0);
		}
	}
}


/// <summary>
/// Handles the game maintenance while the random map generator is working.
/// The generator calls this routine through its callback pointer so that sound, music, and
/// network traffic keep flowing while a map is being built.
/// </summary>
/// <returns>bool; Should the map generation be abandoned?</returns>
bool MapGen_Call_Back(void)
{
	Call_Back();
	return(false);
}


/// <summary>
/// Handles the network maintenance for a network game.
/// This routine services the network connection and deals with the global packets that
/// have arrived -- players signing off, chat messages, kick proposals, and the loading
/// progress the other machines report. It needs to be called as often as possible.
/// </summary>
void IPX_Call_Back(void)
{
	static CDTimerClass<SystemTimerClass> _probe_timer(ProbeDelay); // RA2: a static timer paces the port probes of an internet game (restarted with ProbeDelay)

	Windows_Message_Handler();

	Ipx.Service();

	/*
	**	Read packets only if the game is "closed", so we don't steal global
	**	messages from the connection dialogs.
	*/
	if (Session.Type == GAME_INTERNET && !Session.NetOpen) { // RA2: an internet game first sends NET_PORT_PROBE packets to the other players when the timer ran out or a probe is due
		if ((_probe_timer == 0 || Session.PortProbeDue) && Session.Players.Count()) { // RA2: as above
			Windows_Message_Handler();
			Ipx.Service();
			if (Ipx.Global_Num_Send() == 0) { // RA2: as above
				GlobalPacketType packet; // RA2: as above
				memset(&packet, 0, 0x5b); // RA2: as above
				packet.Command = NET_PORT_PROBE; // RA2: as above
				OwnerDraw::WideToCharString(packet.Name, Session.Players[0]->Name, -1); // RA2: as above
				Queue_Ipx_Clear(); // RA2: as above
				DebugString("Sending NET_PORT_PROBE packets in Call_Back\n"); // RA2: as above

				for (int i = 1; i < Session.Players.Count(); i++) { // RA2: as above
					unsigned short port = Session.PlayerPorts[i]; // RA2: as above
					if (port) { // RA2: as above
						DebugString("Sending to %s\n", Wide_To_Narrow(Session.Players[i]->Name)); // RA2: as above
						Ipx.Send_Global_Message(&packet, 0x5b, 0, &Session.Players[i]->Address, port, 0); // RA2: as above
						Windows_Message_Handler();
						Ipx.Service();

						for (int index = 0; index < 8; index++) { // RA2: as above
							if (Same_Name(Session.Slots[index], Session.Players[i])) { // RA2: as above
								if (TickCount - Session.PlayerLastTime[index] > 2400) { // RA2: as above
									DebugString("No PORT_PROBE from %s in the last 40 seconds\n", Wide_To_Narrow(Session.Players[i]->Name)); // RA2: as above
									if (Session.Players[i]->Slot.Port != port && Session.Players[i]->Slot.Port >= 0x400) { // RA2: as above
										DebugString("Sending to %s on mangled port from pserver\n", Wide_To_Narrow(Session.Players[i]->Name)); // RA2: as above
										Ipx.Send_Global_Message(&packet, 0x5b, 0, &Session.Players[i]->Address, Session.Players[i]->Slot.Port, 0); // RA2: as above
									}
								}
								break;
							}
						}
					}
				}

				CDTimerClass<SystemTimerClass> timer(120); // RA2: as above
				while (Ipx.Global_Num_Send() > 0 && timer > 0) { // RA2: as above
					Windows_Message_Handler();
					Ipx.Service();
				}

				_probe_timer = ProbeDelay; // RA2: as above
				Session.PortProbeDue = false; // RA2: as above
				Ipx.Set_Global_Routed(); // RA2: as above
			}
		}
	}

	if (!Session.NetOpen) {
		while (Ipx.Get_Global_Message(&Session.GPacket, &Session.GPacketlen, &Session.GAddress, &Session.GProductID)) { // RA2: the same loop, written without the space before the arguments

			if (Session.GProductID == IPXGlobalConnClass::COMMAND_AND_CONQUER3) { // RA2: RA2's product id is 0xAA03

				switch (Session.GPacket.Command) { // RA2: the brace is on the same line

					case NET_PROPOSE_KICK:
					{
						Kick_Packet_Received(Session.GPacket, Session.GAddress);
						break;
					}

					case NET_IM_STILL_HERE: // RA2: a keep-alive packet of a router connection (0x27) stamps that connection's timing
					{
						if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) { // RA2: as above
							for (int i = 0; i < Ipx.Num_Connections(); i++) {
								if (Ipx.Connection_ID(i) == Session.GPacket.Chat.ID) { // RA2: as above
									DebugString("Got NET_IM_STILL_HERE packet from %s.\n", Wide_To_Narrow(Ipx.Connection_Name(Session.GPacket.Chat.ID))); // RA2: as above
									Stamp_Their_Timing(i); // RA2: as above
									break;
								}
							}
						}
						break;
					}

					case NET_PORT_PROBE: // RA2: as above
					{
						DebugString("Got NET_PORT_PROBE packet from %s - port = %d\n", Session.GPacket.Name, Session.GAddress.Get_Port()); // RA2: as above
						for (int i = 0; i < Session.Players.Count(); i++) {
							if (Session.Players[i]->Address.Is_Same_Node(Session.GAddress) && stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name) == 0) { // RA2: as above
								for (int index = 0; index < 8; index++) { // RA2: as above
									if (Same_Name(Session.Slots[index], Session.Players[i])) { // RA2: as above
										Session.PlayerLastTime[index] = TickCount; // RA2: as above
										break;
									}
								}
								unsigned short port = Session.GAddress.Get_Port(); // RA2: as above
								if (port != Session.Players[i]->Address.Get_Port() || port != Session.PlayerPorts[i]) { // RA2: as above
									DebugString("Strange port number in PORT_PROBE packet\n"); // RA2: as above
									int old = Session.PlayerPorts[i]; // RA2: as above
									if (port == 0) { // RA2: as above
										port = 1234; // RA2: as above
									}
									Session.PlayerPorts[i] = port; // RA2: as above
									Session.Players[i]->Address.Set_Port(port); // RA2: as above
									DebugString("Adjusting port number for player %s from %d to %d\n", Session.GPacket.Name, old, port); // RA2: as above
								}
							}
						}
						break;
					}

					case NET_PLACE_BEACON: // RA2: as above
					{
						BeaconManager.Place_Beacon(Session.GPacket.Beacon.House, Coord(Session.GPacket.Beacon.X, Session.GPacket.Beacon.Y, Session.GPacket.Beacon.Z), Session.GPacket.Beacon.ID); // RA2: as above
						break;
					}

					case NET_REMOVE_BEACON: // RA2: as above
					{
						BeaconManager.Remove_Beacon(Session.GPacket.Beacon.House, Session.GPacket.Beacon.ID); // RA2: as above
						break;
					}

					case NET_BEACON_TEXT: // RA2: as above
					{
						BeaconManager.EditBeaconMessage(Session.GPacket.BeaconText.Text, Session.GPacket.BeaconText.House, Session.GPacket.BeaconText.ID, false); // RA2: as above
						break;
					}

					/*
					**	If this is another player signing off, remove the connection &
					**	mark that player's house as non-human, so the computer will take
					**	it over.
					*/
					case NET_SIGN_OFF:
					{
						for (int i = 0; i < Ipx.Num_Connections(); i++) {

							int id = Ipx.Connection_ID(i);

							if (Session.GAddress == (*Ipx.Connection_Address(id))) {
								Destroy_Connection(id, 0);
							}
						}
						break;
					}

					/*
					**	Process a message from another user.
					*/
					case NET_PORT_UNREACHABLE: // RA2: a peer's packets did not arrive (0x2F); the UDP transport learns the peer's address
					{
						DebugString("Got NET_PORT_UNREACHABLE message from %s\n", Session.GPacket.Name); // RA2: as above
						if (Session.Type == GAME_INTERNET && PacketTransport != NULL) { // RA2: as above
							NetNumType net;
							NetNodeType node;
							unsigned short port; // RA2: as above
							IPXAddressClass * address = OwnerTalk.Address_From_Name(Session.GPacket.Name); // RA2: as above
							address->Get_Address(net, node, &port); // RA2: as above
							((UDPInterfaceClass *)PacketTransport)->Add_Peer(node, port); // RA2: as above
						}
						break;
					}

					case NET_MESSAGE:
					{
						bool msg_ok = false;

						/*
						**	If NetProtect is set, make sure this message came from within
						**	this game.
						*/
						if (!Session.NetProtect) {
							msg_ok = true;
						} else {
							if (Session.GPacket.Message.NameCRC == Compute_Name_CRC(Session.GameName)) { // RA2: the game name is wide, so the wide CRC is used
								msg_ok = true;
							} else {
								msg_ok = false;
							}
						}
						if (msg_ok && Session.ViaPacketRouter) { // RA2: behind the packet router a message only counts for the players in its recipient mask
							msg_ok = (Session.GPacket.Message.Recipients & OwnerTalk.Mask_From_ID(Session.Players[0]->Player.ID)) != 0; // RA2: as above
						}

						if (msg_ok) {
							wchar_t * name = Narrow_To_Wide(Session.GPacket.Name); // RA2: the sender's name is converted to wide text
							Session.Messages.Add_Message(name, // RA2: as above
								Session.GPacket.Message.Color,
								(wchar_t *)Session.GPacket.Message.Buf, // RA2: the message text is wide
								Session.Color_Index_To_Scheme(Session.GPacket.Message.Color),
								(TextPrintType)(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW),
								int(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: Add_Message takes the single player flag; no Concat_Message try first

							VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, NULL); // RA2: the message sound plays through VocClass::PlayGlobal

							/*
							**	Tell the map to do a partial update (just to force the messages
							**	to redraw).
							*/
							Map.Flag_To_Redraw(GS_REDRAW_ALL);

							/*
							**	Save this message in our last-message buffer
							*/
							wcscpy(Session.LastMessage, (wchar_t *)Session.GPacket.Message.Buf); // RA2: the last message is wide text
						}
						break;
					}

					case NET_TAUNT: // RA2: as above
					{
						bool msg_ok = false;
						if (!Session.NetProtect) {
							msg_ok = true;
						} else {
							if (Session.GPacket.Taunt.NameCRC == Compute_Name_CRC(Session.GameName)) { // RA2: as above
								msg_ok = true;
							} else {
								msg_ok = false;
							}
						}
						if (msg_ok && Session.ViaPacketRouter) { // RA2: as above
							msg_ok = (Session.GPacket.Taunt.Recipients & OwnerTalk.Mask_From_ID(Session.Players[0]->Player.ID)) != 0; // RA2: as above
						}
						if (msg_ok) {
							if ((Session.LANTaunts && Session.Type == GAME_IPX) || (Session.WOLTaunts && Session.Type == GAME_INTERNET)) { // RA2: as above
								Play_Taunt(Session.GPacket.Taunt.ID); // RA2: as above
							}
						}
						break;
					}

					case NET_PROGRESS_REPORT:
					{
						DebugString("Processing NET_PROGRESS_REPORT\n"); // RA2: the report is logged, then matched to its player by node address and name
						for (int i = 0; i < Session.Players.Count(); i++) {
							if (Session.Players[i]->Address.Is_Same_Node(Session.GAddress) && stricmp(Wide_To_Narrow(Session.Players[i]->Name), Session.GPacket.Name) == 0) { // RA2: the sender is found by node address and name, not by address alone
								Progress.Set_Progress_Percent(i, Session.GPacket.Progress.Percent, Point2D(-1, -1)); // RA2: as above
								break;
							}
						}
						break;
					}

					case NET_REQ_SCENARIO:
					{
						break;
					}

					case NET_READY_TO_GO:
					{
						break;
					}

					default:
						Process_Global_Packet(&Session.GPacket, &Session.GAddress);
						break;
				}
			}

			Windows_Message_Handler();
			Ipx.Service();
		}
	}
}


/***********************************************************************************************
 * Source_From_Name -- Converts ASCII name into SourceType.                                    *
 *                                                                                             *
 *    This routine is used to convert an ASCII name representing a                             *
 *    SourceType into the actual SourceType value. Typically, this is                          *
 *    used when processing the scenario INI file.                                              *
 *                                                                                             *
 * INPUT:   name  -- The ASCII source name to process.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the SourceType represented by the name                                *
 *          specified.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SourceType Source_From_Name(char const * name)
{
	if (name) {
		for (SourceType source = SOURCE_FIRST; source < SOURCE_COUNT; source++) {
			if (stricmp(SourceName[source], name) == 0) {
				return(source);
			}
		}
	}
	return(SOURCE_NONE);
}


/***********************************************************************************************
 * Name_From_Source -- retrieves the name for the given SourceType                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      source      SourceType to get the name for                                             *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      name of SourceType                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1994 BR : Created.                                                                  *
 *=============================================================================================*/
char const * Name_From_Source(SourceType source)
{
	if ((unsigned)source < SOURCE_COUNT) {
		return(SourceName[source]);
	}
	return("None");
}


/***********************************************************************************************
 * Theater_From_Name -- Converts ASCII name into a theater number.                             *
 *                                                                                             *
 *    This routine converts an ASCII representation of a theater and converts it into a        *
 *    matching theater number. If no match was found, then THEATER_NONE is returned.           *
 *                                                                                             *
 * INPUT:   name  -- Pointer to ASCII name to convert.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the name converted into a theater number.                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TheaterType Theater_From_Name(char const * name)
{
	TheaterType	index;

	//if (name) {
		for (index = THEATER_FIRST; index < THEATER_COUNT; index++) {
			if (stricmp(name, Theaters[index].Name) == 0) {
				return(index);
			}
		}
	//}
	return(THEATER_NONE);
}


/***********************************************************************************************
 * KN_To_Facing -- Converts a keyboard input number into a facing value.                       *
 *                                                                                             *
 *    This routine determine which compass direction is represented by the keyboard value      *
 *    provided. It is used for map scrolling and other directional control operations from     *
 *    the keyboard.                                                                            *
 *                                                                                             *
 * INPUT:   input -- The KN number to convert.                                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the facing type that the keyboard number represents. If it could      *
 *          not be translated, then FACING_NONE is returned.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
FacingType KN_To_Facing(int input)
{
	input &= ~(KN_ALT_BIT|KN_SHIFT_BIT|KN_CTRL_BIT);
	switch (input) {
		case KN_LEFT:
			return(FACING_W);

		case KN_RIGHT:
			return(FACING_E);

		case KN_UP:
			return(FACING_N);

		case KN_DOWN:
			return(FACING_S);

		case KN_UPLEFT:
			return(FACING_NW);

		case KN_UPRIGHT:
			return(FACING_NE);

		case KN_DOWNLEFT:
			return(FACING_SW);

		case KN_DOWNRIGHT:
			return(FACING_SE);

		default:
			break;
	}
	return(FACING_NONE);
}

#ifdef _DEBUG
/***************************************************************************
 * Map_Edit_Loop -- a mini-main loop for map edit mode only                *
 *                                                                         *
 * INPUT:                                                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/19/1994 BR : Created.                                              *
 *=========================================================================*/
bool Map_Edit_Loop(void)
{
	if (!Debug_Map) return false;

	/*
	**	Redraw the map.
	*/
	Map.Render();

	if (GameActive) {

		/*
		**	Update the display, unless we're inside a dialog.
		*/
		if (SpecialDialog == SDLG_NONE && GameInFocus) {

			Map.Flag_To_Redraw();

			/*
			**	Get user input (keys, mouse clicks).
			*/
			KeyNumType input;
			int x;
			int y;

			MouseCursor->Erase_Mouse(HiddenSurface, true);

			Map.Input(input, x, y);

			/*
			**	Process keypress.
			*/
			if (input != KN_NONE) {

				/*
				**
				*/
				int scroll_distance = 21;
				bool scroll = false;

				if (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2)) {
					scroll_distance = scroll_distance * 2.5f;
					scroll = true;
				}

				if (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2)) {
					if (Map.MapRect.Width <= Map.MapRect.Height) {
						scroll_distance = Map.MapRect.Height * CELL_LEPTON;
					} else {
						scroll_distance = Map.MapRect.Width * CELL_LEPTON;
					}
					scroll = true;
				}

				if (scroll) {
					if (input == KN_E_LEFT) {
						Map.Scroll_Map(FACING_W, scroll_distance, true);
					}
					if (input == KN_E_RIGHT) {
						Map.Scroll_Map(FACING_E, scroll_distance, true);
					}
					if (input == KN_E_UP) {
						Map.Scroll_Map(FACING_N, scroll_distance, true);
					}
					if (input == KN_E_DOWN) {
						Map.Scroll_Map(FACING_S, scroll_distance, true);
					}
					input = KN_NONE;
				}

				switch (input) {
					case KN_ESC:
					case KN_SPACE:
						SpecialDialog = SDLG_OPTIONS;
						break;

					/*
					 * Block use of the arrow keys, the interfere with the gadgets.
					 */
					case KN_LEFT:
					case KN_RIGHT:
					case KN_UP:
					case KN_DOWN:
						break;

					default:
						Keyboard_Process(input);
						break;
				};
			}

			/// ::Frame++;

			Map.Render();
			TacticalMap->AI();
		}

	}

	Call_Back();								// maintains Theme.AI() for music
//	Color_Cycle();

	Sleep(1);

	return(!GameActive);
}


/// <summary>
/// Resizes the tactical view to suit the map editor.
/// This routine reallocates the drawing surfaces and hands the map its new view dimensions,
/// so that the tactical display either takes over the whole screen for editing, or shrinks
/// back to leave room for the sidebar and the tab strip.
/// </summary>
/// <param name="flag">Should the tactical view expand to fill the entire screen?</param>
static void Resize_Tactical_View(bool flag)
{
	static int _tab_height = 16;
	static int _sidebar_width = SidebarClass::SIDE_WIDTH;

	if (flag) {

		Rect hidden(0, 0, Options.ScreenWidth-_sidebar_width, Options.ScreenHeight);
		Rect comp(0, 0, Options.ScreenWidth, Options.ScreenHeight);
		Rect tile(0, 0, Options.ScreenWidth, Options.ScreenHeight);
		Rect sidebar(0, 0, _sidebar_width, Options.ScreenHeight);
		Allocate_Surfaces(hidden, comp, tile, sidebar);

		Rect view(0, 0, Options.ScreenWidth, Options.ScreenHeight);
		Map.Set_View_Dimensions(view);

		Sleep(2);

	} else {

		Rect hidden(0, 0, Options.ScreenWidth-_sidebar_width, Options.ScreenHeight);
		Rect comp(0, 0, Options.ScreenWidth-_sidebar_width, Options.ScreenHeight);
		Rect tile(0, 0, Options.ScreenWidth-_sidebar_width, Options.ScreenHeight);
		Rect sidebar(0, 0, _sidebar_width, Options.ScreenHeight);
		Allocate_Surfaces(hidden, comp, tile, sidebar);

		Rect view(0, _tab_height, Options.ScreenWidth-_sidebar_width, Options.ScreenHeight-_tab_height);
		Map.Set_View_Dimensions(view);

		Sleep(2);
	}
}


/***************************************************************************
 * Go_Editor -- Enables/disables the map editor                            *
 *                                                                         *
 * INPUT:                                                                  *
 *      flag      true = go into editor mode; false = go into game mode    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/19/1994 BR : Created.                                              *
 *=========================================================================*/
void Go_Editor(bool flag)
{
	static bool _input_locked = false;

	Keyboard->Clear();

	/*
	 * Toggle the tactical view area.
	 */
	Resize_Tactical_View(flag);

	/*
	**	Go into Scenario Editor mode
	*/
	if (flag) {
		Debug_Map = true;
		Debug_Unshroud = true;

		/*
		 * Added otherwise scrolling does not work
		 */
		_input_locked = IgnoreInput;
		IgnoreInput = false;
		Scen->IsInputLocked = false;

		AllowVoice = false;

		/*
		**	Un-select any selected objects
		*/
		Unselect_All();
		Map.Abort_Drag_Select();

		/*
		**	Turn off the sidebar if it's on
		*/
		Map.Activate(0);

		/*
		**	Reset the map's Button list for the new mode
		*/
		Map.Init_IO();

		/*
		**	Force a complete redraw of the screen
		*/
		HiddenSurface->Fill(0);
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		Map.Render();

	} else {

		/*
		**	Go into normal game mode
		*/
		Debug_Map = false;
		Debug_Unshroud = false;

		/*
		 * Added otherwise scrolling does not restore after editor switch.
		 */
		IgnoreInput = _input_locked;
		Scen->IsInputLocked = _input_locked;

		AllowVoice = true;

		/*
		**	Un-select any selected objects
		*/
		Unselect_All();
		Map.Abort_Drag_Select();

		/*
		**	Reset the map's Button list for the new mode
		*/
		Map.Init_IO();

		/*
		**	Force a complete redraw of the screen
		*/
		HiddenSurface->Fill(0);
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		Map.Render();
	}

	Update_Visible_Surface(true, HiddenSurface);
}

#endif


/***********************************************************************************************
 * Unselect_All -- Causes all selected objects to become unselected.                           *
 *                                                                                             *
 *    This routine will unselect all objects that are currently selected.                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void Unselect_All(void)
{
	while (CurrentObject.Count()) {
		CurrentObject[0]->Unselect();
	}
	Set_Type_Select_Mode(0);	// RA2: also leaves the select-by-type mode
	BeaconManager.Clear_Selection();	// RA2: and stops editing the selected beacon
}


/***********************************************************************************************
 * Fetch_Techno_Type -- Convert type and ID into TechnoTypeClass pointer.                      *
 *                                                                                             *
 *    This routine will convert the supplied RTTI type number and the ID value into a valid    *
 *    TechnoTypeClass pointer. If there is an error in conversion, then NULL is returned.      *
 *                                                                                             *
 * INPUT:   type  -- RTTI type of the techno class object.                                     *
 *                                                                                             *
 *          id    -- Integer representation of the techno sub type number.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the techno type class object specified or NULL if the    *
 *          conversion could not occur.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/08/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
TechnoTypeClass const * Fetch_Techno_Type(RTTIType type, int id)
{
	switch (type) {
		case RTTI_UNITTYPE:
		case RTTI_UNIT:
			return(TechnoTypeClass *)(UnitTypes[id]);

		case RTTI_BUILDINGTYPE:
		case RTTI_BUILDING:
			return(TechnoTypeClass *)(BuildingTypes[id]);

		case RTTI_INFANTRYTYPE:
		case RTTI_INFANTRY:
			return(TechnoTypeClass *)(InfantryTypes[id]);

		case RTTI_AIRCRAFTTYPE:
		case RTTI_AIRCRAFT:
			return(TechnoTypeClass *)(AircraftTypes[id]);

		default:
			break;
	}
	return(NULL);
}


/***************************************************************************
 * DISK_SPACE_AVAILABLE -- returns bytes of free disk space                *
 *                                                                         *
 * INPUT:      none                                                        *
 *                                                                         *
 * OUTPUT:     returns amount of free disk space                           *
 *                                                                         *
 * HISTORY:                                                                *
 *   08/11/1995 PWG : Created.                                             *
 *=========================================================================*/
unsigned int Disk_Space_Available(void)
{
	ULARGE_INTEGER freebytecount;		// Free bytes on disk available to caller (caller may not have access to entire disk).
	ULARGE_INTEGER totalbytecount;		// Total bytes on disk.
	ULARGE_INTEGER totalfreebytecount;
	unsigned int diskspace;

	/// This pointer is declared as returning bool, where the API it is bound to returns BOOL.
	bool (__stdcall *getfreediskspaceex) (LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

	DebugString("Checking available disk space\n");

	// Get the free disk space on the drive.
	// NOTE IML: For Win'95, must query for support for GetDiskFreeSpaceEx before using it - otherwise use GetDiskFreeSpace().
	HINSTANCE kernel = GetModuleHandle("KERNEL32.DLL");
	if (kernel != NULL) {
		getfreediskspaceex = (bool (_stdcall*) (LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER)) GetProcAddress (kernel, "GetDiskFreeSpaceExA");
		if (getfreediskspaceex != NULL) {

			DebugString("Using GetDiskFreeSpaceEx\n");

			// NOTE: This function uses GetDiskFreeSpaceEx() and therefore assumes Win '95 OSR2 or greater.
			if (!getfreediskspaceex(NULL, &freebytecount, &totalbytecount, &totalfreebytecount)) {
				DebugString("GetDiskFreeSpaceEx failed with error code %d - %s\n", GetLastError(), Last_Error_Text());
			} else {
				/// Convert to a 32-bit integer.
				diskspace = int(((int)freebytecount.LowPart + ((int)freebytecount.HighPart * (double)((__int64)UINT_MAX + 1))) / (double)1024);
				DebugString("Free disk space is %d Mb\n", diskspace / 1024);
				return(diskspace);
			}
		} else {
			DebugString("GetProcAddress failed with error code %d - %s\n", GetLastError(), Last_Error_Text());
		}
	} else {
		DebugString("Failed to get module handle for KERNEL32.DLL\n");
	}

	DWORD sectorspercluster, bytespersector, freeclustercount, totalclustercount;

	// The Ex version is not available. Use the Win'95 version.
	// QUESTION: SDK docs say that values returned by this function are erroneous if partition > 2Gb.
	//				 Does that mean that the partition is guaranteed to be <= 2Gb if Ex is not available?

	if (GetDiskFreeSpace(NULL, &sectorspercluster, &bytespersector, &freeclustercount, &totalclustercount)) {
		diskspace = ((sectorspercluster * bytespersector) / 1024) * freeclustercount;
		DebugString("Free disk space is %d Mb\n", diskspace / 1024);
		return(diskspace);
	}

	return(0);
}


/***********************************************************************************************
 * Crate_From_Name -- Given a crate name convert it to a crate type.                           *
 *                                                                                             *
 *    Use this routine to convert an ASCII crate name into a crate type. If no match could     *
 *    be found, then CRATE_MONEY is assumed.                                                   *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the crate name text to convert into a crate type.              *
 *                                                                                             *
 * OUTPUT:  Returns with the crate name converted into a crate type.                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
CrateType Crate_From_Name(char const * name)
{
	if (name != NULL) {
		for (CrateType crate = CRATE_FIRST; crate < CRATE_COUNT; crate++) {
			if (stricmp(name, CrateNames[crate]) == 0) return(crate);
		}
	}
	return(CRATE_MONEY);
}


/***********************************************************************************************
 * Owner_From_Name -- Convert an owner name into a bitfield.                                   *
 *                                                                                             *
 *    This will take an owner specification and convert it into a bitfield that represents     *
 *    it. Sometimes this will be just a single house bit, but other times it could be          *
 *    all the allies or soviet house bits combined.                                            *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text to convert into a house bitfield.                     *
 *                                                                                             *
 * OUTPUT:  Returns with the houses specified. The value is in the form of a bit field with    *
 *          one bit per house type.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
int Owner_From_Name(char const * text)
{
	int ownable = 0;
	HousesType h = HouseTypeClass::From_Name(text);
	if (h != HOUSE_NONE) {
		ownable |= (1 << h);
	}
	return(ownable);
}


/***********************************************************************************************
 * Shake_The_Screen -- Dispatcher that shakes the screen.                                      *
 *                                                                                             *
 *    This routine will shake the game screen the number of shakes requested.                  *
 *                                                                                             *
 * INPUT:   shakes   -- The number of shakes to shake the screen.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 BWG : Created.                                                                 *
 *=============================================================================================*/
void Shake_The_Screen(int shakes)
{
	//
}


/***********************************************************************************************
 * List_Copy -- Makes a copy of a cell offset list.                                            *
 *                                                                                             *
 *    This routine will make a copy of a cell offset list. It will only copy the significant   *
 *    elements of the list limited by the maximum length specified.                            *
 *                                                                                             *
 * INPUT:   source   -- Pointer to a cell offset list.                                         *
 *                                                                                             *
 *          len      -- The maximum number of cell offset elements to store in to the          *
 *                      destination list pointer.                                              *
 *                                                                                             *
 *          dest     -- Pointer to the destination list to store the copy into.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Ensure that the destination list is large enough to hold the list copy.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void List_Copy(Cell const * source, int len, Cell * dest)
{
	if (source == NULL || dest == NULL) {
		return;
	}

	while (len > 0) {
		*dest = *source;
		if (*dest == REFRESH_EOL) return;
		dest++;
		source++;
		len--;
	}

	/// Terminate the list.
	*(dest-1) = REFRESH_EOL;
}


/// <summary>
/// Converts an ASCII name into a VQType.
/// This routine is used when processing the movie names found in the scenario INI file.
/// </summary>
/// <param name="name">Pointer to the ASCII movie name to convert.</param>
/// <returns>Returns with the movie that matches the name. VQ_NONE is returned if there is
/// no match.</returns>
VQType VQ_From_Name(char const * name)
{
	if (name != NULL && strcmpi("<none>", name)) {
		for (int movie = 0; movie < Movies.Count(); movie++) {
			if (stricmp(name, Movies[movie]) == 0) return(VQType(movie));
		}
	}
	return(VQ_NONE);
}


/// <summary>
/// Converts an ASCII name into a LandType.
/// This routine is used when processing the terrain land type specified in an INI file.
/// </summary>
/// <param name="name">Pointer to the ASCII name to convert.</param>
/// <returns>Returns with the land type that matches the name. LAND_NONE is returned if
/// there is no match.</returns>
LandType Land_From_Name(char const * name)
{
	if (strcmpi("<none>", name) && name != NULL) {
		for (LandType land = LAND_FIRST; land < LAND_COUNT; land++) {
			if (stricmp(LandName[land], name) == 0) return(land);
		}
	}
	return(LAND_NONE);
}


/// <summary>
/// Converts a land type into its ASCII name.
/// This routine is used when writing a terrain land type back out to an INI file.
/// </summary>
/// <param name="land">The land type to fetch the name of.</param>
/// <returns>Returns with a pointer to the name of the land type. An illegal land type
/// fetches the "none" name instead.</returns>
char const * Name_From_Land(LandType land)
{
	if ((unsigned)land < LAND_COUNT) {
		return(LandName[land]);
	}
	return("<none>");
}


/// <summary>
/// Converts an ASCII name into a SpeedType.
/// This routine is used when processing the movement speed specified in an INI file.
/// </summary>
/// <param name="name">Pointer to the ASCII name to convert.</param>
/// <returns>Returns with the speed type that matches the name. SPEED_NONE is returned if
/// there is no match.</returns>
SpeedType Speed_From_Name(char const * name)
{
	if (name != NULL) {
		for (SpeedType speed = SPEED_FIRST; speed < SPEED_COUNT; speed++) {
			if (stricmp(SpeedName[speed], name) == 0) return(speed);
		}
	}
	return(SPEED_NONE);
}


/// <summary>
/// Converts a speed type into its ASCII name.
/// This routine is used when writing a movement speed back out to an INI file.
/// </summary>
/// <param name="speed">The speed type to fetch the name of.</param>
/// <returns>Returns with a pointer to the name of the speed type. An illegal speed type
/// fetches the "none" name instead.</returns>
char const * Name_From_Speed(SpeedType speed)
{
	if ((unsigned)speed < SPEED_COUNT) {
		return(SpeedName[speed]);
	}
	return("<none>");
}


/// <summary>
/// Converts an ASCII name into a LayerType.
/// This routine is used when processing the layer specified in an INI file.
/// </summary>
/// <param name="name">Pointer to the ASCII name to convert.</param>
/// <returns>Returns with the layer type that matches the name. LAYER_NONE is returned if
/// there is no match.</returns>
LayerType Layer_From_Name(char const * name) // RA2: TibSun has no layer names; same shape as Speed_From_Name, table at 0x007D38C8
{
	if (name != NULL) {	// RA2: the loop only runs for a non-NULL name
		for (LayerType layer = LAYER_FIRST; layer < LAYER_COUNT; layer++) {	// RA2: five layers, walked through the table at 0x007D38C8
			if (stricmp(LayerName[layer], name) == 0) return(layer); // RA2: as above
		}
	}
	return(LAYER_NONE); // RA2: as above
}


/// <summary>
/// Converts a layer type into its ASCII name.
/// This routine is used when writing a layer back out to an INI file.
/// </summary>
/// <param name="layer">The layer type to fetch the name of.</param>
/// <returns>Returns with a pointer to the name of the layer type. An illegal layer type
/// fetches the "<none>" name instead.</returns>
char const * Name_From_Layer(LayerType layer) // RA2: same shape as Name_From_Speed over the layer table
{
	if ((unsigned)layer < LAYER_COUNT) {	// RA2: unsigned compare against 5
		return(LayerName[layer]); // RA2: as above
	}
	return("<none>");
}


/// <summary>
/// Writes the multiplayer connection statistics out to a text file.
/// Use this routine to dump the round trip times, resends, stalls, and packet loss that
/// were recorded for each player. Only an internet game is reported upon.
/// </summary>
void Print_MP_Stats(void)
{
	if (Session.Type == GAME_INTERNET) {
		FILE *file = fopen("mpstats.txt", "wt");
		if (file != NULL) {
			fprintf(file, "Frames: %d\n", Frame);

			fprintf(file, "Average FPS: %d\n", SecondsPassed != 0 ? TotalFrames / SecondsPassed : 0);
			fprintf(file, "Max MaxAhead: %d\n", Session.MaxMaxAhead);
			fprintf(file, "Latency setting: %d\n", Session.LatencyFudge);
			fprintf(file, "Game speed setting: %d\n", Options.GameSpeed);

			if (PacketTransport != NULL) {
				for (int i = 0; i < PacketTransport->Get_Num_Local_Addresses(); i++) {
					unsigned char *addr = PacketTransport->Get_Local_Address(i);
					if (addr != NULL) {
						unsigned int a = *(unsigned int *)addr;
						fprintf(file, "Local address: %d.%d.%d.%d\n", (a) & 0xFF, (a >> 8) & 0xFF, (a >> 16) & 0xFF, (a >> 24) & 0xFF);
					}
				}
			}

			for (int i = 0; i < MAX_PLAYERS; i++) {

				MPStatsType &stats = Session.ConnectionStats[i];;
				if (stats.Name[0] == '\0') {
					continue;
				}

				fprintf(file, "\nName: %s\n", stats.Name);
				fprintf(file, "Address: %s\n", stats.Address.As_String(0));	// RA2: the address prints as text
				fprintf(file, "Max avg round trip: %d\n", stats.MaxAvgRoundTrip);
				fprintf(file, "Max round trip: %d\n", stats.MaxRoundTrip);
				fprintf(file, "Resends: %d\n", stats.Resends);
				fprintf(file, "Frame sync stalls: %d\n", stats.FrameSyncStalls);
				fprintf(file, "Command cound stalls: %d\n", stats.CommandCountStalls);
				fprintf(file, "Lost: %d\n", stats.Lost);
				fprintf(file, "Percent lost: %d\n", stats.PercentLost);
			}

			fclose(file);
		}
	}
}
