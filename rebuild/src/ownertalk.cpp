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


#pragma pack(push, 1) // RA2: as above
struct TalkPacketType { // RA2: the message to the router: 0x152 bytes, the command, one parameter and the table of players
	unsigned char Command; // RA2: 1 player list, 2 remove player, 3 shut down, 6 port blitz; the router answers 4 (hello) and 5 (address change)
	unsigned char Param; // RA2: the number of players, the id of the removed player or the port
	OwnerTalkClass::ConnectionListStruct Entries[8]; // RA2: the table of players (0x2A bytes each)
};
#pragma pack(pop) // RA2: as above

#pragma pack(push, 1) // RA2: as above
struct RouterMessageType { // RA2: what the router sends: a command, then for a change of address the player's id and its new address
	unsigned char Command; // RA2: 5 for a change of address
	signed char ID; // RA2: the player whose address changed
	unsigned long Address; // RA2: the player's new address
	unsigned char Pad[0x3F8]; // RA2: the receive buffer is 1024 bytes
};
#pragma pack(pop) // RA2: as above

OwnerTalkClass OwnerTalk; // RA2: 0x00AB6D18; the one connection to the router (its initializer follows the geometry constants, with the destructor registered by it)

RouterMessageType RouterMessage; // RA2: the buffer Handle_Router_Messages receives into (1024 bytes)

/// <summary>
/// Makes a connection table with no players and no router running.
/// </summary>
OwnerTalkClass::OwnerTalkClass(void) : // RA2: 0x0063F040
	IsRouterRunning(false), // RA2: as above
	RouterProcess(NULL) // RA2: as above
{
}

/// <summary>
/// Forgets every player of the table and the running router.
/// </summary>
void OwnerTalkClass::Reset(void) // RA2: 0x0063F070
{
	while (Connections.Count() != 0) { // RA2: as above
		delete Connections[0]; // RA2: as above
		Connections.Delete_Index(0); // RA2: as above
	}
	IsRouterRunning = false; // RA2: as above
	RouterProcess = NULL; // RA2: as above
}

/// <summary>
/// Frees the players of the table.
/// </summary>
OwnerTalkClass::~OwnerTalkClass(void) // RA2: 0x0063F0C0
{
	while (Connections.Count() != 0) { // RA2: as above
		delete Connections[0]; // RA2: as above
		Connections.Delete_Index(0); // RA2: as above
	}
}

/// <summary>
/// Sends the table of players to the router and waits (at most 900 ticks) until the message is out.
/// A player the local connection manager does not know and who is not the local player goes out as inactive.
/// </summary>
bool OwnerTalkClass::Send_Player_List(void) // RA2: 0x0063F130
{
	TalkPacketType packet = {0}; // RA2: as above
	CDTimerClass<SystemTimerClass> timer; // RA2: as above

	if (!IsRouterRunning) { // RA2: as above
		return(false); // RA2: as above
	}
	DebugString("About to send master player list to router\n"); // RA2: as above
	packet.Command = 1; // RA2: as above
	packet.Param = Connections.Count(); // RA2: as above
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		packet.Entries[i] = *Connections[i]; // RA2: as above
		if (Ipx.Connection_Index(Connections[i]->ID) == -1 && PlayerPtr->HeapID != Connections[i]->ID) { // RA2: as above
			DebugString("Marking player %s as inactive\n", Connections[i]->Name); // RA2: as above
			packet.Entries[i].IsActive = false; // RA2: as above
			packet.Entries[i].IsMaster = false; // RA2: as above
		}
	}
	Ipx.Send_Lobby_Message(&packet, 0x152, true); // RA2: as above
	DebugString("Player list sent to router\n"); // RA2: as above
	timer = 900; // RA2: as above
	while (Ipx.Lobby_Num_Send() > 0 && timer != 0) { // RA2: as above
		Call_Back(); // RA2: as above
	}
	return(true); // RA2: as above
}

/// <summary>
/// Rebuilds the table from the human houses that are still in the game: id, name, address and port of the matching
/// session player, whether it is the master, and its mask bit.
/// </summary>
void OwnerTalkClass::Build_Connection_List(void) // RA2: 0x0063F2A0
{
	while (Connections.Count() != 0) { // RA2: as above
		delete Connections[0]; // RA2: as above
		Connections.Delete_Index(0); // RA2: as above
	}
	for (int i = 0; i < Houses.Count(); i++) { // RA2: as above
		HouseClass * house = Houses[i]; // RA2: as above
		if (house->Is_Human_Player() && !house->IsToDie) { // RA2: as above
			ConnectionListStruct * entry = new ConnectionListStruct; // RA2: as above
			entry->ID = i; // RA2: as above
			strncpy(entry->Name, Wide_To_Narrow(house->UIName), 20); // RA2: as above
			for (int j = 0; j < Session.Players.Count(); j++) { // RA2: as above
				if (Session.Players[j]->Player.ID == i) { // RA2: as above
					NetNumType net; // RA2: as above
					NetNodeType node; // RA2: as above
					Session.Players[j]->Address.Get_Address(net, node); // RA2: as above
					entry->Address = *(unsigned long *)node; // RA2: as above
					entry->Port = Session.PlayerPorts[j]; // RA2: as above
					break; // RA2: as above
				}
			}
			entry->IsActive = true; // RA2: as above
			entry->IsMaster = (Session.MasterPlayerID == house->HeapID); // RA2: as above
			entry->Mask = 1 << i; // RA2: as above
			Connections.Add(entry); // RA2: as above
		}
	}
}

/// <summary>
/// Finds the mask bit of the player with an address; everyone for no address or a broadcast, nobody when unknown.
/// </summary>
char OwnerTalkClass::Mask_From_Address(IPXAddressClass * address) // RA2: 0x0063F430
{
	if (address == NULL) { // RA2: as above
		return(-1); // RA2: as above
	}
	if (address->Is_Broadcast()) { // RA2: as above
		return(-1); // RA2: as above
	}
	NetNumType net; // RA2: as above
	NetNodeType node; // RA2: as above
	char mask = 0; // RA2: as above
	address->Get_Address(net, node); // RA2: as above
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (*(unsigned long *)node == Connections[i]->Address) { // RA2: as above
			mask = Connections[i]->Mask; // RA2: as above
			break; // RA2: as above
		}
	}
	return(mask); // RA2: as above
}

/// <summary>
/// Finds the mask bit of the player with an id; everyone for -1 or an unknown id.
/// </summary>
unsigned char OwnerTalkClass::Mask_From_ID(int id) // RA2: 0x0063F4C0
{
	unsigned char mask = 0; // RA2: as above

	if (id == -1) { // RA2: as above
		return(0xFF); // RA2: as above
	}
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (id == Connections[i]->ID) { // RA2: as above
			mask = Connections[i]->Mask; // RA2: as above
			break; // RA2: as above
		}
	}
	if (mask == 0) { // RA2: as above
		mask = 0xFF; // RA2: as above
	}
	return(mask); // RA2: as above
}

/// <summary>
/// Starts mph.exe with the address the router listens on and waits (at most 1800 ticks) for its hello. A router
/// that runs for another player gets that player's address as the one packets are translated to.
/// </summary>
bool OwnerTalkClass::Start_Router(unsigned char * node) // RA2: 0x0063F510
{
	CDTimerClass<SystemTimerClass> timer; // RA2: as above
	IPXAddressClass address; // RA2: as above
	STARTUPINFO startup; // RA2: as above
	PROCESS_INFORMATION process; // RA2: as above
	char command[256]; // RA2: as above
	char buffer[1024]; // RA2: as above
	int length; // RA2: as above
	unsigned short product_id; // RA2: as above

	DebugString("Starting packet router process\n"); // RA2: as above
	GetStartupInfo(&startup); // RA2: as above
	if (!PacketTransport->Get_Num_Local_Addresses()) { // RA2: as above
		return(false); // RA2: as above
	}
	if (node == NULL) { // RA2: as above
		if (Session.Players.Count() == 0) { // RA2: as above
			unsigned char * local = PacketTransport->Get_Local_Address(0); // RA2: as above
			sprintf(command, "mph.exe %d.%d.%d.%d", local[0], local[1], local[2], local[3]); // RA2: as above
		} else { // RA2: as above
			IPXAddressClass * player = &Session.Players[0]->Address; // RA2: as above
			if (player != NULL) { // RA2: as above
				NetNumType net; // RA2: as above
				NetNodeType host; // RA2: as above
				player->Get_Address(net, host); // RA2: as above
				sprintf(command, "mph.exe %d.%d.%d.%d", host[0], host[1], host[2], host[3]); // RA2: as above
				Session.PacketRouterAddressTranslate = *player; // RA2: as above
				Session.PacketRouterPort = 0; // RA2: as above
				DebugString("Session.PacketRouterAddressTranslate = %s\n", Session.PacketRouterAddressTranslate.As_String()); // RA2: as above
			}
		}
	} else { // RA2: as above
		sprintf(command, "mph.exe %d.%d.%d.%d", node[0], node[1], node[2], node[3]); // RA2: as above
		IPXAddressClass * player = &Session.Players[0]->Address; // RA2: as above
		if (player != NULL) { // RA2: as above
			NetNumType net; // RA2: as above
			NetNodeType host; // RA2: as above
			player->Get_Address(net, host); // RA2: as above
			host[0] = node[0]; // RA2: as above
			host[1] = node[1]; // RA2: as above
			host[2] = node[2]; // RA2: as above
			host[3] = node[3]; // RA2: as above
			Session.PacketRouterAddressTranslate.Set_Address(net, host, 0); // RA2: as above
			Session.PacketRouterAddressTranslate.Set_Port(Session.Players[0]->Address.Get_Port()); // RA2: as above
			Session.PacketRouterPort = Session.Players[0]->Address.Get_Port(); // RA2: as above
			Session.PacketRouterAddress = Session.Players[0]->Address; // RA2: as above
			DebugString("Session.PacketRouterAddressTranslate = %s\n", Session.PacketRouterAddressTranslate.As_String()); // RA2: as above
		}
	}
	BOOL result = CreateProcess("mph.exe", command, NULL, NULL, FALSE, 0x200, NULL, NULL, &startup, &process); // RA2: as above
	if (!result) { // RA2: as above
		DebugString("CreateProcess failed with error %d, %s\n", GetLastError(), result); // RA2: as above
	} else { // RA2: as above
		RouterProcess = process.hProcess; // RA2: as above
		DebugString("hRouterProcess = %d\n", process.hProcess); // RA2: as above
		SetFocus(MainWindow); // RA2: as above
		timer = 1800; // RA2: as above
		result = FALSE; // RA2: as above
		while (timer) { // RA2: as above
			Call_Back(); // RA2: as above
			if (Ipx.Get_Lobby_Message(buffer, &length, &address, &product_id) && buffer[0] == 4) { // RA2: as above
				result = TRUE; // RA2: as above
				DebugString("Received TALK_HELLO from router\n"); // RA2: as above
				IsRouterRunning = true; // RA2: as above
				break; // RA2: as above
			}
		}
	}
	return(result != FALSE); // RA2: as above
}

/// <summary>
/// Reads the messages the router sent; a change of address becomes an event for the other players.
/// </summary>
void OwnerTalkClass::Handle_Router_Messages(void) // RA2: 0x0063F840
{
	IPXAddressClass address; // RA2: as above
	int length; // RA2: as above
	unsigned short product_id; // RA2: as above

	if (Frame > 0) { // RA2: as above
		while (Ipx.Get_Lobby_Message(&RouterMessage, &length, &address, &product_id)) { // RA2: as above
			if (RouterMessage.Command != 5) { // RA2: as above
				DebugString("Got spurious message %d from the router\n", RouterMessage.Command); // RA2: as above
			} else { // RA2: as above
				DebugString("Received change of address notification.\n"); // RA2: as above
				int id = RouterMessage.ID; // RA2: as above
				NetNumType net; // RA2: as above
				NetNodeType node; // RA2: as above
				memset(node, 0, sizeof(node)); // RA2: as above
				*(unsigned long *)node = RouterMessage.Address;
				IPXAddressClass new_address; // RA2: as above
				new_address.Set_Address(net, node, 0); // RA2: as above
				OutList.Add(EventClass(PlayerPtr->HeapID, new_address, id)); // RA2: as above
			}
		}
	}
}

/// <summary>
/// Stores the address of the player with an id.
/// </summary>
void OwnerTalkClass::Set_Address(int id, IPXAddressClass * address) // RA2: 0x0063F9A0
{
	NetNumType net; // RA2: as above
	NetNodeType node; // RA2: as above

	address->Get_Address(net, node); // RA2: as above
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (Connections[i]->ID == id) { // RA2: as above
			Connections[i]->Address = *(unsigned long *)node; // RA2: as above
			break; // RA2: as above
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
	NetNodeType node; // RA2: as above

	memset(node, 0, sizeof(node)); // RA2: as above

	address.Set_Node(node); // RA2: as above
	address.Set_Port(0); // RA2: as above
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (stricmp(Connections[i]->Name, name) == 0) { // RA2: as above
			NetNodeType found; // RA2: as above

			*(unsigned long *)found = Connections[i]->Address;
			address.Set_Node(found); // RA2: as above
			address.Set_Port(Connections[i]->Port); // RA2: as above
			break; // RA2: as above
		}
	}
	return(&address); // RA2: as above
}

/// <summary>
/// Finds the port of the player with a name; 0 when there is none.
/// </summary>
unsigned short OwnerTalkClass::Port_From_Name(char const * name) // RA2: 0x0063FAF0
{
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (stricmp(Connections[i]->Name, name) == 0) { // RA2: as above
			return(Connections[i]->Port); // RA2: as above
		}
	}
	return(0); // RA2: as above
}

/// <summary>
/// Gives the name of the player at an index of the table; NULL past its end.
/// </summary>
char * OwnerTalkClass::Name_From_Index(int index) // RA2: 0x0063FB40
{
	if (index < Connections.Count()) { // RA2: as above
		return(Connections[index]->Name); // RA2: as above
	}
	return(NULL); // RA2: as above
}

/// <summary>
/// Stores the port of the player with an id.
/// </summary>
void OwnerTalkClass::Set_Port(int id, unsigned short port) // RA2: 0x0063FB60
{
	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		if (Connections[i]->ID == id) { // RA2: as above
			Connections[i]->Port = port; // RA2: as above
			break; // RA2: as above
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
	int i; // RA2: as above

	if (id == -1) { // RA2: as above
		for (i = 0; i < Connections.Count(); i++) { // RA2: as above
			if (Connections[i]->IsActive && !Connections[i]->IsMaster) { // RA2: as above
				id = Connections[i]->ID; // RA2: as above
				Connections[i]->IsMaster = true; // RA2: as above
			}
		}
	} else { // RA2: as above
		for (i = 0; i < Connections.Count(); i++) { // RA2: as above
			if (Connections[i]->ID == id) { // RA2: as above
				Connections[i]->IsMaster = true; // RA2: as above
			} else { // RA2: as above
				Connections[i]->IsMaster = false; // RA2: as above
			}
		}
	}
	if (!Session.Am_I_Master(NULL) && id != -1 && id == PlayerPtr->HeapID && Session.Players.Count() > 2) { // RA2: as above
		IPXAddressClass * address = NULL; // RA2: as above
		NetNumType net; // RA2: as above
		NetNodeType node; // RA2: as above
		for (i = 0; i < Session.Players.Count(); i++) { // RA2: as above
			if (Session.Players[i]->Player.ID == id) { // RA2: as above
				address = &Session.Players[i]->Address; // RA2: as above
				break; // RA2: as above
			}
		}
		address->Get_Address(net, node); // RA2: as above
		Start_Router(node); // RA2: as above
	}
	return(id); // RA2: as above
}

/// <summary>
/// Tells the router to shut down and waits (at most 900 ticks) until the message is out.
/// </summary>
void OwnerTalkClass::Send_Shutdown(void) // RA2: 0x0063FC90
{
	TalkPacketType packet = {0}; // RA2: as above
	CDTimerClass<SystemTimerClass> timer; // RA2: as above

	if (IsRouterRunning && RouterProcess != NULL) { // RA2: as above
		packet.Command = 3; // RA2: as above
		Ipx.Send_Lobby_Message(&packet, 0x152, true); // RA2: as above
		timer = 900; // RA2: as above
		while (Ipx.Lobby_Num_Send() > 0 && timer != 0) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	}
}

/// <summary>
/// Waits for the router process to end; with force, a router that is still running after the timeout is terminated.
/// Returns false when the router still runs.
/// </summary>
bool OwnerTalkClass::Wait_For_Router_Shutdown(unsigned long timeout, bool force) // RA2: 0x0063FD70
{
	DebugString("Wait_For_Router_Shutdown - hRouterProcess = %d\n", RouterProcess); // RA2: as above
	if (RouterProcess != NULL) { // RA2: as above
		if (WaitForMultipleObjects(1, &RouterProcess, FALSE, timeout) == WAIT_TIMEOUT) { // RA2: as above
			if (!force) { // RA2: as above
				return(false); // RA2: as above
			}
			DebugString("***Router process still running - shutting it down***\n"); // RA2: as above
			BOOL result = TerminateProcess(RouterProcess, 0); // RA2: as above
			if (!result) { // RA2: as above
				DebugString("Failed to shut down router - error code %d - %s\n", GetLastError(), result); // RA2: as above
			} else { // RA2: as above
				RouterProcess = NULL; // RA2: as above
			}
			return(result ? true : false); // RA2: as above
		}
		DebugString("Router not running\n"); // RA2: as above
		RouterProcess = NULL; // RA2: as above
	}
	return(true); // RA2: as above
}

/// <summary>
/// Marks the player with an id as gone and tells the router.
/// </summary>
void OwnerTalkClass::Send_Remove_Player(int id) // RA2: 0x0063FE20
{
	TalkPacketType packet = {0}; // RA2: as above

	for (int i = 0; i < Connections.Count(); i++) { // RA2: as above
		ConnectionListStruct * entry = Connections[i]; // RA2: as above
		if (entry->ID == id) { // RA2: as above
			entry->IsActive = false; // RA2: as above
			entry->IsMaster = false; // RA2: as above
		}
	}
	if (IsRouterRunning) { // RA2: as above
		packet.Command = 2; // RA2: as above
		packet.Param = id; // RA2: as above
		DebugString("Sending TALK_REMOVE_PLAYER to router\n"); // RA2: as above
		Ipx.Send_Lobby_Message(&packet, 0x152, true); // RA2: as above
	}
}

/// <summary>
/// Tells the router to blitz a port.
/// </summary>
void OwnerTalkClass::Send_Port_Blitz(char port) // RA2: 0x0063FEB0
{
	TalkPacketType packet = {0}; // RA2: as above

	if (IsRouterRunning) { // RA2: as above
		packet.Command = 6; // RA2: as above
		packet.Param = port; // RA2: as above
		DebugString("Sending TALK_PORT_BLITZ to router\n"); // RA2: as above
		Ipx.Send_Lobby_Message(&packet, 0x152, true); // RA2: as above
	}
}
