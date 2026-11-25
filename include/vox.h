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

#pragma once

#include "vox.hh"
#include "audlist.h" // RA2: the waiting speeches are in lists of the audio library

struct IStream; // RA2: the speech queue is saved to a stream
class CCINIClass;

// RA2: TibSun's Speak routines have no counterpart in RA2 (EVA speaks through Queue_Speech); code not yet matched still calls them
void Speak(VoxType voice, bool now=false); // RA2: no such routine
bool Is_Speaking(void); // RA2: 0x00715E20

void Set_Speech_Volume(int volume); // RA2: 0x00715EF0

// RA2: the EVA speech is a list of VoxClass objects (one for each [DialogList] entry of EVA.ini) and a queue of
// waiting speeches, which are played one at a time on a stream of the audio library.
enum VoxSpeechType {
	VOXSPEECH_STANDARD, // RA2: 0; plays when nothing else waits
	VOXSPEECH_QUEUE, // RA2: 1; waits its turn
	VOXSPEECH_INTERRUPT, // RA2: 2; drops the waiting interrupts and stops the speech playing
	VOXSPEECH_QUEUED_INTERRUPT // RA2: 3; waits in the first list
};
enum VoxSpeechPriority {
	VOXPRIORITY_LOW,
	VOXPRIORITY_NORMAL,
	VOXPRIORITY_IMPORTANT,
	VOXPRIORITY_CRITICAL
};
enum VoxSpeechState {
	VOXSTATE_PLAYING,
	VOXSTATE_QUEUED,
	VOXSTATE_IDLE
};

class VoxClass
{
	public:
		VoxClass(char const * name); // RA2: 0x007160E0; adds itself to the list
		~VoxClass(void); // RA2: 0x00716190; leaves the list

		bool Read_INI(CCINIClass & ini); // RA2: 0x007161E0; the volume, type, priority and the two file names of the EVA.ini section
		char * Speech_File(void); // RA2: 0x00716770; the file of the Allied or the Soviet speech

		static char const * __fastcall Name_From(int index); // RA2: 0x007158A0; "none" out of range
		static void __fastcall Process(CCINIClass & ini); // RA2: 0x007163F0; makes a VoxClass for each [DialogList] entry
		static VoxClass * __fastcall Find(char const * name); // RA2: 0x007166C0
		static int __fastcall Index_Of(VoxClass * vox); // RA2: 0x00716740

	public:
		char Name[40]; // RA2: +0x00; the EVA.ini section
		float Volume; // RA2: +0x28
		char Russian[9]; // RA2: +0x2C; the Soviet speech file, without ".WAV"
		char Allied[9]; // RA2: +0x35; the Allied speech file
		int Priority; // RA2: +0x40; VoxSpeechPriority
		int Type; // RA2: +0x44; VoxSpeechType
		int State; // RA2: +0x48; VoxSpeechState
};

template<class T> class DynamicVectorClass;
extern DynamicVectorClass<VoxClass *> Voxs; // RA2: 0x00AD0930; the EVA voices (vox.cpp)

int Speech_Init(void); // RA2: 0x007156D0; makes the "EVA Stream" stream, 0 without one. Returns int, so the names.csv row ?Speech_Init@@YIXXZ is stale
void Speech_Shutdown(void); // RA2: 0x00715780
void Flush_Speech(void); // RA2: 0x007157B0; drops every waiting speech
void __fastcall Queue_Speech(int index, int type, int priority); // RA2: 0x007158C0; plays the VoxClass at an index (-1 for its own type or priority)
void __fastcall Queue_Speech(char const * name, int type, int priority); // RA2: 0x00715B40; the same by name
void Speech_Service(void); // RA2: 0x00715BA0; starts the next waiting speech when the stream is free
void __fastcall Speech_Stop(bool clear); // RA2: 0x00715DE0; stops the speech playing and, with clear, drops the waiting ones
void __fastcall Speech_Cancel(int index); // RA2: 0x00715E80; drops the waiting speeches of the VoxClass at an index
int Aux_Stream_Init(void); // RA2: 0x00715F10; makes the taunt stream. Returns int, so the names.csv row ?Aux_Stream_Init@@YIXXZ is stale
void Aux_Stream_Shutdown(void); // RA2: 0x00715F80
int __fastcall Play_Taunt(char taunt); // RA2: 0x00715FB0; low four bits the taunt (1 to 8), the next four the side
void __fastcall Aux_Stream_Stop(void); // RA2: 0x007160D0
void Speech_Free_All(void); // RA2: 0x00716590; stops the speech and deletes every VoxClass
char * __fastcall Vox_Name(int index); // RA2: 0x00716720; the name of the VoxClass at an index, "none" for -1 or out of range; no caller
VoxType __fastcall Vox_From_Name(char const * name); // RA2: 0x00716640; the index of the VoxClass of this name, -1 for none
bool __fastcall Is_Speech_Finished(int voice); // RA2: 0x00716690; true unless the VoxClass at that index is playing or queued
long __fastcall Speech_Load(IStream * stream); // RA2: 0x007167D0; queues the speeches of a saved game
bool Speech_Always_True(void); // RA2: 0x00716780; returns true, nothing calls it
long __fastcall Speech_Save(IStream * stream); // RA2: 0x00716790
void __fastcall Speech_Set_Side(int soviet); // RA2: 0x007168C0; nonzero plays the Soviet speech files
void Aux_Stream_Pause(void); // RA2: 0x007168D0
void Aux_Stream_Resume(void); // RA2: 0x007168E0
void Aux_Stream_Hold(void); // RA2: 0x007168F0; keeps taunts from starting
void Aux_Stream_Unblock(void); // RA2: 0x00716910
void Add_Speech_Hold(void); // RA2: 0x00716940; keeps speeches from queueing
void Remove_Speech_Hold(void); // RA2: 0x00716950
void Theme_Suspend(void); // RA2: 0x00716980; pauses the speech stream and keeps speeches from starting
void Speech_Reset(void); // RA2: 0x007169A0; stops the speech and clears both holds
void Theme_Resume(void); // RA2: 0x007169F0
