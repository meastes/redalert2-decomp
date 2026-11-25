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

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "mainloop.h"

#include "_bench.h"
#include "_command.h"
#include "_logic.h"
#include "_map.h"
#include "_palette.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "_timer.h"
#include "_xmouse.h"
#include "bench.h"
#include "command.h"
#include "conquer.h"
#include "data.h"
#include "debug.h"
#include "dialog.h"
#include "dsaudio.h"
#include "dsurface.h"
#include "fog.h"
#include "globals.h"
#include "goptions.h"
#include "internet.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "logic.h"
#include "misc.h"
#include "mpscore.h"
#include "msgbox.h"
#include "msgloop.h"
#include "mstimer.h"
#include "netdlg.h"
#include "nullmgr.h"
#include "pcx.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "stats.h"
#include "stimer.h"
#include "surface.h"
#include "tactical.h"
#include "theme.h"
#include "timer.h"
#include "tracker.h"
#include "wonline.h"

#include "bench.hh"
#include "special.hh"

namespace CopyProtection { bool CheckProtectedData(void); }
namespace FPSCounter { extern unsigned CurrentFrameRate; extern unsigned TotalFramesElapsed; extern unsigned TotalTimeElapsed; }

#include "dbgprint.h" // RA2: DebugString
#include "_tooltip.h" // RA2: ToolTips
#include "cctooltip.h" // RA2: CCToolTip
#include "saveload.h" // RA2: Save_Game
#include "_keyboar.h" // RA2: Keyboard
#include "audio.h" // RA2: AudioDeviceAvailable
#include "stringtable.h" // RA2: StringTable::LoadString
#include "beacon.h" // RA2: the beacon messages
#include "txtlabel.h" // RA2: the edit label
#include "houstype.h" // RA2: IsMultiplay
#include "connect.h" // RA2: PacketRouterConnection
#include "multiplayer.h" // RA2: the game mode's virtual slots
#include "ownertalk.h" // RA2: the packet router
#include "_WSProto.h" // RA2: PacketTransport
#include "winstub.h"

#define RA2_SOURCE "D:\\Ra2\\MainLoop.CPP" // RA2: the file name RA2 passes to StringTable::LoadString
unsigned int __fastcall Compute_Name_CRC(wchar_t const * name); // RA2: 0x005BBA50, the wide form of netdlg.h's
namespace OwnerDraw { void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); } // RA2: wide to narrow text, as modemgst.cpp declares it
wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x006FA180, a static buffer
char * __fastcall Wide_To_Narrow(wchar_t const * wide); // RA2: 0x006FA120, a static buffer
int __fastcall Parse_Page_Command(wchar_t const * text); // RA2: 0x00746480
int __fastcall Parse_Online_Command(wchar_t const * text); // RA2: 0x00746240
int __fastcall Reply_To_Last_Pager(wchar_t const * text); // RA2: 0x007466C0
void __fastcall Reload_Art_Set(void); // RA2: 0x006F31B0; frees and loads again the art the end of a game replaced
void __fastcall Send_Port_Unreachable(void); // RA2: 0x0062E840
void __fastcall Planning_Update(void); // RA2: 0x00614010; the planning mode's per frame update
void __fastcall Planning_Free_Nodes(void); // RA2: 0x006142F0; deletes the planning items
static inline const CommandClass * Hotkey_Command(KeyNumType key) // RA2: Keyboard_Process looks the second key up through an inline copy of its own
{
	return HotkeyCommands[key];
}
enum {
	SCROLL_UP = 0x0001, // RA2: bits of ScrollKeys
	SCROLL_DOWN = 0x0010,
	SCROLL_LEFT = 0x0100,
	SCROLL_RIGHT = 0x1000
};


//
// Special module globals for recording and playback
//
int TeamEvent = 0;			// 0 = no event, 1,2,3 = team event type
int TeamNumber = 0;			// which team was selected? (1-9)

void Message_Input(KeyNumType &input);
void Sync_Delay(void);
void Draw_Multiplayer_Debug(bool noframecheck); // RA2: renamed after its address
static void Do_Record_Playback(void);














/// <summary>
/// RA2: tells whether Main_Loop is running.
/// </summary>
bool Is_In_Main_Loop(void) // RA2 has this routine (0x0053F460): it reads the flag Main_Loop sets
{
	return InMainLoop;
}


/// <summary>
/// RA2: marks Main_Loop as running.
/// </summary>
void Enter_Main_Loop(void) // RA2 has this routine (0x0053F470): it sets the flag Main_Loop sets
{
	InMainLoop = true;
}


/// <summary>
/// RA2: marks Main_Loop as not running.
/// </summary>
void Leave_Main_Loop(void) // RA2 has this routine (0x0053F480): it clears the flag
{
	InMainLoop = false;
}


/// <summary>
/// Captures the visible screen to a movie sequence.
/// This routine grabs the client area of the game window into an off screen surface
/// each time it is called, and when the sequence is full it writes the whole thing out
/// as numbered PCX files. It is a debugging aid and does nothing unless motion capture
/// has been switched on.
/// </summary>
/// <remarks>Call this routine once per frame -- one call captures one frame of the
/// sequence.</remarks>
void Motion_Capture(void)
{
	if (Debug_MotionCapture) {
		static Surface ** _array = NULL;
		static int _sequence = 0;
		static int _seqsize = Rule->MovieTime * 900.0; // RA2: the product is a double

		if (_array == NULL) {
			_array = new Surface * [_seqsize];
			memset(_array, '\0', _seqsize * sizeof(Surface*));
		}

		if (_array == NULL) {
			Debug_MotionCapture = false;
		}

		RECT client_rect;

		GetClientRect(MainWindow, &client_rect);

		POINT screen_pt;

		screen_pt.x = client_rect.left;
		screen_pt.y = client_rect.top;
		if (!ClientToScreen(MainWindow, &screen_pt)) {
			return;
		}

		POINT screen_pt2;
		screen_pt2.x = client_rect.right;
		screen_pt2.y = client_rect.bottom;
		if (!ClientToScreen(MainWindow, &screen_pt2)) {
			return;
		}

		Rect rect(screen_pt.x, screen_pt.y, client_rect.right, client_rect.bottom);

		/// Compensate for screen shake
		if (Map.ScreenX != 0 || Map.ScreenY != 0) {
			if (Map.ScreenX > 0) {
				rect.X += Map.ScreenX;
				rect.Width -= Map.ScreenX;
			} else if (Map.ScreenX < 0) {
				rect.Width += Map.ScreenX;
			}
			if (Map.ScreenY > 0) {
				rect.Y += Map.ScreenY;
				rect.Height -= Map.ScreenY;
			} else if (Map.ScreenY < 0) {
				rect.Height += Map.ScreenY;
			}
		}

		if (_sequence < _seqsize) {
			if (_array[_sequence] == NULL) {
				_array[_sequence] = new BSurface(rect.Width, rect.Height, 2);
			}

			if (_array[_sequence] != NULL) {
				_array[_sequence]->Blit_From(Rect(0, 0, rect.Width, rect.Height), *VisibleSurface, rect);
			}
			_sequence++;

		} else {
			Debug_MotionCapture = false;

			DSurface temp_page(rect.Width, rect.Height);

			for (int index = 0; index < _sequence; index++) {
				char filename[30];
				sprintf(filename, "cap%04d.pcx", index);
				CCFileClass file(filename);
				temp_page.Blit_From(*_array[index], false, true);
				Write_PCX_File(file, temp_page, & GamePalette);
			}

			_sequence = 0;
		}
	}
}





/***********************************************************************************************
 * Do_Record_Playback -- handles saving/loading map pos & current object                       *
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
 *   08/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
static inline void Do_Record_Playback()
{
	int count;
	int tgt;
	int i;
	Point2D coord;
	ObjectClass * obj;
	unsigned int sum;
	unsigned int sum2;
	unsigned int ltgt;

	if (Session.Record) {
		coord = TacticalMap->Get_Tactical_Position();
		Session.RecordFile.Write(&coord, sizeof(coord));

		count = CurrentObject.Count();
		Session.RecordFile.Write(&count, sizeof(count));

		sum = 0;
		for (i = 0; i < count; i++) {
			ltgt = TargetClass(CurrentObject[i]).Encode();
			sum += ltgt;
		}
		Session.RecordFile.Write(&sum, sizeof(sum));

		for (i = 0; i < count; i++) {
			tgt = TargetClass(CurrentObject[i]).Encode();
			Session.RecordFile.Write(&tgt, sizeof(tgt));
		}

		Session.RecordFile.Write(&TeamEvent, sizeof(TeamEvent));
		Session.RecordFile.Write(&TeamNumber, sizeof(TeamNumber));
		TeamEvent = 0;
		TeamNumber = 0;
	}

	if (Session.Play) {
		if (Session.RecordFile.Read(&coord, sizeof(coord)) == sizeof(coord)) {
			TacticalMap->Set_Tactical_Position(coord);
		}

		if (Session.RecordFile.Read(&count, sizeof(count)) == sizeof(count)) {
			sum = 0;
			for (i = 0; i < CurrentObject.Count(); i++) {
				ltgt = TargetClass(CurrentObject[i]).Encode();
				sum += ltgt;
			}

			Session.RecordFile.Read(&sum2, sizeof(sum2));
			if (sum2 != sum) {
				Unselect_All();
			}

			AllowVoice = true;

			for (i = 0; i < count; i++) {
				if (Session.RecordFile.Read(&tgt, sizeof(tgt)) == sizeof(tgt)) {
					TargetClass tmp;
					tmp.Decode(tgt);
					obj = tmp.As_Object();
					if (obj && (sum2 != sum)) {
						obj->Select();
						AllowVoice = false;
					}
				}
			}

			AllowVoice = true;
		}

		Session.RecordFile.Read(&TeamEvent, sizeof(TeamEvent));
		Session.RecordFile.Read(&TeamNumber, sizeof(TeamNumber));

		Map.Flag_To_Redraw();
		Map.Render();
	}
}


bool InMainLoop = false;

/// <summary>
/// Handles the game losing the input focus.
/// This routine parks the main loop while another application holds the focus, pumping
/// the Windows message queue so that the game can be restored. A network game cannot
/// afford to stall, so it pumps the queue once and lets play carry on regardless.
/// RA2: leaves an internet game that goes through the packet router. It tells the router
/// to shut down, waits up to 480 ticks for it, and then reports the port as unreachable
/// unless the game mode says it handled the departure itself.
/// </summary>
void Disconnect_Gracefully(void) // RA2 0x0053F780: leaves an internet game through the packet router; its own debug text names it
{
	DebugString("Disconnect_Gracefully\n"); // RA2 0x0053F780: leaves an internet game through the packet router; its own debug text names it
	if (Session.Type == GAME_INTERNET && PacketTransport != NULL && Session.ViaPacketRouter) {
		if (PacketRouterConnection != NULL) {
			OwnerTalk.Send_Shutdown();
			CDTimerClass<SystemTimerClass> timer(480);
			while (timer()) {
				Call_Back();
				if (OwnerTalk.Wait_For_Router_Shutdown(10, false)) {
					break;
				}
			}
		} // RA2 0x0053F780: leaves an internet game through the packet router; its own debug text names it
		if (Session.MPGameMode == NULL || !Session.MPGameMode->vt_entry_08()) {
			Send_Port_Unreachable();
		}
	}
}


/// <summary>
/// RA2: the part of Main_Loop that acts on the win, lose, restart and abort flags: checks
/// the protected data every 1024 frames, then, with the game suspended, reports the end of
/// the game and runs the matching scenario routine.
/// </summary>
bool Process_Game_End(void) // RA2 0x0053F840: the win, lose, restart and abort handling is a function of its own, not part of Main_Loop
{
	if ((Frame & 0x3FF) == 0 && !CopyProtection::CheckProtectedData()) {
		OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::DESTRUCT));
	}
	if (PlayerWins || PlayerLoses || PlayerRestarts || PlayerAborts) {
		Session.Suspended++;
		Lock_Scenario_Input();
		if (PlayerWins || PlayerLoses) {
			if (ToolTips != NULL) {
				ToolTips->Activate(false);
			}
		}
		if (PlayerWins || PlayerLoses) {
			if (Session.MPGameMode != NULL && (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET)) {
				Session.MPGameMode->vt_entry_88(PlayerWins);
			}
		}
		if (PlayerWins) {
			if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
				if (WestwoodOnline_Tournament) {
					Session.SawGameCompletion = true;
				}
				Register_Game_End_Time();
				Send_Statistics_Packet(false);
			}
			PlayerLoses = false;
			PlayerWins = false;
			PlayerRestarts = false;
			PlayerAborts = false;
			Reload_Art_Set();
			Session.Leave_Game();
			Do_Win();
			Session.Resume();
			InMainLoop = false;
			if (ToolTips != NULL) {
				ToolTips->Activate(Options.ToolTips);
			}
		} else if (PlayerLoses) {
			if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
				if (WestwoodOnline_Tournament) {
					Session.SawGameCompletion = true;
				}
				Register_Game_End_Time();
				Send_Statistics_Packet(false);
			}
			PlayerWins = false;
			PlayerLoses = false;
			PlayerRestarts = false;
			PlayerAborts = false;
			Reload_Art_Set();
			Session.Leave_Game();
			Do_Lose();
			Session.Resume();
			InMainLoop = false;
			if (ToolTips != NULL) {
				ToolTips->Activate(Options.ToolTips);
			}
		} else if (PlayerRestarts) {
			PlayerWins = false;
			PlayerLoses = false;
			PlayerRestarts = false;
			PlayerAborts = false;
			Session.Leave_Game();
			Do_Restart();
			Session.Resume();
			InMainLoop = false;
		} else if (PlayerAborts) {
			PlayerWins = false;
			PlayerLoses = false;
			PlayerRestarts = false;
			PlayerAborts = false;
			Reload_Art_Set();
			Session.Leave_Game();
			Do_Abort();
			Session.Resume();
			InMainLoop = false;
			Disconnect_Gracefully();
		}
		Session.Suspended--;
	}
	return(!GameActive);
}


/***********************************************************************************************
 * Main_Loop -- This is the main game loop (as a single loop).                                 *
 *                                                                                             *
 *    This function will perform one game loop.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Should the game end?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/01/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Main_Loop(void)
{
	KeyNumType	input;					// Player input.
	int x;
	int y;

	//Mono_Set_Cursor(0,0);
	if (!GameActive) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		return(!GameActive);
	}

	InMainLoop = true;

	while (!GameInFocus) {
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Sleep(500);
			Windows_Message_Handler();
		} else {
			Sleep(10);
			Windows_Message_Handler();
			break;
		}
	}

	/*
	**	Sync-bug trapping code
	*/
	if (Frame >= Session.TrapFrame) {
		Session.Trap_Object();
	}

	//
	// Initialize our AI processing timer
	//
	Session.ProcessTimer = timeGetTime();/// TickCount;

	if (Session.TrapCheckHeap) {
		Debug_Trap_Check_Heap = true;
	}

	if (TacticalActive == true && AudioDeviceAvailable()) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		ThemeType theme = Theme.Score;
		if (theme == THEME_NONE) {
			theme = Theme.Pending;
		}
		if (Options.IsInGameMusic) {
			if (theme == THEME_NONE) {
				Theme.Queue_Song(THEME_PICK_ANOTHER);
			} // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		} else if (theme != THEME_QUIET) {
			Theme.Stop(true);
			Theme.Queue_Song(THEME_QUIET);
		}
	}

	/*
	**	Setup the timer so that the Main_Loop function processes at the correct rate.
	*/
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		if (Session.Play) {
			FrameTimer = 0;
		} else {
			if (Session.DesiredFrameRate == 0) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
				FrameTimer = 2;
				NetFrameTimer = 33;
			} else {
				FrameTimer = TIMER_SECOND / Session.DesiredFrameRate;
				NetFrameTimer = 1000 / Session.DesiredFrameRate;
			}
			LastNetFrameTimer = NetFrameTimer;
			int maxahead = Session.MaxAhead;
			int worst_latency = 0;
			if (Session.Type == GAME_INTERNET) {
				if (maxahead > 30) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
					for (int i = 0; i < Ipx.Num_Connections(); i++) {
						if (worst_latency <= Session.PlayerLatency[i]) {
							worst_latency = Session.PlayerLatency[i];
						}
					}
					if (worst_latency) {
						if (worst_latency >= maxahead / 4) {
							NetFrameTimer = NetFrameTimer + 10;
						}
						if (worst_latency >= maxahead / 2) {
							NetFrameTimer = NetFrameTimer + 10;
						}
						if (worst_latency >= (3 * maxahead) / 4) {
							NetFrameTimer = NetFrameTimer + 10;
						}
					}
				}
			}
		} // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
	} else if (Session.Type == GAME_NORMAL && !GameControlsShown) {
		Options.GameSpeed = 2;
		FrameTimer = 2;
	} else {
		FrameTimer = Options.GameSpeed;
	}

	/*
	**	Update the display, unless we're inside a dialog.
	*/
	if (Session.Type == GAME_NORMAL) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		if (Scen->IsGamePaused && Scen->PauseTimer <= 0) {
			Scen->IsGamePaused = false;
			Release_Scenario();
		}
		if (Scen->HoldCount) {
			Windows_Message_Handler();
			Call_Back();
			TacticalMap->AI();
			Map.Render();
			Sync_Delay();
			InMainLoop = false;
			return(!GameActive);
		}
	}

	if (!Session.Play) {
		if (SpecialDialog == SDLG_NONE && GameInFocus) {
			Map.Input(input, x, y);
			Keyboard_Process(input);
			if (Session.EnableMultiplayerDebug) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
				Draw_Multiplayer_Debug(false);
			}
			if ((Frame & 7) == 7 && Session.Type == GAME_INTERNET) {
				Ipx.Store_Stats();
			}
			Update_Fogged_Objects();
			Map.Render();
		}
	}

	drag_select_aborted = false;

	/*
	**	Save map's position & selected objects, if we're recording the game.
	*/
	if (Session.Record || Session.Play) {
		Do_Record_Playback();
	}

	/*
	**	Sort the map's ground layer by y-coordinate value.  This is done
	**	outside the IsToRedraw check, for the purposes of game sync'ing
	**	between machines; this way, all machines will sort the Map's
	**	layer in the same way, and any processing done that's based on
	**	the order of this layer will remain in sync.
	*/
	DisplayClass::Layer[LAYER_GROUND].Sort();

	/*
	**	AI logic operations are performed here.
	*/
	if (Session.Type == GAME_NORMAL && !ScenarioSaveNameSet) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		wchar_t description[56];
		char name[261];
		ScenarioSaveNameSet = true;
		strcpy(name, Scen->ScenarioName);
		name[260] = '\0';
		char * dot = strrchr(name, '.');
		if (dot != NULL) {
			*dot = '\0';
		}
		strcat(name, ".sav");
		swprintf(description, L"[%s]", Scen->Description);
		Save_Game(name, description, false);
	}

	Logic.AI();

	/*
	**	Manage the inter-player message list.  If Manage() returns true, it means
	**	a message has expired & been removed, and the entire map must be updated.
	*/
	int scroll = ScrollSpeed; // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
	if (Keyboard->Down(Options.KeySelect1) || Keyboard->Down(Options.KeySelect2)) {
		scroll = scroll * ScrollBoost;
	} else if (Keyboard->Down(Options.KeyForceAttack1) || Keyboard->Down(Options.KeyForceAttack2)) {
		scroll = MapSizeX > MapSizeY ? MapSizeX : MapSizeY;
		scroll <<= 8;
	}
	if (ScrollKeys & SCROLL_LEFT) {
		Map.Scroll_Map(FACING_W, scroll, true);
	}
	if (ScrollKeys & SCROLL_RIGHT) {
		Map.Scroll_Map(FACING_E, scroll, true);
	}
	if (ScrollKeys & SCROLL_UP) {
		Map.Scroll_Map(FACING_N, scroll, true);
	}
	if (ScrollKeys & SCROLL_DOWN) {
		Map.Scroll_Map(FACING_S, scroll, true);
	}

	Planning_Free_Nodes();
	Session.Messages.Manage();

	//
	// Measure how long it took to process the AI
	//
	Session.ProcessTicks += min<int>(1000, (timeGetTime() - Session.ProcessTimer)); /// (TickCount - Session.ProcessTimer)
	Session.ProcessFrames++;

	/*
	**	Process all commands that are ready to be processed.
	*/
	Queue_AI();
	Planning_Free_Nodes(); // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
	Call_Back();

	if (!PlayerWins && !PlayerLoses && !PlayerRestarts && !PlayerAborts) { // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		Frame++;
		if (UnlockInputFrame != 0 && UnlockInputFrame < Frame) {
			Unlock_Scenario_Input();
			UnlockInputFrame = 0; // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		}
		Sync_Delay(); // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
		Process_Deferred_Deletion();
		Planning_Update();
		InMainLoop = false;
		return(!GameActive);
	}

	InMainLoop = false; // RA2 0x0053FBD0: the loop without the bench calls and without the end of game handling (Process_Game_End)
	return(true);
}



void Ingame_Menu_Dialog(void);

/***********************************************************************************************
 * Keyboard_Process -- Processes the tactical map input codes.                                 *
 *                                                                                             *
 *    This routine is used to process the input codes while the player                         *
 *    has the tactical map displayed. It handles all the keys that                             *
 *    are appropriate to that mode.                                                            *
 *                                                                                             *
 * INPUT:   input -- Input code as returned from Input_Num().                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1992 JLB : Created.                                                                 *
 *   07/04/1995 JLB : Handles team and map control hotkeys.                                    *
 *=============================================================================================*/
void Keyboard_Process(KeyNumType & input)
{
	/*
	**	Don't do anything if there is not keyboard event.
	*/
	Message_Input(input); // RA2 0x005406E0: Message_Input first, then the commands through the three extra CommandClass slots
	if (input == KN_NONE) {
		return;
	}

	/*
	**	The VK_BIT must be stripped from the "plain" value of the key so that a comparison to
	**	KN_1, for example, will yield TRUE if in fact the "1" key was pressed.
	*/
	KeyNumType key = KeyNumType(input & ~KN_RLSE_BIT); // RA2 0x005406E0: Message_Input first, then the commands through the three extra CommandClass slots
	KeyNumType plain = KeyNumType(key & ~(WWKEY_SHIFT_BIT | WWKEY_ALT_BIT | WWKEY_CTRL_BIT | WWKEY_VK_BIT));

	CommandClass const * cmd = HotkeyCommands[plain];
	if (cmd == NULL) {
		cmd = HotkeyCommands[key];
	} else if (!cmd->PreventCombinationOverride(key)) {
		cmd = Hotkey_Command(key);
	}

	if (cmd != NULL && (!IgnoreInput || _wcsicmp((wchar_t const *)cmd->Get_Category(), L"Debug") == 0)) { // RA2 0x005406E0: Message_Input first, then the commands through the three extra CommandClass slots
		if (cmd->ExtraTriggerCondition(input)) {
			cmd->Execute(input);
		}
		if (cmd->Unknown1C(input)) {
			int count = 0;
			int next = Keyboard->Check() & 0xFFFF;
			if (next == input) {
				do {
					Keyboard->Get();
					next = Keyboard->Check() & 0xFFFF;
					count++;
				} while (count < 10 && next == input);
			}
		}
		return;
	}

	if (plain == KN_ESC) { // RA2 0x005406E0: Message_Input first, then the commands through the three extra CommandClass slots
		Queue_Options();
	} else if (plain == KN_TAB) {
		Map.Zoom_Mode_Control();
	} else if (plain == KN_LEFT) {
		if (input & KN_RLSE_BIT) {
			ScrollKeys &= ~SCROLL_LEFT;
		} else {
			ScrollKeys |= SCROLL_LEFT;
		}
	} else if (plain == KN_RIGHT) { // RA2 0x005406E0: Message_Input first, then the commands through the three extra CommandClass slots
		if (input & KN_RLSE_BIT) {
			ScrollKeys &= ~SCROLL_RIGHT;
		} else {
			ScrollKeys |= SCROLL_RIGHT;
		}
	} else if (plain == KN_UP) {
		if (input & KN_RLSE_BIT) {
			ScrollKeys &= ~SCROLL_UP;
		} else {
			ScrollKeys |= SCROLL_UP;
		}
	} else if (plain == KN_DOWN) {
		if (input & KN_RLSE_BIT) {
			ScrollKeys &= ~SCROLL_DOWN;
		} else {
			ScrollKeys |= SCROLL_DOWN;
		}
	}
}



/***********************************************************************************************
 * Sync_Delay -- Forces the game into a 15 FPS rate.                                           *
 *                                                                                             *
 *    This routine will wait until the timer for the current frame has expired before          *
 *    returning. It is called at the end of every game loop in order to force the game loop    *
 *    to run at a fixed rate.                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine will delay an amount of time according to the game speed setting.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/04/1995 JLB : Created.                                                                 *
 *   03/06/1995 JLB : Fixed.                                                                   *
 *=============================================================================================*/
void Sync_Delay(void)
{
	/*
	**	Accumulate the number of 'spare' ticks that are frittered away here.
	*/
	SpareTicks += FrameTimer;
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
		while (NetFrameTimer) {
			Call_Back();
			if (SpecialDialog == SDLG_NONE && GameInFocus == true) {
				KeyNumType input = KN_NONE;
				int x, y;
				if (NetFrameTimer > 10) {
					Map.Input(input, x, y);
					Keyboard_Process(input);
					TacticalMap->AI();
					Map.Render();
				} else {
					Sleep(0);
				}
				if (!NetFrameTimer()) {
					break;
				}
			}
			Sleep(0);
		}
	} else {
		if (FrameTimer) { // RA2 0x00540960: waits on the frame timer in one loop
			Wait_Blit();
			while (FrameTimer) {
				Sleep(FrameTimer); // RA2 0x00540960: waits on the frame timer in one loop
			}
		}
	}

	static CDTimerClass<SystemTimerClass> fps_timer;
	if (!fps_timer) {
		FPSCounter::CurrentFrameRate = FramesThisSecond; // RA2 0x00540960: waits on the frame timer in one loop
		FramesThisSecond = 0;
		TotalFrames += FPSCounter::CurrentFrameRate; // RA2 0x00540960: waits on the frame timer in one loop
		SecondsPassed++;
		if (TotalFrames > 0x7FFFFFFF) {
			TotalFrames = 0;
			SecondsPassed = 0;
		}
		fps_timer = TIMER_SECOND;
	}
}



/// <summary>
/// RA2
/// </summary>
static inline Point2D * Scratch(Point2D const & p) { return (Point2D *)&p; }


/***********************************************************************************************
 * Message_Input -- allows inter-player message input processing                               *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      input      key value                                                                   *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/22/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
//#pragma off (unreferenced)
void Message_Input(KeyNumType &input)
{
	int rc;
	int i;
	int count; // RA2 0x00540C20: wide text, IME input and beacon messages
	wchar_t name[16];
	wchar_t command[64];
	wchar_t command2[64];
	wchar_t edit[162];
	wchar_t message[224];
	wchar_t prefix[112];
	wchar_t rest[224];
	wchar_t txt[224];

	/*
	**	Check keyboard input for a request to send a message.
	**	The 'to' argument for Add_Edit is prefixed to the message buffer; the
	**	message buffer is big enough for the 'to' field plus MAX_MESSAGE_LENGTH.
	**	To send the message, calling Get_Edit_Buf retrieves the buffer minus the
	**	'to' portion.  At the other end, the buffer allocated to display the
	**	message must be MAX_MESSAGE_LENGTH plus the size of "From: xxx (house)".
	*/
	int len = wcslen(IMEResultString);
	if (len) {
		KeyNumType key = KN_NONE;
		for (int k = 0; k < len; k++) {
			Session.Messages.Input(key, IMEResultString[k]);
		}
		IMEResultString[0] = 0;
	}

		/*
		**	For a serial game, send a message on F1 or F4; set 'txt' to the
		**	"Message:" string & add an editable message to the list.
		*/
	if (input == KN_NONE) {
		return;
	}

	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH && !Session.Messages.Is_Edit()) {
		if (input & KN_BUTTON) {
			return;
		}
		int ascii = (unsigned char)Keyboard->To_ASCII(input);
		if (ascii == '\r' || ascii == '\\' || ascii == '\b') {
			txt[0] = 0;
			bool beacon = BeaconManager.FindSelected(PlayerPtr->HeapID) != 0;
			if (beacon) {
				swprintf(txt, StringTable::LoadString("TXT_BEACON_MESSAGE", 0, RA2_SOURCE, 1796));
			} else {
				swprintf(txt, StringTable::LoadString("TXT_FROM", 0, RA2_SOURCE, 1798), PlayerPtr->UIName);
				if (ascii == '\r') {
					ChatTarget = 1;
				} else if (ascii == '\\') {
					ChatTarget = 3;
				} else if (ascii == '\b') {
					ChatTarget = 2;
				}
			}
			input = KN_NONE;
			int color = Session.Color_Index_To_Scheme(Session.PrefColor);
			TextLabelClass * label = Session.Messages.Add_Edit(color, TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), txt, 0, beacon ? 10000 : -1);
			if (beacon) {
				if (label) {
					label->SkipDraw = true;
				}
				Session.Messages.EnableOverflow = false;
				wcscpy(name, Narrow_To_Wide("_"));
				BeaconManager.EditBeaconMessage(name, -1, -1, false);
			} else {
				Session.Messages.EnableOverflow = true;
			}
			Map.Flag_To_Redraw();
			IMEResultString[0] = 0; // RA2 0x00540C20: wide text, IME input and beacon messages
		}
	}

	/*
	**	Process message-system input; send the message out if RETURN is hit.
	*/
	KeyNumType copy_input = input; // RA2 0x00540C20: wide text, IME input and beacon messages
	rc = Session.Messages.Input(input, 0);

	/*
	**	If a single character has been added to an edit buffer, update the display.
	*/
	if (rc == 1 && Session.Type != GAME_NORMAL) {
		Map.Flag_To_Redraw();
	} else if (rc == 2 && Session.Type != GAME_NORMAL) { // RA2 0x00540C20: wide text, IME input and beacon messages
		if (copy_input == KN_ESC) {
			Map.Flag_To_Redraw(GS_REDRAW_ALL);
			ChatTarget = 0; // RA2 0x00540C20: wide text, IME input and beacon messages
		} else {
			Map.Flag_To_Redraw();
		}
	}

	/*
	**	Send a message
	*/
	if ((BeaconManager.FindSelected(PlayerPtr->HeapID) && copy_input && rc == 1) || rc == 2 || rc == 4 || IMEComposing) { // RA2 0x00540C20: wide text, IME input and beacon messages
		if (copy_input == KN_ESC) {
			BeaconManager.EditBeaconMessage(0, -1, -1, true);
		} else {
			wcsncpy(edit, Session.Messages.Get_Edit_Buf(), 162);
			edit[161] = 0;
			wcsncat(edit, IMECompositionString, 162);
			edit[161] = 0;
			wcsncat(edit, Narrow_To_Wide("_"), 162);
			edit[161] = 0;
			BeaconManager.EditBeaconMessage(edit, -1, -1, false);
		}
	}

	if ((rc == 3 || rc == 4) && Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
		if (Session.Type == GAME_NULL_MODEM || Session.Type == GAME_MODEM) {
			SerialPacketType * packet = (SerialPacketType *)NullModem.BuildBuf;
			if (rc == 3 || rc == 4) {
				packet->Command = SERIAL_MESSAGE;
			}
			OwnerDraw::WideToCharString(packet->Name, Session.Players[0]->Name, -1);
			packet->ID = (char)Session.PrefColor;
			if (rc == 3) {
				wcscpy(packet->Message.Message, Session.Messages.Get_Edit_Buf());
			} else if (rc == 4) {
				wcscpy(packet->Message.Message, Session.Messages.Get_Overflow_Buf());
				Session.Messages.Clear_Overflow_Buf();
			}
			NullModem.Send_Message(NullModem.BuildBuf, sizeof(SerialPacketType), 1);
			if (rc == 3 || rc == 4) {
				wcscpy(Session.LastMessage, packet->Message.Message);
			}
		} else if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
			if (BeaconManager.FindSelected(PlayerPtr->HeapID)) {
				BeaconManager.EditBeaconMessage(Session.Messages.Get_Edit_Buf(), -1, -1, true);
			} else {
				if (Session.Type == GAME_INTERNET) { // RA2 0x00540C20: wide text, IME input and beacon messages
					if (rc == 3) {
						wcscpy(message, Session.Messages.Get_Edit_Buf());
					} else if (rc == 4) {
						wcscpy(message, Session.Messages.Get_Overflow_Buf());
					}
					int page = Parse_Page_Command(message);
					if (page) {
						if (page == 1) {
							int n = 0;
							wcscpy(command, StringTable::LoadString("TXT_PAGE_COMMAND", 0, RA2_SOURCE, 1984));
							wchar_t * p = &message[wcslen(command)];
							while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
								p++;
							}
							while (*p != 0 && wcsncmp(p, L" ", 1) != 0 && n <= 10) {
								name[n++] = *p++;
							}
							name[n] = 0;
							while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
								p++;
							}
							if (wcslen(name) == 0) {
								Session.Messages.Add_Message(0, -1,
									StringTable::LoadString("TXT_NO_NAME_SPECIFIED", 0, RA2_SOURCE, 2000),
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							} else if (wcslen(p) == 0) {
								Session.Messages.Add_Message(0, -1,
									StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE", 0, RA2_SOURCE, 2005),
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							} else {
								swprintf(command2, L"%s > %s", Session.Players[0]->Name, name);
								Session.Messages.Add_Message(command2, Session.PrefColor, p,
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							}
							if (rc == 4) {
								wchar_t * buf = Session.Messages.Get_Edit_Buf();
								if (buf) {
									swprintf(prefix, StringTable::LoadString("TXT_FROM", 0, RA2_SOURCE, 2024), PlayerPtr->UIName);
									swprintf(rest, L"%s%s %s", command, name, buf);
									Session.Messages.Remove_Edit();
									Session.Messages.Add_Edit(Session.PrefColor, TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), prefix, 0, -1);
									buf = Session.Messages.Get_Edit_Buf();
									if (buf) {
										wcscat(buf, rest);
									}
								}
							}
						}
						Session.Messages.Clear_Overflow_Buf();
						return; // RA2 0x00540C20: wide text, IME input and beacon messages
					}
					if (Parse_Online_Command(message)) {
						Session.Messages.Clear_Overflow_Buf();
						return;
					}
					int reply = Reply_To_Last_Pager(message);
					if (reply) {
						if (reply == 1) {
							bool shortform = false;
							wcscpy(command, StringTable::LoadString("TXT_REPLY_COMMAND", 0, RA2_SOURCE, 2054));
							wcscpy(command2, StringTable::LoadString("TXT_REPLY_COMMAND_SHORT", 0, RA2_SOURCE, 2055));
							wchar_t * p;
							if (wcsncmp(message, command, wcslen(command)) == 0) {
								p = &message[wcslen(command)];
							} else {
								p = &message[wcslen(command2)];
								shortform = true;
							}
							while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
								p++;
							}
							swprintf(prefix, L"%s > %s", Session.Players[0]->Name, Narrow_To_Wide(LastPagerName));
							if (wcslen(Narrow_To_Wide(LastPagerName)) == 0) {
								Session.Messages.Add_Message(0, Session.PrefColor,
									StringTable::LoadString("TXT_NAME_NOT_SPECIFIED", 0, RA2_SOURCE, 2069),
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							} else if (wcslen(p) > 0) {
								Session.Messages.Add_Message(prefix, Session.PrefColor, p,
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							} else {
								Session.Messages.Add_Message(0, Session.PrefColor,
									StringTable::LoadString("TXT_CANT_SEND_BLANK_PAGE", 0, RA2_SOURCE, 2082),
									Session.Color_Index_To_Scheme(Session.PrefColor), TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
							}
							if (rc == 4) {
								wchar_t * buf = Session.Messages.Get_Edit_Buf();
								if (buf) {
									swprintf(edit, StringTable::LoadString("TXT_FROM", 0, RA2_SOURCE, 2094), PlayerPtr->UIName);
									if (shortform) {
										swprintf(rest, L"%s %s", command2, buf);
									} else {
										swprintf(rest, L"%s %s", command, buf);
									}
									Session.Messages.Remove_Edit();
									Session.Messages.Add_Edit(Session.PrefColor, TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), edit, 0, -1);
									buf = Session.Messages.Get_Edit_Buf();
									if (buf) {
										wcscat(buf, rest);
									}
								}
							}
						}
						Session.Messages.Clear_Overflow_Buf();
						return;
					}
				}

			/*
			**	Send the message, and store this message in our LastMessage
			**	buffer; the computer may send us a version of it later.
			*/
				if (rc == 3 || rc == 4) { // RA2 0x00540C20: wide text, IME input and beacon messages
					Session.GPacket.Command = NET_MESSAGE;
				}
				strcpy(Session.GPacket.Name, Wide_To_Narrow(Session.Players[0]->Name));
				Session.GPacket.Message.Color = Session.PrefColor;
				Session.GPacket.Message.NameCRC = Compute_Name_CRC(Session.GameName);
				Session.GPacket.Message.Recipients = 0;
				if (rc == 3) {
					wcscpy((wchar_t *)Session.GPacket.Message.Buf, Session.Messages.Get_Edit_Buf());
				} else if (rc == 4) {
					wcscpy((wchar_t *)Session.GPacket.Message.Buf, Session.Messages.Get_Overflow_Buf());
					Session.Messages.Clear_Overflow_Buf();
				}

				count = Houses.Count();
				if (Session.MultiplayerObserver && (ChatTarget == 1 || ChatTarget == 2)) {
					for (i = 0; i < count; i++) {
						HouseClass * house = Houses[i];
						if (house && house->Class->IsMultiplay
							&& ((!Session.PlayerChatEnabled[i] && ChatTarget == 1)
								|| (!Houses[Session.Players[0]->Player.ID]->Is_Ally(house) && ChatTarget == 2))) {
							int scheme = GadgetClass::Get_Color_Scheme();
							Session.Messages.Add_Message(0, 0,
								StringTable::LoadString("TXT_OBSERVER_CANT_SEND", 0, RA2_SOURCE, 2167),
								scheme, TPF_TEXT, (int)(Rule->MessageDelay * TICKS_PER_MINUTE), true);
							ChatTarget = 0;
							break;
						}
					}
				}

			/*
			**	Network game: fill in a GlobalPacketType & send it.
			*/
				for (i = 0; i < count; i++) {
					HouseClass * house = Houses[i];
					if (house && house->Class->IsMultiplay
						&& ((Session.PlayerChatEnabled[i] && ChatTarget == 1)
							|| (Houses[Session.Players[0]->Player.ID]->Is_Ally(house) && ChatTarget == 2)
							|| ChatTarget == 3)) {
						int connections = Ipx.Num_Connections();
						for (int j = 0; j < connections; j++) {
							int id = Ipx.Connection_ID(j);
							if (wcscmp(house->UIName, Ipx.Connection_Name(id)) == 0) {
								IPXAddressClass * address = Ipx.Connection_Address(id);
								if (Session.ViaPacketRouter) {
									Session.GPacket.Message.Recipients |= OwnerTalk.Mask_From_ID(id);
								} else {
									Ipx.Send_Global_Message(&Session.GPacket, sizeof(GlobalPacketType), 1, address, 0, 0); // RA2 0x00540C20: wide text, IME input and beacon messages
									Ipx.Service();
								}
							}
						}
					}
				}

				if (Session.ViaPacketRouter && Session.GPacket.Message.Recipients) { // RA2 0x00540C20: wide text, IME input and beacon messages
					DebugString("Sending chat, mask = %d\n", (unsigned char)Session.GPacket.Message.Recipients);
					Ipx.Send_Global_Message(&Session.GPacket, sizeof(GlobalPacketType), 1, &Session.Players[0]->Address, 0, 0);
					Ipx.Service();
				}

				/*
				**	Otherwise, MessageAddress contains the exact address to send to.
				**	Send to that address only.
				*/
				wchar_t *from = Narrow_To_Wide(Session.GPacket.Name); // RA2 0x00540C20: wide text, IME input and beacon messages
				Session.Messages.Add_Message(from, Session.GPacket.Message.Color,
					(wchar_t *)Session.GPacket.Message.Buf, Session.Color_Index_To_Scheme(Session.GPacket.Message.Color),
					TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), (int)(Rule->MessageDelay * TICKS_PER_MINUTE), false);
				wcscpy(Session.LastMessage, (wchar_t *)Session.GPacket.Message.Buf);
			}
		}

		/*
		**	Tell the map to completely update itself, since a message is now missing.
		*/
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
	}

	if (rc == 3) { // RA2 0x00540C20: wide text, IME input and beacon messages
		ChatTarget = 0;
	}
} // RA2 0x00540C20: wide text, IME input and beacon messages



//#pragma on (unreferenced)


/// <summary>
/// Draws the network statistics overlay.
/// This routine prints the frame counter, frame rate, latency and processing figures
/// across the bottom of the screen, and then lets the connection manager add its own
/// per-connection display. It is used while debugging a multiplayer game and does
/// nothing at all in a single player, modem or null modem game.
/// </summary>
/// <param name="noframecheck">Should the display be drawn regardless of the frame
/// counter?</param>
void Draw_Multiplayer_Debug(bool noframecheck) // RA2 0x005419E0: TibSun has no such function
{
	wchar_t buffer[256]; // RA2 0x005419E0: TibSun has no such function

	if (DebugTime != -1) {
		double seconds = DebugTime * 0.001;
		Fancy_Text_Print(L"%.2f", *CompositeSurface, Rect(0, 0, 500, 15), Point2D(100, 0), ColorSchemes[GadgetClass::Get_Color_Scheme()], 0x1F, TextPrintType(TPF_6PT_GRAD), seconds, Scen->ScenarioName);
	}

	if (Session.DrawMPDebugStats && Session.Type != GAME_NORMAL && Session.Type != GAME_MODEM && Session.Type != GAME_NULL_MODEM) { // RA2 0x005419E0: TibSun has no such function
		Surface * surface = CompositeSurface;
		MouseCursor->Hide_Mouse();
		surface->Fill_Rect(Rect(0, 300, 639, 230), 0);

		swprintf(buffer, L"Frame : %d", Frame);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 302), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"FPS : %d", FPSCounter::CurrentFrameRate);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 312), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"MaxAhead : %d", Session.MaxAhead);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 322), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"Resp Time : %d ms", (int)(Ipx.Response_Time() * 1000) / TIMER_SECOND);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 332), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"Req fps : %d", Session.DesiredFrameRate);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 342), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"Process : %d", Session.Players[0]->Player.ProcessTime);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 352), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"Rout resp: %d", (unsigned)(Session.RouterResponse * 1000) / TIMER_SECOND);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 362), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"NFT : %d", NetFrameTimer);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 372), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		swprintf(buffer, L"LNFT : %d", LastNetFrameTimer);
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 382), Fetch_Scheme_By_Name("Grey"), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));

		Ipx.Multiplayer_Debug_Print();
		MouseCursor->Show_Mouse(); // RA2 0x005419E0: TibSun has no such function
	}
}


/***********************************************************************************************
 * Do_Record_Playback -- handles saving/loading map pos & current object                       *
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
 *   08/15/1995 BRR : Created.                                                                 *
 *=============================================================================================*/

/// <summary>
/// Handles the game losing the input focus.
/// This routine parks the main loop while another application holds the focus, pumping
/// the Windows message queue so that the game can be restored. A network game cannot
/// afford to stall, so it pumps the queue once and lets play carry on regardless.
/// </summary>
static void Check_For_Focus_Loss(void)
{
	while (!GameInFocus) {
		if (Session.Type == GAME_NORMAL || Session.Type == GAME_SKIRMISH) {
			Sleep(500);
			Windows_Message_Handler();
		} else {
			Sleep(10);
			Windows_Message_Handler();
			break;
		}
	}
	return;
}

