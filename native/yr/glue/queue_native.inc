
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/queue.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

// NATIVE: YR's Execute_DoList is FUN_0064C380 (gamemd 0x0064C380); its network arguments are
// untyped in the tree (TibSun: ConnManClass *, CDTimerClass<FrameTimerClass> *, FrameSyncStruct *).
int __fastcall FUN_0064C380(int max_houses, HousesType base_house, void * net_arg, void * skip_crc_arg, void * their_arg)
{
	ConnManClass * net = (ConnManClass *)net_arg;
	CDTimerClass<FrameTimerClass> * skip_crc = (CDTimerClass<FrameTimerClass> *)skip_crc_arg;
	FrameSyncStruct * their = (FrameSyncStruct *)their_arg;
	HousesType house;
	HouseClass *hptr;
	int i,j,k;
	int index;
	int check_crc;

	Check_Mirror();

#if (TIMING_FIX)
	//
	// If MPlayerMaxAhead is recomputed such that it increases, the systems
	// may try to free-run to the new MaxAhead value.  If so, they may miss
	// an event that was generated after the TIMING event was created, but
	// before it executed; this event will be scheduled with the older,
	// shorter MaxAhead value.  If a system doesn't receive this event, it
	// may execute past the frame it's scheduled to execute on, creating
	// a Packet-Recieved-Too-Late error.  To prevent this, find any events
	// that are scheduled to execute during this "period of vulnerability",
	// and re-schedule for the end of that period.
	//
	for (j = 0; j < DoList.Count; j++) {
		if (DoList[j].Type != EventClass::FRAMEINFO &&
			DoList[j].Frame > NewMaxAheadFrame1 &&
			DoList[j].Frame < NewMaxAheadFrame2) {
			DebugString("DoList: Moving event from frame %d to frame %d\n", DoList[j].Frame, NewMaxAheadFrame2);
			DoList[j].Frame = NewMaxAheadFrame2;
			#ifdef MIRROR_QUEUE
			MirrorList[j].Frame = NewMaxAheadFrame2;
			#endif
		}
	}
#endif

	//------------------------------------------------------------------------
	// Execute the DoList.  Events must be executed in the same order on all
	//	systems; so, execute them in the order of the HouseClass array.  This
	// array is stored in the same order on all systems.
	//------------------------------------------------------------------------
	for (i = 0; i < Houses.Count(); i++) {
		//.....................................................................
		// Convert our index into a HousesType value
		//.....................................................................
		house = (HousesType)(i);
		hptr = Houses[house];

		//.....................................................................
		// If for some reason this house doesn't exist, skip it.
		// Also, if this house has exited the game, skip it.  (The user can
		// generate events after he exits, because the exit event is scheduled
		// at least FrameSendRate*3 frames ahead.  If one system gets these
		// packets & another system doesn't, they'll go out of sync because
		// they aren't checking the CommandCount for that house, since that
		// house isn't connected any more.)
		//.....................................................................
		if (!hptr) {
			continue;
		}
		if (!hptr->IsHuman && !hptr->IsPlayerControl) {
			continue;
		}

		//.....................................................................
		// Loop through all events
		//.....................................................................
		for (j = 0; j < DoList.Count; j++) {

			if (net)
				/* NATIVE: TibSun's mono-screen debug view (Update_Queue_Mono) is not in YR */;

			//..................................................................
			// If this event was from the currently-executing player ID, and it's
			// time to execute it, execute it.
			//..................................................................
			if (DoList[j].ID == hptr->HeapID && Frame >= DoList[j].Frame &&
				!DoList[j].IsExecuted) {

				//...............................................................
				// Error if it's too late to execute this packet!
				// (Hack: disable this check for solo or skirmish mode.)
				//...............................................................
				if (Frame > DoList[j].Frame && DoList[j].Type !=
					EventClass::FRAMEINFO && Session.Type != GAME_NORMAL &&
					Session.Type != GAME_SKIRMISH) {
					Dump_Packet_Too_Late_Stuff(&DoList[j]);
					Session.Suspended++;
					WWMessageBox().Process(StringTable::LoadString("TXT_PACKET_TOO_LATE", NULL, __FILE__, __LINE__), StringTable::LoadString("TXT_OK", NULL, __FILE__, __LINE__), NULL, NULL);	// NATIVE: YR's string table
					Session.Suspended--;
					return(0);
				}

				//...............................................................
				// Only execute EXIT & OPTIONS commands if they're from myself.
				//...............................................................
				if (DoList[j].Type==EventClass::EXIT ||
						DoList[j].Type==EventClass::OPTIONS ||
						DoList[j].Type==EventClass::PAGEUSER) {

					if (DoList[j].Type==EventClass::EXIT) {
						int house_count = Houses.Count();
						/*
						**	Flag that this house lost because it quit.
						*/
						HousesType quithouse = HOUSE_NONE;
						HouseClass *quithptr = NULL;

						for (int player = 0; player < house_count ; player++) {
							quithouse = (HousesType)(player);
							quithptr = Houses[quithouse];
							if (!quithptr) {
								continue;
							}
							if (quithptr->HeapID == DoList[j].ID) {
								quithptr->IsGiverUpper = true;
								break;
							}
						}

						/*
						**	Send the game statistics packet now since the game is effectivly over
						*/
						if (CountAliveTeams(quithptr) == 1 &&
								Session.Type == GAME_INTERNET &&
								!GameStatisticsPacketSent) {
							Session.SawGameCompletion = true;
							Register_Game_End_Time();
							Send_Statistics_Packet();		// Event - player aborted, and there were only 2 left.
						}
						if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent && PlayerPtr != NULL && PlayerPtr == quithptr) {
							DebugString("Sending game results because I quit, but didn't see completion");
							Send_Statistics_Packet();
						}
					}

					if (Debug_Print_Events) {
						if (DoList[j].Type==EventClass::EXIT) {
							DebugString("Exit Event: ID:%d (%s),  Event Frame:%d,  My Frame:%d\n",
								DoList[j].ID,
								(char *)Houses[(HousesType)(DoList[j].ID)]->IniName,
								DoList[j].Frame,
								Frame);
						}
					}

					if (DoList[j].ID == PlayerPtr->HeapID) {
						DoList[j].Execute();
					} else if (DoList[j].Type==EventClass::EXIT) {
					//............................................................
					// If this EXIT event isn't from myself, destroy the connection
					//	for that player.  The HousesType for this event is the
					// connection ID.
					//............................................................
						if (Session.Type == GAME_MODEM ||
							Session.Type == GAME_NULL_MODEM) {
							Destroy_Null_Connection( house, 0 );
						} else if ((Session.Type == GAME_IPX ||
							Session.Type == GAME_INTERNET) && net) {
							index = net->Connection_Index (house);
							if (index != -1) {
								for (k = index; k < net->Num_Connections() - 1; k++) {
									their[k] = their[k+1];
								}
								Destroy_Connection(house,0);
							}
						}
						//
						// Special case for recording playback: turn the house over
						// to the computer.
						//
						if (Session.Play && DoList[j].Type==EventClass::EXIT) {
							hptr->IsHuman = false;
							hptr->IQ = Rule->MaxIQ;
							hptr->Computer_Paranoid();
							hptr->IniName = TStringID<HOUSE_NAME_MAX>("Computer");	// NATIVE: as YR's HouseClass (house.cpp)
							DebugString("Removing a player %s:%d\n", __FILE__, __LINE__);
							Session.NumPlayers--;
						}
					}
				}

				//...............................................................
				// For a FRAMEINFO event, check the CRC value.
				//...............................................................
				else if (DoList[j].Type == EventClass::FRAMEINFO) {
					//............................................................
					// Skip the CRC check if we're less than 32 frames into the game;
					// this will prevent a newly-loaded modem game from instantly
					// going out of sync, if the games were saved at different
					// frame numbers.
					//............................................................
					if (!skip_crc || *skip_crc == 0) {
						check_crc = 1;
					}
					else {
						check_crc = 0;
					}
					if (check_crc
						&& DoList[j].Frame == Frame
						&& DoList[j].Data.FrameInfo.Delay < ARRAY_SIZE(CRC)) {
						index = ((DoList[j].Frame - DoList[j].Data.FrameInfo.Delay) &
							ARRAY_SIZE(DAT_00B04474)-1);
						if (DAT_00B04474[index] != DoList[j].Data.FrameInfo.CRC) {
							Print_CRCs(&DoList[j]);
							Session.Suspended++;
							if (WWMessageBox().Process(StringTable::LoadString("TXT_OUT_OF_SYNC", NULL, __FILE__, __LINE__),
								StringTable::LoadString("TXT_CONTINUE", NULL, __FILE__, __LINE__), StringTable::LoadString("TXT_STOP", NULL, __FILE__, __LINE__), NULL) == 0) {	// NATIVE: YR's string table
								Session.Suspended--;
								if (Session.Type == GAME_MODEM ||
									Session.Type == GAME_NULL_MODEM) {
									Destroy_Null_Connection( house, -1 );
									Shutdown_Modem();
									Session.Type = GAME_NORMAL;
								}
								else if ((Session.Type == GAME_IPX ||
									Session.Type == GAME_INTERNET) && net) {
									while (net->Num_Connections()) {
										Destroy_Connection (net->Connection_ID(0), -1);
									}
								}
								Map.Flag_To_Redraw(GS_REDRAW_ALL);
							}
							else {
								Session.Suspended--;
								return(0);
							}
							return(1);
						}
					}
				}
				//...............................................................
				// Execute other commands
				//...............................................................
				else {
					DoList[j].Execute();
				}

				//...............................................................
				// Mark this event as executed.
				//...............................................................
				DoList[j].IsExecuted = 1;
				#ifdef MIRROR_QUEUE
				MirrorList[j].IsExecuted = 1;
				#endif
			}
		}
	}

	return(1);

}	// end of Execute_DoList

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): FUN_00652540 (YR 0x00652540), the event queue's Next for
// OutList, is unmatched in the YR tree: gamemd.exe drops the first event (the head moves on
// within the 128-entry ring and the count drops), as TibSun's QueueClass::Next.
// ---------------------------------------------------------------------------------------------
void __fastcall FUN_00652540(void * list)
{
	((decltype(&OutList))list)->Next();
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW2, LAN): FUN_00650A90, YR 0x00650A90 (3,161 bytes, unmatched in
// ra2-decomp; their name Game::LogFrameCRC), from gamemd.exe's code: called by Compute_Game_CRC
// each network frame with the frame's slot (Frame % 256). Normally it only draws the scenario's
// next random number (kept at gamemd.exe 0x00AFA26C, read by nothing else). With the extra sync
// logging on (DAT_00B04880: -MPDEBUG or the debug key) it records the frame for the sync dump
// (DAT_00AC6660[slot], written by FUN_006516F0 on a desync): the CRC, that random number and the
// frame. PARTIAL: the record's house, object, layer and logic lists are not filled (counts zero),
// so a sync dump lists the CRCs only. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern unsigned char DAT_00B04880;
extern unsigned int DAT_00AC51FC;
static int Yr_Frame_Random;

void __fastcall FUN_00650A90(int slot)
{
	if (!DAT_00B04880) {
		Yr_Frame_Random = Scen->RandomNumber();
		return;
	}
	SyncFrameLog * log = &DAT_00AC6660[slot];
	log->CRC = DAT_00AC51FC;
	log->Random = Scen->RandomNumber();
	log->Frame = Frame;
	for (int i = 0; i < 8; i++) {
		log->Houses[i].ID = -1;
	}
	log->Total = 0;
	log->InfantryCount = 0;
	log->UnitCount = 0;
	log->AircraftCount = 0;
	log->BuildingCount = 0;
	for (int layer = 0; layer < 5; layer++) {
		log->LayerCount[layer] = 0;
	}
	log->LogicCount = 0;
}

// The frame's game CRC in the log for the LAN tests (lane YW2; fixups.json, Queue_AI_Multiplayer):
// "crc <frame> <value>" with RA2_NATIVE_CRC_LOG set, as the RA2 game's (native/link/native_event_data.cpp).
void Yr_Native_Log_Frame_CRC(int frame, unsigned int crc)
{
	static int enabled = -1;
	if (enabled < 0) enabled = getenv("RA2_NATIVE_CRC_LOG") != NULL;
	if (enabled) {
		fprintf(stderr, "crc %d %08x\n", frame, crc);
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane YW2, LAN): the network game's frame loop, unmatched in ra2-decomp.
// Written from gamemd.exe's code; the text is the RA2 tree's exact Queue_AI_Multiplayer and
// Wait_For_Players (RA2 0x00623B00 and 0x00624C10, src/recovered/queue.cpp, with its inlined
// Close_Reconnect_Dialog / Process_Reconnect_Dialog / Can_Advance), which gamemd.exe repeats, in YR's
// names; TibSun's queue.cpp (TibSun archive, GPL-3.0-or-later with EA's section 7 terms; THIRD_PARTY.md)
// is the shape and its timing table. Westwood Online's parts (the Internet game: the packet router's
// IM_STILL_HERE packets, the global channel's routing) are left out: Session.Type is never
// GAME_INTERNET natively. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern int DAT_00B04874;
extern unsigned char DAT_00B04878;
extern int DAT_00B04884;
extern int DAT_00B04888;
extern unsigned char DAT_00B0488C;
extern unsigned char DAT_00B0488D;
extern int DAT_00AC50FC[8][8];
extern unsigned int ScenarioCRC;
extern bool ConnectionLost;
void FUN_005BC3C0(void);
void FUN_005BC400(void);
void __fastcall FUN_0072DDB0(void);
void Register_Game_End_Time(void);
BOOL CALLBACK Reconnect_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Cast_Kick_Vote(int kicker, int kickee);
void _DrawMessage(int color, const wchar_t * message, HWND window);
HWND WS_Create_Dialog(HINSTANCE instance, int id, HWND parent, DLGPROC proc, BOOL force_show);
bool WS_Destroy_Dialog(HWND window, int id);
HWND WS_Find_Dialog(int id);
extern wchar_t DAT_00887734[];
extern Surface * VisibleSurface;
namespace UI { void RegisterWindow(HWND window, int id); }
namespace Game { extern HINSTANCE hInstance; extern HWND hWnd; }
void __fastcall Draw_Sync_Bars(HWND window);

// The reconnect dialog's state (gamemd.exe 0x00AC6658 the dialog, 0x00B04468 its result, 0x00B04470
// whether the mouse was captured, 0x00B04894 the tactical view's state, 0x00B04898 the seconds
// shown, 0x00AFA418 the time waited, which the sync bars show).
static HWND Yr_Reconnect_Dialog;
static int Yr_Reconnect_Result;
static bool Yr_Reconnect_Mouse_Captured;
static bool Yr_Reconnect_Tactical_Active;
static int Yr_Reconnect_Seconds;
static int Yr_Reconnect_Elapsed;

static void Yr_Close_Reconnect_Dialog(void)
{
	HWND dialog = WS_Find_Dialog(0xEA);
	if (dialog) {
		if (Yr_Reconnect_Mouse_Captured) {
			MouseCursor->Capture_Mouse();
		}
		WS_Destroy_Dialog(dialog, 0);
		TacticalActive = Yr_Reconnect_Tactical_Active;
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		if (TacticalActive) {
			Map.Render();
		}
	}
}

static int Yr_Process_Reconnect_Dialog(ConnManClass * net, CDTimerClass<SystemTimerClass> * timeout_timer, FrameSyncStruct * their,
	int num_conn, int reconn, int fresh, BasicTimerClass<SystemTimerClass> * timer)
{
	int i;
	wchar_t buf[512];

	Yr_Reconnect_Elapsed = timer->Value();
	for (i = 0; i < num_conn; i++) {
		SyncBarFrameSync[i] = their[i];
	}
	if (fresh) {
		Yr_Reconnect_Tactical_Active = TacticalActive;
		TacticalActive = false;
		Yr_Reconnect_Result = -1;
		Yr_Reconnect_Dialog = WS_Create_Dialog(Game::hInstance, 0xEA, Game::hWnd, Reconnect_Dialog_Proc, 0);
		if (Yr_Reconnect_Dialog) {
			SetWindowLongA(Yr_Reconnect_Dialog, 8, (LONG)&Yr_Reconnect_Result);
			UI::RegisterWindow(Yr_Reconnect_Dialog, 0xEA);
			ShowWindow(Yr_Reconnect_Dialog, SW_SHOWNORMAL);
			SendMessage(Yr_Reconnect_Dialog, 0x4A9, 0, 1);
			Yr_Reconnect_Mouse_Captured = MouseCursor->Is_Captured();
			if (Yr_Reconnect_Mouse_Captured) {
				MouseCursor->Release_Mouse();
			}
			MouseCursor->Show_Mouse();
		}
	}
	if (Yr_Reconnect_Result == 2) {
		return(1);
	}
	int new_time = timeout_timer->Value() / 60;
	if (fresh || new_time != Yr_Reconnect_Seconds) {
		Yr_Reconnect_Seconds = new_time;
		HWND item = GetDlgItem(Yr_Reconnect_Dialog, 0x650);
		if (item) {
			swprintf(buf, StringTable::LoadString("TXT_TIME_ALLOWED", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf0d), Yr_Reconnect_Seconds);
			SendMessage(item, 0x4B2, 0, (LPARAM)buf);
		}
		if (!(Yr_Reconnect_Seconds & 1)) {
			PostMessage(Yr_Reconnect_Dialog, WM_PAINT, 0, 0);
		}
		if (fresh) {
			while (Session.KickProposals.Count()) {
				delete Session.KickProposals[0];
				Session.KickProposals.Delete_Index(0);
			}
			memset(Session.KickVoteCount, 0, sizeof(Session.KickVoteCount));
			memset(Session.KickVoteWho, 0xFF, sizeof(Session.KickVoteWho));
			HWND list = GetDlgItem(Yr_Reconnect_Dialog, 0x651);
			if (list) {
				if (reconn) {
					int oldest = 0x7FFFFFFF;
					int oldest_index = 0;
					for (i = 0; i < num_conn; i++) {
						if (their[i].frame < oldest) {
							oldest = their[i].frame;
							oldest_index = i;
						}
					}
					if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
						swprintf(buf, StringTable::LoadString("TXT_RECONNECTING_TO", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf44),
							Ipx.Connection_Name(Ipx.Connection_ID(oldest_index)));
					} else {
						swprintf(buf, StringTable::LoadString("TXT_RECONNECTING_TO", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf4a),
							Session.Players[1]->Name);
					}
					_DrawMessage(-1, buf, list);
					_DrawMessage(-1, DAT_00887734, list);
					if (Session.Type == GAME_IPX) {
						wcscpy(buf, StringTable::LoadString("TXT_RECONNECT_HELP2", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf66));
						wcscat(buf, L"\n");
						wcscat(buf, StringTable::LoadString("TXT_RECONNECT_HELP4", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf6c));
						_DrawMessage(-1, buf, list);
						_DrawMessage(-1, DAT_00887734, list);
					}
					_DrawMessage(-1, DAT_00887734, list);
					_DrawMessage(-1, StringTable::LoadString("TXT_RECONNECT_HELP5", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf76), list);
					_DrawMessage(-1, StringTable::LoadString("TXT_RECONNECT_HELP1", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf7b), list);
					_DrawMessage(-1, DAT_00887734, list);
				} else {
					swprintf(buf, StringTable::LoadString("TXT_WAITING_FOR_CONNECTIONS", NULL, "D:\\ra2mdpost\\Queue.CPP", 0xf7e));
					_DrawMessage(-1, buf, list);
				}
			}
		}
	}
	if (Session.KickProposals.Count() > 0) {
		Cast_Kick_Vote(Session.KickProposals[0]->Kick.KickerID, Session.KickProposals[0]->Kick.KickeeID);
		delete Session.KickProposals[0];
		Session.KickProposals.Delete_Index(0);
	}
	for (i = 0; i < Session.Players.Count(); i++) {
		int votes = Session.KickVoteCount[Session.Players[i]->Player.ID];
		if (votes >= Session.Players.Count() - 1) {
			DebugString("Kicking player %s from the game due to %d votes\n", FUN_007350C0(Session.Players[i]->Name), votes);
			if (i == 0) {
				DebugString("Outvoted player is me!\n");
				for (i = 1; i < Session.Players.Count(); i++) {
					Kick_Player_Now(net, net->Connection_Index(Session.Players[i]->Player.ID), their, false);
				}
				break;
			}
			Kick_Player_Now(net, net->Connection_Index(Session.Players[i]->Player.ID), their, true);
		}
	}
	return(0);
}

static int Yr_Can_Advance(ConnManClass * net, int max_ahead, FrameSyncStruct * their, int * frame_stall, int * count_stall)
{
	int i;
	*frame_stall = -1;
	if (Session.NumPlayers == 1) {
		return(1);
	}
	int their_oldest_frame = Frame + 1000;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their[i].frame < their_oldest_frame) {
			their_oldest_frame = their[i].frame;
			*frame_stall = i;
		}
	}
	int count_ok = 1;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their[i].recv < their[i].sent) {
			count_ok = 0;
			break;
		}
	}
	if (count_ok) {
		if (Frame < their_oldest_frame + max_ahead) {
			return(1);
		}
		i = -1;
	}
	if (Frame < their_oldest_frame + max_ahead) {
		*frame_stall = -1;
	}
	*count_stall = i;
	return(0);
}

// Wait_For_Players, YR 0x00648710 (5,519 bytes): waits until every other player has been heard from
// (first_time: the game's start; else until this frame may run: Can_Advance), resending frame syncs,
// with the reconnect dialog and its kick votes after dialog_time, and the frame-sync stall bars.
int __fastcall Wait_For_Players(int first_time, ConnManClass * net, int resend_delta, int dialog_time, int timeout,
	char * multi_packet_buf, int my_sent, FrameSyncStruct * their)
{
	int packetlen;
	int id;
	int num_ready = 0;
	int loop_count = 0;
	int messages_this_loop;
	int x, y;
	int i;
	int rc;
	bool stall_drawn = false;
	int reconnect_dlg = 0;
	CDTimerClass<SystemTimerClass> retry_timer = resend_delta;
	CDTimerClass<SystemTimerClass> dialog_timer = dialog_time;
	CDTimerClass<SystemTimerClass> timeout_timer = timeout;
	BasicTimerClass<SystemTimerClass> timer;
	CDTimerClass<SystemTimerClass> progress_timer = 180;
	CDTimerClass<SystemTimerClass> keepalive_timer = 300;

	unsigned int now = timeGetTime();
	for (i = 0; i < net->Num_Connections(); i++) {
		their[i].timing = now;
	}

	while (true) {
		Keyboard->Check();
		if (retry_timer == 0) {
			retry_timer = resend_delta;
			Send_FrameSync(net, my_sent);
		}
		if (keepalive_timer == 0 && !first_time) {
			keepalive_timer = 300;
		} else if (first_time) {
			if (progress_timer == 0) {
				if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM || Session.ViaPacketRouter) {
					DebugString("Resending progress message\n");
					Session.Update_Progress(Scen->IsRandom ? 200 : 100);
					progress_timer = 180;
				}
			}
		}
		net->Service();

		if (dialog_timer == 0) {
			int fresh = reconnect_dlg == 0;
			int reconn = first_time == 0;
			if (Yr_Process_Reconnect_Dialog(net, &timeout_timer, their, net->Num_Connections(), reconn, fresh, &timer)) {
				WS_Destroy_Dialog(Yr_Reconnect_Dialog, 0);
				if (Yr_Reconnect_Mouse_Captured) {
					MouseCursor->Capture_Mouse();
				}
				TacticalActive = Yr_Reconnect_Tactical_Active;
				Map.Flag_To_Redraw(GS_REDRAW_ALL);
				if (reconnect_dlg) {
					Yr_Close_Reconnect_Dialog();
				}
				FUN_006523A0();
				return(DAT_00B04878 ? RC_CANCEL + 1 : RC_CANCEL);
			}
			reconnect_dlg = 1;
		}

		if (timeout_timer == 0) {
			if (first_time) {
				if (reconnect_dlg) {
					Yr_Close_Reconnect_Dialog();
				}
				FUN_006523A0();
				return(RC_NOT_RESPONDING);
			}
			if (Handle_Timeout(net, their)) {
				Map.Flag_To_Redraw(GS_REDRAW_ALL);
				Map.Render();
				retry_timer = resend_delta;
				dialog_timer = dialog_time;
				timeout_timer = timeout;
			} else {
				if (reconnect_dlg) {
					Yr_Close_Reconnect_Dialog();
				}
				return(RC_NOT_RESPONDING);
			}
		}

		messages_this_loop = 0;
		while (messages_this_loop++ < 64 && net->Get_Private_Message(multi_packet_buf, &packetlen, &id)) {
			Keyboard->Check();
			if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
				rc = Process_Serial_Packet(multi_packet_buf, first_time);
				if (rc == RC_SERIAL_PROCESSED) {
					net->Service();
					retry_timer = resend_delta;
					dialog_timer = dialog_time;
					timeout_timer = timeout;
					continue;
				} else if (rc == RC_PLAYER_LEFT) {
					if (first_time) {
						num_ready++;
					}
					break;
				} else if (rc == RC_HUNG_UP) {
					if (Handle_Timeout(net, their)) {
						Map.Flag_To_Redraw(GS_REDRAW_ALL);
						Map.Render();
						retry_timer = resend_delta;
						dialog_timer = dialog_time;
						timeout_timer = timeout;
					} else {
						if (reconnect_dlg) {
							Yr_Close_Reconnect_Dialog();
						}
						FUN_006523A0();
						return(RC_NOT_RESPONDING);
					}
				} else if (rc != RC_NORMAL) {
					break;
				}
			}
			rc = FUN_0064A0C0(net, multi_packet_buf, id, packetlen, their, &timer);
			if (rc == RC_PLAYER_READY) {
				num_ready++;
			} else if (rc == RC_SCENARIO_MISMATCH) {
				if (reconnect_dlg) {
					Yr_Close_Reconnect_Dialog();
				}
				return(RC_SCENARIO_MISMATCH);
			} else if (rc == RC_DOLIST_FULL) {
				if (reconnect_dlg) {
					Yr_Close_Reconnect_Dialog();
				}
				FUN_006523A0();
				return(RC_DOLIST_FULL);
			}
			net->Service();
		}

		if (first_time) {
			if (num_ready >= net->Num_Connections()) {
				break;
			}
		} else {
			int frame_stall;
			int count_stall;
			if (Yr_Can_Advance(net, Session.MaxAhead, their, &frame_stall, &count_stall)) {
				break;
			}
			bool show = true;
			if (Session.EnableMultiplayerDebug && Session.DrawMPDebugStats && loop_count > 0
				&& (!stall_drawn || frame_stall != -1 || count_stall != -1)) {
				Multiplayer_Debug_Print(true);
			} else if (stall_drawn) {
				show = false;
			}
			if (show) {
				if (frame_stall != -1) {
					if (Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) {
						VisibleSurface->Fill_Rect(Rect((frame_stall * 5 + 5) * 20, 400, 35, 3), DSurface::Build_Hicolor_Pixel(255, 255, 40));
					}
					Session.ConnectionStats[frame_stall].FrameSyncStalls++;
				}
				if (count_stall != -1) {
					if (Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) {
						VisibleSurface->Fill_Rect(Rect(count_stall * 100 + 140, 400, 40, 3), DSurface::Build_Hicolor_Pixel(255, 40, 40));
					}
					Session.ConnectionStats[frame_stall].CommandCountStalls++;
				}
				stall_drawn = true;
			}
		}

		Call_Back();
		if (!first_time && SpecialDialog == SDLG_NONE && !reconnect_dlg) {
			KeyNumType input;
			Map.Input(input, x, y);
			Keyboard_Process(input);
			TacticalMap->AI();
			Map.Render();
		}
		loop_count++;
	}

	if (reconnect_dlg) {
		Yr_Close_Reconnect_Dialog();
	}
	if (stall_drawn && Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) {
		VisibleSurface->Fill_Rect(Rect(100, 400, 540, 4), 0);
	}
	return(RC_NORMAL);
}

// The timings by Session.Type (TibSun's _timings; the Internet row is gamemd.exe's): the frame-sync
// resend period, the time before the reconnect dialog, the in-game timeout and the start's timeout.
static struct {
	int Resend;
	int Dialog;
	int Timeout;
	int Load;
} const Yr_Queue_Timings[6] = {
	{ 0, 0, 0, 0 },
	{ 120, 900, 420, 3600 },
	{ 120, 900, 420, 3600 },
	{ 60, 900, 420, 2400 },
	{ 120, 1800, 720, 7200 },
	{ 0, 0, 0, 0 },
};

static void Yr_Stop_Game(bool is_cancelled)
{
	Session.LoadGame = 0;
	Session.EmergencySave = 0;
	GameActive = false;
	if (DAT_00B04874) {
		FUN_005BC400();
	}
	if (Session.Type == GAME_INTERNET) {
		ConnectionLost = true;
		if (!is_cancelled) Session.SawGameCompletion = true;
		Session.OutOfSync = true;
		Register_Game_End_Time();
		Send_Statistics_Packet(false);
	}
	FUN_0072DDB0();
	Toggle_Display_Mode(false);
}

// The error box for Wait_For_Players' result: returns whether it was an error (not a cancel).
static bool Yr_Report_Wait_Result(int rc, int line)
{
	char const * text = NULL;
	bool is_error = true;
	if (rc == RC_NOT_RESPONDING) text = "TXT_SYSTEM_NOT_RESPONDING";
	else if (rc == RC_SCENARIO_MISMATCH) text = "TXT_SCENARIOS_DO_NOT_MATCH";
	else if (rc == RC_DOLIST_FULL) text = "TXT_QUEUE_FULL";
	else if (rc == RC_CANCEL) is_error = false;
	else if (rc == RC_CANCEL + 1) { text = "TXT_DISCON_ABORT"; is_error = false; }
	if (text) {
		WWMessageBox().Process(StringTable::LoadString(text, NULL, "D:\\ra2mdpost\\Queue.CPP", line),
			StringTable::LoadString("TXT_OK", NULL, "D:\\ra2mdpost\\Queue.CPP", line), NULL, NULL);
	}
	return(is_error);
}

// Queue_AI_Multiplayer, YR 0x006475F0 (3,404 bytes): a network game's frame: the CRC, the start's
// wait for every player (frame 0 or a loaded game), the process-time and timing events, the packets
// sent each send period, the wait until this frame may run, then the DoList executed and cleaned.
void __fastcall FUN_006475F0(void)
{
	static CDTimerClass<FrameTimerClass> skip_crc;
	ConnManClass * net;
	char * buffer;
	int maximum;
	int rc;
	int i;
	int j;

	if (Session.Type == GAME_SKIRMISH) {
		return;
	}
	if (Session.Type >= GAME_IPX && Session.Type <= GAME_INTERNET) {
		buffer = Session.MetaPacket;
		maximum = Session.MetaSize;
		net = &Ipx;
	} else {
		buffer = NullModem.BuildBuf;
		maximum = NullModem.MaxLen - 0xe;
		net = &NullModem;
	}

	FUN_0064DAB0();
	DAT_00B04474[Frame & 0xff] = DAT_00AC51FC;
	Yr_Native_Log_Frame_CRC(Frame, DAT_00AC51FC);

	if (Frame == 0 || Session.LoadGame) {
		for (i = 0; i < 7; i++) {
			TheirFrameSync[i].frame = -1;
			TheirFrameSync[i].sent = 0;
			TheirFrameSync[i].recv = 0;
			TheirFrameSync[i].unknown1 = 0;
			TheirFrameSync[i].timing = 0;
			TheirFrameSync[i].unknown2 = 20;
		}
		skip_crc = Frame + 256;
		SentCommandCount = 0;
		memset(DAT_00B04474, 0, 256 * sizeof(DAT_00B04474[0]));
		memset(DAT_00AFA42C, 0, sizeof(DAT_00AFA42C));
		DAT_00B04884 = 0;
		DAT_00B04888 = 0;
		DAT_00B0488C = 0;
		DAT_00B0488D = 0;
		DAT_00B04878 = 0;
		memset(DAT_00AFA338, 0, sizeof(DAT_00AFA338));
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				DAT_00AC50FC[j][i] = 0;
			}
		}
		if (Session.LoadGame) {
			ScenarioCRC = Session.EmergencySave ? 0 : DAT_00AC51FC;
		}
		SentFrameSyncCount = 0;
		SentFrameSyncTimer = 0;
		Send_FrameSync(net, SentCommandCount);
		rc = Wait_For_Players(1, net, Yr_Queue_Timings[Session.Type].Resend, Yr_Queue_Timings[Session.Type].Dialog,
			Yr_Queue_Timings[Session.Type].Load, buffer, SentCommandCount, TheirFrameSync);
		if (rc != RC_NORMAL) {
			if (Session.Type == GAME_INTERNET) {
				Register_Game_End_Time();
			}
			Session.Suspended++;
			bool is_error = Yr_Report_Wait_Result(rc, 0x40c);
			Session.Suspended--;
			Yr_Stop_Game(!is_error);
			return;
		}
		for (i = 0; i < 7; i++) {
			Session.PlayerLatency[i] = 0;
		}
		for (i = 0; i < 7; i++) {
			TheirFrameSync[i].frame = 0;
		}
		net->Reset_Response_Time(true);
		if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP
			&& Frame != (int)(((Frame + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate)) {
			net->Service();
			if (DAT_00B04874) {
				FUN_005BC400();
			}
		}
		if (Session.LoadGame) {
			Session.EmergencySave = 0;
			Session.LoadGame = 0;
		}
	} else if ((Frame & 0x7f) == 0) {
		// The process-time event: this player's average frame time, and the connection's timing.
		unsigned int response = net->Response_Time();
		switch (Session.LatencyFudge) {
			case 0: break;
			case 1: response += response >> 1; DebugString("Response time = %d\n", response); break;
			case 2: response *= 2; DebugString("Response time = %d\n", response); break;
			case 3: response *= 3; DebugString("Response time = %d\n", response); break;
			default: break;
		}
		if (Session.LatencyFudge == 0) {
			DebugString("Response time = %d\n", response);
		}
		unsigned int timeout = response * 8 + 15;
		if (timeout < 120) timeout = 120;
		net->Set_Timing(response + 10, -1, timeout, false);
		if (DAT_00B04874) {
			FUN_005BC3C0();
			DebugPage * page = FUN_005BC450(4);
			page->Set_Cursor(0, 23);
			FUN_005BD1F0(page, "Processing Ticks:%03d Frames:%03d\n", Session.ProcessTicks, Session.ProcessFrames);
			FUN_005BC400();
		}
		EventClass packet;
		packet.Type = EventClass::PROCESS_TIME;
		packet.Data.ProcessTime.AverageTicks = (unsigned short)(Session.ProcessTicks / Session.ProcessFrames);
		packet.Frame = Frame;
		packet.ID = PlayerPtr->HeapID;
		OutList.Add(packet);
		Session.ProcessTicks = 0;
		Session.ProcessFrames = 0;
		if (Session.Type == GAME_INTERNET && (Frame & 0x7ff) == 0) {
			net->Reset_Response_Time(false);
		}
	}

	if (Session.Am_I_Master(0)
		&& (Session.PrecalcMaxAhead != 0 || Session.PrecalcDesiredFrameRate != 0 || (Frame & 0xff) == 0)) {
		// The timing event (the host only): the frame rate and MaxAhead every player then uses.
		int my_sent = SentCommandCount;
		DebugString("Generate_Real_Timing_Event\n");
		EventClass packet;
		if (Session.PrecalcMaxAhead != 0 || Session.PrecalcDesiredFrameRate != 0) {
			DebugString("Sending precalculated network timings on frame %d\n", Frame);
			packet.Type = EventClass::TIMING;
			packet.Data.Timing.DesiredFrameRate = (unsigned short)Session.PrecalcDesiredFrameRate;
			packet.Data.Timing.MaxAhead = (unsigned short)Session.PrecalcMaxAhead;
			packet.Data.Timing.FrameSendRate = (unsigned int)Session.PrecalcDesiredFrameRate > 30 ? 10 : 5;
			packet.Frame = Frame;
			packet.ID = PlayerPtr->HeapID;
			OutList.Add(packet);
			Session.PrecalcMaxAhead = 0;
			Session.PrecalcDesiredFrameRate = 0;
		} else if (my_sent >= 5) {
			int worst = 0;
			bool known = true;
			for (i = 0; i < Session.Players.Count(); i++) {
				int ticks = Session.Players[i]->Player.ProcessTime;
				if (ticks == -1) {
					known = false;
					break;
				}
				if (ticks > worst) worst = ticks;
			}
			if (known) {
				int desired = worst == 0 ? 60 : (1000 / worst < 1 ? 1 : 1000 / worst);
				int cap = Options.GameSpeed == 0 ? 60 : (Options.GameSpeed == 1 ? 45 : 60 / Options.GameSpeed);
				if (desired > cap) desired = cap;
				unsigned int response;
				if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {
					response = Ipx.Multicast_Response_Time();
					unsigned int worst_response = 0;
					for (i = 0; i < net->Num_Connections(); i++) {
						if (worst_response <= TheirFrameSync[i].unknown2) worst_response = TheirFrameSync[i].unknown2;
					}
					response += worst_response;
				} else {
					response = net->Response_Time();
				}
				int connection_response = net->Response_Time();
				unsigned char send_rate = (unsigned char)Session.FrameSendRate;
				if (Session.Type == GAME_INTERNET) send_rate = desired > 30 ? 10 : 5;
				int base = 0;
				if (response != 0) {
					switch (Session.LatencyFudge) {
						case 0: break;
						case 1: response += response >> 1; connection_response += connection_response / 2; base = 10; break;
						case 2: response *= 2; connection_response *= 2; base = 20; break;
						case 3: response *= 3; connection_response *= 3; base = 30; break;
						default: break;
					}
					if (Session.LatencyFudge >= 0 && Session.LatencyFudge <= 3) {
						DebugString("Response time = %d\n", response);
					}
				}
				int ahead = (int)((response * desired) / 120) + base;
				if (ahead > (int)Session.MaxAhead && ahead >= (int)Session.MaxAhead + send_rate) {
					ahead = Session.MaxAhead + send_rate;
				}
				ahead = ((ahead + send_rate - 1) / send_rate) * send_rate;
				if (ahead <= send_rate * 3) ahead = send_rate * 3;
				int max_ahead = ((send_rate + 249) / send_rate) * send_rate;
				if (ahead >= max_ahead) ahead = max_ahead;
				packet.Type = EventClass::TIMING;
				packet.Data.Timing.DesiredFrameRate = (unsigned short)desired;
				packet.Frame = Frame;
				packet.ID = PlayerPtr->HeapID;
				packet.Data.Timing.MaxAhead = (unsigned short)(ahead + (Scen->Special.IsFogOfWar ? 10 : 0));
				packet.Data.Timing.FrameSendRate = send_rate;
				OutList.Add(packet);
				unsigned int timeout = connection_response * 8 + 15;
				if (timeout < 120) timeout = 120;
				net->Set_Timing(connection_response + 10, -1, timeout, false);
			}
		}
	}

	if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP
		&& Frame != (int)(((Frame + Session.FrameSendRate - 1) / Session.FrameSendRate) * Session.FrameSendRate)) {
		net->Service();
		if (DAT_00B04874) {
			FUN_005BC400();
			if (DAT_00B04874) {
				FUN_005BC400();
			}
		}
		return;
	}

	unsigned short sent = (unsigned short)Send_Packets(net, buffer, maximum, Session.MaxAhead, SentCommandCount);
	SentCommandCount += sent;
	if (sent) {
		DebugString("Frame %d, my sent = %d\n", Frame, SentCommandCount);
	}
	if (Frame == 0) {
		if (DAT_00B04874) {
			FUN_005BC400();
		}
		return;
	}

	int sync = (int)net->Response_Time() * 3;
	if (sync <= Yr_Queue_Timings[Session.Type].Timeout) sync = Yr_Queue_Timings[Session.Type].Timeout;
	rc = Wait_For_Players(0, net, 180, sync, Yr_Queue_Timings[Session.Type].Load, buffer, SentCommandCount, TheirFrameSync);
	if (rc != RC_NORMAL) {
		DebugString("Wait_For_Players returned %d\n", rc);
		if (Session.Type == GAME_INTERNET) {
			Register_Game_End_Time();
		}
		Session.Suspended++;
		bool is_error = Yr_Report_Wait_Result(rc, 0x4aa);
		Session.Suspended--;
		Yr_Stop_Game(!is_error);
		return;
	}
	if (Session.Record) {
		Queue_Record();
	}
	if (!FUN_0064C380(Session.MaxPlayers, HOUSE_FIRST, net, &skip_crc, TheirFrameSync)) {
		DebugString("Failure executing DoList\n");
		if (Session.Type == GAME_INTERNET) {
			Register_Game_End_Time();
		}
		Yr_Stop_Game(false);
		return;
	}
	while (DoList.Count && (DoList.First().IsExecuted || Frame > DoList.First().Frame)) {
		DoList.Next();
	}
	if (DAT_00B04874) {
		FUN_005BC400();
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW2, LAN): FUN_0064BDD0, YR 0x0064BDD0 (1,455 bytes, unmatched in
// ra2-decomp): Extract_Compressed_Events, the receive side of FUN_0064B660 (the compressed events of
// COMM_PROTOCOL_MULTI_E_COMP, the network game's protocol): a packet's events back into the DoList.
// From gamemd.exe's code, which is the RA2 game's Extract_Compressed_Events (RA2 0x006282D0, the same
// 414 instructions; its native draft in src/recovered/queue.cpp, played in RA2's LAN games, with its
// fixes: the packet's end ends the loop, malformed records stop the decoding), in YR's names. A
// megamission with a unit list stores the list by the sender's house and slot (gamemd.exe 0x00AFA468,
// counts DAT_00AC50FC) for later megamissions that name only the slot (FUN_0064B4D0's cache).
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
static xTargetClass Yr_Received_Unit_Lists[8][8][128];

int __fastcall FUN_0064BDD0(void * buf, int bufsize)
{
	int pos = 0;
	int leftover = bufsize;
	EventClass * event;
	int count = 0;
	int datasize;
	EventClass eventdata;
	unsigned char numunits = 0;
	int house;
	int slot;
	int units;
	xTargetClass * list;
	char * src;

	memset(&eventdata, 0, sizeof(EventClass));
	house = ((EventClass *)buf)->ID;
	datasize = 13;
	event = (EventClass *)buf;

	while (leftover >= datasize + 1) {
		if (event->Type == EventClass::FRAMESYNC) {
			pos += datasize + 1;
			leftover -= datasize + 1;
			event = (EventClass *)(((char *)buf) + pos);
			datasize = 13;
			continue;
		}
		if (event->Type == EventClass::FRAMEINFO) {
			eventdata.ID = event->ID;
			eventdata.Frame = event->Frame;
			pos += 6;
			leftover -= 6;
		} else if (event->Type == EventClass::MEGAMISSION) {
			numunits = *(((unsigned char *)buf) + pos + 1);
			pos += 1;
			leftover -= 1;
		}
		memset(&eventdata.Data, 0, sizeof(eventdata.Data));
		eventdata.Type = event->Type;
		if (eventdata.Type >= EventClass::LAST_EVENT) {
			DebugString("Extract_Compressed_Events: bad event type %d at %d of %d\n", eventdata.Type, pos, bufsize);
			return(count);
		}
		datasize = EventClass::EventLength[eventdata.Type];
		if (pos + 1 + datasize > bufsize) {
			DebugString("Extract_Compressed_Events: event %d runs past the packet (%d of %d)\n", eventdata.Type, pos, bufsize);
			return(count);
		}

		switch (eventdata.Type) {
			case EventClass::RESPONSE_TIME:
				memcpy(&eventdata.Data.FrameInfo.Delay, ((char *)buf) + pos + 1, datasize);
				break;

			case EventClass::ADDPLAYER:
				memcpy(&eventdata.Data.Variable.Size, ((char *)buf) + pos + 1, datasize);
				if ((int)eventdata.Data.Variable.Size < 0 || pos + 1 + datasize + (int)eventdata.Data.Variable.Size > bufsize) {
					return(count);
				}
				eventdata.Data.Variable.Pointer = new char[eventdata.Data.Variable.Size];
				memcpy(eventdata.Data.Variable.Pointer, ((char *)buf) + pos + 1 + datasize, eventdata.Data.Variable.Size);
				pos += eventdata.Data.Variable.Size;
				leftover -= eventdata.Data.Variable.Size;
				break;

			case EventClass::MEGAMISSION:
				memcpy(&eventdata.Data, ((char *)buf) + pos + 1, datasize);
				slot = eventdata.Data.MegaMission.Slot;
				if (numunits > 1) {
					pos += datasize + 1;
					leftover -= datasize + 1;
					datasize = sizeof(xTargetClass);
					DebugString("Decoding new MEGAMISSION\n");
					if (house < 0 || house >= 8 || slot < 0 || slot >= 8 || numunits > 128 || pos + (numunits - 1) * datasize > bufsize) {
						return(count);
					}
					Yr_Received_Unit_Lists[house][slot][0] = eventdata.Data.MegaMission.Whom;
					units = 1;
					DAT_00AC50FC[house][slot] = units;
					src = ((char *)buf) + pos;
					list = &Yr_Received_Unit_Lists[house][slot][1];
					while (numunits) {
						if (!DoList.Add(eventdata)) {
							return(-1);
						}
						count++;
						*list = *(xTargetClass *)src;
						eventdata.Data.MegaMission.Whom = *list++;
						numunits--;
						units++;
						DAT_00AC50FC[house][slot] = units;
						if (numunits == 1) {
							datasize -= 1;
							DebugString("Got new unit list for player %d with %d units in slot %d\n", house, units, slot);
							break;
						}
						pos += datasize;
						src += datasize;
						leftover -= datasize;
					}
				} else if (numunits == 0) {
					pos += datasize + 1;
					leftover -= datasize + 1;
					datasize = sizeof(xTargetClass);
					units = 1;
					if (house < 0 || house >= 8 || slot < 0 || slot >= 8 || DAT_00AC50FC[house][slot] > 128) {
						return(count);
					}
					numunits = DAT_00AC50FC[house][slot];
					eventdata.Data.MegaMission.Whom = Yr_Received_Unit_Lists[house][slot][0];
					list = &Yr_Received_Unit_Lists[house][slot][1];
					while (numunits) {
						if (!DoList.Add(eventdata)) {
							return(-1);
						}
						count++;
						numunits--;
						units++;
						eventdata.Data.MegaMission.Whom = *list++;
						if (numunits == 1) {
							datasize = -1;
							DebugString("Used old unit list in slot %d - %d bytes saved\n", slot, units * 5);
							break;
						}
					}
				}
				break;

			default:
				memcpy(&eventdata.Data, ((char *)buf) + pos + 1, datasize);
				break;
		}

		if (!DoList.Add(eventdata)) {
			if (eventdata.Type == EventClass::ADDPLAYER) {
				delete[] (char *)eventdata.Data.Variable.Pointer;
			}
			return(-1);
		}
		count++;
		pos += datasize + 1;
		leftover -= datasize + 1;
		if (leftover == 0) {
			return(count);
		}
		event = (EventClass *)(((char *)buf) + pos);
		datasize = EventClass::EventLength[event->Type];
		if (event->Type == EventClass::MEGAMISSION) {
			datasize += 1;
		}
	}
	return(count);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW2, LAN): Draw_Sync_Bars, YR 0x0064A7B0 (741 bytes, unmatched in
// ra2-decomp), from gamemd.exe's code (TibSun's queue.cpp Draw_Sync_Bars the shape): the reconnect
// dialog's bar per player, shorter the longer the wait on that player (none at 1,200 ticks, at least
// 6 pixels), green, yellow after 240 ticks and red after 480, but blue while that player's packets
// still arrive (the last one under 420 ticks ago: the bar then shows that time); framed in the
// dialog colour. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern RGBClass DAT_00A8EAE4;
extern Surface * AlternateSurface;
int __fastcall Get_Display_Rect(HWND window, RECT * rect);

void __fastcall Draw_Sync_Bars(HWND window)
{
	int i;
	for (i = 0; i < Session.Players.Count(); i++) {
		RECT winrect;
		Get_Display_Rect(GetDlgItem(window, SyncBarControlIDs[i]), &winrect);
		Rect bar(winrect.left, winrect.top, winrect.right - winrect.left, winrect.bottom - winrect.top);
		int id = Session.Players[i]->Player.ID;
		int index = (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) ? 0 : Ipx.Connection_Index(id);
		unsigned int progress = i == 0 ? 0 : (unsigned int)(Yr_Reconnect_Elapsed - SyncBarFrameSync[index].unknown1);
		int color = DSurface::Build_Hicolor_Pixel(0, 200, 0);
		if (progress > 240) {
			color = DSurface::Build_Hicolor_Pixel(200, 200, 0);
			if (progress > 480) {
				color = DSurface::Build_Hicolor_Pixel(200, 0, 0);
				unsigned int since = (timeGetTime() * 60 - SyncBarFrameSync[index].timing * 60) / 1000;
				if (since < 420) {
					color = DSurface::Build_Hicolor_Pixel(0, 0, 200);
					progress = since;
				}
			}
		}
		int percent = 100 - (int)(progress * 100 / 1200);
		if (percent < 0) percent = 0;
		int width = bar.Width * percent / 100;
		if (width < 6) width = 6;
		Rect frame(bar.X, bar.Y, bar.Width, bar.Height);
		AlternateSurface->Draw_Rect(AlternateSurface->Get_Rect(), frame, DSurface::Build_Hicolor_Pixel(DAT_00A8EAE4));
		Rect fill(bar.X + 2, bar.Y + 2, width - 4, bar.Height - 4);
		AlternateSurface->Fill_Rect(AlternateSurface->Get_Rect(), fill, color);
	}
}
