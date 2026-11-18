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

#include "geozero.h"
#include "geoconst1.h" // RA2: this module holds its own copy of the geometry constants
#include "voc.h"

#include "_rect.h"
#include "_tactica.h"
#include "audlist.h" // RA2: the static sounds are in a list of the audio library
#include "audmem.h" // RA2: and in a pool of it
#include "ccini.h"
#include "cell.h" // RA2: the includes this file needs
#include "globals.h"
#include "geoconst2.h" // RA2: as above; CELL_NONE
#include "map.h" // RA2: the includes this file needs
#include "tactical.h"
#include "vector.h"
#include <objidl.h> // RA2: the static sounds are saved to a stream
#include <stdlib.h> // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs


DynamicVectorClass<VocClass *> Vocs; // RA2: 0x00AD0818

/*
**	RA2: the settings a sound effect that does not name its own starts from, read from the
**	[Defaults] section of the sound rules.
*/
int DefaultPriority = 2; // RA2: 0x007F9A6C
float DefaultVolume = 80.0f; // RA2: 0x007F9A70; percent
float DefaultMinVolume = 20.0f; // RA2: 0x007F9A74; percent
unsigned int DefaultType = 0x20; // RA2: 0x007F9A78
int DefaultRange = 10; // RA2: 0x007F9A7C; cells
int DefaultLimit = 5; // RA2: 0x007F9A80
unsigned int DefaultControl; // RA2: 0x00AD0850

/*
**	RA2: a sound effect kept playing at a location, in the list of static sounds.
*/
struct VocStaticSound : ListNode // RA2: as above
{
	AudioController Controller; // RA2: +0x0C
	Coord Position; // RA2: +0x20
	int Flag; // RA2: +0x2C; the flags Stop_At matches
	volatile int PauseCount; // RA2: +0x30; volatile because Sound_Resume_All (0x7144F0) and Static_Sounds_Load (0x714B80) read it again after every write (YR declares it so too)
	int Unused[2]; // RA2: the node is 0x3C bytes, the size the pool is made for

	void Pause(void); // RA2: 0x00714460
	void Resume(void); // RA2: 0x00714480
};

ListNode StaticSounds; // RA2: 0x00AD07D0
AudioMemoryPool * StaticSoundPool; // RA2: 0x00AD0844
int StaticSoundCount; // RA2: 0x00AD0848
int StaticSoundMaximum; // RA2: 0x00AD084C


/// <summary>
/// Creates a sound effect for the sample name specified.
/// The new sound adds itself to the master sound list and fetches its sample from the
/// mixfiles right away. The priority and volume ratings stay at their defaults until the
/// rules are read.
/// </summary>
/// <param name="filename">The root name of the sound sample, without any extension.</param>
VocClass::VocClass(const char *filename) :
	Event(NULL) // RA2: the sound effect is only the audio library's event type
{
	Vocs.Add(this);

	Event = AudioEventTypeFind(filename); // RA2: the audio library keeps the settings and samples
}


/// <summary>
/// Removes this sound effect from the master sound list.
/// </summary>
VocClass::~VocClass(void)
{
	Vocs.Delete(this);
}


/// <summary>
/// Fetches this sound effect's settings from the rules.
/// This routine will look for a section named after the sound and take its priority and
/// volume from there. A sound with no section of its own falls back to the defaults. The
/// sample itself is retrieved from the mixfiles either way.
/// </summary>
/// <param name="ini">The rules database to fetch the settings from.</param>
/// <returns>bool; Did the sound have a section of its own?</returns>
bool VocClass::Fill_In(CCINIClass &ini) // RA2: the rules are read through a non-const reference
{
	char buffer[0x800]; // RA2: the line buffer for the sample list and the keyword lists
	char * token; // RA2: as above

	if (Event == NULL || !ini.Is_Present(Name())) { // RA2: no event type or no section
		return(false);
	}

	ini.Get_String(Name(), "Sounds", "", buffer, sizeof(buffer)); // RA2: the samples, split at white space
	for (token = strtok(buffer, " \t\n"); token != NULL; token = strtok(NULL, " \t\n")) { // RA2: as above
		AudioEventTypeAddSample(Event, token); // RA2: as above
	}

	float volume = (float)ini.Get_Float(Name(), "Volume", DefaultVolume) * 0.01f; // RA2: percent, kept within 0 to 1
	if (volume > 1.0) { // RA2: as above
		volume = 1.0f; // RA2: as above
	} else if (volume <= 0) { // RA2: as above
		volume = 0.0f; // RA2: as above
	}
	AudioEventTypeSetVolume(Event, (int)(volume * 16384.0f)); // RA2: 16384 is full volume

	AudioEventTypeSetVShift(Event, ini.Get_Int(Name(), "VShift", 0)); // RA2: as above

	float minvolume = (float)ini.Get_Float(Name(), "MinVolume", DefaultMinVolume) * 0.01f; // RA2: as above
	if (minvolume > 1.0) { // RA2: as above
		minvolume = 1.0f; // RA2: as above
	} else if (minvolume <= 0) { // RA2: as above
		minvolume = 0.0f; // RA2: as above
	}
	AudioEventTypeSetMinVolume(Event, minvolume); // RA2: as above

	ini.Get_String(Name(), "Priority", "NORMAL", buffer, sizeof(buffer)); // RA2: a priority name
	int priority = DefaultPriority; // RA2: as above
	AudioPriorityFromName(buffer, &priority); // RA2: as above
	AudioEventTypeSetPriority(Event, priority); // RA2: as above

	AudioEventTypeSetAttack(Event, ini.Get_Int(Name(), "Attack", 0)); // RA2: as above
	AudioEventTypeSetDecay(Event, ini.Get_Int(Name(), "Decay", 0)); // RA2: as above

	ini.Get_String(Name(), "Control", "", buffer, sizeof(buffer)); // RA2: keywords, the defaults when there are none
	token = strtok(buffer, " \t\n"); // RA2: as above
	unsigned int control = DefaultControl; // RA2: as above
	if (token != NULL) { // RA2: as above
		control = 0; // RA2: as above
		do { // RA2: as above
			AudioControlApplyKeyword(&control, token); // RA2: as above
			token = strtok(NULL, " \t\n"); // RA2: as above
		} while (token != NULL); // RA2: as above
	}
	AudioEventTypeSetControl(Event, control); // RA2: as above

	ini.Get_String(Name(), "Type", "", buffer, sizeof(buffer)); // RA2: as above, starting from 0x20
	token = strtok(buffer, " \t\n"); // RA2: as above
	unsigned int type = DefaultType; // RA2: as above
	if (token != NULL) { // RA2: as above
		type = 0x20; // RA2: as above
		do { // RA2: as above
			AudioTypeApplyKeyword(&type, token); // RA2: as above
			token = strtok(NULL, " \t\n"); // RA2: as above
		} while (token != NULL); // RA2: as above
	}
	AudioEventTypeSetType(Event, type); // RA2: as above

	AudioEventTypeSetLimit(Event, ini.Get_Int(Name(), "Limit", DefaultLimit)); // RA2: as above
	AudioEventTypeSetLoop(Event, ini.Get_Int(Name(), "Loop", 0)); // RA2: as above
	AudioEventTypeSetRange(Event, ini.Get_Int(Name(), "Range", DefaultRange)); // RA2: as above

	ini.Get_String(Name(), "Delay", "", buffer, sizeof(buffer)); // RA2: a low and a high value, the high one defaulting to the low one
	token = strtok(buffer, " \t\n"); // RA2: as above
	int low = 0; // RA2: as above
	if (token != NULL) { // RA2: as above
		low = atoi(token); // RA2: as above
		token = strtok(NULL, " \t\n"); // RA2: as above
	}
	int high = low; // RA2: as above
	if (token != NULL) { // RA2: as above
		high = atoi(token); // RA2: as above
	}
	AudioEventTypeSetDelay(Event, low, high); // RA2: as above

	ini.Get_String(Name(), "FShift", "", buffer, sizeof(buffer)); // RA2: as above
	token = strtok(buffer, " \t\n"); // RA2: as above
	low = 0; // RA2: as above
	if (token != NULL) { // RA2: as above
		low = atoi(token); // RA2: as above
		token = strtok(NULL, " \t\n"); // RA2: as above
	}
	high = low; // RA2: as above
	if (token != NULL) { // RA2: as above
		high = atoi(token); // RA2: as above
	}
	AudioEventTypeSetFShift(Event, low, high); // RA2: as above

	return(true);
}


/// <summary>
/// Plays this sound effect at the volume specified.
/// The volume requested is scaled by both this sound's own volume rating and the player's
/// sound effect option setting, so the sound falls silent when the effects are turned off.
/// </summary>
/// <param name="vol">The volume to play at, where 1.0 is this sound's own full volume.</param>
/// <param name="var">The variation number for sound effects that have variations.</param>
/// <returns>Returns with the sound handle, or -1 if no sound was played.</returns>
/// <param name="voc">The sound effect number to play.</param>
/// <param name="volume">The volume to play at, from 0 to 1.</param>
/// <param name="panning">The panning, 0x2000 being the centre.</param>
/// <param name="controller">The optional handle that keeps the event.</param>
/// <returns>Returns with the event, or NULL if no sound was played.</returns>
void * VocClass::PlayGlobal(int voc, float volume, int panning, AudioController * controller) // RA2: the sound effect's event is started and kept in the controller
{
	AudioEventType * type = NULL; // RA2: as above
	if (voc >= 0 && voc < Vocs.Count()) { // RA2: as above
		type = Vocs[voc]->Event; // RA2: as above
	}

	AudioEvent * event = NULL; // RA2: as above
	if (controller != NULL) { // RA2: as above
		event = AudioControllerGetEvent(controller); // RA2: as above
		if (event != NULL && AudioEventGetType(event) != type) { // RA2: as above
			AudioEventStop(event); // RA2: as above
			event = NULL; // RA2: as above
		}
	}

	if (type != NULL) { // RA2: as above
		unsigned int level = (unsigned int)(volume * 16384.0f); // RA2: 16384 is full volume
		if (level >= 0x4000) { // RA2: as above
			level = 0x4000; // RA2: as above
		}
		if (event != NULL || (event = AudioEventCreate(type)) != NULL) { // RA2: as above
			AudioEventSetVolume(event, level); // RA2: as above
			AudioEventSetPan(event, panning); // RA2: as above
		}
	}

	if (controller != NULL) { // RA2: as above
		controller->SetEvent(event, (VocClass *)type); // RA2: the controller's type member is declared as a VocClass pointer
	}
	return(event); // RA2: as above
}


/// <summary>
/// Plays this sound effect at the volume specified.
/// This routine is used by the voice sound effects, where the caller has already worked
/// out the final volume and the sound effect option setting does not apply.
/// </summary>
/// <param name="vol">The volume to play at, where 1.0 is this sound's own full volume.</param>
/// <returns>Returns with the sound handle, or -1 if no sound was played.</returns>
/// <param name="voc">The sound effect number to play.</param>
/// <param name="coord">The world location the sound comes from.</param>
/// <param name="controller">The optional handle that keeps the event.</param>
/// <returns>Returns with the event, or NULL if no sound is playing.</returns>
void * VocClass::PlayAt(int voc, Coord const & coord, AudioController * controller) // RA2: as above
{
	AudioEvent * event = NULL; // RA2: as above
	AudioEventType * type = NULL; // RA2: as above
	if (voc >= 0 && voc < Vocs.Count()) { // RA2: as above
		type = Vocs[voc]->Event; // RA2: as above
	}

	if (controller != NULL) { // RA2: as above
		event = AudioControllerGetEvent(controller); // RA2: as above
		if (event != NULL && AudioEventGetType(event) != type) { // RA2: as above
			AudioEventStop(event); // RA2: as above
			event = NULL; // RA2: as above
		}
	}

/***********************************************************************************************
 * Sound_Effect -- General purpose sound player.                                               *
 *                                                                                             *
 *    This is used for general purpose sound effects. These are sounds that occur outside      *
 *    of the game world. They do not have a corresponding game world location as their source. *
 *                                                                                             *
 * INPUT:   voc      -- The sound effect number to play.                                       *
 *                                                                                             *
 *          volume   -- The volume to assign to this sound effect.                             *
 *                                                                                             *
 *          variation   -- This is the optional variation number to use when playing special   *
 *                         sound effects that have variations. For normal sound effects, this  *
 *                         parameter is ignored.                                               *
 *                                                                                             *
 *          house -- This specifies the optional house override value to use when playing      *
 *                   sound effects that have a variation. If not specified, then the current   *
 *                   player is examined for the house variation to use.                        *
 *                                                                                             *
 * OUTPUT:  Returns with the sound handle (-1 if no sound was played).                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   11/12/1994 JLB : Handles cache logic.                                                     *
 *   05/04/1995 JLB : Variation adjustments.                                                   *
 *   11/01/1996 JLB : House override control.                                                  *
 *=============================================================================================*/
	if (type != NULL) { // RA2: as above
		int panning; // RA2: as above
		float volume = Volume_At(coord, &panning, type); // RA2: as above
		if (volume > 0) { // RA2: as above
			unsigned int level = (unsigned int)(volume * 16384.0f); // RA2: as above
			if (level >= 0x4000) { // RA2: as above
				level = 0x4000; // RA2: as above
			}
			if (event != NULL || (event = AudioEventCreate(type)) != NULL) { // RA2: as above
				AudioEventSetVolume(event, level); // RA2: as above
				AudioEventSetPan(event, panning); // RA2: as above
			}
		} else if (event != NULL) { // RA2: as above
			AudioEventStop(event); // RA2: as above
			event = NULL; // RA2: as above
		}
	}

	if (controller != NULL) { // RA2: as above
		controller->SetEvent(event, (VocClass *)type); // RA2: as above
	}
	return(event); // RA2: as above
}


/// <summary>
/// Plays a voice sound effect.
/// This routine is used for the spoken responses and announcements. Unlike a general
/// purpose sound effect, the volume is not scaled by the sound effect option setting.
/// </summary>
/// <param name="voc">The sound effect to play.</param>
/// <param name="volume">The volume to assign to this sound effect.</param>
/// <returns>Returns with the sound handle, or -1 if no sound was played.</returns>
/// <param name="coord">The world location the sound comes from.</param>
/// <param name="panning">Receives the panning, from 0 to 0x4000.</param>
/// <param name="type">The audio library's event type of the sound.</param>
/// <returns>Returns with the volume from 0 to 1, or 0 if the sound is not to be heard.</returns>
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (adopted text)
float __fastcall Volume_At(Coord const & coord, int * panning, AudioEventType * type) // RA2: a free function the sound effect routines share
{
	*panning = 0;
	float halfwidth = TacticalRect.Width * 0.5f; // RA2: as above
	float halfheight = TacticalRect.Height * 0.5f; // RA2: as above
	float volume = 0; // RA2: as above
	float width = halfwidth + halfwidth; // RA2: as above
	float range = AudioEventTypeRange(type) * 60; // RA2: the range in cells, 60 pixels each
	unsigned int flags = AudioEventTypeGetType(type); // RA2: as above
	if (flags & 0x800) { // RA2: sounds that only play where the player has seen
		Cell cell = Cell(coord.X / 256, coord.Y / 256); // RA2: as above
		if (cell == CELL_NONE) { // RA2: as above
			return(0); // RA2: as above
		}
		CellClass & cellptr = Map[cell]; // RA2: as above
		if (!cellptr.IsMapped && !cellptr.IsVisible) { // RA2: as above
			return(0); // RA2: as above
		}
	}

	Point2D pixel; // RA2: as above
	TacticalMap->Coord_To_Pixel(coord, pixel); // RA2: as above
	float dx = pixel.X - halfwidth; // RA2: as above
	float x = abs((int)dx); // RA2: as above
	float y = abs((int)(pixel.Y - halfheight)); // RA2: as above
	if (!(flags & 0x40)) { // RA2: as above
		x -= halfwidth; // RA2: as above
		y -= halfheight; // RA2: as above
		if (x < 0) { // RA2: as above
			x = 0; // RA2: as above
		}
		if (y < 0) { // RA2: as above
			y = 0; // RA2: as above
		}
	}
	y = y + y; // RA2: as above
	if (x < range && y < range && range > 0) { // RA2: as above
		float distance = x > y ? x : y; // RA2: as above
		volume = (range - distance) / range; // RA2: as above
	}
	if (flags & 0x10) { // RA2: as above
		if (AudioEventTypeMinVolume(type) > volume) { // RA2: as above
			volume = AudioEventTypeMinVolume(type); // RA2: as above
		}
	}
	if (volume < 0.05) { // RA2: as above
		return(0); // RA2: as above
	}

	if (dx < -width) { // RA2: as above
		dx = -width; // RA2: as above
	} else if (dx > width) { // RA2: as above
		dx = width; // RA2: as above
	}
	*panning = (int)(dx * 8192.0f / width + 8192.0f);
	return(volume); // RA2: as above
}
#endif

/***********************************************************************************************
 * Sound_Effect -- Plays a sound effect in the tactical map.                                   *
 *                                                                                             *
 *    This routine is used when a sound effect occurs in the game world. It handles fading     *
 *    the sound according to distance.                                                         *
 *                                                                                             *
 * INPUT:   voc   -- The sound effect number to play.                                          *
 *                                                                                             *
 *          coord -- The world location that the sound originates from.                        *
 *                                                                                             *
 *          variation   -- This is the optional variation number to use when playing special   *
 *                         sound effects that have variations. For normal sound effects, this  *
 *                         parameter is ignored.                                               *
 *                                                                                             *
 *          house -- This specifies the optional house override value to use when playing      *
 *                   sound effects that have a variation. If not specified, then the current   *
 *                   player is examined for the house variation to use.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Reduces sound more dramatically when off screen.                         *
 *   09/15/1996 JLB : Revamped volume logic.                                                   *
 *   11/01/1996 JLB : House override control.                                                  *
 *=============================================================================================*/

/// <summary>
/// Updates the sound a controller keeps for a location in the world.
/// The volume and the panning are worked out again for the location. A sound that has gone
/// quiet is stopped, and a sound that loops forever is started again when it can be heard.
/// </summary>
/// <param name="coord">The world location the sound comes from.</param>
/// <param name="controller">The handle that keeps the event.</param>
/// <returns>Returns with the event, or NULL if no sound is playing.</returns>
void * VocClass::UpdateAt(Coord const & coord, AudioController * controller) // RA2: updates a kept event for a location
{
	AudioEvent * event = NULL; // RA2: as above
	if (controller != NULL) { // RA2: as above
		event = AudioControllerGetEvent(controller); // RA2: as above
		AudioEventType * type = AudioControllerGetType(controller); // RA2: as above
		if (type != NULL) { // RA2: as above
			int panning; // RA2: as above
			float volume = Volume_At(coord, &panning, type); // RA2: as above
			if (volume > 0) { // RA2: as above
				if (event == NULL && AudioEventTypeLoopsForever(type)) { // RA2: as above
					event = AudioEventCreate(type); // RA2: as above
					if (event != NULL) { // RA2: as above
						AudioEventSetFlag8(event); // RA2: as above
					}
				}
				if (event != NULL) { // RA2: as above
					unsigned int level = (unsigned int)(volume * 16384.0f); // RA2: as above
					if (level >= 0x4000) { // RA2: as above
						level = 0x4000; // RA2: as above
					}
					AudioEventSetVolume(event, level); // RA2: as above
					AudioEventSetPan(event, panning); // RA2: as above
				}
			} else if (event != NULL) { // RA2: as above
				AudioEventStop(event); // RA2: as above
				event = NULL; // RA2: as above
			}
		}
		controller->SetEvent(event, (VocClass *)type); // RA2: as above
	}
	return(event); // RA2: as above
}


/// <summary>
/// Starts a static sound, a sound effect that keeps playing at a location in the world.
/// The sound is kept in the list of static sounds with the flags it was started with, so
/// that Stop_At can end it later.
/// </summary>
/// <param name="voc">The sound effect number to play.</param>
/// <param name="coord">The world location the sound comes from.</param>
/// <param name="flag">The flags to find the sound again by.</param>
/// <returns>Returns with the event, or NULL if no sound was started.</returns>
void * VocClass::PlayIndexAtPos(int voc, Coord const & coord, int flag) // RA2: static sounds are kept in a list
{
	if (voc < 0) { // RA2: as above
		return(NULL);
	}

	VocStaticSound * sound = (VocStaticSound *)MemoryPoolGetItem(StaticSoundPool); // RA2: as above
	if (sound == NULL) { // RA2: as above
		return(NULL);
	}

	sound->Controller.Init(); // RA2: as above
	ListNodeInit(sound); // RA2: as above
	sound->PauseCount = 0; // RA2: as above
	sound->Position = coord; // RA2: as above
	sound->Flag = flag; // RA2: as above
	void * event = PlayAt(voc, sound->Position, &sound->Controller); // RA2: as above
	ListNodeInsert(&StaticSounds, sound); // RA2: as above
	StaticSoundCount++; // RA2: as above
	if (StaticSoundCount > StaticSoundMaximum) { // RA2: as above
		StaticSoundMaximum = StaticSoundCount; // RA2: as above
	}
	return(event); // RA2: as above
}


/// <summary>
/// Updates every static sound that is not paused and drops the ones whose event has ended.
/// A sound that loops forever is kept, so that it can start again when it comes into hearing.
/// </summary>
void Sound_Service(void) // RA2: services the static sounds
{
	VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); // RA2: as above
	while (sound != NULL) { // RA2: as above
		VocStaticSound * next = (VocStaticSound *)ListNextItem(sound); // RA2: as above
		if (sound->PauseCount == 0) { // RA2: as above
			VocClass::UpdateAt(sound->Position, &sound->Controller); // RA2: as above
		}
		if (AudioControllerGetEvent(&sound->Controller) == NULL) { // RA2: as above
			AudioEventType * type = AudioControllerGetType(&sound->Controller); // RA2: as above
			if (type == NULL || !AudioEventTypeLoopsForever(type)) { // RA2: as above
				ListNodeRemove(sound); // RA2: as above
				MemoryPoolReturnItem(StaticSoundPool, sound); // RA2: as above
				StaticSoundCount--; // RA2: as above
			}
		}
		sound = next; // RA2: as above
	}
}


/// <summary>
/// Lets the static sounds at a location end.
/// Only the sounds that were started with any of the flags are affected.
/// </summary>
/// <param name="coord">The world location the sounds come from.</param>
/// <param name="flag">The flags the sounds were started with.</param>
void VocClass::Stop_At(Coord const & coord, int flag) // RA2: ends the static sounds that match
{
	for (VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); sound != NULL; sound = (VocStaticSound *)ListNextItem(sound)) { // RA2: as above
		if ((sound->Flag & flag) && sound->Position.X == coord.X && sound->Position.Y == coord.Y && sound->Position.Z == coord.Z) { // RA2: as above
			sound->Controller.End(); // RA2: as above
		}
	}
}


/// <summary>
/// Stops every static sound and drops it from the list.
/// </summary>
void Sound_Clear_Static(void) // RA2: empties the static sound list
{
	VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); // RA2: as above
	while (sound != NULL) { // RA2: as above
		VocStaticSound * next = (VocStaticSound *)ListNextItem(sound); // RA2: as above
		sound->Controller.Stop(); // RA2: as above
		ListNodeRemove(sound); // RA2: as above
		MemoryPoolReturnItem(StaticSoundPool, sound); // RA2: as above
		StaticSoundCount--; // RA2: as above
		sound = next; // RA2: as above
	}
}


/// <summary>
/// Counts one more pause and pauses the sound's event.
/// </summary>
void VocStaticSound::Pause(void) // RA2: a static sound that can be paused
{
	PauseCount++; // RA2: as above
	AudioEvent * event = AudioControllerGetEvent(&Controller); // RA2: as above
	if (event != NULL) { // RA2: as above
		AudioEventPause(event); // RA2: as above
	}
}


/// <summary>
/// Counts one pause less, and resumes the sound's event when it was paused.
/// </summary>
void VocStaticSound::Resume(void) // RA2: as above
{
	if (PauseCount != 0) { // RA2: as above
		if (--PauseCount < 0) { // RA2: as above
			PauseCount = 0; // RA2: as above
		}
		AudioEvent * event = AudioControllerGetEvent(&Controller); // RA2: as above
		if (event != NULL) { // RA2: as above
			AudioEventResume(event); // RA2: as above
		}
	}
}


/// <summary>
/// Pauses every static sound.
/// </summary>
void Sound_Pause_All(void) // RA2: pauses the static sounds
{
	for (VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); sound != NULL; sound = (VocStaticSound *)ListNextItem(sound)) { // RA2: as above
		sound->Pause(); // RA2: as above
	}
}


/// <summary>
/// Resumes every static sound.
/// </summary>
void Sound_Resume_All(void) // RA2: resumes the static sounds
{
	for (VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); sound != NULL; sound = (VocStaticSound *)ListNextItem(sound)) { // RA2: as above
		sound->Resume(); // RA2: as above
	}
}


/// <summary>
/// Creates the master sound effect list from the rules.
/// This routine will fetch every sound named in the sound list section, creating a sound
/// effect for any that does not exist yet, and then let each one fill itself in from its
/// own section.
/// </summary>
/// <param name="ini">The rules database to fetch the sound list from.</param>
void Init_Vocs(CCINIClass &ini) // RA2: the rules are read through a non-const reference, and the defaults come with them
{
	char const * const SECTION = "SoundList";

	ListInit(&StaticSounds); // RA2: the static sounds start out empty, in a pool of 100
	StaticSoundPool = MemoryPoolCreate(100, sizeof(VocStaticSound)); // RA2: as above
	StaticSoundCount = 0; // RA2: as above
	StaticSoundMaximum = 0; // RA2: as above

	char buffer[0x800]; // RA2: as above
	char * token; // RA2: as above
	if (ini.Is_Present("Defaults")) { // RA2: as above
		DefaultVolume = (float)ini.Get_Float("Defaults", "Volume", DefaultVolume); // RA2: as above
		DefaultMinVolume = (float)ini.Get_Float("Defaults", "MinVolume", DefaultMinVolume); // RA2: as above
		ini.Get_String("Defaults", "Priority", "NORMAL", buffer, sizeof(buffer)); // RA2: as above
		AudioPriorityFromName(buffer, &DefaultPriority); // RA2: as above

		ini.Get_String("Defaults", "Control", "", buffer, sizeof(buffer)); // RA2: as above
		token = strtok(buffer, " \t\n"); // RA2: as above
		if (token != NULL) { // RA2: as above
			DefaultControl = 0; // RA2: as above
			do { // RA2: as above
				AudioControlApplyKeyword(&DefaultControl, token); // RA2: as above
				token = strtok(NULL, " \t\n"); // RA2: as above
			} while (token != NULL); // RA2: as above
		}

		ini.Get_String("Defaults", "Type", "", buffer, sizeof(buffer)); // RA2: as above
		token = strtok(buffer, " \t\n"); // RA2: as above
		if (token != NULL) { // RA2: as above
			DefaultType = 0x20; // RA2: as above
			do { // RA2: as above
				AudioTypeApplyKeyword(&DefaultType, token); // RA2: as above
				token = strtok(NULL, " \t\n"); // RA2: as above
			} while (token != NULL); // RA2: as above
		}

		DefaultLimit = ini.Get_Int("Defaults", "Limit", DefaultLimit); // RA2: as above
		DefaultRange = ini.Get_Int("Defaults", "Range", DefaultRange); // RA2: as above
	}

	if (ini.Is_Present(SECTION)) {
		int count = ini.Entry_Count(SECTION);
		for (int i = 0; i < count; i++) {
			char name[32];
			if (ini.Get_String(SECTION, ini.Get_Entry(SECTION, i), "", name, sizeof(name)) != 0) {

				VocClass *voc = NULL;
				VocType type = VocClass::From_Name(name);
				if (type == VOC_NONE) {
					voc = new VocClass(name);
				} else {
					voc = Vocs[type];
				}
				voc->Fill_In(ini);
			}
		}
	}
}


/// <summary>
/// Destroys every sound effect in the master sound list.
/// This routine is used when shutting the game down or before the sound list is rebuilt
/// from a fresh set of rules.
/// </summary>
void Free_Vocs(void)
{
	while (Vocs.Count() > 0) {
		VocClass *voc = Vocs[0];
		delete voc;
	}

	if (StaticSoundPool != NULL) { // RA2: the static sounds go with the sound effects
		Sound_Clear_Static(); // RA2: as above
		MemoryPoolDestroy(StaticSoundPool); // RA2: as above
		StaticSoundPool = NULL; // RA2: as above
	}
}


/***********************************************************************************************
 * Voc_From_Name -- Fetch VocType from ASCII name specified.                                   *
 *                                                                                             *
 *    This will find the corresponding VocType from the ASCII string specified. It does this   *
 *    by finding a root filename that matches the string.                                      *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII string that will be converted into a VocType.        *
 *                                                                                             *
 * OUTPUT:  Returns with the VocType that matches the string specified. If no match could be   *
 *          found, then VOC_NONE is returned.                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VocType VocClass::From_Name(char const * name)
{
	if (name != NULL) { // RA2: the name test wraps the loop
		for (int voc = VOC_FIRST; voc < Vocs.Count(); voc++) { // RA2: counts with an int
			if (stricmp(name, Vocs[voc]->Name()) == 0) { // RA2: the name comes from the event type
				return(VocType(voc)); // RA2: as above
			}
		}
	}

	return(VOC_NONE);
}


/// <summary>
/// Fetches the sound effect that matches the ASCII name specified.
/// This routine is used when reading sound assignments out of the rules, where a sound is
/// named by the root of its filename. The placeholder "none" name is recognized as meaning
/// no sound at all.
/// </summary>
/// <param name="name">Pointer to the ASCII name of the sound effect to find.</param>
/// <returns>Returns with a pointer to the matching sound effect. Otherwise, NULL is
/// returned.</returns>
VocClass * VocClass_From_Name(char const * name)
{
	if (name != NULL && strcmpi(name, "<none>") != 0) { // RA2: one test for both cases
		for (int voc = VOC_FIRST; voc < Vocs.Count(); voc++) { // RA2: counts with an int
			if (stricmp(name, Vocs[voc]->Name()) == 0) { // RA2: the name comes from the event type
				return(Vocs[voc]);
			}
		}
	}

	return(NULL);
}


/***********************************************************************************************
 * Voc_Name -- Fetches the name for the sound effect.                                          *
 *                                                                                             *
 *    This routine returns the descriptive name of the sound effect. Currently, this is just   *
 *    the root of the file name.                                                               *
 *                                                                                             *
 * INPUT:   voc   -- The VocType that the corresponding name is requested.                     *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the text string the represents the sound effect.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
char const * Voc_Name(VocType voc)
{
	if (voc >= 0 && voc < Vocs.Count()) { // RA2: any index in the list, "none" is -1
		if (Vocs[voc]->Event != NULL) { // RA2: a sound effect without an event type has no name
			return(AudioEventTypeName(Vocs[voc]->Event)); // RA2: as above
		}
		return("Invalid Voc"); // RA2: as above
	}
	return("<none>");
}


/// <summary>
/// Fetches the sound effect number of this sound.
/// This is the inverse of the master sound list lookup -- it recovers the VocType that the
/// rest of the game uses to refer to this sound.
/// </summary>
/// <returns>Returns with the VocType of this sound, or VOC_NONE if it is not
/// registered.</returns>
VocType VocClass::Voc_Type(void)
{
	for (int index = 0; index < Vocs.Count(); index++) {
		if (Vocs[index] == this) {
			return(VocType)(index);
		}
	}
	return(VOC_NONE);
}


/// <summary>
/// Can this sound effect be played?
/// A sound whose sample could not be found in the mixfiles is silent, as is every sound
/// while the game is running in quiet mode.
/// </summary>
/// <returns>bool; Is this sound effect available to be played?</returns>
bool VocClass::Has_Event(void) const // RA2: a test of the event type
{
	return(Event != NULL); // RA2: as above
}


/// <summary>
/// Fetches the name of a sound effect's event type.
/// </summary>
/// <param name="voc">The sound effect.</param>
/// <returns>Returns with the name of the event type, or "Invalid Voc" if there is none.</returns>
char const * __fastcall Voc_Event_Name(VocClass const * voc) // RA2: 0x00714A70; the same test VocClass::Name makes, out of line
{
	if (voc->Event != NULL) { // RA2: as above
		return(AudioEventTypeName(voc->Event)); // RA2: as above
	}
	return("Invalid Voc"); // RA2: as above
}


/// <summary>
/// Saves the static sounds: how many there are, then the sound effect number and the location
/// of each in the order of the list.
/// </summary>
/// <param name="stream">The stream to save to.</param>
HRESULT __fastcall Static_Sounds_Save(IStream * stream) // RA2: the static sounds are saved with the game
{
	int count = ListCountItems(&StaticSounds); // RA2: as above
	HRESULT result = stream->Write(&count, sizeof(count), 0); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		VocStaticSound * sound = (VocStaticSound *)ListFirstItem(&StaticSounds); // RA2: as above
		while (sound) { // RA2: as above
			struct { // RA2: as above
				int Index; // RA2: as above
				int X; // RA2: the location, as three words
				int Y; // RA2: as above
				int Z; // RA2: as above
			} entry; // RA2: as above
			int index = -1; // RA2: as above
			entry.Index = index; // RA2: as above
			if (sound->Controller.EventType) { // RA2: as above
				char const * name = AudioEventTypeName((AudioEventType *)sound->Controller.EventType); // RA2: as above
				if (name) { // RA2: as above
					index = VocClass::From_Name(name); // RA2: as above
				}
				entry.Index = index; // RA2: as above
			}
			entry.X = sound->Position.X; // RA2: as above
			entry.Y = sound->Position.Y; // RA2: as above
			entry.Z = sound->Position.Z; // RA2: as above
			result = stream->Write(&entry, sizeof(entry), 0); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			sound = (VocStaticSound *)ListNextItem(sound); // RA2: as above
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Loads the static sounds. Every sound is dropped first, then each saved sound whose sound
/// effect loops forever is started again at its location.
/// </summary>
/// <param name="stream">The stream to load from.</param>
long __fastcall Static_Sounds_Load(IStream * stream) // RA2: the static sounds are loaded with the game
{
	Sound_Clear_Static(); // RA2: as above
	Audio_Stop_Events(); // RA2: as above
	int count; // RA2: as above
	HRESULT result = stream->Read(&count, sizeof(count), 0); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			struct { // RA2: as above
				int Index; // RA2: as above
				int X; // RA2: the location, as three words
				int Y; // RA2: as above
				int Z; // RA2: as above
			} entry; // RA2: as above
			result = stream->Read(&entry, sizeof(entry), 0); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			if (entry.Index != -1 && entry.Index < Vocs.Count() && entry.Index >= 0) { // RA2: as above
				VocClass * voc = Vocs[entry.Index]; // RA2: as above
				if (voc->Event != NULL && AudioEventTypeLoopsForever(voc->Event)) { // RA2: as above
					Coord position; // RA2: as above
					position.X = entry.X; // RA2: as above
					position.Y = entry.Y; // RA2: as above
					position.Z = entry.Z; // RA2: as above
					VocClass::PlayIndexAtPos(entry.Index, position, 0); // RA2: as above
				}
			}
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Points a controller at the event type of a sound effect, without an event.
/// </summary>
/// <param name="controller">The controller to point.</param>
/// <param name="voc">The sound effect number.</param>
void __fastcall Controller_Set_Voc(AudioController * controller, int voc) // RA2: sets only the type of the controller
{
	if (voc >= 0 && voc < Vocs.Count()) { // RA2: as above
		controller->SetEvent(NULL, (VocClass *)Vocs[voc]->Event); // RA2: as above
	}
}
