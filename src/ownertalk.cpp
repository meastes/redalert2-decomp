/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the connection to the packet router of an internet game (the process mph.exe): the table of the players'
**	names, addresses and ports, and the messages that tell the router who plays. TibSun has no such file; the code
**	is the executable's from 0x0063ED70.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "_timer.h" // RA2: the includes this file needs
#include "conquer.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "event.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "ipxaddr.h" // RA2: the includes this file needs
#include "ipxmgr.h" // RA2: the includes this file needs
#include "session.h" // RA2: the includes this file needs
#include "ownertalk.h" // RA2: the includes this file needs
#include "queue.h" // RA2: the includes this file needs
#include "win.h" // RA2: MainWindow
#include "wsproto.h" // RA2: the includes this file needs
#include "_WSProto.h" // RA2: PacketTransport


#pragma pack(push, 1)
struct TalkPacketType { // RA2: the message to the router: 0x152 bytes, the command, one parameter and the table of players
	unsigned char Command; // RA2: 1 player list, 2 remove player, 3 shut down, 6 port blitz; the router answers 4 (hello) and 5 (address change)
	unsigned char Param; // RA2: the number of players, the id of the removed player or the port
	OwnerTalkClass::ConnectionListStruct Entries[8]; // RA2: the table of players (0x2A bytes each)
};
#pragma pack(pop)

#pragma pack(push, 1)
struct RouterMessageType { // RA2: what the router sends: a command, then for a change of address the player's id and its new address
	unsigned char Command; // RA2: 5 for a change of address
	signed char ID; // RA2: the player whose address changed
	unsigned long Address; // RA2: the player's new address
	unsigned char Pad[0x3F8]; // RA2: the receive buffer is 1024 bytes
};
#pragma pack(pop)

OwnerTalkClass OwnerTalk; // RA2: 0x00AB6D18; the one connection to the router (its initializer follows the geometry constants, with the destructor registered by it)

RouterMessageType RouterMessage; // RA2: the buffer Handle_Router_Messages receives into (1024 bytes)

/// <summary>
/// Makes a connection table with no players and no router running.
/// </summary>
OwnerTalkClass::OwnerTalkClass(void) : // RA2: 0x0063F040
	IsRouterRunning(false),
	RouterProcess(NULL)
{
}

/// <summary>
/// Forgets every player of the table and the running router.
/// </summary>
void OwnerTalkClass::Reset(void) // RA2: 0x0063F070
{
	while (Connections.Count() != 0) {
		delete Connections[0];
		Connections.Delete_Index(0);
	}
	IsRouterRunning = false;
	RouterProcess = NULL;
}

/// <summary>
/// Frees the players of the table.
/// </summary>
OwnerTalkClass::~OwnerTalkClass(void) // RA2: 0x0063F0C0
{
	while (Connections.Count() != 0) {
		delete Connections[0];
		Connections.Delete_Index(0);
	}
}

/// <summary>
/// Sends the table of players to the router and waits (at most 900 ticks) until the message is out.
/// A player the local connection manager does not know and who is not the local player goes out as inactive.
/// </summary>
bool OwnerTalkClass::Send_Player_List(void) // RA2: 0x0063F130
{
	TalkPacketType packet = { 0 };
	CDTimerClass<SystemTimerClass> timer;

	if (!IsRouterRunning) {
		return(false);
	}
	DebugString("About to send master player list to router\n");
	packet.Command = 1;
	packet.Param = Connections.Count();
	for (int i = 0; i < Connections.Count(); i++) {
		packet.Entries[i] = *Connections[i];
		if (Ipx.Connection_Index(Connections[i]->ID) == -1 && PlayerPtr->HeapID != Connections[i]->ID) {
			DebugString("Marking player %s as inactive\n", Connections[i]->Name);
			packet.Entries[i].IsActive = false;
			packet.Entries[i].IsMaster = false;
		}
	}
	Ipx.Send_Lobby_Message(&packet, 0x152, true);
	DebugString("Player list sent to router\n");
	timer = 900;
	while (Ipx.Lobby_Num_Send() > 0 && timer != 0) {
		Call_Back();
	}
	return(true);
}

/// <summary>
/// Rebuilds the table from the human houses that are still in the game: id, name, address and port of the matching
/// session player, whether it is the master, and its mask bit.
/// </summary>
void OwnerTalkClass::Build_Connection_List(void) // RA2: 0x0063F2A0
{
	while (Connections.Count() != 0) {
		delete Connections[0];
		Connections.Delete_Index(0);
	}
	for (int i = 0; i < Houses.Count(); i++) {
		HouseClass * house = Houses[i];
		if (house->Is_Human_Player() && !house->IsToDie) {
			ConnectionListStruct * entry = new ConnectionListStruct;
			entry->ID = i;
			strncpy(entry->Name, Wide_To_Narrow(house->UIName), 20);
			for (int j = 0; j < Session.Players.Count(); j++) {
				if (Session.Players[j]->Player.ID == i) {
					NetNumType net;
					NetNodeType node;
					Session.Players[j]->Address.Get_Address(net, node);
					entry->Address = *(unsigned long *)node;
					entry->Port = Session.PlayerPorts[j];
					break;
				}
			}
			entry->IsActive = true;
			entry->IsMaster = (Session.MasterPlayerID == house->HeapID);
			entry->Mask = 1 << i;
			Connections.Add(entry);
		}
	}
}

/// <summary>
/// Finds the mask bit of the player with an address; everyone for no address or a broadcast, nobody when unknown.
/// </summary>
char OwnerTalkClass::Mask_From_Address(IPXAddressClass * address) // RA2: 0x0063F430
{
	if (address == NULL) {
		return(-1);
	}
	if (address->Is_Broadcast()) {
		return(-1);
	}
	NetNumType net;
	NetNodeType node;
	char mask = 0;
	address->Get_Address(net, node);
	for (int i = 0; i < Connections.Count(); i++) {
		if (*(unsigned long *)node == Connections[i]->Address) {
			mask = Connections[i]->Mask;
			break;
		}
	}
	return(mask);
}

/// <summary>
/// Finds the mask bit of the player with an id; everyone for -1 or an unknown id.
/// </summary>
unsigned char OwnerTalkClass::Mask_From_ID(int id) // RA2: 0x0063F4C0
{
	unsigned char mask = 0;

	if (id == -1) {
		return(0xFF);
	}
	for (int i = 0; i < Connections.Count(); i++) {
		if (id == Connections[i]->ID) {
			mask = Connections[i]->Mask;
			break;
		}
	}
	if (mask == 0) {
		mask = 0xFF;
	}
	return(mask);
}

/// <summary>
/// Starts mph.exe with the address the router listens on and waits (at most 1800 ticks) for its hello. A router
/// that runs for another player gets that player's address as the one packets are translated to.
/// </summary>
bool OwnerTalkClass::Start_Router(unsigned char * node) // RA2: 0x0063F510
{
	CDTimerClass<SystemTimerClass> timer;
	IPXAddressClass address;
	STARTUPINFO startup;
	PROCESS_INFORMATION process;
	char command[256];
	char buffer[1024];
	int length;
	unsigned short product_id;

	DebugString("Starting packet router process\n");
	GetStartupInfo(&startup);
	if (!PacketTransport->Get_Num_Local_Addresses()) {
		return(false);
	}
	if (node == NULL) {
		if (Session.Players.Count() == 0) {
			unsigned char * local = PacketTransport->Get_Local_Address(0);
			sprintf(command, "mph.exe %d.%d.%d.%d", local[0], local[1], local[2], local[3]);
		} else {
			IPXAddressClass * player = &Session.Players[0]->Address;
			if (player != NULL) {
				NetNumType net;
				NetNodeType host;
				player->Get_Address(net, host);
				sprintf(command, "mph.exe %d.%d.%d.%d", host[0], host[1], host[2], host[3]);
				Session.PacketRouterAddressTranslate = *player;
				Session.PacketRouterPort = 0;
				DebugString("Session.PacketRouterAddressTranslate = %s\n", Session.PacketRouterAddressTranslate.As_String());
			}
		}
	} else {
		sprintf(command, "mph.exe %d.%d.%d.%d", node[0], node[1], node[2], node[3]);
		IPXAddressClass * player = &Session.Players[0]->Address;
		if (player != NULL) {
			NetNumType net;
			NetNodeType host;
			player->Get_Address(net, host);
			host[0] = node[0];
			host[1] = node[1];
			host[2] = node[2];
			host[3] = node[3];
			Session.PacketRouterAddressTranslate.Set_Address(net, host, 0);
			Session.PacketRouterAddressTranslate.Set_Port(Session.Players[0]->Address.Get_Port());
			Session.PacketRouterPort = Session.Players[0]->Address.Get_Port();
			Session.PacketRouterAddress = Session.Players[0]->Address;
			DebugString("Session.PacketRouterAddressTranslate = %s\n", Session.PacketRouterAddressTranslate.As_String());
		}
	}
	BOOL result = CreateProcess("mph.exe", command, NULL, NULL, FALSE, 0x200, NULL, NULL, &startup, &process);
	if (!result) {
		DebugString("CreateProcess failed with error %d, %s\n", GetLastError(), result);
	} else {
		RouterProcess = process.hProcess;
		DebugString("hRouterProcess = %d\n", process.hProcess);
		SetFocus(MainWindow);
		timer = 1800;
		result = FALSE;
		while (timer) {
			Call_Back();
			if (Ipx.Get_Lobby_Message(buffer, &length, &address, &product_id) && buffer[0] == 4) {
				result = TRUE;
				DebugString("Received TALK_HELLO from router\n");
				IsRouterRunning = true;
				break;
			}
		}
	}
	return(result != FALSE);
}

/// <summary>
/// Reads the messages the router sent; a change of address becomes an event for the other players.
/// </summary>
void OwnerTalkClass::Handle_Router_Messages(void) // RA2: 0x0063F840
{
	IPXAddressClass address;
	int length;
	unsigned short product_id;

	if (Frame > 0) {
		while (Ipx.Get_Lobby_Message(&RouterMessage, &length, &address, &product_id)) {
			if (RouterMessage.Command != 5) {
				DebugString("Got spurious message %d from the router\n", RouterMessage.Command);
			} else {
				DebugString("Received change of address notification.\n");
				int id = RouterMessage.ID;
				NetNumType net;
				NetNodeType node;
				memset(node, 0, sizeof(node));
				*(unsigned long *)node = RouterMessage.Address;
				IPXAddressClass new_address;
				new_address.Set_Address(net, node, 0);
				OutList.Add(EventClass(PlayerPtr->HeapID, new_address, id));
			}
		}
	}
}

/// <summary>
/// Stores the address of the player with an id.
/// </summary>
void OwnerTalkClass::Set_Address(int id, IPXAddressClass * address) // RA2: 0x0063F9A0
{
	NetNumType net;
	NetNodeType node;

	address->Get_Address(net, node);
	for (int i = 0; i < Connections.Count(); i++) {
		if (Connections[i]->ID == id) {
			Connections[i]->Address = *(unsigned long *)node;
			break;
		}
	}
}

/// <summary>
/// Finds the address and port of the player with a name (a zero address when there is none). The result is a
/// static object that the next call overwrites.
/// </summary>
IPXAddressClass * OwnerTalkClass::Address_From_Name(char const * name) // RA2: 0x0063FA00
{
	static IPXAddressClass address; // RA2: the object returned (its initializer is the one after the geometry constants)
	NetNodeType node;

	memset(node, 0, sizeof(node));

	address.Set_Node(node);
	address.Set_Port(0);
	for (int i = 0; i < Connections.Count(); i++) {
		if (stricmp(Connections[i]->Name, name) == 0) {
			NetNodeType found;

			*(unsigned long *)found = Connections[i]->Address;
			address.Set_Node(found);
			address.Set_Port(Connections[i]->Port);
			break;
		}
	}
	return(&address);
}

/// <summary>
/// Finds the port of the player with a name; 0 when there is none.
/// </summary>
unsigned short OwnerTalkClass::Port_From_Name(char const * name) // RA2: 0x0063FAF0
{
	for (int i = 0; i < Connections.Count(); i++) {
		if (stricmp(Connections[i]->Name, name) == 0) {
			return(Connections[i]->Port);
		}
	}
	return(0);
}

/// <summary>
/// Gives the name of the player at an index of the table; NULL past its end.
/// </summary>
char * OwnerTalkClass::Name_From_Index(int index) // RA2: 0x0063FB40
{
	if (index < Connections.Count()) {
		return(Connections[index]->Name);
	}
	return(NULL);
}

/// <summary>
/// Stores the port of the player with an id.
/// </summary>
void OwnerTalkClass::Set_Port(int id, unsigned short port) // RA2: 0x0063FB60
{
	for (int i = 0; i < Connections.Count(); i++) {
		if (Connections[i]->ID == id) {
			Connections[i]->Port = port;
			break;
		}
	}
}

/// <summary>
/// Marks the master of the table (the player the session names, else the first active player that is none yet) and,
/// when that is the local player of a game with more than two players and the local player is not the session's
/// master, starts the router here. Returns the master's id.
/// </summary>
int OwnerTalkClass::Update_Master(void) // RA2: 0x0063FBA0
{
	int id = Session.LastTimingID; // RA2: +0x303C of the session, -1 when none; main names it LastTimingID, the code reads it as the master's id
	int i;

	if (id == -1) {
		for (i = 0; i < Connections.Count(); i++) {
			if (Connections[i]->IsActive && !Connections[i]->IsMaster) {
				id = Connections[i]->ID;
				Connections[i]->IsMaster = true;
			}
		}
	} else {
		for (i = 0; i < Connections.Count(); i++) {
			if (Connections[i]->ID == id) {
				Connections[i]->IsMaster = true;
			} else {
				Connections[i]->IsMaster = false;
			}
		}
	}
	if (!Session.Am_I_Master(NULL) && id != -1 && id == PlayerPtr->HeapID && Session.Players.Count() > 2) {
		IPXAddressClass * address = NULL;
		NetNumType net;
		NetNodeType node;
		for (i = 0; i < Session.Players.Count(); i++) {
			if (Session.Players[i]->Player.ID == id) {
				address = &Session.Players[i]->Address;
				break;
			}
		}
		address->Get_Address(net, node);
		Start_Router(node);
	}
	return(id);
}

/// <summary>
/// Tells the router to shut down and waits (at most 900 ticks) until the message is out.
/// </summary>
void OwnerTalkClass::Send_Shutdown(void) // RA2: 0x0063FC90
{
	TalkPacketType packet = { 0 };
	CDTimerClass<SystemTimerClass> timer;

	if (IsRouterRunning && RouterProcess != NULL) {
		packet.Command = 3;
		Ipx.Send_Lobby_Message(&packet, 0x152, true);
		timer = 900;
		while (Ipx.Lobby_Num_Send() > 0 && timer != 0) {
			Call_Back();
		}
	}
}

/// <summary>
/// Waits for the router process to end; with force, a router that is still running after the timeout is terminated.
/// Returns false when the router still runs.
/// </summary>
bool OwnerTalkClass::Wait_For_Router_Shutdown(unsigned long timeout, bool force) // RA2: 0x0063FD70
{
	DebugString("Wait_For_Router_Shutdown - hRouterProcess = %d\n", RouterProcess);
	if (RouterProcess != NULL) {
		if (WaitForMultipleObjects(1, &RouterProcess, FALSE, timeout) == WAIT_TIMEOUT) {
			if (!force) {
				return(false);
			}
			DebugString("***Router process still running - shutting it down***\n");
			BOOL result = TerminateProcess(RouterProcess, 0);
			if (!result) {
				DebugString("Failed to shut down router - error code %d - %s\n", GetLastError(), result);
			} else {
				RouterProcess = NULL;
			}
			return(result ? true : false);
		}
		DebugString("Router not running\n");
		RouterProcess = NULL;
	}
	return(true);
}

/// <summary>
/// Marks the player with an id as gone and tells the router.
/// </summary>
void OwnerTalkClass::Send_Remove_Player(int id) // RA2: 0x0063FE20
{
	TalkPacketType packet = { 0 };

	for (int i = 0; i < Connections.Count(); i++) {
		ConnectionListStruct * entry = Connections[i];
		if (entry->ID == id) {
			entry->IsActive = false;
			entry->IsMaster = false;
		}
	}
	if (IsRouterRunning) {
		packet.Command = 2;
		packet.Param = id;
		DebugString("Sending TALK_REMOVE_PLAYER to router\n");
		Ipx.Send_Lobby_Message(&packet, 0x152, true);
	}
}

/// <summary>
/// Tells the router to blitz a port.
/// </summary>
void OwnerTalkClass::Send_Port_Blitz(char port) // RA2: 0x0063FEB0
{
	TalkPacketType packet = { 0 };

	if (IsRouterRunning) {
		packet.Command = 6;
		packet.Param = port;
		DebugString("Sending TALK_PORT_BLITZ to router\n");
		Ipx.Send_Lobby_Message(&packet, 0x152, true);
	}
}
