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

#pragma once

#include "coord.h"

#include "voc.hh"

struct AudioController;
class CCINIClass;
class VocClass;
class AudioEventType; // RA2: the audio library's event type

void Init_Vocs(CCINIClass &ini); // RA2: 0x00714540; reads the rules through a non-const reference
void Free_Vocs(void); // RA2: 0x00714860

int Sound_Effect(VocType voc, float volume=1, int=0); // RA2: no such routine; callers play through VocClass::PlayGlobal
int Sound_Effect(VocType voc, Coord const & coord);
VocClass * VocClass_From_Name(char const * name);
char const * Voc_Name(VocType voc); // RA2: 0x00714A00
void Sound_Service(void); // RA2: 0x00714330; updates the static sounds
void Sound_Clear_Static(void); // RA2: 0x00714410; stops and drops every static sound
void Sound_Pause_All(void); // RA2: 0x007144B0
void Sound_Resume_All(void); // RA2: 0x007144F0
float __fastcall Volume_At(Coord const & coord, int * panning, AudioEventType * type); // RA2: 0x00713F30; the volume (0 to 1) and panning of a sound at a location
void __fastcall Controller_Set_Voc(AudioController * controller, int voc); // RA2: 0x00714D10; points a controller at a sound effect's event type

extern int StaticSoundCount; // RA2: 0x00AD0848; the static sounds playing now (the audio events dump prints it)
extern int StaticSoundMaximum; // RA2: 0x00AD084C; the most static sounds played at once


/***************************************************************************
**	Controls what special effects may occur on the sound effect.
*/
enum ContextType {
	IN_NOVAR,			// No variation or alterations allowed.
	IN_VAR				// Infantry variance response modification.
};
// RA2: VocClass is only the audio library event type

/***************************************************************************
**	RA2: the audio library's event types and events (0x00405110-0x00406880), which the sound effects
**	are made of. The library is not reconstructed yet; what the sound effect routines call is listed here.
**	An event type holds the samples and the settings of one sound effect.
*/
class AudioEvent;
class AudioEventType;
AudioEventType * __fastcall AudioEventTypeFind(char const * name); // RA2: 0x004063C0; the event type of this name, made with the defaults when there is none
char const * __fastcall AudioEventTypeName(AudioEventType * type); // RA2: 0x00405150
int __fastcall AudioEventTypeRange(AudioEventType * type); // RA2: 0x00405110; +0x50, in cells
float __fastcall AudioEventTypeMinVolume(AudioEventType * type); // RA2: 0x00405120; +0x54
unsigned int __fastcall AudioEventTypeGetType(AudioEventType * type); // RA2: 0x00405140; +0x14, the "Type" flags
int __fastcall AudioEventTypeAddSample(AudioEventType * type, char const * name); // RA2: 0x004064B0
void __fastcall AudioEventTypeSetAttack(AudioEventType * type, int count); // RA2: 0x00406530; +0x138
void __fastcall AudioEventTypeSetDecay(AudioEventType * type, int count); // RA2: 0x00406540; +0x13C
void __fastcall AudioEventTypeSetPriority(AudioEventType * type, int priority); // RA2: 0x00406550; +0x40
void __fastcall AudioEventTypeSetVolume(AudioEventType * type, int volume); // RA2: 0x00406560
void __fastcall AudioEventTypeSetControl(AudioEventType * type, unsigned int flags); // RA2: 0x00406580
void __fastcall AudioEventTypeSetType(AudioEventType * type, unsigned int flags); // RA2: 0x004065D0; +0x14
void __fastcall AudioEventTypeSetLimit(AudioEventType * type, int limit); // RA2: 0x004065E0; +0x48
void __fastcall AudioEventTypeSetRange(AudioEventType * type, int range); // RA2: 0x004065F0; +0x50
void __fastcall AudioEventTypeSetMinVolume(AudioEventType * type, float volume); // RA2: 0x00406600; +0x54
void __fastcall AudioEventTypeSetDelay(AudioEventType * type, int low, int high); // RA2: 0x00406610; +0x58, +0x5C
void __fastcall AudioEventTypeSetFShift(AudioEventType * type, int low, int high); // RA2: 0x00406620; +0x60, +0x64
void __fastcall AudioEventTypeSetVShift(AudioEventType * type, int percent); // RA2: 0x00406630; +0x68
void __fastcall AudioEventTypeSetLoop(AudioEventType * type, int count); // RA2: 0x00406650; +0x4C
int __fastcall AudioEventTypeLoopsForever(AudioEventType * type); // RA2: 0x00406660
bool __fastcall AudioPriorityFromName(char const * name, int * priority); // RA2: 0x004067E0
bool __fastcall AudioControlApplyKeyword(unsigned int * flags, char const * name); // RA2: 0x00406830
int __fastcall AudioTypeApplyKeyword(unsigned int * flags, char const * name); // RA2: 0x00406880
AudioEvent * __fastcall AudioEventCreate(AudioEventType * type); // RA2: 0x00405170
void __fastcall AudioEventStop(AudioEvent * event); // RA2: 0x00405300
void __fastcall AudioEventPause(AudioEvent * event); // RA2: 0x004053F0
void __fastcall AudioEventResume(AudioEvent * event); // RA2: 0x00405440
void __fastcall AudioEventSetFlag8(AudioEvent * event); // RA2: 0x004052F0; sets bit 3 of the flags
void __fastcall AudioEventSetVolume(AudioEvent * event, int volume); // RA2: 0x004061E0
void __fastcall AudioEventSetPan(AudioEvent * event, int pan); // RA2: 0x00406280
AudioEventType * __fastcall AudioEventGetType(AudioEvent * event); // RA2: 0x00406320; +0x24
AudioEvent * __fastcall AudioControllerGetEvent(AudioController * controller); // RA2: 0x00406140; the event while the controller is still good
AudioEventType * __fastcall AudioControllerGetType(AudioController * controller); // RA2: 0x00406180
void Audio_Stop_Events(void); // RA2: 0x00406DC0

class VocClass
{
	public:
		VocClass(const char *filename);
		~VocClass(void);

		bool Fill_In(CCINIClass &ini); // RA2: 0x007138D0; the settings go to the event type
		bool Has_Event(void) const; // RA2: 0x00714A60

		VocType Voc_Type(void); // RA2: 0x00714A30
		char const * Name(void) const { return Event != NULL ? AudioEventTypeName(Event) : "Invalid Voc"; } // RA2: the name of the event type

		static VocType From_Name(char const * name);
		static void * PlayGlobal(int voc, float volume, int panning, AudioController * controller = NULL); // RA2: 0x00713DB0
		static void * PlayAt(int voc, Coord const & coord, AudioController * controller = NULL); // RA2: 0x00713E60
		static void * UpdateAt(Coord const & coord, AudioController * controller); // RA2: 0x007141B0; updates the kept event for a location
		static void * PlayIndexAtPos(int voc, Coord const & coord, int flag); // RA2: 0x714290
		static void Stop_At(Coord const & coord, int flag); // RA2: 0x7143B0, stops the static sounds at a spot
		friend VocClass *VocClass_From_Name(char const * name);
		friend char const * Voc_Name(VocType voc);

	public:
		AudioEventType * Event; // RA2: +0x00; the audio library's event type holds the samples and the settings
};
