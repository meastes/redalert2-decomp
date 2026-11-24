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

/* $Header: /counterstrike/QUEUE.CPP 6     3/14/97 5:12p Steve_tall $ */
/***************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : QUEUE.CPP                                *
 *                                                                         *
 *                   Programmer : Bill R. Randolph                         *
 *                                                                         *
 *                   Start Date : 11/28/95                                 *
 *                                                                         *
 *                  Last Update : October 14, 1996 [BRR]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions for Queueing Events:                                          *
 *   Queue_Mission -- Queue a mega mission event.                          *
 *   Queue_Options -- Queue the options event.                             *
 *   Queue_Exit -- Add the exit game event to the queue.                   *
 *                                                                         *
 * Functions for processing Queued Events:                                 *
 *   Queue_AI -- Process all queued events.                                *
 *   Queue_AI_Normal -- Process all queued events.                         *
 *   Queue_AI_Multiplayer -- Process all queued events.                    *
 *                                                                         *
 * Main Multiplayer Queue Logic:                                           *
 *   Wait_For_Players -- Waits for other systems to come on-line           *
 *   Generate_Timing_Event -- computes & queues a RESPONSE_TIME event      *
 *   Process_Send_Period -- timing for sending packets every 'n' frames    *
 *   Send_Packets -- sends out events from the OutList                     *
 *   Send_FrameSync -- Sends a FRAMESYNC packet                            *
 *   Process_Receive_Packet -- processes an incoming packet                *
 *   Process_Serial_Packet -- Handles an incoming serial packet            *
 *   Can_Advance -- determines if it's OK to advance to the next frame     *
 *   Process_Reconnect_Dialog -- processes the reconnection dialog         *
 *   Handle_Timeout -- attempts to reconnect; if fails, bails.             *
 *   Stop_Game -- stops the game                                           *
 *                                                                         *
 * Packet Compression / Decompression:                                     *
 *   Build_Send_Packet -- Builds a big packet from a bunch of little ones. *
 *   Add_Uncompressed_Events -- adds uncompressed events to a packet       *
 *   Add_Compressed_Events -- adds compressed events to a packet           *
 *   Breakup_Receive_Packet -- Splits a big packet into little ones.       *
 *   Extract_Uncompressed_Events -- extracts events from a packet          *
 *   Extract_Compressed_Events -- extracts events from a packet            *
 *                                                                         *
 * DoList Management:                                                      *
 *   Execute_DoList -- Executes commands from the DoList                   *
 *   Clean_DoList -- Cleans out old events from the DoList                 *
 *   Queue_Record -- Records the DoList to disk                            *
 *   Queue_Playback -- plays back queue entries from a record file         *
 *                                                                         *
 * Debugging:                                                              *
 *   Compute_Game_CRC -- Computes a CRC value of the entire game.          *
 *   Add_CRC -- Adds a value to a CRC                                      *
 *   Print_CRCs -- Prints a data file for finding Sync Bugs                *
 *   Init_Queue_Mono -- inits mono display                                 *
 *   Update_Queue_Mono -- updates mono display                             *
 *   Print_Framesync_Values -- displays frame-sync variables               *
 *   Check_Mirror -- Checks mirror memory                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "queue.h"

#include "_keyboar.h"
#include "_logic.h"
#include "_map.h"
#include "_mono.h"
#include "_rtti.h"
#include "_rules.h"
#include "_script.h"
#include "astar.h" // RA2: Search
#include "_astar.h"
#include "_surface.h"
#include "_tactica.h"
#include "_vanim.h"
#include "_warhead.h"
#include "_weapon.h"
#include "_xmouse.h"
#include "aircraft.h"
#include "airctype.h"
#include "anim.h"
#include "animtype.h"
#include "blight.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "cell.h"
#include "connmgr.h"
#include "conquer.h"
#include "crc.h"
#include "data.h"
#include "dbgprint.h"
#include "dsurface.h"
#include "empulse.h"
#include "factory.h"
#include "getcpu.h"
#include "globals.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "incdec.h"
#include "infantry.h"
#include "infatype.h"
#include "internet.h"
#include "ipxmgr.h"
#include "keyboard.h"
#include "language\language.h"
#include "light.h"
#include "mainloop.h"
#include "mono.h"
#include "msgbox.h"
#include "msgloop.h"
#include "ownertalk.h" // RA2: OwnerTalk
#include "netdlg.h"
#include "netshare.h"
#include "nulldlg.h"
#include "nullmgr.h"
#include "overlay.h"
#include "overtype.h"
#include "ownrdraw.h"
#include "ownrpaint.h"
#include "particle.h"
#include "partsys.h"
#include "psystype.h"
#include "ptype.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "script.h"
#include "session.h"
#include "side.h"
#include "smudge.h"
#include "smudtype.h"
#include "stats.h"
#include "stimer.h"
#include "tactical.h"
#include "taction.h"
#include "tag.h"
#include "tagtype.h"
#include "taskforc.h"
#include "terrain.h"
#include "tevent.h"
#include "tiberium.h"
#include "trigger.h"
#include "trigtype.h"
#include "tube.h"
#include "unit.h"
#include "unittype.h"
#include "vanim.h"
#include "vanimtype.h"
#include "version.h"
#include "vox.h" // RA2: Play_Taunt
#include "warhead.h"
#include "waypoint.h"
#include "weapon.h"
#include "windlg.h"
#include "winstub.h"
#include "wonline.h"
#include "wolapi.h"
#include "fpscounter.h"



#include "stringtable.h" // RA2: the includes this file needs
#include "wsproto.h"

#include "special.hh"

#include <ctime>










/********************************** Defines *********************************/
//#define SHOW_MONO		0			// Replaced with _DEBUG


/********************************** Globals *********************************/
//---------------------------------------------------------------------------
// GameCRC is the current computed CRC value for this frame.
// CRC[] is a record of our last 32 game CRC's.
// ColorNames is for debug output in Print_CRCs
//---------------------------------------------------------------------------
static unsigned int GameCRC;

// RA2: file-level state of the reconnect dialog and the stop test (addresses from meastes' Queue.CPP)
int ReconnectResult; // RA2: 0x00AB5ED0; the dialog's answer (-1 while open, 2 = cancel)
bool ReconnectMouseCaptured; // RA2: 0x00AB5ED8; the mouse was captured when the dialog opened
bool ReconnectSavedTactical; // RA2: 0x00AB62FC; TacticalActive before the dialog
HWND ReconnectDialog; // RA2: 0x00A780D4; the open reconnect dialog
int ReconnectSeconds; // RA2: 0x00AB6300; time left shown in the dialog
long ReconnectElapsed; // RA2: 0x00AABE90; timer value when the dialog was last drawn
bool ConnectionLost; // RA2: 0x00ABD500; set when the connection is given up on

bool ExtraSyncInfo; // RA2: 0x00AB62E8; -MPDEBUG: a mismatch writes the sync log files instead of printing the CRCs
void __fastcall Write_Sync_File(int slot, EventClass * event); // RA2: 0x0062DBA0, the sync log written when ExtraSyncInfo is set
#define WAIT_TEXT(label, line) StringTable::LoadString(label, NULL, "D:\\Ra2\\Queue.CPP", line) // RA2: the string table lookup carries the source path and line
static unsigned int CRC[256] =
	{0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0,0,0,0,0,
	 0,0,0,0,0,0};


//...........................................................................
// Mono debugging variables:
// NetMonoMode: 0 = show connection output, 1 = flowcount output
// NewMonoMode: set by anything that toggles NetMonoMode; re-inits screen
// IsMono: used for taking control of Mono screen away from the engine
//...........................................................................
int NetMonoMode = 1;
int NewMonoMode = 1;
static int IsMono = 0;
bool ReconnectPingTimedOut; // RA2: 0x00AB62E0; the reconnect dialog's pings showed a dead connection (set by the dialog, tested after it closes)
int LastKickTime; // RA2: 0x00AB6304; time() of the last kick, for telling a run of kicks from resignations
int ReconnectPingSeconds; // RA2: 0x00AB62F8; seconds of pings counted while the dialog is open
int ReconnectSlowCount; // RA2: 0x00AB62EC; times the dialog's ping count passed 5
int ReconnectVerySlowCount; // RA2: 0x00AB62F0; times the dialog's ping count passed 10
bool ReconnectSlowCounted; // RA2: 0x00AB62F4; the 5-ping step was counted this time
bool ReconnectVerySlowCounted; // RA2: 0x00AB62F5; the 10-ping step was counted this time
int MegamissionCounts[8]; // RA2: 0x00AABDB0; unit counts of the cached repeated-megamission lists
char MegamissionLists[8][0x280]; // RA2: 0x00A76CA8; the cached repeated-megamission lists (the first unit and the event fields, then the other units, 5 bytes each)
int MegamissionFrames[8]; // RA2: 0x00AABE94; frame each cached list was last used (0 = free)
#ifdef RA2_NON_MATCHING // cached targets used only by the packet decoding draft
xTargetClass MegamissionUnitLists[64][128]; // RA2: 0x00AABED0; the units of the runs of the last megamission lists, 8 slots per player
#endif
int SavedUnitCounts[MAX_PLAYERS * ARRAY_SIZE(MegamissionCounts)]; // RA2: 0x00A76B74; unit counts of the saved lists (64 ints; cleared column by column)
int SentFrameSyncCount = 0;

//---------------------------------------------------------------------------
// Several routines return various codes; here's an enum for all of them.
//---------------------------------------------------------------------------
enum RetcodeType {
	RC_NORMAL,              // no news is good news
	RC_PLAYER_READY,        // a new player has been heard from
	RC_SCENARIO_MISMATCH,   // scenario mismatch
	RC_DOLIST_FULL,         // DoList is full
	RC_SERIAL_PROCESSED,    // modem: SERIAL packet was processed
	RC_PLAYER_LEFT,         // modem: other player left the game
	RC_HUNG_UP,             // modem has hung up
	RC_NOT_RESPONDING,      // other player not responding (timeout/hung up)
	RC_CANCEL,              // user cancelled
	RC_DISCONNECT_ABORT,    // RA2: the reconnect dialog gave up on a slow connection (TXT_DISCON_ABORT)
};





/// <summary>
/// Constructs an empty frame sync record.
/// The counters start at zero and the frame number starts out invalid, so that a player
/// who has not reported in yet cannot be mistaken for one sitting on frame zero.
/// </summary>











//........................................................................
// Frame-sync'ing variables
//........................................................................
FrameSyncStruct SyncBarFrameSync[MAX_PLAYERS - 1];
BasicTimerClass<SystemTimerClass> SentFrameSyncTimer;
FrameSyncStruct TheirFrameSync[MAX_PLAYERS - 1];
unsigned short SentCommandCount;								// # cmds I've sent out



/********************************* Prototypes *******************************/
//...........................................................................
// Main multiplayer queue logic
//...........................................................................
static void Queue_AI_Normal(void);
static void Queue_AI_Multiplayer(void);
static RetcodeType Wait_For_Players(int first_time, ConnManClass *net,
	int resend_delta, int dialog_time, int timeout, char *multi_packet_buf,
	int my_sent, FrameSyncStruct *their);
static void Generate_Timing_Event(ConnManClass *net, int my_sent);
static void Generate_Real_Timing_Event(ConnManClass *net, int my_sent);
static void Generate_Process_Time_Event(ConnManClass *net);
static int Process_Send_Period(ConnManClass *net);	//, int init);
static int Send_Packets(ConnManClass *net, char *multi_packet_buf,
	int multi_packet_max, int max_ahead, int my_sent);
static void Send_FrameSync(ConnManClass *net, int cmd_count);
static RetcodeType Process_Receive_Packet(ConnManClass *net,
	char *multi_packet_buf, int id, int packetlen, FrameSyncStruct *their, BasicTimerClass<SystemTimerClass> *timer);
static RetcodeType Process_Serial_Packet(char *multi_packet_buf,
	int first_time);
static int Can_Advance(ConnManClass *net, int max_ahead, FrameSyncStruct *their, int *frame_stall, int *count_stall);
static int Process_Reconnect_Dialog(ConnManClass *net, CDTimerClass<SystemTimerClass> *timeout_timer, // RA2: takes the connection manager first
	FrameSyncStruct *their, int num_conn, int reconn, int fresh,
	BasicTimerClass<SystemTimerClass> *timer);
static int Handle_Timeout(ConnManClass *net, FrameSyncStruct *their);
static void Stop_Game(bool=false);
BOOL CALLBACK Reconnect_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
static void Close_Reconnect_Dialog(void);
void Kick_Player_Now(ConnManClass *net, int kickee, FrameSyncStruct * their, bool error);
void Cast_Kick_Vote(int kicker, int kickee);

unsigned int RouterResponseTimes[8]; // RA2: 0x00A76C88, the packet router round trip per connection




SyncState SyncStates[256]; // RA2: 0x00A780D8; filled by Fill_Sync_State when ExtraSyncInfo is set
int SyncRandomNumber; // RA2: 0x00AABCE0; the random number of the frame while no sync log is kept
void __fastcall Fill_Sync_State(int index); // RA2: 0x0062CF40, saves the game state of the frame in slot 0 to 255 of the sync log
void __fastcall Update_Event_Destination(int start, int count, TargetClass whom, TargetClass const & target); // RA2: 0x00629220
void __fastcall Spread_Units_Around_Cell(int start, int count); // RA2: 0x00629290, spreads a run of missions to one cell around it
static int Connection_Index(int player);
static inline Vector3 Normalized(Vector3 const & v);
void __fastcall Calc_Frames_Behind(FrameSyncStruct *their, int index); // RA2: 0x0062E5C0, a player's latency in frames
void Draw_Sync_Bars(HWND window); // RA2: 0x00626CB0, paints the reconnect dialog's response bars
void Queue_Ipx_Set(void); // RA2: 0x00623260, switches the global channel to routed sending in an internet game
void Queue_Ipx_Clear(void); // RA2: 0x00623280, switches it back
void Reset_Reconnect_Pings(void); // RA2: 0x00627330, clears the reconnect dialog's ping counters
void __fastcall Send_Port_Unreachable(void); // RA2: 0x0062E840, tells the router connections this port is unreachable
bool __fastcall Add_Outgoing_Event(EventClass event); // RA2: 0x0062E670
void __fastcall Enqueue_Event(EventClass * event); // RA2: 0x0062E6E0

//...........................................................................
// Packet compression/decompression:
//...........................................................................
static int Build_Send_Packet(void *buf, int bufsize, int frame_delay,
	int num_cmds, int cap, int & processed);
int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay, int size,
	int cap);
int Add_Compressed_Events(void *buf, int bufsize, int frame_delay, int size,
	int cap,  int & processed);
static int Breakup_Receive_Packet(void *buf, int bufsize );
int Extract_Uncompressed_Events(void *buf, int bufsize);
int Extract_Compressed_Events(void *buf, int bufsize);

//...........................................................................
// DoList management:
//...........................................................................
int Execute_DoList(int max_houses, HousesType base_house,
	ConnManClass *net, CDTimerClass<FrameTimerClass> *skip_crc,
//	ConnManClass *net, TCountDownTimerClass *skip_crc,
	FrameSyncStruct *their);
static void Clean_DoList(ConnManClass *net);
static void Queue_Record(void);
static void Queue_Playback(void);

//...........................................................................
// Debugging:
//...........................................................................
static void Compute_Game_CRC(void);
void Add_CRC(unsigned int *crc, unsigned int val);
void Print_CRCs(EventClass * ev);
static void Init_Queue_Mono(ConnManClass *net);
static void Update_Queue_Mono(ConnManClass *net, int flow_index);
static void Print_Framesync_Values(int curframe, unsigned int max_ahead,
	int num_connections, FrameSyncStruct *their, unsigned short my_sent);

void Dump_Packet_Too_Late_Stuff(EventClass *event);
void Check_Mirror(void);


/**************************************************************************  *
 * Queue_Mission -- Queue a mega mission event.                              *
 *                                                                           *
 * This routine is called when the player causes a change to a game unit.    *
 * The event that initiates the change is queued to as a result of a call    *
 * to this routine.                                                          *
 *                                                                           *
 * INPUT:                                                                    *
 *      whom      Whom this mission request applies to (a friendly unit).    *
 *    mission   The mission to assign to this object.                        *
 *    target   The target of this mission (if any).                          *
 *    dest      The movement destination for this mission (if any).          *
 *                                                                           *
 * OUTPUT:                                                                   *
 *      Was the mission request queued successfully?                         *
 *                                                                           *
 * WARNINGS:                                                                 *
 *      none.                                                                *
 *                                                                           *
 * HISTORY:                                                                  *
 *   09/21/1995 JLB : Created.                                               *
 *=========================================================================*/
bool Queue_Mission(TargetClass whom, MissionType mission, TargetClass & target, TargetClass & destination)
{
	if (! OutList.Add(EventClass(PlayerPtr->HeapID, whom, mission, target, destination, TargetClass()))) { // RA2: as above
		return(false);
	} else {
		return(true);
	}
}


/***********************************************************************************************
 * Queue_Mission -- Queue a mega mission event, formation override for common speed.           *
 *                                                                                             *
 *    This routine is called when the player causes a change to a game unit. The event that    *
 *    initiates the change is queued to as a result of a call to this routine.                 *
 *                                                                                             *
 * INPUT:   whom     -- Whom this mission request applies to (a friendly unit).                *
 *                                                                                             *
 *          mission  -- The mission to assign to this object.                                  *
 *                                                                                             *
 *          target   -- The target of this mission (if any).                                   *
 *                                                                                             *
 *          dest     -- The movement destination for this mission (if any).                    *
 *                                                                                             *
 *            speed      -- The override speed for this unit.                                  *
 *                                                                                             *
 *            maxspeed -- The override maximum speed for this unit.                            *
 *                                                                                             *
 * OUTPUT:  Was the mission request queued successfully?                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Queue_Mission(TargetClass whom, MissionType mission, TargetClass & target, TargetClass & destination, SpeedType speed, MPHType maxspeed)
{
	if (! OutList.Add(EventClass(PlayerPtr->HeapID, whom, mission, target, destination, speed, maxspeed))) {
		return(false);
	} else {
		return(true);
	}
}


/***************************************************************************
 * Queue_Options -- Queue the options event.                               *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      Was the options screen event queued successfully?                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
bool Queue_Options(void)
{
	if (PlayerPtr->IsToWin || PlayerPtr->IsToLose || PlayerPtr->IsToDie) {
		return(false);
	}

	if (! OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::OPTIONS))) {
		return(false);
	}
	else {
		return(true);
	}

}		/* end of Queue_Options */


/// <summary>
/// Adds the page-user event to the outgoing queue.
/// This routine is used to nudge the other player when the game is sitting around waiting
/// on them. A player whose own game has already been decided has nothing to page about.
/// </summary>
/// <returns>bool; Was the event queued?</returns>
bool Queue_PageUser(void)
{
	if (PlayerPtr->IsToWin || PlayerPtr->IsToLose || PlayerPtr->IsToDie) {
		return(false);
	}

	if (! OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::PAGEUSER))) {
		return(false);
	}
	else {
		return(true);
	}

}		/* end of Queue_PageUser */


/***************************************************************************
 * Queue_Exit -- Add the exit game event to the queue.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      Was the exit event queued successfully?                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
bool Queue_Exit(void)
{
	unsigned char type = EventClass::EXIT; // RA2: as above
	if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter && Session.Players.Count() > 2 && Session.Am_I_Master()) { // RA2: as above
		type = EventClass::ABOUT_TO_EXIT; // RA2: as above
	}
	if (! OutList.Add(EventClass(PlayerPtr->HeapID, type))) { // RA2: as above
		return(false);
	}
	else {
		return(true);
	}
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList1(ConnManClass *net) // RA2: as above
{
	Clean_DoList(net);
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList2(ConnManClass *net) // RA2: as above
{
	_Clean_DoList1(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList3(ConnManClass *net) // RA2: as above
{
	_Clean_DoList2(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList4(ConnManClass *net) // RA2: as above
{
	_Clean_DoList3(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList5(ConnManClass *net) // RA2: as above
{
	_Clean_DoList4(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList6(ConnManClass *net) // RA2: as above
{
	_Clean_DoList5(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList7(ConnManClass *net) // RA2: as above
{
	_Clean_DoList6(net); // RA2: as above
}


/// <summary>
/// RA2
/// </summary>
static inline void _Clean_DoList8(ConnManClass *net) // RA2: as above
{
	_Clean_DoList7(net); // RA2: as above
}


/***************************************************************************
 * Queue_AI -- Process all queued events.                                  *
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
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
void Queue_AI(void)
{
	if (Session.Play) {
		Queue_Playback();
	}

	else {

		switch (Session.Type) {

			case GAME_SKIRMISH:
			case GAME_NORMAL:
				Queue_AI_Normal();
				break;

			case GAME_MODEM:
			case GAME_NULL_MODEM:
			case GAME_IPX:
			case GAME_INTERNET:
				Queue_AI_Multiplayer();
				break;
		}
	}
}


static int SyncNameButtonControlsIDs[MAX_PLAYERS] = {
	IDC_DISCONNECT_PLAYER1,
	IDC_DISCONNECT_PLAYER2,
	IDC_DISCONNECT_PLAYER3,
	IDC_DISCONNECT_PLAYER4,
	IDC_DISCONNECT_PLAYER5,
	IDC_DISCONNECT_PLAYER6,
	IDC_DISCONNECT_PLAYER7,
	IDC_DISCONNECT_PLAYER8
};
static int SyncBarControlIDs[MAX_PLAYERS] = {
	IDC_DISCONNECT_PLAYER1_BOX,
	IDC_DISCONNECT_PLAYER2_BOX, // RA2: the table runs 1608 to 1615
	IDC_DISCONNECT_PLAYER3_BOX,
	IDC_DISCONNECT_PLAYER4_BOX,
	IDC_DISCONNECT_PLAYER5_BOX,
	IDC_DISCONNECT_PLAYER6_BOX,
	IDC_DISCONNECT_PLAYER7_BOX,
	IDC_DISCONNECT_PLAYER8_BOX
};
#define PING_TIMER 1
#define PING_TIMEOUT 1000
#define WM_RECONNECT_SETUP (WM_USER + 0x97) // RA2: the message that fills the reconnect dialog in

/***************************************************************************
 * Queue_AI_Multiplayer -- Process all queued events.                      *
 *                                                                         *
 * This is the network version of the queue management routine.  It does   *
 * the following:                                                          *
 * - If this is the 1st frame, waits for other systems to signal ready     *
 * - Generates a timing event, to allow the connection time to be dynamic  *
 * - Handles timing related to sending packets every 'n' frames            *
 * - Sends outgoing events                                                 *
 * - Frame-syncs to the other systems (see below)                          *
 * - Executes & cleans out the DoList                                      *
 *                                                                         *
 * The Frame-Sync'ing logic is the heart & soul of network play.  It works *
 * by ensuring that any system won't out-run the other system by more than *
 * 'Session.MaxAhead' frames; this in turn ensures that a packet's         *
 * execution frame # won't have been passed by the time that packet is     *
 * received by all systems.                                                *
 *                                                                         *
 * To achieve this, the system must keep track of all other system's       *
 * current frame #'s; these are stored in an array called 'their_frame[]'. *
 * However, because current frame #'s are sent in FRAMEINFO packets, which *
 * don't require an ACK, and command packets are sent in packets requiring *
 * an ACK, it's possible for a command packet to get lost, and the next    *
 * frame's FRAMEINFO packet to not get lost; the other system may then     *
 * advance past the frame # the command is to execute on!  So, to prevent  *
 * this, all FRAMEINFO packets include a CommandCount field.  This value   *
 * tells the other system how many events it should have received by this  *
 * time.  This system can therefore keep track of how many commands it's   *
 * actually received, and compare it to the CommandCount field, to see if  *
 * it's missed an event packet.  The # of events we've received from each  *
 * system is stored in 'their_recv[]', and the # events they say they've   *
 * sent is stored in 'their_sent[]'.                                       *
 *                                                                         *
 * Thus, two conditions must be met in order to advance to the next frame: *
 * - Our current frame # must be < their_frame + Session.MaxAhead          *
 * - their_recv[i] must be >= their_sent[i]                                *
 *                                                                         *
 * 'their_frame[] is updated by Process_Receive_Packet()                   *
 * 'their_recv[] is updated by Process_Receive_Packet()                    *
 * 'their_sent[] is updated by Process_Receive_Packet()                    *
 * 'my_sent' is updated by this routine.                                   *
 *                                                                         *
 * The order of the arrays their_frame[] etc is the same order the         *
 * connections are created in.  The Sender's ID is passed to               *
 * Connection_Index() to obtain the array index.                           *
 *                                                                         *
 * The only routines allowed to pop up dialogs are:                        *
 *    Wait_For_Players() (only pops up the reconnect dialog)               *
 *    Execute_DoList() (tells if out of sync, or packet recv'd too late)   *
 *                                                                         *
 * Sign-off's are detected by:                                             *
 * - Timing out while waiting for a packet                                 *
 * - Detecting that the other player is now at the score screen or         *
 *   connection dialog (serial)                                            *
 * - If we see an EventClass::EXIT event on the private channel            *
 *                                                                         *
 * The current communications protocol, COMM_PROTOCOL_MULTI_E_COMP, has    *
 * the following properties:                                               *
 * - It compresses packets, so that the minimum number of bytes are        *
 *   transmitted.  Packets are compressed by extracting all info common to *
 *   the events into the packet header, and then sending only the bytes    *
 *   relevant to each type of event.  For instance, if 100 infantry guys   *
 *   are told to move to the same location, the command itself & the       *
 *   location will be included in the 1st movement command only; after     *
 *   that, there will be a rep count then 99 infantry TARGET numbers,      *
 *   identifying all the infantry told to move.                            *
 * - The protocol also only sends packets out every 'n' frames.  This cuts *
 *   the data rate dramatically.  It means that 'Session.MaxAhead' must be *
 *   divisible by 'n'; also, the minimum value for 'Session.MaxAhead' is   *
 *   'n * 2', to give both sides some "breathing" room in case a FRAMEINFO *
 *   packet gets missed.                                                   *
 *                                                                         *
 * Note:  For synchronization-waiting loops (like waiting to hear from all *
 * other players, waiting to advance to the next frame, etc), use          *
 * Net.Num_Connections() rather than Session.NumPlayers; this reflects the *
 * actual # of connections, and can be "faked" into playing even when      *
 * there aren't any other players actually there.  A typical example of    *
 * this is playing back a recorded game.  For command-execution loops, use *
 * Session.NumPlayers.  This ensures all commands get executed, even if    *
 * there isn't a human generating those commands.                          *
 *                                                                         *
 * The modem works a little differently from the network in this respect:  *
 * - The connection has to stay "alive" even if the other player exits to  *
 *   the join dialog.  This prevents each system from timing out & hanging *
 *   the modem up.  Thus, packets are sent back & forth & just thrown away,*
 *   but each system knows the other is still there.  Messages may be sent *
 *   between systems, though.                                              *
 * - Destroy_Null_Connection doesn't hang up the modem, so                 *
 *   Num_Connections() still reports a value of 1 even though the other    *
 *   player has left.                                                      *
 * - Any waits on Num_Connections() must also check for                    *
 *   Session.NumPlayers > 1, to keep from waiting forever if the other     *
 *   guy has left                                                          *
 * - Packets sent to a player who's left require no ACK                    *
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
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Queue_AI_Multiplayer(void)
{
	if (Session.Type == GAME_SKIRMISH) return;

	//........................................................................
	// Enums:
	//........................................................................
	static struct {
		int MIXFILE_RESEND_DELTA;	// ticks b/w resends
		int FRAMESYNC_DLG_TIME;		// timeout waiting for mixfiles.
		int FRAMESYNC_TIMEOUT;		// time until displaying reconnect dialog
		int MIXFILE_TIMEOUT;		// timeout waiting for frame sync packet
	} _timings[6] = { // RA2: six rows, the internet row is 120/1800/720/7200 (0x7EC438)
		{ 0, 0, 0, 0 },
		{ 120, 900, 420, 3600 },
		{ 120, 900, 420, 3600 },
		{ 60, 900, 420, 2400 },
		{ 120, 1800, 720, 7200 },
		{ 0, 0, 0, 0 }
	};

	//........................................................................
	// Variables for sending, receiving & parsing packets:
	//........................................................................
	ConnManClass *net;					// ptr to access all multiplayer functions
	char *multi_packet_buf;				// buffer for sending/receiving
	int multi_packet_max;				// max length of multi_packet_buf
	bool is_error;
	static CDTimerClass<FrameTimerClass> skip_crc;	// to delay the CRC check // RA2: a static local; the exit helper destroys it

	//........................................................................
	// Other misc variables
	//........................................................................
	int i;
	RetcodeType rc;

	//------------------------------------------------------------------------
	// Initialize the packet buffer pointer & its max size
	//------------------------------------------------------------------------
	if (Session.Type >= GAME_IPX && Session.Type <= GAME_INTERNET) {
		multi_packet_buf = Session.MetaPacket;
		multi_packet_max = Session.MetaSize;
		net = &Ipx;
	} else {
		multi_packet_buf = NullModem.BuildBuf;
		multi_packet_max = NullModem.MaxLen - sizeof (CommHeaderType);
		net = &NullModem;
	}

	//------------------------------------------------------------------------
	// Compute the Game's CRC
	//------------------------------------------------------------------------
	Compute_Game_CRC();
	CRC[Frame & (ARRAY_SIZE(CRC) - 1)] = GameCRC;

	//------------------------------------------------------------------------
	// If we've just started a game, or loaded a multiplayer game, we must
	// wait for all other systems to signal ready.
	//------------------------------------------------------------------------
	if (Frame==0 || Session.LoadGame) {
		//.....................................................................
		// Initialize static locals
		//.....................................................................
		for (i = 0; i < MAX_PLAYERS - 1; i++) {
			TheirFrameSync[i].frame = -1;
			TheirFrameSync[i].sent = 0;
			TheirFrameSync[i].recv = 0;
			TheirFrameSync[i].timing = 0; // RA2: as above
			TheirFrameSync[i].heard = 0; // RA2: as above
			TheirFrameSync[i].response = 20; // RA2: as above
		}
		SentCommandCount = 0;
		skip_crc = Frame + ARRAY_SIZE(CRC);
		for (i = 0; i < ARRAY_SIZE(CRC); i++)
			CRC[i] = 0;
		for (i = 0; i < ARRAY_SIZE(MegamissionFrames); i++) // RA2: the repeated-megamission cache starts empty
			MegamissionFrames[i] = 0; // RA2: as above
		Reset_Reconnect_Pings(); // RA2: the reconnect dialog's ping counters start over
		for (i = 0; i < ARRAY_SIZE(MegamissionCounts); i++) // RA2: as above
			MegamissionCounts[i] = 0; // RA2: as above
		for (i = 0; i < 32; i += 4) // RA2: as above
			for (int j = 0; j < 8; j++) // RA2: as above
				*(int *)((char *)&SavedUnitCounts[j * ARRAY_SIZE(MegamissionCounts)] + i) = 0; // RA2: as above

		//.....................................................................
		// If we've loaded a saved game:
		// - If this game was saved as the result of a lost connection, clear
		//   the CRC value so it will always match the other system's
		// - Otherwise, use the GameCRC value, so we'll compare save-game files
		//   rather than scenario INI files
		//.....................................................................
		if (Session.LoadGame) {
			if (Session.EmergencySave)
				ScenarioCRC = 0;
			else
				ScenarioCRC = GameCRC;
		}

		//.....................................................................
		// Send our initial FRAMESYNC packet
		//.....................................................................
		SentFrameSyncCount = 0;
		SentFrameSyncTimer = BasicTimerClass<SystemTimerClass>();
		Send_FrameSync(net, SentCommandCount);

		//.....................................................................
		// Wait for the other guys
		//.....................................................................
		rc = Wait_For_Players (1, net, _timings[Session.Type].MIXFILE_RESEND_DELTA, _timings[Session.Type].FRAMESYNC_DLG_TIME,
			_timings[Session.Type].MIXFILE_TIMEOUT, multi_packet_buf, SentCommandCount, TheirFrameSync);

		if (rc != RC_NORMAL) {
			if (Session.Type == GAME_INTERNET){
				Register_Game_End_Time();
			}
			is_error = true;
			Session.Suspended++;
			if (rc == RC_NOT_RESPONDING) {
				WWMessageBox().Process (WAIT_TEXT("TXT_SYSTEM_NOT_RESPONDING", 1037), WAIT_TEXT("TXT_OK", 1037), NULL, NULL, false); // RA2: wide text from the string table
			}
			else if (rc == RC_SCENARIO_MISMATCH) {
				WWMessageBox().Process (WAIT_TEXT("TXT_SCENARIOS_DO_NOT_MATCH", 1040), WAIT_TEXT("TXT_OK", 1040), NULL, NULL, false); // RA2: as above
			}
			else if (rc == RC_DOLIST_FULL) {
				WWMessageBox().Process(WAIT_TEXT("TXT_QUEUE_FULL", 1043), WAIT_TEXT("TXT_OK", 1043), NULL, NULL, false); // RA2: as above
			}
			else if (rc == RC_CANCEL) {
				is_error = false;
			}
			else if (rc == RC_DISCONNECT_ABORT) { // RA2: the reconnect dialog gave up on a slow connection
				WWMessageBox().Process(WAIT_TEXT("TXT_DISCON_ABORT", 1049), WAIT_TEXT("TXT_OK", 1049), NULL, NULL, false); // RA2: as above
				is_error = false; // RA2: as above
			}
			Session.Suspended--;
			Stop_Game(!is_error);
			return;
		}

		//.....................................................................
		// Re-initialize frame numbers (in case somebody signed off while I was
		//	waiting for MIX files to load; we would have fallen through, but
		//	their frame # would still be -1).
		//.....................................................................
		FrameSyncStruct *timing = TheirFrameSync; // RA2: walked by pointer
		for (i = 0; i < MAX_PLAYERS - 1; i++) {
			Session.PlayerLatency[i] = 0;
		}
		for (; (int)timing < (int)(TheirFrameSync + (MAX_PLAYERS - 1)); timing++) { // RA2: as above (compared as signed)
			timing->frame = 0; // RA2: as above
		}

		//.....................................................................
		// Reset the network response time computation, now that we're both
		// sending data again  (loading MIX files will have introduced
		// deceptively large values).
		//.....................................................................
		net->Reset_Response_Time(true);

		//.....................................................................
		// Initialize the frame timers
		//.....................................................................
		if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
			Process_Send_Period(net);//, 1);
		}

		//.....................................................................
		// Turn off our special load-game flags
		//.....................................................................
		if (Session.LoadGame) {
			Session.EmergencySave = false;
			Session.LoadGame = false;
		}

	} 	// end of Frame 0 wait

	//------------------------------------------------------------------------
	// Adjust connection timing parameters every 128 frames.
	//------------------------------------------------------------------------

	else if ( (Frame & 0x007f) == 0) {
		//
		// All systems will transmit their required process time.
		//
		Generate_Process_Time_Event(net);
	}

	//
	// The game "host" will transmit timing adjustment events.
	//
	if (Session.Am_I_Master() && (Session.PrecalcMaxAhead != 0 || Session.PrecalcDesiredFrameRate != 0 || !(char)Frame)) {
		Generate_Real_Timing_Event(net, SentCommandCount);
	}

	//------------------------------------------------------------------------
	// Only process every 'FrameSendRate' frames
	//------------------------------------------------------------------------
	if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		if (!Process_Send_Period(net)) {	//, 0)) {
			if (IsMono) {
				MonoClass::Disable();
			}
			return;
		}
	}

	//------------------------------------------------------------------------
	// Send our data packet(s); update my command-sent counter
	//------------------------------------------------------------------------
	{
		int sent = (unsigned short)Send_Packets(net, multi_packet_buf, multi_packet_max, // RA2: the count is logged
			Session.MaxAhead, SentCommandCount); // RA2: as above
		SentCommandCount += sent; // RA2: as above
		if (sent) { // RA2: as above
			DebugString("Frame %d, my sent = %d\n", Frame, SentCommandCount); // RA2: as above
		}
	}

	//------------------------------------------------------------------------
	// If this is our first time through, we're done.
	//------------------------------------------------------------------------
	if (Frame==0) {
		if (IsMono) {
			MonoClass::Disable();
		}
		return;
	}

	//------------------------------------------------------------------------
	// Frame-sync'ing: wait until it's OK to advance to the next frame.
	//------------------------------------------------------------------------
	rc = Wait_For_Players (0, net,
	3 * TIMER_SECOND, // RA2: three seconds
	MAX ((int) net->Response_Time() * 3, _timings[Session.Type].FRAMESYNC_TIMEOUT ),
	_timings[Session.Type].MIXFILE_TIMEOUT,
	multi_packet_buf, SentCommandCount, TheirFrameSync);

	if (rc != RC_NORMAL) {
		DebugString("Wait_For_Players returned %d\n", rc);
		if (Session.Type == GAME_INTERNET) {
			Register_Game_End_Time();
		}
		is_error = true;
		Session.Suspended++;
		if (rc == RC_NOT_RESPONDING) {
			WWMessageBox().Process (WAIT_TEXT("TXT_SYSTEM_NOT_RESPONDING", 1195), WAIT_TEXT("TXT_OK", 1195), NULL, NULL, false); // RA2: wide text from the string table
		}
		else if (rc == RC_SCENARIO_MISMATCH) {
			WWMessageBox().Process (WAIT_TEXT("TXT_SCENARIOS_DO_NOT_MATCH", 1199), WAIT_TEXT("TXT_OK", 1199), NULL, NULL, false); // RA2: as above
		}
		else if (rc == RC_DOLIST_FULL) {
			WWMessageBox().Process(WAIT_TEXT("TXT_QUEUE_FULL", 1203), WAIT_TEXT("TXT_OK", 1203), NULL, NULL, false); // RA2: as above
		}
		else if (rc == RC_CANCEL) {
			is_error = false;
		}
		else if (rc == RC_DISCONNECT_ABORT) { // RA2: as above
			WWMessageBox().Process(WAIT_TEXT("TXT_DISCON_ABORT", 1211), WAIT_TEXT("TXT_OK", 1211), NULL, NULL, false); // RA2: as above
			is_error = false; // RA2: as above
		}
		Session.Suspended--;
		Stop_Game(!is_error);
		return;
	}

	//------------------------------------------------------------------------
	// Save the DoList to disk, if we're in "Record" mode
	//------------------------------------------------------------------------
	if (Session.Record) {
		Queue_Record();
	}

	//------------------------------------------------------------------------
	// Execute the DoList; if an error occurs, bail out.
	//------------------------------------------------------------------------
	if (!Execute_DoList(Session.MaxPlayers, HOUSE_FIRST, net, &skip_crc, TheirFrameSync)) {
		DebugString("Failure executing DoList\n");
		if (Session.Type == GAME_INTERNET){
			Register_Game_End_Time();
		}
		Stop_Game();
		return;
	}

	//------------------------------------------------------------------------
	// Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(net);

	if (IsMono) {
		MonoClass::Disable();
	}

}	// end of Queue_AI_Multiplayer


/// <summary>
/// Waits for the other players to reach the end of the game.
/// This routine is called once the game is decided but the other systems may still be a
/// few frames behind. It keeps the connections serviced, the frame syncs flowing and the
/// map rendering until everybody has caught up, then signs off with a flurry of frame
/// syncs so nobody is left waiting on us. A player who never answers is given up on rather
/// than waited for forever.
/// </summary>
void Wait_For_End_Of_Queue(void)
{
	//........................................................................
	// Variables for sending, receiving & parsing packets:
	//........................................................................
	ConnManClass *net;					// ptr to access all multiplayer functions
	char *multi_packet_buf;				// buffer for sending/receiving
	int multi_packet_max;				// max length of multi_packet_buf
	int packetlen;          // size of meta-packet sent, & received
	int id;                 // id of other player
	KeyNumType key;			// RA2: as above
	int x,y;
	int i;
	int their_oldest_frame;			// other players' oldest frame #
	int their_oldest_index;
	RetcodeType rc;

	DebugString("Waiting to exit game\n");

	//------------------------------------------------------------------------
	// Initialize the packet buffer pointer & its max size
	//------------------------------------------------------------------------
	if (Session.Type >= GAME_IPX && Session.Type <= GAME_INTERNET) {
		multi_packet_buf = Session.MetaPacket;
		net = &Ipx;
		multi_packet_max = Session.MetaSize;
	} else {
		multi_packet_buf = NullModem.BuildBuf;
		net = &NullModem;
		multi_packet_max = NullModem.MaxLen - sizeof (CommHeaderType);
	}

	//------------------------------------------------------------------------
	// Send our data packet(s); update my command-sent counter
	//------------------------------------------------------------------------
	SentCommandCount += Send_Packets(net, multi_packet_buf, multi_packet_max,
		Session.MaxAhead, SentCommandCount);

	CDTimerClass<SystemTimerClass> timer1 = TIMER_SECOND / 4;
	CDTimerClass<SystemTimerClass> timer2 = 10 * TIMER_SECOND;
	BasicTimerClass<SystemTimerClass> timer3;

	DebugString("Entering wait loop\n");

	while (true) {

		Keyboard->Check();

		//---------------------------------------------------------------------
		// Resend a frame-sync packet if longer than one propagation delay goes
		// by; this prevents a "deadlock".  If he's waiting for me to advance,
		// but has missed my last few FRAMEINFO packets, I may be waiting for
		// him to advance.  Resending a FRAMESYNC ensures he knows what frame
		// number I'm on.
		//---------------------------------------------------------------------
		if (!timer1) {
			timer1 = TIMER_SECOND / 4;		// time to retry
			DebugString("Resending framesync\n");
			Send_FrameSync(net, SentCommandCount);
		}

		//---------------------------------------------------------------------
		// Service the connections
		//---------------------------------------------------------------------
		net->Service();

		if (!timer2) {
			DebugString("Timed out waiting for end game!\n");
			return;
		}


		//---------------------------------------------------------------------
		// Check for an incoming message.  We must still process commands
		// even if 'first_time' is set, in case the other system got my 1st
		// FRAMESYNC, but I didn't get his; he'll be at the next frame, and
		// may be sending commands.
		// We have to limit the number of incoming messages we handle; it's
		// possible to go into an infinite loop processing modem messages.
		// (This feature is disabled for Ten; we need to keep the TCP buffers
		// clear, so we read all the packets we can every time.)
		//---------------------------------------------------------------------
		int messages_this_loop = 1; // RA2: the limit is tested after a message was handled
		while (true) {
			if (!net->Get_Private_Message (multi_packet_buf, &packetlen, &id)) { // RA2: as above
				break; // RA2: as above
			}

			Keyboard->Check();

			//------------------------------------------------------------------
			// Special processing for a modem game: process SERIAL packets
			//------------------------------------------------------------------
			if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
				rc = Process_Serial_Packet(multi_packet_buf, false);
				switch (rc) {
					case RC_NORMAL:
						break;
					case RC_PLAYER_READY:
					case RC_SCENARIO_MISMATCH:
					case RC_DOLIST_FULL:
						return;

					//...............................................................
					// SERIAL packet received & processed
					//...............................................................
					case RC_SERIAL_PROCESSED:
						net->Service();
						timer1 = TIMER_SECOND / 4;
						timer2 = 10 * TIMER_SECOND;
						break;

					case RC_PLAYER_LEFT:
					case RC_HUNG_UP:
						return;

					//...............................................................
					// If it was any other type of serial packet, break
					//...............................................................
					default:
						return;
				}
			} else {

				//------------------------------------------------------------------
				// Process the incoming packet
				//------------------------------------------------------------------
				rc = Process_Receive_Packet(net, multi_packet_buf, id, packetlen, TheirFrameSync, &timer3);
				//..................................................................
				// DoList was full
				//..................................................................
				if (rc == RC_DOLIST_FULL) {
					DebugString("Dolist is full!\n");
					return;
				}
			}

			//..................................................................
			// Service the connection, to clean out the receive queues
			//..................................................................
			net->Service();
			if (messages_this_loop++ >= 64) { // RA2: as above, at most 64 messages per pass
				break; // RA2: as above
			}
		}

		Call_Back();

		if (SpecialDialog == SDLG_NONE) {
			Map.Input(key, x, y);
			Keyboard_Process(key); // RA2: the key is passed on without a test
			TacticalMap->AI();
			Map.Render();
		}

		//------------------------------------------------------------------------
		//	Find the oldest frame # in 'their_frame'
		//------------------------------------------------------------------------
		their_oldest_frame = Frame + 1000;
		their_oldest_index = -1;
		for (i = 0; i < net->Num_Connections(); i++) {
			if (TheirFrameSync[i].frame < their_oldest_frame) {
				their_oldest_frame = TheirFrameSync[i].frame;
				their_oldest_index = i;
			}
		}

		if (int(their_oldest_frame + Session.FrameSendRate) >= Frame) { // RA2: the finish is inside the loop
			DebugString("their_oldest_frame = %d\n", their_oldest_frame);
			DebugString("All players have finished - ready to proceed to score screen\n");

			Send_FrameSync(net, SentCommandCount);
			Send_FrameSync(net, SentCommandCount);
			Send_FrameSync(net, SentCommandCount);
			Send_FrameSync(net, SentCommandCount);
			return; // RA2: as above
		}
		DebugString("Waiting for player %d on frame %d\n", their_oldest_index, TheirFrameSync[their_oldest_index].frame); // RA2: as above
	}
}


static RetcodeType Wait_For_Players(int first_time, ConnManClass *net,
	int resend_delta, int dialog_time, int timeout, char *multi_packet_buf,
	int my_sent, FrameSyncStruct *their)
{
	//........................................................................
	// Variables for sending, receiving & parsing packets:
	//........................................................................
	int packetlen;          // size of meta-packet sent, & received
	int id;                 // id of other player
	int num_ready;							// # players signalling ready
	int loop_count;						/// # of times through the wait loop
	int messages_this_loop; // to limit # messages processed each loop
	KeyNumType input;   // for user input
	int x,y;            // for map input
	int i;
	RetcodeType rc;

	/*
	 * ........................................................................
	 * Variables for the frame-sync stall display
	 * ........................................................................
	 */
	bool stall_drawn = false;			/// true = the current stall has been recorded
	Surface *surface = VisibleSurface; // RA2: the stall bars are drawn on the visible surface

	//........................................................................
	// Timing variables
	//........................................................................
	CDTimerClass<SystemTimerClass> retry_timer;		// time between FRAMESYNC packet resends
	CDTimerClass<SystemTimerClass> dialog_timer;	// time to pop up a dialog
	CDTimerClass<SystemTimerClass> timeout_timer;	// general-purpose timeout
	BasicTimerClass<SystemTimerClass> timer;
	CDTimerClass<SystemTimerClass> progress_timer;	/// time between load progress updates

	//........................................................................
	// Dialog variables
	//........................................................................
	int reconnect_dlg = 0;				// 1 = the reconnect dialog is displayed

	CDTimerClass<SystemTimerClass> unused_timer1 = dialog_time * 2; // RA2: two timers the executable starts and never reads
	CDTimerClass<SystemTimerClass> unused_timer2 = dialog_time / 2; // RA2: as above

	//------------------------------------------------------------------------
	// Wait to hear from all other players
	//------------------------------------------------------------------------
	num_ready = 0;
	retry_timer = resend_delta; // time to retry
	dialog_timer = dialog_time; // time to show dlg
	timeout_timer = timeout;    // time to bail out
	timer = BasicTimerClass<SystemTimerClass>();
	progress_timer = 3 * TIMER_SECOND; // RA2: three seconds
	CDTimerClass<SystemTimerClass> keepalive_timer = 5 * TIMER_SECOND; // RA2: a keep-alive timer, only reset
	loop_count = 0;

	unsigned long now = timeGetTime(); // RA2: every connection starts out as just heard from
	for (i = 0; i < net->Num_Connections(); i++) { // RA2: as above
		their[i].heard = now; // RA2: as above
	}

	while (1) {
		Keyboard->Check();

		//---------------------------------------------------------------------
		// Resend a frame-sync packet if longer than one propagation delay goes
		// by; this prevents a "deadlock".  If he's waiting for me to advance,
		// but has missed my last few FRAMEINFO packets, I may be waiting for
		// him to advance.  Resending a FRAMESYNC ensures he knows what frame
		// number I'm on.
		//---------------------------------------------------------------------
		if (!retry_timer) {
			retry_timer = resend_delta;		// time to retry
			Send_FrameSync(net, my_sent);

			//------------------------------------------------------------------
			// RA2: behind a packet router the other players are told we are still here
			//------------------------------------------------------------------
			if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {
				Ipx.Clear_Global_Routed();
				DebugString("Time to send IM_STILL_HERE packets\n");
				GlobalPacketType packet;
				packet.Command = NET_IM_STILL_HERE;
				strncpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name), 20);
				packet.StillHere.ID = PlayerPtr->HeapID;
				IPXAddressClass address = Session.PacketRouterAddress;
				address.Set_Port(Session.PacketRouterPort);
				*(IPXAddressClass *)packet.StillHere.Address = address;
				for (i = 1; i < Session.Players.Count(); i++) {
					int id2 = Session.Players[i]->Player.ID;
					if (id2 != PlayerPtr->HeapID) {
						Ipx.Send_Global_Message(&packet, 0x5B, 0,
							&Session.Players[i]->Address, Session.PlayerPorts[i], 0);
					}
				}
				Call_Back();
			}
		}

		//---------------------------------------------------------------------
		/*
		 * While we wait for the other system to finish loading the scenario,
		 * pulse the loading progress bar periodically.
		 */
		//---------------------------------------------------------------------
		if (keepalive_timer == 0 && !first_time) { // RA2: as above
			keepalive_timer = 5 * TIMER_SECOND; // RA2: as above
		} else if (first_time) {
			if (progress_timer == 0) {
				if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM || Session.ViaPacketRouter) {
					DebugString("Resending progress message\n"); // RA2: as above
					if (Session.Type == GAME_INTERNET) { // RA2: as above
						Ipx.Clear_Global_Routed(); // RA2: as above
					}
					Session.Update_Progress(Scen->IsRandom ? 200 : 100);
					progress_timer = 3 * TIMER_SECOND;
				}
			}
		}

		//---------------------------------------------------------------------
		// Service the connections
		//---------------------------------------------------------------------
		net->Service();

		//---------------------------------------------------------------------
		// Pop up a reconnect dialog if enough time goes by
		//---------------------------------------------------------------------
		if (!dialog_timer) {
			if (Session.Type == GAME_INTERNET) { // RA2: the global channel is cleared first
				Ipx.Clear_Global_Routed(); // RA2: as above
			}
			int fresh = (reconnect_dlg==0);
			int reconn = (first_time==0);

			if (Process_Reconnect_Dialog(net, &timeout_timer, their, // RA2: the connection manager is passed
				net->Num_Connections(), reconn, fresh, &timer)) {
				WS_Destroy_Dialog(ReconnectDialog, 0); // RA2: the dialog is closed here
				if (ReconnectMouseCaptured) { // RA2: as above
					MouseCursor->Capture_Mouse(); // RA2: as above
				}
				TacticalActive = ReconnectSavedTactical; // RA2: as above
				Map.Flag_To_Redraw(GS_REDRAW_ALL); // RA2: as above
				if (reconnect_dlg) {
					Close_Reconnect_Dialog();
				}
				if (Session.Type == GAME_INTERNET) { // RA2: as above
					Ipx.Set_Global_Routed(); // RA2: as above
				}
				if (!ReconnectPingTimedOut) { // RA2: as above
					Send_Port_Unreachable(); // RA2: as above
					return(RC_CANCEL);
				}
				Send_Port_Unreachable(); // RA2: as above
				return(RC_DISCONNECT_ABORT); // RA2: as above
			}

			reconnect_dlg = 1;
		}

		//---------------------------------------------------------------------
		// Exit if too much time goes by (the other system has crashed or
		// bailed)
		//---------------------------------------------------------------------
		if (!timeout_timer) {
			//..................................................................
			// For the first-time run, just give up; something's wrong.
			//..................................................................
			if (first_time) {
				if (reconnect_dlg) {
					Close_Reconnect_Dialog();
				}
				Send_Port_Unreachable(); // RA2: as above
				if (Session.Type == GAME_INTERNET) { // RA2: as above
					Ipx.Set_Global_Routed(); // RA2: as above
				}
				return(RC_NOT_RESPONDING);
			}
			//..................................................................
			// Otherwise, we're in the middle of a game; so, the modem &
			// network must deal with a timeout differently.
			//..................................................................
			else {
				if (Handle_Timeout(net, their)) {
					Map.Flag_To_Redraw(GS_REDRAW_ALL);	// erase modem reconnect dialog
					Map.Render();
					retry_timer = resend_delta;
					dialog_timer = dialog_time;
					timeout_timer = timeout;
				} else {
					if (reconnect_dlg) {
						Close_Reconnect_Dialog();
					}
					if (Session.Type == GAME_INTERNET) { // RA2: as above
						Ipx.Set_Global_Routed(); // RA2: as above
					}
					return(RC_NOT_RESPONDING);
				}
			}
		}

		//---------------------------------------------------------------------
		// Check for an incoming message.  We must still process commands
		// even if 'first_time' is set, in case the other system got my 1st
		// FRAMESYNC, but I didn't get his; he'll be at the next frame, and
		// may be sending commands.
		// We have to limit the number of incoming messages we handle; it's
		// possible to go into an infinite loop processing modem messages.
		//---------------------------------------------------------------------
		messages_this_loop = 0;

		while ( (messages_this_loop++ < 64) && // RA2: the limit is 64 messages
			net->Get_Private_Message (multi_packet_buf, &packetlen, &id) ) {

			Keyboard->Check();

			//------------------------------------------------------------------
			// Special processing for a modem game: process SERIAL packets
			//------------------------------------------------------------------
			if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
				rc = Process_Serial_Packet(multi_packet_buf, first_time);
				//...............................................................
				// SERIAL packet received & processed
				//...............................................................
				if (rc == RC_SERIAL_PROCESSED) {
					net->Service();
					retry_timer = resend_delta;
					dialog_timer = dialog_time;
					timeout_timer = timeout;
					continue;
				}
				//...............................................................
				// other player has left the game
				//...............................................................
				else if (rc == RC_PLAYER_LEFT) {
					if (first_time) {
						num_ready++;
					}
					break;
				}
				//...............................................................
				// Connection was lost
				//...............................................................
				/// Try to reconnect before giving up.
				else if (rc == RC_HUNG_UP) {
					if (Handle_Timeout(net, their)) {
						Map.Flag_To_Redraw(GS_REDRAW_ALL);
						Map.Render();
						retry_timer = resend_delta;
						dialog_timer = dialog_time;
						timeout_timer = timeout;
					} else {
						if (reconnect_dlg) {
							Close_Reconnect_Dialog();
						}
						if (Session.Type == GAME_INTERNET) { // RA2: as above
							Ipx.Set_Global_Routed(); // RA2: as above
						}
						Send_Port_Unreachable(); // RA2: as above
						return(RC_NOT_RESPONDING);
					}
				}
				//...............................................................
				// If it was any other type of serial packet, break
				//...............................................................
				else if (rc != RC_NORMAL) {
					break;
				}
			}

			//------------------------------------------------------------------
			// Process the incoming packet
			//------------------------------------------------------------------
			rc = Process_Receive_Packet(net, multi_packet_buf, id, packetlen, their, &timer);
			//..................................................................
			// New player heard from
			//..................................................................
			if (rc == RC_PLAYER_READY) {
				num_ready++;
			}
			//..................................................................
			// Scenario's don't match
			//..................................................................
			else if (rc == RC_SCENARIO_MISMATCH) {
				if (reconnect_dlg) {
					Close_Reconnect_Dialog();
				}
				if (Session.Type == GAME_INTERNET) { // RA2: as above
					Ipx.Set_Global_Routed(); // RA2: as above
				}
				return(RC_SCENARIO_MISMATCH);
			}
			//..................................................................
			// DoList was full
			//..................................................................
			else if (rc == RC_DOLIST_FULL) {
				if (reconnect_dlg) {
					Close_Reconnect_Dialog();
				}
				if (Session.Type == GAME_INTERNET) { // RA2: as above
					Ipx.Set_Global_Routed(); // RA2: as above
				}
				Send_Port_Unreachable(); // RA2: as above
				return(RC_DOLIST_FULL);
			}

			//..................................................................
			// Service the connection, to clean out the receive queues
			//..................................................................
			net->Service();
		}

		//---------------------------------------------------------------------
		// Debug output
		//---------------------------------------------------------------------
		if (reconnect_dlg) {
			net->Num_Connections(); // RA2: the framesync values are no longer printed
		}

		//---------------------------------------------------------------------
		// Attempt to advance to the next frame.
		//---------------------------------------------------------------------
		//.....................................................................
		// For the first-time run, just check to see if we've heard from
		// everyone.
		//.....................................................................
		if (first_time) {
			if (num_ready >= net->Num_Connections()) {
				break;
			}
		}
		//.....................................................................
		// For in-game processing, we have to check their_sent, their_recv,
		// their_frame, etc.
		//.....................................................................
		else {
			int frame_stall;					/// who's holding up the frame count
			int count_stall;					/// who's holding up the command count

			if (Can_Advance(net, Session.MaxAhead, their, &frame_stall, &count_stall)) {
				break;
			}

			/*
			 * ..................................................................
			 * Record (and optionally display) who's stalling the game.
			 * ..................................................................
			 */
			bool show = Session.EnableMultiplayerDebug && Session.DrawMPDebugStats && loop_count > 0 && // RA2: as above
				(!stall_drawn || frame_stall != -1 || count_stall != -1); // RA2: as above
			if (show) { // RA2: as above
				Draw_Multiplayer_Debug(true); // RA2: as above
			}
			if (show || !stall_drawn) { // RA2: a stall already recorded is not counted again
				if (frame_stall != -1) {
					if (Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) { // RA2: the stall bars
						surface->Fill_Rect(Rect((frame_stall * 5 + 5) * 20, 400, 35, 3), DSurface::Build_Hicolor_Pixel(255, 255, 40)); // RA2: as above
					}
					Session.ConnectionStats[frame_stall].FrameSyncStalls++;
				}
				if (count_stall != -1) {
					if (Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) { // RA2: as above
						surface->Fill_Rect(Rect(count_stall * 100 + 140, 400, 40, 3), DSurface::Build_Hicolor_Pixel(255, 40, 40)); // RA2: as above
					}
					Session.ConnectionStats[frame_stall].CommandCountStalls++;
				}
				stall_drawn = true;
			}
		}

		//---------------------------------------------------------------------
		// Service game stuff.  Servicing the map's input, and rendering the
		// map, allows the map to scroll even though we're hung up waiting for
		// packets.  Don't do this if 'first_time' is set, since users could be
		// waiting a very long time for all systems to load the scenario, and
		// it gets frustrating being able to scroll around without doing
		// anything.
		//---------------------------------------------------------------------
		Call_Back();
		if (!first_time && SpecialDialog == SDLG_NONE && reconnect_dlg==0) {
			Map.Input(input, x, y);
			Keyboard_Process(input); // RA2: the key is passed on without a test
			TacticalMap->AI();
			Map.Render();
		}

		loop_count++;

	}	/* end of while */

	if (reconnect_dlg) {
		Close_Reconnect_Dialog();
		if (net->Num_Connections() > 0) {
			g_PingsSent = 0;
			g_PingsReceived = 0;
		}
	}
	if (stall_drawn && Session.EnableMultiplayerDebug && Session.DrawMPDebugStats) { // RA2: the stall bars are erased
		Rect rect(100, 400, 540, 4); // RA2: as above
		surface->Fill_Rect(rect, 0); // RA2: as above
	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		Ipx.Set_Global_Routed(); // RA2: as above
	}
	return(RC_NORMAL);

}


static int Send_Packets(ConnManClass *net, char *multi_packet_buf,
	int multi_packet_max, int max_ahead, int my_sent)
{
	int cnt;
	int cap;        // max # events to send, NOT including FRAMEINFO event
	int ack_req;    // 0 = no ack required on outgoing packet
	int packetlen;  // size of meta-packet sent
	int processed;
	int num = 0;

	cap = OutList.Count;

	//........................................................................
	// Make sure we don't send so many events that our DoList fills up
	//........................................................................
	if (cap > (MAX_EVENTS * 64) - DoList.Count) {
		cap = (MAX_EVENTS * 64) - DoList.Count;
	}
	cnt = OutList.Count;

	//------------------------------------------------------------------------
	// Build our meta-packet & transmit it.
	//------------------------------------------------------------------------
	while (1) {
		//Keyboard->Check();

		//.....................................................................
		// If there are no commands this frame, we'll just be sending a FRAMEINFO
		//	packet; no ack is required.  For the modem's sake, check
		// Session.NumPlayers; no ACK is needed if we're just sending to someone
		// who's left the game.
		//.....................................................................
		if (cap == 0 || OutList.Count == 0 || Session.NumPlayers == 1) {
			ack_req = 0;
		}
		else {
			ack_req = 1;
		}

		//.....................................................................
		// Build & send out our message
		//.....................................................................
		packetlen = Build_Send_Packet (multi_packet_buf, multi_packet_max,
			max_ahead, my_sent, cap, processed);

		cap -= processed;
		num += processed;
		if (processed) {
			DebugString("Send_Packets - created packet with %d commands on frame %d\n", processed, Frame); // RA2: logged
			ack_req = 1;
		}

		net->Send_Private_Message (multi_packet_buf, packetlen, ack_req);
		SentFrameSyncCount++;

		//.....................................................................
		// Call Service() to actually send the packet
		//.....................................................................
		net->Service();

		//.....................................................................
		// Stop if there's no more data to send, or if our send queue is
		// filling up.
		//.....................................................................
		if (OutList.Count == 0 || OutList.Count > cnt) {
			break;
		}
		if (cap == 0) {
			break;
		}
	}

	if (num) { // RA2: logged
		DebugString("Send_Packets - added total of %d commands on frame %d\n", num, Frame); // RA2: as above
	}
	return(num);

}


static void Send_FrameSync(ConnManClass *net, int cmd_count)
{
	EventClass packet;

	//------------------------------------------------------------------------
	// Build a frame-sync event to send.  FRAMESYNC packets contain a
	// scenario-based CRC rather than a game-state-based CRC, to let the
	// games compare scenario CRC's on startup.
	//------------------------------------------------------------------------
	memset (&packet, 0, sizeof(EventClass));
	packet.Type = EventClass::FRAMESYNC;
	if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		packet.Frame = ((Frame + Session.MaxAhead + (Session.FrameSendRate - 1)) /
			 Session.FrameSendRate) * Session.FrameSendRate;
	}
	else {
		packet.Frame = Frame + Session.MaxAhead;
	}
	packet.ID = PlayerPtr->HeapID;
	packet.Data.FrameInfo.CRC = ScenarioCRC;
	packet.Data.FrameInfo.CommandCount = cmd_count;
	packet.Data.FrameInfo.Delay = Session.MaxAhead;

	//------------------------------------------------------------------------
	// Send the event.  For modem, this just sends to the other player;
	// for network, it sends to everyone we're connected to.
	//------------------------------------------------------------------------

	net->Send_Private_Message (&packet, (offsetof(EventClass, Data) +
		size_of(EventClass, Data.FrameInfo)), 0 );
	DebugString("Sent FrameSync only packet\n"); // RA2: logged
	SentFrameSyncCount++;
	return;

}


/***************************************************************************
 * Process_Receive_Packet -- processes an incoming packet                  *
 *                                                                         *
 * This routine receives a packet from another system, adds it to our      *
 * execution queue (the DoList), and updates my arrays of their frame #,   *
 * their commands-sent, and their commands-received.                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      net               ptr to connection manager                        *
 *      multi_packet_buf   buffer containing packet(s) to parse            *
 *      id                  id of sender                                   *
 *      their_frame         array containing frame #'s of other players    *
 *      their_sent         array containing command count of other players *
 *      their_recv         array containing # recv'd cmds from other players*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      RC_NORMAL:               nothing unusual happened, although        *
 *                            their_sent or their_recv may have been       *
 *                              altered                                    *
 *      RC_PLAYER_READY:         player has been heard from for the 1st time;*
 *                            this presumes that his original              *
 *                            'their_frame[]' value was -1 when this       *
 *                            routine was called                           *
 *      RC_SCENARIO_MISMATCH:   FRAMEINFO scenario CRC doesn't match;      *
 *                            normally only applies after loading a new    *
 *                            scenario or save-game                        *
 *      RC_DOLIST_FULL:         fatal error; unable to add events to DoList*
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static RetcodeType Process_Receive_Packet(ConnManClass *net,
	char *multi_packet_buf, int id, int packetlen, FrameSyncStruct *their, BasicTimerClass<SystemTimerClass> *timer)
{
	EventClass *event;
	int index;
	RetcodeType retcode = RC_NORMAL;
	int i;

	//------------------------------------------------------------------------
	// Get an event ptr to the incoming message
	//------------------------------------------------------------------------
	event = (EventClass *)multi_packet_buf;

	//------------------------------------------------------------------------
	// Get the index of the sender
	//------------------------------------------------------------------------
	index = net->Connection_Index(id);

	//------------------------------------------------------------------------
	// RA2: a packet router sends the other players' frames and response times
	// in two event types of its own
	//------------------------------------------------------------------------
	if (Session.ViaPacketRouter && (event->Type == EventClass::MEGAFRAMEINFO || event->Type == EventClass::PACKET_TIMING)) { // RA2: as above
		int count = (char)event->ID; // RA2: as above
		unsigned long most = 0; // RA2: as above
		if (event->Type == EventClass::PACKET_TIMING) { // RA2: as above
			DebugString("Received PACKETTIMING packet\n"); // RA2: as above
		} else if (event->Type == EventClass::MEGAFRAMEINFO) { // RA2: as above
			DebugString("Received MEGAFRAMEINFO packet\n"); // RA2: as above
		}
		if (count == 0) { // RA2: as above
			DebugString("MEGAFRAMEINFO is part of a pserver port blitz!\n"); // RA2: as above
		}
		i = 0; // RA2: as above
		if (i < count) { // RA2: as above
			PacketTimingEntry *timing = (PacketTimingEntry *)&event->Data; // RA2: as above
			MegaFrameEntry *mega = (MegaFrameEntry *)&event->Data; // RA2: as above
			for (; i < count; i++, mega++, timing++) { // RA2: as above
				if (event->Type == EventClass::MEGAFRAMEINFO) { // RA2: as above
					int player = net->Connection_Index(mega->ID); // RA2: as above
					if (player == -1) { // RA2: as above
						continue; // RA2: as above
					}
					if (their[player].frame < (long)(mega->Frame - mega->Delay)) { // RA2: as above
						their[player].frame = mega->Frame - mega->Delay; // RA2: as above
						Calc_Frames_Behind(their, player); // RA2: as above
						if (Ipx.Connection_Name(mega->ID)) { // RA2: as above
							DebugString("Got MEGAFRAMEINFO frame update from %s. New frame = %d\n", // RA2: as above
								Wide_To_Narrow(Ipx.Connection_Name(mega->ID)), their[player].frame); // RA2: as above
						}
						if ((unsigned)their[player].sent < mega->CommandCount) { // RA2: as above
							their[player].sent = mega->CommandCount; // RA2: as above
							DebugString("Command count is %d\n", their[player].sent); // RA2: as above
						}
					} else {
						if (Ipx.Connection_Name(mega->ID)) { // RA2: as above
							DebugString("Got useless MEGAFRAMEINFO frame update from %s. frame = %d\n", // RA2: as above
								Wide_To_Narrow(Ipx.Connection_Name(mega->ID)), their[player].frame); // RA2: as above
						}
					}
					if (!(mega->Flags & 1)) { // RA2: as above
						their[player].timing = timer->Value(); // RA2: as above
					}
				} else {
					if (timing->ID == PlayerPtr->HeapID) { // RA2: as above
						unsigned long response = timing->ResponseTime; // RA2: as above
						Session.RouterResponse = response; // RA2: as above
						DebugString("My router response time = %d ms\n", response * 1000 / TIMER_SECOND); // RA2: as above
						continue; // RA2: as above
					}
					int player = net->Connection_Index(timing->ID); // RA2: as above
					if (player == -1) { // RA2: as above
						continue; // RA2: as above
					}
					unsigned long response = timing->ResponseTime; // RA2: as above
					their[player].response = response; // RA2: as above
					RouterResponseTimes[player] = response; // RA2: as above
					if (most <= response) { // RA2: as above
						most = response; // RA2: as above
					}
					if (timing->Report && Session.Am_I_Master() && Session.LastTimingID != timing->ID) { // RA2: as above
						OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::FALLBACK_HOST, (int)timing->ID)); // RA2: as above
					}
				}
			}
		}
		if (count && most) { // RA2: as above
			Session.SlowestResponse = most; // RA2: as above
		}
	} else {

	//------------------------------------------------------------------------
	//	Compute the other player's frame # (at the time this packet was sent)
	//------------------------------------------------------------------------
	FrameSyncStruct *theirs = &their[index]; // RA2: as above
	if (theirs->frame < (long)(event->Frame - event->Data.FrameInfo.Delay)) {

		//.....................................................................
		// If the original frame # for this player is -1, it means we've heard
		// from this player for the 1st time; return the appropriate value.
		//.....................................................................
		if (theirs->frame==-1) {
			retcode = RC_PLAYER_READY;
			DebugString("Player ready\n"); // RA2: logged
		}

		theirs->frame = event->Frame - event->Data.FrameInfo.Delay;
		Calc_Frames_Behind(their, index); // RA2: the latency is worked out out of line
	}

	//------------------------------------------------------------------------
	// Extract the other player's CommandCount.  This count will include
	// the commands in this packet, if there are any.
	//------------------------------------------------------------------------
	if ((unsigned short)event->Data.FrameInfo.CommandCount > (unsigned)theirs->sent) { // RA2: the bad-count log file is gone
		theirs->sent = (unsigned short)event->Data.FrameInfo.CommandCount; // RA2: as above
	}

	//------------------------------------------------------------------------
	// If this packet was not a FRAMESYNC packet:
	// - Add the events in it to our DoList
	// - Increment our commands-received counter by the number of non-
	//   FRAMEINFO packets received
	//------------------------------------------------------------------------
	if (event->Type != EventClass::FRAMESYNC) {
		//.....................................................................
		// Break up the packet into its component events.  A returned packet
		// count of -1 indicates a fatal queue-full error.
		//.....................................................................
		i = Breakup_Receive_Packet( event, packetlen);
		if (i==-1) {
			DebugString("***** Error DOLIST_FULL! ******\n"); // RA2: logged
			return(RC_DOLIST_FULL);
		}
		//.....................................................................
		// Compute the actual # commands in the packet by subtracting off the
		// FRAMEINFO event
		//.....................................................................
		if ( (event->Type==EventClass::FRAMEINFO) && (i > 0)) {
			i--;
		}

		DebugString("Frame %d, Receive packet uncompressed into %d commmands\n", Frame, i); // RA2: logged
		theirs->recv += (unsigned short)i; /// This mask should not be necessary.
	}

	//------------------------------------------------------------------------
	// If the event was a FRAMESYNC packet, there will be no commands to add,
	// but we must check the ScenarioCRC value.
	//------------------------------------------------------------------------
	else {
		if (Session.Type == GAME_INTERNET || Session.Type == GAME_IPX) { // RA2: logged
			DebugString("Received FRAMESYNC packet from %s. Frame = %d\n", Ipx.Connection_Name(id), // RA2: as above
				event->Frame - event->Data.FrameInfo.Delay); // RA2: as above
		}
		if (event->Data.FrameInfo.CRC != ScenarioCRC) {
			return(RC_SCENARIO_MISMATCH);
		}
		theirs->timing = timer->Value(); // RA2: the elapsed time
	}
	}

	return(retcode);

}	// end of Process_Receive_Packet


static RetcodeType Process_Serial_Packet(char *multi_packet_buf,
	int first_time)
{
	SerialPacketType *serial_packet;		// for parsing serial packets
	int player_gone;
	EventClass *event;

	//------------------------------------------------------------------------
	// Determine if this packet means that the other player has left the game
	//------------------------------------------------------------------------
	serial_packet = (SerialPacketType *)multi_packet_buf;
	player_gone = 0;
	//........................................................................
	// On Frame 0, only a SIGN_OFF means the other player left; the other
	// packet types may be left over from a previous session.
	//........................................................................
	if (first_time) {
		if (serial_packet->Command == SERIAL_SIGN_OFF) {
			player_gone = 1;
		}
	}
	//........................................................................
	// On subsequent frames, any of SIGN_OFF, TIMING, or SCORE_SCREEN means
	// the other player is gone.
	//........................................................................
	else {
		if (serial_packet->Command == SERIAL_SIGN_OFF ||
			serial_packet->Command == SERIAL_TIMING ||
			serial_packet->Command == SERIAL_SCORE_SCREEN ) {
			player_gone = 1;
		}
	}
	if (player_gone) {
		Destroy_Null_Connection(serial_packet->ScenarioInfo.Color, 0);
		return(RC_PLAYER_LEFT);
	}

	//------------------------------------------------------------------------
	// Process an incoming message
	//------------------------------------------------------------------------
	if (serial_packet->Command == SERIAL_MESSAGE) {
		wchar_t *name = Narrow_To_Wide(serial_packet->Name); // RA2: the name is widened, the result kept
		if (!Session.Messages.Concat_Message(name, // RA2: as above
			serial_packet->ID, serial_packet->Message.Message, Rule->MessageDelay * TICKS_PER_MINUTE)) {
			name = Narrow_To_Wide(serial_packet->Name); // RA2: as above
			Session.Messages.Add_Message (name, // RA2: as above
				serial_packet->ID, serial_packet->Message.Message,
				serial_packet->ID,
				TextPrintType(TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW), Rule->MessageDelay * TICKS_PER_MINUTE, false); // RA2: Add_Message takes a final flag

			VocClass::PlayGlobal(Rule->IncomingMessage, 1.0f, 0x2000, 0); // RA2: the message sound plays centred
		}

		//.....................................................................
		// Save this message in our last-message buffer
		//.....................................................................
		if (wcslen (serial_packet->Message.Message)) { // RA2: wide message
			wcscpy (Session.LastMessage, serial_packet->Message.Message); // RA2: as above
		}

		//.....................................................................
		// Tell the map to do a partial update (just to force the
		// messages to redraw).
		//.....................................................................
		//Map.Flag_To_Redraw(false);
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		return(RC_SERIAL_PROCESSED);
	}

	//------------------------------------------------------------------------
	// RA2: a taunt is played when the game type allows them
	//------------------------------------------------------------------------
	if (serial_packet->Command == SERIAL_TAUNT) { // RA2: as above
		if ((Session.LANTaunts && Session.Type == GAME_IPX) || (Session.WOLTaunts && Session.Type == GAME_INTERNET)) { // RA2: as above
			Play_Taunt(serial_packet->Taunt); // RA2: as above
			return(RC_SERIAL_PROCESSED); // RA2: as above
		}
		return(RC_SERIAL_PROCESSED); // RA2: as above
	}

	//------------------------------------------------------------------------
	// Any other SERIAL-type packet means the other player is still there;
	// throw them away, but let the caller know the connection is OK.
	//------------------------------------------------------------------------
	if ( (serial_packet->Command >= SERIAL_CONNECT &&
		serial_packet->Command < SERIAL_LAST_COMMAND) ||
		(serial_packet->Command >= SERIAL_REQ_SCENARIO &&
		 serial_packet->Command <= SERIAL_ACCEPT_OPTIONS) ||
		Session.NumPlayers == 1) {
		return(RC_SERIAL_PROCESSED);
	}

	//........................................................................
	// are we getting our own packets back??
	//........................................................................
	event = (EventClass *)multi_packet_buf;

	if (event->Type <= EventClass::EMPTY || event->Type >= EventClass::LAST_EVENT) return(RC_SERIAL_PROCESSED);

	if (event->ID == PlayerPtr->HeapID) {
		return(RC_HUNG_UP);
	}

	return(RC_NORMAL);
}


/// <summary>
/// Draws the frame sync bars on the reconnect dialog.
/// Every player in the game gets a bar that shrinks and changes color as the wait on that
/// player drags on, so the humans can see who the game is actually stalled on.
/// </summary>
/// <param name="window">The reconnect dialog that owns the bar controls.</param>
void Draw_Sync_Bars(HWND window)
{
	for (int i = 0; i < Session.Players.Count(); i++) {
		RECT bar_winrect;
		Get_Display_Rect(GetDlgItem(window, SyncBarControlIDs[i]), &bar_winrect);
		Rect bar_rect(bar_winrect.left, bar_winrect.top,
			bar_winrect.right - bar_winrect.left, bar_winrect.bottom - bar_winrect.top);
		Rect border = bar_rect;
		int playerid = Connection_Index(Session.Players[i]->Player.ID);
		unsigned progress;
		if (i == 0) {
			progress = 0;
		} else {
			progress = ReconnectElapsed - SyncBarFrameSync[playerid].timing;
		}
		unsigned short color = DSurface::Build_Hicolor_Pixel(0, 200, 0);
		if (progress > 240) {
			color = DSurface::Build_Hicolor_Pixel(200, 200, 0);
			if (progress > 480) {
				color = DSurface::Build_Hicolor_Pixel(200, 0, 0);
				unsigned elapsed = (timeGetTime() * 60 - SyncBarFrameSync[playerid].heard * 60) / 1000;
				if (elapsed < 420) {
					progress = elapsed;
					color = DSurface::Build_Hicolor_Pixel(0, 0, 200);
				}
			}
		}
		int percent = MAX(100 - (int)(100 * progress / 1200), 0);
		int w = percent * bar_rect.Width;
		bar_rect.Width = MAX(6, w / 100);
		int border_color = DSurface::Build_Hicolor_Pixel(SyncBarOutlineColor);
		AlternateSurface->Draw_Rect(AlternateSurface->Get_Rect(), border, border_color);
		Rect inside = bar_rect;
		inside.X += 2;
		inside.Y += 2;
		inside.Width -= 4;
		inside.Height -= 4;
		AlternateSurface->Fill_Rect(AlternateSurface->Get_Rect(), inside, color);
	}
}



/// <summary>
/// Trims the message list box and scrolls it to the end.
/// Use this routine after adding a line to the reconnect dialog's message list, so that
/// the list stays a manageable length and the newest message stays in view.
/// </summary>
/// <param name="listbox">The list box to trim.</param>
void ListBox_Trim(HWND listbox)
{
	int string_count = ListBox_GetCount(listbox);
	if (string_count > 50) {
		ListBox_DeleteString(listbox, 0);
		string_count--;
	}
	ListBox_SetTopIndex(listbox, string_count - 1);
}


/// <summary>
/// Proposes that a player be kicked out of the game.
/// This routine is called when one of the kick buttons on the reconnect dialog is pressed.
/// The proposal is sent to every other player and the local vote is cast right away.
/// Proposing to kick yourself, or to kick anybody at all during a tournament game, earns
/// nothing but a message in the dialog.
/// </summary>
/// <param name="window">The reconnect dialog to report the outcome in.</param>
/// <param name="id">Index into the session player list of the one to be kicked.</param>
void Propose_Kick_Player(HWND window, int id)
{
	if (id < 0 || id >= Session.Players.Count()) { // RA2: an index outside the player list is ignored
		return; // RA2: as above
	}
	DebugString("Propose_Kick_Player %d - %s. Local id is %d\n", id, Session.Players[id]->Name, Session.Players[0]->Player.ID);
	HWND listbox = GetDlgItem(window, IDC_DISCONNECT_MESSAGES);

	if (id == 0) {
		_DrawMessage(-1, WAIT_TEXT("TXT_RECONNECT_KICK_SELF", 3378), listbox); // RA2: wide text from the string table
		ListBox_Trim(listbox);
		return;
	}

	if (Session.Type == GAME_INTERNET && WestwoodOnline_Tournament) {
		_DrawMessage(-1, WAIT_TEXT("TXT_CANT_KICK", 3388), listbox); // RA2: as above
		ListBox_Trim(listbox);
		return;
	}

	GlobalPacketType gpacket;
	gpacket.Command = NET_PROPOSE_KICK;
	strncpy(gpacket.Name, Wide_To_Narrow(Session.Players[0]->Name), ARRAY_SIZE(gpacket.Name)); // RA2: as above
	gpacket.Kick.KickerID = Session.Players[0]->Player.ID;
	gpacket.Kick.KickeeID = Session.Players[id]->Player.ID;

	for (int i = 1; i < Session.Players.Count(); i++) {
		DebugString("Sending kick proposal to %s\n", Session.Players[i]->Name);
		Ipx.Send_Global_Message(&gpacket, sizeof(gpacket), 1, &Session.Players[i]->Address);
	}

	Cast_Kick_Vote(Session.Players[0]->Player.ID, Session.Players[id]->Player.ID);
}


/// <summary>
/// Handles a kick proposal arriving from another player.
/// The packet is copied and queued up on the session, so that the wait-for-players loop
/// can act on the proposal when it next gets the chance.
/// </summary>
/// <param name="packet">The global packet that carries the kick proposal.</param>
void Kick_Packet_Received(GlobalPacketType & packet, IPXAddressClass & address)
{
	GlobalPacketType *newpacket = new GlobalPacketType;
	memcpy(newpacket, &packet, sizeof(*newpacket));
	Session.KickProposals.Add(newpacket);
}


/// <summary>
/// Records a vote to kick a player out of the game.
/// A player gets only the one vote against any given victim, so a repeat vote is quietly
/// discarded. In a network game the vote is announced in the reconnect dialog's message
/// list, so everyone can see who wants whom gone.
/// </summary>
/// <param name="kicker">Player ID of the one casting the vote.</param>
/// <param name="kickee">Player ID of the one being voted against.</param>
void Cast_Kick_Vote(int kicker, int kickee)
{
	wchar_t buffer[256]; // RA2: wide

	int votes = Session.KickVoteCount[kickee];
	for (int i = 0; i < votes; i++) {
		if (Session.KickVoteWho[kickee][i] == kicker) {
			return;
		}
	}

	Session.KickVoteWho[kickee][votes] = kicker;
	Session.KickVoteCount[kickee]++;

	if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
		wchar_t const * kicker_name = Ipx.Connection_Name(kicker); // RA2: connection names are wide
		if (kicker_name == NULL) {
			kicker_name = Session.Players[0]->Name;
		}

		DebugString("Player %s votes to kick player %s from the game\n", kicker_name, Ipx.Connection_Name(kickee));
		swprintf(buffer, WAIT_TEXT("TXT_RECONNECT_KICK_RECEIVED", 3488), kicker_name, Ipx.Connection_Name(kickee)); // RA2: wide text from the string table

		HWND topwindow = WS_Top_Window();
		HWND listbox = GetDlgItem(topwindow, IDC_DISCONNECT_MESSAGES);
		_DrawMessage(-1, buffer, listbox); // RA2: the message list is filled by _DrawMessage
		ListBox_Trim(listbox);
	}
}


/// <summary>
/// Clears the counters the reconnect dialog keeps of how slow the pings to the other player are.
/// </summary>
void Reset_Reconnect_Pings(void) // RA2: 0x00627330
{
	ReconnectSlowCount = 0; // RA2: as above
	ReconnectVerySlowCount = 0; // RA2: as above
	ReconnectSlowCounted = false; // RA2: as above
	ReconnectVerySlowCounted = false; // RA2: as above
	ReconnectPingTimedOut = false; // RA2: as above
}


/// <summary>
/// Handles the messages for the reconnect dialog.
/// This is the dialog that appears when the game stalls waiting on somebody. It paints the
/// per-player sync bars, offers a kick button for each player in the game, and keeps
/// pinging the game server for the duration of an internet game stall.
/// </summary>
#define PING_TIMER 1
#define PING_TIMEOUT 1000
BOOL CALLBACK Reconnect_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int * rc = (int *)GetWindowLong(window, DWL_USER);
	HWND button;
	HWND bar;
	int i;

	switch (message) {
		case WM_DESTROY: // RA2: the dialog is removed from the modeless list when it goes
			Remove_Modeless_Dialog(window);
			return(FALSE);

		case WM_DRAWITEM:
			Dialog_On_WM_DRAWITEM(lparam); // RA2: the movie-aware draw (0x005FE380)
			return(TRUE);

		case WM_ERASEBKGND:
			return(TRUE);

		case WM_PAINT:
			Dialog_On_WM_PAINT(window); // RA2: the owner draw paint (0x005FEE70)
			Draw_Sync_Bars(window); // RA2: the bars are painted with the dialog
			ValidateRect(window, NULL);
			return(FALSE); // RA2: as above

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDCANCEL:
					*rc = IDCANCEL;
					return(FALSE);

				case IDC_DISCONNECT_PLAYER1:
					Propose_Kick_Player(window, 0);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER2:
					Propose_Kick_Player(window, 1);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER3:
					Propose_Kick_Player(window, 2);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER4:
					Propose_Kick_Player(window, 3);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER5:
					Propose_Kick_Player(window, 4);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER6:
					Propose_Kick_Player(window, 5);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER7:
					Propose_Kick_Player(window, 6);
					return(FALSE);

				case IDC_DISCONNECT_PLAYER8:
					Propose_Kick_Player(window, 7);
					return(FALSE);
			}
			break;

		case WM_TIMER:
			if (Session.Type == GAME_INTERNET && wparam == PING_TIMER && g_pNetUtil) {
				if (g_PingsSent < 5) {
					int handle;
					g_pNetUtil->RequestPing(g_GameServerHost, PING_TIMEOUT, &handle);
					g_PingsSent++;
				}
				ReconnectPingSeconds++; // RA2: a tournament game is given up on when the pings stay slow
				if (WestwoodOnline_Tournament) { // RA2: as above
					if (!ReconnectSlowCounted && ReconnectPingSeconds > 5 && !g_PingsReceived) { // RA2: as above
						ReconnectSlowCounted = true; // RA2: as above
						ReconnectSlowCount++; // RA2: as above
					} else if (!ReconnectVerySlowCounted && ReconnectPingSeconds > 10 && !g_PingsReceived) { // RA2: as above
						ReconnectVerySlowCounted = true; // RA2: as above
						ReconnectVerySlowCount++; // RA2: as above
					}
					if (ReconnectSlowCount > 5 || ReconnectVerySlowCount > 2) { // RA2: as above
						ReconnectPingTimedOut = true; // RA2: as above
						*rc = IDCANCEL; // RA2: as above
						return(FALSE); // RA2: as above
					}
				}
				SetTimer(window, PING_TIMER, PING_TIMEOUT, NULL);
			}
			break;

		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			return((BOOL)GetStockObject(BLACK_BRUSH));

		case WM_RECONNECT_SETUP: { // RA2: sent by Process_Reconnect_Dialog's creation; replaces WM_INITDIALOG
			Add_Modeless_Dialog(window);

			for (i = 0; i < MAX_PLAYERS; i++) {
				button = GetDlgItem(window, SyncNameButtonControlsIDs[i]); // RA2: as above
				EnableWindow(button, FALSE); // RA2: as above
				bar = GetDlgItem(window, SyncBarControlIDs[i]); // RA2: as above
				EnableWindow(bar, FALSE); // RA2: as above
			}

			for (i = 0; i < MAX_PLAYERS; i++) {
				button = GetDlgItem(window, SyncNameButtonControlsIDs[i]); // RA2: as above
				bar = GetDlgItem(window, SyncBarControlIDs[i]); // RA2: as above
				if (i < Session.Players.Count()) {
					SendMessage(button, 0x04B2, 0, (LPARAM)Session.Players[i]); // RA2: the player is handed to the button
					EnableWindow(button, TRUE); // RA2: as above
					EnableWindow(bar, TRUE); // RA2: as above
				} else {
					DestroyWindow(button); // RA2: as above
					DestroyWindow(bar); // RA2: as above
				}
			}

			if (Session.Type == GAME_INTERNET) {
				g_PingsSent = 0;
				g_PingsReceived = 0;
				ReconnectSlowCounted = false; // RA2: as above
				ReconnectVerySlowCounted = false; // RA2: as above
				ReconnectPingSeconds = 0; // RA2: as above
				ReconnectPingTimedOut = false; // RA2: as above
				SetTimer(window, PING_TIMER, PING_TIMEOUT, NULL);
			}
			MouseCursor->Show_Mouse(); // RA2: as above
			break;
		}
	}

	return(FALSE);
}


/// <summary>
/// Removes a player from the running multiplayer game.
/// This routine is called once the other players have voted someone out, or a player has
/// stopped answering altogether. The offending house is flagged as having lost its
/// connection, the game statistics are shipped off if the game is effectively decided, and
/// the connection itself is destroyed.
/// </summary>
/// <param name="kickee">Connection index of the player to be removed.</param>
/// <param name="their">The frame sync records, compacted as the connection goes away.</param>
/// <param name="error">Should the disconnection be reported as an error?</param>
void Kick_Player_Now(ConnManClass *net, int kickee, FrameSyncStruct * their, bool error)
{
	/*
	 * Kicking several players inside a minute is treated as spam rather than as a series of
	 * resignations, so those kicks leave the victims' resigner flags alone.
	 */
	bool spamkick = false;
	if (time(NULL) - LastKickTime < 60) { // RA2: the time of the last kick is a file-level variable
		spamkick = true;
	}
	LastKickTime = time(NULL); // RA2: as above

	int id = net->Connection_ID(kickee);
	DebugString("Kicking player %d from the game - Frame is %d\n", id, Frame);

	if (id == -1) {
		DebugString("Kick_Player_Now bailing - bad house ID\n");
		return;
	}

	Houses[id]->LostConnection = true;

	if (CountAliveTeams(Houses[id]) == 1 && Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
		Register_Game_End_Time(); // RA2: the game ends for the statistics
		ConnectionLost = true; // RA2: as above
		if (!spamkick) {
			Session.SawGameCompletion = true;
		}
		for (int i = 0; i < Houses.Count(); i++) {
			HouseClass * house = Houses[i];
			if (house != NULL && !house->IsDefeated && house->IsHuman) {
				house->LostConnection = true;
			}
		}
		Send_Statistics_Packet(false); // RA2: the results go out at once
	}

	if (CountAliveTeams(Houses[id]) > 1 && !spamkick) {
		Houses[id]->IsResigner = true;
	}

	for (int i = kickee; i < net->Num_Connections() - 1; i++) {
		their[i] = their[i+1];
		Session.KickVoteCount[i] = Session.KickVoteCount[i+1];
		memcpy(Session.KickVoteWho[i], Session.KickVoteWho[i+1], sizeof(Session.KickVoteWho[i]));
	}

	Destroy_Connection(id, error);
}


static int Handle_Timeout(ConnManClass *net, FrameSyncStruct *their)
{
	int oldest_index;						// index of person requiring a reconnect
	int i,j;
	//int id;

	//------------------------------------------------------------------------
	// For modem, attempt to reconnect; if that fails, save the game & bail.
	//------------------------------------------------------------------------
	if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		if ( net->Num_Connections() ) {
			Session.Suspended++;
			if (!Reconnect_Modem()) {
				Session.Suspended--;
				return(0);
			} else {
				Session.Suspended--;
				return(1);
			}
		}
	}

	//------------------------------------------------------------------------
	// For network, destroy the oldest connection
	//------------------------------------------------------------------------
	else if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
		j = 0x7fffffff;
		oldest_index = 0;
		for (i = 0; i < net->Num_Connections(); i++) {
			if (their[i].frame < j) {
				j = their[i].frame;
				oldest_index = i;
			}
		}
		Kick_Player_Now(net, oldest_index, their, true);

#if 0
		id = net->Connection_ID(oldest_index);
		/*
		**	Send the game statistics packet now if the game is effectivly over
		*/
		if (Session.Players.Count() == 2 &&
				Session.Type == GAME_INTERNET &&
				!GameStatisticsPacketSent) {
			Register_Game_End_Time();
			ConnectionLost = true;
			Send_Statistics_Packet();		//	Disconnect, and I'll be the only one left.
		}

		if (id != ConnManClass::CONNECTION_NONE) {
			for (i = oldest_index; i < net->Num_Connections() - 1; i++) {
				their[i].frame = their[i+1].frame;
				their[i].sent = their[i+1].sent;
				their[i].recv = their[i+1].recv;
			}
			if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
				Destroy_Connection(id,1);
			}
		}
#endif
	}

	return(1);

}


/// <summary>
/// Looks for a repeated megamission list in the cache of the last eight, or saves it there.
/// The mission events that follow one another with the same unit list send the list once;
/// the packet then only carries the number of the cached list. With 'save' set the
/// repetition is applied to the packet (the size shrinks, the count is cleared and the slot
/// number is stored in the event), and a list that is not cached yet replaces the oldest slot.
/// </summary>
/// <param name="count">The number of units in the list; cleared when the list is a repetition.</param>
/// <param name="size">The packet size, reduced by the units that are no longer sent.</param>
/// <param name="whom">The first unit of the list (5 bytes), followed by the event's other fields.</param>
/// <param name="units">The other units of the list (5 bytes each).</param>
/// <param name="save">Apply the result to the packet and cache a new list.</param>
/// <returns>Returns true when the list was found in the cache.</returns>
bool __fastcall Compress_Megamission_List(unsigned char *count, int *size, char *whom, char *units, bool save) // RA2: 0x006279D0
{
	bool matched = false;
	if (*count > 1) {
		for (int i = 0; i < ARRAY_SIZE(MegamissionCounts); i++) {
			int saved_count = MegamissionCounts[i];
			if (*count == saved_count &&
				memcmp(MegamissionLists[i], whom, 5) == 0 &&
				memcmp(MegamissionLists[i] + 5, units, saved_count * 5 - 5) == 0) {
				if (save) {
					*size += 5 - *count * 5;
				}
				matched = true;
				if (save) {
					*count = 0;
					whom[6] = (char)i;
				}
				break;
			}
		}
	}
	if (!matched && save && *count > 1) {
		int oldest = -1;
		int greatest_age = -1;
		for (int slot = 0; slot < ARRAY_SIZE(MegamissionFrames); slot++) {
			if (MegamissionFrames[slot] == 0) {
				oldest = slot;
				break;
			}
			int age = Frame - MegamissionFrames[slot];
			if (age > greatest_age) {
				greatest_age = age;
				oldest = slot;
			}
		}
		if (oldest == -1) {
			oldest = 0;
		}
		DebugString("Compress: Saving new megamission list with %d units to slot %d\n", *count, oldest);
		MegamissionFrames[oldest] = Frame;
		memcpy(MegamissionLists[oldest], whom, 5);
		memcpy(MegamissionLists[oldest] + 5, units, *count * 5 - 5);
		MegamissionCounts[oldest] = *count;
		whom[6] = (char)oldest;
	}
	return(matched);
}


/***************************************************************************
 * Add_Compressed_Events -- adds an compressed events to a packet          *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf            buffer to store packet in                           *
 *      bufsize         max size of buffer                                 *
 *      frame_delay      desired frame delay to attach to all outgoing packets*
 *      size            reference to current packet size                   *
 *      cap            max # events to process                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      new size value                                                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      This routine MUST check to be sure it doesn't overflow the buffer. *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Add_Compressed_Events(void *buf, int bufsize, int frame_delay,
	int size, int cap, int & processed)
{
	int num = 0;                        // # of events processed
	unsigned char eventtype;            // type of event being compressed
	EventClass prevevent;               // last event processed
	int datasize;                       // size of element plucked from event union
	int storedsize;                     // actual # bytes stored from event
	unsigned char *unitsptr = NULL;     // ptr to buffer pos to store mega. rep count
	unsigned char numunits = 0;         // megamission rep count value
	bool missiondup = false;            // flag: is this event a megamission repeat?
	bool cached = false;                // RA2: the run was split because its unit list repeats an earlier one
	char mission;                       // RA2: as above
	char *units;                        // RA2: the other units of the run in the packet
	char *whom;                         // RA2: the first unit of the run in the packet

	//------------------------------------------------------------------------
	// clear previous event
	//------------------------------------------------------------------------
	memset (&prevevent, 0, sizeof(EventClass));
	units = NULL; // RA2: as above
	whom = NULL; // RA2: as above

	if (Debug_Print_Events) {
		DebugString("\nFrame %d: Building Send Packet\n", Frame);
	}

	//------------------------------------------------------------------------
	// Loop until there are no more events, we've processed our max # of
	// events, or the buffer is full.
	//------------------------------------------------------------------------
	while (OutList.Count && (num < cap)) {

		//Keyboard->Check();

		eventtype = OutList.First().Type;
		datasize = EventClass::EventLength[ eventtype ];
		//.....................................................................
		// For a variable-sized event, pull the size from the event; otherwise,
		// the size will be the data element size plus the event type value.
		// (The other data elements in the event, Frame, ID, etc, are stored
		// in the packet header.)
		//.....................................................................
		if (eventtype==EventClass::ADDPLAYER) {
			storedsize = datasize + size_of(EventClass, Type) +
				OutList.First().Data.Variable.Size;
		}
		else {
			storedsize = datasize + size_of(EventClass, Type);
		}

		//.....................................................................
		// MegaMission compression:  MegaMissions are stored as:
		//   EventType
		//   Rep Count
		//   MegaMission structure (event # 1 only)
		//   Whom #2
		//   Whom #3
		//   Whom #4
		//   ...
		//   Whom #n
		//.....................................................................
		if (prevevent.Type == EventClass::MEGAMISSION) {
			//..................................................................
			// If previous & current events are both MegaMissions:
			//..................................................................
			if (eventtype == EventClass::MEGAMISSION) {
				//...............................................................
				// If the Mission, Target, & Destination are the same, compress
				// the events into one:
				// - Change datasize to the size of the 'Whom' field only
				// - set total # bytes to store to the size of the 'Whom' only
				// - increment the MegaMission rep count
				// - set the MegaMission rep flag
				//...............................................................
				if (!cached // RA2: a run split at a cached unit list starts over
					&& (mission = OutList.First().Data.MegaMission.Mission) == prevevent.Data.MegaMission.Mission // RA2: as above
					&& OutList.First().Data.MegaMission.Target == prevevent.Data.MegaMission.Target // RA2: as above
					&& OutList.First().Data.MegaMission.Destination == prevevent.Data.MegaMission.Destination) { // RA2: as above

					if (Debug_Print_Events) {
						DebugString("      adding Whom:%x Mission:%s Target:%x Dest:%x\n",
						OutList.First().Data.MegaMission.Whom.Encode(),
						MissionClass::Mission_Name((MissionType)mission), // RA2: as above
						OutList.First().Data.MegaMission.Target.Encode(),
						OutList.First().Data.MegaMission.Destination.Encode());
					}

					datasize = sizeof(prevevent.Data.MegaMission.Whom);
					storedsize = datasize;
					numunits++;
					missiondup = true;
				}
				//...............................................................
				// Data doesn't match; start a new run of MegaMissions:
				// - Store previous MegaMission rep count
				// - Init 'unitsptr' to buffer pos after next EventType
				// - set total # bytes to store to 'datasize' + sizeof(EventType) +
				//   sizeof (numunits)
				// - init the MegaMission rep count to 1
				// - clear the MegaMission rep flag
				//...............................................................
				else {

					if (Debug_Print_Events) {
						DebugString("  New MEGAMISSION run:\n");
					}

					DebugString("Compress: New megamission run - checking for previous repeat of %d units\n", numunits); // RA2: logged
					Compress_Megamission_List(&numunits, &size, whom, units, true); // RA2: the finished run's unit list is cached
					*unitsptr = numunits;
					unitsptr = ((unsigned char *)buf) + size +
						size_of(EventClass, Type);
					storedsize += sizeof(numunits);
					numunits = 1;
					missiondup = false;
				}
			}
			//..................................................................
			// Previous event was a MegaMission, but this one isn't: end the
			// run of MegaMissions:
			// - Store previous MegaMission rep count
			// - Clear variables
			//..................................................................
			else {
				DebugString("Compress: Non-megamission event - checking for previous repeat of %d units\n", numunits); // RA2: logged
				Compress_Megamission_List(&numunits, &size, whom, units, true); // RA2: as above
				*unitsptr = numunits;   // save # events in our run
				unitsptr = NULL;        // init other values
				numunits = 0;
				missiondup = false;
			}
		}

		//.....................................................................
		// The previous event is not a MEGAMISSION but the current event is:
		// Set up a new run of MegaMissions:
		// - Init 'unitsptr' to buffer pos after next EventType
		// - set total # bytes to store to 'datasize' + sizeof(EventType) +
		//   sizeof (numunits)
		// - init the MegaMission rep count to 1
		// - clear the MegaMission rep flag
		//.....................................................................
		else if (eventtype == EventClass::MEGAMISSION) {

			if (Debug_Print_Events) {
				DebugString("  New MEGAMISSION run:\n");
			}

			unitsptr = ((unsigned char *)buf) + size +
				size_of(EventClass, Type);
			storedsize += sizeof(numunits);
			numunits = 1;
			missiondup = false;
		}

		cached = false; // RA2: as above

		//.....................................................................
		// Will the next event exceed the size of the buffer?  If so,
		// stop compressing.
		//.....................................................................
		if ( (size + storedsize) > bufsize ) {
			if (eventtype == EventClass::MEGAMISSION && missiondup) { // RA2: the run ends here, its list is cached
				numunits--; // RA2: as above
				DebugString("Compress: Out of buffer space at %d units\n", numunits); // RA2: as above
				Compress_Megamission_List(&numunits, &size, whom, units, true); // RA2: as above
				*unitsptr = numunits; // RA2: as above
			}
			break;
		}

		//.....................................................................
		// Set the event's frame delay (this is protocol-dependent)
		//.....................................................................
		if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
			OutList.First().Frame = ((Frame + frame_delay +
				(Session.FrameSendRate - 1)) / Session.FrameSendRate) *
				Session.FrameSendRate;
		}
		else {
			OutList.First().Frame = Frame + frame_delay;
		}

		//.....................................................................
		// Set the event's ID
		//.....................................................................
		OutList.First().ID = PlayerPtr->HeapID;

		//.....................................................................
		// Transfer the event in OutList to DoList, un-queue the OutList event.
		// If the DoList is full, stop transferring immediately.
		//.....................................................................
		OutList.First().IsExecuted = 0;
		if ( !DoList.Add( OutList.First(), OutList.Timings[OutList.Head] ) ) { // RA2: the time the event was queued travels with it
			break;
		}
		#ifdef MIRROR_QUEUE
		MirrorList.Add(OutList.First());
		#endif

		//---------------------------------------------------------------------
		// Compress the event into the send packet buffer
		//---------------------------------------------------------------------
		switch ( eventtype ) {
			//..................................................................
			// RESPONSE_TIME: just use the Delay field of the FrameInfo union
			//..................................................................
			case (EventClass::RESPONSE_TIME):

				*(unsigned char *)( ((char *)buf) + size) = eventtype;

				memcpy ( ((char *)buf) + size + size_of(EventClass, Type),
					&OutList.First().Data.FrameInfo.Delay, datasize );

				size += (datasize + size_of(EventClass, Type));
				break;

			//..................................................................
			// MEGAMISSION:
			//..................................................................
			case (EventClass::MEGAMISSION):
				//...............................................................
				// Repeated mission in a run:
				//   - Update the rep count (in case we break out)
				//   - Copy the Whom field only
				//...............................................................
				if (missiondup) {
					*unitsptr = numunits;

					memcpy ( ((char *)buf) + size,
						&OutList.First().Data.MegaMission.Whom, datasize );

					size += datasize;
					if (OutList.Count != 1 && num != cap - 1) { // RA2: a list that was sent before ends the run early
						if (Compress_Megamission_List(&numunits, &size, whom, units, false)) { // RA2: as above
							DebugString("Compress: Found early repeat megamission - splitting after %d units\n", numunits); // RA2: as above
							cached = true; // RA2: as above
							*unitsptr = numunits; // RA2: as above
						}
					} else {
						DebugString("Compress: Ended megamission run at %d units - no more events\n", numunits); // RA2: as above
						Compress_Megamission_List(&numunits, &size, whom, units, true); // RA2: as above
						*unitsptr = numunits; // RA2: as above
					}
				}
				//...............................................................
				// 1st mission in a run:
				//   - Init the rep count (in case we break out)
				//   - Set the EventType
				//   - Copy the MegaMission structure, leaving room for 'numunits'
				//...............................................................
				else {
					*unitsptr = numunits;

					*(unsigned char *)( ((char *)buf) + size) = eventtype;

					whom = ((char *)buf) + size + size_of(EventClass, Type) + sizeof(numunits); // RA2: the run's first unit is remembered
					memcpy ( whom,
						&OutList.First().Data.MegaMission, datasize );
					units = whom + datasize; // RA2: as above

					size += (datasize + size_of(EventClass, Type) + sizeof(numunits));
				}
				break;

			//..................................................................
			// Variable-sized packets: Copy the packet Size & the buffer
			//..................................................................
			case (EventClass::ADDPLAYER):
				*(unsigned char *)( ((char *)buf) + size) = eventtype;

				memcpy ( ((char *)buf) + size + size_of(EventClass, Type),
					&OutList.First().Data.Variable.Size, datasize );
				size += (datasize + size_of(EventClass, Type));

				memcpy ( ((char *)buf) + size,
					OutList.First().Data.Variable.Pointer,
					OutList.First().Data.Variable.Size);
				size += OutList.First().Data.Variable.Size;

				break;

			//..................................................................
			// Default case: Just copy over the data field from the union
			//..................................................................
			default:
				*(unsigned char *)( ((char *)buf) + size) = eventtype;

				memcpy ( ((char *)buf) + size + size_of(EventClass, Type),
					&OutList.First().Data, datasize );

				size += (datasize + size_of(EventClass, Type));

				break;
		}

		//---------------------------------------------------------------------
		// update # events processed
		//---------------------------------------------------------------------
		num++;

		//---------------------------------------------------------------------
		// Update 'prevevent'
		//---------------------------------------------------------------------
		memcpy ( &prevevent, &OutList.First(), sizeof(EventClass) );

		//---------------------------------------------------------------------
		// Go to the next event to process
		//---------------------------------------------------------------------
		OutList.Next();
	}

//	if (Debug_Print_Events) {
//		DebugString("\n");
//	}

	processed = num;
	return(size);

}	// end of Add_Compressed_Events


/***************************************************************************
 * Extract_Compressed_Events -- extracts events from a packet              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer containing events to extract                    *
 *      bufsize      length of 'buf'                                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # events extracted                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
#ifdef RA2_NON_MATCHING // not exact: against the image bound (1,455 B) 945 bytes differ (was 972; the archive decrements numunits before copying the next target; the megamission Slot byte and the flat saved-count table as ra2-decomp's event.h and queue.cpp have them, TibSun's memcpy record copies); main: best 57.62, tried 6 times (register, frame, inline)
int Extract_Compressed_Events(void * buf, int bufsize)
{
	int pos = 0;                // current buffer parsing position
	int leftover = bufsize;     // # bytes left to process
	EventClass *event;          // event ptr for parsing buffer
	int count = 0;              // # events processed
	int datasize = 0;           // size of data to copy
	EventClass eventdata;       // stores Frame, ID, etc
	unsigned char numunits = 0; // # units stored in compressed MegaMissions
	int house; // RA2: the sender, to find his cached unit lists

	//------------------------------------------------------------------------
	// Clear work event structure
	//------------------------------------------------------------------------
	memset (&eventdata, 0, sizeof(EventClass));
	event = (EventClass *)(((char *)buf) + pos);
	house = event->ID; // RA2: as above

	//------------------------------------------------------------------------
	// Assume the first event is a FRAMEINFO event
	// Init 'datasize' to the amount of data to copy, minus the EventType value
	// For the 1st packet only, this will include all info before the Data
	// union, plus the size of the FrameInfo structure, minus the EventType size.
	//------------------------------------------------------------------------
	datasize = (offsetof(EventClass, Data) +
		size_of(EventClass, Data.FrameInfo)) - size_of(EventClass, Type);

	while ((unsigned)leftover >= (datasize + size_of(EventClass, Type)) ) {

		//.....................................................................
		// add event to the DoList, only if it's not a FRAMESYNC
		// (but FRAMEINFO's do get added.)
		//.....................................................................
		if (event->Type != EventClass::FRAMESYNC) {
			//..................................................................
			// initialize the common data from the FRAMEINFO event
			// keeping IsExecuted 0
			//..................................................................
			if (event->Type == EventClass::FRAMEINFO) {
				eventdata.Frame = event->Frame;
				eventdata.ID = event->ID;

				//...............................................................
				// Adjust position past the common data
				//...............................................................
				pos += (offsetof(EventClass, Data) -
						 size_of(EventClass, Type));
				leftover -= (offsetof(EventClass, Data) -
								size_of(EventClass, Type));
			}
			//..................................................................
			// if MEGAMISSION event get the number of units (events to generate)
			//..................................................................
			else if (event->Type == EventClass::MEGAMISSION) {
				numunits = *(((unsigned char *)buf) + pos + sizeof(eventdata.Type));
				pos += sizeof(numunits);
				leftover -= sizeof(numunits);
			}

			//..................................................................
			// clear the union data portion of the event
			//..................................................................
			memset (&eventdata.Data, 0, sizeof(eventdata.Data));
			eventdata.Type = event->Type;
			datasize = EventClass::EventLength[ eventdata.Type ];

			switch (eventdata.Type) {
				case (EventClass::RESPONSE_TIME):
					memcpy ( &eventdata.Data.FrameInfo.Delay,
						((char *)buf) + pos + size_of(EventClass, Type),
						datasize );
					break;

				case (EventClass::ADDPLAYER):

					memcpy ( &eventdata.Data.Variable.Size,
						((char *)buf) + pos + size_of(EventClass, Type),
						datasize );

					eventdata.Data.Variable.Pointer =
						new char[eventdata.Data.Variable.Size];
					memcpy (eventdata.Data.Variable.Pointer,
						((char *)buf) + pos + size_of(EventClass, Type) + datasize,
						eventdata.Data.Variable.Size);

					pos += eventdata.Data.Variable.Size;
					leftover -= eventdata.Data.Variable.Size;

					break;

				case (EventClass::MEGAMISSION):
					memcpy ( &eventdata.Data.MegaMission,
						((char *)buf) + pos + size_of(EventClass, Type),
						datasize );

					if (numunits > 1) {
						//......................................................
						// RA2: a new run; its units are kept in the sender's slot
						//......................................................
						pos += (datasize + size_of(EventClass, Type));
						leftover -= (datasize + size_of(EventClass, Type));
						datasize = sizeof(eventdata.Data.MegaMission.Whom);
						DebugString("Decoding new MEGAMISSION\n"); // RA2: as above
						int index = eventdata.Data.MegaMission.Slot + house * ARRAY_SIZE(MegamissionCounts); // RA2: as above
						memcpy(&MegamissionUnitLists[index][0], &eventdata.Data.MegaMission.Whom, datasize); // RA2: as above
						int units = 1; // RA2: as above
						SavedUnitCounts[index] = units; // RA2: as above
						if (eventdata.Frame == 0 || eventdata.Type == 0) { // RA2: as above
							DebugString("Frame %d:Empty megamission decoded\n", Frame); // RA2: as above
						}

						if (numunits) {
							xTargetClass * list = &MegamissionUnitLists[index][units]; // RA2: as above
							do {
								if ( !DoList.Add( eventdata ) ) {
									return(-1);
								}
								count++;
								numunits--;
								memcpy(&eventdata.Data.MegaMission.Whom, ((char *)buf) + pos, datasize);
								memcpy(list++, &eventdata.Data.MegaMission.Whom, datasize); // RA2: save the decoded target in this run
								units++; // RA2: as above
								SavedUnitCounts[index] = units; // RA2: as above
								if (numunits == 1) {
									datasize -= size_of(EventClass, Type);
									DebugString("Got new unit list for player %d with %d units in slot %d\n", house, units, eventdata.Data.MegaMission.Slot); // RA2: as above
									break;
								}
								pos += datasize;
								leftover -= datasize;
							} while (numunits);
						}
					}
					//.........................................................
					// RA2: a run with the units of an earlier list
					//.........................................................
					else if (numunits == 0) {
						pos += (datasize + size_of(EventClass, Type)); // RA2: as above
						leftover -= (datasize + size_of(EventClass, Type)); // RA2: as above
						datasize = sizeof(eventdata.Data.MegaMission.Whom); // RA2: as above
						int units = 1; // RA2: as above
						int index = eventdata.Data.MegaMission.Slot + house * ARRAY_SIZE(MegamissionCounts); // RA2: as above
						numunits = SavedUnitCounts[index]; // RA2: as above
						memcpy(&eventdata.Data.MegaMission.Whom, &MegamissionUnitLists[index][0], datasize); // RA2: as above
						if (eventdata.Frame == 0 || eventdata.Type == 0) { // RA2: as above
							DebugString("Frame %d:Empty megamission decoded from slot %d. Player %d\n", Frame, eventdata.Data.MegaMission.Slot, house); // RA2: as above
						}
						if (numunits) { // RA2: as above
							xTargetClass const * src = &MegamissionUnitLists[index][units]; // RA2: as above
							do { // RA2: as above
								if ( !DoList.Add( eventdata ) ) { // RA2: as above
									return(-1); // RA2: as above
								}
								count++; // RA2: as above
								numunits--; // RA2: as above
								units++; // RA2: as above
								memcpy(&eventdata.Data.MegaMission.Whom, src++, datasize); // RA2: as above
								if (numunits == 1) { // RA2: as above
									datasize = -1; // RA2: as above
									DebugString("Used old unit list in slot %d - %d bytes saved\n", eventdata.Data.MegaMission.Slot, units * sizeof(eventdata.Data.MegaMission.Whom)); // RA2: as above
									break; // RA2: as above
								}
							} while (numunits); // RA2: as above
						}
					}
					break;

				default:
					memcpy ( &eventdata.Data,
						((char *)buf) + pos + size_of(EventClass, Type),
						datasize );
					break;
			}

			if (eventdata.Frame == 0 || eventdata.Type == 0) { // RA2: as above
				DebugString("Frame %d: Empty event being added to queue\n", Frame); // RA2: as above
			}
			if ( !DoList.Add( eventdata ) ) {
				if (eventdata.Type == EventClass::ADDPLAYER) {
					delete [] eventdata.Data.Variable.Pointer;
				}
				return(-1);
			}

			//..................................................................
			// Keep count of how many events we add to the queue
			//..................................................................
			count++;

			pos += (datasize + size_of(EventClass, Type));
			leftover -= (datasize + size_of(EventClass, Type));

			if (leftover) {
				event = (EventClass *)(((char *)buf) + pos);
				datasize = EventClass::EventLength[event->Type];
				if (event->Type == EventClass::MEGAMISSION) {
					datasize += sizeof(numunits);
				}
			} else {
				return(count);
			}
		}
		//.....................................................................
		// FRAMESYNC event: This >should< be the only event in the buffer,
		// and it will be uncompressed.
		//.....................................................................
		else {
			pos += (datasize + size_of(EventClass, Type));
			leftover -= (datasize + size_of(EventClass, Type));
			event = (EventClass *)(((char *)buf) + pos);

			//..................................................................
			// size of FRAMESYNC event - EventType size
			//..................................................................
			datasize = (offsetof(EventClass, Data) +
							size_of(EventClass, Data.FrameInfo)) -
							size_of(EventClass, Type);
		}
	}

	return(count);

}	// end of Extract_Compressed_Events
#endif


/***************************************************************************
 * Execute_DoList -- Executes commands from the DoList                     *
 *                                                                         *
 * This routine executes any events in the DoList that need to be executed *
 * on the current game frame.  The events must be executed in a special    *
 * order, so that all systems execute all events in exactly the same       *
 * order.                                                                  *
 *                                                                         *
 * This routine also handles checking the Game CRC sent by other systems   *
 * against my own, to be sure we're still in sync.                         *
 *                                                                         *
 * INPUT:                                                                  *
 *      max_houses   # houses to execute commands for                      *
 *      base_house   HousesType to start with                              *
 *      net         ptr to connection manager; NULL if none                *
 *      skip_crc      a frame-based countdown timer; if it's non-zero, the *
 *                  CRC check will be skipped.  Ignored if NULL.           *
 *      their_frame   array of their frame #'s                             *
 *      their_sent   array of # commands they've sent                      *
 *      their_recv   array of # commands I've received from them           *
 *                                                                         *
 * (their_xxx are ignored if 'net' is NULL.)                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = some error occurred (CRC error, packet rcv'd too late.)*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
int Execute_DoList(int max_houses, HousesType base_house,
	ConnManClass *net, CDTimerClass<FrameTimerClass> *skip_crc,
	FrameSyncStruct *their)
{
	int count = DoList.Count;
	int house_count = Houses.Count();
	int i, j, k;
	int index;
	int check_crc;
	HouseClass * hptr;

	for (j = 0; j < count; j++) {
		if (DoList[j].Type != EventClass::FRAMEINFO && DoList[j].Frame > NewMaxAheadFrame1 &&
			DoList[j].Frame < NewMaxAheadFrame2) {
			DebugString("DoList: Moving event from frame %d to frame %d\n", DoList[j].Frame, NewMaxAheadFrame2);
			DoList[j].Frame = NewMaxAheadFrame2;
		}
	}

	if (Session.ExitEventPending && Session.ExitEventFrame && Session.ExitEventPlayer != -1) {
		for (j = 0; j < count; j++) {
			if (DoList[j].Type == EventClass::EXIT && DoList[j].ID == Session.ExitEventPlayer &&
				DoList[j].Frame > Session.ExitEventPending && DoList[j].Frame < Session.ExitEventFrame) {
				DebugString("DoList: Moving EXIT event from frame %d to frame %d\n", DoList[j].Frame, Session.ExitEventFrame);
				DoList[j].Frame = Session.ExitEventFrame;
			}
		}
		if (::Frame > Session.ExitEventFrame) {
			Session.ExitEventPending = 0;
			Session.ExitEventFrame = 0;
			Session.ExitEventPlayer = -1;
		}
	}

	for (i = 0; i < house_count; i++) {
		hptr = Houses[i];
		if (!hptr) {
			continue;
		}
		if (!hptr->IsHuman && !hptr->IsPlayerControl) {
			continue;
		}

		for (j = 0; j < count; j++) {
			if (DoList[j].ID == hptr->HeapID && ::Frame >= DoList[j].Frame && !DoList[j].IsExecuted) {
				if (::Frame > DoList[j].Frame && DoList[j].Type != EventClass::FRAMEINFO &&
					Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
					EventClass * event = &DoList[j];
					DebugString("Packet received too late!\n");
					DebugString("--------- Event data: -------------------\n");
					DebugString("Type:       %s\n", EventClass::EventNames[event->Type]);
					DebugString("Frame:      %d\n", event->Frame);
					DebugString("ID:         %d\n", event->ID);
					DebugString("MaxAhead=%d\n", Session.MaxAhead);
					DebugString("Frame=%d\n", ::Frame);
					DebugString("FrameSendRate=%d\n", Session.FrameSendRate);
					net->Num_Connections();
					continue;
				}

				if (DoList[j].Type == EventClass::EXIT || DoList[j].Type == EventClass::OPTIONS || DoList[j].Type == EventClass::PAGEUSER) {
					if (DoList[j].Type == EventClass::EXIT) {
						HouseClass * quithptr = 0;
						for (int player = 0; player < house_count; player++) {
							quithptr = Houses[player];
							if (!quithptr) {
								continue;
							}
							if (quithptr->HeapID == DoList[j].ID) {
								quithptr->IsGiverUpper = true;
								break;
							}
						}
						if (CountAliveTeams(quithptr) == 1 && Session.Type == GAME_INTERNET && !GameStatisticsPacketSent) {
							Session.SawGameCompletion = true;
							Register_Game_End_Time();
							Send_Statistics_Packet(false);
						}
						if (Session.Type == GAME_INTERNET && !GameStatisticsPacketSent && PlayerPtr != 0 &&
							PlayerPtr == quithptr) {
							DebugString("Sending game results because I quit, but didn't see completion");
							Send_Statistics_Packet(false);
						}
					}
					if (Debug_Print_Events) {
						if (DoList[j].Type == EventClass::EXIT) {
							DebugString("Exit Event: ID:%d (%s),  Event Frame:%d,  My Frame:%d\n",
								DoList[j].ID, (char const *)Houses[DoList[j].ID]->IniName, DoList[j].Frame, ::Frame);
						}
					}
					if (DoList[j].ID == PlayerPtr->HeapID) {
						int stamp = DoList.Timings[(DoList.Head + j) & 0x3FFF];
						DebugTime = timeGetTime() - stamp;
						DoList[j].Execute();
					} else if (DoList[j].Type == EventClass::EXIT) {
						DebugString("Processing EXIT event on frame %d\n", ::Frame);
						if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
							Destroy_Null_Connection(i, 0);
						} else if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && net) {
							index = net->Connection_Index(i);
							if (index != -1) {
								for (k = index; k < net->Num_Connections() - 1; k++) {
									their[k] = their[k + 1];
								}
								Destroy_Connection(i, 0);
							}
						}
						if (Session.Play && DoList[j].Type == EventClass::EXIT) {
							hptr->IsHuman = false;
							hptr->IQ = Rule->MaxIQ;
							HouseClass::Computer_Paranoid();
							hptr->IniName = TStringID<HOUSE_NAME_MAX>("Computer");
							wcsncpy(hptr->UIName, StringTable::LoadString("TXT_COMPUTER", 0, "D:\\Ra2\\Queue.CPP", 0x1623), 0x15);
							hptr->UIName[20] = 0;
							DebugString("Removing a player %s:%d\n", "D:\\Ra2\\Queue.CPP", 0x1625);
							Session.NumPlayers--;
						}
					}
				} else if (DoList[j].Type == EventClass::FRAMEINFO) {
					if (!skip_crc || *skip_crc == 0) {
						check_crc = 1;
					} else {
						check_crc = 0;
					}
					if (check_crc && DoList[j].Frame == ::Frame && DoList[j].Data.FrameInfo.Delay < 256) {
						index = (DoList[j].Frame - DoList[j].Data.FrameInfo.Delay) & 0xFF;
						if (CRC[index] != (unsigned long)DoList[j].Data.FrameInfo.CRC) {
							if (ExtraSyncInfo) {
								// The CRC log of every sync slot (k is reduced to a slot
								// number before each call; the loop count is separate).
								k = 0;
								for (int n = 256; n; n--) {
									k &= 0xFF;
									Write_Sync_File(k, &DoList[j]);
									k++;
								}
							} else {
								Print_CRCs(&DoList[j]);
							}
							Session.Suspended++;
							if (WWMessageBox().Process(StringTable::LoadString("TXT_OUT_OF_SYNC", NULL, "D:\\Ra2\\Queue.CPP", 0x1655), StringTable::LoadString("TXT_STOP", NULL, "D:\\Ra2\\Queue.CPP", 0x1656), NULL, NULL) == 0) { // RA2: the answer is tested and both arms resume alike, as ra2-decomp's twin has it (the archive's modem shutdown arm is gone)
								Session.Suspended--;
							} else {
								Session.Suspended--;
							}
							return(0);
						}
					}
				} else {
					int stamp = DoList.Timings[(DoList.Head + j) & 0x3FFF];
					DebugTime = timeGetTime() - stamp;
					if (DoList[j].Type == EventClass::MEGAMISSION) {
						MegaMissionQueue.Add(DoList[j]);
					} else {
						DoList[j].Execute();
					}
				}
				DoList[j].IsExecuted = 1;
			}
		}

		int mission = MISSION_NONE;
		TargetClass target;
		TargetClass destination;
        
        
        int start = 0;
		for (k = 0; k < MegaMissionQueue.Count; k++) {
			bool differ = false;
			if (mission != MISSION_NONE) {
				EventClass & event = MegaMissionQueue[k];
				if ((char)event.Data.MegaMission.Mission != (char)mission) {
					differ = true;
				}
				if ((RTTIType) * (TargetClass *)&event.Data.MegaMission.Target != (RTTIType)target || ((TargetClass *)&event.Data.MegaMission.Target)->Value() != target.Value() ||
					(RTTIType) * (TargetClass *)&event.Data.MegaMission.Destination != (RTTIType)destination || ((TargetClass *)&event.Data.MegaMission.Destination)->Value() != destination.Value()) {
					differ = true;
				}
			}
			mission = (char)MegaMissionQueue[k].Data.MegaMission.Mission;
			target = *(TargetClass *)&MegaMissionQueue[k].Data.MegaMission.Target;
			destination = *(TargetClass *)&MegaMissionQueue[k].Data.MegaMission.Destination;
			if (differ) {
				if (k - start > 1) {
					Spread_Units_Around_Cell(start, k - start);
				}
				start = k;
			} else if (k == MegaMissionQueue.Count - 1 && k - start + 1 > 1) {
				Spread_Units_Around_Cell(start, k - start + 1);
			}
		}
		while (MegaMissionQueue.Count) {
			MegaMissionQueue.First().Execute();
			MegaMissionQueue.Next();
		}
	}
	return 1;
}


/// <summary>
/// Gives the first event of a run of queued mission events whose unit is 'whom' a new destination.
/// The collected missions of a run all went to one cell; when the units are spread around that
/// cell, each unit's event is pointed at the cell it was given.
/// </summary>
/// <param name="start">Index in the collected mission queue where the run starts.</param>
/// <param name="count">The length of the run.</param>
/// <param name="whom">The unit whose event is changed.</param>
/// <param name="target">The new destination.</param>
void __fastcall Update_Event_Destination(int start, int count, TargetClass whom, TargetClass const & target) // RA2: 0x00629220
{
	for (int i = start; i <= start + count; i++) {
		if (whom == *(xTargetClass *)&MegaMissionQueue[i].Data.MegaMission.Whom) {
			*(TargetClass *)&MegaMissionQueue[i].Data.MegaMission.Destination = target;
			return;
		}
	}
}


/// <summary>
/// Spreads a run of mission events that send several units to one cell over the cells around it.
/// The units that can be moved are ordered by their distance from the one nearest their centre.
/// The first keeps the cell; each later one walks from it along its own offset from that unit for
/// up to six cells, and its event gets the first free cell of the same movement zone and level.
/// Units that stand in the same cell are sent to the same destination. Cells taken here are
/// marked while the run is worked out and released again at the end.
/// </summary>
/// <param name="start">Index in the collected mission queue where the run starts.</param>
/// <param name="count">The length of the run.</param>
void __fastcall Spread_Units_Around_Cell(int start, int count) // RA2: 0x00629290
{
	if (!((TargetClass *)&MegaMissionQueue[start].Data.MegaMission.Destination)->Is_Valid() || (RTTIType) * (TargetClass *)&MegaMissionQueue[start].Data.MegaMission.Destination != RTTI_CELL) {
		return;
	}
	CellClass * cptr = ((TargetClass *)&MegaMissionQueue[start].Data.MegaMission.Destination)->As_Cell();
	if (cptr == NULL || cptr == &BlubCell) {
		return;
	}
	Cell dest = cptr->CellID;
	if (dest == CELL_NONE) {
		return;
	}
	DynamicVectorClass<ObjectClass *> list;
	int i;
	for (i = start; i < start + count; i++) {
		TechnoClass *techno = ((TargetClass *)&MegaMissionQueue[i].Data.MegaMission.Whom)->As_Techno();
		bool foot = false;
		if (techno != NULL) {
			foot = (techno->AbstractFlags & 4);
		}
		if (foot && ((FootClass *)techno)->CurrentTube < 0) {
			list.Add(techno);
		}
	}
	if (list.Count() > 0) {
		Coord center = Vector_Center(list);
		ObjectClass *closest = Vector_Closest_Object(list, center);
		IndexClass<int, ObjectClass *> index;
		for (i = 0; i < list.Count(); i++) {
			Coord nearest = closest->Position;
			int key = list[i]->Distance(nearest) * 1000 + i;
			index.Add_Index(key, list[i]);
		}
		list.Clear();
		DynamicVectorClass<Cell> cells;
		DynamicVectorClass<bool> done;
		for (i = 0; i < index.Count(); i++) {
			list.Add(index.Fetch_By_Position(i));
			cells.Add(dest);
			done.Add(false);
		}
		Cell origin = closest->Get_Cell();
		for (i = 0; i < list.Count();) {
			ObjectClass *object = list[i];
			Cell best = dest;
			if (i == 0) {
				Map[dest].IsToGrowVeins = true;
				done[0] = true;
			} else if (!done[i]) {
				Cell fallback = dest;
				Cell cell(object->Destination_Coord(false));
				int level = Map[dest].IsUnderBridge * 4 + Map[dest].Height;
				Vector3 position(dest.X + 0.5, dest.Y + 0.5, 0);
				int offset_x = cell.X - origin.X;
				int offset_y = cell.Y - origin.Y;
				Vector3 step(offset_x, offset_y, 0);
				step = Normalized(step);
				int tries = 0;
				bool detour = false;
				TechnoTypeClass const *type = object->Techno_Type_Class();
				int zone = Map.Get_Cell_Zone(dest, type->MZone, true);
				bool found = false;
				Cell spot;
				for (; tries < 6; tries++) {
					position = step + position;
					spot = Cell((short)position.X, (short)position.Y);
					CellClass *spot_ptr = &Map[spot];
					if (!Map.In_Local_Radar(spot, true)) {
						break;
					}
					int spot_zone = Map.Get_Cell_Zone(spot, type->MZone, spot_ptr->IsUnderBridge);
					int enter = object->Can_Enter_Cell(spot_ptr, FACING_NONE, level, NULL, true);
					if (spot_zone == zone) {
						if (spot_ptr->IsToGrowVeins) {
							fallback = spot;
						} else {
							if (spot_ptr->IsUnderBridge && abs(level - spot_ptr->Height - 4) > 2) {
								break;
							}
							if (!spot_ptr->IsUnderBridge && abs(level - spot_ptr->Height) > 2) {
								break;
							}
							if (enter < 4 || enter == 6) {
								if (detour) {
									int dx = abs(spot.X - dest.X);
									int dy = abs(spot.Y - dest.Y);
									int range = (dx > dy ? dx : dy) + 3;
									// The executable stores range, then keeps the fallback either way
									if (Search.Test_Cell_Walk(spot, dest, (FootClass *)object,
											Map[spot].IsUnderBridge, Map[dest].IsUnderBridge, MZONE_NONE) > range && range > 0) {
										found = false;
									}
									break;
								}
								found = true;
								break;
							}
							detour = true;
						}
					} else {
						detour = true;
					}
				}
				best = !found ? fallback : Cell((short)position.X, (short)position.Y);
				cells[i] = best;
				TargetClass target(&Map[best]);
				Update_Event_Destination(start, count, TargetClass((AbstractClass *)object), target);
				Map[best].IsToGrowVeins = true;
				done[i] = true;
			}
			for (int j = i + 1; j < list.Count(); j++) {
				if (!done[j] && object->Get_Cell() == list[j]->Get_Cell()) {
					TargetClass target(&Map[best]);
					Update_Event_Destination(start, count, TargetClass((AbstractClass *)list[j]), target);
					done[j] = true;
				}
			}
			i++;
		}
		for (i = 0; i < cells.Count(); i++) {
			Map[cells[i]].IsToGrowVeins = false;
		}
	}
}


static void Clean_DoList(ConnManClass *net)
{
	while (DoList.Count) {
		if (net)
			Update_Queue_Mono (net, 7);

		//.....................................................................
		// Discard events that have been executed, OR it's too late to execute.
		//	(This happens if another player exits the game; he'll leave FRAMEINFO
		// events lying around in my queue.  They won't have been "executed",
		//	because his IPX connection was destroyed.)
		//.....................................................................
		if ( (DoList.First().IsExecuted) || (Frame > DoList.First().Frame) ) {
			DoList.Next();
			#ifdef MIRROR_QUEUE
			MirrorList.Next();
			#endif
		}
		else {
			break;
		}
	}

}


static void Queue_Record(void)
{
	if (Frame == 0 && Session.Type != GAME_NORMAL) return;

	int i,j;

	//------------------------------------------------------------------------
	//	Compute # of events to save this frame
	//------------------------------------------------------------------------
	j = 0;
	for (i = 0; i < DoList.Count; i++) {
		if (Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
			j++;
		}
	}

	//------------------------------------------------------------------------
	//	Save the # of events, then all events.
	//------------------------------------------------------------------------
	Session.RecordFile.Write (&j,sizeof(j));
	for (i = 0; i < DoList.Count; i++) {
		if (Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
			Session.RecordFile.Write (&DoList[i],sizeof (EventClass));
			j--;
		}
	}

}


static void Compute_Game_CRC(void)
{
	int i,j;
	InfantryClass *infp;
	UnitClass *unitp;
	BuildingClass *bldgp;
	ObjectClass *objp;

	GameCRC = 0;
	//------------------------------------------------------------------------
	// Infantry
	//------------------------------------------------------------------------
	for (i = 0; i < Infantry.Count(); i++) {
		infp = (InfantryClass *)Infantry[i];
		Add_CRC (&GameCRC, (int)infp->PositionCoord.As_Int() + (int)infp->PrimaryFacing.Current().As_Dir256());
	}

	//------------------------------------------------------------------------
	// Units
	//------------------------------------------------------------------------
	for (i = 0; i < Units.Count(); i++) {
		unitp = (UnitClass *)Units[i];
		Add_CRC (&GameCRC, (int)unitp->PositionCoord.As_Int() + (int)unitp->PrimaryFacing.Current().As_Dir256() +
			(int)unitp->SecondaryFacing.Current().As_Dir256());
	}

	//------------------------------------------------------------------------
	// Buildings
	//------------------------------------------------------------------------
	for (i = 0; i < Buildings.Count(); i++) {
		bldgp = (BuildingClass *)Buildings[i];
		Add_CRC (&GameCRC, (int)bldgp->PositionCoord.As_Int() + (int)bldgp->PrimaryFacing.Current().As_Dir256());
	}

	//------------------------------------------------------------------------
	// Map Layers
	//------------------------------------------------------------------------
	for (i = 0; i < LAYER_COUNT; i++) {
		for (j = 0; j < Map.Layer[i].Count(); j++) {
			objp = Map.Layer[i][j];
			if (objp->RTTI == RTTI_ANIM && objp->Fetch_ID() == -2) {
				continue;
			}
			Add_CRC (&GameCRC, (int)objp->PositionCoord.As_Int() + (int)objp->RTTI);
		}
	}

	//------------------------------------------------------------------------
	// Logic Layers
	//------------------------------------------------------------------------
	for (i = 0; i < Logic.Count(); i++) {
		objp = Logic[i];
		if (objp->RTTI == RTTI_ANIM && objp->Fetch_ID() == -2) {
			continue;
		}
		Add_CRC (&GameCRC, (int)objp->PositionCoord.As_Int() + (int)objp->RTTI);
	}

	//------------------------------------------------------------------------
	//	A random #
	//------------------------------------------------------------------------
	Add_CRC(&GameCRC, Scen->RandomNumber);
	Fill_Sync_State(Frame % 256); // RA2: the game state of this frame is saved for the sync log

}	/* end of Compute_Game_CRC */


/***************************************************************************
 * Add_CRC -- Adds a value to a CRC                                        *
 *                                                                         *
 * INPUT:                                                                  *
 *      crc      ptr to crc                                                *
 *      val      value to add                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none                                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/
void Add_CRC(unsigned int *crc, unsigned int val)
{
	int hibit;

	if ( (*crc) & 0x80000000) {
		hibit = 1;
	}
	else {
		hibit = 0;
	}

	(*crc) <<= 1;
	(*crc) += val;
	(*crc) += hibit;
}


inline char const * Sync_Target_Name(AbstractClass * target, int & id)
{
	bool is_object;
	char const * name;
	id = 0;
	name = "None";
	if (target) is_object = (target->AbstractFlags & 1) != 0;
	if (target) {
		if (is_object) id = target->Fetch_Heap_ID();
		else if (target->RTTI == RTTI_CELL) id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
		name = Name_From_RTTI(target->RTTI);
	}
	return name;
}

void __fastcall Print_CRCs(EventClass * event)
{
	char filename[0x200];
	char time[0x80];
	char builder[0x80];
	char machine[0x80];
	int family;
	bool has_mmx;
	int i, j;
	int house;

	sprintf(filename, "SYNC%01d.TXT", PlayerPtr->HeapID);
	DebugString("Writing sync data to file %s\n", filename);
	MonoClass * mono = MonoClass::Get_Page(4);
	mono->Clear();
	mono->Set_Cursor(0, 0);
	FILE * file = fopen(filename, "wt");
	if (!file) {
		DebugString("Error - failed to open sync file for writing. Error code %d - %s\n", GetLastError(), file);
		return;
	}

	fprintf(file, "RA2 Sync\n");
	fprintf(file, "\nVersion %s\n", Version_Name());
	fprintf(file, "Internal Version %s\n", VerNum.Version_Name());
	time[0] = 0;
	Build_Date_String(time, sizeof(time));
	fprintf(file, "Release Build: %s by %s - %s\r\n", Build_Number_String(machine, sizeof(machine)), Build_By_String(builder, sizeof(builder)), time);
	family = 5;
	has_mmx = 0;
	Get_CPU_Type(family, has_mmx, 0, 0);
	fprintf(file, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), has_mmx ? "Yes" : "No", &VendorID);
	fprintf(file, "Frames: %d\n", ::Frame);
	fprintf(file, "Average FPS: %d\n", SecondsPassed ? TotalFrames / SecondsPassed : 0);
	fprintf(file, "Max MaxAhead: %d\n", Session.MaxMaxAhead);
	fprintf(file, "Latency setting: %d\n", Session.LatencyFudge);
	fprintf(file, "Game speed setting: %d\n", Options.GameSpeed);
	fprintf(file, "FrameSendRate: %d\n", Session.FrameSendRate);

	if (PacketTransport) {
		for (i = 0; i < PacketTransport->Get_Num_Local_Addresses(); i++) {
			unsigned int * address = (unsigned int *)PacketTransport->Get_Local_Address(i);
			if (address) {
				unsigned int ip = *address;
				fprintf(file, "Local address: %d.%d.%d.%d\n", ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24);
			}
		}
	}

	for (i = 0; i < 8; i++) {
		MPStatsType & statistics = Session.ConnectionStats[i];
		if (statistics.Name[0]) {
			NetNumType net;
			NetNodeType node;
			statistics.Address.Get_Address(net, node);
			fprintf(file, "\nName: %s\n", statistics.Name);
			fprintf(file, "Address: %d.%d.%d.%d\n", node[0], node[1], node[2], node[3]);
			fprintf(file, "Max avg round trip: %d\n", statistics.MaxAvgRoundTrip);
			fprintf(file, "Max round trip: %d\n", statistics.MaxRoundTrip);
			fprintf(file, "Resends: %d\n", statistics.Resends);
			fprintf(file, "Frame sync stalls: %d\n", statistics.FrameSyncStalls);
			fprintf(file, "Command cound stalls: %d\n", statistics.CommandCountStalls);
			fprintf(file, "Lost: %d\n", statistics.Lost);
			fprintf(file, "Percent lost: %d\n", statistics.PercentLost);
		}
	}

	for (i = 0; i < 256; i++) {
		fprintf(file, "CRC[%d]=%x\n", i, CRC[i]);
	}

	for (house = 0; house < Houses.Count(); house++) {
		GameCRC = 0;
		HouseClass * housep = Houses[house];
		fprintf(file, "%s: IsHuman:%d  Color:%s  ID:%d  HouseType:%s\n", (char const *)housep->IniName, housep->IsHuman,
			ColorSchemes[housep->Class->Scheme]->Name, housep->HeapID, (char const *)HouseTypes[housep->Class->HeapID]->Name());
		mono->Printf("House %s:%x\n", housep->Class->Name(), GameCRC);
	}

	for (house = 0; house < Houses.Count(); house++) {
		HouseClass * housep = Houses[house];
		GameCRC = 0;
		fprintf(file, "-------------------- %s Infantry -------------------\n", housep->Class->Name());
		for (i = 0; i < Infantry.Count(); i++) {
			InfantryClass * infp = Infantry[i];
			if (infp->House == housep) {
				int target_id, navcom_id;
				char const * target_name;
				char const * navcom_name;
				Add_CRC(&GameCRC, infp->PositionCoord.As_Int() + (int)infp->PrimaryFacing.Current().As_Dir256());
				target_name = Sync_Target_Name(infp->TarCom, target_id);
				navcom_name = Sync_Target_Name(infp->NavCom, navcom_id);
				fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d) Speed:%d NavCom:%s(%d)\n",
					infp->PositionCoord.As_Int(), (int)infp->PrimaryFacing.Current().As_Dir256(), infp->Get_Mission(),
					infp->Class->HeapID, infp->Class->Name(), target_name, target_id, int(infp->Speed * 256.0),
					navcom_name, navcom_id);
			}
		}
		mono->Printf("%s Infantry:%x\n", housep->Class->Name(), GameCRC);
	}

	for (house = 0; house < Houses.Count(); house++) {
		HouseClass * housep = Houses[house];
		GameCRC = 0;
		fprintf(file, "-------------------- %s Units -------------------\n", housep->Class->Name());
		for (i = 0; i < Units.Count(); i++) {
			UnitClass * unitp = Units[i];
			if (unitp->House == housep) {
				int target_id, navcom_id;
				char const * target_name;
				char const * navcom_name;
				Add_CRC(&GameCRC, unitp->PositionCoord.As_Int() + (int)unitp->PrimaryFacing.Current().As_Dir256()
					+ (int)unitp->SecondaryFacing.Current().As_Dir256());
				target_name = Sync_Target_Name(unitp->TarCom, target_id);
				navcom_name = Sync_Target_Name(unitp->NavCom, navcom_id);
				fprintf(file, "COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)   TrkNum:%d TrkInd:%d SpdAc:%d\n",
					unitp->PositionCoord.As_Int(), (int)unitp->PrimaryFacing.Current().As_Dir256(), (int)unitp->SecondaryFacing.Current().As_Dir256(),
					unitp->Get_Mission(), unitp->Class->HeapID, unitp->Class->Name(), target_name, target_id,
					navcom_name, navcom_id, unitp->Locomotion->Get_Track_Number(),
					unitp->Locomotion->Get_Track_Index(), unitp->Locomotion->Get_Speed_Accum());
			}
		}
		mono->Printf("%s Units:%x\n", housep->Class->Name(), GameCRC);
	}

	for (house = 0; house < Houses.Count(); house++) {
		HouseClass * housep = Houses[house];
		GameCRC = 0;
		fprintf(file, "-------------------- %s Buildings -------------------\n", housep->Class->Name());
		for (i = 0; i < Buildings.Count(); i++) {
			BuildingClass * bldgp = Buildings[i];
			if (bldgp->House == housep) {
				Add_CRC(&GameCRC, bldgp->PositionCoord.As_Int() + (int)bldgp->PrimaryFacing.Current().As_Dir256());
				fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%x\n",
					bldgp->PositionCoord.As_Int(), (int)bldgp->PrimaryFacing.Current().As_Dir256(), bldgp->Get_Mission(),
					bldgp->Class->HeapID, bldgp->Class->Name(), TargetClass(bldgp).Encode());
			}
		}
		mono->Printf("%s Buildings:%x\n", housep->Class->Name(), GameCRC);
	}

	for (house = 0; house < Houses.Count(); house++) {
		HouseClass * housep = Houses[house];
		GameCRC = 0;
		fprintf(file, "-------------------- %s Aircraft -------------------\n", housep->Class->Name());
		for (i = 0; i < Aircraft.Count(); i++) {
			AircraftClass * aircraftp = Aircraft[i];
			if (aircraftp->House == housep) {
				Add_CRC(&GameCRC, aircraftp->PositionCoord.As_Int() + (int)aircraftp->PrimaryFacing.Current().As_Dir256());
				fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%x\n",
					aircraftp->PositionCoord.As_Int(), (int)aircraftp->PrimaryFacing.Current().As_Dir256(), aircraftp->Get_Mission(),
					aircraftp->Class->HeapID, aircraftp->Class->Name(), TargetClass(aircraftp).Encode());
			}
		}
		mono->Printf("%s Buildings:%x\n", housep->Class->Name(), GameCRC);
	}

	GameCRC = 0;
	for (i = 0; i < 5; i++) {
		fprintf(file, ">>>> MAP LAYER %d <<<<\n", i);
		LayerClass & layer = Map.Layer[i];
		for (j = 0; j < layer.Count(); j++) {
			ObjectClass * objp = layer[j];
			if (objp->RTTI == RTTI_ANIM && objp->Fetch_ID() == -2) continue;
			Add_CRC(&GameCRC, objp->PositionCoord.As_Int() + objp->RTTI);
			fprintf(file, "Object %d: %x ", j, objp->PositionCoord.As_Int());
			if (objp->RTTI == RTTI_WAVE || objp->RTTI == RTTI_LIGHT) {
				fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)objp->RTTI), 0);
			} else {
				fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)objp->RTTI),
					objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
			}
			HouseClass * owner = objp->Owner_HouseClass();
			if (owner) {
				fprintf(file, "Owner: %s\n", owner->Class->Name());
			} else {
				fprintf(file, "Owner: NONE\n");
			}

		}
	}
	mono->Printf("Map Layers:%x  \n", GameCRC);

	GameCRC = 0;
	fprintf(file, ">>>> LOGIC LAYER <<<<\n");
	for (i = 0; i < Logic.Count(); i++) {
		ObjectClass * objp = Logic[i];
		if (objp->RTTI == RTTI_ANIM && objp->Fetch_ID() == -2) continue;
		Add_CRC(&GameCRC, objp->PositionCoord.As_Int() + objp->RTTI);
		fprintf(file, "Object %d: %x ", i, objp->PositionCoord.As_Int());
		if (objp->RTTI == RTTI_AIRCRAFT) {
			fprintf(file, "Aircraft  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_ANIM) {
			fprintf(file, "Anim      (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_BUILDING) {
			fprintf(file, "Building  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_BULLET) {
			fprintf(file, "Bullet    (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_INFANTRY) {
			fprintf(file, "Infantry  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_OVERLAY) {
			fprintf(file, "Overlay   (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_SMUDGE) {
			fprintf(file, "Smudge    (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_TERRAIN) {
			fprintf(file, "Terrain   (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		} else if (objp->RTTI == RTTI_UNIT) {
			fprintf(file, "Unit      (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->IniName);
		}
		HouseClass * owner = objp->Owner_HouseClass();
		if (owner) {
			fprintf(file, "Owner: %s\n", owner->Class->Name());
		} else {
			fprintf(file, "Owner: NONE\n");
		}

	}
	mono->Printf("Logic:%x  \n", GameCRC);

	int random = Scen->RandomNumber();
	mono->Printf("Random Number:%x  \n", random);
	fprintf(file, "\nRandom Number:%d\n", random);
	mono->Printf("My Frame:%d  \n", ::Frame);
	fprintf(file, "My Frame:%d\n", ::Frame);

	if (event) {
		fprintf(file, "Offending event:\n");
		fprintf(file, "Type:         %d\n", event->Type);
		fprintf(file, "Frame:        %d\n", event->Frame);
		fprintf(file, "ID:           %x\n", (char)event->ID);
		fprintf(file, "CRC:          %x\n", event->Data.FrameInfo.CRC);
		fprintf(file, "CommandCount: %d\n", (unsigned short)event->Data.FrameInfo.CommandCount);
		fprintf(file, "Delay:        %d\n", event->Data.FrameInfo.Delay);
	}

	CRCEngine * crc = new CRCEngine;
	fprintf(file, "\n\n*************** House Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (i = 0; i < HouseTypes.Count(); i++) {
		HouseTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** House CRCs*******************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Houses.Count(); i++) {
		Houses[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Unit Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < UnitTypes.Count(); i++) {
		UnitTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Unit CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Units.Count(); i++) {
		Units[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Infantry Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < InfantryTypes.Count(); i++) {
		InfantryTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Infantry CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Infantry.Count(); i++) {
		Infantry[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** BuildingType CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < BuildingTypes.Count(); i++) {
		BuildingTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Building CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Buildings.Count(); i++) {
		Buildings[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Aircraft Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < AircraftTypes.Count(); i++) {
		AircraftTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Aircraft CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Aircraft.Count(); i++) {
		Aircraft[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Anim CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Anims.Count(); i++) {
		AnimClass * animp = Anims[i];
		if (animp->Fetch_ID() != -2) {
			animp->Compute_CRC(*crc);
			fprintf(file, "%05d      %08x\n", i, *crc);
		}
	}
	fprintf(file, "\n\n*************** Anim Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < AnimTypes.Count(); i++) {
		AnimTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Task Force CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < TaskForces.Count(); i++) {
		TaskForces[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Team Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < TeamTypes.Count(); i++) {
		TeamTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Team CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Teams.Count(); i++) {
		Teams[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Script Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < ScriptTypes.Count(); i++) {
		ScriptTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Script CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Scripts.Count(); i++) {
		Scripts[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Tag Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < TagTypes.Count(); i++) {
		TagTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Tag CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Tags.Count(); i++) {
		Tags[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Trigger Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < TriggerTypes.Count(); i++) {
		TriggerTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Trigger CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Triggers.Count(); i++) {
		Triggers[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Action CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Actions.Count(); i++) {
		Actions[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Event CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Events.Count(); i++) {
		Events[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Factory CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Factories.Count(); i++) {
		Factories[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Voxel Anim Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < VoxelAnimTypes.Count(); i++) {
		VoxelAnimTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Voxel Anim CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < VoxelAnims.Count(); i++) {
		VoxelAnims[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Warhead CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Warheads.Count(); i++) {
		Warheads[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Weapon CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Weapons.Count(); i++) {
		Weapons[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Particle Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < ParticleTypes.Count(); i++) {
		ParticleTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Particle CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Particles.Count(); i++) {
		Particles[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Particle System CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < ParticleSystems.Count(); i++) {
		ParticleSystems[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Particle System Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < ParticleSystemTypes.Count(); i++) {
		ParticleSystemTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Bullet Type's**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < BulletTypes.Count(); i++) {
		BulletTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Bullet CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Bullets.Count(); i++) {
		Bullets[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Waypoint Path CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < WaypointPaths.Count(); i++) {
		WaypointPaths[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Smudge Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < SmudgeTypes.Count(); i++) {
		SmudgeTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Overlay Type CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < OverlayTypes.Count(); i++) {
		OverlayTypes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Light Source CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < LightSources.Count(); i++) {
		LightSources[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Building Light CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < BuildingLights.Count(); i++) {
		BuildingLights[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Tube CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Tubes.Count(); i++) {
		Tubes[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Sides CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Sides.Count(); i++) {
		Sides[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Tiberium CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < Tiberiums.Count(); i++) {
		Tiberiums[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	fprintf(file, "\n\n*************** Em Pulse CRCs**************\n\n");
	fprintf(file, "Index     CRC\n");
	fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	delete crc;
	crc = new CRCEngine;
	for (i = 0; i < EMPulseClass::EMPulses.Count(); i++) {
		EMPulseClass::EMPulses[i]->Compute_CRC(*crc);
		fprintf(file, "%05d      %08x\n", i, *crc);
	}
	delete crc;
	fclose(file);
}	/* end of Print_CRCs */


/// <summary>
/// Saves the game state of this frame in a slot of the sync log: the houses, every infantry, unit,
/// aircraft and building, and the objects of the map and logic layers. Without the sync log only the
/// frame's random number is kept.
/// </summary>
void __fastcall Fill_Sync_State(int index) // RA2: 0x0062CF40
{
	if (!ExtraSyncInfo) {
		SyncRandomNumber = Scen->RandomNumber();
		return;
	}

	SyncState *state = &SyncStates[index];
	state->GameCRC = GameCRC;
	state->RandomNumber = Scen->RandomNumber();
	state->Frame = ::Frame;

	int i, j;
	AbstractClass *target;
	int id;
	bool is_object;     // assigned only for a target (left unset otherwise, as the original keeps it in memory)
	char const *name;
	for (i = 0; i < 8; i++) {
		SyncHouse *sh = &state->Houses[i];
		sh->ID = -1;
		if (i < Houses.Count()) {
			sh->Name[0] = 0;
			for (j = 0; j < Session.Players.Count(); j++) {
				if (Session.Players[j]->Player.ID == i) {
					strncpy(sh->Name, Wide_To_Narrow(Session.Players[j]->Name), sizeof(sh->Name));
				}
			}
			if (sh->Name[0] == 0 && (_stricmp(Houses[i]->Class->Name(), "Neutral") == 0 ||
									 _stricmp(Houses[i]->Class->Name(), "Special") == 0)) {
				strcpy(sh->Name, Houses[i]->Class->Name());
			}
			sh->IsHuman = Houses[i]->IsHuman;
			sh->Color[sizeof(sh->Color) - 1] = 0;
			strncpy(sh->Color, ColorSchemes[Houses[i]->Scheme]->Name, sizeof(sh->Color) - 1);
			sh->ID = i;
			sh->HouseType[sizeof(sh->HouseType) - 1] = 0;
			strncpy(sh->HouseType, HouseTypes[Houses[i]->Class->HeapID]->Name(), sizeof(sh->HouseType) - 1);
		}
	}

	int objects = Infantry.Count() + Units.Count() + Aircraft.Count() + Buildings.Count();
	int layered = Logic.Count();
	for (i = 0; i < 5; i++) {
		layered += DisplayClass::Layer[i].Count();
	}
	if (!state->Buffer) {
		state->Buffer = new char[state->BufferSize];
	}
	int size = objects * sizeof(SyncObject) + layered * sizeof(SyncLayerObject);
	if (size + 0x400 > state->BufferSize) {
		delete[] state->Buffer;
		state->BufferSize = (size + 0xFFFF) & 0xFFFF0000;
		DebugString("Adjusted allocation of log %d to %dKb\n", index, state->BufferSize >> 10);
		state->Buffer = new char[state->BufferSize];
	}

	SyncObject *obj = (SyncObject *)state->Buffer;
	int count = 0;
	state->Objects = obj;

	state->Counts[0] = Infantry.Count();
	for (count = 0; count < Infantry.Count(); count++) {
		InfantryClass *infp = Infantry[count];
		obj->Coord = infp->PositionCoord.As_Int();
		obj->Facing = infp->PrimaryFacing.Current().As_Dir256();
		obj->CurrentMission = infp->Get_Mission();
		obj->Type = infp->Class->HeapID;
		obj->Speed = infp->Speed * 256.0;
		obj->Owner = infp->House->HeapID;
		obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
		strncpy(obj->TypeName, infp->Class->Name(), sizeof(obj->TypeName) - 1);
		target = infp->NavCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
			obj->NavComID = id;
			strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
		} else {
			obj->NavComID = 0;
			obj->NavCom[0] = 0;
		}
		target = infp->TarCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->Target[sizeof(obj->Target) - 1] = 0;
			obj->TargetID = id;
			strncpy(obj->Target, name, sizeof(obj->Target) - 1);
		} else {
			obj->TargetID = 0;
			obj->Target[0] = 0;
		}
		obj++;
	}

	state->Counts[1] = Units.Count();
	for (i = 0; i < Units.Count(); i++) {
		UnitClass *unitp = Units[i];
		obj->Coord = unitp->PositionCoord.As_Int();
		obj->Facing = unitp->PrimaryFacing.Current().As_Dir256();
		obj->Facing2 = unitp->SecondaryFacing.Current().As_Dir256();
		obj->CurrentMission = unitp->Get_Mission();
		obj->Type = unitp->Class->HeapID;
		obj->Speed = unitp->Speed * 256.0;
		obj->Owner = unitp->House->HeapID;
		obj->TrackNumber = unitp->Locomotion->Get_Track_Number();
		obj->TrackIndex = unitp->Locomotion->Get_Track_Index();
		obj->SpeedAccum = unitp->Locomotion->Get_Speed_Accum();
		obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
		strncpy(obj->TypeName, unitp->Class->Name(), sizeof(obj->TypeName) - 1);
		target = unitp->NavCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
			obj->NavComID = id;
			strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
		} else {
			obj->NavComID = 0;
			obj->NavCom[0] = 0;
		}
		target = unitp->TarCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->Target[sizeof(obj->Target) - 1] = 0;
			obj->TargetID = id;
			strncpy(obj->Target, name, sizeof(obj->Target) - 1);
		} else {
			obj->TargetID = 0;
			obj->Target[0] = 0;
		}
		obj++;
		count++;
	}

	state->Counts[2] = Aircraft.Count();
	for (i = 0; i < Aircraft.Count(); i++) {
		AircraftClass *aircraftp = Aircraft[i];
		obj->Coord = aircraftp->PositionCoord.As_Int();
		obj->Facing = aircraftp->PrimaryFacing.Current().As_Dir256();
		obj->CurrentMission = aircraftp->Get_Mission();
		obj->Type = aircraftp->Class->HeapID;
		obj->Speed = aircraftp->Speed * 256.0;
		obj->Owner = aircraftp->House->HeapID;
		obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
		strncpy(obj->TypeName, aircraftp->Class->Name(), sizeof(obj->TypeName) - 1);
		target = aircraftp->NavCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
			obj->NavComID = id;
			strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
		} else {
			obj->NavComID = 0;
			obj->NavCom[0] = 0;
		}
		target = aircraftp->TarCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->Target[sizeof(obj->Target) - 1] = 0;
			obj->TargetID = id;
			strncpy(obj->Target, name, sizeof(obj->Target) - 1);
		} else {
			obj->TargetID = 0;
			obj->Target[0] = 0;
		}
		obj++;
		count++;
	}

	state->Counts[3] = Buildings.Count();
	for (i = 0; i < Buildings.Count(); i++) {
		BuildingClass *bldgp = Buildings[i];
		obj->Coord = bldgp->PositionCoord.As_Int();
		obj->Facing = bldgp->PrimaryFacing.Current().As_Dir256();
		obj->CurrentMission = bldgp->Get_Mission();
		obj->Type = bldgp->Class->HeapID;
		obj->Owner = bldgp->House->HeapID;
		obj->NavComID = 0;
		obj->NavCom[0] = 0;
		obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
		strncpy(obj->TypeName, bldgp->Class->Name(), sizeof(obj->TypeName) - 1);
		target = bldgp->TarCom;
		id = 0;
		if (target) {
			is_object = target->AbstractFlags & 1; // RA2: the techno bit, read once
		}
		if (target) {
			if (is_object) {
				id = target->Fetch_Heap_ID();
			} else if (target->Fetch_RTTI() == RTTI_CELL) {
				id = ((CellClass *)target)->CellID.Y << 16 | ((CellClass *)target)->CellID.X;
			}
			name = Name_From_RTTI((RTTIType)target->Fetch_RTTI());
			obj->Target[sizeof(obj->Target) - 1] = 0;
			obj->TargetID = id;
			strncpy(obj->Target, name, sizeof(obj->Target) - 1);
		} else {
			obj->TargetID = 0;
			obj->Target[0] = 0;
		}
		obj++;
		count++;
	}
	state->ObjectCount = count;

	SyncLayerObject *obj_layer = (SyncLayerObject *)obj;
	for (i = 0; i < 5; i++) {
		state->Layers[i] = obj_layer;
		count = 0;
		for (j = 0; j < DisplayClass::Layer[i].Count(); j++) {
			ObjectClass *objp = DisplayClass::Layer[i][j];
			if (objp->Fetch_RTTI() == RTTI_ANIM && objp->Fetch_ID() == -2) {
				continue;
			}
			obj_layer->Value = objp->PositionCoord.As_Int();
			obj_layer->RTTI = objp->Fetch_RTTI();
			if (objp->Fetch_RTTI() != RTTI_WAVE && objp->Fetch_RTTI() != RTTI_LIGHT) {
				obj_layer->Type = objp->Class_Of()->Fetch_Heap_ID();
				obj_layer->Name[sizeof(obj_layer->Name) - 1] = 0;
				strncpy(obj_layer->Name, objp->Class_Of()->Name(), sizeof(obj_layer->Name) - 1);
			} else {
				obj_layer->Type = -1;
				obj_layer->Name[0] = 0;
			}
			HouseClass *owner = objp->Owner_HouseClass();
			if (owner) {
				obj_layer->Owner = owner->HeapID;
			} else {
				obj_layer->Owner = -1;
			}
			obj_layer++;
			count++;
		}
		state->LayerCounts[i] = count;
	}

	state->Logic = obj_layer;
	count = 0;
	for (i = 0; i < Logic.Count(); i++) {
		ObjectClass *objp = Logic[i];
		if (objp->Fetch_RTTI() == RTTI_ANIM && objp->Fetch_ID() == -2) {
			continue;
		}
		obj_layer->Value = objp->PositionCoord.As_Int();
		obj_layer->RTTI = objp->Fetch_RTTI();
		if (objp->Fetch_RTTI() != RTTI_WAVE && objp->Fetch_RTTI() != RTTI_LIGHT) {
			obj_layer->Type = objp->Class_Of()->Fetch_Heap_ID();
			obj_layer->Name[sizeof(obj_layer->Name) - 1] = 0;
			strncpy(obj_layer->Name, objp->Class_Of()->Name(), sizeof(obj_layer->Name) - 1);
		} else {
			obj_layer->Type = -1;
			obj_layer->Name[0] = 0;
		}
		HouseClass *owner = objp->Owner_HouseClass();
		if (owner) {
			obj_layer->Owner = owner->HeapID;
		} else {
			obj_layer->Owner = -1;
		}
		obj_layer++;
		count++;
	}
	state->LogicCount = count;
}


/// <summary>
/// Writes the sync log of a slot to a file, with the settings of the game and the connections, the
/// CRC of every slot and the saved game state, so that two logs of the same frame can be compared.
/// </summary>
void __fastcall Write_Sync_File(int index, EventClass *event) // RA2: 0x0062DBA0
{
	SyncState *state = &SyncStates[index];
	char filename[0x200];
	char time[0x80];
	char builder[0x80];
	char machine[0x80];
	int family;
	bool has_mmx;
	int players;
	int house;
	int i;
	SyncObject *objects;

	sprintf(filename, "SYNC%01d_%03d.TXT", PlayerPtr->HeapID, index);
	DebugString("Writing sync data to file %s\n", filename);
	FILE *file = fopen(filename, "wt");
	if (!file) {
		DebugString("Error - failed to open sync file for writing. Error code %d - %s\n", GetLastError(), file);
		return;
	}

	fprintf(file, "RA2 Sync\n");
	fprintf(file, "\nVersion %s\n", Version_Name());
	fprintf(file, "Internal Version %s\n", VerNum.Version_Name());
	time[0] = 0;
	Build_Date_String(time, sizeof(time));
	fprintf(file, "Release Build: %s by %s - %s\r\n", Build_Number_String(machine, sizeof(machine)), Build_By_String(builder, sizeof(builder)), time);
	family = 5;
	has_mmx = 0;
	Get_CPU_Type(family, has_mmx, NULL, 0);
	fprintf(file, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), has_mmx ? "Yes" : "No", &VendorID);
	fprintf(file, "Frames: %d\n", ::Frame);
	fprintf(file, "Average FPS: %d\n", SecondsPassed != 0 ? TotalFrames / SecondsPassed : 0);
	fprintf(file, "Max MaxAhead: %d\n", Session.MaxMaxAhead);
	fprintf(file, "Latency setting: %d\n", Session.LatencyFudge);
	fprintf(file, "Game speed setting: %d\n", Options.GameSpeed);
	fprintf(file, "FrameSendRate: %d\n", Session.FrameSendRate);
	fprintf(file, "\nFrame: %d\n", state->Frame);
	fprintf(file, "Slot: %d\n", index);

	if (PacketTransport) {
		for (i = 0; i < PacketTransport->Get_Num_Local_Addresses(); i++) {
			unsigned int *address = (unsigned int *)PacketTransport->Get_Local_Address(i);
			if (address) {
				unsigned int ip = *address;
				fprintf(file, "Local address: %d.%d.%d.%d\n", ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24);
			}
		}
	}

	for (i = 0; i < 8; i++) {
		MPStatsType & statistics = Session.ConnectionStats[i];
		if (statistics.Name[0]) {
			NetNumType net;
			NetNodeType node;
			statistics.Address.Get_Address(net, node);
			fprintf(file, "\nName: %s\n", statistics.Name);
			fprintf(file, "Address: %d.%d.%d.%d\n", node[0], node[1], node[2], node[3]);
			fprintf(file, "Max avg round trip: %d\n", statistics.MaxAvgRoundTrip);
			fprintf(file, "Max round trip: %d\n", statistics.MaxRoundTrip);
			fprintf(file, "Resends: %d\n", statistics.Resends);
			fprintf(file, "Frame sync stalls: %d\n", statistics.FrameSyncStalls);
			fprintf(file, "Command cound stalls: %d\n", statistics.CommandCountStalls);
			fprintf(file, "Lost: %d\n", statistics.Lost);
			fprintf(file, "Percent lost: %d\n", statistics.PercentLost);
		}
	}

	for (i = 0; i < 256; i++) {
		if (i == index) {
			fprintf(file, "CRC[%d]=%x  *****\n", i, CRC[i]);
		} else {
			fprintf(file, "CRC[%d]=%x\n", i, CRC[i]);
		}
	}

	players = 0;
	for (i = 0; i < 8; i++) {
		SyncHouse &h = state->Houses[i];
		if (h.ID != -1) {
			players = h.ID;
			fprintf(file, "%s: IsHuman:%d  Color:%s  ID:%d  HouseType:%s\n", h.Name, h.IsHuman, h.Color, h.ID, h.HouseType);
		}
	}

	objects = state->Objects;
	for (house = 0; house < players; house++) {
		if (state->Houses[house].ID != -1) {
			fprintf(file, "-------------------- %s Infantry -------------------\n", state->Houses[house].Name);
			SyncObject *o = objects;
			for (i = 0; i < state->Counts[0]; i++) {
				if (o->Owner == house) {
					fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d) Speed:%d NavCom:%s(%d)\n",
						o->Coord, o->Facing, o->CurrentMission, o->Type, o->TypeName, o->Target, o->TargetID, o->Speed, o->NavCom, o->NavComID);
				}
				o++;
			}
		}
	}
	objects += state->Counts[0];
	for (house = 0; house < players; house++) {
		if (state->Houses[house].ID != -1) {
			fprintf(file, "-------------------- %s Units -------------------\n", state->Houses[house].Name);
			SyncObject *o = objects;
			for (i = 0; i < state->Counts[1]; i++) {
				if (o->Owner == house) {
					fprintf(file, "COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)   TrkNum:%d TrkInd:%d SpdAc:%d\n",
						o->Coord, o->Facing, o->Facing2, o->CurrentMission, o->Type, o->TypeName, o->Target, o->TargetID, o->NavCom, o->NavComID, o->TrackNumber, o->TrackIndex, o->SpeedAccum);
				}
				o++;
			}
		}
	}
	objects += state->Counts[1];
	for (house = 0; house < players; house++) {
		if (state->Houses[house].ID != -1) {
			fprintf(file, "-------------------- %s Aircraft -------------------\n", state->Houses[house].Name);
			SyncObject *o = objects;
			for (i = 0; i < state->Counts[2]; i++) {
				if (o->Owner == house) {
					fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)\n",
						o->Coord, o->Facing, o->CurrentMission, o->Type, o->TypeName, o->Target, o->TargetID, o->NavCom, o->NavComID);
				}
				o++;
			}
		}
	}
	objects += state->Counts[2];
	for (house = 0; house < players; house++) {
		if (state->Houses[house].ID != -1) {
			fprintf(file, "-------------------- %s Buildings -------------------\n", state->Houses[house].Name);
			SyncObject *o = objects;
			for (i = 0; i < state->Counts[3]; i++) {
				if (o->Owner == house) {
					fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)\n",
						o->Coord, o->Facing, o->CurrentMission, o->Type, o->TypeName, o->Target, o->TargetID);
				}
				o++;
			}
		}
	}

	for (int layer = 0; layer < 5; layer++) {
		fprintf(file, ">>>> MAP LAYER %d <<<<\n", layer);
		for (i = 0; i < state->LayerCounts[layer]; i++) {
			SyncLayerObject &o = state->Layers[layer][i];
			fprintf(file, "Object %d: %x ", i, o.Value);
			if (o.RTTI == RTTI_WAVE || o.RTTI == RTTI_LIGHT) {
				fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)o.RTTI), 0);
			} else {
				fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)o.RTTI), o.Type, o.Name);
			}
			if (o.Owner != -1) {
				fprintf(file, "Owner: %s\n", state->Houses[o.Owner].Name);
			} else {
				fprintf(file, "Owner: None\n");
			}
		}
	}

	fprintf(file, ">>>> LOGIC LAYER <<<<\n");
	for (i = 0; i < state->LogicCount; i++) {
		SyncLayerObject &o = state->Logic[i];
		fprintf(file, "Object %d: %x ", i, o.Value);
		if (o.RTTI == RTTI_WAVE || o.RTTI == RTTI_LIGHT) {
			fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)o.RTTI), 0);
		} else {
			fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)o.RTTI), o.Type, o.Name);
		}
		if (o.Owner != -1) {
			fprintf(file, "Owner: %s\n", state->Houses[o.Owner].Name);
		} else {
			fprintf(file, "Owner: None\n");
		}
	}

	fprintf(file, "\nRandom Number:%d\n", state->RandomNumber);
	if (event) {
		fprintf(file, "Offending event:\n");
		fprintf(file, "Type:         %d\n", event->Type);
		fprintf(file, "Frame:        %d\n", event->Frame);
		fprintf(file, "ID:           %x\n", event->ID);
		fprintf(file, "CRC:          %x\n", event->Data.FrameInfo.CRC);
		fprintf(file, "CommandCount: %d\n", event->Data.FrameInfo.CommandCount);
		fprintf(file, "Delay:        %d\n", event->Data.FrameInfo.Delay);
	}
	fclose(file);
}


/***************************************************************************
 * Dump_Packet_Too_Late_Stuff -- Dumps a debug file to disk                *
 *                                                                         *
 * INPUT:                                                                  *
 *      event      ptr to event to print                                   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/28/1996 BRR : Created.                                             *
 *=========================================================================*/
void __fastcall Dump_Packet_Too_Late_Stuff(EventClass *event) // RA2: as above
{
	DebugString("Packet received too late!\n");
	DebugString("--------- Event data: -------------------\n");
	DebugString("Type:       %s\n", EventClass::EventNames[event->Type]); // RA2: as above
	DebugString("Frame:      %d\n", event->Frame); // RA2: as above
	DebugString("ID:         %d\n", event->ID); // RA2: as above
	DebugString("MaxAhead=%d\n", Session.MaxAhead); // RA2: as above
	DebugString("Frame=%d\n", Frame); // RA2: as above
	DebugString("FrameSendRate=%d\n", Session.FrameSendRate);
}


/***************************************************************************
 * Check_Mirror -- Checks mirror memory                                    *
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
 *   10/14/1996 BRR : Created.                                             *
 *=========================================================================*/
void Check_Mirror(void)
{
}


/// <summary>
/// Works out how many frames behind us a player is, from the frame he reported and the
/// time his packets take to arrive. Through a packet router the router's and his response
/// times are used, otherwise the connection's own.
/// </summary>
void __fastcall Calc_Frames_Behind(FrameSyncStruct *their, int index) // RA2: 0x0062E5C0
{
	long frame;
	unsigned int delay;
	unsigned int half_latency;
	unsigned int half_response;
	int frame_delay;
	int behind;
	int now;

	if (Session.Type != GAME_INTERNET) { // RA2: as above
		Session.PlayerLatency[index] = 0; // RA2: as above
		return; // RA2: as above
	}
	frame = their[index].frame; // RA2: as above
	if (Session.ViaPacketRouter) { // RA2: as above
		half_latency = Session.RouterResponse / 2; // RA2: as above
		half_response = (unsigned int)their[index].response / 2; // RA2: as above
		double ratio = (double)Session.DesiredFrameRate / Session.FrameSendRate; // RA2: as above
		float rate = (float)ratio; // RA2: as above
		int frames = (int)(-60.0f / rate); // RA2: as above
		frame_delay = frames / 2; // RA2: as above
		delay = half_response - frame_delay + half_latency; // RA2: as above
	} else {
		delay = Ipx.Avg_Response_Time(index) / 2; // RA2: as above
	}
	now = Frame; // RA2: as above
	unsigned int scaled = delay * FPSCounter::CurrentFrameRate; // RA2: as above
	unsigned int ticks = scaled / TIMER_SECOND; // RA2: as above
	behind = now - ticks - frame; // RA2: as above
	Session.PlayerLatency[index] = behind; // RA2: as above
}


/// <summary>
/// Sends an event to the other players: the frame it belongs to is stamped on it here.
/// </summary>
/// <param name="event">The event, taken by value so the frame can be changed.</param>
/// <returns>Returns false when the OutList is full.</returns>
bool __fastcall Add_Outgoing_Event(EventClass event) // RA2: 0x0062E670
{
	event.Frame = Frame;
	if (OutList.Add(event)) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Puts an event at the head of the DoList so that it runs on the next frame.
/// If the DoList is full the game is stopped with the message for a full queue.
/// </summary>
/// <param name="event">The event to run.</param>
void __fastcall Enqueue_Event(EventClass * event) // RA2: 0x0062E6E0
{
	long frame = Frame + 1;
	if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		frame = ((Frame + 1 + (Session.FrameSendRate - 1)) / Session.FrameSendRate) * Session.FrameSendRate;
	}
	event->Frame = frame;
	if (DoList.Count < 0x4000) {
		if (DoList.Head == 0) {
			DoList.Head = 0x3FFF;
		} else {
			DoList.Head--;
		}
		DoList.Array[DoList.Head] = *event;
		DoList.Timings[DoList.Head] = timeGetTime();
		((int &)DoList.Count)++;
		return;
	}
	if (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) {
		if (Session.Type == GAME_INTERNET) {
			Register_Game_End_Time();
		}
		Session.Suspended++;
		WWMessageBox().Process(WAIT_TEXT("TXT_QUEUE_FULL", 8826), WAIT_TEXT("TXT_OK", 8826), NULL, NULL, false);
		Session.Suspended--;
		Stop_Game(false);
	}
}


/// <summary>
/// Tells the other players of an internet game that this port cannot be reached.
/// A NET_PORT_UNREACHABLE packet goes to every player the packet router knows by name, except
/// the local one; the call then waits up to five seconds for the packets to be sent.
/// </summary>
void __fastcall Send_Port_Unreachable(void) // RA2: 0x0062E840
{
	GlobalPacketType packet;
	int start;
	int i;
	char *name;

	DebugString("Sending NET_PORT_UNREACHABLE packets\n");
	if (Session.Type != GAME_INTERNET) {
		return;
	}
	Queue_Ipx_Clear();
	start = Ipx.Global_Num_Send();
	for (i = 0; i < OwnerTalk.Connections.Count(); i++) {
		memset(&packet, 0, 0x2F);
		packet.Command = NET_PORT_UNREACHABLE;
		strcpy(packet.Name, Wide_To_Narrow(Session.Players[0]->Name));
		name = OwnerTalk.Name_From_Index(i);
		if (name && stricmp(name, Wide_To_Narrow(Session.Players[0]->Name))) {
			DebugString("Sending NET_PORT_UNREACHABLE to %s\n", name);
			IPXAddressClass address = *OwnerTalk.Address_From_Name(name);
			Ipx.Send_Global_Message(&packet, 0x2F, 1, &address, OwnerTalk.Port_From_Name(name), 0);
		}
	}
	CDTimerClass<SystemTimerClass> timer(300);
	while (Ipx.Global_Num_Send() > start && timer) {
		Call_Back();
	}
	Queue_Ipx_Set();
}









/***************************************************************************
 * Add_Uncompressed_Events -- adds uncompressed events to a packet         *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf            buffer to store packet in                           *
 *      bufsize         max size of buffer                                 *
 *      frame_delay      desired frame delay to attach to all outgoing packets*
 *      size            current packet size                                *
 *      cap            max # events to process                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      new size value                                                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      This routine MUST check to be sure it doesn't overflow the buffer. *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay, int size, int cap) // RA2: as above
{
	int num = 0;
	int ev_size; // RA2: as above

	while (OutList.Count && (num < cap)) {
		if (OutList.First().Type == EventClass::ADDPLAYER) { // RA2: as above
			ev_size = sizeof(EventClass) + OutList.First().Data.Variable.Size;
		}
		else {
			ev_size = sizeof(EventClass);
		}
		if ((size + ev_size) > bufsize) { // RA2: as above
			return(size);
		}

		OutList.First().Frame = Frame + frame_delay;
		OutList.First().ID = PlayerPtr->HeapID;
		OutList.First().IsExecuted = 0;
		if (!DoList.Add(OutList.First())) {
			return(size);
		}

		if (OutList.First().Type == EventClass::ADDPLAYER) { // RA2: as above
			memcpy(((char *)buf) + size, &OutList.First(), sizeof(EventClass)); // RA2: as above
			size += sizeof(EventClass);
			memcpy(((char *)buf) + size, // RA2: as above
				OutList.First().Data.Variable.Pointer,
				OutList.First().Data.Variable.Size);
			size += OutList.First().Data.Variable.Size;
		}
		else {
			memcpy(((char *)buf) + size, &OutList.First(), sizeof(EventClass)); // RA2: as above
			size += sizeof(EventClass);
		}

		num++;
		OutList.Next();
	}

	return(size);
}


/***************************************************************************
 * Build_Send_Packet -- Builds a big packet from a bunch of little ones.   *
 *                                                                         *
 * This routine takes events from the OutList, and puts them into a        *
 * "meta-packet", which is transmitted to all systems we're connected to.  *
 * Also, these events are added to our own DoList.                         *
 *                                                                         *
 * Every Meta-Packet we send uses a FRAMEINFO packet as a header; this     *
 * tells the other systems what frame we're on, as well as serving as a    *
 * standard packet header.                                                 *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf            buffer to store packet in                           *
 *      bufsize         max size of buffer                                 *
 *      frame_delay      desired frame delay to attach to all outgoing packets*
 *      num_cmds         value to use for the CommandCount field           *
 *      cap            max # events to send                                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      new size of packet                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 * 'num_cmds' should be the total of of commands, including all those sent *
 * this frame!                                                             *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Build_Send_Packet(void *buf, int bufsize, int frame_delay, int num_cmds, int cap, int & processed) // RA2: as above
{
	int size = 0;
	EventClass *finfo;

	finfo = (EventClass *)buf;
	finfo->Type = EventClass::FRAMEINFO;
	processed = 0;
	if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		finfo->Frame = ((Frame + frame_delay + (Session.FrameSendRate - 1)) /
			Session.FrameSendRate) * Session.FrameSendRate;
	}
	else {
		finfo->Frame = Frame + frame_delay;
	}
	finfo->ID = PlayerPtr->HeapID;
	finfo->Data.FrameInfo.CRC = GameCRC;
	finfo->Data.FrameInfo.CommandCount = num_cmds;
	finfo->Data.FrameInfo.Delay = frame_delay;

	if (Session.CommProtocol == COMM_PROTOCOL_SINGLE_NO_COMP) { // RA2: as above
		size += sizeof(EventClass);
	}
	else {
		size += (offsetof(EventClass, Data) + sizeof(finfo->Data.FrameInfo)); // RA2: as above
	}

	switch (Session.CommProtocol) {
		case (COMM_PROTOCOL_SINGLE_NO_COMP):
			size = Add_Uncompressed_Events(buf, bufsize, frame_delay, size, cap);
			break;

		case (COMM_PROTOCOL_SINGLE_E_COMP):
		case (COMM_PROTOCOL_MULTI_E_COMP):
			size = Add_Compressed_Events(buf, bufsize, frame_delay, size, cap, processed);
			break;

		default:
			size = 0;
			break;
	}

	return(size);
}


/***************************************************************************
 * Send_Packets -- sends out events from the OutList                       *
 *                                                                         *
 * This routine computes how many events can be sent this frame, and then  *
 * builds the "meta-packet" & sends it.                                    *
 *                                                                         *
 * The 'cap' value is the max # of events we can send.  Ideally, it should *
 * be based upon the bandwidth of our connection.  Currently, it's just    *
 * hardcoded to prevent the modem from having to resend "too much" data,   *
 * which is about 200 bytes per frame.                                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      net                  ptr to connection manager                     *
 *      multi_packet_buf      buffer to store packets in                   *
 *      multi_packet_max      max size of multi_packet_buf                 *
 *      max_ahead            current game MaxAhead value                   *
 *      my_sent               # commands I've sent this game               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # events sent, NOT including the FRAMEINFO event                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/



/***************************************************************************
 * Send_FrameSync -- Sends a FRAMESYNC packet                              *
 *                                                                         *
 * This routine is used to periodically remind the other systems that      *
 * we're still here, and to tell them what frame # we're on, in case       *
 * they've missed my FRAMEINFO packets.                                    *
 *                                                                         *
 * INPUT:                                                                  *
 *      net            ptr to connection manager                           *
 *      cmd_count      # commands I've sent so far                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/



/***************************************************************************
 * Process_Serial_Packet -- Handles an incoming serial packet              *
 *                                                                         *
 * This routine is needed because the modem classes don't support a        *
 * "global channel" like the network classes do, but that functionality is *
 * still needed for modem communications.  Specifically, the modem dialogs *
 * transmit their own special packets back & forth, and messages are sent  *
 * using a special packet type.  Thus, we have to call this routine when   *
 * we receive a modem packet, to allow it to process messages & dialog     *
 * packets.                                                                *
 *                                                                         *
 * INPUT:                                                                  *
 *      multi_packet_buf      packet buffer to process                     *
 *      first_time            1 = this is the 1st game frame               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      RC_NORMAL:            this wasn't a SERIAL-type packet             *
 *      RC_SERIAL_PROCESSED:   this was a SERIAL-type packet, and was      *
 *                         processed; the other player is still connected, *
 *                         even if he's not in the game.                   *
 *      RC_PLAYER_LEFT:      other player has left the game                *
 *      RC_HUNG_UP:            we're getting our own packets back; thus, the*
 *                         modem is mirroring our packets, which means the *
 *                         modem hung up!                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/



void Cast_Kick_Vote(int kicker, int kickee);



















/***************************************************************************
 * Handle_Timeout -- handles a timeout in the wait-for-players loop        *
 *                                                                         *
 * This routine "gracefully" handles a timeout in the frame-sync loop.     *
 * The timeout must be handled differently by a modem game or network      *
 * game.                                                                   *
 *                                                                         *
 * The modem game must detect if the other player is still connected       *
 * physically, even if he's not playing the game any more; if so, this     *
 * routine returns an OK status.  If the other player isn't even           *
 * physically connected, an error is returned.                             *
 *                                                                         *
 * The network game must find the connection that's causing the timeout,   *
 * and destroy it.  The game continues, even if there are no more human    *
 * players left.                                                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      net               ptr to connection manager                        *
 *      their_frame         array containing frame #'s of other players    *
 *      their_sent         array containing command count of other players *
 *      their_recv         array containing # recv'd cmds from other players*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = it's OK; reset timeout timers & keep processing                *
 *      0 = game over, man                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/












/***************************************************************************
 * Clean_DoList -- Cleans out old events from the DoList                   *
 *                                                                         *
 * Currently, an event can only be removed from the DoList if it's at the  *
 * head of the list; and event can't be removed from the middle.  So,      *
 * this routine loops as long as the next event in the DoList has been     *
 * executed, it's removed.                                                 *
 *                                                                         *
 * INPUT:                                                                  *
 *      net      ptr to connection manager; ignored if NULL                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/



/***************************************************************************
 * Queue_Record -- Records the DoList to disk                              *
 *                                                                         *
 * This routine just saves any events in the DoList to disk; we can later  *
 * "play back" the recording just be pulling events from disk rather than  *
 * from the network!                                                       *
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
 *   08/14/1995 BRR : Created.                                             *
 *=========================================================================*/



/***************************************************************************
 * Compute_Game_CRC -- Computes a CRC value of the entire game.            *
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
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/





/***************************************************************************
 * Print_CRCs -- Prints a data file for finding Sync Bugs                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      ev -- event to display                                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none                                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/09/1995 BRR : Created.                                             *
 *=========================================================================*/

















/***************************************************************************
 * Stop_Game -- stops the game                                             *
 *                                                                         *
 * This routine clears any global flags that need it, in preparation for   *
 * halting the game prematurely.                                           *
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
 *   11/22/1995 BRR : Created.                                             *
 *=========================================================================*/
static inline void Stop_Game(bool is_cancelled) // RA2: as above
{
	Session.LoadGame = 0; // RA2: as above
	Session.EmergencySave = 0; // RA2: as above
	GameActive = false; // RA2: as above
	if (IsMono) {
		MonoClass::Disable();
	}
	if (Session.Type == GAME_INTERNET) {
		ConnectionLost = true;
		if (!is_cancelled) Session.SawGameCompletion = true;
		Session.OutOfSync = true;
		Register_Game_End_Time();
		Send_Statistics_Packet(false); // RA2: as above
	}
}











/***************************************************************************
 * Breakup_Receive_Packet -- Splits a big packet into little ones.         *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to break up                                     *
 *      bufsize      length of buffer                                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # events added to queue, -1 if fatal error (queue is full)         *
 *    (return value includes any FRAMEINFO packets encountered;            *
 *      FRAMESYNC's are ignored)                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Breakup_Receive_Packet(void * buf, int bufsize) // RA2: as above
{
	int count = 0;
	switch (Session.CommProtocol) {
	case COMM_PROTOCOL_SINGLE_NO_COMP: // RA2: as above
		count = Extract_Uncompressed_Events(buf, bufsize);
		break;
	default:
		count = Extract_Compressed_Events(buf, bufsize);
		break;
	}
	return count; // RA2: as above
}


/***************************************************************************
 * Extract_Uncompressed_Events -- extracts events from a packet            *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer containing events to extract                    *
 *      bufsize      length of 'buf'                                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # events extracted                                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 DRD : Created.                                             *
 *=========================================================================*/
static int Extract_Uncompressed_Events(void * buf, int bufsize) // RA2: as above
{
	int count = 0;
	int pos = 0;
	int leftover = bufsize;
	EventClass * event; // RA2: as above
	while (leftover >= sizeof(EventClass)) { // RA2: as above
		event = (EventClass *)(((char *)buf) + pos);
		if (event->Type != EventClass::FRAMESYNC) {
			event->IsExecuted = 0;
			if (event->Type == EventClass::ADDPLAYER) {
				event->Data.Variable.Pointer = new char[event->Data.Variable.Size];
				memcpy(event->Data.Variable.Pointer, ((char *)buf) + sizeof(EventClass), // RA2: as above
					   event->Data.Variable.Size);
				pos += event->Data.Variable.Size;
				leftover -= event->Data.Variable.Size;
			}
			if (!DoList.Add(*event)) { // RA2: as above
				if (event->Type == EventClass::ADDPLAYER) {
					delete[] event->Data.Variable.Pointer; // RA2: as above
				}
				return -1; // RA2: as above
			}
			count++;
		}
		pos += sizeof(EventClass);
		leftover -= sizeof(EventClass);
	}
	return count; // RA2: as above
}





/// The name comes from the TS demo build, which ships this routine with symbols.

/// <summary>
/// Closes the reconnect dialog if it is showing.
/// This routine is called once the stalled players have caught up. The tactical map is
/// switched back on and forced to redraw, since the dialog was sitting on top of it.
/// </summary>
static inline void Close_Reconnect_Dialog() // RA2: as above
{
	HWND dialog = WS_Find_Dialog(0xEA); // RA2: as above
	if (dialog) {
		if (ReconnectMouseCaptured) { // RA2: as above
			MouseCursor->Capture_Mouse(); // RA2: as above
		}
		WS_Destroy_Dialog(dialog, 0); // RA2: as above
		TacticalActive = ReconnectSavedTactical; // RA2: as above
		Map.Flag_To_Redraw(GS_REDRAW_ALL);
		if (TacticalActive) { // RA2: as above
			Map.Render();
		}
	}
}


/***************************************************************************
 * Process_Reconnect_Dialog -- processes the reconnection dialog           *
 *                                                                         *
 * This routine [re]draws the reconnection dialog; if 'reconn' is set,     *
 * it tells the user who we're trying to reconnect to; otherwise, is just  *
 * says something generic like "Waiting for connections".                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      timeout_timer   ptr to count down timer, showing time remaining    *
 *      their_frame      array of other players' frame #'s                 *
 *      num_conn         # connections in 'their_frame'                    *
 *      reconn         1 = reconnect, 0 = waiting for first-time connection*
 *      fresh            1 = draw from scratch, 0 = only update time counter*
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = user wants to cancel, 0 = not                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static inline int Process_Reconnect_Dialog(ConnManClass *net, CDTimerClass<SystemTimerClass> *timeout_timer, FrameSyncStruct *their, // RA2: as above
	int num_conn, int reconn, int fresh, BasicTimerClass<SystemTimerClass> *timer) // RA2: as above
{
	int new_time;
	int oldest_index; // RA2: as above
	int i;
	wchar_t buf[512]; // RA2: as above

	ReconnectElapsed = timer->Value(); // RA2: as above
	for (i = 0; i < num_conn; i++) {
		SyncBarFrameSync[i] = their[i];
	}
	if (fresh) {
		ReconnectSavedTactical = TacticalActive; // RA2: as above
		TacticalActive = false;
		ReconnectResult = -1; // RA2: as above
		ReconnectDialog = (HWND)WS_Create_Dialog(ProgramInstance, 0xEA, MainWindow, // RA2: as above
			Reconnect_Dialog_Proc, 0); // RA2: as above
		if (ReconnectDialog) { // RA2: as above
			SetWindowLongA(ReconnectDialog, 8, (long)&ReconnectResult); // RA2: as above
			Show_Dialog(ReconnectDialog, 0xEA); // RA2: as above
			ShowWindow(ReconnectDialog, SW_SHOWNORMAL); // RA2: as above
			SendMessageA(ReconnectDialog, 0x4A9, 0, 1); // RA2: as above
			ReconnectMouseCaptured = MouseCursor->Is_Captured(); // RA2: as above
			if (ReconnectMouseCaptured) { // RA2: as above
				MouseCursor->Release_Mouse(); // RA2: as above
			}
			MouseCursor->Show_Mouse();
		}
	}
	if (ReconnectResult == 2) { // RA2: as above
		return 1; // RA2: as above
	}
	new_time = timeout_timer->Value() / 60; // RA2: as above
	if (fresh || new_time != ReconnectSeconds) { // RA2: as above
		ReconnectSeconds = new_time; // RA2: as above
		HWND item = GetDlgItem(ReconnectDialog, 0x650); // RA2: as above
		if (item) {
			swprintf(buf, WAIT_TEXT("TXT_TIME_ALLOWED", 3856), ReconnectSeconds); // RA2: as above
			SendMessageA(item, 0x4B2, 0, (LPARAM)buf); // RA2: as above
		}
		if (!(ReconnectSeconds & 1)) { // RA2: as above
			PostMessageA(ReconnectDialog, WM_PAINT, 0, 0); // RA2: as above
		}
		if (fresh) {
			while (Session.KickProposals.Count()) {
				delete Session.KickProposals[0];
				Session.KickProposals.Delete_Index(0);
			}
			memset(Session.KickVoteCount, 0, sizeof(Session.KickVoteCount));
			memset(Session.KickVoteWho, 0xFF, sizeof(Session.KickVoteWho));
			HWND list = GetDlgItem(ReconnectDialog, 0x651); // RA2: as above
			if (list) { // RA2: as above
				if (reconn) {
					int oldest = 0x7FFFFFFF; // RA2: as above
					oldest_index = 0;
					for (i = 0; i < num_conn; i++) {
						if (their[i].frame < oldest) { // RA2: as above
							oldest = their[i].frame; // RA2: as above
							oldest_index = i;
						}
					}
					if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
						swprintf(buf, WAIT_TEXT("TXT_RECONNECTING_TO", 3911), // RA2: as above
							Ipx.Connection_Name(Ipx.Connection_ID(oldest_index))); // RA2: as above
					} else {
						swprintf(buf, WAIT_TEXT("TXT_RECONNECTING_TO", 3917), Session.Players[1]->Name); // RA2: as above
					}
					_DrawMessage(-1, buf, list); // RA2: as above
					_DrawMessage(-1, L"", list); // RA2: as above
					if (Session.Type == GAME_INTERNET) {
						wcscpy(buf, WAIT_TEXT("TXT_RECONNECT_HELP3", 3931)); // RA2: as above
						wcscat(buf, L" "); // RA2: as above
						wcscat(buf, WAIT_TEXT("TXT_RECONNECT_HELP3B", 3933)); // RA2: as above
						wcscat(buf, L" "); // RA2: as above
						wcscat(buf, WAIT_TEXT("TXT_RECONNECT_HELP3C", 3935)); // RA2: as above
						_DrawMessage(-1, buf, list); // RA2: as above
						_DrawMessage(-1, L"", list); // RA2: as above
					}
					if (Session.Type == GAME_INTERNET || Session.Type == GAME_IPX) { // RA2: as above
						wcscpy(buf, WAIT_TEXT("TXT_RECONNECT_HELP2", 3942)); // RA2: as above
						wcscat(buf, L" "); // RA2: as above
						if (Session.Type == GAME_INTERNET) {
							wcscat(buf, WAIT_TEXT("TXT_RECONNECT_HELP2B", 3946)); // RA2: as above
						} else if (Session.Type == GAME_IPX) {
							wcscat(buf, WAIT_TEXT("TXT_RECONNECT_HELP4", 3948)); // RA2: as above
						}
						_DrawMessage(-1, buf, list); // RA2: as above
						_DrawMessage(-1, L"", list); // RA2: as above
					}
					_DrawMessage(-1, L"", list); // RA2: as above
					_DrawMessage(-1, WAIT_TEXT("TXT_RECONNECT_HELP5", 3958), list); // RA2: as above
					_DrawMessage(-1, WAIT_TEXT("TXT_RECONNECT_HELP1", 3963), list); // RA2: as above
					_DrawMessage(-1, L"", list); // RA2: as above
				} else {
					swprintf(buf, WAIT_TEXT("TXT_WAITING_FOR_CONNECTIONS", 3966)); // RA2: as above
					_DrawMessage(-1, buf, list); // RA2: as above
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
			DebugString("Kicking player %s from the game due to %d votes\n", Session.Players[i]->Name, votes);
			if (i == 0) {
				DebugString("Outvoted player is me!\n");
				for (i = 1; i < Session.Players.Count(); i++) { // RA2: as above
					Kick_Player_Now(net, net->Connection_Index(Session.Players[i]->Player.ID), their, 0); // RA2: as above
				}
				break;
			}
			Kick_Player_Now(net, net->Connection_Index(Session.Players[i]->Player.ID), their, 1); // RA2: as above
		}
	}
	return 0; // RA2: as above
}


/***************************************************************************
 * Can_Advance -- determines if it's OK to advance to the next frame       *
 *                                                                         *
 * This routine uses the current values stored in their_frame[],           *
 * their_send[], and their_recv[] to see if it's OK to advance to the next *
 * game frame.  We must not advance if:                                    *
 * - If our frame # would be too far ahead of the slowest player (the      *
 *   lowest their_frame[] value).  "Too far" means                         *
 *   (Frame >= their_frame + MaxAhead).                                    *
 * - our current command count doesn't match the sent command count of one *
 *   other player (meaning that we've missed a command packet from that    *
 *   player, and thus the frame # we're receiving from him may be due to a *
 *   FRAMEINFO packet sent later than the command, so we shouldn't use     *
 *   this frame # to see if we should advance; we should wait until we     *
 *   have all the commands before we advance.                              *
 *                                                                         *
 * Of course, this routine assumes the values in their_frame[] etc are     *
 * kept current by the caller.                                             *
 *                                                                         *
 * INPUT:                                                                  *
 *      net            ptr to connection manager                           *
 *      max_ahead      max frames ahead                                    *
 *      their_frame      array of their frame #'s                          *
 *      their_sent      array of their sent command count                  *
 *      their_recv      array of their # received commands                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK to advance; 0 = not OK                                      *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static inline int Can_Advance(ConnManClass *net, int max_ahead, FrameSyncStruct *their, int *frame_stall, int *count_stall) // RA2: as above
{
	int their_oldest_frame; // RA2: as above
	int count_ok; // RA2: as above
	int i;

	*frame_stall = -1;
	if (Session.NumPlayers == 1) {
		return 1; // RA2: as above
	}
	their_oldest_frame = Frame + 1000;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their[i].frame < their_oldest_frame) {
			their_oldest_frame = their[i].frame;
			*frame_stall = i;
		}
	}
	count_ok = 1;
	for (i = 0; i < net->Num_Connections(); i++) {
		if (their[i].recv < their[i].sent) {
			count_ok = 0;
			break;
		}
	}
	if (count_ok) {
		if (Frame < their_oldest_frame + max_ahead) { // RA2: as above
			return 1; // RA2: as above
		}
		i = -1;
	}
	if (Frame < their_oldest_frame + max_ahead) { // RA2: as above
		*frame_stall = -1;
	}
	*count_stall = i;
	return 0; // RA2: as above
}


/***************************************************************************
 * Wait_For_Players -- Waits for other systems to come on-line             *
 *                                                                         *
 * This routine performs the most critical logic in multiplayer; that of   *
 * synchronizing my frame number with those of the other systems.          *
 *                                                                         *
 * INPUT:                                                                  *
 *      first_time            1 = 1st time this routine is called          *
 *      net                  ptr to connection manager                     *
 *      resend_delta         time (ticks) between FRAMESYNC resends        *
 *      dialog_time            time (ticks) until pop up a reconnect dialog*
 *      timeout               time (ticks) until we give up the ghost      *
 *      multi_packet_buf      buffer to store packets in                   *
 *      my_sent               # commands I've sent so far                  *
 *      their_frame            array of their frame #'s                    *
 *      their_sent            array of their CommandCount values           *
 *      their_recv            array of # cmds I've received from them      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      RC_NORMAL            OK to advance to the next frame               *
 *      RC_CANCEL            user hit 'Cancel' at the timeout countdown dlg*
 *      RC_NOT_RESPONDING      other player(s) not responding              *
 *      RC_SCENARIO_MISMATCH   scenario's don't match (first_time only)    *
 *      RC_DOLIST_FULL         DoList was full                             *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/



/***************************************************************************
 * Queue_AI_Normal -- Process all queued events.                           *
 *                                                                         *
 * This is the "normal" version of the queue management routine.  It does  *
 * the following:                                                          *
 * - Transfers items in the OutList to the DoList                          *
 * - Executes any commands in the DoList that are supposed to be done on   *
 *   this frame #                                                          *
 * - Cleans out the DoList                                                 *
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
 *   09/21/1995 JLB : Created.                                             *
 *=========================================================================*/
static void Queue_AI_Normal(void)
{
	//------------------------------------------------------------------------
	// Move events from the OutList (events generated by this player) into the
	// DoList (the list of events to execute).
	//------------------------------------------------------------------------
	while (OutList.Count) {
		OutList.First().IsExecuted = false;
		DoList.Add(OutList.First());
		#ifdef MIRROR_QUEUE
		MirrorList.Add(OutList.First());
		#endif
		OutList.Next();
	}

	//------------------------------------------------------------------------
	// Save the DoList to disk, if we're in "Record" mode
	//------------------------------------------------------------------------
	if (Session.Record) {
		Queue_Record();
	}

	//------------------------------------------------------------------------
	// Execute the DoList; if an error occurs, bail out.
	//------------------------------------------------------------------------
	if (!Execute_DoList(1, PlayerPtr->Class->House, NULL, NULL, NULL)) {
		GameActive = 0;
		return;
	}

	//------------------------------------------------------------------------
	// Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(NULL);

}	/* end of Queue_AI_Normal */


/***************************************************************************
 * Generate_Timing_Event -- computes & queues a RESPONSE_TIME event        *
 *                                                                         *
 * This routine adjusts the connection timing on the local system; it also *
 * optionally generates a RESPONSE_TIME event, to tell all systems to      *
 * dynamically adjust the current MaxAhead value.  This allows both the    *
 * MaxAhead & the connection retry logic to have dynamic timing, to adjust *
 * to varying line conditions.                                             *
 *                                                                         *
 * INPUT:                                                                  *
 *      net         ptr to connection manager                              *
 *      my_sent      # commands I've sent out so far                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Timing_Event(ConnManClass *net, int my_sent)
{
	unsigned int resp_time;			// connection response time, in ticks
	EventClass ev;

	//------------------------------------------------------------------------
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, divide again by 4, assuming a game rate of 15 fps.
	//------------------------------------------------------------------------
	resp_time = net->Response_Time();

	//------------------------------------------------------------------------
	//	Adjust my connection retry timing; only do this if I've sent out more
	// than 5 commands, so I know I have a measure of the response time.
	//------------------------------------------------------------------------
	if (my_sent > 5) {

		net->Set_Timing (resp_time + TIMER_SECOND / 6, -1, (resp_time * 4) + TIMER_SECOND / 4);

		//.....................................................................
		// If I'm the network "master", I'm also responsible for updating the
		// MaxAhead value on all systems, so do that here too.
		//.....................................................................
		if (Session.Am_I_Master()) {
			ev.Type = EventClass::RESPONSE_TIME;
			//..................................................................
			// For multi-frame compressed events, the MaxAhead must be an even
			// multiple of the FrameSendRate.
			//..................................................................
			if (Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
				ev.Data.FrameInfo.Delay = MAX( ((((resp_time / 8) +
					(Session.FrameSendRate - 1)) / Session.FrameSendRate) *
					Session.FrameSendRate), (Session.FrameSendRate * 2) );
			}
			//..................................................................
			// For sending packets every frame, just use the 1-way connection
			// response time.
			//..................................................................
			else {
				if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
					ev.Data.FrameInfo.Delay = MAX( (resp_time / 8),
						 MODEM_MIN_MAX_AHEAD );
				} else if (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) {
					ev.Data.FrameInfo.Delay = MAX( (resp_time / 8),
						 NETWORK_MIN_MAX_AHEAD );
				}
			}
			OutList.Add(ev);
		}
	}

}	// end of Generate_Timing_Event


/***************************************************************************
 * Generate_Real_Timing_Event -- Generates a TIMING event                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      net         ptr to connection manager                              *
 *      my_sent      # commands I've sent out so far                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1996 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Real_Timing_Event(ConnManClass *net, int my_sent)
{
	EventClass ev;
	int highest_ticks;
	int i;
	int specified_frame_rate;
	int maxahead;
	unsigned char frame_send_rate;

	DebugString("Generate_Real_Timing_Event\n"); // RA2: logged on entry
	if (Session.PrecalcMaxAhead != 0 || Session.PrecalcDesiredFrameRate != 0) {
		DebugString("Sending precalculated network timings on frame %d\n"/*, Frame*/);

		ev.Type = EventClass::TIMING;
		ev.Data.Timing.DesiredFrameRate = Session.PrecalcDesiredFrameRate;
		ev.Data.Timing.MaxAhead = Session.PrecalcMaxAhead;
		ev.Data.Timing.FrameSendRate = Session.PrecalcDesiredFrameRate > 30u ? 10 : 5;
		ev.Frame = Frame; // RA2: the event is stamped by hand, not by a constructor
		ev.ID = PlayerPtr->HeapID; // RA2: as above

		OutList.Add(ev);

		Session.PrecalcMaxAhead = 0;
		Session.PrecalcDesiredFrameRate = 0;

		return;
	}


	//
	// If we haven't sent out at least 5 guaranteed-delivery packets, don't
	// bother trying to measure our connection response time; just return.
	//
	if (my_sent < 5) {
		return;
	}

	//
	// Find the highest processing time we have stored
	//
	highest_ticks = 0;
	for (i = 0; i < Session.Players.Count(); i++) {

		//
		// If we haven't heard from all systems yet, bail out.
		//
		if (Session.Players[i]->Player.ProcessTime == -1) {
			return;
		}
		if (Session.Players[i]->Player.ProcessTime > highest_ticks) {
			highest_ticks = Session.Players[i]->Player.ProcessTime;
		}
	}

	//
	// Compute our "desired" frame rate as the lower of:
	// - What the user has dialed into the options screen
	// - What we're really able to run at
	//
	int desired; // RA2: a local, the session's DesiredFrameRate is left alone
	if (highest_ticks == 0) {
		desired = 60;
	} else {
		desired = MAX(1, 1000 / highest_ticks);
	}

	switch (Options.GameSpeed) {
		case 0:
			specified_frame_rate = 60;
			break;
		case 1:
			specified_frame_rate = 45;
			break;
		default:
			specified_frame_rate = 60 / Options.GameSpeed;
			break;
	}

	desired = MIN (desired, specified_frame_rate);

	//
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, ....uh....
	//
	unsigned long resp_time;
	if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) { // RA2: through the packet router the slowest peer's response is added
		resp_time = ((IPXManagerClass *)net)->Multicast_Response_Time(); // RA2: as above
		unsigned long worst_response = 0; // RA2: as above
		for (i = 0; i < net->Num_Connections(); i++) { // RA2: as above
			worst_response = MAX(worst_response, (unsigned long)TheirFrameSync[i].response); // RA2: as above
		}
		resp_time += worst_response; // RA2: as above
	} else {
		resp_time = net->Response_Time();
	}
	int connection_response = net->Response_Time(); // RA2: the connection's own time, for its retry timing
	frame_send_rate = Session.FrameSendRate;
	if (Session.Type == GAME_INTERNET) {
		frame_send_rate = desired > 30 ? 10 : 5;
	}

	int fudge = 0;
	if (resp_time != 0) {
		switch (Session.LatencyFudge) {
			case 0:
				DebugString("Response time = %d\n", resp_time);
				break;
			case 1:
				resp_time += resp_time >> 1;
				connection_response += connection_response / 2; // RA2: as above
				fudge = 10;
				DebugString("Response time = %d\n", resp_time);
				break;
			case 2:
				resp_time *= 2;
				connection_response *= 2; // RA2: as above
				fudge = 20;
				DebugString("Response time = %d\n", resp_time);
				break;
			case 3:
				resp_time *= 3;
				connection_response *= 3; // RA2: as above
				fudge = 30;
				DebugString("Response time = %d\n", resp_time);
				break;
		}
	}

	//
	// Compute our new 'MaxAhead' value, based upon the response time of our
	// connection and our desired frame rate.
	// 'MaxAhead' in frames is:
	//
	// (resp_time / 2 ticks) * (1 sec/60 ticks) * (n Frames / sec)
	//
	// resp_time is divided by 2 because, as reported, it represents a round-
	// trip, and we only want to use a one-way trip.
	//
	maxahead = (resp_time * desired) / (2 * TIMER_SECOND) + fudge; // RA2: the fudge is added first and the send rate only when MaxAhead grows
	if (maxahead > (int)Session.MaxAhead) { // RA2: as above
		maxahead = MIN(maxahead, (int)Session.MaxAhead + frame_send_rate); // RA2: as above
	}

	//
	// Now, we have to round 'maxahead' so it's an even multiple of our
	// send rate.  It also must be at least thrice the FrameSendRate.
	// (Isn't "thrice" a cool word?)
	//
	maxahead = ((maxahead - 1 + frame_send_rate) / frame_send_rate) * frame_send_rate; // RA2: as above
	maxahead = MAX (maxahead, (int)frame_send_rate * 3);
	maxahead = MIN (maxahead, frame_send_rate * ((frame_send_rate + 249) / frame_send_rate));

	ev.Type = EventClass::TIMING;
	ev.Data.Timing.DesiredFrameRate = desired;
	ev.Frame = Frame; // RA2: as above
	ev.ID = PlayerPtr->HeapID; // RA2: as above
	ev.Data.Timing.MaxAhead = maxahead + (Scen->Special.IsFogOfWar ? 10 : 0);
	ev.Data.Timing.FrameSendRate = frame_send_rate;

	OutList.Add(ev);

	//
	// Adjust my connection retry timing.  These values set the retry timeout
	// to just over one round-trip time, the 'maxretries' to -1, and the
	// connection timeout to allow for about 4 retries.
	//
	int timeout = connection_response * 8 + TIMER_SECOND / 4; // RA2: from the connection's own response time
	if (timeout < 2 * TIMER_SECOND) { // RA2: as above
		timeout = 2 * TIMER_SECOND; // RA2: as above
	}
	net->Set_Timing (connection_response + TIMER_SECOND / 6, -1, timeout, false); // RA2: as above
}



/***************************************************************************
 * Generate_Process_Time_Event -- Generates a PROCESS_TIME event           *
 *                                                                         *
 * INPUT:                                                                  *
 *      net         ptr to connection manager                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1996 BRR : Created.                                             *
 *=========================================================================*/
static void Generate_Process_Time_Event(ConnManClass *net)
{
	EventClass ev;
	int avgticks;
	unsigned int resp_time;			// connection response time, in ticks

	//
	// Measure the current connection response time.  This time will be in
	// 60ths of a second, and represents full round-trip time of a packet.
	// To convert to one-way packet time, divide by 2; to convert to game
	// frames, ....uh....
	//
	resp_time = net->Response_Time();

	//
	// Adjust my connection retry timing.  These values set the retry timeout
	// to just over one round-trip time, the 'maxretries' to -1, and the
	// connection timeout to allow for about 4 retries.
	//
	switch (Session.LatencyFudge) {
		case 0:
			DebugString("Response time = %d\n", resp_time);
			break;
		case 1:
			resp_time += resp_time >> 1;
			DebugString("Response time = %d\n", resp_time);
			break;
		case 2:
			resp_time *= 2;
			DebugString("Response time = %d\n", resp_time);
			break;
		case 3:
			resp_time *= 3;
			DebugString("Response time = %d\n", resp_time);
			break;
	}
	net->Set_Timing (resp_time + TIMER_SECOND / 6, -1, MAX(2 * TIMER_SECOND, (resp_time * 8) + TIMER_SECOND / 4), false);

	if (IsMono) {
		MonoClass::Enable();
		MonoClass *page = MonoClass::Get_Page(4); // RA2: the mono pages are objects now
		page->Set_Cursor(0,23); // RA2: as above
		page->Printf("Processing Ticks:%03d Frames:%03d\n", Session.ProcessTicks,Session.ProcessFrames); // RA2: as above
		MonoClass::Disable();
	}

	avgticks = Session.ProcessTicks / Session.ProcessFrames;

	ev.Type = EventClass::PROCESS_TIME;
	ev.Data.ProcessTime.AverageTicks = avgticks;
	ev.Frame = Frame; // RA2: stamped by hand
	ev.ID = PlayerPtr->HeapID; // RA2: as above
	OutList.Add(ev);

	Session.ProcessTicks = 0;
	Session.ProcessFrames = 0;

	if (Session.Type == GAME_INTERNET && (Frame & 0x7FF) == 0) { // RA2: every 2048 frames
		net->Reset_Response_Time(false);
	}
}



/***************************************************************************
 * Process_Send_Period -- timing for sending packets every 'n' frames      *
 *                                                                         *
 * This function is for a CommProtocol of COMM_PROTOCOL_MULTI_E_COMP only. *
 * It determines if it's time to send a packet or not.                     *
 *                                                                         *
 * INPUT:                                                                  *
 *      net      ptr to connection manager                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = it's time to send a packet; 0 = don't send a packet this frame.*
 *                                                                         *
 * WARNINGS:                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Process_Send_Period(ConnManClass *net)	//, int init)
{
	//------------------------------------------------------------------------
	// If the current frame # is not an even multiple of 'FrameSendRate', then
	// it's not time to send a packet; just return.
	//------------------------------------------------------------------------
	if (Frame != (int)(((Frame + (Session.FrameSendRate - 1)) /
		Session.FrameSendRate) * Session.FrameSendRate) ) {

		net->Service();

		if (IsMono) {
			MonoClass::Disable();
		}

		return(0);
	}

	return(1);


}	// end of Process_Send_Period














/// <summary>
/// Fetches the connection index for a player.
/// Serial and modem games only ever have the one connection, so they answer with zero
/// rather than troubling the network manager about it.
/// </summary>
/// <param name="player">The player ID to look up.</param>
/// <returns>Returns with the connection index that belongs to the player.</returns>
static int Connection_Index(int player)
{
	if (Session.Type == GAME_MODEM || Session.Type == GAME_NULL_MODEM) {
		return(0);
	}
	return(Ipx.Connection_Index(player));
}














/// <summary>
/// Scales a vector to length one; a vector of length zero stays as it is.
/// </summary>
static inline Vector3 Normalized(Vector3 const & v) // RA2: as above
{
	double length = fastmath::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
	if (length != 0.0) {
		return Vector3(v.X / length, v.Y / length, v.Z / length);
	}
	return v;
}




/***************************************************************************
 * Queue_Playback -- plays back queue entries from a record file           *
 *                                                                         *
 * This routine reads events from disk, putting them into the DoList;      *
 * it then executes the DoList just like the network version does.  The    *
 * result is that the game "plays back" like a recording.                  *
 *                                                                         *
 * This routine detects mouse motion and stops playback, so it can work    *
 * like an "attract" mode, showing a demo of the game itself.              *
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
 *   05/15/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Queue_Playback(void)
{
	int numevents;
	EventClass event;
	int i;
	int ok;
	static int mx,my;
	int max_houses;
	HousesType base_house;
	int key;
	int testframe;

	//------------------------------------------------------------------------
	// If the user hits ESC, stop the playback
	//------------------------------------------------------------------------
	if (Keyboard->Check()) {
		key = Keyboard->Get();
		if (key == KA_ESC || Session.Attract) {
			GameActive = 0;
			return;
		}
	}

	//------------------------------------------------------------------------
	// If we're in "Attract" mode, and the user moves the mouse, stop the
	// playback.
	//------------------------------------------------------------------------
	if (Session.Attract && Frame > 0 &&
		(mx != Get_Mouse_X() || my != Get_Mouse_Y())) {
		GameActive = 0;
		return;
	}
	mx = Get_Mouse_X();
	my = Get_Mouse_Y();

	//------------------------------------------------------------------------
	// Compute the Game's CRC
	//------------------------------------------------------------------------
	Compute_Game_CRC();
	CRC[Frame & (ARRAY_SIZE(CRC) - 1)] = GameCRC;

	//------------------------------------------------------------------------
	// If we've reached the CRC print frame, do so & exit
	//------------------------------------------------------------------------
	if (Frame >= Session.TrapPrintCRC) {
		if (ExtraSyncInfo) { // RA2: the sync log replaces the CRC print
			Write_Sync_File(Frame % 256, NULL); // RA2: as above
		} else {
			Print_CRCs(NULL);
		}
		Emergency_Exit();
		Exit_Process(0); // RA2: exits through Exit_Process
	}

	//------------------------------------------------------------------------
	// Don't read anything the first time through (since the Queue_AI_Network
	//	routine didn't write anything the first time through); do this after the
	// CRC is computed, since we'll still need a CRC for Frame 0.
	//------------------------------------------------------------------------
	if (Frame==0 && Session.Type!=GAME_NORMAL) {
		return;
	}

	//------------------------------------------------------------------------
	// Only process every 'FrameSendRate' frames
	//------------------------------------------------------------------------
	testframe = ((Frame + (Session.FrameSendRate - 1)) /
		Session.FrameSendRate) * Session.FrameSendRate;
	if ( (Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH) &&
		Session.CommProtocol == COMM_PROTOCOL_MULTI_E_COMP) {
		if (Frame != testframe) {
			return;
		}
	}

	//------------------------------------------------------------------------
	// Read the DoList from disk
	//------------------------------------------------------------------------
	ok = 1;
	if (Session.RecordFile.Read (&numevents, sizeof(numevents)) ==
		sizeof(numevents)) {
		for (i = 0; i < numevents; i++) {
			if (Session.RecordFile.Read (&event, sizeof(EventClass)) ==
				sizeof(EventClass)) {
				event.IsExecuted = 0;
				DoList.Add (event);
				#ifdef MIRROR_QUEUE
				MirrorList.Add(event);
				#endif
			}
			else {
				ok = 0;
				break;
			}
		}
	}
	else {
		ok = 0;
	}

	if (!ok) {
		GameActive = 0;
		return;
	}


	//------------------------------------------------------------------------
	// Execute the DoList; if an error occurs, bail out.
	//------------------------------------------------------------------------
	if (Session.Type == GAME_NORMAL) {
		max_houses = 1;
		base_house = PlayerPtr->Class->House;
	}
	else {
		max_houses = Session.MaxPlayers;
		base_house = HOUSE_FIRST;
	}
	if (!Execute_DoList(max_houses, base_house, NULL, NULL, NULL)) {
		GameActive = 0;
		return;
	}

	//------------------------------------------------------------------------
	// Clean out the DoList
	//------------------------------------------------------------------------
	Clean_DoList(NULL);

}	/* end of Queue_Playback */


/***************************************************************************
 * Compute_Game_CRC -- Computes a CRC value of the entire game.            *
 * Init_Queue_Mono -- inits mono display                                   *
 *                                                                         *
 * This routine steals control of the mono screen away from the rest of    *
 * the engine, by setting the global IsMono; if IsMono is set, the other   *
 * routines in this module turn off the Mono display when they're done     *
 * with it, so the rest of the engine won't over-write what we're writing. *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *      net      ptr to connection manager                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/09/1995 BRR : Created.                                             *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Init_Queue_Mono(ConnManClass *net)
{
#ifdef _DEBUG
	//------------------------------------------------------------------------
	// Set 'IsMono' so we can steal the mono screen from the engine
	//------------------------------------------------------------------------
	if ((Frame==0 || Session.LoadGame) && MonoClass::Is_Enabled()) {
		IsMono = true;
	}

	//------------------------------------------------------------------------
	// Enable mono output for our stuff; we must Disable it before we return
	// control to the engine.
	//------------------------------------------------------------------------
	if (IsMono)
		MonoClass::Enable();

	if (net->Num_Connections() > 0) {
		//.....................................................................
		// Network mono debugging screen
		//.....................................................................
		if (NetMonoMode==0) {
			if (Frame==0 || Session.LoadGame || NewMonoMode) {
				net->Configure_Debug (0, sizeof (CommHeaderType),
					size_of(EventClass, Type), (char **)EventClass::EventNames, 0, 27);
				net->Mono_Debug_Print (0,1);
				NewMonoMode = 0;
			}
			else {
				net->Mono_Debug_Print (0,0);
			}
		}
		//.....................................................................
		// Flow control debugging output
		//.....................................................................
		else {
			if (NewMonoMode) {
				Mono_Clear_Screen();
				Mono_Printf("                         Queue AI:\n");	// flowcount[0]
				Mono_Printf("                Build Packet Loop:\n");	// flowcount[1]
				Mono_Printf("                       Frame Sync:\n");	// flowcount[2]
				Mono_Printf("                Frame Sync Resend:\n");	// flowcount[3]
				Mono_Printf("               Frame Sync Timeout:\n");	// flowcount[4]
				Mono_Printf("           Frame Sync New Message:\n");	// flowcount[5]
				Mono_Printf("                 DoList Execution:\n");	// flowcount[6]
				Mono_Printf("                  DoList Cleaning:\n");	// flowcount[7]
				Mono_Printf("\n");
				Mono_Printf("                            Frame:\n");
				Mono_Printf("                 Session.MaxAhead:\n");
				Mono_Printf("                       their_recv:\n");
				Mono_Printf("                       their_sent:\n");
				Mono_Printf("                          my_sent:\n");
				NewMonoMode = 0;
			}
		}
	}
#else
	net = net;
#endif
}	// end of Init_Queue_Mono


/***************************************************************************
 * Update_Queue_Mono -- updates mono display                               *
 *                                                                         *
 * INPUT:                                                                  *
 *      net            ptr to connection manager                           *
 *      flow_index      index # for flow-count updates                     *
 *                     -1: display                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Update_Queue_Mono(ConnManClass *net, int flow_index)
{
#ifdef _DEBUG
	static int flowcount[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//------------------------------------------------------------------------
	// If 'NetMonoMode' is 1, display flowcount info
	//------------------------------------------------------------------------
	if (NetMonoMode==1) {
		if (flow_index >= 0 && flow_index < 20) {
			Mono_Set_Cursor(35,flow_index);
			flowcount[flow_index]++;
			Mono_Printf("%d",flowcount[flow_index]);
		}
	}
	//------------------------------------------------------------------------
	// Otherwise, display the connection debug screen
	//------------------------------------------------------------------------
	else {
		net->Mono_Debug_Print (0,0);
	}

#else
	flow_index = flow_index;
	net = net;
#endif

}	// end of Update_Queue_Mono


/***************************************************************************
 * Print_Framesync_Values -- displays frame-sync variables                 *
 *                                                                         *
 * INPUT:                                                                  *
 *      curframe               current game Frame #                        *
 *      max_ahead            max-ahead value                               *
 *      num_connections      # connections                                 *
 *      their_recv            # commands I've received from my connections *
 *      their_sent            # commands each connection claims to have sent*
 *      my_sent               # commands I've sent                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
static void Print_Framesync_Values(int curframe, unsigned int max_ahead,
	int num_connections, FrameSyncStruct *their, unsigned short my_sent)
{
#ifdef _DEBUG
	int i;

	if (NetMonoMode==1) {
		Mono_Set_Cursor(35,9);
		Mono_Printf("%d",curframe);

		Mono_Set_Cursor(35,10);
		Mono_Printf("%d",max_ahead);

		for (i = 0; i < num_connections; i++) {
			Mono_Set_Cursor(35 + i*5,11);
//			Mono_Printf("%4d",(int)their_recv[i]);
		}

		for (i = 0; i < num_connections; i++) {
			Mono_Set_Cursor(35 + i*5,12);
//			Mono_Printf("%4d",(int)their_sent[i]);
		}

		Mono_Set_Cursor(35,13);
		Mono_Printf("%4d",(int)my_sent);
	}
#else
	curframe = curframe;
	max_ahead = max_ahead;
	num_connections = num_connections;
	their = their;
	my_sent = my_sent;
#endif
}	// end of Print_Framesync_Values



/*************************** end of queue.cpp ******************************/




















/***************************************************************************
 * Init_Queue_Mono -- inits mono display                                   *
 *                                                                         *
 * This routine steals control of the mono screen away from the rest of    *
 * the engine, by setting the global IsMono; if IsMono is set, the other   *
 * routines in this module turn off the Mono display when they're done     *
 * with it, so the rest of the engine won't over-write what we're writing. *
 *                                                                         *
 * INPUT:                                                                  *
 *      net      ptr to connection manager                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
/// <summary>
/// Constructs an empty frame sync record.
/// The counters start at zero and the frame number starts out invalid, so that a player
/// who has not reported in yet cannot be mistaken for one sitting on frame zero.
/// </summary>
FrameSyncStruct::FrameSyncStruct(void)
{
	frame = -1;
	sent = 0;
	recv = 0;
	timing = 0;
	response = 0; // RA2: the two further members are cleared too
	heard = 0; // RA2: as above
}	// end of Init_Queue_Mono


/***************************************************************************
 * Update_Queue_Mono -- updates mono display                               *
 *                                                                         *
 * INPUT:                                                                  *
 *      net            ptr to connection manager                           *
 *      flow_index      index # for flow-count updates                     *
 *                     -1: display                                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
/// <summary>
/// Marks the global channel of an internet game as routed.
/// </summary>
void Queue_Ipx_Set(void) // RA2: 0x00623260
{
	if (Session.Type == GAME_INTERNET) {
		Ipx.Set_Global_Routed(); // RA2: as above
	}
	//------------------------------------------------------------------------
	// Otherwise, display the connection debug screen
	//------------------------------------------------------------------------
}	// end of Update_Queue_Mono


/***************************************************************************
 * Print_Framesync_Values -- displays frame-sync variables                 *
 *                                                                         *
 * INPUT:                                                                  *
 *      curframe               current game Frame #                        *
 *      max_ahead            max-ahead value                               *
 *      num_connections      # connections                                 *
 *      their_recv            # commands I've received from my connections *
 *      their_sent            # commands each connection claims to have sent*
 *      my_sent               # commands I've sent                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/21/1995 BRR : Created.                                             *
 *=========================================================================*/
/// <summary>
/// Clears the routed mark of the global channel in an internet game.
/// </summary>
void Queue_Ipx_Clear(void) // RA2: 0x00623280
{
	if (Session.Type == GAME_INTERNET) {
		Ipx.Clear_Global_Routed(); // RA2: as above
	}
}	// end of Print_Framesync_Values










SyncState::SyncState(void)
{
	Objects = NULL;
	ObjectCount = 0;
	for (int index = 0; index < 5; index++) {
		Layers[index] = NULL;
		LayerCounts[index] = 0;
	}
	Logic = NULL;
	LogicCount = 0;
	BufferSize = 0x20000;
	Buffer = NULL;
}


SyncState::~SyncState(void)
{
	if (Buffer != NULL) {
		delete[] Buffer;
	}
}


void __fastcall Stamp_Their_Timing(int index)
{
	TheirFrameSync[index].heard = timeGetTime();
}
