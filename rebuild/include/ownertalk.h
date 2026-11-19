/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

#include <windows.h> // RA2: HANDLE
#include "vector.h"
#include "ipxaddr.h"

/*
**	RA2: talks to the packet router that relays the packets of an internet game.
*/
class OwnerTalkClass
{
	public:
#pragma pack(push, 1)
		struct ConnectionListStruct { // RA2: 42 bytes, one peer of the game as the packet router is told of it (the router's player list holds eight)
			char ID; // RA2: +0x00, the player's house index (the id Mask_From_ID takes)
			char Name[20]; // RA2: +0x01, the player's name (Build_Connection_List copies at most 20 characters of it)
			char Reserved[12]; // RA2: +0x15, never written by this module
			unsigned long Address; // RA2: +0x21, the four bytes of the player's address (Get_Address node)
			unsigned short Port; // RA2: +0x25, the player's port (Session.PlayerPorts)
			bool IsMaster; // RA2: +0x27, set for the player Session.MasterPlayerID names
			unsigned char Mask; // RA2: +0x28, 1 << ID, the bit that names the player in a recipient mask
			bool IsActive; // RA2: +0x29, cleared when the player is removed
		};
#pragma pack(pop)

		OwnerTalkClass(void); // RA2: 0x0063F040
		~OwnerTalkClass(void); // RA2: 0x0063F0C0
		void Reset(void); // RA2: 0x0063F070, clears the connection state before a network session starts (Start_Network_Session)
		char Mask_From_Address(IPXAddressClass * address); // RA2: 0x0063F430, the recipient mask of the connection with that address (YR's name); -1 for none or a broadcast
		unsigned char Mask_From_ID(int id);
		void Handle_Router_Messages(void); // RA2: 0x0063F840, turns the router's change-of-address notices into events (YR's name)
		void Set_Address(int id, IPXAddressClass * address); // RA2: 0x0063F9A0, stores the address of the connection of that id (YR's name)
		char * Name_From_Index(int index); // RA2: 0x0063FB40, the name of the connection at an index (YR's name)
		IPXAddressClass * Address_From_Name(char const * name); // RA2: 0x0063FA00, the address of the connection of that name (YR's name)
		unsigned short Port_From_Name(char const * name); // RA2: 0x0063FAF0, its port (YR's name)
		void Set_Port(int id, unsigned short port); // RA2: 0x0063FB60, stores the port of the connection of that id (YR's name)
		int Update_Master(void); // RA2: 0x0063FBA0, marks the master connection and starts the router when the local player became master (YR's name)
		bool Send_Player_List(void); // RA2: 0x0063F130, sends the connection list to the packet router; false if it fails
		void Build_Connection_List(void); // RA2: 0x0063F2A0, fills the connection list from the session players
		bool Start_Router(unsigned char * node); // RA2: 0x0063F510, starts the packet router process (the caller passes NULL)
		void Send_Shutdown(void); // RA2: 0x0063FC90; tells the packet router to shut down (Disconnect_Gracefully)
		bool Wait_For_Router_Shutdown(unsigned long timeout, bool force); // RA2: 0x0063FD70
		void Send_Remove_Player(int id); // RA2: 0x0063FE20, marks the connection inactive and tells the router (YR's name)
		void Send_Port_Blitz(char port); // RA2: 0x0063FEB0, tells the router to try ports (debug text TALK_PORT_BLITZ; YR's name)

		DynamicVectorClass<ConnectionListStruct *> Connections; // RA2: +0x00, the connections the router relays (0x18 bytes)
		bool IsRouterRunning; // RA2: +0x18, set when the router answers TALK_HELLO (Start_Router); the sends of the other functions test it
		HANDLE RouterProcess; // RA2: +0x1C, the packet router's process handle; non-NULL while a router runs (0x00525DC0)
};

extern OwnerTalkClass OwnerTalk;
