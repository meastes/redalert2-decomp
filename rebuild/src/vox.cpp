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
struct VoxSpeech : ListNode // RA2: as above
{
	VoxClass * Vox; // RA2: +0x0C
	int Unused; // RA2: +0x10
	int Priority; // RA2: +0x14
	int Type; // RA2: +0x18
	int Serial; // RA2: +0x1C; the queue serial modulo 100

	void Get(int * type, int * priority) const // RA2: as above
	{
		if (type != NULL) { // RA2: as above
			*type = Type;
		}
		if (priority != NULL) { // RA2: as above
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
	SpeechHold = 0; // RA2: as above
	SpeechSuspend = 0; // RA2: as above
	for (int i = 0; i < 4; i++) { // RA2: as above
		ListInit(&SpeechLists[i]); // RA2: as above
	}
	ListInit(&SpeechUrgentList); // RA2: as above
	ListInit(&SpeechQueuedList); // RA2: as above
	SpeechPending = 0; // RA2: as above
	SpeechEnabled = 1; // RA2: as above
	if (SpeechStream == 0) { // RA2: as above
		SpeechStream = AudioStreamerCreate(AudioGetDevice(), 3000); // RA2: as above
		if (SpeechStream == 0) { // RA2: as above
			return(0); // RA2: as above
		}
	}
	AudioStreamerSetName(SpeechStream, "EVA Stream"); // RA2: as above
	AudioStreamerSetAttribs(SpeechStream, AudioVoiceAttribs); // RA2: as above
	AudioStreamerSetFadeAttribs(SpeechStream, AudioGroupAttribs2); // RA2: as above
	return(1); // RA2: as above
}

void Speech_Shutdown(void) // RA2: 0x00715780; drops the waiting speeches, then stops and destroys the stream
{
	Flush_Speech(); // RA2: as above
	if (SpeechStream != 0) { // RA2: as above
		AudioStreamerStop(SpeechStream); // RA2: as above
		AudioStreamerDestroy(SpeechStream); // RA2: as above
		SpeechStream = 0; // RA2: as above
	}
}

void Flush_Speech(void) // RA2: 0x007157B0; drops every waiting speech, marking its VoxClass idle
{
	if (!SpeechEnabled) { // RA2: as above
		return; // RA2: as above
	}
	if (SpeechPending != 0) { // RA2: as above
		SpeechPending->Vox->State = 2; // RA2: as above
		delete SpeechPending; // RA2: as above
		SpeechPending = 0; // RA2: as above
	}
	VoxSpeech *speech; // RA2: as above
	while ((speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList)) != 0) { // RA2: as above
		ListNodeRemove(speech); // RA2: as above
		speech->Vox->State = 2; // RA2: as above
		delete speech; // RA2: as above
	}
	while ((speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList)) != 0) { // RA2: as above
		ListNodeRemove(speech); // RA2: as above
		speech->Vox->State = 2; // RA2: as above
		delete speech; // RA2: as above
	}
	for (int i = 0; i < 4; i++) { // RA2: as above
		while ((speech = (VoxSpeech *)ListFirstItem(&SpeechLists[i])) != 0) { // RA2: as above
			ListNodeRemove(speech); // RA2: as above
			speech->Vox->State = 2; // RA2: as above
			delete speech; // RA2: as above
		}
	}
}

char const *__fastcall VoxClass::Name_From(int index) // RA2: 0x007158A0; the name of the VoxClass at an index, "none" out of range
{
	if (index != -1 && index >= 0 && index < Voxs.Count()) { // RA2: as above
		return Voxs[index]->Name; // RA2: as above
	}
	return "none"; // RA2: as above
}

void __fastcall Queue_Speech(int index, int type, int priority) // RA2: 0x007158C0; plays the VoxClass at an index unless it is playing already; an interrupt drops the queued interrupts and stops the speech playing first
{
	if (SpeechStream != 0 && index >= 0 && index < Voxs.Count() // RA2: as above
			&& SpeechHold == 0) { // RA2: as above
		VoxClass *vox = Voxs[index]; // RA2: as above
		if (vox != SpeechPlaying) { // RA2: as above
			if (type == -1) { // RA2: as above
				type = vox->Type; // RA2: as above
			}
			if (priority == -1) { // RA2: as above
				priority = vox->Priority; // RA2: as above
			}
			if (SpeechPlaying != 0 && type == 2) { // RA2: as above
				VoxSpeech *speech; // RA2: as above
				while ((speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList)) != 0) { // RA2: as above
					ListNodeRemove(speech); // RA2: as above
					speech->Vox->State = 2; // RA2: as above
					delete speech; // RA2: as above
				}
				Speech_Stop(true); // RA2: as above
				SpeechDelay = 0; // RA2: as above
			}
			VoxSpeech *speech = Speech_Find(vox); // RA2: as above
			if (speech == 0 || speech->Type != type) { // RA2: as above
				Speech_Enqueue(vox, type, priority); // RA2: as above
			}
			Speech_Service(); // RA2: as above
		}
	}
}

void __fastcall Speech_Enqueue(VoxClass *vox, int type, int priority) // RA2: 0x007159D0; makes a waiting speech: queued interrupts and the queue lists by priority and critical ones wait, any other is kept as the one pending speech when nothing else waits and it outranks the pending one, else dropped
{
	VoxSpeech *speech = new VoxSpeech; // RA2: as above
	ListNodeInit(speech); // RA2: as above
	speech->Vox = vox; // RA2: as above
	speech->Priority = priority; // RA2: as above
	speech->Type = type; // RA2: as above
	speech->Serial = SpeechSerial % 100; // RA2: as above
	SpeechSerial++; // RA2: as above
	vox->State = 1; // RA2: as above
	if (type == 3) { // RA2: as above
		ListNodeInsert(&SpeechQueuedList, speech); // RA2: as above
	} else if (type == 1) { // RA2: as above
		ListNodeInsert(&SpeechLists[priority], speech); // RA2: as above
	} else if (priority == 3) { // RA2: as above
		ListNodeInsert(&SpeechUrgentList, speech); // RA2: as above
	} else if (SpeechQueuedList.prev == &SpeechQueuedList // RA2: as above
			&& SpeechUrgentList.prev == &SpeechUrgentList // RA2: as above
			&& (SpeechPending == 0 || SpeechPending->Priority < priority)) { // RA2: as above
		SpeechPending = speech; // RA2: as above
	} else {
		vox->State = 2; // RA2: as above
		delete speech; // RA2: as above
	}
}

VoxSpeech *__fastcall Speech_Find(VoxClass *vox) // RA2: 0x00715AC0; the waiting speech of a VoxClass: the critical list, the pending speech, the queue lists from critical down, then the queued interrupts
{
	VoxSpeech *speech; // RA2: as above
	for (speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList); speech != 0; // RA2: as above
			speech = (VoxSpeech *)ListNextItem(speech)) { // RA2: as above
		if (speech->Vox == vox) { // RA2: as above
			return speech; // RA2: as above
		}
	}
	if (SpeechPending != 0 && SpeechPending->Vox == vox) { // RA2: as above
		return SpeechPending; // RA2: as above
	}
	for (int priority = 3; priority >= 0; priority--) { // RA2: as above
		for (speech = (VoxSpeech *)ListFirstItem(&SpeechLists[priority]); speech != 0; // RA2: as above
				speech = (VoxSpeech *)ListNextItem(speech)) { // RA2: as above
			if (speech->Vox == vox) { // RA2: as above
				return speech; // RA2: as above
			}
		}
	}
	for (speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList); speech != 0; // RA2: as above
			speech = (VoxSpeech *)ListNextItem(speech)) { // RA2: as above
		if (speech->Vox == vox) { // RA2: as above
			return speech; // RA2: as above
		}
	}
	return(0); // RA2: as above
}

void __fastcall Queue_Speech(char const *name, int type, int priority) // RA2: 0x00715B40; the same by the name of the VoxClass; nothing for a null name
{
	if (name != 0) { // RA2: as above
		Queue_Speech(Vox_From_Name(name), type, priority); // RA2: as above
	}
}

void Speech_Service(void) // RA2: 0x00715BA0; when the stream is free and the gap after the last speech has passed, starts the next waiting speech on the stream (its file with ".WAV")
{
	if (Debug_Quiet || !AudioDeviceAvailable() || SpeechStream == 0 // RA2: as above
			|| AudioStreamerIsPlaying(SpeechStream)) { // RA2: as above
		return; // RA2: as above
	}
	if (AudioGetTime() <= AudioStreamerEndTimeStamp(SpeechStream) + SpeechDelay) { // RA2: as above
		return; // RA2: as above
	}
	if (SpeechSuspend != 0) { // RA2: as above
		return; // RA2: as above
	}
	if (SpeechPlaying != 0) { // RA2: as above
		SpeechPlaying->State = 2; // RA2: as above
		SpeechPlaying = 0; // RA2: as above
	}
	int type = 0; // RA2: as above
	int priority = 0; // RA2: as above
	VoxSpeech *speech = (VoxSpeech *)ListFirstItem(&SpeechQueuedList); // RA2: as above
	if (speech != 0) { // RA2: as above
		ListNodeRemove(speech); // RA2: as above
		if (SpeechPending != 0) { // RA2: as above
			delete SpeechPending; // RA2: as above
			SpeechPending = 0; // RA2: as above
		}
	} else {
		speech = (VoxSpeech *)ListFirstItem(&SpeechUrgentList); // RA2: as above
		if (speech != 0) { // RA2: as above
			ListNodeRemove(speech); // RA2: as above
			if (SpeechPending != 0) { // RA2: as above
				delete SpeechPending; // RA2: as above
				SpeechPending = 0; // RA2: as above
			}
		} else if (SpeechPending != 0) { // RA2: as above
			speech = SpeechPending; // RA2: as above
			SpeechPending = 0; // RA2: as above
		} else {
			for (int i = 3; i >= 0; i--) { // RA2: as above
				speech = (VoxSpeech *)ListFirstItem(&SpeechLists[i]); // RA2: as above
				if (speech != 0) { // RA2: as above
					break;
				}
			}
			if (speech != 0) { // RA2: as above
				ListNodeRemove(speech); // RA2: as above
			}
		}
	}
	if (speech == 0) { // RA2: as above
		return; // RA2: as above
	}
	VoxClass *vox = speech->Vox; // RA2: as above
	speech->Get(&type, &priority); // RA2: as above
	delete speech; // RA2: as above
	if (vox == 0) { // RA2: as above
		return; // RA2: as above
	}
	vox->State = 2; // RA2: as above
	char filename[256]; // RA2: as above
	strcpy(filename, SpeechSovietFiles == 0 ? vox->Allied : vox->Russian); // RA2: as above
	strcat(filename, ".WAV"); // RA2: as above
	if (AudioStreamerPlayWAV(SpeechStream, filename, 1)) { // RA2: as above
		SpeechDelay = 500; // RA2: as above
		SpeechPlaying = vox; // RA2: as above
		SpeechPlayingType = type; // RA2: as above
		SpeechPlayingPriority = priority; // RA2: as above
		vox->State = 0; // RA2: as above
	}
}

void __fastcall Speech_Stop(bool clear) // RA2: 0x00715DE0; stops the speech playing and, with clear, drops the waiting ones too
{
	if (SpeechPlaying != 0) { // RA2: as above
		SpeechPlaying->State = 2; // RA2: as above
		SpeechPlaying = 0; // RA2: as above
	}
	if (SpeechStream != 0) { // RA2: as above
		AudioStreamerStop(SpeechStream); // RA2: as above
	}
	if (clear) { // RA2: as above
		Flush_Speech(); // RA2: as above
	}
}

// RA2: whether no speech waits
static inline bool Speech_Lists_Empty(void) // RA2: as above
{
	if (SpeechQueuedList.prev != &SpeechQueuedList // RA2: as above
			|| SpeechUrgentList.prev != &SpeechUrgentList // RA2: as above
			|| SpeechPending != NULL) { // RA2: as above
		return false; // RA2: as above
	}
	for (int i = 0; i < 4; i++) { // RA2: as above
		ListNode * list = &SpeechLists[i]; // RA2: as above
		if (list->prev != list) { // RA2: as above
			return false; // RA2: as above
		}
	}
	return true; // RA2: as above
}

bool Is_Speaking(void) // RA2: 0x00715E20; services the queue, then whether the stream plays or a speech waits
{
	Speech_Service(); // RA2: as above
	return SpeechStream != 0 // RA2: as above
			&& (AudioStreamerIsPlaying(SpeechStream) || !Speech_Lists_Empty()); // RA2: as above
}

void __fastcall Speech_Cancel(int index) // RA2: 0x00715E80; drops every waiting speech of the VoxClass at an index
{
	if (index >= 0 && index < Voxs.Count()) { // RA2: as above
		VoxSpeech *speech; // RA2: as above
		while ((speech = Speech_Find(Voxs[index])) != 0) { // RA2: as above
			if (speech->prev != speech) { // RA2: as above
				ListNodeRemove(speech); // RA2: as above
			}
			if (speech == SpeechPending) { // RA2: as above
				SpeechPending = 0; // RA2: as above
			}
			speech->Vox->State = 2; // RA2: as above
			delete speech; // RA2: as above
		}
	}
}

void Set_Speech_Volume(int volume) // RA2: 0x00715EF0; the speech volume, at most 255
{
	SpeechVolume = volume; // RA2: as above
	if (volume >= 255) { // RA2: as above
		SpeechVolume = 255; // RA2: as above
	}
}

int Aux_Stream_Init(void) // RA2: 0x00715F10; makes the "Taunt Stream" stream (0 without one)
{
	if (AuxStream == 0) { // RA2: as above
		AuxStream = AudioStreamerCreate(AudioGetDevice(), 3000); // RA2: as above
		if (AuxStream == 0) { // RA2: as above
			return(0); // RA2: as above
		}
	}
	AudioStreamerSetName(AuxStream, "Taunt Stream"); // RA2: as above
	AudioStreamerSetAttribs(SpeechStream, AudioVoiceAttribs); // RA2: as above
	AudioStreamerSetFadeAttribs(SpeechStream, AudioGroupAttribs2); // RA2: as above
	AuxStreamHold = 0; // RA2: as above
	return(1); // RA2: as above
}

void Aux_Stream_Shutdown(void) // RA2: 0x00715F80; stops and destroys the taunt stream
{
	if (AuxStream != 0) { // RA2: as above
		AudioStreamerStop(AuxStream); // RA2: as above
		AudioStreamerDestroy(AuxStream); // RA2: as above
		AuxStream = 0; // RA2: as above
	}
}

int __fastcall Play_Taunt(char taunt) // RA2: 0x00715FB0; plays taunt 1 to 8 (low four bits) of a side (next four bits) from the taunts folder on the taunt stream
{
	char name[20]; // RA2: as above
	if (AuxStream == 0 || AuxStreamHold != 0) { // RA2: as above
		return(0); // RA2: as above
	}
	char number = (char)(taunt & 0xf); // RA2: as above
	if (number < 1 || number > 8) { // RA2: as above
		return(0); // RA2: as above
	}
	switch ((taunt >> 4) & 0xf) { // RA2: as above
	case 0: // RA2: as above
		sprintf(name, "taunts\\tauam%02i.wav", number); // RA2: as above
		break;
	case 1: // RA2: as above
		sprintf(name, "taunts\\tauko%02i.wav", number); // RA2: as above
		break;
	case 2: // RA2: as above
		sprintf(name, "taunts\\taufr%02i.wav", number); // RA2: as above
		break;
	case 3: // RA2: as above
		sprintf(name, "taunts\\tauge%02i.wav", number); // RA2: as above
		break;
	case 4: // RA2: as above
		sprintf(name, "taunts\\taubr%02i.wav", number); // RA2: as above
		break;
	case 5: // RA2: as above
		sprintf(name, "taunts\\tauli%02i.wav", number); // RA2: as above
		break;
	case 6: // RA2: as above
		sprintf(name, "taunts\\tauir%02i.wav", number); // RA2: as above
		break;
	case 7: // RA2: as above
		sprintf(name, "taunts\\taucu%02i.wav", number); // RA2: as above
		break;
	case 8: // RA2: as above
		sprintf(name, "taunts\\tauru%02i.wav", number); // RA2: as above
		break;
	default: // RA2: as above
		return(0); // RA2: as above
	}
	return AudioStreamerPlayWAV(AuxStream, name, 0); // RA2: as above
}

void __fastcall Aux_Stream_Stop(void) // RA2: 0x007160D0; stops the taunt stream
{
	if (AuxStream != 0) { // RA2: as above
		AudioStreamerStop(AuxStream); // RA2: as above
	}
}

VoxClass::VoxClass(char const * name) // RA2: 0x007160E0; a speech of a name at full volume, normal priority, standard type and idle, added to the list
{
	Volume = 1.0f; // RA2: as above
	strcpy(Name, name); // RA2: as above
	Russian[0] = '\0'; // RA2: as above
	Allied[0] = '\0'; // RA2: as above
	Priority = VOXPRIORITY_NORMAL; // RA2: as above
	Type = VOXSPEECH_STANDARD; // RA2: as above
	State = VOXSTATE_IDLE; // RA2: as above
	Voxs.Add(this); // RA2: as above
}

VoxClass::~VoxClass(void) // RA2: 0x00716190; leaves the list
{
	Voxs.Delete(this); // RA2: as above
}

bool VoxClass::Read_INI(CCINIClass &ini) // RA2: 0x007161E0; the volume, type, priority and the two file names of the EVA.ini section; false without the section
{
	char buffer[500]; // RA2: as above
	Volume = 1.0f; // RA2: as above
	if (ini.Is_Present(Name)) { // RA2: as above
		Volume = (float)ini.Get_Float(Name, "Volume", Volume); // RA2: as above
		ini.Get_String(Name, "Type", "", buffer, sizeof(buffer)); // RA2: as above
		if (strlen(buffer) != 0) { // RA2: as above
			if (_stricmp(buffer, "QUEUE") == 0) { // RA2: as above
				Type = 1; // RA2: as above
			} else if (_stricmp(buffer, "STANDARD") == 0) { // RA2: as above
				Type = 0; // RA2: as above
			} else if (_stricmp(buffer, "INTERRUPT") == 0) { // RA2: as above
				Type = 2; // RA2: as above
			} else if (_stricmp(buffer, "QUEUED_INTERRUPT") == 0) { // RA2: as above
				Type = 3; // RA2: as above
			}
		}
		ini.Get_String(Name, "Priority", "", buffer, sizeof(buffer)); // RA2: as above
		if (strlen(buffer) != 0) { // RA2: as above
			if (_stricmp(buffer, "LOW") == 0) { // RA2: as above
				Priority = 0; // RA2: as above
			} else if (_stricmp(buffer, "NORMAL") == 0) { // RA2: as above
				Priority = 1; // RA2: as above
			} else if (_stricmp(buffer, "IMPORTANT") == 0) { // RA2: as above
				Priority = 2; // RA2: as above
			} else if (_stricmp(buffer, "CRITICAL") == 0) { // RA2: as above
				Priority = 3; // RA2: as above
			}
		}
		ini.Get_String(Name, "Russian", "", buffer, sizeof(buffer)); // RA2: as above
		strncpy(Russian, buffer, sizeof(Russian)); // RA2: as above
		Russian[sizeof(Russian) - 1] = '\0'; // RA2: as above
		ini.Get_String(Name, "Allied", "", buffer, sizeof(buffer)); // RA2: as above
		strncpy(Allied, buffer, sizeof(Allied)); // RA2: as above
		Allied[sizeof(Allied) - 1] = '\0'; // RA2: as above
		return(true);
	}
	return(false);
}

void __fastcall VoxClass::Process(CCINIClass &ini) // RA2: 0x007163F0; makes (or finds) a VoxClass for each [DialogList] entry of EVA.ini, which then reads its own section
{
	if (ini.Is_Present("DialogList")) { // RA2: as above
		int count = ini.Entry_Count("DialogList"); // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			char name[200]; // RA2: as above
			if (ini.Get_String("DialogList", ini.Get_Entry("DialogList", index), "", name, sizeof(name))) { // RA2: as above
				VoxClass *vox; // RA2: as above
				int vox_index = Vox_From_Name(name); // RA2: as above
				if (vox_index == -1) { // RA2: as above
					vox = new VoxClass(name); // RA2: as above
				} else {
					vox = Voxs[vox_index]; // RA2: as above
				}
				vox->Read_INI(ini); // RA2: as above
			}
		}
	}
}

void Speech_Free_All(void) // RA2: 0x00716590; stops the speech, drops the waiting ones and deletes every VoxClass
{
	Speech_Stop(true); // RA2: as above
	while (Voxs.Count() > 0) { // RA2: as above
		delete Voxs[0]; // RA2: as above
	}
}

// RA2: the search after the null test is a separate inline step: the original returns -1 for a null name before the search saves esi
static inline int Index_Of_Name(char const * name) // RA2: as above
{
	for (int index = 0; index < Voxs.Count(); index++) { // RA2: as above
		if (_stricmp(name, Voxs[index]->Name) == 0) { // RA2: as above
			return index; // RA2: as above
		}
	}
	return -1; // RA2: as above
}

VoxType __fastcall Vox_From_Name(char const * name) // RA2: 0x00716640; the index of the VoxClass of a name, -1 for none or a null name
{
	if (name == NULL) { // RA2: as above
		return (VoxType)-1; // RA2: as above
	}
	return (VoxType)Index_Of_Name(name); // RA2: as above
}

bool __fastcall Is_Speech_Finished(int voice) // RA2: 0x00716690; true unless the VoxClass at that index is playing or queued
{
	if (voice >= 0 && voice < Voxs.Count()) { // RA2: as above
		return Voxs[voice]->State == VOXSTATE_IDLE; // RA2: as above
	}
	return true; // RA2: as above
}

VoxClass *__fastcall VoxClass::Find(char const *name) // RA2: 0x007166C0; the VoxClass of a name, NULL for a null name, "<none>" or no match
{
	if (name != 0 && _stricmp(name, "<none>") != 0) { // RA2: as above
		for (int index = 0; index < Voxs.Count(); index++) { // RA2: as above
			if (_stricmp(name, Voxs[index]->Name) == 0) { // RA2: as above
				return Voxs[index]; // RA2: as above
			}
		}
	}
	return(0); // RA2: as above
}

char * __fastcall Vox_Name(int index) // RA2: 0x00716720; the name of the VoxClass at an index, "<none>" for -1 or past the end; nothing calls it
{
	if (index != -1 && index < Voxs.Count()) { // RA2: as above
		return Voxs[index]->Name; // RA2: as above
	}
	return "<none>"; // RA2: as above
}

int __fastcall VoxClass::Index_Of(VoxClass *vox) // RA2: 0x00716740; the index of a VoxClass in the list, -1 for none
{
	for (int index = 0; index < Voxs.Count(); index++) { // RA2: as above
		if (Voxs[index] == vox) { // RA2: as above
			return index; // RA2: as above
		}
	}
	return -1; // RA2: as above
}

char * VoxClass::Speech_File(void) // RA2: 0x00716770; the Allied file, or the Soviet one while the Soviet files are on
{
	return SpeechSovietFiles == 0 ? Allied : Russian; // RA2: as above
}

bool Speech_Always_True(void) // RA2: 0x00716780; returns true; nothing calls it
{
	return true; // RA2: as above
}

HRESULT __fastcall Speech_Save(IStream *stream) // RA2: 0x00716790; the markers of the speech queue; no speech is saved (SxoV, ExoV)
{
	int marker = 0x566F7853; // RA2: as above
	HRESULT result = stream->Write(&marker, sizeof(marker), 0); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		marker = 0x566F7845; // RA2: as above
		result = stream->Write(&marker, sizeof(marker), 0); // RA2: as above
	}
	return result; // RA2: as above
}

long __fastcall Speech_Load(IStream *stream) // RA2: 0x007167D0; stops the speech, then queues the speeches a saved game lists between the SxoV and ExoV markers (IxoV, the VoxClass index, type and priority)
{
	Speech_Stop(true); // RA2: as above
	int marker; // RA2: as above
	HRESULT result = stream->Read(&marker, sizeof(marker), 0); // RA2: as above
	if (FAILED(result) || marker != 0x566F7853) { // RA2: as above
		return result; // RA2: as above
	}
	for (result = stream->Read(&marker, sizeof(marker), 0); SUCCEEDED(result); // RA2: as above
			result = stream->Read(&marker, sizeof(marker), 0)) { // RA2: as above
		if (marker == 0x566F7845) { // RA2: as above
			break;
		}
		if (marker != 0x566F7849) { // RA2: as above
			return (HRESULT)0x80004005L; // RA2: as above
		}
		struct { // RA2: as above
			int Index; // RA2: as above
			int Type; // RA2: as above
			int Priority; // RA2: as above
		} entry; // RA2: as above
		result = stream->Read(&entry, sizeof(entry), 0); // RA2: as above
		if (FAILED(result)) { // RA2: as above
			break;
		}
		if (SpeechStream != 0 && entry.Index >= 0 && entry.Index < Voxs.Count()) { // RA2: as above
			Speech_Enqueue(Voxs[entry.Index], entry.Type, entry.Priority); // RA2: as above
		}
	}
	return result; // RA2: as above
}

void __fastcall Speech_Set_Side(int side) // RA2: 0x007168C0; nonzero plays the Soviet speech files
{
	SpeechSovietFiles = side; // RA2: as above
}

void Aux_Stream_Pause(void) // RA2: 0x007168D0
{
	if (AuxStream != 0) { // RA2: as above
		AudioStreamerPause(AuxStream); // RA2: as above
	}
}

void Aux_Stream_Resume(void) // RA2: 0x007168E0
{
	if (AuxStream != 0) { // RA2: as above
		AudioStreamerResume(AuxStream); // RA2: as above
	}
}

void Aux_Stream_Hold(void) // RA2: 0x007168F0; counts the taunt hold up while there is a taunt stream
{
	if (AuxStream != 0) { // RA2: as above
		AuxStreamHold++; // RA2: as above
	}
}

void Aux_Stream_Unblock(void) // RA2: 0x00716910; counts the taunt hold down, not below 0
{
	if (AuxStream != 0) { // RA2: as above
		if (--AuxStreamHold < 0) { // RA2: as above
			AuxStreamHold = 0; // RA2: as above
		}
	}
}

void Add_Speech_Hold(void) // RA2: 0x00716940; counts the speech hold up
{
	SpeechHold++; // RA2: as above
}

void Remove_Speech_Hold(void) // RA2: 0x00716950; counts the speech hold down, not below 0
{
	if (SpeechHold != 0) { // RA2: as above
		if (--SpeechHold < 0) { // RA2: as above
			SpeechHold = 0; // RA2: as above
		}
	}
}

void Theme_Suspend(void) // RA2: 0x00716980; pauses the speech stream and keeps speeches from starting
{
	if (SpeechStream != 0) { // RA2: as above
		AudioStreamerPause(SpeechStream); // RA2: as above
	}
	SpeechSuspend++; // RA2: as above
}

void Speech_Reset(void) // RA2: 0x007169A0; stops the speech, drops the waiting ones and clears both holds
{
	Speech_Stop(true); // RA2: as above
	SpeechSuspend = 0; // RA2: as above
	SpeechHold = 0; // RA2: as above
}

void Theme_Resume(void) // RA2: 0x007169F0; resumes the speech stream and counts the suspension down, not below 0
{
	if (SpeechStream != 0) { // RA2: as above
		AudioStreamerResume(SpeechStream); // RA2: as above
	}
	if (SpeechSuspend != 0) { // RA2: as above
		if (--SpeechSuspend < 0) { // RA2: as above
			SpeechSuspend = 0; // RA2: as above
		}
	}
}

// RA2: assembled by edit_vox.py
