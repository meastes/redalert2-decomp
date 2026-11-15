
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
