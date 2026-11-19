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
#include "rect.h"
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
#include "mixfile.h"
#include "cd.h"
#include "_pk.h" // RA2
#include "stringtable.h" // RA2
#include "firewall.h"
#include "_rand.h"
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
#include "multiplayer.h"
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
#include "_WSProto.h"
#include "wspudp.h"

#include <ctime> // for station ID computation
#include <dos.h> // for station ID computation
#include "ftimer.h"









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

char const * const SessionClass::CallWaitStringIDs[ CALL_WAIT_STRINGS_NUM ] = {
	"TXT_CWAIT_STAR70",
	"TXT_CWAIT_70NUM",
	"TXT_CWAIT_1170",
	"TXT_CUSTOM",
};

int NodeNameType::Get_Country(void)
{
	return (Slot.InitialCountry == -2) ? -2 : Slot.Country;
}

int NodeNameType::Get_Color(void)
{
	return (Slot.InitialColor == -2) ? -2 : Slot.ColorIdx;
}

int NodeNameType::Get_Color_Or_Random(void)
{
	if (Slot.InitialColor == -2 && Slot.ColorIdx == -1) {
		return -2;
	}
	return Slot.ColorIdx;
}

void NodeNameType::Set_Country(int value)
{
	if (value == -2) {
		Slot.Country = 0;
		Slot.InitialCountry = -2;
	} else {
		Slot.Country = value;
		Slot.InitialCountry = -1;
	}
}

void NodeNameType::Set_Color(int value)
{
	if (value == -2) {
		Slot.ColorIdx = -1;
		Slot.InitialColor = -2;
	} else {
		Slot.ColorIdx = value;
		Slot.InitialColor = -1;
	}
}

SessionOptionsClass::SessionOptionsClass(void)
{
	MPGameMode = 0;
	ScenIndex = 0;
	GameSpeed = 0;
	Credits = 10000;
	UnitCount = 10;
	ShortGame = true;
	SuperWeaponsAllowed = true;
	BuildOffAlly = true;
	MCVRepacks = true;
	CratesAppear = true;
	for (int i = 0; i < 8; i++) {
		SlotData[i][0] = 0;
		SlotData[i][1] = 0;
		SlotData[i][2] = 0;
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
SessionClass::SessionClass(void) :
	MPGameMode(NULL)
{
	Type = GAME_NORMAL;
	CommProtocol = DEFAULT_COMM_PROTOCOL;

	Options.ScenarioIndex = 0;
	Options.Bases = true;
	Options.Credits = 0;
	Options.BridgeDestruction = false;
	Options.Goodies = false;
	Options.Ghosts = false;
	Options.UnitCount = 0;
	Options.ShortGame = false;
	Options.SWAllowed = false;
	Options.BuildOffAlly = false;
	Options.GameSpeed = 0;
	Options.CrapEngineers = false;
	Options.AlliesAllowed = false;
	Options.HarvTruce = false;
	Options.CTF = false;
	Options.FogOfWar = false;
	Options.MCVRedeploy = false;
	Options.AIDifficulty = DIFF_NORMAL;

	UniqueID = 0;
	Handle[0] = 0;
	House = 0;
	idxSide2 = -1;
	PrefColor = 2;
	ColorIdx = -1;
	Side = House;
	Side2 = idxSide2;
	Color = PrefColor;
	Color2 = ColorIdx;

	WOLTaunts = true;
	LANTaunts = true;
	WOLScrollText = true;
	LANScrollText = true;

	for (int p = 0; p < 8; p++) {
		PlayerPorts[p] = 0;
	}
	RouterProbeActive = false;
	PortProbeDue = false;
	RouterProbePort = 0;
	RouterProbePort2 = 0;
	PortBase = 1250;
	ForcePortBase = 0;
	RotatePortBase = true;
	SendDelay = false;
	PortNumberOverride = 0;
	memset(ManglerAddresses, 0, sizeof(ManglerAddresses));
	memset(ManglerPorts, 0, sizeof(ManglerPorts));

	MultiplayerObserver = 0;
	Unknown_304 = 0;
	LastNicknameSlot = -1;
	WOLLimitResolution = false;
	ViaPacketRouter = false;
	MasterPlayerID = -1;
	memset(MasterPlayerName, 0, sizeof(MasterPlayerName));
	SlowestResponse = 0;
	UseRouter = true;
	LastTimingID = -1;
	AboutToExitActive = false;
	RouterResponse = 20;
	ExitEventPending = 0;
	ExitEventFrame = 0;
	ExitEventPlayer = -1;
	PacketRouterPort = 0;

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
	EnableMultiplayerDebug = false;
	DrawMPDebugStats = false;
	LoadGame = 0;
	EmergencySave = 0;

	LastMessage[0] = 0;
	WWChat = 0;

	RecordFile.Set_Name("RECORD.BIN");  // always uses this name
	Record = 0;                          // set via command line
	Play = 0;                           // set via command line
	Attract = 0;                        // set via command line

	IsBridge = 0;
	NetStealth = false;
	NetProtect = true;
	NetOpen = false;
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
	strcpy(SerialDefaults.CallWaitString, "");
	ModemType = MODEM_NULL_HOST;						// set from INI file

	TrapFrame = 0x7fffffff;     // frame to start trapping object values at
	TrapObjType = RTTI_NONE;    // type of object to trap
	TrapObject.Ptr.All = NULL;  // ptr to object being trapped
	TrapCoord = COORD_NONE;     // COORDINATE of object to trap
	TrapTarget.Invalidate();    // TARGET value of object to trap
	TrapCell = NULL;            // for trapping a cell
	TrapCheckHeap = 0;          // start checking the Heap
	TrapPrintCRC = 0;           // output CRC file

	IsStartReminded = false;
	Options.AIPlayers = 0;
	for (int i = 0; i < 8; i++) {
		Options.AISlots.Countries[i] = -1;
		Options.AISlots.Colors[i] = -1;
	}
	TournamentState = 0;
	unknown_10 = 0;
	ScenarioFileName[0] = 0;
	ScenarioFileLength = 0;
	ScenarioIsOfficial = false;
	ScenarioMaxPlayers = 0;
}

void SessionClass::Load_Call_Wait_Strings(void)
{
	for (int i = 0; i < CALL_WAIT_STRINGS_NUM; i++) {
		CallWaitStrings.Add(_wcsdup(StringTable::LoadString(CallWaitStringIDs[i], NULL, "D:\\Ra2\\Session.CPP", 536)));
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

    for (int i = 0; i < Players.Count(); i++) {
        DebugString("Player %d, Name: %s, ID %d\n", i, Players[i]->Name, Players[i]->Player.ID);
        if (wcsicmp(Players[i]->Name, Houses[Players[i]->Player.ID]->UIName) != 0) {
            return(0);
        }
        if (wcsicmp(Players[i]->Name, MasterPlayerName) == 0) {
            MasterPlayerID = Players[i]->Player.ID;
            Session.Players[i]->Address.Get_Address(net, node);
            Session.PacketRouterAddress.Set_Address(net, node, 0);
            Ipx.Set_Lobby_Address(&Session.PacketRouterAddress);
        }
        Houses[Players[i]->Player.ID]->SquadID = Players[i]->Player.SquadID;
        Session.Players[i]->Address.Get_Address(net, node);
        unsigned long ip = 0;
        for (int j = 0; j < 4; j++) {
            ip = (ip << 8) | node[j];
        }
        DebugString("House[%d] IP = %X  Clan=%d\n", Players[i]->Player.ID, ip, Players[i]->Player.SquadID);
        Houses[Players[i]->Player.ID]->IPAddress = ip;
        Houses[Players[i]->Player.ID]->LostConnection = false;
        if (i > 0) {
            Ipx.Create_Connection(Players[i]->Player.ID, Players[i]->Name, &Players[i]->Address);
            Players[i]->Player.ProcessTime = -1;
        }
    }

    for (int k = 0; k < 8; k++) {
        Session.PlayerLastTime[k] = TickCount;
    }

	//------------------------------------------------------------------------
	// Loop through all entries in 'Players'
	//------------------------------------------------------------------------
    if (Type == GAME_INTERNET && UseRouter) {
        NetNumType net2;
        NetNodeType node2;
        net2[0] = 0;
        net2[1] = 0;
        net2[2] = 0;
        net2[3] = 0;
        node2[0] = 0;
        node2[1] = 0;
        node2[2] = 0;
        node2[3] = 0;
        node2[4] = 0;
        node2[5] = 0;
        Session.PacketRouterAddressTranslate.Set_Address(net2, node2, 0);
        ViaPacketRouter = true;
        SlowestResponse = 0;
        RouterResponse = 20;
        LastTimingID = -1;
        Ipx.Create_Multicast_Connection();
        OwnerTalk.Build_Connection_List();
        if (Session.Am_I_Master(0)) {
            bool connected = OwnerTalk.Start_Router(NULL);
            if (connected) {
                connected = OwnerTalk.Send_Player_List();
            }
            if (!connected) {
                Session.ViaPacketRouter = false;
            }
        }
    } else {
        Session.ViaPacketRouter = false;
    }
    DebugString("Session.ViaPacketRouter = %s\n", Session.ViaPacketRouter ? "true" : "false");
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
bool SessionClass::Am_I_Master(HouseClass *house)
{
    if (house == 0) {
        house = PlayerPtr;
    }
    if (Session.Type == GAME_INTERNET && house != 0) {
        if (MasterPlayerID != -1) {
            return house->HeapID == MasterPlayerID;
        }
        if (_wcsicmp(house->UIName, MasterPlayerName) == 0) {
            return(true);
        }
    }
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *player = Houses[i];
        if (player->IsHuman && !player->IsDefeated) {
            if (house == player) {
                return(true);
            } else {
                return(false);
            }
        }
    }
    return(false);
}

void SessionOptionsClass::Read(CCINIClass & ini, char const * section, int default_slot, int default_first_slot)
{
	char entry[16];

	int mode = First_Game_Mode()->MPModeIndex;
	MPGameMode = ini.Get_Int(section, "GameMode", mode);
	ScenIndex = ini.Get_Int(section, "ScenIndex", 0);
	GameSpeed = ini.Get_Int(section, "GameSpeed", Rule->GameSpeed);
	Credits = ini.Get_Int(section, "Credits", Rule->MPMoney);
	UnitCount = ini.Get_Int(section, "UnitCount", Rule->MPUnitCount);
	ShortGame = ini.Get_Bool(section, "ShortGame", Rule->ShortGame);
	SuperWeaponsAllowed = ini.Get_Bool(section, "SuperWeaponsAllowed", Rule->SuperWeaponsAllowed);
	BuildOffAlly = ini.Get_Bool(section, "BuildOffAlly", Rule->BuildOffAlly);
	MCVRepacks = ini.Get_Bool(section, "MCVRepacks", Rule->MCVRedeploys);
	CratesAppear = ini.Get_Bool(section, "CratesAppear", Rule->IsMPCrates);

	for (int i = 1; i < 8; i++) {
		sprintf(entry, "Slot%02d", i);
		int first = (i == 1) ? default_first_slot : default_slot;
		int second = -2;
		int third = -2;
		ini.Get_Int3(section, entry, &first, &second, &third);
		SlotData[i][0] = first;
		SlotData[i][1] = second;
		SlotData[i][2] = third;
	}
}

	// end of Am_I_Master
void SessionClass::Read_MultiPlayer_Settings(void)
{
	char * tokenptr;
	PhoneEntryClass * phone;
	char * entry;
	char buf[0x400];
	int i;


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
	CCFileClass file("RA2.INI");

	// Get the player's last-used Handle
	if (!PortsRead) {
		PortBase = ConfigINI.Get_Int("MultiPlayer", "PortBase", 0x4E2);
		ForcePortBase = ConfigINI.Get_Int("MultiPlayer", "ForcePortBase", 0);
		FirewallHelper.Port =
			ConfigINI.Get_Int("MultiPlayer", "PortPool", NonCriticalRandomNumber(0, 0x8000) + 0x1000);
		PortsRead = true;
		FirewallHelper.Read_Settings();
	}
	Session.SendDelay = ConfigINI.Get_Bool("MultiPlayer", "SendDelay", false);
	Session.PortNumberOverride = ConfigINI.Get_Int("MultiPlayer", "PortNumberOverride", 0);

	// Get the player's last-used Color
	buf[0] = 0;
	CurPhoneIdx = ConfigINI.Get_Int("MultiPlayer", "PhoneIndex", -1);
	TrapCheckHeap = ConfigINI.Get_Int("MultiPlayer", "CheckHeap", 0);
	WOLLimitResolution = ConfigINI.Get_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution);
	LastNicknameSlot = ConfigINI.Get_Int("MultiPlayer", "LastNickSlot", -1);

	// Read in default serial settings
	wchar_t handle[20];
	ConfigINI.Get_UnicodeString(
		"MultiPlayer", "Handle",
		StringTable::LoadString("TXT_NONAME", 0, "D:\\Ra2\\Session.CPP", 0x4b2), handle, 20);
	OwnerDraw::WideToCharString(Handle, handle, -1);

	Color = ConfigINI.Get_Int("MultiPlayer", "Color", Color);
	Color2 = ConfigINI.Get_Int("MultiPlayer", "ColorEx", Color2);
	Side = ConfigINI.Get_HousesType("MultiPlayer", "Side", (HousesType)Side);
	Side2 = ConfigINI.Get_Int("MultiPlayer", "SideEx", Side2);
	WOLTaunts = ConfigINI.Get_Bool("MultiPlayer", "WOLTaunts", WOLTaunts);
	LANTaunts = ConfigINI.Get_Bool("MultiPlayer", "LANTaunts", LANTaunts);
	WOLScrollText = ConfigINI.Get_Bool("MultiPlayer", "WOLScrollText", WOLScrollText);
	LANScrollText = ConfigINI.Get_Bool("MultiPlayer", "LANScrollText", LANScrollText);

	House = Side;
	if (House == -2) {
		House = 0;
	}
	idxSide2 = Side2;

	MPGameMode = MultiplayerGameMode::Get(ConfigINI.Get_Int("MultiPlayer", "GameMode", 1));
	if (!MPGameMode) {
		MPGameMode = First_Game_Mode();
	}
	Skirmish.Read(ConfigINI, "Skirmish", 1, 6);
	LAN.Read(ConfigINI, "LAN", 2, 2);
	WOL.Read(ConfigINI, "WonlinePref", 2, 2);

	ConfigINI.Get_String("SerialDefaults", "ModemName", buf, SerialDefaults.ModemName, 0x3F);
	if (strcmp(SerialDefaults.ModemName, buf) == 0) {
		SerialDefaults.ModemName[0] = 0;
	}
	SerialDefaults.Port = ConfigINI.Get_Int("SerialDefaults", "Port", 0);
	SerialDefaults.IRQ = ConfigINI.Get_Int("SerialDefaults", "IRQ", -1);
	SerialDefaults.Baud = ConfigINI.Get_Int("SerialDefaults", "Baud", -1);
	SerialDefaults.Compression = ConfigINI.Get_Int("SerialDefaults", "Compression", 0);
	SerialDefaults.ErrorCorrection = ConfigINI.Get_Int("SerialDefaults", "ErrorCorrection", 0);

	ConfigINI.Get_String("SerialDefaults", "DialMethod", "T", buf, 2);
	for (i = 0; i < 2; i++) {
		if (!_stricmp(buf, DialMethodCheck[i])) {
			SerialDefaults.DialMethod = (DialMethodType)i;
			break;
		}
	}
	if (i == 2) {
		SerialDefaults.DialMethod = DIAL_TOUCH_TONE;
	}

	SerialDefaults.InitStringIndex = ConfigINI.Get_Int("SerialDefaults", "InitStringIndex", -1);
	SerialDefaults.CallWaitStringIndex = ConfigINI.Get_Int("SerialDefaults", "CallWaitStringIndex", 3);
	ConfigINI.Get_String("SerialDefaults", "CallWaitString", "", SerialDefaults.CallWaitString, 0x10);

	if (SerialDefaults.IRQ == 0 || SerialDefaults.Baud == 0) {
		SerialDefaults.Port = 0;
		SerialDefaults.IRQ = -1;
		SerialDefaults.Baud = -1;
	}

	int initcount = ConfigINI.Entry_Count("InitStrings");

	int index;
	for (index = 0; index < initcount; index++) {
		entry = new char[0x29];
		if (entry) {
			entry[0] = 0;
			ConfigINI.Get_String("InitStrings", ConfigINI.Get_Entry("InitStrings", index), 0, entry,
									0x29);
			_strupr(entry);
			InitStrings.Add(entry);
		}
	}
	SerialDefaults.InitStringIndex = initcount ? 0 : -1;

	int phonecount = ConfigINI.Entry_Count("PhoneBook");
	for (index = 0; index < phonecount; index++) {
		// Create a new phone book entry
		phone = new PhoneEntryClass;
		ConfigINI.Get_String("PhoneBook", ConfigINI.Get_Entry("PhoneBook", index), 0, buf, sizeof(buf));

		// Read the entire entry in
		tokenptr = strtok(buf, "|");
		if (tokenptr) {
			Char_To_Wide(phone->Name, tokenptr, -1);
			_wcsupr(phone->Name);
		} else {
			phone->Name[0] = 0;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			strcpy(phone->Number, tokenptr);
			_strupr(phone->Number);
		} else {
			phone->Number[0] = 0;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			sscanf(tokenptr, "%x", &phone->Settings.Port);
		} else {
			phone->Settings.Port = 0;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.IRQ = atoi(tokenptr);
		} else {
			phone->Settings.IRQ = -1;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.Baud = atoi(tokenptr);
		} else {
			phone->Settings.Baud = -1;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.Compression = atoi(tokenptr);
		} else {
			phone->Settings.Compression = -1;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.ErrorCorrection = atoi(tokenptr);
		} else {
			phone->Settings.ErrorCorrection = 0;
		}

		/*
		**	Find out if this phonebook entry has the new settings included. If not
		**	then we need to skip this section.
		*/
		tokenptr = strtok(0, "|");
		if (tokenptr) {
			strcpy(buf, tokenptr);
			for (i = 0; i < 2; i++) {
				if (!_stricmp(buf, DialMethodCheck[i])) {
					phone->Settings.DialMethod = (DialMethodType)i;
					break;
				}
			}
			if (i == 2) {
				phone->Settings.DialMethod = DIAL_TOUCH_TONE;
			}
		} else {
			phone->Settings.DialMethod = DIAL_TOUCH_TONE;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.InitStringIndex = atoi(tokenptr);
		} else {
			phone->Settings.InitStringIndex = -1;
		}

		tokenptr = strtok(0, "|");
		if (tokenptr) {
			phone->Settings.CallWaitStringIndex = atoi(tokenptr);
		} else {
			phone->Settings.CallWaitStringIndex = 3;
		}

		tokenptr = strtok(0, "|");
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
		TrapFrame = ConfigINI.Get_Int("SyncBug", "Frame", 0x7FFFFFFF);
		ConfigINI.Get_String("SyncBug", "Type", "NONE", buf, 0x50);
		if (!_stricmp(buf, "AIRCRAFT")) {
			TrapObjType = RTTI_AIRCRAFT;
		} else if (!_stricmp(buf, "ANIM")) {
			TrapObjType = RTTI_ANIM;
		} else if (!_stricmp(buf, "BUILDING")) {
			TrapObjType = RTTI_BUILDING;
		} else if (!_stricmp(buf, "BULLET")) {
			TrapObjType = RTTI_BULLET;
		} else if (!_stricmp(buf, "INFANTRY")) {
			TrapObjType = RTTI_INFANTRY;
		} else if (!_stricmp(buf, "UNIT")) {
			TrapObjType = RTTI_UNIT;
		} else {
			TrapObjType = RTTI_NONE;
		}

		ConfigINI.Get_String("SyncBug", "Coord", "0", buf, 0x50);
		int x;
		int y;
		int z;
		sscanf(buf, "%d,%d,%d", &x, &y, &z);
		int target = ConfigINI.Get_Int("SyncBug", "Target", -1);
		TrapTarget.Decode(target);

		ConfigINI.Get_String("SyncBug", "Cell", "0,0", buf, 0x50);
		sscanf(buf, "%d,%d", &x, &y);
		Cell cell(x, y);
		if (cell != CELL_NONE) {
			TrapCell = &Map[cell];
		}

		TrapPrintCRC = ConfigINI.Get_Int("SyncBug", "PrintCRC", 0x7FFFFFFF);
	}
}







/// <summary>
/// Writes the session settings out to a log file.
/// This routine is used when a sync bug or some other multiplayer failure is being
/// recorded, so that the game options and the players' network addresses are captured
/// alongside it.
/// </summary>
/// <param name="out">The file to write the report to.</param>
bool SessionClass::Log_To_File(FILE * out)
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
		fprintf(out, ":");
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

	fprintf(out, ":");
	for (i = 0; i < sizeof(NetNodeType); i++) {
		fprintf(out, "%02X", node[i]);
	}

	fprintf(out, "\n\n");
	fprintf(out, "MaxAhead = %d\n", MaxAhead);
	fprintf(out, "LoadGame = %d\n", LoadGame);
	fprintf(out, "PrefColor = %d\n", Color);
	fprintf(out, "ColorIdx = %d\n", PrefColor);
	fprintf(out, "House = %d\n", House);
	fprintf(out, "NumPlayers = %d\n", NumPlayers);
	fprintf(out, "Options.Bases = %d\n", Options.Bases);
	fprintf(out, "Options.Credits = %d\n", Options.Credits);
	fprintf(out, "Options.BridgeDestruction = %d\n", Options.BridgeDestruction);
	fprintf(out, "Options.Goodies = %d\n", Options.Goodies);
	fprintf(out, "Options.Ghosts = %d\n", Options.Ghosts);
	fprintf(out, "Options.UnitCount = %d\n", Options.UnitCount);
	fprintf(out, "Options.AIPlayers = %d\n", Options.AIPlayers);
	fprintf(out, "Options.AIDifficulty = %d\n", Options.AIDifficulty);
	fprintf(out, "ObiWan = %d\n", MultiplayerObserver);

	if (Session.Type == GAME_IPX) {
		fprintf(out, "Type = IPX\n");
	} else if (Session.Type == GAME_INTERNET) {
		fprintf(out, "Type = INTERNET\n");
	} else {
		fprintf(out, "Type = !!Unknown!!\n");
	}

	return(true);
}
void SessionOptionsClass::Write(CCINIClass & ini, char const * section)
{
	char entry[16];

	ini.Put_Int(section, "GameMode", MPGameMode);
	ini.Put_Int(section, "ScenIndex", ScenIndex);
	ini.Put_Int(section, "GameSpeed", GameSpeed);
	ini.Put_Int(section, "Credits", Credits);
	ini.Put_Int(section, "UnitCount", UnitCount);
	ini.Put_Bool(section, "ShortGame", ShortGame);
	ini.Put_Bool(section, "SuperWeaponsAllowed", SuperWeaponsAllowed);
	ini.Put_Bool(section, "BuildOffAlly", BuildOffAlly);
	ini.Put_Bool(section, "MCVRepacks", MCVRepacks);
	ini.Put_Bool(section, "CratesAppear", CratesAppear);

	for (int i = 1; i < 8; i++) {
		sprintf(entry, "Slot%02d", i);
		ini.Put_Int3(section, entry, SlotData[i][0], SlotData[i][1], SlotData[i][2]);
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
    RawFileClass file("RA2.INI");

    ConfigINI.Put_Int("MultiPlayer", "PhoneIndex", CurPhoneIdx);
    ConfigINI.Put_Bool("MultiPlayer", "WOLLimitResolution", WOLLimitResolution);
    ConfigINI.Put_Int("MultiPlayer", "LastNickSlot", LastNicknameSlot);
    ConfigINI.Put_UnicodeString("MultiPlayer", "Handle", Narrow_To_Wide(Handle));
    ConfigINI.Put_Int("MultiPlayer", "Color", Color);
    ConfigINI.Put_Int("MultiPlayer", "ColorEx", Color2);
    ConfigINI.Put_HousesType("MultiPlayer", "Side", (HousesType)Side);
    ConfigINI.Put_Int("MultiPlayer", "SideEx", Side2);
    if (MPGameMode) {
        ConfigINI.Put_Int("MultiPlayer", "GameMode", MPGameMode->MPModeIndex);
    } else {
        ConfigINI.Put_Int("MultiPlayer", "GameMode", 0);
    }
    Skirmish.Write(ConfigINI, "Skirmish");
    LAN.Write(ConfigINI, "LAN");
    WOL.Write(ConfigINI, "WonlinePref");

    ConfigINI.Clear("SerialDefaults");
    ConfigINI.Put_String("SerialDefaults", "CallWaitString", SerialDefaults.CallWaitString);
    ConfigINI.Put_Int("SerialDefaults", "CallWaitStringIndex", SerialDefaults.CallWaitStringIndex);
    ConfigINI.Put_Int("SerialDefaults", "InitStringIndex", SerialDefaults.InitStringIndex);
    ConfigINI.Put_String("SerialDefaults", "DialMethod", DialMethodCheck[SerialDefaults.DialMethod]);
    ConfigINI.Put_Int("SerialDefaults", "Baud", SerialDefaults.Baud);
    ConfigINI.Put_Int("SerialDefaults", "IRQ", SerialDefaults.IRQ);
    ConfigINI.Put_Int("SerialDefaults", "Port", SerialDefaults.Port, 1);
    ConfigINI.Put_Int("SerialDefaults", "Compression", SerialDefaults.Compression);
    ConfigINI.Put_Int("SerialDefaults", "ErrorCorrection", SerialDefaults.ErrorCorrection);
    ConfigINI.Put_String("SerialDefaults", "ModemName", SerialDefaults.ModemName);

    ConfigINI.Clear("InitStrings");
    for (int index = 0; index < InitStrings.Count(); index++) {
        char buf[10];
        wsprintfA(buf, "%03d", index);
        ConfigINI.Put_String("InitStrings", buf, InitStrings[index]);
    }

    ConfigINI.Clear("PhoneBook");
    for (int i = PhoneBook.Count() - 1; i >= 0; i--) {
        char buf[128];
        char entrytext[10];
        wsprintfA(buf, "%s|%s|%x|%d|%d|%d|%d|%s|%d|%d|%s",
                  PhoneBook[i]->Name,
                  PhoneBook[i]->Number,
                  PhoneBook[i]->Settings.Port,
                  PhoneBook[i]->Settings.IRQ,
                  PhoneBook[i]->Settings.Baud,
                  PhoneBook[i]->Settings.Compression,
                  PhoneBook[i]->Settings.ErrorCorrection,
                  DialMethodCheck[PhoneBook[i]->Settings.DialMethod],
                  PhoneBook[i]->Settings.InitStringIndex,
                  PhoneBook[i]->Settings.CallWaitStringIndex,
                  PhoneBook[i]->Settings.CallWaitString);
        wsprintfA(entrytext, "%03d", i);
        ConfigINI.Put_String("PhoneBook", entrytext, buf);
    }

    ConfigINI.Save(file, false);
}

void SessionClass::Write_Chat_Settings(void)
{
	RawFileClass file("RA2.INI");

	ConfigINI.Put_Bool("MultiPlayer", "WOLTaunts", WOLTaunts);
	ConfigINI.Put_Bool("MultiPlayer", "LANTaunts", LANTaunts);
	ConfigINI.Put_Bool("MultiPlayer", "WOLScrollText", WOLScrollText);
	ConfigINI.Put_Bool("MultiPlayer", "LANScrollText", LANScrollText);
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





void __fastcall Read_Map_Header(char const * filename, wchar_t * description, char * digest, char * game_modes, bool * official, int * min_players, int * max_players)
{
	CCFileClass file(filename);
	if (!file.Is_Available() || !file.Open(FileClass::READ)) {
		return;
	}

	int percent = 0;
	int size;
	char * head = NULL;
	char * end;
	do {
		percent += 5;
		size = file.Size() * percent / 100;
		if (head) {
			delete head;
		}
		head = new char[size + 1];
		if (!head) {
			return;
		}
		file.Seek(0, SEEK_SET);
		file.Read(head, size);
		head[size] = '\0';
		char * start = strstr(head, "[Basic]");
		if (start) {
			end = strstr(start + 5, "\n[");
			if (end > start) {
				break;
			}
		}
		if (percent >= 100) {
			delete head;
			return;
		}
	} while (true);
	end[1] = '\0';
	DebugString("Read first %d%% of file looking for [Basic] section\n", percent);

	percent = 0;
	char * tail = NULL;
	int length;
	do {
		percent++;
		length = file.Size() * percent / 100;
		if (tail) {
			delete tail;
		}
		tail = new char[length + 1];
		if (!tail) {
			return;
		}
		file.Seek(-length, SEEK_END);
		file.Read(tail, length);
		tail[length] = '\0';
		if (strstr(tail, "[Digest]")) {
			break;
		}
		if (percent >= 100) {
			delete head;
			delete tail;
			return;
		}
	} while (true);
	DebugString("Read last  %d%% of file looking for [Digest] section\n", percent);

	CCINIClass ini;
	BufferStraw straw(head, end - head);
	ini.Load(straw, false, false);
	CCINIClass digest_ini;
	BufferStraw digest_straw(tail, strlen(tail) + 1);
	digest_ini.Load(digest_straw, false, false);

	int min = ini.Get_Int("Basic", "MinPlayers", *min_players);
	*min_players = min;
	int max = ini.Get_Int("Basic", "MaxPlayers", *max_players);
	*max_players = max;
	ini.Get_String("Basic", "GameMode", "", game_modes, 0x100);
	char name[0x40];
	ini.Get_String("Basic", "Name", "No Name", name, 0x40);
	Char_To_Wide(description, name, -1);
	digest_ini.Get_String("Digest", "1", "No Digest", digest, 0x20);
	*official = ini.Get_Bool("Basic", "Official", false);
	DebugString("MPR %s (%d/%d): official: %d, desc: %s\n", filename, *min_players, *max_players, *official, name);
	DebugString("\t\tGameModes: %s\n", game_modes);
	delete head;
	delete tail;
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
	HANDLE handle;

	Scenarios.Clear();

	/*
	**	Fetch the main multiplayer scenario packet data.
	*/
	CCFileClass file("MISSIONS.PKT");
	if (file.Is_Available()) {
		ini.Load(file, false);
		Add_Missions(Scenarios, ini, name_buffer);
	}
	ini.Clear();
	file.Close();

	handle = FindFirstFile("*.PKT", &block);
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) {
			char const * name = &block.cAlternateFileName[0];
			if (strlen(name) == 0) {
				name = &block.cFileName[0];
			}

	/*
	**	Fetch any scenario packet lists and apply them first.
	*/
			if (stricmp(name, "MISSIONS.PKT")) {
				file.Set_Name(name);
				ini.Load(file, false);
				Add_Missions(Scenarios, ini, name_buffer);
			}
		}

		if (!Next_File(handle, block)) {
			break;
		}
	}

	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}

	ini.Clear();
	file.Close();

	handle = FindFirstFile("*.MMX", &block);
	int previous_disk = CD::Get_Required_Disk();
	CD::Set_Required_Disk((DiskID)-2);
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) {
			char const * name = &block.cAlternateFileName[0];
			if (strlen(name) == 0) {
				name = &block.cFileName[0];
			}

	/*
	**	Scan the current directory for any loose .MPR files and build the appropriate entries
	**	into the scenario list list
	*/
			if (stricmp(name, "MISSIONS.MMX")) {
				MixFileClass * mix = new MixFileClass(name, &FastKey);
				if (mix != NULL) {
					ScenarioMixFiles.Add(mix);

					char pkt_name[260];
					int length = strlen(name);
					strncpy(pkt_name, name, length - 3);
					pkt_name[length - 3] = 0;
					strcat(pkt_name, "PKT");
					pkt_name[length] = 0;
					file.Set_Name(pkt_name);
					if (file.Is_Available()) {
						ini.Load(file, false);

						int count = ini.Entry_Count("MultiMaps");
						for (int index = 0; index < count; index++) {
							if (ini.Get_String("MultiMaps", ini.Get_Entry("MultiMaps", index), "", name_buffer, sizeof(name_buffer))) {
								MultiMission * mission = new MultiMission(ini, name_buffer);
								if (mission != NULL) {
									wchar_t players[64];
									wchar_t text[44];
									int max_count = mission->MaxPlayers;
									int min_count = mission->MinPlayers;
									if (min_count != max_count) {
										swprintf(players, L"%c%d-%d%c", L'(', min_count, max_count, L')');
									} else {
										swprintf(players, L"%c%d%c", L'(', min_count, L')');
									}
									int length = wcslen(mission->ScenarioDescription);
									int & used = length;
									wcscpy(text, mission->ScenarioDescription);
									wcsncat(text, L" ", 44 - used);
									used++;
									wcsncat(text, players, 44 - used);
									mission->Set_Description(text);
									Scenarios.Add(mission);
								}
							}
						}
						ini.Clear();
					} else {
						DebugString("Can't see .pkt inside .mmx!\n");
					}
				}
			}
		}

		if (!Next_File(handle, block)) {
			break;
		}
	}

	CD::Set_Required_Disk((DiskID)previous_disk);
	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}

	ini.Clear();
	file.Close();

	char digest_buffer[32];
	wchar_t description[64];
	char game_modes[256];
	bool official;
	int min_players;
	int max_players;

	handle = FindFirstFile("*.MPR", &block);
	official = false;
	while (handle != INVALID_HANDLE_VALUE) {
		if ((block.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY)) == 0) {
			char const * file_name = &block.cAlternateFileName[0];
			if (strlen(file_name) == 0) {
				file_name = &block.cFileName[0];
			}

			ini.Get_String("Basic", "Name", "No Name", name_buffer, sizeof(name_buffer));
			ini.Get_String("Digest", "1", "No Digest", digest_buffer, sizeof(digest_buffer));
			Char_To_Wide(description, name_buffer, -1);
			official = ini.Get_Bool("Basic", "Official", false);
			min_players = 2;
			max_players = 4;

			Read_Map_Header(file_name, description, digest_buffer, game_modes, &official, &min_players, &max_players);
			MultiMission * mission = new MultiMission(file_name, description, digest_buffer, official, game_modes, min_players, max_players);
			Scenarios.Add(mission);
		}

		if (!Next_File(handle, block)) {
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
static char const ColorSchemes[9] = {3, 11, 21, 29, 13, 25, 17, 15, 5};

int SessionClass::Color_Index_To_Scheme(int color)
{
	/*
	 * The list holds each scheme twice, and the odd entry of each pair is the lighting-aware
	 * one, so the index is doubled and stepped one past the plain copy.
	 */
    if (color == -2) {
        color = 8;
    }
    if ((unsigned)color < 9) {
        return ColorSchemes[color];
    }
    return color;
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
    unsigned int id = timeGetTime();
    unsigned int free = Disk_Space_Available();
    Add_CRC(&id, free);
    char *path = getenv("PATH");
    if (path != 0) {
        for (unsigned i = 0; i < strlen(path); i++) {
            Add_CRC(&id, path[i]);
        }
    }
    return id;
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
#if 0
MultiMission::MultiMission(INIClass const & ini, char const * name)
{
    ScenarioDescription[0] = 0;
    Filename[sizeof(Filename) - 1] = '\0';
    strcpy(Digest, "No Digest");
    IsOfficial = true;
    MinPlayers = 2;
    MaxPlayers = 4;
    Disks.Clear();
    if (!name) {
        return;
    }
    strcpy(Filename, name);
    strcat(Filename, ".MAP");
    if (ini.Is_Present(name, "DescriptionText")) {
        char text[0x2C];
        ini.Get_String(name, "DescriptionText", "", text, 0x2C);
        Char_To_Wide(ScenarioDescription, text, -1);
    } else {
        ini.ReadStringtableEntry(name, "Description", ScenarioDescription, 0x2C);
    }
    MinPlayers = ini.Get_Int(name, "MinPlayers", MinPlayers);
    MaxPlayers = ini.Get_Int(name, "MaxPlayers", MaxPlayers);
    char disks[0x80];
    if (ini.Get_String(name, "CD", "", disks, 0x80)) {
        char *token = strtok(disks, ",");
        while (token && *token) {
            Disks.Add((DiskID)atoi(token));
            token = strtok(0, ",");
        }
    }
    Wstring modes;
    bool found = ini.Get_String(name, "GameMode", "", modes) > 0;
    if (found) {
        Wstring token;
        int position = modes.getToken(0, ",", token);
        while (position != -1) {
            token.trimLeading(" ");
            token.trimTrailing(" ");
            GameModes.Add(token);
            position = modes.getToken(position, ",", token);
        }
    }
    CCFileClass file(Filename);
    if (!file.Is_Available()) {
        DebugString("Unable to locate scenario %s - No digest info will be loaded\n", Filename);
    } else {
        if (!file.Open(FileClass::READ)) {
            return;
        }
        file.Seek(-100, 2);
        char tail[0x80];
        memset(tail, 0, sizeof(tail));
        file.Read(tail, 0x64);
        file.Close();
        if (strstr(tail, "[Digest]")) {
            CCINIClass digest_ini;
            BufferStraw straw(tail, 0x64);
            digest_ini.Load(straw, false, false);
            digest_ini.Get_String("Digest", "1", 0, tail, 0x80);
            strncpy(Digest, tail, 0x20);
            Digest[0x1F] = '\0';
        }
    }
}
#endif




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
#if 0
MultiMission::MultiMission(char const * filename, wchar_t const * description, char const * digest, bool official,
                           char const * game_modes, int min_players, int max_players)
{
    if (filename) {
        strncpy(Filename, filename, sizeof(Filename));
        Filename[sizeof(Filename) - 1] = '\0';
    } else {
        strcpy(Filename, "No File Name");
    }
    if (description) {
        wcsncpy(ScenarioDescription, description, 0x2C);
        ScenarioDescription[0x2B] = 0;
    } else {
        wcscpy(ScenarioDescription, StringTable::LoadString("MSG:NoDescription", 0, "D:\\Ra2\\Session.CPP", 0xb0a));
    }
    if (digest) {
        strncpy(Digest, digest, sizeof(Digest));
        Digest[sizeof(Digest) - 1] = '\0';
    } else {
        strcpy(Digest, "No Digest");
    }
    IsOfficial = official;
    MinPlayers = min_players;
    MaxPlayers = max_players;
    Disks.Clear();
    Disks.Add((DiskID)-2);
    if (game_modes) {
        Wstring list(game_modes);
        if (list.length() > 0) {
            Wstring token;
            int position = list.getToken(0, ",", token);
            while (position != -1) {
                token.trimLeading(" ");
                token.trimTrailing(" ");
                GameModes.Add(token);
                position = list.getToken(position, ",", token);
            }
        }
    }
}
#endif



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
		return RawFileClass(Filename).Is_Available();
	}

	if (Disks.Is_In_List(DISK_ANY) == true) {
		return true;
	}
	return Disks.Is_In_List(cd);
}


/// <summary>
/// Fetches the disk this mission should be looked for on.
/// </summary>
/// <returns>Returns with the first disk the mission was registered against.</returns>
DiskID MultiMission::On_Which_CD(void) const
{
	return Disks[0];
}


/// <summary>
/// Sets the description of this mission.
/// This is the text the player is shown in the scenario list, so a mission that arrives
/// without one is given a placeholder rather than an empty line.
/// </summary>
/// <param name="description">The description to record, or NULL if the mission has none.</param>
void MultiMission::Set_Description(wchar_t const * description)
{
	if (description != NULL) {
		wcsncpy(ScenarioDescription, description, ARRAY_SIZE(ScenarioDescription));
		ScenarioDescription[ARRAY_SIZE(ScenarioDescription) - 1] = L'\0';
	} else {
		wcscpy(ScenarioDescription, StringTable::LoadString("MSG:NoDescription", NULL, "D:\\Ra2\\Session.CPP", 2826));
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
bool MultiMission::Is_Random_Map(void) const
{
	return stricmp(Filename, "RandMap.Sed") == 0;
}

bool MultiMission::Supports_Game_Mode(Wstring & mode) const
{
	if (GameModes.Count() == 0) {
		return mode == "standard";
	}
	for (int index = 0; index < GameModes.Count(); index++) {
		if (mode == GameModes[index]) {
			return true;
		}
	}
	return false;
}

bool SessionClass::Is_Random_Map(void) const
{
	return stricmp(ScenarioFileName, "RandMap.Sed") == 0;
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
	if (Debug_Map) {
		return;
	}

	int new_percent = percent;
	if (Scen->IsRandom) {
		new_percent = new_percent / 2;
	}

	if (Progress.Get_Current_Progress(0) * 100.0 < new_percent) {
		Progress.Set_Progress_Percent(0, new_percent);
	}

	if (Players.Count() == 1) {
		return;
	}

	CDTimerClass<SystemTimerClass> timer = 240;

	switch (Type) {
		case GAME_IPX:
		case GAME_INTERNET: {
				GlobalPacketType prog_packet;
				memset((void *)&prog_packet, 0, sizeof(prog_packet));

				prog_packet.Command = NET_PROGRESS_REPORT;
				prog_packet.Progress.Percent = int(100.0 * Progress.Get_Current_Progress(0));

				strcpy(prog_packet.Name, Wide_To_Narrow(Session.Players[0]->Name));

				if (prog_packet.Progress.Percent < 99.95) {
					Ipx.Send_Global_Message(&prog_packet, 0x33, 0, NULL);
				} else {
					for (int p = 1; p < Session.Players.Count(); p++) {
						DebugString("Sending guarenteed progress message to all players\n");
						Ipx.Send_Global_Message_To_Name(&prog_packet, 0x33, 1, Session.Players[p]->Name);
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
    DebugString("Squad 1 = %s\n", Squad1Names);
    DebugString("Squad 2 = %s\n", Squad2Names);
    if (strlen(Squad1Names) == 0 || strlen(Squad2Names) == 0) {
        return;
    }
    int i;
    int j;
    char *name;
    DynamicVectorClass<HouseClass *> squad1;
    DynamicVectorClass<HouseClass *> squad2;
    char *names = new char[(strlen(Squad1Names) > strlen(Squad2Names) ? strlen(Squad1Names) : strlen(Squad2Names)) + 0x20];
    strcpy(names, Squad1Names);
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) {
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i];
            if (house->IsHuman && stricmp(house->IniName, name) == 0) {
                squad1.Add(house);
            }
        }
    }
    strcpy(names, Squad2Names);
    for (name = strtok(names, ","); name && *name; name = strtok(0, ",")) {
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i];
            if (house->IsHuman && stricmp(house->IniName, name) == 0) {
                squad2.Add(house);
            }
        }
    }
    if (squad1.Count() > 1) {
        for (i = 0; i < squad1.Count(); i++) {
            for (j = 0; j < squad1.Count(); j++) {
                if (squad1[i]->HeapID != squad1[j]->HeapID) {
                    squad1[i]->Make_Ally(squad1[j]);
                }
            }
        }
    }
    if (squad2.Count() > 1) {
        for (i = 0; i < squad2.Count(); i++) {
            for (j = 0; j < squad2.Count(); j++) {
                if (squad2[i]->HeapID != squad2[j]->HeapID) {
                    squad2[i]->Make_Ally(squad2[j]);
                }
            }
        }
    }
    delete [] names;
    SquadAlliances = true;
    Scen->Special.IsAllianceFixed = true;
}

void GameOptionsType::Reset_AI(void)
{
	AIPlayers = 0;
	for (int index = 0; index < 8; index++) {
		AISlots.Countries[index] = -1;
		AISlots.Colors[index] = -1;
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
bool SessionClass::Is_Color_Used(int color)
{
	if (color == -2) {
		return false;
	}
	for (int index = 0; index < Players.Count(); index++) {
		if (Players[index]->Get_Color_Or_Random() == color) {
			return true;
		}
	}
	for (int slot = 0; slot < 8; slot++) {
		if (Options.AISlots.Colors[slot] == color) {
			return true;
		}
	}
	return false;
}

int SessionClass::Random_Side(void)
{
	int last = HouseTypes.Count() - 1;
	int index;
	do {
		index = Scen->RandomNumber(0, last);
	} while (!HouseTypes[index]->IsMultiplay || HouseTypes[index]->IsMultiplayPassive);
	return index;
}

int SessionClass::Random_Color(void)
{
	int color;
	do {
		color = Scen->RandomNumber(0, 7);
	} while (Is_Color_Used(color));
	return color;
}

int SessionClass::Chosen_House(void) const
{
	return (idxSide2 == -2) ? -2 : House;
}

int SessionClass::Chosen_Color(void) const
{
	return (ColorIdx == -2) ? -2 : PrefColor;
}

void SessionClass::Set_Side(int side, bool random)
{
	if (side == -2) {
		if (random) {
			Side = Random_Side();
		} else {
			Side = 0;
		}
		Side2 = -2;
	} else {
		Side = side;
		Side2 = -1;
	}
	House = Side;
	idxSide2 = Side2;
}

void SessionClass::Set_Color(int color, bool random)
{
	if (color == -2) {
		if (random) {
			Color = Random_Color();
		} else {
			Color = 0;
		}
		Color2 = -2;
	} else {
		Color = color;
		Color2 = -1;
	}
	PrefColor = Color;
	ColorIdx = Color2;
}

void SessionClass::Assign_Random_Choices(void)
{
	int count = Session.Players.Count();
	int i;
	for (i = 0; i < count; i++) {
		NodeNameType * player = Session.Players[i];
		if (player->Slot.InitialCountry == -3 || player->Slot.Country == -3) {
			player->Slot.Country = -3;
			player->Slot.InitialCountry = -1;
			player->Slot.ColorIdx = 8;
			player->Slot.InitialColor = -1;
		} else {
			if (player->Slot.InitialCountry == -2) {
				player->Slot.InitialCountry = -1;
				if (MPGameMode) {
					player->Slot.Country = MPGameMode->RandomHumanCountryIndex();
				} else {
					player->Slot.Country = Session.Random_Side();
				}
			}
			if (player->Slot.InitialColor == -2) {
				player->Slot.InitialColor = -1;
				player->Slot.ColorIdx = -1;
				player->Slot.ColorIdx = Session.Random_Color();
			}
		}
		if (i == 0) {
			Game::PlayerColor = player->Slot.ColorIdx;
		}
	}
	for (i = 0; i < 8; i++) {
		if (Session.Options.AISlots.Countries[i] == -2) {
			if (MPGameMode) {
				Session.Options.AISlots.Countries[i] = MPGameMode->RandomAICountryIndex();
			} else {
				Session.Options.AISlots.Countries[i] = Session.Random_Side();
			}
		}
		if (Session.Options.AISlots.Colors[i] == -2) {
			Session.Options.AISlots.Colors[i] = Session.Random_Color();
		}
	}
}

void SessionClass::Resume(void)
{
	if (!CurrentlyInGame && GameActive) {
		CurrentlyInGame = true;
		if (SavedGameSpeed == -1) {
			SavedGameSpeed = ::Options.GameSpeed;
		}
		if (Type == GAME_NORMAL) {
			::Options.GameSpeed = 2;
		}
		if (!Scen->IsInputLocked) {
			Lock_Scenario_Input();
			UnlockInputFrame = Frame + 3;
		}
		if (!MouseCursor->Is_Captured()) {
			MouseCursor->Capture_Mouse();
		}
		AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, 0x4000);
	}
}

void SessionClass::Leave_Game(void)
{
	if (CurrentlyInGame) {
		CurrentlyInGame = false;
		if (SavedGameSpeed != -1) {
			::Options.GameSpeed = SavedGameSpeed;
			SavedGameSpeed = -1;
		}
		Leave_Game_Hook();
		Stop_Ingame_Movie();
		Audio_Stop_Events();
		Sound_Clear_Static();
		Speech_Reset();
		Aux_Stream_Stop();
		AudioLevelSet(&AudioGroupAttribs1->VolumeLevel, 0x4000);
		if (ToolTips) {
			ToolTips->Activate(false);
		}
		if (MouseCursor->Is_Captured()) {
			MouseCursor->Release_Mouse();
		}
		MouseCursor->Hide_Mouse();
		UnlockInputFrame = 0;
	}
}


bool SessionClass::Is_In_Game(void) const
{
	return CurrentlyInGame;
}

void SessionClass::Remove_Player_Port(int index)
{
	for (int i = index; i < MAX_PLAYERS - 1; i++) {
		PlayerPorts[i] = PlayerPorts[i + 1];
	}
}

unsigned short SessionClass::Player_Port(int connection_id)
{
	int count = Session.Players.Count();
	for (int i = 0; i < count; i++) {
		if (Session.Players[i]->Player.ID == connection_id) {
			return PlayerPorts[i];
		}
	}
	return 0;
}

int SessionClass::Player_To_Slot(int index)
{
	int i = 0;
	NodeNameType * node;
	char * a;
	char * b;

	for (NodeNameType ** slot = Session.Slots;
		 (int)slot < (int)&Session.Slots[8];
		 i++, slot++) {
		node = *slot;
		a = Wide_To_Narrow(Session.Players[index]->Name);
		b = Wide_To_Narrow(node->Name);
		if (stricmp(a, b) == 0) {
			return i;
		}
	}
	return 0;
}

int SessionClass::Name_To_Slot(char const * name)
{
	int i = 0;
	for (NodeNameType ** slot = Session.Slots; (int)slot < (int)&Session.Slots[8]; slot++, i++) {
		if (stricmp(name, Wide_To_Narrow((*slot)->Name)) == 0) {
			return i;
		}
	}
	return 0;
}

int SessionClass::Slot_To_Player(int slot)
{
	for (int i = 0; i < Session.Players.Count(); i++) {
		NodeNameType * node = Session.Slots[slot];
		char * a = Wide_To_Narrow(Session.Players[i]->Name);
		if (stricmp(a, Wide_To_Narrow(node->Name)) == 0) {
			return i;
		}
	}
	return 0;
}


#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00673090 has no row)
void SessionPortAllocator::Allocate_Player_Ports(void)
{
	int player;
	unsigned short port_offset = 1;
	DynamicVectorClass<NodeNameType *> ordered;

	DebugString("Allocating port numbers and local addresses\n");
	if (Players.Count() < 2) {
		return;
	}

	int local_count = 0;
	memset(NatPeerNames, 0, sizeof(NatPeerNames));
	RotatePortBase = true;

	for (player = 0; player < Players.Count(); player++) {
		int slot = Player_To_Slot(player);
		if (slot >= 0 && slot < MAX_PLAYERS) {
			Players[player]->Address.Set_Port(PlayerPortBase[slot]);
			char * name = Wide_To_Narrow(Players[player]->Name);
			char const * address = Players[player]->Address.As_String(NULL);
			DebugString("Allocated port for player %s - %s\n", name, address);
		}
	}

	bool used[MAX_PLAYERS];
	memset(used, 0, sizeof(used));

	for (int i = 0; i < Players.Count(); i++) {
		int index = INT_MAX;
		int smallest = INT_MAX;

		for (int j = 0; j < Players.Count(); j++) {
			if (!used[j] && Players[j]->Slot.ColorIdx < smallest) {
				smallest = Players[j]->Slot.ColorIdx;
				index = j;
			}
		}

		if (index == INT_MAX) {
			break;
		}

		used[index] = true;
		ordered.Add(Players[index]);

		if (ordered.Count() == Players.Count()) {
			break;
		}
	}

	memset(used, 0, sizeof(used));

	for (i = 0; i < ordered.Count(); i++) {
		if (!used[i]) {
			IPXAddressClass address = ordered[i]->Address;

			for (int j = 0; j < ordered.Count(); j++) {
				if (j != i && !used[j] && address.Is_Same_Node(ordered[j]->Address)) {
					int peer_slot = Name_To_Slot(Wide_To_Narrow(ordered[j]->Name));
					int local_slot = Name_To_Slot(Wide_To_Narrow(ordered[i]->Name));

					if (PlayerPortBase[peer_slot] == PlayerPortBase[local_slot]) {
						ordered[j]->Address.Set_Port(PlayerPortBase[peer_slot] + port_offset++);
						used[j] = true;
						DebugString("Adjusted port number to %d for player %s\n", port_offset - 1, Wide_To_Narrow(ordered[j]->Name));
					} else {
						DebugString("Two player behand the same NAT but they are using different port bases anyway\n");
					}

					char * name = Wide_To_Narrow(ordered[i]->Name);
					if (stricmp(name, g_NickName.get()) == 0) {
						Copy_Player_Name(NatPeerNames[local_count++], ordered[j]->Name);
					}

					name = Wide_To_Narrow(ordered[j]->Name);
					if (stricmp(name, g_NickName.get()) == 0) {
						Copy_Player_Name(NatPeerNames[local_count++], ordered[i]->Name);
					}
				}
			}
		}
	}

	if (PortNumberOverride) {
		DebugString("Using %d for local port\n", PortNumberOverride);
		Players[0]->Address.Set_Port(PortNumberOverride);
		Slots[Player_To_Slot(0)]->Address.Set_Port(PortNumberOverride);
	}

	for (i = 0; i < Players.Count(); i++) {
		PlayerPorts[i] = Players[i]->Address.Get_Port();
		if (PlayerPorts[i] == 0) {
			PlayerPorts[i] = 1234;
		}
	}

	IPXAddressClass own_address = Players[0]->Address;
	NetNodeType own_node;
	own_address.Get_Node(own_node);

	for (i = 1; i < Players.Count(); i++) {
		if (own_address.Is_Same_Node(Players[i]->Address)) {
			DebugString("%s has the same IP address as me\n", Wide_To_Narrow(Players[i]->Name));

			for (int j = 0; j < MAX_PLAYERS; j++) {
				NodeNameType * node = Slots[j];
				char * name = Wide_To_Narrow(Players[i]->Name);
				char * peer_name = Wide_To_Narrow(node->Name);

				if (stricmp(name, peer_name) == 0) {
					NetNodeType local_node;
					SlotAddresses[j].Get_Node(local_node);
					Players[i]->Address.Set_Address(NULL, local_node, Players[i]->Address.Get_Port());

					name = Wide_To_Narrow(Players[i]->Name);
					DebugString("Talking to NAT player %s on IP address %s\n", name, Players[i]->Address.As_String(NULL));
				}
			}
		}
	}

	if (PacketTransport) {
		((UDPInterfaceClass *)PacketTransport)->Close_All_Port_Sockets();
		((UDPInterfaceClass *)PacketTransport)->OpenSocket(PlayerPorts[0]);
		PacketTransport->Start_Listening();
	}
}
#endif




























































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

template <>
VectorClass<DiskID>::VectorClass(int size, DiskID const * array) :
	Vector(0),
	VectorMax(size),
	IsValid(true),
	IsAllocated(false)
{
	/*
	**	Allocate the vector. The default constructor will be called for every
	**	object in this vector.
	*/
	if (size) {
		if (array) {
			Vector = new((void*)array) DiskID[size];
		} else {
			Vector = new DiskID[size];
			IsAllocated = true;
		}
	}
}

template <>
bool DynamicVectorClass<DiskID>::Resize(int newsize, DiskID const * array)
{
	if (BASECLASS::Resize(newsize, array)) {
		if (Length() < ActiveCount) ActiveCount = Length();
		return(true);
	}
	return(false);
}

template <>
int DynamicVectorClass<DiskID>::ID(DiskID const & object)
{
	for (int index = 0; index < Count(); index++) {
		if ((*this)[index] == object) return(index);
	}
	return(-1);
}

template <>
VectorClass<Wstring>::VectorClass(int size, Wstring const * array) :
	Vector(0),
	VectorMax(size),
	IsValid(true),
	IsAllocated(false)
{
	/*
	**	Allocate the vector. The default constructor will be called for every
	**	object in this vector.
	*/
	if (size) {
		if (array) {
			Vector = new((void*)array) Wstring[size];
		} else {
			Vector = new Wstring[size];
			IsAllocated = true;
		}
	}
}

template <>
bool DynamicVectorClass<Wstring>::Resize(int newsize, Wstring const * array)
{
	if (BASECLASS::Resize(newsize, array)) {
		if (Length() < ActiveCount) ActiveCount = Length();
		return(true);
	}
	return(false);
}

template <>
int DynamicVectorClass<Wstring>::ID(Wstring const & object)
{
	for (int index = 0; index < Count(); index++) {
		if ((*this)[index] == object) return(index);
	}
	return(-1);
}
