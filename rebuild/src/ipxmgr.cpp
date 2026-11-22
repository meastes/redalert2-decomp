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

/* $Header: /CounterStrike/IPXMGR.CPP 3     10/13/97 2:20p Steve_t $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXMGR.CPP                               *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 20, 1994                        *
 *                                                                         *
 *                  Last Update : May 4, 1995 [BRR]                        *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   IPXManagerClass::IPXManagerClass -- class constructor                 *
 *   IPXManagerClass::~IPXManagerClass -- class destructor                 *
 *   IPXManagerClass::Init -- initialization routine                       *
 *   IPXManagerClass::Is_IPX -- tells if IPX is installed or not           *
 *   IPXManagerClass::Set_Timing -- sets timing for all connections        *
 *   IPXManagerClass::Create_Connection -- creates a new connection        *
 *   IPXManagerClass::Delete_Connection -- deletes a connection            *
 *   IPXManagerClass::Num_Connections -- gets the # of connections         *
 *   IPXManagerClass::Connection_ID -- gets the given connection's ID      *
 *   IPXManagerClass::Connection_Name -- gets name for given connection    *
 *   IPXManagerClass::Connection_Address -- retrieves connection's address *
 *   IPXManagerClass::Connection_Index -- gets given connection's index    *
 *   IPXManagerClass::Set_Connection_Parms -- sets connection's name & id  *
 *   IPXManagerClass::Send_Global_Message -- sends a Global Message        *
 *   IPXManagerClass::Get_Global_Message -- polls the Global Message queue *
 *   IPXManagerClass::Send_Private_Message -- Sends a Private Message      *
 *   IPXManagerClass::Get_Private_Message -- Polls Private Message queue   *
 *   IPXManagerClass::Service -- main polling routine for IPX Connections  *
 *   IPXManagerClass::Get_Bad_Connection -- returns bad connection ID      *
 *   IPXManagerClass::Global_Num_Send  -- gets # entries in send queue     *
 *   IPXManagerClass::Global_Num_Receive -- gets # entries in recv queue   *
 *   IPXManagerClass::Private_Num_Send -- gets # entries in send queue     *
 *   IPXManagerClass::Private_Num_Receive -- gets # entries in recv queue  *
 *   IPXManagerClass::Set_Bridge -- prepares to cross a bridge             *
 *   IPXManagerClass::Set_Socket -- sets socket ID for all connections     *
 *   IPXManagerClass::Response_Time -- Returns largest Avg Response Time   *
 *   IPXManagerClass::Global_Response_Time -- Returns Avg Response Time    *
 *   IPXManagerClass::Reset_Response_Time -- Reset response time           *
 *   IPXManagerClass::Oldest_Send -- gets ptr to oldest send buf           *
 *   IPXManagerClass::Mono_Debug_Print -- debug output routine             *
 *   IPXManagerClass::Alloc_RealMode_Mem -- allocates real-mode memory     *
 *   IPXManagerClass::Free_RealMode_Mem -- frees real-mode memory          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "ipxmgr.h"

#include "_surface.h"
#include "dbgprint.h"
#include "dialog.h"
#include "globals.h"
#include "house.h"
#include "queue.h"
#include "scheme.h"
#include "session.h"
#include "stimer.h"
#include "surface.h"
#include "vector.h"
#include "wspipx.h"
#include "wsproto.h"
#include "wspudp.h"	// RA2

wchar_t * __fastcall Narrow_To_Wide(char const * narrow);	// RA2: 0x006FA180, a static buffer


wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x006FA180, a static buffer

#include "ownertalk.h"	// RA2


/***************************************************************************
 * IPXManagerClass::IPXManagerClass -- class constructor                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      glb_maxlen      Global Channel maximum packet length               *
 *      pvt_maxlen      Private Channel maximum packet length              *
 *      socket         socket ID to use                                    *
 *      product_id      a unique numerical ID for this product             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      The socket number is byte-swapped, since IPX requires socket ID's  *
 *      to be stored high/low.                                             *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
IPXManagerClass::IPXManagerClass (int glb_maxlen, int pvt_maxlen,
	int glb_num_packets, int pvt_num_packets, unsigned short socket,
	unsigned short product_id)
{
	int i;

	/*
	**	Find out if Packet protocol services are available through Winsock.
	*/
	if ( PacketTransport ) {
		delete  PacketTransport;
		PacketTransport = NULL;
	}
	PacketTransport = new IPXInterfaceClass;

	if ( PacketTransport->Init() ){
		IPXStatus = 1;
	}else{
		IPXStatus = 0;
	}
	delete PacketTransport;
	PacketTransport = NULL;

	//........................................................................
	// Set listening state flag to off
	//........................................................................
	Listening = 0;

	//........................................................................
	// Set max packet sizes, for allocating real-mode memory
	//........................................................................
	Glb_MaxPacketLen = glb_maxlen;
	Glb_NumPackets = glb_num_packets;
	Pvt_MaxPacketLen = pvt_maxlen;
	Pvt_NumPackets = pvt_num_packets;
	Lobby_MaxPacketLen = glb_maxlen; // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
	Lobby_NumPackets = pvt_num_packets; // RA2: as above

	//........................................................................
	// Save the app's product ID
	//........................................................................
	ProductID = product_id;

	//........................................................................
	// Save our socket ID number
	//........................................................................
	Socket = (unsigned short)( (((unsigned int)socket & 0x00ff) << 8) |
		(((unsigned int)socket & 0xff00) >> 8));

	//------------------------------------------------------------------------
	// Get the user's IPX local connection number
	//------------------------------------------------------------------------
	ConnectionNum = 0;

	//------------------------------------------------------------------------
	// Init connection states
	//------------------------------------------------------------------------
	NumConnections = 0;
	CurConnection = 0;
	for (i = 0; i < CONNECT_MAX; i++) {
		Connection[i] = 0;
	}
	GlobalChannel = 0;
	LobbyChannel = 0; // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
	MulticastChannel = 0; // RA2: as above

	SendOverflows = 0;
	ReceiveOverflows = 0;
	BadConnection = CONNECTION_NONE;

	//------------------------------------------------------------------------
	// Init timing parameters
	//------------------------------------------------------------------------
	RetryDelta = 2;     // 2 ticks between retries
	MaxRetries = -1;    // disregard # retries
	Timeout = 60;       // report bad connection after 1 second

}	/* end of IPXManagerClass */


/***************************************************************************
 * IPXManagerClass::~IPXManagerClass -- class destructor                   *
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
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
IPXManagerClass::~IPXManagerClass(void)
{
	int i;

	//------------------------------------------------------------------------
	// Stop all IPX events
	//------------------------------------------------------------------------
	if (Listening) {
		IPXConnClass::Stop_Listening();
		Listening = 0;
	}

	//------------------------------------------------------------------------
	// Free all protected-mode memory
	//------------------------------------------------------------------------
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	if (LobbyChannel) { // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
		delete LobbyChannel; // RA2: as above
		LobbyChannel = 0; // RA2: as above
	}
	if (MulticastChannel) { // RA2: as above
		delete MulticastChannel; // RA2: as above
		MulticastChannel = 0; // RA2: as above
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;
}	/* end of ~IPXManagerClass */


/***************************************************************************
 * IPXManagerClass::Init -- initialization routine                         *
 *                                                                         *
 * This routine allocates memory, & initializes variables                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Init(void)
{
	int i;

	DebugString("**** IPXManagerClass::Init ***\n"); // RA2: RA2 flushes the transport buffers here
	if (PacketTransport) { // RA2: as above
		PacketTransport->Discard_In_Buffers(); // RA2: as above
		PacketTransport->Discard_Out_Buffers(); // RA2: as above
	}

	if (Session.Type != GAME_INTERNET) {

		//------------------------------------------------------------------------
		// Error if IPX not installed
		//------------------------------------------------------------------------
		if (!IPXStatus) {
			return(0);
		}

		//------------------------------------------------------------------------
		// Stop Listening
		//------------------------------------------------------------------------
		if (Listening) {
			IPXConnClass::Stop_Listening();
			Listening = 0;
		}
	} else {
		/*
		**	Pretend IPX is available for Internet games whether it is or not
		*/
		IPXStatus = 1;
	}

	//------------------------------------------------------------------------
	// Free protected-mode memory
	//------------------------------------------------------------------------
	if (GlobalChannel) {
		delete GlobalChannel;
		GlobalChannel = 0;
	}
	if (LobbyChannel) { // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
		delete LobbyChannel; // RA2: as above
		LobbyChannel = 0; // RA2: as above
	}
	if (MulticastChannel) { // RA2: as above
		delete MulticastChannel; // RA2: as above
		MulticastChannel = 0; // RA2: as above
	}
	for (i = 0; i < NumConnections; i++) {
		delete Connection[i];
		Connection[i] = 0;
	}
	NumConnections = 0;

	//------------------------------------------------------------------------
	// Allocate the Global Channel
	//------------------------------------------------------------------------
	GlobalChannel = new IPXGlobalConnClass (Glb_NumPackets, Glb_NumPackets * 2,
		Glb_MaxPacketLen, ProductID, 0x1236); // RA2: RA2's form, as in YR's body; TibSun's form does not match
	if (!GlobalChannel) {
		return(0);
	}
	GlobalChannel->Init();
	GlobalChannel->Set_Retry_Delta (RetryDelta);
	GlobalChannel->Set_Max_Retries (MaxRetries);
	GlobalChannel->Set_TimeOut (Timeout);

	//------------------------------------------------------------------------
	// Configure the IPX Connections
	//------------------------------------------------------------------------
	LobbyChannel = new IPXGlobalConnClass (Lobby_NumPackets, Lobby_NumPackets * 2, // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
		Lobby_MaxPacketLen, ProductID, 0xCDEF); // RA2: as above
	if (!LobbyChannel) { // RA2: as above
		return(0); // RA2: as above
	}
	LobbyChannel->Init(); // RA2: as above
	LobbyChannel->Set_Retry_Delta (RetryDelta); // RA2: as above
	LobbyChannel->Set_Max_Retries (MaxRetries); // RA2: as above
	LobbyChannel->Set_TimeOut (Timeout); // RA2: as above

	IPXConnClass::Configure(Socket, ConnectionNum, // RA2: as above
		FirstHeaderBuf, SendHeader, FirstDataBuf, SendBuf, PacketLen); // RA2: as above

	if (PacketTransport == NULL) {
		return(0);
	}

	//------------------------------------------------------------------------
	// Start Listening
	//------------------------------------------------------------------------
	if (Session.Type != GAME_INTERNET) {
		if (!IPXConnClass::Start_Listening()) return(0);
	}
	Listening = 1;

	return(1);

}	/* end of Init */


/***************************************************************************
 * IPXManagerClass::Is_IPX -- tells if IPX is installed or not             *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = IPX is installed; 0 = isn't                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Is_IPX(void)
{
	return(IPXStatus);

}	/* end of Is_IPX */


/***************************************************************************
 * IPXManagerClass::Set_Timing -- sets timing for all connections          *
 *                                                                         *
 * This will set the timing parameters for all existing connections, and   *
 * all connections created from now on.  This allows an application to     *
 * measure the Response_Time while running, and adjust timing accordingly. *
 *                                                                         *
 * INPUT:                                                                  *
 *      retrydelta   value to set for retry delta                          *
 *      maxretries   value to set for max # retries                        *
 *      timeout      value to set for connection timeout                   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/02/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Timing (unsigned int retrydelta,
	unsigned int maxretries, unsigned int timeout, bool set_external)
{
	int i;

	RetryDelta = retrydelta;
	MaxRetries = maxretries;
	Timeout = timeout;

	DebugString("IPX Manager: RetryDelta = %d\n", retrydelta);
	DebugString("MaxAhead is %d\n", Session.MaxAhead);

	if (set_external) {
		Set_External_Timing(RetryDelta, MaxRetries, Timeout);
	}

	for (i = 0; i < NumConnections; i++) {
		Connection[i]->Set_Retry_Delta (RetryDelta);
		Connection[i]->Set_Max_Retries (MaxRetries);
		Connection[i]->Set_TimeOut (Timeout);
	}

	if (MulticastChannel) { // RA2: the multicast channel is part of the RA2 class
		MulticastChannel->Set_Retry_Delta (RetryDelta); // RA2: as above
		MulticastChannel->Set_Max_Retries (MaxRetries); // RA2: as above
		MulticastChannel->Set_TimeOut (Timeout); // RA2: as above
	}
}


/// <summary>
/// Sets the timing for the global channel.
/// This routine adjusts the retry and timeout behavior of the global (broadcast) channel
/// alone, leaving the private connections as they were. The lobby uses this routine to
/// pace its own chatter independently of the game connections.
/// </summary>
/// <param name="retrydelta">Value to set for the retry delta.</param>
/// <param name="maxretries">Value to set for the maximum number of retries.</param>
/// <param name="timeout">Value to set for the channel timeout.</param>
void IPXManagerClass::Set_External_Timing (unsigned int retrydelta,
	unsigned int maxretries, unsigned int timeout)
{
	if (GlobalChannel) {
		GlobalChannel->Set_Retry_Delta (retrydelta);
		GlobalChannel->Set_Max_Retries (maxretries);
		GlobalChannel->Set_TimeOut (timeout);
	}
}

void IPXManagerClass::Save_Timing(void) // RA2: 0x00525480 keeps the global channel's timing
{
	if (GlobalChannel) { // RA2: as above
		SavedRetryDelta = GlobalChannel->Retry_Delta(); // RA2: as above
		SavedMaxRetries = GlobalChannel->Max_Retries(); // RA2: as above
		SavedTimeout = GlobalChannel->Time_Out(); // RA2: as above
	}
}


void IPXManagerClass::Restore_Timing(void) // RA2: 0x005254A0 puts the saved timing back
{
	if (GlobalChannel) { // RA2: as above
		GlobalChannel->Set_Retry_Delta(SavedRetryDelta); // RA2: as above
		GlobalChannel->Set_Max_Retries(SavedMaxRetries); // RA2: as above
		GlobalChannel->Set_TimeOut(SavedTimeout); // RA2: as above
	}
}


void IPXManagerClass::Set_Lobby_Timing(unsigned int retrydelta, unsigned int maxretries, unsigned int timeout) // RA2: 0x005254C0 timing of the lobby and multicast channels
{
	if (LobbyChannel) { // RA2: as above
		LobbyChannel->Set_Retry_Delta(retrydelta); // RA2: as above
		LobbyChannel->Set_Max_Retries(maxretries); // RA2: as above
		LobbyChannel->Set_TimeOut(timeout); // RA2: as above
	}
	if (MulticastChannel) { // RA2: as above
		MulticastChannel->Set_Retry_Delta(retrydelta); // RA2: as above
		MulticastChannel->Set_Max_Retries(maxretries); // RA2: as above
		MulticastChannel->Set_TimeOut(timeout); // RA2: as above
	}
}












/***************************************************************************
 * IPXManagerClass::Create_Connection -- creates a new connection          *
 *                                                                         *
 * INPUT:                                                                  *
 *      id            application-specific numerical ID for this connection*
 *      node         ptr to IPXNodeIDType (name & address)                 *
 *      address      IPX address for this connection                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Never create a connection with an 'id' of -1.                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Create_Connection(int id, wchar_t const * name,	// RA2: wide connection name
	IPXAddressClass *address)
{
	//------------------------------------------------------------------------
	// Error if IPX not installed
	//------------------------------------------------------------------------
	if (!IPXStatus) {
		return(0);
	}

	//------------------------------------------------------------------------
	// Error if no more room
	//------------------------------------------------------------------------
	if (NumConnections==CONNECT_MAX) {
		return(0);
	}

	//------------------------------------------------------------------------
	// Create new connection
	//------------------------------------------------------------------------
	Connection[NumConnections] = new IPXConnClass(Pvt_NumPackets,
		Pvt_NumPackets, Pvt_MaxPacketLen, ProductID, address, id, name);
	if (!Connection[NumConnections]) {
		return(0);
	}

	Connection[NumConnections]->Init ();
	Connection[NumConnections]->Set_Retry_Delta (RetryDelta);
	Connection[NumConnections]->Set_Max_Retries (MaxRetries);
	Connection[NumConnections]->Set_TimeOut (Timeout);

	NumConnections++;

	return(1);

}



int IPXManagerClass::Create_Multicast_Connection(void) // RA2: 0x005255B0 the multicast connection, addressed like the first session player
{
	if (!IPXStatus) { // RA2: as above
		return(0); // RA2: as above
	}

	MulticastChannel = new IPXConnClass(Pvt_NumPackets, // RA2: as above
		Pvt_NumPackets, Pvt_MaxPacketLen, ProductID, // RA2: as above
		&Session.Players[0]->Address, -1, Narrow_To_Wide("Multicast")); // RA2: as above
	if (!MulticastChannel) { // RA2: as above
		return(0); // RA2: as above
	}

	MulticastChannel->Init (); // RA2: as above
	MulticastChannel->Set_Retry_Delta (RetryDelta); // RA2: as above
	MulticastChannel->Set_Max_Retries (MaxRetries); // RA2: as above
	MulticastChannel->Set_TimeOut (Timeout); // RA2: as above
	return(1); // RA2: as above
}






/***************************************************************************
 * IPXManagerClass::Delete_Connection -- deletes a connection              *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      ID of connection to delete                                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Delete_Connection(int id)
{
	int i,j;

	//------------------------------------------------------------------------
	// Error if IPX not installed
	//------------------------------------------------------------------------
	if (!IPXStatus) {
		return(0);
	}

	//------------------------------------------------------------------------
	// Error if no connections to delete
	//------------------------------------------------------------------------
	if (NumConnections==0) {
		return(0);
	}

	//------------------------------------------------------------------------
	// Loop through all connections
	//------------------------------------------------------------------------
	for (i = 0; i < NumConnections; i++) {
		//.....................................................................
		// If a match, delete it
		//.....................................................................
		if (Connection[i]->ID==id) {
			delete Connection[i];

			//..................................................................
			// Move array elements back one index
			//..................................................................
			for (j = i; j < NumConnections - 1; j++) {
				Connection[j] = Connection[j+1];
			}

			//..................................................................
			// Adjust counters
			//..................................................................
			NumConnections--;
			if (CurConnection >= NumConnections)
				CurConnection = 0;
			return(1);
		}
	}

	//------------------------------------------------------------------------
	//	No match; error
	//------------------------------------------------------------------------
	return(0);

}	/* end of Delete_Connection */


/***************************************************************************
 * IPXManagerClass::Num_Connections -- gets the # of connections           *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # of connections                                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Num_Connections(void)
{
	return NumConnections; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}


/***************************************************************************
 * IPXManagerClass::Connection_ID -- gets the given connection's ID        *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of connection to retrieve                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ID for that connection, CONNECTION_NONE if invalid index           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Connection_ID(int index)
{
	if (index >= 0 && index < NumConnections) {
		return Connection[index]->ID; // RA2: RA2's form, as in YR's body; TibSun's form does not match
	}
	return CONNECTION_NONE; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}


/***************************************************************************
 * IPXManagerClass::Connection_Name -- retrieves name for given connection *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      ID of connection to get name of                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ptr to connection's name, NULL if not found                        *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
wchar_t *IPXManagerClass::Connection_Name(int id) // RA2: RA2's form, as in YR's body; TibSun's form does not match
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID == id) {
			return Connection[i]->Name; // RA2: RA2's form, as in YR's body; TibSun's form does not match
		}
	}
	return NULL; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}


/***************************************************************************
 * IPXManagerClass::Connection_Address -- retrieves connection's address   *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      ID of connection to get address for                        *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      pointer to IXPAddressClass, NULL if not found                      *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/19/1995 BR : Created.                                              *
 *=========================================================================*/
IPXAddressClass * IPXManagerClass::Connection_Address(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID == id) {
			return &Connection[i]->Address; // RA2: RA2's form, as in YR's body; TibSun's form does not match
		}
	}
	return NULL; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}


/***************************************************************************
 * IPXManagerClass::Connection_Index -- gets given connection's index      *
 *                                                                         *
 * INPUT:                                                                  *
 *      ID to retrieve index for                                           *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      index for this connection, CONNECTION_NONE if not found            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Connection_Index(int id)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		if (Connection[i]->ID == id) {
			return i; // RA2: RA2's form, as in YR's body; TibSun's form does not match
		}
	}
	return CONNECTION_NONE; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}

int IPXManagerClass::Index_Of_Connection(IPXConnClass * connection) // RA2: 0x005257B0 vtable slot 19
{
	for (int i = 0; i < NumConnections; i++) { // RA2: as above
		if (Connection[i] == connection) { // RA2: as above
			return i; // RA2: as above
		}
	}
	if (connection == MulticastChannel) { // RA2: as above
		return -4; // RA2: as above
	}
	if (connection == LobbyChannel) { // RA2: as above
		return -3; // RA2: as above
	}
	if (connection == GlobalChannel) { // RA2: as above
		return -2; // RA2: as above
	}
	return -1; // RA2: as above
}






/***************************************************************************
 * IPXManagerClass::Set_Connection_Parms -- sets connection's name & id    *
 *                                                                         *
 * INPUT:                                                                  *
 *      index      connection index                                        *
 *      id         new connection ID                                       *
 *      name      new connection name                                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Connection_Parms(int index, int id, wchar_t const * name)	// RA2: wide name
{
	if (index >= NumConnections)
		return;

	Connection[index]->ID = id;
	wcscpy(Connection[index]->Name,name);	// RA2: wcscpy

}
	/* end of Set_Connection_Parms */


/***************************************************************************
 * IPXManagerClass::Send_Global_Message -- sends a Global Message          *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of buf                                          *
 *      ack_req      1 = ACK required; 0 = no ACK required                 *
 *      address      address to send to; NULL = broadcast                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Send_Global_Message(void *buf, int buflen,
	int ack_req, IPXAddressClass *address, unsigned short port, unsigned long packet_id)	// RA2: port and packet ID
{
	int rc;

	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return(0);

	if (ack_req != 0 && (address == NULL || address->Is_Broadcast())) {
		ack_req = 0;
	}

	rc = GlobalChannel->Send_Packet (buf, buflen, address, ack_req, port, packet_id);	// RA2: port and packet ID
	if (!rc) {
		SendOverflows++;
		DebugString("Send overflow %d\n", SendOverflows);
	}

	return(rc);

}



int IPXManagerClass::Send_Global_Message_To_Player(void *buf, int buflen, int ack_req, int id) // RA2: 0x005258B0
{
	int count = Session.Players.Count(); // RA2: as above
	if (count && Session.Players[0]->Player.ID == id) { // RA2: as above
		return(0); // RA2: as above
	}
	for (int i = 1; i < count; i++) { // RA2: as above
		if (Session.Players[i]->Player.ID == id) { // RA2: as above
			IPXAddressClass address = Session.Players[i]->Address; // RA2: as above
			return Send_Global_Message(buf, buflen, ack_req, &address, Session.PlayerPorts[i], 0); // RA2: as above
		}
	}
	return(0); // RA2: as above
}


int IPXManagerClass::Send_Global_Message_To_Name(void *buf, int buflen, int ack_req, wchar_t const *name) // RA2: 0x005259A0
{
	if (Session.Players.Count() && !wcsicmp(Session.Players[0]->Name, name)) { // RA2: as above
		return(0); // RA2: as above
	}
	for (int i = 1; i < Session.Players.Count(); i++) { // RA2: as above
		if (!wcsicmp(Session.Players[i]->Name, name)) { // RA2: as above
			IPXAddressClass address = Session.Players[i]->Address; // RA2: as above
			return Send_Global_Message(buf, buflen, ack_req, &address, Session.PlayerPorts[i], 0); // RA2: as above
		}
	}
	return(0); // RA2: as above
}









/***************************************************************************
 * IPXManagerClass::Get_Global_Message -- polls the Global Message queue   *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf            buffer to store received packet                     *
 *      buflen         length of data placed in 'buf'                      *
 *      address         IPX address of sender                              *
 *      product_id      product ID of sender                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Get_Global_Message(void *buf, int *buflen,
	IPXAddressClass *address, unsigned short *product_id)
{
	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return 0; // RA2: RA2's form, as in YR's body; TibSun's form does not match

	return GlobalChannel->Get_Packet(buf, buflen, address, product_id); // RA2: RA2's form, as in YR's body; TibSun's form does not match
}

int IPXManagerClass::Peek_Global_Message(int index, void *buf, int *buflen, IPXAddressClass *address, // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
	unsigned short *product_id, unsigned long *packet_id) // RA2: 0x00525AF0 looks at a queued global packet without removing it
{
	if (!IPXStatus || !Listening) { // RA2: as above
		return(0); // RA2: as above
	}
	return GlobalChannel->Peek_Packet(index, (int)buf, (int)buflen, (int)address, (int)product_id, (int)packet_id); // RA2: as above
}


void IPXManagerClass::Mark_Global_Message_Read(int index) // RA2: 0x00525B30
{
	if (IPXStatus && Listening) { // RA2: as above
		GlobalChannel->Mark_Packet_Read(index); // RA2: as above
	}
}


int IPXManagerClass::Send_Lobby_Message(void *buf, int buflen, bool ack_req) // RA2: 0x00525B50 to the lobby channel's own address
{
	if (!IPXStatus || !Listening || !LobbyChannel) { // RA2: as above
		return(0); // RA2: as above
	}
	int rc = LobbyChannel->Send_Packet(buf, buflen, &LobbyChannel->Address, ack_req, 0, 0); // RA2: as above
	if (!rc) { // RA2: as above
		SendOverflows++; // RA2: as above
		DebugString("Send overflow %d\n", SendOverflows); // RA2: as above
	}
	return(rc); // RA2: as above
}


int IPXManagerClass::Get_Lobby_Message(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id) // RA2: 0x00525BC0
{
	if (!IPXStatus || !Listening) { // RA2: as above
		return(0); // RA2: as above
	}
	return LobbyChannel->Get_Packet(buf, buflen, address, product_id); // RA2: as above
}















/***************************************************************************
 * IPXManagerClass::Send_Private_Message -- Sends a Private Message        *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of 'buf'                                        *
 *      conn_id      connection ID to send to (CONNECTION_NONE = all)      *
 *      ack_req      1 = ACK required; 0 = no ACK required                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Send_Private_Message(void *buf, int buflen, int ack_req,
	int conn_id)
{
	int i;						// loop counter
	int connect_idx;			// index of channel to send to, if specified

	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections == 0)) {
		return 0; // RA2: RA2's form, as in YR's body; TibSun's form does not match
	}

	//------------------------------------------------------------------------
	// Send the message to all connections
	//------------------------------------------------------------------------
	if (conn_id == CONNECTION_NONE) {
		if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) { // RA2: the multicast channel is part of the RA2 class
			if (MulticastChannel->Queue->Num_Send() >= MulticastChannel->Queue->Max_Send()) { // RA2: as above
				{ SendOverflows++; return 0; } // RA2: as above
			}
			return MulticastChannel->Send_Packet(buf, buflen, ack_req, 0xff); // RA2: as above
		}

		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Send() == Connection[i]->Queue->Max_Send()) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
				{ SendOverflows++; return 0; } // RA2: as above
			}
		}

		//.....................................................................
		// Send packet to all connections
		//.....................................................................
		for (i = 0; i < NumConnections; i++) {
			Connection[i]->Send_Packet(buf, buflen, ack_req, 0); // RA2: RA2's form, as in YR's body; TibSun's form does not match
		}
		return 1; // RA2: RA2's form, as in YR's body; TibSun's form does not match
	}

	//------------------------------------------------------------------------
	// Send the message to the specified connection
	//------------------------------------------------------------------------
	connect_idx = Connection_Index(conn_id);
	if (connect_idx == CONNECTION_NONE) {
		{ SendOverflows++; return 0; } // RA2: RA2's form, as in YR's body; TibSun's form does not match
	}
	if (Connection[connect_idx]->Queue->Num_Send() == Connection[connect_idx]->Queue->Max_Send()) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
		{ SendOverflows++; return 0; } // RA2: as above
	}
	Connection[connect_idx]->Send_Packet(buf, buflen, ack_req, 0); // RA2: RA2's form, as in YR's body; TibSun's form does not match
	return 1; // RA2: as above
}


/***************************************************************************
 * IPXManagerClass::Get_Private_Message -- Polls the Private Message queue *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to store incoming packet                        *
 *      buflen      length of data placed in 'buf'                         *
 *      conn_id      filled in with connection ID of sender                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Get_Private_Message(void *buf, int *buflen, int *conn_id)
{
	int i;
	int rc;
	int c_id;

	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections == 0)) {
		return 0; // RA2: RA2's form, as in YR's body; TibSun's form does not match
	}

	//------------------------------------------------------------------------
	// Safety check: ensure CurConnection is in range.
	//------------------------------------------------------------------------
	if (CurConnection >= NumConnections) {
		CurConnection = 0;
	}

	//------------------------------------------------------------------------
	// Scan all connections for a received packet, starting with 'CurConnection'
	//------------------------------------------------------------------------
	for (i = 0; i < NumConnections; i++) {
		rc = Connection[CurConnection]->Get_Packet(buf, buflen);
		c_id = Connection[CurConnection]->ID;

		//.....................................................................
		// Increment CurConnection to the next connection index
		//.....................................................................
		CurConnection++;
		if (CurConnection >= NumConnections) {
			CurConnection = 0;
		}

		//.....................................................................
		// If we got a packet, return the connection ID
		//.....................................................................
		if (rc) {
			(*conn_id) = c_id;
			return 1; // RA2: RA2's form, as in YR's body; TibSun's form does not match
		}
	}

	return 0; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}

bool IPXManagerClass::Forward_Router_Packet(CommHeaderType *packet, void *buf, int buflen, // RA2: RA2's form, as in YR's body; TibSun's form does not match
	IPXAddressClass *from, unsigned short port) // RA2: 0x00525DC0 an internet game's packet that came through (or is for) the packet router
{
	static unsigned long loopback[2] = {0, htonl(0x7F000001)}; // RA2: as above

	if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter && OwnerTalk.RouterProcess) { // RA2: as above
		if (packet->ForwardPort == LoopbackPort) { // RA2: as above
			packet->ForwardPort = port; // RA2: as above
			memcpy(&packet->ForwardAddress, (char *)from + 4, 4); // RA2: as above
			DebugString("Forwarding packet from %s ; %d to forwarder\n", from->As_String(), port); // RA2: as above
			PacketTransport->WriteTo(buf, buflen, loopback, 12, false, true, LoopbackPort); // RA2: as above
			return true; // RA2: as above
		}
		if (packet->ForwardPort) { // RA2: as above
			NetNodeType node; // RA2: as above
			Session.Players[0]->Address.Get_Node(node); // RA2: as above
			if (memcmp(node, &packet->ForwardAddress, 4) == 0 && Session.PlayerPorts[0] == packet->ForwardPort) { // RA2: as above
				DebugString("Packet is supposed to be forwarded to us - just adding it to our queue\n"); // RA2: as above
				return false; // RA2: as above
			}
			if (packet->ForwardAddress) { // RA2: as above
				unsigned short oldport = packet->ForwardPort; // RA2: as above
				IPXAddressClass address; // RA2: as above
				NetNodeType dest; // RA2: as above
				memcpy(dest, &packet->ForwardAddress, 4); // RA2: as above
				address.Set_Node(dest); // RA2: as above
				address.Set_Port(oldport); // RA2: as above
				packet->ForwardPort = 0; // RA2: as above
				memset(&packet->ForwardAddress, 0, 4); // RA2: as above
				DebugString("Forwarding packet from router to %s\n", address.As_String()); // RA2: as above
				PacketTransport->WriteTo(buf, buflen, &address, 12, false, true, oldport); // RA2: as above
				return true; // RA2: as above
			}
		}
	}
	return false; // RA2: as above
}


/***************************************************************************
 * IPXManagerClass::Service -- main polling routine for IPX Connections    *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (00525f60 has no row); 101 bytes differ against main's function-map extent; main: best 97.99, tried 14 times (register, frame, order)
int IPXManagerClass::Service(void)
{
	int rc = 1;
	int i;
	CommHeaderType *packet;
	int packetlen;
	IPXAddressClass address;
	unsigned char temp_receive_buffer[1024];
	char temp_address[128];
	unsigned short port;
	// RA2: Routed games also receive lobby and multicast traffic on this transport.
	if (PacketTransport) {
		do {
			int temp_receive_buffer_len = sizeof(temp_receive_buffer);
			int temp_address_len = sizeof(temp_address);
			port = 0;
			packetlen = PacketTransport->Read(temp_receive_buffer, temp_receive_buffer_len, temp_address, temp_address_len, port);
			if (packetlen) {
				CurDataBuf = (char *)temp_receive_buffer;
				address = *(IPXAddressClass *)temp_address;
				packet = (CommHeaderType *)CurDataBuf;
				if (packet->MagicNumber == GlobalChannel->Magic_Num()) {
					DebugString("Adding packet from %s ; %d to global queue\n", address.As_String(), port);
					if (!Forward_Router_Packet(packet, (char *)temp_receive_buffer, packetlen, (IPXAddressClass *)temp_address, port)) {
						if (!GlobalChannel->Receive_Packet(packet, packetlen, &address, port)) {
							++ReceiveOverflows;
							DebugString("GlobalChannel recive buffer overflow %d\n", ReceiveOverflows);
							break;
						}
						if (Session.Type == GAME_INTERNET && Session.UseRouter) {
							for (i = 0; i < Session.Players.Count(); ++i) {
								if (Session.Players[i]->Address.Is_Same_Node(address) && Session.PlayerPorts[i] == port) goto next_packet;
							}
							if (Session.Players.Count() != 0) {
								DebugString("NAT Alert - port number fiddling detected!\n");
								if ((unsigned)packetlen >= 16 && packet->Code != ConnectionClass::PACKET_ACK && (unsigned)packetlen >= 63) {
									int * message = (int *)(CurDataBuf + 16);
									if (*message == NET_IM_STILL_HERE || *message == NET_PORT_PROBE || *message == NET_PING) {
										char name[22];
										memset(name, 0, sizeof(name));
										strncpy(name, (char *)(message + 1), 20);
										for (i = 0; i < Session.Players.Count(); ++i) {
											wchar_t * wide_name = Session.Players[i]->Name;
											if (!_stricmp(Wide_To_Narrow(wide_name), name)) {
												if (!Session.Players[i]->Address.Is_Same_Node(address) || Session.Players[i]->Address.Get_Port() != port) {
													DebugString("Player %s is having his port translated to %d\n", name, port);
													unsigned short old_port = Session.PlayerPorts[i];
													Session.PlayerPorts[i] = port;
													OwnerTalk.Set_Port(Session.Players[i]->Player.ID, port);
													GlobalChannel->Discard_Packets_To(&address, old_port);
													GlobalChannel->Discard_Packets_To(&address, Session.Players[i]->Address.Get_Port());
													Session.Players[i]->Address = address;
													Session.Players[i]->Address.Set_Port(port);
													OwnerTalk.Set_Address(Session.Players[i]->Player.ID, &Session.Players[i]->Address);
													Session.Slots[Session.Player_To_Slot(i)]->Address = address;
													Session.Slots[Session.Player_To_Slot(i)]->Address.Set_Port(port);
													for (int connection = 0; connection < NumConnections; ++connection) {
														wchar_t * connection_name = Connection[connection]->Name;
														if (!_stricmp(Wide_To_Narrow(connection_name), name)) {
															Connection[connection]->Address = address;
															Connection[connection]->Address.Set_Port(port);
															DebugString("Set address for connection %d to %s\n", connection, Connection[connection]->Address.As_String());
															break;
														}
													}
													if (!Session.NetOpen && NumConnections != 0 && Houses.Count() > Session.Players[i]->Player.ID && Session.Am_I_Master(Houses[Session.Players[i]->Player.ID])) {
														DebugString("Frame %d, Router apparently changed address from %s to %s\n", Frame, MulticastChannel->Address.As_String(), address.As_String());
														MulticastChannel->Address = address;
														MulticastChannel->Address.Set_Port(port);
														Session.HostAddress = address;
														Session.HostAddress.Set_Port(port);
														Session.PacketRouterAddress = address;
														Session.PacketRouterAddress.Set_Port(port);
														Session.PacketRouterPort = port;
													}
												}
												break;
											}
										}
									}
								}
							}
						}
					}
				} else if (packet->MagicNumber == LobbyChannel->Magic_Num()) {
					if (Session.ViaPacketRouter && !LobbyChannel->Receive_Packet(packet, packetlen, &address, 0)) {
						++ReceiveOverflows;
						DebugString("RouterChannel recive buffer overflow %d\n", ReceiveOverflows);
						break;
					}
				} else if (packet->MagicNumber == ProductID && !Forward_Router_Packet(packet, (char *)temp_receive_buffer, packetlen, (IPXAddressClass *)temp_address, port)) {
					if (Session.ViaPacketRouter && packet->ForwardMask == 0xff) {
						if (packet->Code == ConnectionClass::PACKET_ACK) MulticastChannel->Receive_Packet(packet, packetlen);
						else if (NumConnections > 0) {
							switch (packet->Code) {
								case ConnectionClass::PACKET_DATA_ACK: DebugString("Adding DATA_ACK packet %d from %s ; %d to multicast queue\n", packet->PacketID, address.As_String(), port); break;
								case ConnectionClass::PACKET_DATA_NOACK: DebugString("Adding DATA_NOACK packet %d from %s ; %d to multicast queue\n", packet->PacketID, address.As_String(), port); break;
								case ConnectionClass::PACKET_ACK: DebugString("Adding ACK packet %d from %s ; %d to multicast queue\n", packet->PacketID, address.As_String(), port); break;
								default: DebugString("Adding ??? packet %d from %s ; %d to multicast queue\n", packet->PacketID, address.As_String(), port); break;
							}
							if (!Connection[0]->Receive_Packet(packet, packetlen)) {
								packetlen = 0;
								++ReceiveOverflows;
								DebugString("Receive buffer overflow %d\n", ReceiveOverflows);
							}
						}

					} else {
						bool found = false;
						for (i = 0; i < NumConnections; ++i) {
							if ((Session.ViaPacketRouter && OwnerTalk.Mask_From_ID(Connection[i]->ID) == packet->ForwardMask) || (!Session.ViaPacketRouter && Connection[i]->Address == address)) {
								found = true;
								break;
							}
						}
						if (found) {
							if (!Connection[i]->Receive_Packet(packet, packetlen)) {
								packetlen = 0;
								++ReceiveOverflows;
								DebugString("Reecive buffer overflow %d\n", ReceiveOverflows);
							}
						} else {
							if (Session.Type == GAME_INTERNET && !ScenarioInit && Frame > 8 && !Session.ViaPacketRouter && (unsigned)packetlen > 15 && packet->Code == ConnectionClass::PACKET_DATA_NOACK) {
								EventClass * event = (EventClass *)((char *)packet + sizeof(CommHeaderType));
								if (event->Type == EventClass::FRAMESYNC) {
									int id = event->ID;
									for (int player = 1; player < Session.Players.Count(); ++player) {
										if (Session.Players[player]->Player.ID == id) {
											Session.Players[player]->Address = address;
											if (Connection_Index(id) != CONNECTION_NONE) Connection[Connection_Index(id)]->Address = address;
											break;
										}
									}
								}
							}
							goto next_packet;
						}

					}
					if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter && address != MulticastChannel->Address) {
						for (int player = 1; player < Session.Players.Count(); ++player) {
							if (Session.Players[player]->Address.Is_Same_Node(MulticastChannel->Address) && Session.Am_I_Master(Houses[Session.Players[player]->Player.ID])) {
								DebugString("Frame %d, Router apparently changed address from %s to %s\n", Frame, MulticastChannel->Address.As_String(), address.As_String());
								Session.Players[player]->Address = address;
								MulticastChannel->Address = address;
								Session.HostAddress = address;
								Session.PacketRouterAddress = address;
								OwnerTalk.Set_Address(Session.Players[player]->Player.ID, &address);
							}
						}
					}
				}
			}
		next_packet:
			;
		} while (packetlen);
	}
	if (GlobalChannel && PacketTransport && !GlobalChannel->Service()) {
		GlobalChannel->Discard_Undeliverable_Packets();
		rc = 0;
	}
	if (Session.ViaPacketRouter) {
		if (LobbyChannel) {
			if (!LobbyChannel->Service()) {
				LobbyChannel->Discard_Undeliverable_Packets();
				rc = 0;
			}
			if (!Session.AboutToExitActive) OwnerTalk.Handle_Router_Messages();
		}
		if (Session.ViaPacketRouter && MulticastChannel && !MulticastChannel->Service()) {
			DebugString("Multicast channel has gone bad\n");
			rc = 0;
		}
	}
	if (PacketTransport && NumConnections > 0) {
		i = 0;
		do {
			if (!Connection[i]->Service()) {
				rc = 0;
				BadConnection = Connection[i]->ID;
				DebugString("Error - Connection %d has gone bad\n", BadConnection);
			}
			++i;
		} while (i < NumConnections);
	}
	if (rc) BadConnection = CONNECTION_NONE;
	return rc;
}
#endif
void IPXManagerClass::Set_Connection_Address(int id, IPXAddressClass *address) // RA2: 0x005269F0
{
	int index = Connection_Index(id); // RA2: as above
	if (index != CONNECTION_NONE && index < NumConnections) { // RA2: as above
		Connection[index]->Address = *address; // RA2: as above
	}
}









/***************************************************************************
 * IPXManagerClass::Get_Bad_Connection -- returns bad connection ID        *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      ID of bad connection; CONNECTION_NONE if none.                     *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
int IPXManagerClass::Get_Bad_Connection(void)
{
	return BadConnection; // RA2: RA2's form, as in YR's body; TibSun's form does not match
}


/***************************************************************************
 * IPXManagerClass::Global_Num_Send   -- reports # entries in send queue   *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # entries in the Global Send Queue                                 *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Global_Num_Send(void)
{
	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return 0; // RA2: RA2's form, as in YR's body; TibSun's form does not match

	return GlobalChannel->Queue->Num_Send(); // RA2: as above
}


/***************************************************************************
 * IPXManagerClass::Global_Num_Receive -- reports # entries in recv queue  *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # entries in the Global Receive Queue                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Global_Num_Receive(void)
{
	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening) return 0; // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class

	return GlobalChannel->Queue->Num_Receive(); // RA2: as above
}

int IPXManagerClass::Lobby_Num_Send(void) // RA2: 0x00526A80 vtable slot 20
{
	if (!IPXStatus || !Listening || !LobbyChannel) { // RA2: as above
		return(0); // RA2: as above
	}
	return LobbyChannel->Queue->Num_Send(); // RA2: as above
}


int IPXManagerClass::Lobby_Num_Receive(void) // RA2: 0x00526AA0 vtable slot 21
{
	if (!IPXStatus || !Listening || !LobbyChannel) { // RA2: as above
		return(0); // RA2: as above
	}
	return LobbyChannel->Queue->Num_Receive(); // RA2: as above
}









/***************************************************************************
 * IPXManagerClass::Private_Num_Send -- reports # entries in send queue    *
 *                                                                         *
 * INPUT:                                                                  *
 *      # entries in the Private Send Queue                                *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Private_Num_Send(int id)
{
	int i;
	int maxnum;

	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections==0)) {
		return(0);
	}

	//------------------------------------------------------------------------
	//	If connection ID specified, return that connection's # of packets
	//------------------------------------------------------------------------
	if (id != CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Send());
		}
		else {
			return(0);
		}

	}

	//------------------------------------------------------------------------
	//	Otherwise, return the max # of all connections
	//------------------------------------------------------------------------
	else {
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Send() > maxnum) {
				maxnum = Connection[i]->Queue->Num_Send();
			}
		}
		return(maxnum);
	}

}	/* end of Private_Num_Send */


/***************************************************************************
 * IPXManagerClass::Private_Num_Receive -- reports # entries in recv queue *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      ID of connection to query                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      # entries in the Private Receive Queue                             *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
int IPXManagerClass::Private_Num_Receive(int id)
{
	int i;
	int maxnum;

	//------------------------------------------------------------------------
	// Error if IPX not installed or not Listening
	//------------------------------------------------------------------------
	if (!IPXStatus || !Listening || (NumConnections==0)) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
		return(0);
	}

	//------------------------------------------------------------------------
	//	If connection ID specified, return that connection's # of packets
	//------------------------------------------------------------------------
	if (id != CONNECTION_NONE) {
		i = Connection_Index(id);
		if (i != CONNECTION_NONE) {
			return(Connection[i]->Queue->Num_Receive());
		}
		else {
			return(0);
		}

	}

	//------------------------------------------------------------------------
	//	Otherwise, return the max # of all connections
	//------------------------------------------------------------------------
	else {
		maxnum = 0;
		for (i = 0; i < NumConnections; i++) {
			if (Connection[i]->Queue->Num_Receive() > maxnum) {
				maxnum = Connection[i]->Queue->Num_Receive();
			}
		}
		return(maxnum);
	}

}	/* end of Private_Num_Receive */


/***************************************************************************
 * IPXManagerClass::Set_Socket -- sets socket ID for all connections       *
 *                                                                         *
 * INPUT:                                                                  *
 *      socket   new socket ID to use                                      *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Do not call this function after communications have started; you   *
 *      must call it before calling Init().                                *
 *      The socket number is byte-swapped, since IPX requires socket ID's  *
 *      to be stored high/low.                                             *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Set_Socket(unsigned short socket)
{
	Socket = (unsigned short)( (((unsigned int)socket & 0x00ff) << 8) |
		(((unsigned int)socket & 0xff00) >> 8));

}	/* end of Set_Socket */


/***************************************************************************
 * IPXManagerClass::Response_Time -- Returns largest Avg Response Time     *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      largest avg response time                                          *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned int IPXManagerClass::Response_Time(void)
{
	unsigned int resp;
	unsigned int maxresp = 0;
	int i;

	for (i = 0; i < NumConnections; i++) {
		resp = Connection[i]->Queue->Avg_Response_Time();
		if (resp > maxresp) {
			maxresp = resp;
		}
	}

	if (MulticastChannel) { // RA2: the multicast channel is part of the RA2 class
		resp = MulticastChannel->Queue->Avg_Response_Time(); // RA2: as above
		if (resp > maxresp) { // RA2: as above
			maxresp = resp; // RA2: as above
		}
	}

	return(maxresp);

}	/* end of Response_Time */


/// <summary>
/// Fetches the average response time of a single connection.
/// This routine is used by the network queue logic to pace itself against the slowest
/// player, and by the debug display.
/// </summary>
/// <param name="index">Index of the connection to examine.</param>
/// <returns>Returns with the average round trip time, expressed in game timer ticks.</returns>
unsigned int IPXManagerClass::Avg_Response_Time(int index)
{
	return(Connection[index]->Queue->Avg_Response_Time());
}


/***************************************************************************
 * IPXManagerClass::Global_Response_Time -- Returns Avg Response Time      *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      avg global channel response time                                   *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
unsigned int IPXManagerClass::Global_Response_Time(void)
{
	if (GlobalChannel) {
		return(GlobalChannel->Queue->Avg_Response_Time());
	}
	else {
		return(0);
	}
}

unsigned int IPXManagerClass::Lobby_Response_Time(void) // RA2: 0x00526C50
{
	if (LobbyChannel) { // RA2: as above
		return LobbyChannel->Queue->Avg_Response_Time(); // RA2: as above
	}
	return(0); // RA2: as above
}


unsigned int IPXManagerClass::Multicast_Response_Time(void) // RA2: 0x00526C70
{
	if (MulticastChannel) { // RA2: as above
		return MulticastChannel->Queue->Avg_Response_Time(); // RA2: as above
	}
	return(0); // RA2: as above
}









/// <summary>
/// Records the connection statistics for the remote players.
/// This routine folds each connection's round trip, resend and packet loss figures into
/// the session statistics, so that the end of game screen can report on how well the
/// network behaved. The local player's own connection is passed over.
/// </summary>
namespace OwnerDraw { // RA2: RA2's form, as in YR's body; TibSun's form does not match
void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); // RA2: as above
}

void IPXManagerClass::Store_Stats(void)
{
	for (int i = 0; i < NumConnections; i++) {
		HouseClass * house = Houses[Connection[i]->ID];

		if (house != NULL && house != PlayerPtr) {
			MPStatsType & stats = Session.ConnectionStats[i];
			if (stats.Name[0] == '\0') {
				OwnerDraw::WideToCharString(stats.Name, Connection[i]->Name, -1); // RA2: wide name
				stats.Address = Connection[i]->Address;
			}

			int maxtime = Connection[i]->Queue->Max_Response_Time();
			stats.MaxRoundTrip = MAX(Session.ConnectionStats[i].MaxRoundTrip, 1000 * maxtime / TIMER_SECOND);

			unsigned int avgtime = Connection[i]->Queue->Avg_Response_Time();
			stats.MaxAvgRoundTrip = MAX(int(1000 * avgtime / TIMER_SECOND), Session.ConnectionStats[i].MaxAvgRoundTrip);

			stats.Resends = Connection[i]->Num_Resends();
			stats.Lost = Connection[i]->Num_Lost();
			stats.PercentLost = Connection[i]->Percent_Lost();
		}
	}
}




/// <summary>
/// Draws the network diagnostics display.
/// This routine prints the retry and timeout settings currently in force, followed by a
/// column of round trip, resend and packet loss figures for every remote player in the
/// game. Use this routine when the multiplayer debug display has been switched on.
/// </summary>
void IPXManagerClass::Multiplayer_Debug_Print(void)
{
	wchar_t buffer[256];	// RA2: wide text
	wchar_t name[200];	// RA2: as above
	Surface * surface = CompositeSurface;	// RA2: drawn on the composite surface

	swprintf(buffer, L"Rtry delta: %d", 1000 * RetryDelta / TIMER_SECOND);	// RA2: as above
	Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 402), Fetch_Scheme_By_Name("Grey", 1), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

	swprintf(buffer, L"Rtry timeout: %d", 1000 * Timeout / TIMER_SECOND);	// RA2: as above
	Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 412), Fetch_Scheme_By_Name("Grey", 1), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

	swprintf(buffer, L"Lat Fudge : %d", Session.LatencyFudge);	// RA2: as above
	Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 422), Fetch_Scheme_By_Name("Grey", 1), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

	if (SentFrameSyncTimer / TIMER_SECOND) {
		swprintf(buffer, L"FSPS : %d", SentFrameSyncCount / (SentFrameSyncTimer / TIMER_SECOND));	// RA2: as above
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 432), Fetch_Scheme_By_Name("Grey", 1), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above
		if ((Frame & 0x7F) == 0x7F) {
			SentFrameSyncTimer = 0;
			SentFrameSyncCount = 0;
		}
	}

	if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {	// RA2: as above
		swprintf(buffer, L"MC resp: %d", 1000 * Ipx.Multicast_Response_Time() / TIMER_SECOND);	// RA2: as above
		Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(0, 442), Fetch_Scheme_By_Name("Grey", 1), 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above
	}

	for (int i = 0; i < NumConnections; i++) {
		HouseClass * house = Houses[Connection[i]->ID];
		if (house != NULL && house != PlayerPtr) {
			int scheme = house->Scheme;
			int x = (i + 1) * 100;	// RA2: as above

			swprintf(name, L"%s", Connection[i]->Name);	// RA2: as above
			Fancy_Text_Print(name, *surface, surface->Get_Rect(), Point2D(x, 302), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int avg = Connection[i]->Queue->Avg_Response_Time();
			swprintf(buffer, L"Average  : %d", 1000 * avg / TIMER_SECOND);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 311), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int max = Connection[i]->Queue->Max_Response_Time();
			swprintf(buffer, L"Max      : %d", 1000 * max / TIMER_SECOND);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 319), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int resends = Connection[i]->Num_Resends();
			swprintf(buffer, L"Resends  : %d", resends);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 327), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int numlost = MAX(0, Connection[i]->Num_Lost());
			swprintf(buffer, L"Num lost : %d", numlost);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 335), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int pcnt_lost = Connection[i]->Percent_Lost();
			swprintf(buffer, L"Pcnt lost: %d", pcnt_lost);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 343), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			int process_time = 0;
			for (int j = 0; j < Session.Players.Count(); ++j) {
				if (Connection[i]->ID == Session.Players[j]->Player.ID) {
					process_time = Session.Players[j]->Player.ProcessTime;
					break;
				}
			}
			swprintf(buffer, L"Process : %d", process_time);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 351), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			swprintf(buffer, L"Frame   : %d", -Session.PlayerLatency[i]);	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 359), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			swprintf(buffer, L"Queue s/r: %d/%d", Connection[i]->Queue->Num_Send(), Connection[i]->Queue->Num_Receive());	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 367), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			swprintf(buffer, L"Missed o/m: %d/%d", Connection[i]->Missed_Overall(), Connection[i]->Missed_Magic());	// RA2: as above
			Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 375), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above

			if (Session.Type == GAME_INTERNET && Session.ViaPacketRouter) {	// RA2: as above
				swprintf(buffer, L"Rout resp: %d", 1000 * RouterResponseTimes[i] / TIMER_SECOND);	// RA2: the router round trip per connection
				Fancy_Text_Print(buffer, *surface, surface->Get_Rect(), Point2D(x, 383), ColorSchemes[scheme], 0, TextPrintType(TPF_EFNT | TPF_NOSHADOW));	// RA2: as above
			}
		}
	}
}




/***************************************************************************
 * IPXManagerClass::Reset_Response_Time -- Reset response time             *
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
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Reset_Response_Time(bool zero)
{
	int i;

	for (i = 0; i < NumConnections; i++) {
		Connection[i]->Queue->Reset_Response_Time(zero);
	}

	if (GlobalChannel) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
		GlobalChannel->Queue->Reset_Response_Time(true);
	}
	if (LobbyChannel) { // RA2: the lobby channel (magic 0xCDEF, members +0x18/+0x1C) and the multicast channel are part of the RA2 class
		LobbyChannel->Queue->Reset_Response_Time(true); // RA2: as above
	}
	if (MulticastChannel) { // RA2: as above
		MulticastChannel->Queue->Reset_Response_Time(true); // RA2: as above
	}
}


/***************************************************************************
 * IPXManagerClass::Oldest_Send -- gets ptr to oldest send buf             *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      buf ptr                                                            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/04/1995 BRR : Created.                                             *
 *=========================================================================*/
void * IPXManagerClass::Oldest_Send(void)
{
	int i,j;
	unsigned int time;
	unsigned int mintime = 0xffffffff;
	SendQueueType *send_entry;				// ptr to send entry header
	CommHeaderType *packet;
	void *buf = NULL;

	for (i = 0; i < NumConnections; i++) {

		send_entry = NULL;

		for (j = 0; j < Connection[i]->Queue->Num_Send(); j++) {
			send_entry = Connection[i]->Queue->Get_Send(j);
			if (send_entry) {
				packet = (CommHeaderType *)send_entry->Buffer;
				if (packet->Code == ConnectionClass::PACKET_DATA_ACK &&
					send_entry->IsACK == 0) {
					break;
				}
				else {
					send_entry = NULL;
				}
			}
		}

		if (send_entry!=NULL) {

			time = send_entry->FirstTime;

			if (time < mintime) {
				mintime = time;
				buf = send_entry->Buffer;
			}
		}
	}

	return(buf);

}	/* end of Oldest_Send */


/***************************************************************************
 * IPXManagerClass::Set_Bridge -- prepares to cross a bridge               *
 *                                                                         *
 * This routine is designed to prevent the connection from having to       *
 * call Get_Local_Target, except the minimum number of times, since that   *
 * routine is buggy & goes away for long periods sometimes.                *
 *                                                                         *
 * INPUT:                                                                  *
 *      bridge      network number of the destination bridge               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none                                                               *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none                                                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   07/06/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Set_Bridge(NetNumType bridge)
{
	if (GlobalChannel) {
		GlobalChannel->Set_Bridge(bridge);
	}

}	/* end of Set_Bridge */


/***************************************************************************
 * IPXManagerClass::Configure_Debug -- sets up special debug values        *
 *                                                                         *
 * Mono_Debug_Print2() can look into a packet to pull out a particular     *
 * ID, and can print both that ID and a string corresponding to            *
 * that ID.  This routine configures these values so it can find           *
 * and decode the ID.  This ID is used in addition to the normal           *
 * CommHeaderType values.                                                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      index            connection index to configure (-1 = Global Channel)*
 *      type_offset      ID's byte offset into packet                      *
 *      type_size      size of ID, in bytes; 0 if none                     *
 *      names            ptr to array of names; use ID as an index into this*
 *      namestart      numerical value of 1st name in the array            *
 *      namecount      # in the names array; 0 if none.                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Names shouldn't be longer than 12 characters.                      *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/31/1995 BRR : Created.                                             *
 *=========================================================================*/
void IPXManagerClass::Configure_Debug(int index, int type_offset, int type_size, // RA2: RA2's form, as in YR's body; TibSun's form does not match
	char **names, int namestart, int namecount) // RA2: as above
{
	if (index == CONNECTION_NONE) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
		GlobalChannel->Queue->Configure_Debug(type_offset, type_size, names,
			namestart, namecount);
	} else if (Connection[index]) { // RA2: RA2's form, as in YR's body; TibSun's form does not match
		Connection[index]->Queue->Configure_Debug(type_offset, type_size, names,
			namestart, namecount);
	}
}


/***************************************************************************
 * IPXManagerClass::Mono_Debug_Print -- debug output routine               *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of connection to display (-1 = Global Channel) *
 *      refresh      1 = complete screen refresh                           *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/25/1995 BR : Created.                                              *
 *=========================================================================*/
void IPXManagerClass::Mono_Debug_Print(int, int) // RA2: RA2's form, as in YR's body; TibSun's form does not match
{
}

void IPXManagerClass::Set_Lobby_Address(IPXAddressClass *address) // RA2: 0x005277C0
{
	if (LobbyChannel) { // RA2: as above
		LobbyChannel->Address = *address; // RA2: as above
	}
}


void IPXManagerClass::Clear_Global_Routed(void) // RA2: 0x005277F0
{
	if (GlobalChannel) { // RA2: as above
		GlobalChannel->IsRouted = false; // RA2: as above
	}
}


void IPXManagerClass::Set_Global_Routed(void) // RA2: 0x00527800
{
	if (GlobalChannel) { // RA2: as above
		GlobalChannel->IsRouted = true; // RA2: as above
	}
}


void IPXManagerClass::Discard_Global_Packets_To(IPXAddressClass *address, unsigned short port) // RA2: 0x00527810
{
	if (GlobalChannel) { // RA2: as above
		GlobalChannel->Discard_Packets_To(address, port); // RA2: as above
	}
}


void IPXManagerClass::Discard_Old_Global_Packets(int age) // RA2: 0x00527830
{
	if (GlobalChannel) { // RA2: as above
		GlobalChannel->Discard_Old_Packets(age); // RA2: as above
	}
}





















	/* end of Service */ // RA2: as above


/*************************** end of ipxmgr.cpp *****************************/
