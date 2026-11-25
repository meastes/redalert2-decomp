/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/IPXMGR.H 1     3/03/97 10:24a Joe_bostic $ */
/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPXMGR.H                                 *
 *                                                                         *
 *                   Programmer : Bill Randolph                            *
 *                                                                         *
 *                   Start Date : December 19, 1994                        *
 *                                                                         *
 *                  Last Update : April 3, 1995   [BR]                     *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 * This is the Connection Manager for IPX network communications.  It      *
 * creates, manages, & orchestrates multiple IPX connections, as well as   *
 * the "global" connection ("Global Channel"), which can talk to any       *
 * system on the net.                                                      *
 *                                                                         *
 * Use the Global Channel to query systems for their names, ID's, &        *
 * IPX addresses.  Then, create a Private Connection with each system      *
 * that joins your game, and use the Private Channel to send game packets  *
 * (the private channel will perform somewhat faster, & gives you better   *
 * control than the Global Channel; it can detect retries, and the Global  *
 * Channel can't).                                                         *
 *                                                                         *
 * HOW THIS CLASS WORKS:                                                   *
 * This class has to set up an IPX Event Service Routine in low (DOS)      *
 * memory.  So, it uses DPMI to allocate & lock a chunk of DOS memory;     *
 * this memory is used for all incoming packet buffers, the outgoing       *
 * packet buffer, and the actual code for the event handler.  The real-    *
 * mode handler code & this class share a portion of memory that's mapped  *
 * into a "RealModeDataType" structure.  As packets come in, the handler   *
 * points IPX to the next available packet buffer & restarts listening;    *
 * it sets a flag to tell this class that a packet is present at that      *
 * buffer slot.  This class must read all the packets & determine which    *
 * connection they go with (the Global Channel, or one of the Private      *
 * Channels).  This parsing is done in the Service routine for this class. *
 *                                                                         *
 * Constructor:   Just inits some variables, checks to see if IPX is there *
 * Destructor:    Complete shutdown; stops IPX listening, frees all memory *
 * Init:          Should only be called once (but can be called more);     *
 *                allocates all memory, creates the Global Channel         *
 *                connection, starts IPX listening.  By not placing this   *
 *                step in the constructor, the app can control when        *
 *                listening actually starts; also, you don't get a bunch   *
 *                of allocations just by declaring an IPXManagerClass      *
 *                instance.  You have to call Init() for the allocations   *
 *                to occur.                                                *
 * Connection utilities: Create & manage Private Connections.  Each        *
 *                connection has its own IPX address, numerical ID, and    *
 *                character name (presumably the name of the other         *
 *                player).                                                 *
 * Send/Get_Global_Message: adds a packet to the Global Connection queue,  *
 *                or reads from the queue.  The caller should check the    *
 *                ProductID value from returned packets to be sure it's    *
 *                talking to the right product.                            *
 * Send/Get_Private_Message: adds a packet to a Private Connection queue,  *
 *                or reads from the queue                                  *
 * Service:       Checks the Real-Mode-Memory packet array to see if any   *
 *                new packets have come in; if they have, it parses them   *
 *                & distributes them to the right connection queue.  The   *
 *                queue's Service routine handles ACK'ing or Resending     *
 *                packets.                                                 *
 *                                                                         *
 * Here's a memory map of the Real-Mode memory block.  'N' is the number   *
 * of packet buffers allocated in low memory:                              *
 *                                                                         *
 *                ----------------------------------                       *
 *                |       Shared-memory data       |                       *
 *                |--------------------------------|                       *
 *                |  Real-mode event handler code  |                       *
 *                |--------------------------------|                       *
 *                |  IPX Header & Packet Buffer 0  |                       *
 *                |--------------------------------|                       *
 *                |  IPX Header & Packet Buffer 1  |                       *
 *                |--------------------------------|                       *
 *                |  IPX Header & Packet Buffer 2  |                       *
 *                |--------------------------------|                       *
 *                |             . . .              |                       *
 *                |--------------------------------|                       *
 *                |  IPX Header & Packet Buffer N  |                       *
 *                |--------------------------------|                       *
 *                |    Send Event Control Block    |                       *
 *                |--------------------------------|                       *
 *                |         Send IPX Header        |                       *
 *                |--------------------------------|                       *
 *                |       Send Packet Buffer       |                       *
 *                |--------------------------------|                       *
 *                |        Flags Array [N]         |                       *
 *                ----------------------------------                       *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#pragma once


/*
********************************* Includes **********************************
*/
#include "connmgr.h"
#include "ipxaddr.h"
#include "ipxconn.h"
#include "ipxgconn.h"

/*
********************************** Defines **********************************
*/
/*---------------------------------------------------------------------------
This is Virgin Interactive Entertainment's registered socket ID.
---------------------------------------------------------------------------*/
#define	VIRGIN_SOCKET		0x8813

/*---------------------------------------------------------------------------
This is the maximum number of IPX connections supported.  Just change this
value to support more.
---------------------------------------------------------------------------*/
#define	CONNECT_MAX			7

/*---------------------------------------------------------------------------
These routines report the location & length of the real-mode routine, as
it's stored in protected-mode memory.
---------------------------------------------------------------------------*/
extern "C" {
	void * __cdecl Get_RM_IPX_Address(void);
	int __cdecl Get_RM_IPX_Size(void);
}

/*
***************************** Class Declaration *****************************
*/
class IPXManagerClass : public ConnManClass
{
		typedef ConnManClass BASECLASS;

	/*
	---------------------------- Public Interface ----------------------------
	*/
	public:
		/*.....................................................................
		Constructor/destructor.
		.....................................................................*/
		IPXManagerClass (int glb_maxlen, int pvt_maxlen, int glb_num_packets,
			int pvt_num_packets, unsigned short socket, unsigned short product_id);
		virtual ~IPXManagerClass () override;	// stop listening

		/*.....................................................................
		Initialization routines.
		.....................................................................*/
		int Init (void);
		int Is_IPX(void);
		virtual void Set_Timing (unsigned int retrydelta, unsigned int maxretries,
			unsigned int timeout, bool set_external = true) override;
		virtual void Set_External_Timing (unsigned int retrydelta, unsigned int maxretries,
			unsigned int timeout) override;
		void Set_Bridge(NetNumType bridge);

		/*.....................................................................
		These routines control creation of the "Connections" (data queues) for
		each remote system.
		.....................................................................*/
		int Create_Connection(int id, wchar_t const *name, IPXAddressClass *address); // RA2: wide name
		int Create_Multicast_Connection(void); // RA2: 0x005255B0, the multicast connection to the first session player
		int Delete_Connection(int id);
		virtual int Num_Connections(void) override;
		virtual int Connection_ID(int index) override;
		wchar_t *Connection_Name(int id); // RA2: wide name
		IPXAddressClass * Connection_Address(int id);
		virtual int Connection_Index(int id) override;
		void Set_Connection_Parms(int index, int id, wchar_t const *name); // RA2: wide name
		void Set_Connection_Address(int id, IPXAddressClass *address); // RA2: 0x005269F0

		/*.....................................................................
		This is how the application sends & receives messages.
		.....................................................................*/
		int Send_Global_Message (void *buf, int buflen, int ack_req = 0,
			IPXAddressClass *address = NULL, unsigned short port = 0, unsigned long packet_id = 0); // RA2: port and packet ID
		int Send_Global_Message_To_Player(void *buf, int buflen, int ack_req, int id); // RA2: 0x005258B0, to the session player with this ID
		int Send_Global_Message_To_Name(void *buf, int buflen, int ack_req, wchar_t const *name); // RA2: 0x005259A0, to the session player with this name
		int Peek_Global_Message(int index, void *buf, int *buflen, IPXAddressClass *address,
			unsigned short *product_id, unsigned long *packet_id); // RA2: 0x00525AF0
		void Mark_Global_Message_Read(int index); // RA2: 0x00525B30
		int Send_Lobby_Message(void *buf, int buflen, bool ack_req); // RA2: 0x00525B50, on the lobby channel
		int Get_Lobby_Message(void *buf, int *buflen, IPXAddressClass *address, unsigned short *product_id); // RA2: 0x00525BC0
		int Get_Global_Message (void *buf, int *buflen, IPXAddressClass *address,
			unsigned short *product_id);

		virtual int Send_Private_Message (void *buf, int buflen,
			int ack_req = 1, int conn_id = CONNECTION_NONE) override;
		virtual int Get_Private_Message (void *buf, int *buflen, int *conn_id) override;

		/*.....................................................................
		The main polling routine; should be called as often as possible.
		.....................................................................*/
		virtual int Service (void) override;

		/*.....................................................................
		This routine reports which connection has an error on it.
		.....................................................................*/
		int Get_Bad_Connection(void);

		/*.....................................................................
		Queue utility routines.  The application can determine how many
		messages are in the send/receive queues.
		.....................................................................*/
		virtual int Global_Num_Send(void) override;
		virtual int Global_Num_Receive(void) override;
		virtual int Private_Num_Send(int id = CONNECTION_NONE) override;
		virtual int Private_Num_Receive(int id = CONNECTION_NONE) override;

		/*.....................................................................
		This routine changes the socket ID assigned the IPX Manager when it
		was constructed.  Do not call this function after calling Init()!
		The Socket ID should be known by both ends of the communications before
		any packets are sent.
		.....................................................................*/
		void Set_Socket(unsigned short socket);

		/*.....................................................................
		Routines to return the largest average queue response time, and to
		reset the response time for all queues.
		.....................................................................*/
		virtual unsigned int Response_Time(void) override;
		unsigned int Global_Response_Time(void);
		virtual void Reset_Response_Time(bool zero) override;

		unsigned int Lobby_Response_Time(void); // RA2: 0x00526C50
		unsigned int Multicast_Response_Time(void); // RA2: 0x00526C70

		/*.....................................................................
		This routine returns a pointer to the oldest non-ACK'd buffer I've sent.
		.....................................................................*/
		void * Oldest_Send(void);

		/*.....................................................................
		Debug routines
		.....................................................................*/
		bool Forward_Router_Packet(CommHeaderType *packet, void *buf, int buflen, IPXAddressClass *from, unsigned short port); // RA2: 0x00525DC0
		// RA2: vtable slots 16-24 in this order (ConnManClass ends at slot 15 in release builds)
		virtual void Save_Timing(void); // RA2: slot 16, 0x00525480
		virtual void Restore_Timing(void); // RA2: slot 17, 0x005254A0
		virtual void Set_Lobby_Timing(unsigned int retrydelta, unsigned int maxretries, unsigned int timeout); // RA2: slot 18, 0x005254C0
		virtual int Index_Of_Connection(IPXConnClass * connection); // RA2: slot 19, 0x005257B0; the index, -2 global, -3 lobby, -4 multicast channel, -1 none
		virtual int Lobby_Num_Send(void); // RA2: slot 20, 0x00526A80
		virtual int Lobby_Num_Receive(void); // RA2: slot 21, 0x00526AA0
		virtual unsigned int Avg_Response_Time(int index); // RA2: slot 22
		virtual void Store_Stats(void); // RA2: slot 23
		void Set_Lobby_Address(IPXAddressClass *address); // RA2: 0x005277C0
		void Clear_Global_Routed(void); // RA2: 0x005277F0
		void Set_Global_Routed(void); // RA2: 0x00527800
		void Discard_Global_Packets_To(IPXAddressClass *address, unsigned short port); // RA2: 0x00527810
		void Discard_Old_Global_Packets(int age); // RA2: 0x00527830
		virtual void Configure_Debug(int index, int type_offset, int type_size,
			char **names, int namestart, int namecount) override;

		virtual void Mono_Debug_Print(int index, int refresh = 0);

		void Multiplayer_Debug_Print(void);

	/*
	--------------------------- Private Interface ----------------------------
	*/
	private:

		/*.....................................................................
		Misc variables
		.....................................................................*/
		bool IPXStatus;		// 0 = no IPX, 1 = IPX found
		bool Listening;		// 1 = Listening is on
		bool RealMemAllocd;	// 1 = Real-mode memory has been alloc'd

		/*.....................................................................
		Packet Sizes, used for allocating real-mode memory
		.....................................................................*/
		int Glb_MaxPacketLen;				// Global Channel maximum packet size
		int Glb_NumPackets;					// # Global send/receive packets
		int Pvt_MaxPacketLen;				// Private Channel maximum packet size
		int Pvt_NumPackets;					// # Private send/receive packets
		int Lobby_MaxPacketLen;				// RA2: Lobby Channel maximum packet size
		int Lobby_NumPackets;				// RA2: # Lobby send/receive packets

		/*.....................................................................
		The ProductID is used in the Global Channel's packet header, and it's
		used for the Private Channels' Magic Number.
		.....................................................................*/
		unsigned short ProductID;			// product ID

		/*.....................................................................
		The Socket ID, and local Novell Connection Number
		.....................................................................*/
		unsigned short Socket;				// Our socket ID for sending/receiving
		int ConnectionNum;					// local connection #, 0=not logged in

		/*.....................................................................
		Array of connection queues
		.....................................................................*/
		IPXConnClass * Connection[CONNECT_MAX]; // array of connection object ptrs
		int NumConnections;                     // # connection objects in use
		IPXGlobalConnClass *GlobalChannel;      // the Global Channel
		IPXGlobalConnClass * LobbyChannel;
		IPXConnClass * MulticastChannel;

		/*.....................................................................
		Current queue for polling for received packets
		.....................................................................*/
		int CurConnection;

		/*.....................................................................
		Timing parameters for all connections
		.....................................................................*/
		unsigned int RetryDelta;
		unsigned int MaxRetries;
		unsigned int Timeout;
		unsigned int SavedRetryDelta;
		unsigned int SavedMaxRetries;
		unsigned int SavedTimeout;

		/*---------------------------------------------------------------------
		Real-mode memory pointers and such
		---------------------------------------------------------------------*/
		/*.....................................................................
		This is a structure that mirrors data in real-mode memory:
		.....................................................................*/
		struct RealModeDataType {
			short Marker1;                  // the byte ID marker
			ECBType ListenECB;              // the Listening ECB
			short NumBufs;                  // # of buffers we're giving to the handler
			char *BufferFlags;              // array of buffer-avail flags
			short PacketSize;               // size of packet including IPX header
			IPXHeaderType *FirstPacketBuf;  // ptr to 1st packet buffer
			short CurIndex;                 // handler's current packet index
			IPXHeaderType *CurPacketBuf;    // handler's current packet buf
			short FuncOffset;               // contains offset of code
			char Semaphore;                 // prevents re-entrancy
			short ReEntrantCount;           // times we've been called re-entrantly
			short StackPtr;                 // real-mode stack pointer
			short StackSeg;                 // real-mode stack segment
			short StackPtr_int;             // internal stack pointer
			short StackSeg_int;             // internal stack segment
			short StackCheck;               // stack check value (0x1234)
			short Stack[256];               // actual stack space
			short StackSpace;               // label for top of stack
			short Marker2;                  // the byte ID marker
		};

		/*.....................................................................
		The number & size of packet buffers in low memory
		.....................................................................*/
		int NumBufs;        // # packet buffers allocated
		int PacketLen;      // size of packet without IPX header
		int FullPacketLen;  // size of packet including IPX header

#if 0
		/*.....................................................................
		Selector & Segment of the DOS allocation;
		Size of the allocation;
		Ptr to the real-mode assembly data area
		.....................................................................*/
		unsigned short Selector;				// selector of DOS allocation pointer
		unsigned short Segment;					// real-mode segment of DOS allocation
		int RealMemSize;							// size of real mode memory allocated
		RealModeDataType *RealModeData;		// assembly routine & its data

		/*.....................................................................
		This is a real-mode pointer to the address of the real-mode assembly
		entry point.
		.....................................................................*/
		int Handler;

		/*.....................................................................
		Event Control Block for listening; contained within the real-mode
		assembly routine's data area
		.....................................................................*/
		ECBType *ListenECB;						// ECB for listening
#endif

		/*.....................................................................
		ptr to the 1st header & data buffers in the packet buffer array
		.....................................................................*/
		IPXHeaderType *FirstHeaderBuf;  // array of packet headers & buffers
		char *FirstDataBuf;             // 1st data buffer area

		/*.....................................................................
		Current packet index & ptrs for parsing packets
		.....................................................................*/
		int CurIndex;                   // Current packet index, for reading
		IPXHeaderType *CurHeaderBuf;    // Current packet ptr, for reading
		char *CurDataBuf;               // Current actual data ptr

		/*.....................................................................
		ECB, header, & buffer for sending
		.....................................................................*/
		ECBType *SendECB;           // ECB for sending
		IPXHeaderType *SendHeader;  // Header for sending
		char *SendBuf;              // buffer for sending

		/*.....................................................................
		Flags indicating whether a buffer contains data or not (1 = full)
		The IPXManager must clear this flag; the real-mode routine will set it.
		.....................................................................*/
		char *BufferFlags;						// array of rx-buffer-avail flags

		/*.....................................................................
		Various Statistics
		.....................................................................*/
		int SendOverflows;
		int ReceiveOverflows;
		int BadConnection;
};


/*************************** end of ipxmgr.h *******************************/
