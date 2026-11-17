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

/* $Header: /counterstrike/SESSION.CPP 3     3/10/97 6:23p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SESSION.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Bill R. Randolph                                             *
 *                                                                                             *
 *                   Start Date : 11/30/95                                                     *
 *                                                                                             *
 *                  Last Update : September 10, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SessionClass::SessionClass -- Constructor                                                 *
 *   SessionClass::~SessionClass -- Destructor                                                 *
 *   SessionClass::One_Time -- one-time initializations                                        *
 *   SessionClass::Init -- Initializes all values                                              *
 *   SessionClass::Create_Connections -- forms connections to other players                    *
 *   SessionClass::Am_I_Master -- tells if the local system is the "master"                    *
 *   SessionClass::Save -- Saves this class to a file                                          *
 *   SessionClass::Load -- Loads this class from a file                                        *
 *   SessionClass::Read_MultiPlayer_Settings -- reads settings from INI                        *
 *   SessionClass::Write_MultiPlayer_Settings -- writes settings to INI                        *
 *   SessionClass::Read_Scenario_Descriptions -- reads scen. descriptions                      *
 *   SessionClass::Free_Scenario_Descriptions -- frees scen. descriptions                      *
 *   SessionClass::Trap_Object -- searches for an object, for debugging                        *
 *   SessionClass::Compute_Unique_ID -- computes unique local ID number                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "session.h"

#include "_keyboar.h"
#include "_map.h"
#include "_rules.h"
#include "ownertalk.h" // RA2
#include "_timer.h" // RA2
#include "addon.h"
#include "ccini.h"
#include "conquer.h"
#include "data.h"
#include "dbgprint.h"
#include "globals.h"
#include "ipxmgr.h"
#include "language\language.h"
#include "mixfile.h" // RA2: MixFileClass
#include "cd.h" // RA2: CD
#include "_pk.h" // RA2
#include "stringtable.h" // RA2
#include "firewall.h" // RA2: FirewallHelperClass
#include "_rand.h" // RA2: NonCriticalRandomNumber
#include "houstype.h" // RA2
#include "goptions.h" // RA2
#include "auddsnd.h" // RA2
#include "audlevel.h" // RA2
#include "audattr.h" // RA2
#include "movie.h" // RA2
#include "vox.h" // RA2
#include "voc.h" // RA2
#include "_tooltip.h" // RA2
#include "cctooltip.h" // RA2
#include "tooltip.h" // RA2
#include "msgloop.h"
#include "multiplayer.h" // RA2: MultiplayerGameMode
#include "nullmgr.h"
#include "phone.h"
#include "progress.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"
#include "special.h"
#include "wonline.h"
#include "xstraw.h"
#include "wstring.h"
#include "_wsproto.h" // RA2: PacketTransport
#include "wspudp.h" // RA2: UDPInterfaceClass

#include <ctime> // for station ID computation
#include <dos.h> // for station ID computation
#include "ftimer.h"


char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x6FA120, static buffer
wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x6FA180, static buffer

void __fastcall Char_To_Wide(wchar_t * dest, char const * source, int limit); // RA2: 0x6FA0C0
void __fastcall Read_Map_Header(char const * filename, wchar_t * description, char * digest, char * game_modes, bool * official, int * min_players, int * max_players); // RA2: 0x6707D0, reads a .mpr file's header


MultiplayerGameMode * First_Game_Mode(void); // RA2: 0x5B6EC0

namespace OwnerDraw { // RA2: RA2's form, as in ipxmgr.cpp
void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); // RA2: 0x6FA... narrows the player name
}


void Leave_Game_Hook(void); // RA2: 0x005173A0, an empty function that Leave_Game calls first

namespace Game { extern int PlayerColor; }

































/***************************** Globals *************************************/
/*---------------------------------------------------------------------------
Min & Max unit count values; index0 = bases OFF, index1 = bases ON
---------------------------------------------------------------------------*/
int SessionClass::CountMin[2] = {1,1};
int SessionClass::CountMax[2] = {50,10};

//---------------------------------------------------------------------------
// This is a list of all the names of the multiplayer scenarios
//---------------------------------------------------------------------------
char SessionClass::Descriptions[100][40];

#if _DEBUG

//---------------------------------------------------------------------------
// These values are used purely for the Mono debug display.  They show the
// names of the Global Channel packet types, and the event types.
//---------------------------------------------------------------------------
char const * SessionClass::GlobalPacketNames[] = {
	"NET_QUERY_GAME",
	"NET_ANSWER_GAME",
	"NET_QUERY_PLAYER",
	"NET_ANSWER_PLAYER",
	"NET_CHAT_ANNOUNCE",
	"NET_CHAT_REQUEST",
	"NET_QUERY_JOIN",
	"NET_CONFIRM_JOIN",
	"NET_REJECT_JOIN",
	"NET_GAME_OPTIONS",
	"NET_SIGN_OFF",
	"NET_GO",
	"NET_MESSAGE",
	"NET_PING",
	"NET_LOADGAME",
	"NET_PROGRESS_REPORT",
	"NET_REQ_SCENARIO",
	"NET_FILE_INFO",
	"NET_FILE_CHUNK",
	"NET_READY_TO_GO",
	"NET_NO_SCENARIO",
	"NET_FILE_INFO_ACK",
	"NET_PUB_GAMEOPT",
	"NET_PRIV_GAMEOPT",
	"NET_PREVIEW_MODE",
	"NET_PREVIEW_ACK",
	"NET_REQ_PREVIEW",
	"NET_PROPOSE_KICK",
};

char const * SessionClass::SerialPacketNames[] = {
	"CONNECT",
	"GAME_OPTIONS",
	"SIGN_OFF",
	"GO",
	"MESSAGE",
	"TIMING",
	"SCORE_SCREEN",
	"LOADGAME",
	"LAST_COMMAND",
	"REQ_SCENARIO",
	"SERIAL_FILE_INFO",
	"SERIAL_FILE_CHUNK",
	"SERIAL_FILE_INFO_ACK",
	"SERIAL_READY_TO_GO",
	"SERIAL_NO_SCENARIO"
};

#endif

char const * SessionClass::DialMethodCheck[ DIAL_METHODS ] = {
	"T",
	"P"
};

char const * const SessionClass::CallWaitStringIDs[ CALL_WAIT_STRINGS_NUM ] = { // RA2: string-table labels (0x7F3214)
	"TXT_CWAIT_STAR70",
	"TXT_CWAIT_70NUM",
	"TXT_CWAIT_1170",
	"TXT_CUSTOM",
};

int NodeNameType::Get_Country(void) // RA2: RA2's signature
{
	return (Slot.InitialCountry == -2) ? -2 : Slot.Country; // RA2: 0x0066E290
}

int NodeNameType::Get_Color(void) // RA2: RA2's signature
{
	return (Slot.InitialColor == -2) ? -2 : Slot.ColorIdx; // RA2: 0x0066E2A0
}

int NodeNameType::Get_Color_Or_Random(void) // RA2: RA2's signature
{
	if (Slot.InitialColor == -2 && Slot.ColorIdx == -1) { // RA2: 0x0066E2B0
		return -2; // RA2: RA2's form of NodeNameType::Get_Color_Or_Random
	}
	return Slot.ColorIdx; // RA2: as above
}

void NodeNameType::Set_Country(int value) // RA2: RA2's signature
{
	if (value == -2) { // RA2: 0x0066E2D0
		Slot.Country = 0; // RA2: RA2's form of NodeNameType::Set_Country
		Slot.InitialCountry = -2; // RA2: as above
	} else {
		Slot.Country = value; // RA2: as above
		Slot.InitialCountry = -1; // RA2: as above
	}
}

void NodeNameType::Set_Color(int value) // RA2: RA2's signature
{
	if (value == -2) { // RA2: 0x0066E300
		Slot.ColorIdx = -1; // RA2: RA2's form of NodeNameType::Set_Color
		Slot.InitialColor = -2; // RA2: as above
	} else {
		Slot.ColorIdx = value; // RA2: as above
		Slot.InitialColor = -1; // RA2: as above
	}
}

SessionOptionsClass::SessionOptionsClass(void) // RA2: RA2's signature
{
	MPGameMode = 0; // RA2: 0x0066E330
	ScenIndex = 0; // RA2: RA2's form of SessionOptionsClass::SessionOptionsClass
	GameSpeed = 0; // RA2: as above
	Credits = 10000; // RA2: as above
	UnitCount = 10; // RA2: as above
	ShortGame = true; // RA2: as above
	SuperWeaponsAllowed = true; // RA2: as above
	BuildOffAlly = true; // RA2: as above
	MCVRepacks = true; // RA2: as above
	CratesAppear = true; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		SlotData[i][0] = 0; // RA2: as above
		SlotData[i][1] = 0; // RA2: as above
		SlotData[i][2] = 0; // RA2: as above
	}
}

/***************************************************************************
 * SessionClass::SessionClass -- Constructor                               *
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
 *   11/30/1995 BRR : Created.                                             *
 *=========================================================================*/
SessionClass::SessionClass(void) : // RA2: RA2's signature
	MPGameMode(NULL) // RA2: initialized before the body
{
	Type = GAME_NORMAL; // RA2: statement order and values of 0x0066E380
	CommProtocol = DEFAULT_COMM_PROTOCOL; // RA2: as above

	Options.ScenarioIndex = 0; // RA2: as above
	Options.Bases = true; // RA2: as above
	Options.Credits = 0; // RA2: as above
	Options.BridgeDestruction = false; // RA2: as above
	Options.Goodies = false; // RA2: as above
	Options.Ghosts = false; // RA2: as above
	Options.UnitCount = 0; // RA2: as above
	Options.ShortGame = false; // RA2: as above
	Options.SWAllowed = false; // RA2: as above
	Options.BuildOffAlly = false; // RA2: as above
	Options.GameSpeed = 0; // RA2: as above
	Options.CrapEngineers = false; // RA2: as above
	Options.AlliesAllowed = false; // RA2: as above
	Options.HarvTruce = false; // RA2: as above
	Options.CTF = false; // RA2: as above
	Options.FogOfWar = false; // RA2: as above
	Options.MCVRedeploy = false; // RA2: as above
	Options.AIDifficulty = DIFF_NORMAL; // RA2: as above

	UniqueID = 0;
	Handle[0] = 0;
	House = 0; // RA2: as above
	idxSide2 = -1; // RA2: as above
	PrefColor = 2; // RA2: as above
	ColorIdx = -1; // RA2: as above
	Side = House; // RA2: as above
	Side2 = idxSide2; // RA2: as above
	Color = PrefColor; // RA2: as above
	Color2 = ColorIdx; // RA2: as above

	WOLTaunts = true; // RA2: as above
	LANTaunts = true; // RA2: as above
	WOLScrollText = true; // RA2: as above
	LANScrollText = true; // RA2: as above

	for (int p = 0; p < 8; p++) { // RA2: RA2's form of SessionClass::SessionClass
		PlayerPorts[p] = 0; // RA2: as above
	}
	RouterProbeActive = false; // RA2: as above
	PortProbeDue = false; // RA2: as above
	RouterProbePort = 0; // RA2: as above
	RouterProbePort2 = 0; // RA2: as above
	PortBase = 1250; // RA2: as above
	ForcePortBase = 0; // RA2: as above
	RotatePortBase = true; // RA2: as above
	SendDelay = false; // RA2: as above
	PortNumberOverride = 0; // RA2: as above
	memset(ManglerAddresses, 0, sizeof(ManglerAddresses)); // RA2: 0xB26CBC, the WOL mangler server addresses
	memset(ManglerPorts, 0, sizeof(ManglerPorts)); // RA2: 0xB291A4

	MultiplayerObserver = 0; // RA2: as above
	Unknown_304 = 0; // RA2: as above
	LastNicknameSlot = -1; // RA2: as above
	WOLLimitResolution = false; // RA2: as above
	ViaPacketRouter = false; // RA2: as above
	MasterPlayerID = -1;
	memset(MasterPlayerName, 0, sizeof(MasterPlayerName));
	SlowestResponse = 0; // RA2: as above
	UseRouter = true; // RA2: as above
	LastTimingID = -1; // RA2: as above
	AboutToExitActive = false; // RA2: as above
	RouterResponse = 20; // RA2: as above
	ExitEventPending = 0; // RA2: as above
	ExitEventFrame = 0; // RA2: as above
	ExitEventPlayer = -1; // RA2: as above
	PacketRouterPort = 0; // RA2: as above

	SquadAlliances = false;
	SawGameCompletion = false;
	OutOfSync = false;

	MaxPlayers = MAX_PLAYERS;
	NumPlayers = 0;

	FrameSendRate = DEFAULT_FRAME_SEND_RATE;

	MaxAhead = FrameSendRate * 3;
	MaxMaxAhead = MaxAhead;

	memset(ConnectionStats, 0, sizeof(ConnectionStats));

	PrecalcMaxAhead = 0;
	PrecalcDesiredFrameRate = 0;
	EnableMultiplayerDebug = false; // RA2: as above
	DrawMPDebugStats = false; // RA2: as above
	LoadGame = 0;
	EmergencySave = 0;

	LastMessage[0] = 0;
	WWChat = 0;

	RecordFile.Set_Name("RECORD.BIN");  // always uses this name
	Record = 0;                          // set via command line // RA2: as above
	Play = 0;                           // set via command line
	Attract = 0;                        // set via command line

	IsBridge = 0;
	NetStealth = false; // RA2: as above
	NetProtect = true; // RA2: as above
	NetOpen = false; // RA2: as above
	GameName[0] = 0;
	GProductID = 0;
	Suspended = 0;

	LatencyFudge = 1;

	MetaSize = MAX_IPX_PACKET_SIZE;

	memset(KickVoteCount, 0, sizeof(KickVoteCount));
	memset(KickVoteWho, -1, sizeof(KickVoteWho));

	ModemService = true;
	CurPhoneIdx = 0;                                    // set from INI file
	SerialDefaults.Port = 0x2f8;                        // set from INI file
	SerialDefaults.IRQ = 3;                             // set from INI file
	SerialDefaults.Baud = 9600;                         // set from INI file
	SerialDefaults.DialMethod = DIAL_TOUCH_TONE;        // set from INI file
	SerialDefaults.InitStringIndex = -1;                // set from INI file
	SerialDefaults.CallWaitStringIndex = 0;             // set from INI file
	strcpy(SerialDefaults.CallWaitString, ""); // RA2: set from INI file
	ModemType = MODEM_NULL_HOST;						// set from INI file

	TrapFrame = 0x7fffffff;     // frame to start trapping object values at
	TrapObjType = RTTI_NONE;    // type of object to trap
	TrapObject.Ptr.All = NULL;  // ptr to object being trapped
	TrapCoord = COORD_NONE;     // COORDINATE of object to trap
	TrapTarget.Invalidate();    // TARGET value of object to trap
	TrapCell = NULL;            // for trapping a cell
	TrapCheckHeap = 0;          // start checking the Heap
	TrapPrintCRC = 0;           // output CRC file

	IsStartReminded = false; // RA2: as above
	Options.AIPlayers = 0; // RA2: as above
	for (int i = 0; i < 8; i++) { // RA2: as above
		Options.AISlots.Countries[i] = -1; // RA2: as above
		Options.AISlots.Colors[i] = -1; // RA2: as above
	}
	TournamentState = 0; // RA2: +0x0C
	unknown_10 = 0; // RA2: as above
	ScenarioFileName[0] = 0; // RA2: as above
	ScenarioFileLength = 0; // RA2: as above
	ScenarioIsOfficial = false; // RA2: as above
	ScenarioMaxPlayers = 0; // RA2: as above
}

void SessionClass::Load_Call_Wait_Strings(void) // RA2: RA2's signature
{
	for (int i = 0; i < CALL_WAIT_STRINGS_NUM; i++) {
		CallWaitStrings.Add(_wcsdup(StringTable::LoadString(CallWaitStringIDs[i], NULL, "D:\\Ra2\\Session.CPP", 536))); // RA2: 0x0066EAA0
	}
}	// end of SessionClass


/***************************************************************************
 * SessionClass::~SessionClass -- Destructor                               *
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
 *   11/30/1995 BRR : Created.                                             *
 *=========================================================================*/
SessionClass::~SessionClass(void)
{
	while (InitStrings.Count()) {
		delete[] InitStrings[0];
		InitStrings.Delete_Index(0);
	}

	while (CallWaitStrings.Count()) {
		free((void *)CallWaitStrings[0]);
		CallWaitStrings.Delete_Index(0);
	}
}	// end of ~SessionClass


/***************************************************************************
 * SessionClass::One_Time -- one-time initializations                      *
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
 *   12/01/1995 BRR : Created.                                             *
 *=========================================================================*/
void SessionClass::One_Time(void)
{
	//Read_MultiPlayer_Settings();
    Read_Scenario_Descriptions();
    UniqueID = Compute_Unique_ID();
    DebugString("Session one time init. UniqueID is %08x\n", UniqueID);
}



/***************************************************************************
 * SessionClass::Init -- Initializes all values                            *
 *                                                                         *
 * This function should be called for every new game played; it only sets  *
 * those variables that should be set for a new game.                      *
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
 *   11/30/1995 BRR : Created.                                             *
 *=========================================================================*/
void SessionClass::Init(void)
{

}	// end of Init


/***************************************************************************
 * SessionClass::Create_Connections -- forms connections to other players  *
 *                                                                         *
 * This routine uses the contents of the Players vector, combined with     *
 * that of the Houses array, to create connections to each other player.   *
 * It is assumed that 'Players' contains all the other players to connect  *
 * to, and that the HouseClass's have been filled in with players' data.   *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = success, 0 = failure                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   11/30/1995 BRR : Created.                                             *
 *=========================================================================*/
int SessionClass::Create_Connections(void)
{
    NetNumType net;
    NetNodeType node;

    DebugString("Entering Create_Connections\n");
    if (Session.Type != GAME_IPX && Session.Type != GAME_INTERNET) {
        return(0);
    }

    for (int i = 0; i < Players.Count(); i++) { // RA2: RA2's form of SessionClass::Create_Connections
        DebugString("Player %d, Name: %s, ID %d\n", i, Players[i]->Name, Players[i]->Player.ID);
        if (wcsicmp(Players[i]->Name, Houses[Players[i]->Player.ID]->UIName) != 0) { // RA2: as above
            return(0);
        }
        if (wcsicmp(Players[i]->Name, MasterPlayerName) == 0) { // RA2: as above
            MasterPlayerID = Players[i]->Player.ID;
            Session.Players[i]->Address.Get_Address(net, node);
            Session.PacketRouterAddress.Set_Address(net, node, 0); // RA2: as above
            Ipx.Set_Lobby_Address(&Session.PacketRouterAddress); // RA2: as above
        }
        Houses[Players[i]->Player.ID]->SquadID = Players[i]->Player.SquadID;
        Session.Players[i]->Address.Get_Address(net, node);
        unsigned long ip = 0; // RA2: as above
        for (int j = 0; j < 4; j++) { // RA2: as above
            ip = (ip << 8) | node[j]; // RA2: as above
        }
        DebugString("House[%d] IP = %X  Clan=%d\n", Players[i]->Player.ID, ip, Players[i]->Player.SquadID); // RA2: as above
        Houses[Players[i]->Player.ID]->IPAddress = ip;
        Houses[Players[i]->Player.ID]->LostConnection = false;
        if (i > 0) {
            Ipx.Create_Connection(Players[i]->Player.ID, Players[i]->Name, &Players[i]->Address); // RA2: as above
            Players[i]->Player.ProcessTime = -1;
        }
    }

    for (int k = 0; k < 8; k++) { // RA2: as above
        Session.PlayerLastTime[k] = TickCount; // RA2: as above
    }

	//------------------------------------------------------------------------
	// Loop through all entries in 'Players'
	//------------------------------------------------------------------------
    if (Type == GAME_INTERNET && UseRouter) { // RA2: as above
        NetNumType net2; // RA2: as above
        NetNodeType node2; // RA2: as above
        net2[0] = 0; // RA2: as above
        net2[1] = 0; // RA2: as above
        net2[2] = 0; // RA2: as above
        net2[3] = 0; // RA2: as above
        node2[0] = 0; // RA2: as above
        node2[1] = 0; // RA2: as above
        node2[2] = 0; // RA2: as above
        node2[3] = 0; // RA2: as above
        node2[4] = 0; // RA2: as above
        node2[5] = 0; // RA2: as above
        Session.PacketRouterAddressTranslate.Set_Address(net2, node2, 0); // RA2: as above
        ViaPacketRouter = true; // RA2: as above
        SlowestResponse = 0; // RA2: as above
        RouterResponse = 20; // RA2: as above
        LastTimingID = -1; // RA2: as above
        Ipx.Create_Multicast_Connection(); // RA2: as above
        OwnerTalk.Build_Connection_List(); // RA2: as above
        if (Session.Am_I_Master(0)) { // RA2: as above
            bool connected = OwnerTalk.Start_Router(NULL); // RA2: as above
            if (connected) { // RA2: as above
                connected = OwnerTalk.Send_Player_List(); // RA2: as above
            }
            if (!connected) { // RA2: as above
                Session.ViaPacketRouter = false; // RA2: as above
            }
        }
    } else {
        Session.ViaPacketRouter = false; // RA2: as above
    }
    DebugString("Session.ViaPacketRouter = %s\n", Session.ViaPacketRouter ? "true" : "false"); // RA2: as above
    DebugString("Leaving Create_Connections\n");
    return(1);
}



/***************************************************************************
 * SessionClass::Am_I_Master -- tells if the local system is the "master"  *
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
 *   11/29/1995 BRR : Created.                                             *
 *=========================================================================*/
bool SessionClass::Am_I_Master(HouseClass *house) // RA2: RA2's signature
{
    if (house == 0) { // RA2: RA2's form of SessionClass::Am_I_Master
        house = PlayerPtr; // RA2: as above
    }
    if (Session.Type == GAME_INTERNET && house != 0) { // RA2: as above
        if (MasterPlayerID != -1) {
            return house->HeapID == MasterPlayerID; // RA2: as above
        }
        if (_wcsicmp(house->UIName, MasterPlayerName) == 0) { // RA2: as above
            return(true);
        }
    }
    for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
        HouseClass *player = Houses[i]; // RA2: as above
        if (player->IsHuman && !player->IsDefeated) { // RA2: as above
            if (house == player) { // RA2: as above
                return(true);
            } else {
                return(false);
            }
        }
    }
    return(false);
}

void SessionOptionsClass::Read(CCINIClass & ini, char const * section, int default_slot, int default_first_slot) // RA2: RA2's signature
{
	char entry[16]; // RA2: RA2's form of SessionOptionsClass::Read

	int mode = First_Game_Mode()->MPModeIndex; // RA2: the executable fetches the default game mode into a local before the first Get_Int (0x0066F1F9)
	MPGameMode = ini.Get_Int(section, "GameMode", mode); // RA2: as above
	ScenIndex = ini.Get_Int(section, "ScenIndex", 0); // RA2: as above
	GameSpeed = ini.Get_Int(section, "GameSpeed", Rule->GameSpeed); // RA2: as above
	Credits = ini.Get_Int(section, "Credits", Rule->MPMoney); // RA2: as above
	UnitCount = ini.Get_Int(section, "UnitCount", Rule->MPUnitCount); // RA2: as above
	ShortGame = ini.Get_Bool(section, "ShortGame", Rule->ShortGame); // RA2: as above
	SuperWeaponsAllowed = ini.Get_Bool(section, "SuperWeaponsAllowed", Rule->SuperWeaponsAllowed); // RA2: as above
	BuildOffAlly = ini.Get_Bool(section, "BuildOffAlly", Rule->BuildOffAlly); // RA2: as above
	MCVRepacks = ini.Get_Bool(section, "MCVRepacks", Rule->MCVRedeploys); // RA2: as above
	CratesAppear = ini.Get_Bool(section, "CratesAppear", Rule->IsMPCrates); // RA2: as above

	for (int i = 1; i < 8; i++) { // RA2: as above
		sprintf(entry, "Slot%02d", i); // RA2: as above
		int first = (i == 1) ? default_first_slot : default_slot; // RA2: as above
		int second = -2; // RA2: as above
		int third = -2; // RA2: as above
		ini.Get_Int3(section, entry, &first, &second, &third); // RA2: as above
		SlotData[i][0] = first; // RA2: as above
		SlotData[i][1] = second; // RA2: as above
		SlotData[i][2] = third; // RA2: as above
	}
}
	// end of Am_I_Master
void SessionClass::Read_MultiPlayer_Settings(void)
{
	char * tokenptr; // RA2: RA2's form of SessionClass::Read_MultiPlayer_Settings
	PhoneEntryClass * phone; // RA2: as above
	char * entry; // RA2: as above
	char buf[0x400]; // RA2: as above
	int i;

	// RA2: Read_MultiPlayer_Settings reads RA2.INI
	Rule->Do_HouseTypes(*RuleINI);

	//------------------------------------------------------------------------
	// Clear the initstring entries
	//------------------------------------------------------------------------
	for (i = 0; i < InitStrings.Count(); i++) {
		delete[] InitStrings[i];
	}
	InitStrings.Clear();

	// Clear the dialing entries
	for (i = 0; i < PhoneBook.Count(); i++) {
		delete PhoneBook[i];
	}
	PhoneBook.Clear();

	// Create filename and read the file.
	CCFileClass file("RA2.INI"); // RA2: as above

	// Get the player's last-used Handle
	if (!PortsRead) { // RA2: as above
		PortBase = ConfigINI.Get_Int("MultiPlayer", "PortBase", 0x4E2); // RA2: as above
		ForcePortBase = ConfigINI.Get_Int("MultiPlayer", "ForcePortBase", 0); // RA2: as above
		FirewallHelper.Port = // RA2: as above
			ConfigINI.Get_Int("MultiPlayer", "PortPool", NonCriticalRandomNumber(0, 0x8000) + 0x1000); // RA2: as above
		PortsRead = true; // RA2: as above
		FirewallHelper.Read_Settings(); // RA2: as above
	}
	Session.SendDelay = ConfigINI.Get_Bool("MultiPlayer", "SendDelay", false); // RA2: as above
	Session.PortNumberOverride = ConfigINI.Get_Int("MultiPlayer", "PortNumberOverride", 0); // RA2: as above

	// Get the player's last-used Color
	buf[0] = 0;
	CurPhoneIdx = ConfigINI.Get_Int("MultiPlayer", "PhoneIndex", -1);
	TrapCheckHeap = ConfigINI.Get_Int("MultiPlayer", "CheckHeap", 0);
	WOLLimitResolution = ConfigINI.Get_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution); // RA2: as above
	LastNicknameSlot = ConfigINI.Get_Int("MultiPlayer", "LastNickSlot", -1);

	// Read in default serial settings
	wchar_t handle[20]; // RA2: as above
	ConfigINI.Get_UnicodeString( // RA2: as above
		"MultiPlayer", "Handle", // RA2: as above
		StringTable::LoadString("TXT_NONAME", 0, "D:\\Ra2\\Session.CPP", 0x4b2), handle, 20); // RA2: as above
	OwnerDraw::WideToCharString(Handle, handle, -1); // RA2: as above

	Color = ConfigINI.Get_Int("MultiPlayer", "Color", Color); // RA2: as above
	Color2 = ConfigINI.Get_Int("MultiPlayer", "ColorEx", Color2); // RA2: as above
	Side = ConfigINI.Get_HousesType("MultiPlayer", "Side", (HousesType)Side); // RA2: as above
	Side2 = ConfigINI.Get_Int("MultiPlayer", "SideEx", Side2); // RA2: as above
	WOLTaunts = ConfigINI.Get_Bool("MultiPlayer", "WOLTaunts", WOLTaunts); // RA2: as above
	LANTaunts = ConfigINI.Get_Bool("MultiPlayer", "LANTaunts", LANTaunts); // RA2: as above
	WOLScrollText = ConfigINI.Get_Bool("MultiPlayer", "WOLScrollText", WOLScrollText); // RA2: as above
	LANScrollText = ConfigINI.Get_Bool("MultiPlayer", "LANScrollText", LANScrollText); // RA2: as above

	House = Side; // RA2: as above
	if (House == -2) { // RA2: as above
		House = 0; // RA2: as above
	}
	idxSide2 = Side2; // RA2: as above

	MPGameMode = MultiplayerGameMode::Get(ConfigINI.Get_Int("MultiPlayer", "GameMode", 1)); // RA2: as above
	if (!MPGameMode) { // RA2: as above
		MPGameMode = First_Game_Mode(); // RA2: as above
	}
	Skirmish.Read(ConfigINI, "Skirmish", 1, 6); // RA2: as above
	LAN.Read(ConfigINI, "LAN", 2, 2); // RA2: as above
	WOL.Read(ConfigINI, "WonlinePref", 2, 2); // RA2: as above

	ConfigINI.Get_String("SerialDefaults", "ModemName", buf, SerialDefaults.ModemName, 0x3F); // RA2: as above
	if (strcmp(SerialDefaults.ModemName, buf) == 0) { // RA2: as above
		SerialDefaults.ModemName[0] = 0;
	}
	SerialDefaults.Port = ConfigINI.Get_Int("SerialDefaults", "Port", 0);
	SerialDefaults.IRQ = ConfigINI.Get_Int("SerialDefaults", "IRQ", -1);
	SerialDefaults.Baud = ConfigINI.Get_Int("SerialDefaults", "Baud", -1);
	SerialDefaults.Compression = ConfigINI.Get_Int("SerialDefaults", "Compression", 0);
	SerialDefaults.ErrorCorrection = ConfigINI.Get_Int("SerialDefaults", "ErrorCorrection", 0);

	ConfigINI.Get_String("SerialDefaults", "DialMethod", "T", buf, 2);
	for (i = 0; i < 2; i++) { // RA2: as above
		if (!_stricmp(buf, DialMethodCheck[i])) { // RA2: as above
			SerialDefaults.DialMethod = (DialMethodType)i;
			break;
		}
	}
	if (i == 2) { // RA2: as above
		SerialDefaults.DialMethod = DIAL_TOUCH_TONE;
	}

	SerialDefaults.InitStringIndex = ConfigINI.Get_Int("SerialDefaults", "InitStringIndex", -1);
	SerialDefaults.CallWaitStringIndex = ConfigINI.Get_Int("SerialDefaults", "CallWaitStringIndex", 3); // RA2: as above
	ConfigINI.Get_String("SerialDefaults", "CallWaitString", "", SerialDefaults.CallWaitString, 0x10); // RA2: as above

	if (SerialDefaults.IRQ == 0 || SerialDefaults.Baud == 0) {
		SerialDefaults.Port = 0;
		SerialDefaults.IRQ = -1;
		SerialDefaults.Baud = -1;
	}

	int initcount = ConfigINI.Entry_Count("InitStrings");

	int index;
	for (index = 0; index < initcount; index++) {
		entry = new char[0x29]; // RA2: as above
		if (entry) { // RA2: as above
			entry[0] = 0;
			ConfigINI.Get_String("InitStrings", ConfigINI.Get_Entry("InitStrings", index), 0, entry, // RA2: as above
									0x29); // RA2: as above
			_strupr(entry); // RA2: as above
			InitStrings.Add(entry); // RA2: as above
		}
	}
	SerialDefaults.InitStringIndex = initcount ? 0 : -1; // RA2: as above

	int phonecount = ConfigINI.Entry_Count("PhoneBook");
	for (index = 0; index < phonecount; index++) {
		// Create a new phone book entry
		phone = new PhoneEntryClass; // RA2: as above
		ConfigINI.Get_String("PhoneBook", ConfigINI.Get_Entry("PhoneBook", index), 0, buf, sizeof(buf)); // RA2: as above

		// Read the entire entry in
		tokenptr = strtok(buf, "|"); // RA2: as above
		if (tokenptr) {
			Char_To_Wide(phone->Name, tokenptr, -1); // RA2: as above
			_wcsupr(phone->Name); // RA2: as above
		} else {
			phone->Name[0] = 0;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			strcpy(phone->Number, tokenptr); // RA2: as above
			_strupr(phone->Number); // RA2: as above
		} else {
			phone->Number[0] = 0;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			sscanf(tokenptr, "%x", &phone->Settings.Port); // RA2: as above
		} else {
			phone->Settings.Port = 0;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.IRQ = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.IRQ = -1;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.Baud = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.Baud = -1;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.Compression = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.Compression = -1;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.ErrorCorrection = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.ErrorCorrection = 0;
		}

		/*
		**	Find out if this phonebook entry has the new settings included. If not
		**	then we need to skip this section.
		*/
		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			strcpy(buf, tokenptr); // RA2: as above
			for (i = 0; i < 2; i++) { // RA2: as above
				if (!_stricmp(buf, DialMethodCheck[i])) { // RA2: as above
					phone->Settings.DialMethod = (DialMethodType)i;
					break;
				}
			}
			if (i == 2) { // RA2: as above
				phone->Settings.DialMethod = DIAL_TOUCH_TONE;
			}
		} else {
			phone->Settings.DialMethod = DIAL_TOUCH_TONE;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.InitStringIndex = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.InitStringIndex = -1;
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			phone->Settings.CallWaitStringIndex = atoi(tokenptr); // RA2: as above
		} else {
			phone->Settings.CallWaitStringIndex = 3; // RA2: as above
		}

		tokenptr = strtok(0, "|"); // RA2: as above
		if (tokenptr) {
			strcpy(phone->Settings.CallWaitString, tokenptr);
		} else {
			phone->Settings.CallWaitString[0] = 0;
		}

		// Add it to our list
		PhoneBook.Add(phone);
	}

	// Read special recording playback values, to help find sync bugs
	if (Session.Play) {
		TrapFrame = ConfigINI.Get_Int("SyncBug", "Frame", 0x7FFFFFFF); // RA2: as above
		ConfigINI.Get_String("SyncBug", "Type", "NONE", buf, 0x50); // RA2: as above
		if (!_stricmp(buf, "AIRCRAFT")) { // RA2: as above
			TrapObjType = RTTI_AIRCRAFT;
		} else if (!_stricmp(buf, "ANIM")) { // RA2: as above
			TrapObjType = RTTI_ANIM;
		} else if (!_stricmp(buf, "BUILDING")) { // RA2: as above
			TrapObjType = RTTI_BUILDING;
		} else if (!_stricmp(buf, "BULLET")) { // RA2: as above
			TrapObjType = RTTI_BULLET;
		} else if (!_stricmp(buf, "INFANTRY")) { // RA2: as above
			TrapObjType = RTTI_INFANTRY;
		} else if (!_stricmp(buf, "UNIT")) { // RA2: as above
			TrapObjType = RTTI_UNIT;
		} else {
			TrapObjType = RTTI_NONE;
		}

		ConfigINI.Get_String("SyncBug", "Coord", "0", buf, 0x50); // RA2: as above
		int x;
		int y;
		int z;
		sscanf(buf, "%d,%d,%d", &x, &y, &z); // RA2: as above
		int target = ConfigINI.Get_Int("SyncBug", "Target", -1);
		TrapTarget.Decode(target);

		ConfigINI.Get_String("SyncBug", "Cell", "0,0", buf, 0x50); // RA2: as above
		sscanf(buf, "%d,%d", &x, &y);
		Cell cell(x, y);
		if (cell != CELL_NONE) {
			TrapCell = &Map[cell]; // RA2: as above
		}

		TrapPrintCRC = ConfigINI.Get_Int("SyncBug", "PrintCRC", 0x7FFFFFFF); // RA2: as above
	}
}






/// <summary>
/// Writes the session settings out to a log file.
/// This routine is used when a sync bug or some other multiplayer failure is being
/// recorded, so that the game options and the players' network addresses are captured
/// alongside it.
/// </summary>
/// <param name="out">The file to write the report to.</param>
bool SessionClass::Log_To_File(FILE * out) // RA2: RA2's signature
{
	int i;
	int j;
	NetNumType net;
	NetNodeType node;

	if (Session.Options.ScenarioIndex < 0) {
		fprintf(out, "BOGUS scenario index!?!\n");
		Session.Options.ScenarioIndex = 0;
	}

	fprintf(out, "Scenario = %s\n", Session.Scenarios[Session.Options.ScenarioIndex]->Description());
	fprintf(out, "Handle = %s\n", Handle);

	for (i = 0; i < NumPlayers; i++) {
		Session.Players[i]->Address.Get_Address(net, node);
		fprintf(out, "Player Addr[%d] = ", i);
		for (j = 0; j < sizeof(NetNumType); j++) {
			fprintf(out, "%02X", net[j]);
		}
		fprintf(out, ":"); // RA2: RA2's form of SessionClass::Log_To_File
		for (j = 0; j < sizeof(NetNodeType); j++) {
			fprintf(out, "%02X", node[j]);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "\n");

	Session.HostAddress.Get_Address(net, node);
	fprintf(out, "Address = ");
	for (i = 0; i < sizeof(NetNumType); i++) {
		fprintf(out, "%02X", net[i]);
	}

	fprintf(out, ":"); // RA2: as above
	for (i = 0; i < sizeof(NetNodeType); i++) {
		fprintf(out, "%02X", node[i]);
	}

	fprintf(out, "\n\n"); // RA2: as above
	fprintf(out, "MaxAhead = %d\n", MaxAhead); // RA2: as above
	fprintf(out, "LoadGame = %d\n", LoadGame); // RA2: as above
	fprintf(out, "PrefColor = %d\n", Color); // RA2: as above
	fprintf(out, "ColorIdx = %d\n", PrefColor); // RA2: as above
	fprintf(out, "House = %d\n", House); // RA2: as above
	fprintf(out, "NumPlayers = %d\n", NumPlayers); // RA2: as above
	fprintf(out, "Options.Bases = %d\n", Options.Bases); // RA2: as above
	fprintf(out, "Options.Credits = %d\n", Options.Credits); // RA2: as above
	fprintf(out, "Options.BridgeDestruction = %d\n", Options.BridgeDestruction); // RA2: as above
	fprintf(out, "Options.Goodies = %d\n", Options.Goodies); // RA2: as above
	fprintf(out, "Options.Ghosts = %d\n", Options.Ghosts); // RA2: as above
	fprintf(out, "Options.UnitCount = %d\n", Options.UnitCount); // RA2: as above
	fprintf(out, "Options.AIPlayers = %d\n", Options.AIPlayers); // RA2: as above
	fprintf(out, "Options.AIDifficulty = %d\n", Options.AIDifficulty); // RA2: as above
	fprintf(out, "ObiWan = %d\n", MultiplayerObserver); // RA2: as above

	if (Session.Type == GAME_IPX) {
		fprintf(out, "Type = IPX\n");
	} else if (Session.Type == GAME_INTERNET) {
		fprintf(out, "Type = INTERNET\n");
	} else {
		fprintf(out, "Type = !!Unknown!!\n");
	}

	return(true);
}
void SessionOptionsClass::Write(CCINIClass & ini, char const * section) // RA2: RA2's signature
{
	char entry[16]; // RA2: RA2's form of SessionOptionsClass::Write

	ini.Put_Int(section, "GameMode", MPGameMode); // RA2: 0x00670270
	ini.Put_Int(section, "ScenIndex", ScenIndex); // RA2: as above
	ini.Put_Int(section, "GameSpeed", GameSpeed); // RA2: as above
	ini.Put_Int(section, "Credits", Credits); // RA2: as above
	ini.Put_Int(section, "UnitCount", UnitCount); // RA2: as above
	ini.Put_Bool(section, "ShortGame", ShortGame); // RA2: as above
	ini.Put_Bool(section, "SuperWeaponsAllowed", SuperWeaponsAllowed); // RA2: as above
	ini.Put_Bool(section, "BuildOffAlly", BuildOffAlly); // RA2: as above
	ini.Put_Bool(section, "MCVRepacks", MCVRepacks); // RA2: as above
	ini.Put_Bool(section, "CratesAppear", CratesAppear); // RA2: as above

	for (int i = 1; i < 8; i++) { // RA2: as above
		sprintf(entry, "Slot%02d", i); // RA2: as above
		ini.Put_Int3(section, entry, SlotData[i][0], SlotData[i][1], SlotData[i][2]); // RA2: as above
	}
}






/***************************************************************************
 * SessionClass::Write_MultiPlayer_Settings -- writes settings INI         *
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
 *   02/14/1995 BR : Created.                                              *
 *=========================================================================*/
void SessionClass::Write_MultiPlayer_Settings(void)
{
    RawFileClass file("RA2.INI"); // RA2: RA2's form of SessionClass::Write_MultiPlayer_Settings

    ConfigINI.Put_Int("MultiPlayer", "PhoneIndex", CurPhoneIdx);
    ConfigINI.Put_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution); // RA2: as above
    ConfigINI.Put_Int("MultiPlayer", "LastNickSlot", LastNicknameSlot);
    ConfigINI.Put_UnicodeString("MultiPlayer", "Handle", Narrow_To_Wide(Handle)); // RA2: as above
    ConfigINI.Put_Int("MultiPlayer", "Color", Color); // RA2: as above
    ConfigINI.Put_Int("MultiPlayer", "ColorEx", Color2); // RA2: as above
    ConfigINI.Put_HousesType("MultiPlayer", "Side", (HousesType)Side); // RA2: as above
    ConfigINI.Put_Int("MultiPlayer", "SideEx", Side2); // RA2: as above
    if (MPGameMode) { // RA2: as above
        ConfigINI.Put_Int("MultiPlayer", "GameMode", MPGameMode->MPModeIndex); // RA2: as above
    } else {
        ConfigINI.Put_Int("MultiPlayer", "GameMode", 0); // RA2: as above
    }
    Skirmish.Write(ConfigINI, "Skirmish"); // RA2: as above
    LAN.Write(ConfigINI, "LAN"); // RA2: as above
    WOL.Write(ConfigINI, "WonlinePref"); // RA2: as above

    ConfigINI.Clear("SerialDefaults");
    ConfigINI.Put_String("SerialDefaults", "CallWaitString", SerialDefaults.CallWaitString);
    ConfigINI.Put_Int("SerialDefaults", "CallWaitStringIndex", SerialDefaults.CallWaitStringIndex);
    ConfigINI.Put_Int("SerialDefaults", "InitStringIndex", SerialDefaults.InitStringIndex);
    ConfigINI.Put_String("SerialDefaults", "DialMethod", DialMethodCheck[SerialDefaults.DialMethod]); // RA2: as above
    ConfigINI.Put_Int("SerialDefaults", "Baud", SerialDefaults.Baud);
    ConfigINI.Put_Int("SerialDefaults", "IRQ", SerialDefaults.IRQ);
    ConfigINI.Put_Int("SerialDefaults", "Port", SerialDefaults.Port, 1);
    ConfigINI.Put_Int("SerialDefaults", "Compression", SerialDefaults.Compression); // RA2: as above
    ConfigINI.Put_Int("SerialDefaults", "ErrorCorrection", SerialDefaults.ErrorCorrection); // RA2: as above
    ConfigINI.Put_String("SerialDefaults", "ModemName", SerialDefaults.ModemName);

    ConfigINI.Clear("InitStrings");
    for (int index = 0; index < InitStrings.Count(); index++) {
        char buf[10];
        wsprintfA(buf, "%03d", index); // RA2: as above
        ConfigINI.Put_String("InitStrings", buf, InitStrings[index]);
    }

    ConfigINI.Clear("PhoneBook");
    for (int i = PhoneBook.Count() - 1; i >= 0; i--) { // RA2: as above
        char buf[128];
        char entrytext[10];
        wsprintfA(buf, "%s|%s|%x|%d|%d|%d|%d|%s|%d|%d|%s", // RA2: as above
                  PhoneBook[i]->Name,
                  PhoneBook[i]->Number,
                  PhoneBook[i]->Settings.Port,
                  PhoneBook[i]->Settings.IRQ,
                  PhoneBook[i]->Settings.Baud,
                  PhoneBook[i]->Settings.Compression,
                  PhoneBook[i]->Settings.ErrorCorrection,
                  DialMethodCheck[PhoneBook[i]->Settings.DialMethod], // RA2: as above
                  PhoneBook[i]->Settings.InitStringIndex,
                  PhoneBook[i]->Settings.CallWaitStringIndex,
                  PhoneBook[i]->Settings.CallWaitString);
        wsprintfA(entrytext, "%03d", i); // RA2: as above
        ConfigINI.Put_String("PhoneBook", entrytext, buf);
    }

    ConfigINI.Save(file, false);
}

void SessionClass::Write_Chat_Settings(void) // RA2: RA2's signature
{
	RawFileClass file("RA2.INI"); // RA2: 0x00670730

	ConfigINI.Put_Bool("MultiPlayer", "WOLTaunts", WOLTaunts); // RA2: RA2's form of SessionClass::Write_Chat_Settings
	ConfigINI.Put_Bool("MultiPlayer", "LANTaunts", LANTaunts); // RA2: as above
	ConfigINI.Put_Bool("MultiPlayer", "WOLScrollText", WOLScrollText); // RA2: as above
	ConfigINI.Put_Bool("MultiPlayer", "LANScrollText", LANScrollText); // RA2: as above
	ConfigINI.Save(file, false);
}






/// <summary>
/// RA2
/// </summary>
static inline void Add_Missions(DynamicVectorClass<MultiMission *> & scenarios, INIClass & ini, char * name_buffer)
{
	int count = ini.Entry_Count("MultiMaps");
	for (int index = 0; index < count; index++) {
		if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), "", name_buffer, 64)) {
			scenarios.Add(new MultiMission(ini, name_buffer));
		}
	}
}


/// <summary>
/// RA2
/// </summary>
static inline bool Next_File(HANDLE handle, WIN32_FIND_DATA & block)
{
	return(FindNextFile(handle, &block) != 0);
}





void __fastcall Read_Map_Header(char const * filename, wchar_t * description, char * digest, char * game_modes, bool * official, int * min_players, int * max_players) // RA2: 0x006707D0, reads the [Basic] and [Digest] sections of a map file
{
	CCFileClass file(filename); // RA2: as above
	if (!file.Is_Available() || !file.Open(FileClass::READ)) { // RA2: as above
		return; // RA2: as above
	}

	int percent = 0; // RA2: as above
	int size; // RA2: as above
	char * head = NULL; // RA2: as above
	char * end; // RA2: as above
	do { // RA2: as above
		percent += 5; // RA2: reads 5% steps of the file until the [Basic] section is complete
		size = file.Size() * percent / 100; // RA2: as above
		if (head) { // RA2: as above
			delete head; // RA2: as above
		}
		head = new char[size + 1]; // RA2: as above
		if (!head) { // RA2: as above
			return; // RA2: as above
		}
		file.Seek(0, SEEK_SET); // RA2: as above
		file.Read(head, size); // RA2: as above
		head[size] = '\0'; // RA2: as above
		char * start = strstr(head, "[Basic]"); // RA2: as above
		if (start) { // RA2: as above
			end = strstr(start + 5, "\n["); // RA2: as above
			if (end > start) { // RA2: as above
				break; // RA2: as above
			}
		}
		if (percent >= 100) { // RA2: as above
			delete head; // RA2: as above
			return; // RA2: as above
		}
	} while (true); // RA2: as above
	end[1] = '\0'; // RA2: as above
	DebugString("Read first %d%% of file looking for [Basic] section\n", percent); // RA2: as above

	percent = 0; // RA2: as above
	char * tail = NULL; // RA2: as above
	int length; // RA2: as above
	do { // RA2: as above
		percent++; // RA2: reads 1% steps from the end of the file until the [Digest] section is complete
		length = file.Size() * percent / 100; // RA2: as above
		if (tail) { // RA2: as above
			delete tail; // RA2: as above
		}
		tail = new char[length + 1]; // RA2: as above
		if (!tail) { // RA2: as above
			return; // RA2: as above
		}
		file.Seek(-length, SEEK_END); // RA2: as above
		file.Read(tail, length); // RA2: as above
		tail[length] = '\0'; // RA2: as above
		if (strstr(tail, "[Digest]")) { // RA2: as above
			break; // RA2: as above
		}
		if (percent >= 100) { // RA2: as above
			delete head; // RA2: as above
			delete tail; // RA2: as above
			return; // RA2: as above
		}
	} while (true); // RA2: as above
	DebugString("Read last  %d%% of file looking for [Digest] section\n", percent); // RA2: as above

	CCINIClass ini; // RA2: as above
	BufferStraw straw(head, end - head); // RA2: as above
	ini.Load(straw, false, false); // RA2: as above
	CCINIClass digest_ini; // RA2: as above
	BufferStraw digest_straw(tail, strlen(tail) + 1); // RA2: as above
	digest_ini.Load(digest_straw, false, false); // RA2: as above

	int min = ini.Get_Int("Basic", "MinPlayers", *min_players); // RA2: as above
	*min_players = min; // RA2: as above
	int max = ini.Get_Int("Basic", "MaxPlayers", *max_players); // RA2: as above
	*max_players = max; // RA2: as above
	ini.Get_String("Basic", "GameMode", "", game_modes, 0x100); // RA2: as above
	char name[0x40]; // RA2: as above
	ini.Get_String("Basic", "Name", "No Name", name, 0x40); // RA2: as above
	Char_To_Wide(description, name, -1); // RA2: as above
	digest_ini.Get_String("Digest", "1", "No Digest", digest, 0x20); // RA2: as above
	*official = ini.Get_Bool("Basic", "Official", false); // RA2: as above
	DebugString("MPR %s (%d/%d): official: %d, desc: %s\n", filename, *min_players, *max_players, *official, name); // RA2: as above
	DebugString("\t\tGameModes: %s\n", game_modes); // RA2: as above
	delete head; // RA2: as above
	delete tail; // RA2: as above
}


/***************************************************************************
 * SessionClass::Read_Scenario_Descriptions -- reads scen. descriptions    *
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
 *   02/14/1995 BR : Created.                                              *
 *   09/10/1996 JLB : Searches using different method.                     *
 *=========================================================================*/
void SessionClass::Read_Scenario_Descriptions(void)
{
	INIClass ini;
	char name_buffer[64];
	WIN32_FIND_DATA block;
	HANDLE handle; // RA2: RA2's form of SessionClass::Read_Scenario_Descriptions

	Scenarios.Clear();

	/*
	**	Fetch the main multiplayer scenario packet data.
	*/
	CCFileClass file("MISSIONS.PKT"); // RA2: as above
	if (file.Is_Available()) {
		ini.Load(file, false); // RA2: as above
		Add_Missions(Scenarios, ini, name_buffer); // RA2: as above
	}
	ini.Clear();
	file.Close();

	handle = FindFirstFile("*.PKT", &block); // RA2: as above
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) { // RA2: as above
			char const * name = &block.cAlternateFileName[0];
			if (strlen(name) == 0) { // RA2: as above
				name = &block.cFileName[0]; // RA2: as above
			}

	/*
	**	Fetch any scenario packet lists and apply them first.
	*/
			if (stricmp(name, "MISSIONS.PKT")) { // RA2: as above
				file.Set_Name(name);
				ini.Load(file, false); // RA2: as above
				Add_Missions(Scenarios, ini, name_buffer); // RA2: as above
			}
		}

		if (!Next_File(handle, block)) { // RA2: as above
			break;
		}
	}

	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}

	ini.Clear();
	file.Close();

	handle = FindFirstFile("*.MMX", &block); // RA2: as above
	int previous_disk = CD::Get_Required_Disk(); // RA2: as above
	CD::Set_Required_Disk((DiskID)-2); // RA2: as above
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) { // RA2: as above
			char const * name = &block.cAlternateFileName[0];
			if (strlen(name) == 0) { // RA2: as above
				name = &block.cFileName[0]; // RA2: as above
			}

	/*
	**	Scan the current directory for any loose .MPR files and build the appropriate entries
	**	into the scenario list list
	*/
			if (stricmp(name, "MISSIONS.MMX")) { // RA2: as above
				MixFileClass * mix = new MixFileClass(name, &FastKey); // RA2: as above
				if (mix != NULL) { // RA2: as above
					ScenarioMixFiles.Add(mix); // RA2: as above

					char pkt_name[260]; // RA2: as above
					int length = strlen(name); // RA2: as above
					strncpy(pkt_name, name, length - 3); // RA2: as above
					pkt_name[length - 3] = 0; // RA2: as above
					strcat(pkt_name, "PKT"); // RA2: as above
					pkt_name[length] = 0; // RA2: as above
					file.Set_Name(pkt_name); // RA2: as above
					if (file.Is_Available()) {
						ini.Load(file, false); // RA2: as above

						int count = ini.Entry_Count("MultiMaps");
						for (int index = 0; index < count; index++) {
							if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), "", name_buffer, sizeof(name_buffer))) {
								MultiMission * mission = new MultiMission(ini, name_buffer); // RA2: as above
								if (mission != NULL) { // RA2: as above
									wchar_t players[64]; // RA2: as above
									wchar_t text[44]; // RA2: as above
									int max_count = mission->MaxPlayers; // RA2: as above
									int min_count = mission->MinPlayers; // RA2: as above
									if (min_count != max_count) { // RA2: as above
										swprintf(players, L"%c%d-%d%c", L'(', min_count, max_count, L')'); // RA2: as above
									} else {
										swprintf(players, L"%c%d%c", L'(', min_count, L')'); // RA2: as above
									}
									int length = wcslen(mission->ScenarioDescription); // RA2: as above
									int & used = length; // RA2: as above
									wcscpy(text, mission->ScenarioDescription); // RA2: as above
									wcsncat(text, L" ", 44 - used); // RA2: as above
									used++; // RA2: as above
									wcsncat(text, players, 44 - used); // RA2: as above
									mission->Set_Description(text); // RA2: as above
									Scenarios.Add(mission); // RA2: as above
								}
							}
						}
						ini.Clear();
					} else {
						DebugString("Can't see .pkt inside .mmx!\n"); // RA2: as above
					}
				}
			}
		}

		if (!Next_File(handle, block)) { // RA2: as above
			break;
		}
	}

	CD::Set_Required_Disk((DiskID)previous_disk); // RA2: as above
	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}

	ini.Clear();
	file.Close();

	char digest_buffer[32];
	wchar_t description[64]; // RA2: as above
	char game_modes[256]; // RA2: as above
	bool official; // RA2: as above
	int min_players; // RA2: as above
	int max_players; // RA2: as above

	handle = FindFirstFile("*.MPR", &block); // RA2: as above
	official = false; // RA2: as above
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) { // RA2: as above
			char const * file_name = &block.cAlternateFileName[0]; // RA2: as above
			if (strlen(file_name) == 0) { // RA2: as above
				file_name = &block.cFileName[0]; // RA2: as above
			}

			ini.Get_String("Basic", "Name", "No Name", name_buffer, sizeof(name_buffer)); // RA2: as above
			ini.Get_String("Digest", "1", "No Digest", digest_buffer, sizeof(digest_buffer)); // RA2: as above
			Char_To_Wide(description, name_buffer, -1); // RA2: as above
			official = ini.Get_Bool("Basic", "Official", false); // RA2: as above
			min_players = 2; // RA2: as above
			max_players = 4; // RA2: as above

			Read_Map_Header(file_name, description, digest_buffer, game_modes, &official, &min_players, &max_players); // RA2: as above
			MultiMission * mission = new MultiMission(file_name, description, digest_buffer, official, game_modes, min_players, max_players); // RA2: as above
			Scenarios.Add(mission); // RA2: as above
		}

		if (!Next_File(handle, block)) { // RA2: as above
			break;
		}
	}

	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}
}



/***************************************************************************
 * SessionClass::Free_Scenario_Descriptions -- frees scen. descriptions    *
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
 *   06/05/1995 BRR : Created.                                             *
 *=========================================================================*/
void SessionClass::Free_Scenario_Descriptions(void)
{
	int i;

	//------------------------------------------------------------------------
	// Clear the scenario descriptions & filenames
	//------------------------------------------------------------------------
	for (int index = 0; index < Scenarios.Count(); index++) {
		delete Scenarios[index];
	}
	Scenarios.Clear();
//	Filenum.Clear();

	//------------------------------------------------------------------------
	// Clear the initstring entries
	//------------------------------------------------------------------------
	for (i = 0; i < InitStrings.Count(); i++) {
		delete [] InitStrings[i];
	}
	InitStrings.Clear();

	//------------------------------------------------------------------------
	// Clear the dialing entries
	//------------------------------------------------------------------------
    for (i = 0; i < PhoneBook.Count(); i++) {
        delete PhoneBook[i];
    }
    PhoneBook.Clear();
}



/***************************************************************************
 * SessionClass::Trap_Object -- searches for an object, for debugging      *
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
 *   06/02/1995 BRR : Created.                                             *
 *=========================================================================*/
void SessionClass::Trap_Object(void)
{
}


/// <summary>
/// Converts a multiplayer color choice into a color scheme.
/// This routine turns the color a player picked in the lobby into the scheme their units
/// are actually drawn with. A choice that is not one of the multiplayer colors is passed
/// back untouched.
/// </summary>
/// <param name="id">The multiplayer color the player has chosen.</param>
/// <returns>Returns with the color scheme to draw that player with.</returns>
static char const ColorSchemes[9] = {3, 11, 21, 29, 13, 25, 17, 15, 5}; // RA2: 0x7F3224, the color scheme of each player color

int SessionClass::Color_Index_To_Scheme(int color) // RA2: RA2's signature
{
	/*
	 * The list holds each scheme twice, and the odd entry of each pair is the lighting-aware
	 * one, so the index is doubled and stepped one past the plain copy.
	 */
    if (color == -2) { // RA2: RA2's form of SessionClass::Color_Index_To_Scheme
        color = 8; // RA2: as above
    }
    if ((unsigned)color < 9) { // RA2: as above
        return ColorSchemes[color]; // RA2: as above
    }
    return color; // RA2: as above
}



/***************************************************************************
 * SessionClass::Compute_Unique_ID -- computes unique local ID number      *
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
 *   12/07/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned int SessionClass::Compute_Unique_ID(void)
{
//	time_t tm;
    unsigned int id = timeGetTime(); // RA2: RA2's form of SessionClass::Compute_Unique_ID
    unsigned int free = Disk_Space_Available(); // RA2: as above
    Add_CRC(&id, free); // RA2: as above
    char *path = getenv("PATH"); // RA2: as above
    if (path != 0) { // RA2: as above
        for (unsigned i = 0; i < strlen(path); i++) { // RA2: as above
            Add_CRC(&id, path[i]); // RA2: as above
        }
    }
    return id; // RA2: as above
}



/// <summary>
/// Constructs a mission from an entry in the mission list.
/// The description, player limits and disc list come out of the section named, while the
/// digest is fetched from the tail of the map file so that the other players' copies can be
/// checked against it. A map that cannot be located is still listed, merely without a
/// digest to its name.
/// </summary>
/// <param name="ini">The INI database that holds the mission list.</param>
/// <param name="name">Name of the section that describes this mission.</param>
MultiMission::MultiMission(INIClass const & ini, char const * name)
{
    ScenarioDescription[0] = 0; // RA2: RA2's form of MultiMission::MultiMission
    Filename[sizeof(Filename) - 1] = '\0'; // RA2: as above
    strcpy(Digest, "No Digest");
    IsOfficial = true;
    MinPlayers = 2;
    MaxPlayers = 4;
    Disks.Clear();
    if (!name) { // RA2: as above
        return;
    }
    strcpy(Filename, name);
    strcat(Filename, ".MAP");
    if (ini.Is_Present(name, "DescriptionText")) { // RA2: as above
        char text[0x2C]; // RA2: as above
        ini.Get_String(name, "DescriptionText", "", text, 0x2C); // RA2: as above
        Char_To_Wide(ScenarioDescription, text, -1); // RA2: as above
    } else {
        ini.ReadStringtableEntry(name, "Description", ScenarioDescription, 0x2C); // RA2: as above
    }
    MinPlayers = ini.Get_Int(name, "MinPlayers", MinPlayers);
    MaxPlayers = ini.Get_Int(name, "MaxPlayers", MaxPlayers); // RA2: as above
    char disks[0x80]; // RA2: as above
    if (ini.Get_String(name, "CD", "", disks, 0x80)) { // RA2: as above
        char *token = strtok(disks, ","); // RA2: as above
        while (token && *token) { // RA2: as above
            Disks.Add((DiskID)atoi(token));
            token = strtok(0, ","); // RA2: as above
        }
    }
    Wstring modes; // RA2: as above
    bool found = ini.Get_String(name, "GameMode", "", modes) > 0; // RA2: as above
    if (found) { // RA2: as above
        Wstring token; // RA2: as above
        int position = modes.getToken(0, ",", token); // RA2: as above
        while (position != -1) { // RA2: as above
            token.trimLeading(" "); // RA2: as above
            token.trimTrailing(" "); // RA2: as above
            GameModes.Add(token); // RA2: as above
            position = modes.getToken(position, ",", token); // RA2: as above
        }
    }
    CCFileClass file(Filename);
    if (!file.Is_Available()) {
        DebugString("Unable to locate scenario %s - No digest info will be loaded\n", Filename);
    } else {
        if (!file.Open(FileClass::READ)) { // RA2: as above
            return;
        }
        file.Seek(-100, 2); // RA2: as above
        char tail[0x80]; // RA2: as above
        memset(tail, 0, sizeof(tail)); // RA2: as above
        file.Read(tail, 0x64); // RA2: as above
        file.Close();
        if (strstr(tail, "[Digest]")) { // RA2: as above
            CCINIClass digest_ini; // RA2: as above
            BufferStraw straw(tail, 0x64); // RA2: as above
            digest_ini.Load(straw, false, false); // RA2: as above
            digest_ini.Get_String("Digest", "1", 0, tail, 0x80); // RA2: as above
            if (tail) { // RA2: as above
                strncpy(Digest, tail, 0x20); // RA2: as above
                Digest[0x1F] = '\0'; // RA2: as above
            } else {
                strcpy(Digest, "No Digest");
            }
        }
    }
}



/// <summary>
/// Constructs a mission from a map file found on disk.
/// The map itself is consulted for a better description, for the player limits and for the
/// discs it expects to be found on. A map that says nothing about discs is presumed to live
/// on the hard drive.
/// </summary>
/// <param name="filename">The map file this mission is played from.</param>
/// <param name="description">The description to show in the scenario list.</param>
/// <param name="digest">The digest the other players' copies are checked against.</param>
/// <param name="official">Is this one of the missions that shipped with the game?</param>
MultiMission::MultiMission(char const * filename, wchar_t const * description, char const * digest, bool official, // RA2: RA2's signature
                           char const * game_modes, int min_players, int max_players) // RA2: RA2's form of MultiMission::MultiMission
{
    if (filename) { // RA2: as above
        strncpy(Filename, filename, sizeof(Filename)); // RA2: as above
        Filename[sizeof(Filename) - 1] = '\0'; // RA2: as above
    } else {
        strcpy(Filename, "No File Name");
    }
    if (description) { // RA2: as above
        wcsncpy(ScenarioDescription, description, 0x2C); // RA2: as above
        ScenarioDescription[0x2B] = 0; // RA2: as above
    } else {
        wcscpy(ScenarioDescription, StringTable::LoadString("MSG:NoDescription", 0, "D:\\Ra2\\Session.CPP", 0xb0a)); // RA2: as above
    }
    if (digest) { // RA2: as above
        strncpy(Digest, digest, sizeof(Digest)); // RA2: as above
        Digest[sizeof(Digest) - 1] = '\0'; // RA2: as above
    } else {
        strcpy(Digest, "No Digest");
    }
    IsOfficial = official;
    MinPlayers = min_players; // RA2: as above
    MaxPlayers = max_players; // RA2: as above
    Disks.Clear();
    Disks.Add((DiskID)-2); // RA2: as above
    if (game_modes) { // RA2: as above
        Wstring list(game_modes); // RA2: as above
        if (list.length() > 0) { // RA2: as above
            Wstring token; // RA2: as above
            int position = list.getToken(0, ",", token); // RA2: as above
            while (position != -1) { // RA2: as above
                token.trimLeading(" "); // RA2: as above
                token.trimTrailing(" "); // RA2: as above
                GameModes.Add(token); // RA2: as above
                position = list.getToken(position, ",", token); // RA2: as above
            }
        }
    }
}


/// <summary>
/// Can this mission be played from the specified disk?
/// This routine is used when deciding whether a scenario may be offered to the player,
/// since a multiplayer map might live on the hard drive or on either of the game discs.
/// </summary>
/// <param name="cd">The disk to look for the mission on.</param>
/// <returns>bool; Is the mission reachable from that disk?</returns>
bool MultiMission::Is_On_CD(DiskID cd) const
{
	if (cd == DISK_LOCAL) {
		return RawFileClass(Filename).Is_Available(); // RA2: RA2's form of MultiMission::Is_On_CD
	}

	if (Disks.Is_In_List(DISK_ANY) == true) {
		return true; // RA2: as above
	}
	return Disks.Is_In_List(cd); // RA2: as above
}


/// <summary>
/// Fetches the disk this mission should be looked for on.
/// </summary>
/// <returns>Returns with the first disk the mission was registered against.</returns>
DiskID MultiMission::On_Which_CD(void) const
{
	return Disks[0]; // RA2: RA2's form of MultiMission::On_Which_CD
}


/// <summary>
/// Sets the description of this mission.
/// This is the text the player is shown in the scenario list, so a mission that arrives
/// without one is given a placeholder rather than an empty line.
/// </summary>
/// <param name="description">The description to record, or NULL if the mission has none.</param>
void MultiMission::Set_Description(wchar_t const * description) // RA2: RA2's signature
{
	if (description != NULL) {
		wcsncpy(ScenarioDescription, description, ARRAY_SIZE(ScenarioDescription)); // RA2: copies 44 and ends the string at 43
		ScenarioDescription[ARRAY_SIZE(ScenarioDescription) - 1] = L'\0'; // RA2: RA2's form of MultiMission::Set_Description
	} else {
		wcscpy(ScenarioDescription, StringTable::LoadString("MSG:NoDescription", NULL, "D:\\Ra2\\Session.CPP", 2826)); // RA2: the placeholder text
	}
}



/// <summary>
/// Sets the map filename for this mission.
/// </summary>
/// <param name="filename">The filename to record, or NULL if the mission has none.</param>
void MultiMission::Set_Filename(char const * filename)
{
	if (filename != NULL) {
		strncpy(Filename, filename, ARRAY_SIZE(Filename));
		Filename[ARRAY_SIZE(Filename) - 1] = '\0';
	} else {
		strcpy(Filename, "No File Name");
	}
}


/// <summary>
/// Sets the digest for this mission.
/// The digest is what the other players' copies of the map are checked against, so a
/// mission that arrives without one is given a placeholder rather than a blank.
/// </summary>
/// <param name="digest">The digest to record, or NULL if the mission has none.</param>
void MultiMission::Set_Digest(char const * digest)
{
	if (digest != NULL) {
		strncpy(Digest, digest, ARRAY_SIZE(Digest));
		Digest[ARRAY_SIZE(Digest) - 1] = '\0';
	} else {
		strcpy(Digest, "No Digest");
	}
}


/// <summary>
/// Sets whether this is an official mission.
/// </summary>
/// <param name="official">Is this one of the missions that shipped with the game?</param>
void MultiMission::Set_Official(bool official)
{
	IsOfficial = official;
}
bool MultiMission::Is_Random_Map(void) const // RA2: RA2's signature
{
	return stricmp(Filename, "RandMap.Sed") == 0; // RA2: 0x006720C0
}

bool MultiMission::Supports_Game_Mode(Wstring & mode) const // RA2: RA2's signature
{
	if (GameModes.Count() == 0) { // RA2: 0x006720E0
		return mode == "standard"; // RA2: RA2's form of MultiMission::Supports_Game_Mode
	}
	for (int index = 0; index < GameModes.Count(); index++) { // RA2: as above
		if (mode == GameModes[index]) { // RA2: as above
			return true; // RA2: as above
		}
	}
	return false; // RA2: as above
}

bool SessionClass::Is_Random_Map(void) const // RA2: RA2's signature
{
	return stricmp(ScenarioFileName, "RandMap.Sed") == 0; // RA2: 0x00672140
}












/// <summary>
/// RA2
/// </summary>
static inline void Copy_Player_Name(char * destination, wchar_t const * name)
{
	strcpy(destination, Wide_To_Narrow(name));
}


/// <summary>
/// Updates the scenario loading progress and reports it to the other players.
/// The local gauge only ever moves forward, and a random map is allowed half of it since
/// generating the map is only half of the job. In a multiplayer game the new figure is sent
/// out so the other machines can see how far behind we are.
/// </summary>
/// <param name="percent">How much of the load has been completed, as a percentage.</param>
void SessionClass::Update_Progress(int percent)
{
	if (Debug_Map) { // RA2: RA2's form of SessionClass::Update_Progress
		return;
	}

	int new_percent = percent;
	if (Scen->IsRandom) {
		new_percent = new_percent / 2;
	}

	if (Progress.Get_Current_Progress(0) * 100.0 < new_percent) {
		Progress.Set_Progress_Percent(0, new_percent);
	}

	if (Players.Count() == 1) { // RA2: as above
		return;
	}

	CDTimerClass<SystemTimerClass> timer = 240; // RA2: as above

	switch (Type) {
		case GAME_IPX:
		case GAME_INTERNET: {
				GlobalPacketType prog_packet;
				memset((void *)&prog_packet, 0, sizeof(prog_packet));

				prog_packet.Command = NET_PROGRESS_REPORT;
				prog_packet.Progress.Percent = int(100.0 * Progress.Get_Current_Progress(0));

				strcpy(prog_packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above

				if (prog_packet.Progress.Percent < 99.95) {
					Ipx.Send_Global_Message(&prog_packet, 0x33, 0, NULL); // RA2: as above
				} else {
					for (int p = 1; p < Session.Players.Count(); p++) { // RA2: as above
						DebugString("Sending guarenteed progress message to all players\n"); // RA2: as above
						Ipx.Send_Global_Message_To_Name(&prog_packet, 0x33, 1, Session.Players[p]->Name); // RA2: as above
					}
				}

				Call_Back();

				while (Ipx.Global_Num_Send() > 5 && timer > 0) {
					Sleep(20);
					Windows_Message_Handler();
					Call_Back();
				}
			}
			break;

		case GAME_MODEM:
		case GAME_NULL_MODEM: {
				SerialPacketType send_packet;
				memset((void *)&send_packet, 0, sizeof(send_packet));

				send_packet.Command = SERIAL_PROGRESS_REPORT;
				send_packet.Progress.Percent = int(100.0 * Progress.Get_Current_Progress(0));
				send_packet.ID = Session.ModemType;

				NullModem.Send_Message(&send_packet, sizeof(send_packet), 0);
				NullModem.Service();

				while (NullModem.Num_Send() > 5 && timer > 0) {
					Sleep(20);
					Keyboard->Check();
					NullModem.Service();
				}
			}
			break;

		default:
			break;
	}

	Call_Back();
}


/// <summary>
/// Sets up the fixed alliances for a clan game.
/// Westwood Online hands down the two squad rosters, and this routine allies the players
/// within each squad and then fixes the alliances so that nobody can change sides for the
/// rest of the match. An ordinary game is left to sort out its own alliances.
/// </summary>
void SessionClass::Init_Fixed_Alliances(void)
{
    SquadAlliances = false;
    if (Type != GAME_INTERNET) {
        return;
    }
    Scen->Special.IsAllianceFixed = false;
    DebugString("Squad 1 = %s\n", Squad1Names); // RA2: RA2's form of SessionClass::Init_Fixed_Alliances
    DebugString("Squad 2 = %s\n", Squad2Names); // RA2: as above
    if (strlen(Squad1Names) == 0 || strlen(Squad2Names) == 0) { // RA2: as above
        return;
    }
    int i;
    int j;
    char *name; // RA2: as above
    DynamicVectorClass<HouseClass *> squad1;
    DynamicVectorClass<HouseClass *> squad2;
    char *names = new char[(strlen(Squad1Names) > strlen(Squad2Names) ? strlen(Squad1Names) : strlen(Squad2Names)) + 0x20]; // RA2: as above
    strcpy(names, Squad1Names); // RA2: as above
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) { // RA2: as above
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i]; // RA2: as above
            if (house->IsHuman && stricmp(house->IniName, name) == 0) { // RA2: as above
                squad1.Add(house); // RA2: as above
            }
        }
    }
    strcpy(names, Squad2Names); // RA2: as above
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) { // RA2: as above
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i]; // RA2: as above
            if (house->IsHuman && stricmp(house->IniName, name) == 0) { // RA2: as above
                squad2.Add(house); // RA2: as above
            }
        }
    }
    if (squad1.Count() > 1) {
        for (i = 0; i < squad1.Count(); i++) { // RA2: as above
            for (j = 0; j < squad1.Count(); j++) {
                if (squad1[i]->HeapID != squad1[j]->HeapID) {
                    squad1[i]->Make_Ally(squad1[j]);
                }
            }
        }
    }
    if (squad2.Count() > 1) {
        for (i = 0; i < squad2.Count(); i++) { // RA2: as above
            for (j = 0; j < squad2.Count(); j++) {
                if (squad2[i]->HeapID != squad2[j]->HeapID) {
                    squad2[i]->Make_Ally(squad2[j]);
                }
            }
        }
    }
    delete [] names; // RA2: as above
    SquadAlliances = true;
    Scen->Special.IsAllianceFixed = true;
}

void GameOptionsType::Reset_AI(void) // RA2: RA2's signature
{
	AIPlayers = 0; // RA2: 0x00672810
	for (int index = 0; index < 8; index++) { // RA2: RA2's form of GameOptionsType::Reset_AI
		AISlots.Countries[index] = -1; // RA2: as above
		AISlots.Colors[index] = -1; // RA2: as above
	}
}






/// <summary>
/// Saves the game options to a save game.
/// The block is written with its own size in front of it, so that a later version of the
/// game can recognize an incompatible save rather than misread it.
/// </summary>
/// <returns>bool; Were the options written successfully?</returns>
bool GameOptionsType::Save(IStream * stream) const
{
	if (stream != NULL) {
		unsigned int size = sizeof(*this);
		HRESULT res = stream->Write(&size, sizeof(size), NULL);
		if (SUCCEEDED(res)) {
			res = stream->Write(this, size, NULL);
			if (SUCCEEDED(res)) {
				return(true);
			}
		}
	}

	return(false);
}


/// <summary>
/// Loads the game options from a save game.
/// A block of a different size belongs to another version of the game and is refused
/// rather than read. The scenario index is invalidated on the way in, since the saved game
/// brings its own scenario with it.
/// </summary>
/// <returns>bool; Were the options read back successfully?</returns>
bool GameOptionsType::Load(IStream * stream)
{
	if (stream != NULL) {
		unsigned int size;
		HRESULT res = stream->Read(&size, sizeof(size), NULL);
		if (SUCCEEDED(res)) {
			if (size == sizeof(*this)) {
				res = stream->Read(this, size, NULL);
				ScenarioIndex = -1;
				if (SUCCEEDED(res)) {
					return(true);
				}
			}
		}
	}

	return(false);
}
bool SessionClass::Is_Color_Used(int color) // RA2: RA2's signature
{
	if (color == -2) { // RA2: 0x006728D0
		return false; // RA2: RA2's form of SessionClass::Is_Color_Used
	}
	for (int index = 0; index < Players.Count(); index++) { // RA2: as above
		if (Players[index]->Get_Color_Or_Random() == color) { // RA2: as above
			return true; // RA2: as above
		}
	}
	for (int slot = 0; slot < 8; slot++) { // RA2: as above
		if (Options.AISlots.Colors[slot] == color) { // RA2: as above
			return true; // RA2: as above
		}
	}
	return false; // RA2: as above
}

int SessionClass::Random_Side(void) // RA2: RA2's signature
{
	int last = HouseTypes.Count() - 1; // RA2: 0x00672940, a random house that is allowed in multiplayer and is not passive
	int index;
	do { // RA2: RA2's form of SessionClass::Random_Side
		index = Scen->RandomNumber(0, last); // RA2: as above
	} while (!HouseTypes[index]->IsMultiplay || HouseTypes[index]->IsMultiplayPassive); // RA2: as above
	return index; // RA2: as above
}

int SessionClass::Random_Color(void) // RA2: RA2's signature
{
	int color; // RA2: 0x00672980
	do { // RA2: RA2's form of SessionClass::Random_Color
		color = Scen->RandomNumber(0, 7); // RA2: as above
	} while (Is_Color_Used(color)); // RA2: as above
	return color; // RA2: as above
}

int SessionClass::Chosen_House(void) const // RA2: RA2's signature
{
	return (idxSide2 == -2) ? -2 : House; // RA2: 0x006729F0
}

int SessionClass::Chosen_Color(void) const // RA2: RA2's signature
{
	return (ColorIdx == -2) ? -2 : PrefColor; // RA2: 0x00672A10
}

void SessionClass::Set_Side(int side, bool random) // RA2: RA2's signature
{
	if (side == -2) { // RA2: 0x00672A30
		if (random) { // RA2: RA2's form of SessionClass::Set_Side
			Side = Random_Side(); // RA2: as above
		} else {
			Side = 0; // RA2: as above
		}
		Side2 = -2; // RA2: as above
	} else {
		Side = side; // RA2: as above
		Side2 = -1; // RA2: as above
	}
	House = Side; // RA2: as above
	idxSide2 = Side2; // RA2: as above
}

void SessionClass::Set_Color(int color, bool random) // RA2: RA2's signature
{
	if (color == -2) { // RA2: 0x00672AE0
		if (random) { // RA2: RA2's form of SessionClass::Set_Color
			Color = Random_Color(); // RA2: as above
		} else {
			Color = 0; // RA2: as above
		}
		Color2 = -2; // RA2: as above
	} else {
		Color = color; // RA2: as above
		Color2 = -1; // RA2: as above
	}
	PrefColor = Color; // RA2: as above
	ColorIdx = Color2; // RA2: as above
}

void SessionClass::Assign_Random_Choices(void) // RA2: RA2's signature
{
	int count = Session.Players.Count(); // RA2: 0x00672BB0, settles every random country and color of the players and the computer players
	int i;
	for (i = 0; i < count; i++) { // RA2: RA2's form of SessionClass::Assign_Random_Choices
		NodeNameType * player = Session.Players[i]; // RA2: as above
		if (player->Slot.InitialCountry == -3 || player->Slot.Country == -3) { // RA2: as above
			player->Slot.Country = -3; // RA2: as above
			player->Slot.InitialCountry = -1; // RA2: as above
			player->Slot.ColorIdx = 8; // RA2: as above
			player->Slot.InitialColor = -1; // RA2: as above
		} else {
			if (player->Slot.InitialCountry == -2) { // RA2: as above
				player->Slot.InitialCountry = -1; // RA2: as above
				if (MPGameMode) { // RA2: as above
					player->Slot.Country = MPGameMode->RandomHumanCountryIndex(); // RA2: as above
				} else {
					player->Slot.Country = Session.Random_Side(); // RA2: as above
				}
			}
			if (player->Slot.InitialColor == -2) { // RA2: as above
				player->Slot.InitialColor = -1; // RA2: as above
				player->Slot.ColorIdx = -1; // RA2: as above
				player->Slot.ColorIdx = Session.Random_Color(); // RA2: as above
			}
		}
		if (i == 0) { // RA2: as above
			Game::PlayerColor = player->Slot.ColorIdx; // RA2: as above
		}
	}
	for (i = 0; i < 8; i++) { // RA2: as above
		if (Session.Options.AISlots.Countries[i] == -2) { // RA2: as above
			if (MPGameMode) { // RA2: as above
				Session.Options.AISlots.Countries[i] = MPGameMode->RandomAICountryIndex(); // RA2: as above
			} else {
				Session.Options.AISlots.Countries[i] = Session.Random_Side(); // RA2: as above
			}
		}
		if (Session.Options.AISlots.Colors[i] == -2) { // RA2: as above
			Session.Options.AISlots.Colors[i] = Session.Random_Color(); // RA2: as above
		}
	}
}

void SessionClass::Resume(void) // RA2: RA2's signature
{
	if (!CurrentlyInGame && GameActive) { // RA2: 0x00672DE0
		CurrentlyInGame = true; // RA2: RA2's form of SessionClass::Resume
		if (SavedGameSpeed == -1) { // RA2: as above
			SavedGameSpeed = ::Options.GameSpeed; // RA2: as above
		}
		if (Type == GAME_NORMAL) { // RA2: as above
			::Options.GameSpeed = 2; // RA2: as above
		}
		if (!Scen->IsInputLocked) { // RA2: as above
			Lock_Scenario_Input(); // RA2: as above
			UnlockInputFrame = Frame + 3; // RA2: as above
		}
		if (!MouseCursor->Is_Captured()) { // RA2: as above
			MouseCursor->Capture_Mouse(); // RA2: as above
		}
		AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, 0x4000); // RA2: as above
	}
}

void SessionClass::Leave_Game(void) // RA2: RA2's signature
{
	if (CurrentlyInGame) { // RA2: 0x00672E70
		CurrentlyInGame = false; // RA2: RA2's form of SessionClass::Leave_Game
		if (SavedGameSpeed != -1) { // RA2: as above
			::Options.GameSpeed = SavedGameSpeed; // RA2: as above
			SavedGameSpeed = -1; // RA2: as above
		}
		Leave_Game_Hook(); // RA2: as above
		Stop_Ingame_Movie(); // RA2: as above
		Audio_Stop_Events(); // RA2: as above
		Sound_Clear_Static(); // RA2: as above
		Speech_Reset(); // RA2: as above
		Aux_Stream_Stop(); // RA2: as above
		AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, 0x4000); // RA2: as above
		if (ToolTips) { // RA2: as above
			ToolTips->Activate(false); // RA2: as above
		}
		if (MouseCursor->Is_Captured()) { // RA2: as above
			MouseCursor->Release_Mouse(); // RA2: as above
		}
		MouseCursor->Hide_Mouse(); // RA2: as above
		UnlockInputFrame = 0; // RA2: as above
	}
}

bool SessionClass::Is_In_Game(void) const // RA2: RA2's signature
{
	return CurrentlyInGame; // RA2: 0x00672F10
}

void SessionClass::Remove_Player_Port(int index) // RA2: RA2's signature
{
	for (int i = index; i < MAX_PLAYERS - 1; i++) { // RA2: 0x00672F20
		PlayerPorts[i] = PlayerPorts[i + 1]; // RA2: RA2's form of SessionClass::Remove_Player_Port
	}
}

unsigned short SessionClass::Player_Port(int connection_id) // RA2: RA2's signature
{
	int count = Session.Players.Count(); // RA2: 0x00672F50
	for (int i = 0; i < count; i++) { // RA2: RA2's form of SessionClass::Player_Port
		if (Session.Players[i]->Player.ID == connection_id) { // RA2: as above
			return PlayerPorts[i]; // RA2: as above
		}
	}
	return 0; // RA2: as above
}

int SessionClass::Player_To_Slot(int index) // RA2: RA2's signature
{
	int i = 0; // RA2: 0x00672F90
	NodeNameType * node; // RA2: RA2's form of SessionClass::Player_To_Slot
	char * a; // RA2: as above
	char * b; // RA2: as above

	for (NodeNameType ** slot = Session.Slots; // RA2: as above
		 (int)slot < (int)&Session.Slots[8]; // RA2: as above
		 i++, slot++) { // RA2: as above
		node = *slot; // RA2: as above
		a = Wide_To_Narrow(Session.Players[index]->Name); // RA2: as above
		b = Wide_To_Narrow(node->Name); // RA2: as above
		if (stricmp(a, b) == 0) { // RA2: as above
			return i; // RA2: as above
		}
	}
	return 0; // RA2: as above
}

int SessionClass::Name_To_Slot(char const * name) // RA2: RA2's signature
{
	int i = 0; // RA2: 0x00672FF0
	for (NodeNameType ** slot = Session.Slots; (int)slot < (int)&Session.Slots[8]; slot++, i++) { // RA2: RA2's form of SessionClass::Name_To_Slot
		if (stricmp(name, Wide_To_Narrow((*slot)->Name)) == 0) { // RA2: as above
			return i; // RA2: as above
		}
	}
	return 0; // RA2: as above
}

int SessionClass::Slot_To_Player(int slot) // RA2: RA2's signature
{
	for (int i = 0; i < Session.Players.Count(); i++) { // RA2: 0x00673030
		NodeNameType * node = Session.Slots[slot]; // RA2: RA2's form of SessionClass::Slot_To_Player
		char * a = Wide_To_Narrow(Session.Players[i]->Name); // RA2: as above
		if (stricmp(a, Wide_To_Narrow(node->Name)) == 0) { // RA2: as above
			return i; // RA2: as above
		}
	}
	return 0; // RA2: as above
}


void SessionPortAllocator::Allocate_Player_Ports(void) // RA2: 0x00673090, SessionClass::Allocate_Player_Ports; a member of this file-local subclass because declaring it in session.h unmatches wonline Draw_Player_List
{
	int player; // RA2: as above
	unsigned short port_offset = 1; // RA2: as above
	DynamicVectorClass<NodeNameType *> ordered; // RA2: as above

	DebugString("Allocating port numbers and local addresses\n"); // RA2: as above
	if (Players.Count() < 2) { // RA2: as above
		return; // RA2: as above
	}

	int local_count = 0; // RA2: as above
	memset(NatPeerNames, 0, sizeof(NatPeerNames)); // RA2: as above
	RotatePortBase = true; // RA2: as above

	for (player = 0; player < Players.Count(); player++) { // RA2: as above
		int slot = Player_To_Slot(player); // RA2: as above
		if (slot >= 0 && slot < MAX_PLAYERS) { // RA2: as above
			Players[player]->Address.Set_Port(PlayerPortBase[slot]); // RA2: as above
			char * name = Wide_To_Narrow(Players[player]->Name); // RA2: as above
			char const * address = Players[player]->Address.As_String(NULL); // RA2: as above
			DebugString("Allocated port for player %s - %s\n", name, address); // RA2: as above
		}
	}

	bool used[MAX_PLAYERS]; // RA2: as above
	memset(used, 0, sizeof(used)); // RA2: as above

	for (int i = 0; i < Players.Count(); i++) { // RA2: as above
		int index = INT_MAX; // RA2: as above
		int smallest = INT_MAX; // RA2: as above

		for (int j = 0; j < Players.Count(); j++) { // RA2: as above
			if (!used[j] && Players[j]->Slot.ColorIdx < smallest) { // RA2: as above
				smallest = Players[j]->Slot.ColorIdx; // RA2: as above
				index = j; // RA2: as above
			}
		}

		if (index == INT_MAX) { // RA2: as above
			break; // RA2: as above
		}

		used[index] = true; // RA2: as above
		ordered.Add(Players[index]); // RA2: as above

		if (ordered.Count() == Players.Count()) { // RA2: as above
			break; // RA2: as above
		}
	}

	memset(used, 0, sizeof(used)); // RA2: as above

	for (i = 0; i < ordered.Count(); i++) { // RA2: as above
		if (!used[i]) { // RA2: as above
			IPXAddressClass address = ordered[i]->Address; // RA2: as above

			for (int j = 0; j < ordered.Count(); j++) { // RA2: as above
				if (j != i && !used[j] && address.Is_Same_Node(ordered[j]->Address)) { // RA2: as above
					int peer_slot = Name_To_Slot(Wide_To_Narrow(ordered[j]->Name)); // RA2: as above
					int local_slot = Name_To_Slot(Wide_To_Narrow(ordered[i]->Name)); // RA2: as above

					if (PlayerPortBase[peer_slot] == PlayerPortBase[local_slot]) { // RA2: as above
						ordered[j]->Address.Set_Port(PlayerPortBase[peer_slot] + port_offset++); // RA2: as above
						used[j] = true; // RA2: as above
						DebugString("Adjusted port number to %d for player %s\n", port_offset - 1, Wide_To_Narrow(ordered[j]->Name)); // RA2: as above
					} else {
						DebugString("Two player behand the same NAT but they are using different port bases anyway\n"); // RA2: as above
					}

					char * name = Wide_To_Narrow(ordered[i]->Name); // RA2: as above
					if (stricmp(name, g_NickName.get()) == 0) { // RA2: as above
						Copy_Player_Name(NatPeerNames[local_count++], ordered[j]->Name); // RA2: as above
					}

					name = Wide_To_Narrow(ordered[j]->Name); // RA2: as above
					if (stricmp(name, g_NickName.get()) == 0) { // RA2: as above
						Copy_Player_Name(NatPeerNames[local_count++], ordered[i]->Name); // RA2: as above
					}
				}
			}
		}
	}

	if (PortNumberOverride) { // RA2: as above
		DebugString("Using %d for local port\n", PortNumberOverride); // RA2: as above
		Players[0]->Address.Set_Port(PortNumberOverride); // RA2: as above
		Slots[Player_To_Slot(0)]->Address.Set_Port(PortNumberOverride); // RA2: as above
	}

	for (i = 0; i < Players.Count(); i++) { // RA2: as above
		PlayerPorts[i] = Players[i]->Address.Get_Port(); // RA2: as above
		if (PlayerPorts[i] == 0) { // RA2: as above
			PlayerPorts[i] = 1234; // RA2: as above
		}
	}

	IPXAddressClass own_address = Players[0]->Address; // RA2: as above
	NetNodeType own_node; // RA2: as above
	own_address.Get_Node(own_node); // RA2: as above

	for (i = 1; i < Players.Count(); i++) { // RA2: as above
		if (own_address.Is_Same_Node(Players[i]->Address)) { // RA2: as above
			DebugString("%s has the same IP address as me\n", Wide_To_Narrow(Players[i]->Name)); // RA2: as above

			for (int j = 0; j < MAX_PLAYERS; j++) { // RA2: as above
				NodeNameType * node = Slots[j]; // RA2: as above
				char * name = Wide_To_Narrow(Players[i]->Name); // RA2: as above
				char * peer_name = Wide_To_Narrow(node->Name); // RA2: as above

				if (stricmp(name, peer_name) == 0) { // RA2: as above
					NetNodeType local_node; // RA2: as above
					SlotAddresses[j].Get_Node(local_node); // RA2: as above
					Players[i]->Address.Set_Address(NULL, local_node, Players[i]->Address.Get_Port()); // RA2: as above

					name = Wide_To_Narrow(Players[i]->Name); // RA2: as above
					DebugString("Talking to NAT player %s on IP address %s\n", name, Players[i]->Address.As_String(NULL)); // RA2: as above
				}
			}
		}
	}

	if (PacketTransport) { // RA2: as above
		((UDPInterfaceClass *)PacketTransport)->Close_All_Port_Sockets(); // RA2: as above
		((UDPInterfaceClass *)PacketTransport)->OpenSocket(PlayerPorts[0]); // RA2: as above
		PacketTransport->Start_Listening(); // RA2: as above
	}
}




























































/***************************************************************************
 * SessionClass::Read_MultiPlayer_Settings -- reads settings INI           *
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
 *   02/14/1995 BR : Created.                                              *
 *=========================================================================*/



/************************** end of session.cpp *****************************/
