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

#include "always.h"

#include "geometry.h" // RA2: this module holds its own copy of the geometry constants
#include "vox.h"

#include "audattr.h" // RA2: the speech stream takes the voice and group attributes of the audio library
#include "auddsnd.h" // RA2: the audio device the stream is made on
#include "audio.h" // RA2: whether a device is there
#include "audplay.h" // RA2: the stream that plays the speech
#include "audtimer.h" // RA2: the gap after a speech is counted in the audio library's time
#include "ccini.h" // RA2: the includes this file needs
#include "globals.h"
#include "vector.h" // RA2: the includes this file needs
#include <objidl.h> // RA2: the speech queue is saved to a stream
#include <stdio.h> // RA2: the taunt file names are made with sprintf
#include <stdlib.h> // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs


DynamicVectorClass<VoxClass *> Voxs; // RA2: 0x00AD0930; the EVA voices

/*
**	RA2: a speech waiting to be played, in one of the lists below.
*/
struct VoxSpeech : ListNode
{
	VoxClass * Vox; // RA2: +0x0C
	int Unused; // RA2: +0x10
	int Priority; // RA2: +0x14
	int Type; // RA2: +0x18
	int Serial; // RA2: +0x1C; the queue serial modulo 100

	void Get(int * type, int * priority) const
	{
		if (type != NULL) {
			*type = Type;
		}
		if (priority != NULL) {
			*priority = Priority;
		}
	}
};

void __fastcall Speech_Enqueue(VoxClass * vox, int type, int priority); // RA2: 0x007159D0; queues a speech of a VoxClass
VoxSpeech * __fastcall Speech_Find(VoxClass * vox); // RA2: 0x00715AC0; the waiting speech of a VoxClass, NULL for none

int SpeechPlayingType; // RA2: 0x00AD0858; the type of the speech playing
ListHead SpeechQueuedList; // RA2: 0x00AD0868; the queued interrupts
volatile int SpeechHold; // RA2: 0x00AD0878; nonzero keeps speeches from queueing; volatile: without it Add_Speech_Hold (0x716940) and Remove_Speech_Hold (0x716950) no longer match (tested)
int SpeechPlayingPriority; // RA2: 0x00AD0880
ListHead SpeechUrgentList; // RA2: 0x00AD0890; the critical speeches
volatile int SpeechSuspend; // RA2: 0x00AD08C8; nonzero keeps speeches from starting; volatile: without it Speech_Service (0x715BA0), Theme_Suspend (0x716980) and Theme_Resume (0x7169F0) no longer match (tested)
volatile int AuxStreamHold; // RA2: 0x00AD0910; nonzero keeps taunts from starting; volatile: without it Aux_Stream_Hold (0x7168F0) no longer matches (tested)
ListHead SpeechLists[4]; // RA2: 0x00AD08E0; the queued speeches by priority
VoxSpeech * SpeechPending; // RA2: 0x00AD0948; the one other speech that waits
int SpeechEnabled; // RA2: 0x00AD094C
int SpeechSerial; // RA2: 0x00AD0950
VoxClass * SpeechPlaying; // RA2: 0x00AD0954
int SpeechSovietFiles; // RA2: 0x00AD0958; nonzero plays the Soviet speech files
AudioStreamer * SpeechStream; // RA2: 0x00AD095C
TimeStamp SpeechDelay; // RA2: 0x00AD0960; the gap after the last speech
AudioStreamer * AuxStream; // RA2: 0x00AD0968; the stream the taunts play on
int SpeechVolume = 255; // RA2: 0x007F9BD4; the speech volume, 0 to 255


int Speech_Init(void) // RA2: 0x007156D0; empties the speech lists and makes the "EVA Stream" stream on the voice and group attributes (0 without a stream)
{
	SpeechHold = 0;
	SpeechSuspend = 0;
	for (int i = 0; i < 4; i++) {
		ListInit(&SpeechLists[i]);
	}
	ListInit(&SpeechUrgentList);
	ListInit(&SpeechQueuedList);
	SpeechPending = 0;
	SpeechEnabled = 1;
	if (SpeechStream == 0) {
		SpeechStream = AudioStreamerCreate(AudioGetDevice(), 3000);
		if (SpeechStream == 0) {
			return(0);
		}
	}
	AudioStreamerSetName(SpeechStream, "EVA Stream");
	AudioStreamerSetAttribs(SpeechStream, AudioVoiceAttribs);
	AudioStreamerSetFadeAttribs(SpeechStream, AudioGroupAttribs2);
	return(1);
}

void Speech_Shutdown(void) // RA2: 0x00715780; drops the waiting speeches, then stops and destroys the stream
{
	Flush_Speech();
	if (SpeechStream != 0) {
		AudioStreamerStop(SpeechStream);
		AudioStreamerDestroy(SpeechStream);
		SpeechStream = 0;
	}
}

void Flush_Speech(void) // RA2: 0x007157B0; drops every waiting speech, marking its VoxClass idle
{
	if (!SpeechEnabled) {
		return;
	}
	if (SpeechPending != 0) {
		SpeechPending->Vox->State = VOXSTATE_IDLE;
		delete SpeechPending;
		SpeechPending = 0;
	}
	VoxSpeech * speech;
	while ((speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList)) != 0) {
		ListNodeRemove(speech);
		speech->Vox->State = VOXSTATE_IDLE;
		delete speech;
	}
	while ((speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList)) != 0) {
		ListNodeRemove(speech);
		speech->Vox->State = VOXSTATE_IDLE;
		delete speech;
	}
	for (int i = 0; i < 4; i++) {
		while ((speech = (VoxSpeech *)ListFirstItem(&SpeechLists[i])) != 0) {
			ListNodeRemove(speech);
			speech->Vox->State = VOXSTATE_IDLE;
			delete speech;
		}
	}
}

char const *__fastcall VoxClass::Name_From(int index) // RA2: 0x007158A0; the name of the VoxClass at an index, "none" out of range
{
	if (index != -1 && index >= 0 && index < Voxs.Count()) {
		return Voxs[index]->Name;
	}
	return "none";
}

void __fastcall Queue_Speech(int index, int type, int priority) // RA2: 0x007158C0; plays the VoxClass at an index unless it is playing already; an interrupt drops the queued interrupts and stops the speech playing first
{
	if (SpeechStream != 0 && index >= 0 && index < Voxs.Count()
			&& SpeechHold == 0) {
		VoxClass * vox = Voxs[index];
		if (vox != SpeechPlaying) {
			if (type == -1) {
				type = vox->Type;
			}
			if (priority == -1) {
				priority = vox->Priority;
			}
			if (SpeechPlaying != 0 && type == VOXSPEECH_INTERRUPT) {
				VoxSpeech * speech;
				while ((speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList)) != 0) {
					ListNodeRemove(speech);
					speech->Vox->State = VOXSTATE_IDLE;
					delete speech;
				}
				Speech_Stop(true);
				SpeechDelay = 0;
			}
			VoxSpeech * speech = Speech_Find(vox);
			if (speech == 0 || speech->Type != type) {
				Speech_Enqueue(vox, type, priority);
			}
			Speech_Service();
		}
	}
}

void __fastcall Speech_Enqueue(VoxClass *vox, int type, int priority) // RA2: 0x007159D0; makes a waiting speech: queued interrupts and the queue lists by priority and critical ones wait, any other is kept as the one pending speech when nothing else waits and it outranks the pending one, else dropped
{
	VoxSpeech * speech = new VoxSpeech;
	ListNodeInit(speech);
	speech->Vox = vox;
	speech->Priority = priority;
	speech->Type = type;
	speech->Serial = SpeechSerial % 100;
	SpeechSerial++;
	vox->State = VOXSTATE_QUEUED;
	if (type == VOXSPEECH_QUEUED_INTERRUPT) {
		ListNodeInsert(&SpeechQueuedList, speech);
	} else if (type == VOXSPEECH_QUEUE) {
		ListNodeInsert(&SpeechLists[priority], speech);
	} else if (priority == VOXPRIORITY_CRITICAL) {
		ListNodeInsert(&SpeechUrgentList, speech);
	} else if (SpeechQueuedList.prev == &SpeechQueuedList
			&& SpeechUrgentList.prev == &SpeechUrgentList
			&& (SpeechPending == 0 || SpeechPending->Priority < priority)) {
		SpeechPending = speech;
	} else {
		vox->State = VOXSTATE_IDLE;
		delete speech;
	}
}

VoxSpeech *__fastcall Speech_Find(VoxClass *vox) // RA2: 0x00715AC0; the waiting speech of a VoxClass: the critical list, the pending speech, the queue lists from critical down, then the queued interrupts
{
	VoxSpeech * speech;
	for (speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList); speech != 0;
		speech = (VoxSpeech *)ListNextItem(speech)) {
		if (speech->Vox == vox) {
			return speech;
		}
	}
	if (SpeechPending != 0 && SpeechPending->Vox == vox) {
		return SpeechPending;
	}
	for (int priority = VOXPRIORITY_CRITICAL; priority >= VOXPRIORITY_LOW; priority--) {
		for (speech = (VoxSpeech *)ListFirstItem(&SpeechLists[priority]); speech != 0;
			speech = (VoxSpeech *)ListNextItem(speech)) {
			if (speech->Vox == vox) {
				return speech;
			}
		}
	}
	for (speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList); speech != 0;
		speech = (VoxSpeech *)ListNextItem(speech)) {
		if (speech->Vox == vox) {
			return speech;
		}
	}
	return(0);
}

void __fastcall Queue_Speech(char const *name, int type, int priority) // RA2: 0x00715B40; the same by the name of the VoxClass; nothing for a null name
{
	if (name != 0) {
		Queue_Speech(Vox_From_Name(name), type, priority);
	}
}

void Speech_Service(void) // RA2: 0x00715BA0; when the stream is free and the gap after the last speech has passed, starts the next waiting speech on the stream (its file with ".WAV")
{
	if (Debug_Quiet || !AudioDeviceAvailable() || SpeechStream == 0
			|| AudioStreamerIsPlaying(SpeechStream)) {
		return;
	}
	if (AudioGetTime() <= AudioStreamerEndTimeStamp(SpeechStream) + SpeechDelay) {
		return;
	}
	if (SpeechSuspend != 0) {
		return;
	}
	if (SpeechPlaying != 0) {
		SpeechPlaying->State = VOXSTATE_IDLE;
		SpeechPlaying = 0;
	}
	int type = 0;
	int priority = 0;
	VoxSpeech * speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList);
	if (speech != 0) {
		ListNodeRemove(speech);
		if (SpeechPending != 0) {
			delete SpeechPending;
			SpeechPending = 0;
		}
	} else {
		speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList);
		if (speech != 0) {
			ListNodeRemove(speech);
			if (SpeechPending != 0) {
				delete SpeechPending;
				SpeechPending = 0;
			}
		} else if (SpeechPending != 0) {
			speech = SpeechPending;
			SpeechPending = 0;
		} else {
			for (int i = 3; i >= 0; i--) {
				speech = (VoxSpeech *)ListFirstItem(&SpeechLists[i]);
				if (speech != 0) {
					break;
				}
			}
			if (speech != 0) {
				ListNodeRemove(speech);
			}
		}
	}
	if (speech == 0) {
		return;
	}
	VoxClass * vox = speech->Vox;
	speech->Get(&type, &priority);
	delete speech;
	if (vox == 0) {
		return;
	}
	vox->State = VOXSTATE_IDLE;
	char filename[256];
	strcpy(filename, SpeechSovietFiles == 0 ? vox->Allied : vox->Russian);
	strcat(filename, ".WAV");
	if (AudioStreamerPlayWAV(SpeechStream, filename, 1)) {
		SpeechDelay = 500;
		SpeechPlaying = vox;
		SpeechPlayingType = type;
		SpeechPlayingPriority = priority;
		vox->State = VOXSTATE_PLAYING;
	}
}

void __fastcall Speech_Stop(bool clear) // RA2: 0x00715DE0; stops the speech playing and, with clear, drops the waiting ones too
{
	if (SpeechPlaying != 0) {
		SpeechPlaying->State = VOXSTATE_IDLE;
		SpeechPlaying = 0;
	}
	if (SpeechStream != 0) {
		AudioStreamerStop(SpeechStream);
	}
	if (clear) {
		Flush_Speech();
	}
}

// RA2: whether no speech waits
static inline bool Speech_Lists_Empty(void)
{
	if (SpeechQueuedList.prev != &SpeechQueuedList
			|| SpeechUrgentList.prev != &SpeechUrgentList
			|| SpeechPending != NULL) {
		return false;
	}
	for (int i = 0; i < 4; i++) {
		ListNode * list = &SpeechLists[i];
		if (list->prev != list) {
			return false;
		}
	}
	return true;
}

bool Is_Speaking(void) // RA2: 0x00715E20; services the queue, then whether the stream plays or a speech waits
{
	Speech_Service();
	return SpeechStream != 0
			&& (AudioStreamerIsPlaying(SpeechStream) || !Speech_Lists_Empty());
}

void __fastcall Speech_Cancel(int index) // RA2: 0x00715E80; drops every waiting speech of the VoxClass at an index
{
	if (index >= 0 && index < Voxs.Count()) {
		VoxSpeech * speech;
		while ((speech = Speech_Find(Voxs[index])) != 0) {
			if (speech->prev != speech) {
				ListNodeRemove(speech);
			}
			if (speech == SpeechPending) {
				SpeechPending = 0;
			}
			speech->Vox->State = VOXSTATE_IDLE;
			delete speech;
		}
	}
}

void Set_Speech_Volume(int volume) // RA2: 0x00715EF0; the speech volume, at most 255
{
	SpeechVolume = volume;
	if (volume >= 255) {
		SpeechVolume = 255;
	}
}

int Aux_Stream_Init(void) // RA2: 0x00715F10; makes the "Taunt Stream" stream (0 without one)
{
	if (AuxStream == 0) {
		AuxStream = AudioStreamerCreate(AudioGetDevice(), 3000);
		if (AuxStream == 0) {
			return(0);
		}
	}
	AudioStreamerSetName(AuxStream, "Taunt Stream");
	AudioStreamerSetAttribs(SpeechStream, AudioVoiceAttribs);
	AudioStreamerSetFadeAttribs(SpeechStream, AudioGroupAttribs2);
	AuxStreamHold = 0;
	return(1);
}

void Aux_Stream_Shutdown(void) // RA2: 0x00715F80; stops and destroys the taunt stream
{
	if (AuxStream != 0) {
		AudioStreamerStop(AuxStream);
		AudioStreamerDestroy(AuxStream);
		AuxStream = 0;
	}
}

int __fastcall Play_Taunt(char taunt) // RA2: 0x00715FB0; plays taunt 1 to 8 (low four bits) of a side (next four bits) from the taunts folder on the taunt stream
{
	char name[20];
	if (AuxStream == 0 || AuxStreamHold != 0) {
		return(0);
	}
	char number = (char)(taunt & 0xf);
	if (number < 1 || number > 8) {
		return(0);
	}
	switch ((taunt >> 4) & 0xf) {
		case 0:
			sprintf(name, "taunts\\tauam%02i.wav", number);
		break;
		case 1:
			sprintf(name, "taunts\\tauko%02i.wav", number);
		break;
		case 2:
			sprintf(name, "taunts\\taufr%02i.wav", number);
		break;
		case 3:
			sprintf(name, "taunts\\tauge%02i.wav", number);
		break;
		case 4:
			sprintf(name, "taunts\\taubr%02i.wav", number);
		break;
		case 5:
			sprintf(name, "taunts\\tauli%02i.wav", number);
		break;
		case 6:
			sprintf(name, "taunts\\tauir%02i.wav", number);
		break;
		case 7:
			sprintf(name, "taunts\\taucu%02i.wav", number);
		break;
		case 8:
			sprintf(name, "taunts\\tauru%02i.wav", number);
		break;
		default:
			return(0);
	}
	return AudioStreamerPlayWAV(AuxStream, name, 0);
}

void __fastcall Aux_Stream_Stop(void) // RA2: 0x007160D0; stops the taunt stream
{
	if (AuxStream != 0) {
		AudioStreamerStop(AuxStream);
	}
}

VoxClass::VoxClass(char const * name) // RA2: 0x007160E0; a speech of a name at full volume, normal priority, standard type and idle, added to the list
{
	Volume = 1.0f;
	strcpy(Name, name);
	Russian[0] = '\0';
	Allied[0] = '\0';
	Priority = VOXPRIORITY_NORMAL;
	Type = VOXSPEECH_STANDARD;
	State = VOXSTATE_IDLE;
	Voxs.Add(this);
}

VoxClass::~VoxClass(void) // RA2: 0x00716190; leaves the list
{
	Voxs.Delete(this);
}

bool VoxClass::Read_INI(CCINIClass &ini) // RA2: 0x007161E0; the volume, type, priority and the two file names of the EVA.ini section; false without the section
{
	char buffer[500];
	Volume = 1.0f;
	if (ini.Is_Present(Name)) {
		Volume = (float)ini.Get_Float(Name, "Volume", Volume);
		ini.Get_String(Name, "Type", "", buffer, sizeof(buffer));
		if (strlen(buffer) != 0) {
			if (_stricmp(buffer, "QUEUE") == 0) {
				Type = VOXSPEECH_QUEUE;
			} else if (_stricmp(buffer, "STANDARD") == 0) {
				Type = VOXSPEECH_STANDARD;
			} else if (_stricmp(buffer, "INTERRUPT") == 0) {
				Type = VOXSPEECH_INTERRUPT;
			} else if (_stricmp(buffer, "QUEUED_INTERRUPT") == 0) {
				Type = VOXSPEECH_QUEUED_INTERRUPT;
			}
		}
		ini.Get_String(Name, "Priority", "", buffer, sizeof(buffer));
		if (strlen(buffer) != 0) {
			if (_stricmp(buffer, "LOW") == 0) {
				Priority = VOXPRIORITY_LOW;
			} else if (_stricmp(buffer, "NORMAL") == 0) {
				Priority = VOXPRIORITY_NORMAL;
			} else if (_stricmp(buffer, "IMPORTANT") == 0) {
				Priority = VOXPRIORITY_IMPORTANT;
			} else if (_stricmp(buffer, "CRITICAL") == 0) {
				Priority = VOXPRIORITY_CRITICAL;
			}
		}
		ini.Get_String(Name, "Russian", "", buffer, sizeof(buffer));
		strncpy(Russian, buffer, sizeof(Russian));
		Russian[sizeof(Russian) - 1] = '\0';
		ini.Get_String(Name, "Allied", "", buffer, sizeof(buffer));
		strncpy(Allied, buffer, sizeof(Allied));
		Allied[sizeof(Allied) - 1] = '\0';
		return(true);
	}
	return(false);
}

void __fastcall VoxClass::Process(CCINIClass &ini) // RA2: 0x007163F0; makes (or finds) a VoxClass for each [DialogList] entry of EVA.ini, which then reads its own section
{
	if (ini.Is_Present("DialogList")) {
		int count = ini.Entry_Count("DialogList");
		for (int index = 0; index < count; index++) {
			char name[200];
			if (ini.Get_String("DialogList", ini.Get_Entry("DialogList", index), "", name, sizeof(name))) {
				VoxClass * vox;
				int vox_index = Vox_From_Name(name);
				if (vox_index == VOX_NONE) {
					vox = new VoxClass(name);
				} else {
					vox = Voxs[vox_index];
				}
				vox->Read_INI(ini);
			}
		}
	}
}

void Speech_Free_All(void) // RA2: 0x00716590; stops the speech, drops the waiting ones and deletes every VoxClass
{
	Speech_Stop(true);
	while (Voxs.Count() > 0) {
		delete Voxs[0];
	}
}

// RA2: the search after the null test is a separate inline step: the original returns -1 for a null name before the search saves esi
static inline int Index_Of_Name(char const * name)
{
	for (int index = 0; index < Voxs.Count(); index++) {
		if (_stricmp(name, Voxs[index]->Name) == 0) {
			return index;
		}
	}
	return VOX_NONE;
}

VoxType __fastcall Vox_From_Name(char const * name) // RA2: 0x00716640; the index of the VoxClass of a name, -1 for none or a null name
{
	if (name == NULL) {
		return VOX_NONE;
	}
	return (VoxType)Index_Of_Name(name);
}

bool __fastcall Is_Speech_Finished(int voice) // RA2: 0x00716690; true unless the VoxClass at that index is playing or queued
{
	if (voice >= 0 && voice < Voxs.Count()) {
		return Voxs[voice]->State == VOXSTATE_IDLE;
	}
	return true;
}

VoxClass *__fastcall VoxClass::Find(char const *name) // RA2: 0x007166C0; the VoxClass of a name, NULL for a null name, "<none>" or no match
{
	if (name != 0 && _stricmp(name, "<none>") != 0) {
		for (int index = 0; index < Voxs.Count(); index++) {
			if (_stricmp(name, Voxs[index]->Name) == 0) {
				return Voxs[index];
			}
		}
	}
	return(0);
}

char * __fastcall Vox_Name(int index) // RA2: 0x00716720; the name of the VoxClass at an index, "<none>" for -1 or past the end; nothing calls it
{
	if (index != -1 && index < Voxs.Count()) {
		return Voxs[index]->Name;
	}
	return "<none>";
}

int __fastcall VoxClass::Index_Of(VoxClass *vox) // RA2: 0x00716740; the index of a VoxClass in the list, -1 for none
{
	for (int index = 0; index < Voxs.Count(); index++) {
		if (Voxs[index] == vox) {
			return index;
		}
	}
	return -1;
}

char * VoxClass::Speech_File(void) // RA2: 0x00716770; the Allied file, or the Soviet one while the Soviet files are on
{
	return SpeechSovietFiles == 0 ? Allied : Russian;
}

bool Speech_Always_True(void) // RA2: 0x00716780; returns true; nothing calls it
{
	return true;
}

HRESULT __fastcall Speech_Save(IStream *stream) // RA2: 0x00716790; the markers of the speech queue; no speech is saved (SxoV, ExoV)
{
	int marker = 0x566F7853;
	HRESULT result = stream->Write(&marker, sizeof(marker), 0);
	if (SUCCEEDED(result)) {
		marker = 0x566F7845;
		result = stream->Write(&marker, sizeof(marker), 0);
	}
	return result;
}

long __fastcall Speech_Load(IStream *stream) // RA2: 0x007167D0; stops the speech, then queues the speeches a saved game lists between the SxoV and ExoV markers (IxoV, the VoxClass index, type and priority)
{
	Speech_Stop(true);
	int marker;
	HRESULT result = stream->Read(&marker, sizeof(marker), 0);
	if (FAILED(result) || marker != 0x566F7853) {
		return result;
	}
	for (result = stream->Read(&marker, sizeof(marker), 0); SUCCEEDED(result);
		result = stream->Read(&marker, sizeof(marker), 0)) {
		if (marker == 0x566F7845) {
			break;
		}
		if (marker != 0x566F7849) {
			return (HRESULT)0x80004005L;
		}
		struct {
				int Index;
				int Type;
				int Priority;
		} entry;
		result = stream->Read(&entry, sizeof(entry), 0);
		if (FAILED(result)) {
			break;
		}
		if (SpeechStream != 0 && entry.Index >= 0 && entry.Index < Voxs.Count()) {
			Speech_Enqueue(Voxs[entry.Index], entry.Type, entry.Priority);
		}
	}
	return result;
}

void __fastcall Speech_Set_Side(int side) // RA2: 0x007168C0; nonzero plays the Soviet speech files
{
	SpeechSovietFiles = side;
}

void Aux_Stream_Pause(void) // RA2: 0x007168D0
{
	if (AuxStream != 0) {
		AudioStreamerPause(AuxStream);
	}
}

void Aux_Stream_Resume(void) // RA2: 0x007168E0
{
	if (AuxStream != 0) {
		AudioStreamerResume(AuxStream);
	}
}

void Aux_Stream_Hold(void) // RA2: 0x007168F0; counts the taunt hold up while there is a taunt stream
{
	if (AuxStream != 0) {
		AuxStreamHold++;
	}
}

void Aux_Stream_Unblock(void) // RA2: 0x00716910; counts the taunt hold down, not below 0
{
	if (AuxStream != 0) {
		if (--AuxStreamHold < 0) {
			AuxStreamHold = 0;
		}
	}
}

void Add_Speech_Hold(void) // RA2: 0x00716940; counts the speech hold up
{
	SpeechHold++;
}

void Remove_Speech_Hold(void) // RA2: 0x00716950; counts the speech hold down, not below 0
{
	if (SpeechHold != 0) {
		if (--SpeechHold < 0) {
			SpeechHold = 0;
		}
	}
}

void Theme_Suspend(void) // RA2: 0x00716980; pauses the speech stream and keeps speeches from starting
{
	if (SpeechStream != 0) {
		AudioStreamerPause(SpeechStream);
	}
	SpeechSuspend++;
}

void Speech_Reset(void) // RA2: 0x007169A0; stops the speech, drops the waiting ones and clears both holds
{
	Speech_Stop(true);
	SpeechSuspend = 0;
	SpeechHold = 0;
}

void Theme_Resume(void) // RA2: 0x007169F0; resumes the speech stream and counts the suspension down, not below 0
{
	if (SpeechStream != 0) {
		AudioStreamerResume(SpeechStream);
	}
	if (SpeechSuspend != 0) {
		if (--SpeechSuspend < 0) {
			SpeechSuspend = 0;
		}
	}
}

// RA2: assembled by edit_vox.py
