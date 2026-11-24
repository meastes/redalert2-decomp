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

/*
**	RA2: the network setup helpers of Tiberian Sun's NETDLG.CPP that RA2 still has at 0x005BB540:
**	starting and stopping the packet transport, dropping a player's connection, the CRC of a game
**	name and clearing a list box. RA2 moved the dialogs of that file to netdlg2.cpp.
*/

#include "always.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include <string.h> // RA2: the includes this file needs
#include <stdio.h> // RA2: the includes this file needs
#include <wchar.h> // RA2: the includes this file needs
#undef LoadString
#include "_map.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "event.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "houstype.h" // RA2: the includes this file needs
#include "ipxmgr.h" // RA2: the includes this file needs
#include "list.h" // RA2: the includes this file needs
#include "msglist.h" // RA2: the includes this file needs
#include "netdlg.h"
#include "ownertalk.h" // RA2: OwnerTalk
#include "queue.h" // RA2: Add_CRC and OutList
#include "rules.h" // RA2: the includes this file needs
#include "session.h" // RA2: the includes this file needs
#include "stringtable.h" // RA2: the includes this file needs
#include "wonline.h" // RA2: WestwoodOnline_Tournament
#include "wsproto.h" // RA2: PacketTransport, deleted by Shutdown_Network

/// <summary>
/// Initializes network stuff: starts IPX and, when a bridge was given on the command line, tells it which network lies behind it.
/// </summary>
bool Init_Network(void)
{
	NetNumType net;
	NetNodeType node;

	if (!Ipx.Init()) { // RA2: no assert on PacketTransport; Init alone is called (0x005251C0)
		return(false);
	}

	if (Session.Type != GAME_INTERNET) { // RA2: the type is compared with 4, as in TibSun
		if (Session.IsBridge) { // RA2: the flag at Session +0x236C, tested before BridgeNet (+0x2370) is read
			Session.BridgeNet.Get_Address(net, node);
			Ipx.Set_Bridge(net);
		}
	}

	return(true);
}

/// <summary>
/// Shuts down network stuff: forgets the game name and deletes the packet transport.
/// </summary>
void Shutdown_Network(void)
{
	Session.GameName[0] = 0; // RA2: the game name is wide, so a 16 bit zero is stored at Session +0x2380

	if (PacketTransport) { // RA2: as in TibSun; the delete calls the virtual destructor with the delete flag
		delete PacketTransport;
	}
	PacketTransport = NULL;
}

/// <summary>
/// Destroys the given connection. id is the house of the player on it; error 0 = signed off, 1 = connection error.
/// </summary>
void Destroy_Connection(int id, int error)
{
	int i;
	HouseClass *housep;
	wchar_t txt[80];

	housep = Houses[id]; // RA2: the house is fetched before the debug print and the null test

	if (Debug_Print_Events) {
		DebugString("Destroying connection for house %d (%ls)\n", id, housep->UIName); // RA2: the house name is the wide UIName (+0x15F16)
	}

	if (!housep || !housep->IsHuman)
		return;

	txt[0] = L'\0'; // RA2: the text is wide (80 wchar_t at esp+28)
	if (error == 1) {
		swprintf(txt, StringTable::LoadString("TXT_CONNECTION_LOST", NULL, "D:\\Ra2\\NetDlg.CPP", 463), housep->UIName);
	} else if (error == 0) {
		swprintf(txt, StringTable::LoadString("TXT_LEFT_GAME", NULL, "D:\\Ra2\\NetDlg.CPP", 465), housep->UIName);
	}

	if (wcslen(txt)) { // RA2: wcslen of the wide text
		Session.Messages.Add_Message(NULL, 0, txt, housep->Class->Scheme, TextPrintType(TPF_6PT_GRAD | TPF_FULLSHADOW | TPF_USE_GRAD_PAL), int(Rule->MessageDelay * TICKS_PER_MINUTE), false); // RA2: the sender name is NULL, the colour scheme is the house type's (+0xC0), Add_Message takes a final flag (false)
		Map.Flag_To_Redraw();
	}

	Ipx.Delete_Connection(id); // RA2: the connection is deleted before the player is dropped from the list, which TibSun does first

	if (error) { // RA2: the error branch tests the frame: before the first frame the house dies at once
		if (Frame == 0) {
			housep->Flag_To_Die();
		} else {
			OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::REMOVEPLAYER, id)); // RA2: the REMOVEPLAYER event (0x23) is queued, as in TibSun
		}
	} else {
		if (Session.Type == GAME_INTERNET && WestwoodOnline_Tournament) { // RA2: a clean sign-off; no AI_Takeover any more
			housep->Flag_To_Die();
		}
		if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) { // RA2: the byte at Session +0x2910 (0x00A3FBA8); the router is told the player left
			OwnerTalk.Send_Remove_Player(id);
		}
	}

	if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter && Session.Am_I_Master(housep)) { // RA2: the leaver was the game master: choose a new host
		Session.AboutToExitActive = true; // RA2: the byte at Session +0x3040 (0x00A402D8) is set to 1 here; the name is the one the other writer (event.cpp) gave it
		int host = OwnerTalk.Update_Master(); // RA2: the new master's house, -1 when none
		if (host != -1) {
			HouseClass * hostp = Houses[host];
			if (hostp) {
				if (Session.Players.Count() > 2) { // RA2: with two players left no router is needed
					IPXAddressClass *address = NULL; // RA2: the new host's address
					for (i = 0; i < Session.Players.Count(); i++) {
						if (Session.Players[i]->Player.ID == host) { // RA2: the ID (+0x5F) is compared, not the name
							address = &Session.Players[i]->Address;
							Session.PacketRouterPort = Session.PlayerPorts[i]; // RA2: the router port is that of the same slot (Session +0x3022)
							break;
						}
					}
					NetNumType network;
					NetNodeType node;
					address->Get_Address(network, node);
					Session.PacketRouterAddress.Set_Address(network, node, 0); // RA2: the address is copied to the router address (+0x2914) with port 0
					DebugString("Session.PacketRouterAddress = %s\n", Session.PacketRouterAddress.As_String(0)); // RA2: the format string at 0x007D696C
					DebugString("Session.PacketRouterPort = %d\n", Session.PacketRouterPort);
					Ipx.Set_Lobby_Address(&Session.PacketRouterAddress); // RA2: IPX sends lobby traffic to the router (0x005277C0)
				}
				wcscpy(Session.MasterPlayerName, hostp->UIName); // RA2: the new master's wide name is copied to Session +0x2FC8
				DebugString("New game host is %ls\n", Session.MasterPlayerName);
				Session.MasterPlayerID = host;
				if (Session.Am_I_Master(NULL)) { // RA2: when this system became the master, the router gets the player list
					OwnerTalk.Send_Player_List();
				}
			}
		}
	}

	for (i = 0; i < Session.Players.Count(); i++) { // RA2: the player is found by ID (+0x5F), not by name
		if (Session.Players[i]->Player.ID == id) {
			delete Session.Players[i];
			Session.Players.Delete(Session.Players[i]);
			Session.Remove_Player_Port(i); // RA2: the port slot of that player is released (0x00672F20)
			if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {
				OwnerTalk.Send_Remove_Player(id);
			}
			break;
		}
	}

	Session.NumPlayers--; // RA2: no "just you and me" message any more
}

/// <summary>
/// Computes the CRC of an upper-cased name (a narrow game name).
/// </summary>
unsigned int Compute_Name_CRC(char *name)
{
	char buf[80];
	unsigned int crc = 0L;
	int i;

	strcpy(buf, name);
	_strupr(buf);

	for (i = 0; (unsigned)i < strlen(buf); i++) {
		Add_CRC(&crc, (unsigned int)buf[i]);
	}

	return(crc);
}

/// <summary>
/// Computes the CRC of an upper-cased name (a wide game name, which is what the session keeps).
/// </summary>
unsigned int __fastcall Compute_Name_CRC(wchar_t const * name) // RA2: the wide form, 0x005BBA50 (the other files declare it with this prototype)
{
	wchar_t buf[80]; // RA2: 80 wchar_t
	unsigned int crc = 0L;
	unsigned int i;

	wcscpy(buf, name);
	_wcsupr(buf);

	for (i = 0; i < wcslen(buf); i++) { // RA2: i is unsigned, the compare is unsigned (jb)
		Add_CRC(&crc, (unsigned int)buf[i]); // RA2: the 16 bit character is zero extended (no sign extension as in the narrow form)
	}

	return(crc);
}

/// <summary>
/// Clears the given list box: deletes every item (a wide string buffer), then flags the list to redraw.
/// </summary>
void Clear_Listbox(ListClass * list)
{
	wchar_t const * item; // RA2: the items are wide strings

	while (list->Count()) {
		item = list->Get_Item(0);
		list->Remove_Item(item); // RA2: the overload taking the text (vtable +0xAC), not the index
		delete[] (wchar_t *)item;
	}
	list->Flag_To_Redraw();
}
