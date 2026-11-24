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

/* $Header: /CounterStrike/QUEUE.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : QUEUE.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/08/94                                                     *
 *                                                                                             *
 *                  Last Update : December 9, 1994 [JLB]                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   QueueClass<T,size>::Add -- Add object to queue.                                           *
 *   QueueClass<T,size>::First -- Fetches reference to first object in list.                   *
 *   QueueClass<T,size>::Init -- Initializes queue to empty state.                             *
 *   QueueClass<T,size>::Next -- Throws out the head of the line.                              *
 *   QueueClass<T,size>::operator[] -- Fetches reference to sub object in queue.               *
 *   QueueClass<T,size>::QueueClass -- Default constructor for QueueClass objects.             *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "mission.h"
#include "target.h"
#include "win.h"

/*
**	This class implements a classic FIFO queue (also known as - standing in line). Objects
**	are added to the end (tail) of the line. Objects are removed from the start (first) of
**	the line. A keyboard buffer is a good example of a common computer use of a queue. There
**	is no provision for "taking cuts" or leaving the line once an object has been added.
**
**	The implementation uses a circular list of objects. This allows adding and deleting of
**	elements without any maintenance moves of remaining objects that would otherwise be
**	necessary in a simple array storage method. A side effect of this means that accessing the
**	internal array directly is not allowed. Supporting functions are provided for this purpose.
**
**	WARNING WARNING WARNING WARNING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**	The size parameter MUST be an exact power of two (2, 4, 8, 16, etc.) otherwise the internal
**	indexing algorithm will fail.
*/
template<class T, int size>
class QueueClass
{
	public:
		/*
		**	This is the count of the number of objects in the queue. If this count is zero,
		**	then the operator[], First(), and Next() functions are undefined. Check this
		**	value BEFORE calling these functions.
		*/
		const int Count;

		//-------------- Functions --------------------
		QueueClass(void);						// Default constructor.

		/*
		**	The bracket subscript operator functions similarly to the way a normal subscript
		**	operator works except that entry [0] matches the first-in-line and entry
		**	[Count-1] matches the last-in-line. This is ensured regardless of the actual position
		**	of the object in the circular internal list.
		*/
		T & operator[](int);

		/*
		**	This function will return a reference to the "head of the line" object.
		*/
		T & First(void);

		/*
		**	This function clears the list of objects.
		*/
		void Init(void);

		/*
		**	This function discards the head-of-the-line object and advances all the remaining
		**	objects up by one. Mnemonic: Imagine a broadway audition and the director yells
		**	"NEXT!"
		*/
		int Next(void);

		/*
		**	This will add an object to the tail of the line. If there is no more room to add
		**	the object, then false will be returned.
		*/
		int Add(T const &);
		int Add(T const &, unsigned long timing); // RA2: keeps the time the object was queued elsewhere

		int Get_Head(void);
		int Get_Tail(void);
		T * Get_Array(void);

	public: // RA2: the queue's storage is read and written directly by the event code
		int Head;								// Index of element in list the longest.
		int Tail;								// Index where next new addition will go.

		T Array[size];							// Raw array of objects.
		unsigned long Timings[size];			// RA2: the time each object was added (timeGetTime)
};


/***********************************************************************************************
 * QueueClass<T,size>::QueueClass -- Default constructor for QueueClass objects.               *
 *                                                                                             *
 *    This default constructor for QueueClass objects initializes the queue to an empty        *
 *    state.                                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline QueueClass<T,size>::QueueClass(void) : Count(0)
{
	Init();
}


/***********************************************************************************************
 * QueueClass<T,size>::Init -- Initializes queue to empty state.                               *
 *                                                                                             *
 *    This function resets the queue to an empty state.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline void QueueClass<T,size>::Init(void)
{
	((int &)Count) = 0;
	Head = 0;
	Tail = 0;
}


/***********************************************************************************************
 * QueueClass<T,size>::Add -- Add object to queue.                                             *
 *                                                                                             *
 *    This function is used to add an object to the tail of the line. If the queue cannot      *
 *    accept any more entries, then the object won't be added and false will be returned.      *
 *                                                                                             *
 * INPUT:   object   -- The object that is to be added to the queue.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added successfully?                                           *
 *                                                                                             *
 * WARNINGS:   If the queue is full, then the object won't be added. Be sure to check for this.*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline int QueueClass<T,size>::Add(T const &q)
{
	if (Count < size) {
		Array[Tail] = q;
		Timings[Tail] = timeGetTime(); // RA2
		Tail = (Tail + 1) & (size-1);
		((int &)Count) = Count + 1;
		return(true);
	}
	return(false);
}


template<class T, int size>
inline int QueueClass<T, size>::Add(T const & q, unsigned long timing)
{
	if (Count < size) {
		Array[Tail] = q;
		Timings[Tail] = timing;
		Tail = (Tail + 1) & (size-1);
		((int &)Count) = Count + 1;
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * QueueClass<T,size>::Next -- Throws out the head of the line.                                *
 *                                                                                             *
 *    This routine is used to discard the object at the head of the line. All remaining        *
 *    objects "move up" one. No actual movement occurs, merely the index is adjusted, but      *
 *    the affect is that the next oldest object in the queue will now be returned with the     *
 *    next call to the First() function.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the number of object remaining in the queue.                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline int QueueClass<T,size>::Next(void)
{
	if (Count) {
		Head = (Head + 1) & (size-1);
		((int &)Count) = Count - 1;
	}
	return(Count);
}


/***********************************************************************************************
 * QueueClass<T,size>::operator[] -- Fetches reference to sub object in queue.                 *
 *                                                                                             *
 *    Use this routine to examine individual objects within the queue. The oldest object in    *
 *    the queue is referenced by an index value of zero. The newest object in the queue is     *
 *    referenced by a value of Count-1. If there are no objects in the queue, then this        *
 *    operator is undefined. Although this operator allows examination of the queue, there is  *
 *    no corresponding ability to insert or delete objects from the middle of the queue.       *
 *                                                                                             *
 * INPUT:   index -- The index into the queue of objects. Valid values range from zero to      *
 *                   Count-1. All other values return an undefined reference!                  *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the object indicated by the index.                     *
 *                                                                                             *
 * WARNINGS:   Check to make sure that Count is not zero before using this operator. Failure   *
 *             to do so will return a reference to an undefined object.                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline T & QueueClass<T,size>::operator[](int index)
{
	return(Array[(Head + index) & (size-1)]);
}


/***********************************************************************************************
 * QueueClass<T,size>::First -- Fetches reference to first object in list.                     *
 *                                                                                             *
 *    This routine is used to fetch the first object in the list (head of the line). This      *
 *    object is the oldest in the list. Typical use of this function is to get and process     *
 *    the first object so that it may be discarded with the Next() function in order to bring  *
 *    subsequent objects to the first position.                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the oldest object in the queue.                        *
 *                                                                                             *
 * WARNINGS:   If there are no objects in the queue, then this function returns an undefined   *
 *             reference. Be sure to check Count against zero before calling this function.    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
template<class T, int size>
inline T & QueueClass<T,size>::First(void)
{
	return(Array[Head]);
}

template<class T, int size>
inline int QueueClass<T,size>::Get_Head(void)
{
	return(Head);
}

template<class T, int size>
inline int QueueClass<T,size>::Get_Tail(void)
{
	return(Tail);
}

template<class T, int size>
inline T * QueueClass<T,size>::Get_Array(void)
{
	return(Array);
}

bool Queue_Mission(TargetClass whom, MissionType mission, TargetClass & target, TargetClass & destination);
bool Queue_Mission(TargetClass whom, MissionType mission, TargetClass & target, TargetClass & destination, SpeedType speed, MPHType maxspeed);
bool Queue_Options(void);
bool Queue_PageUser(void);
bool Queue_Exit(void);
void Queue_AI(void);
void Add_CRC(unsigned int *crc, unsigned int val);

void Wait_For_End_Of_Queue(void);

class IPXAddressClass;
struct GlobalPacketType;
void Kick_Packet_Received(GlobalPacketType & packet, IPXAddressClass & address);

extern BasicTimerClass<SystemTimerClass> SentFrameSyncTimer;
extern int SentFrameSyncCount;

// Declarations moved from the units that used them (one declaration per name).
extern void __fastcall Enqueue_Event(EventClass * event); // RA2: 0x0062E6E0 (queue.cpp); puts the event on the list to run
extern bool __fastcall Add_Outgoing_Event(EventClass event); // RA2: 0x0062E670 (queue.cpp); sends the event to the other players, false when the list is full
extern unsigned int RouterResponseTimes[]; // RA2: 0x00A76C88, the packet router round trip per connection (Multiplayer_Debug_Print "Rout resp")
extern bool ExtraSyncInfo; // RA2: 0x00AB62E8, set by -MPDEBUG


// Frame synchronization and saved-state records used by the multiplayer event queue.

struct FrameSyncStruct {
		int frame;				/// other players' frame #
		unsigned int sent;		// # cmds other player claims to have sent
		unsigned int recv;		// # cmds actually received from others
		unsigned int timing;		// RA2: +0x0C, the timer value when this player's frame sync arrived
		unsigned int response;		// RA2: +0x10, this player's response time (YR's unknown2)
		unsigned int heard;		// RA2: +0x14, timeGetTime() when this player was last heard from (the constructor clears it)

		FrameSyncStruct(void);
};

#pragma pack(push, 1)

struct MegaFrameEntry { // RA2: one player's entry in a MEGAFRAMEINFO event from the packet router (13 bytes, after the event header)
		long Frame; // RA2: +0x00
		long Unknown_04; // RA2: +0x04
		unsigned short CommandCount; // RA2: +0x08
		unsigned char Delay; // RA2: +0x0A
		char ID; // RA2: +0x0B
		unsigned char Flags; // RA2: +0x0C, bit 0 set: no response time in this entry
};

struct PacketTimingEntry { // RA2: one player's entry in a PACKET_TIMING event from the packet router (8 bytes)
		char ID; // RA2: +0x00
		bool Report; // RA2: +0x01, set: the master reports this player
		unsigned long ResponseTime; // RA2: +0x02
		short Unknown_06; // RA2: +0x06
};

#pragma pack(pop)

struct SyncHouse { // RA2: one house in a saved sync state
		char Name[20]; // RA2: +0x00
		unsigned char IsHuman; // RA2: +0x14
		char Color[32]; // RA2: +0x15
		int ID; // RA2: +0x38, -1 when the house does not exist
		char HouseType[32]; // RA2: +0x3C
};

struct SyncObject { // RA2: one infantry, unit, aircraft or building in a saved sync state
		int Coord; // RA2: +0x00
		int Type; // RA2: +0x04
		char TypeName[24]; // RA2: +0x08
		char Owner; // RA2: +0x20, a house index
		int TargetID; // RA2: +0x24
		char Target[24]; // RA2: +0x28
		int NavComID; // RA2: +0x40
		char NavCom[24]; // RA2: +0x44
		int SpeedAccum; // RA2: +0x5C
		short TrackNumber; // RA2: +0x60
		short TrackIndex; // RA2: +0x62
		unsigned char Facing; // RA2: +0x64
		unsigned char Facing2; // RA2: +0x65
		unsigned char CurrentMission; // RA2: +0x66
		char Speed; // RA2: +0x67
};

struct SyncLayerObject { // RA2: one object of a map layer or the logic layer in a saved sync state
		int Value; // RA2: +0x00
		int Type; // RA2: +0x04
		char Name[24]; // RA2: +0x08
		char Owner; // RA2: +0x20, a house index, -1 for none
		char Unknown_21[3]; // RA2: +0x21
		short RTTI; // RA2: +0x24
};

struct SyncState { // RA2: the game state of one frame, kept for the sync log (256 of them, one per CRC slot)
		int GameCRC; // RA2: +0x000
		int RandomNumber; // RA2: +0x004
		int Frame; // RA2: +0x008
		SyncHouse Houses[8]; // RA2: +0x00C
		int ObjectCount; // RA2: +0x2EC
		int Counts[4]; // RA2: +0x2F0, infantry, units, aircraft, buildings
		SyncObject * Objects; // RA2: +0x300, the records in that order
		int LayerCounts[5]; // RA2: +0x304
		SyncLayerObject * Layers[5]; // RA2: +0x318
		int LogicCount; // RA2: +0x32C
		SyncLayerObject * Logic; // RA2: +0x330
		char * Buffer; // RA2: +0x334, the records, allocated on first use
		unsigned int BufferSize; // RA2: +0x338, grown in 64K steps

		SyncState(void);
		~SyncState(void);
};


void __fastcall Stamp_Their_Timing(int index);
