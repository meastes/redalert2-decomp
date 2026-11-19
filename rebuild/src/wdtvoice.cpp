/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "ccrand.h" // RA2: NonCriticalRandomNumber
#include "audplay.h" // RA2: AudioStreamerDestroy
#include "auddsnd.h" // RA2: AudioGetDevice
#include "ini.h"
#include "vox.h" // RA2: the speech queue
#include "wdtnet.h"
#include "wstring.h"

using namespace WorldDominationTour;

Wstring operator+(Wstring const & a, Wstring const & b); // RA2: 0x0076F7D0; the free join of two strings (wstring.cpp defines it, wstring.h has only the member), which the exe calls here


/// <summary>
/// RA2: constructs the voice player that speaks for one faction.
/// The six groups are named after the INI keys their lines are read under.
/// </summary>
Voices::Voices(int faction) : // RA2: constructs the voice player that speaks for one faction
	Faction(faction), // RA2: as above
	Idle(true), // RA2: as above
	Pending(0), // RA2: as above
	Stream(NULL) // RA2: as above
{
	AudioGetDevice(); // RA2: the exe fetches the audio device here and does nothing with it
	Groups[0].Set_Name("OldCycle"); // RA2: the group names
	Groups[1].Set_Name("History"); // RA2: as above
	Groups[2].Set_Name("Status"); // RA2: as above
	Groups[3].Set_Name("Territory"); // RA2: as above
	Groups[4].Set_Name("Game"); // RA2: as above
	Groups[5].Set_Name("Evacuate"); // RA2: as above
}


/// <summary>
/// RA2: destroys the voice player; whatever is queued is stopped and the audio stream is freed.
/// </summary>
Voices::~Voices(void) // RA2: destroys the voice player; whatever is queued is stopped and the audio stream is freed
{
	Stop(); // RA2: expanded here
	if (Stream != NULL) { // RA2: as above
		AudioStreamerDestroy((AudioStreamer *)Stream); // RA2: as above
	}
}


/// <summary>
/// RA2: reads every group's Win, Lose and Draw lines and the two stand-alone lists.
/// </summary>
void Voices::Load(INIClass & ini, Wstring const & section) // RA2: reads every group's Win, Lose and Draw lines and the two stand-alone lists
{
	for (int group = 0; group < VOICECAT_COUNT; group++) { // RA2: as above
		Groups[group].Load(ini, section, Faction); // RA2: as above
	}
	{
		Wstring key("Startup"); // RA2: as above
		Load_List(ini, section, 0, key); // RA2: as above
	}
	{
		Wstring key("TerritorySelect"); // RA2: as above
		Load_List(ini, section, 1, key); // RA2: as above
	}
}


/// <summary>
/// RA2: reads one comma separated list of line numbers, named after the faction and the key.
/// </summary>
void Voices::Load_List(INIClass & ini, Wstring const & section, int index, Wstring const & key) // RA2: reads one comma separated list of line numbers, named after the faction and the key
{
	Wstring entry = operator+(Faction_Name(Faction), key); // RA2: the free join
	Wstring value; // RA2: as above
	{
		Wstring empty(""); // RA2: as above
		ini.Get_String(section, entry, empty, value); // RA2: as above
	}
	Parse_List(value, Lists[index]); // RA2: as above
}


/// <summary>
/// RA2: picks the speech name of an ordinary line of a group for a result.
/// </summary>
bool Voices::Pick(int group, int kind, Wstring & name) const // RA2: picks the speech name of an ordinary line of a group for a result
{
	return(Groups[group].Pick(Faction, kind, name)); // RA2: as above
}


/// <summary>
/// RA2: picks the speech name of an emphatic line of a group for a result.
/// </summary>
bool Voices::Pick_Emphasis(int group, int kind, Wstring & name) const // RA2: picks the speech name of an emphatic line of a group for a result
{
	return(Groups[group].Pick_Emphasis(Faction, kind, name)); // RA2: as above
}


/// <summary>
/// RA2: picks the speech name of a line of one of the stand-alone lists.
/// </summary>
bool Voices::Pick_List(int index, Wstring & name) const // RA2: picks the speech name of a line of one of the stand-alone lists
{
	return(Pick(Faction, Lists[index], name)); // RA2: as above
}


/// <summary>
/// RA2: picks a line of a list at random and makes its speech name; an empty list gives nothing.
/// </summary>
bool __fastcall Voices::Pick(int faction, DynamicVectorClass<unsigned int> const & lines, Wstring & name) // RA2: picks a line of a list at random and makes its speech name; an empty list gives nothing
{
	name.clear(); // RA2: as above
	if (lines.Count() <= 0) { // RA2: as above
		return(false); // RA2: as above
	}
	int index = lines.Count() == 1 ? 0 : NonCriticalRandomNumber(0, lines.Count() - 1); // RA2: as above
	return(Make_Name(faction, lines[index], name)); // RA2: as above
}


/// <summary>
/// RA2: the speech name of a line, "WWD-Soviet" for faction 3 and "WWD_Allied" for the others, with the number.
/// </summary>
bool __fastcall Voices::Make_Name(int faction, int line, Wstring & name) // RA2: the speech name of a line, "WWD-Soviet" for faction 3 and "WWD_Allied" for the others, with the number
{
	if (faction == 3) { // RA2: as above
		name = "WWD-Soviet" + Wstring_FromInt(line, 2); // RA2: as above
	} else { // RA2: as above
		name = "WWD_Allied" + Wstring_FromInt(line, 2); // RA2: as above
	}
	return(true); // RA2: as above
}


/// <summary>
/// RA2: replaces a list with the comma separated numbers of a text.
/// </summary>
void __fastcall Voices::Parse_List(Wstring const & text, DynamicVectorClass<unsigned int> & lines) // RA2: replaces a list with the comma separated numbers of a text
{
	lines.Clear(); // RA2: as above
	Wstring token; // RA2: as above
	int position = text.getToken(0, ",", token); // RA2: as above
	while (position != -1) { // RA2: as above
		lines.Add(token.toInt()); // RA2: as above
		position = text.getToken(position, ",", token); // RA2: as above
	}
}


/// <summary>
/// RA2: a group starts with an empty name.
/// </summary>
Voices::Group::Group(void) // RA2: a group starts with an empty name
{
	Name = ""; // RA2: as above
}


/// <summary>
/// RA2: names the group after the INI key its lines are read under.
/// </summary>
void Voices::Group::Set_Name(Wstring const & name) // RA2: names the group after the INI key its lines are read under
{
	Name = name; // RA2: as above
}


/// <summary>
/// RA2: reads the lines of all three results of this group.
/// </summary>
void Voices::Group::Load(INIClass & ini, Wstring const & section, int faction) // RA2: reads the lines of all three results of this group
{
	{
		Wstring key("Win"); // RA2: as above
		Load(ini, section, faction, 0, key); // RA2: as above
	}
	{
		Wstring key("Lose"); // RA2: as above
		Load(ini, section, faction, 1, key); // RA2: as above
	}
	{
		Wstring key("Draw"); // RA2: as above
		Load(ini, section, faction, 2, key); // RA2: as above
	}
}


/// <summary>
/// RA2: picks an ordinary line for a result.
/// </summary>
bool Voices::Group::Pick(int faction, int kind, Wstring & name) const // RA2: picks an ordinary line for a result
{
	return(Voices::Pick(faction, Lines[kind * 2], name)); // RA2: as above
}


/// <summary>
/// RA2: picks an emphatic line for a result.
/// </summary>
bool Voices::Group::Pick_Emphasis(int faction, int kind, Wstring & name) const // RA2: picks an emphatic line for a result
{
	return(Voices::Pick(faction, Lines[kind * 2 + 1], name)); // RA2: as above
}


/// <summary>
/// RA2: reads the plain and the emphatic lines of one result of this group (the entry is <faction><group><key>[Emphasis]).
/// </summary>
void Voices::Group::Load(INIClass & ini, Wstring const & section, int faction, int kind, Wstring const & key) // RA2: reads the plain and the emphatic lines of one result of this group (the entry is <faction><group><key>[Emphasis])
{
	Wstring entry = operator+(Faction_Name(faction), Name).operator+(key); // RA2: the free join of faction and group name, then the member join of the key
	Wstring value; // RA2: as above
	{
		Wstring empty(""); // RA2: as above
		ini.Get_String(section, entry, empty, value); // RA2: as above
	}
	Voices::Parse_List(value, Lines[kind * 2]); // RA2: as above
	entry += "Emphasis"; // RA2: as above
	{
		Wstring empty(""); // RA2: as above
		ini.Get_String(section, entry, empty, value); // RA2: as above
	}
	Voices::Parse_List(value, Lines[kind * 2 + 1]); // RA2: as above
}


/// <summary>
/// RA2: queues the speech of a result of a group, and with emphasis a second line behind it.
/// </summary>
void Voices::Say(int group, int kind, bool emphasis, bool counted, int delay) // RA2: queues the speech of a result of a group, and with emphasis a second line behind it
{
	Trim(); // RA2: as above
	Wstring name; // RA2: as above
	if (Pick(group, kind, name)) { // RA2: as above
		if (!Is_Speaking()) { // RA2: as above
			Timer = delay; // RA2: as above
		}
		Queue.Add(name); // RA2: as above
		if (counted) { // RA2: as above
			Pending++; // RA2: as above
		}
		if (emphasis && Pick_Emphasis(group, kind, name)) { // RA2: as above
			Queue.Add(name); // RA2: as above
			if (counted) { // RA2: as above
				Pending++; // RA2: as above
			}
		}
	}
}


/// <summary>
/// RA2: queues a line of one of the stand-alone lists (the startup list is 0).
/// </summary>
void Voices::Say_List(int index, bool counted, int delay) // RA2: queues a line of one of the stand-alone lists (the startup list is 0)
{
	Trim(); // RA2: as above
	Wstring name; // RA2: as above
	if (Pick_List(index, name)) { // RA2: as above
		if (!Is_Speaking()) { // RA2: as above
			Timer = delay; // RA2: as above
		}
		Queue.Add(name); // RA2: as above
		if (counted) { // RA2: as above
			Pending++; // RA2: as above
		}
	}
}


/// <summary>
/// RA2: once the delay has run out and nothing is playing, drops the line just said and plays the next one.
/// </summary>
void Voices::Update(void) // RA2: once the delay has run out and nothing is playing, drops the line just said and plays the next one
{
	if (Timer() == 0 && Queue.Count() > 0 && !Is_Speaking()) { // RA2: as above
		if (!Idle) { // RA2: as above
			Queue.Delete_Index(0); // RA2: as above
			if (Pending > 0) { // RA2: as above
				Pending--; // RA2: as above
			}
		}
		if (Queue.Count() > 0) { // RA2: as above
			Queue_Speech(Vox_From_Name(Queue[0].get()), -1, -1); // RA2: as above
			Idle = false; // RA2: as above
		} else { // RA2: as above
			Pending = 0; // RA2: as above
			Idle = true; // RA2: as above
		}
	}
}


/// <summary>
/// RA2: drops the queued lines past the counted ones.
/// </summary>
void Voices::Trim(void) // RA2: drops the queued lines past the counted ones
{
	int index = Queue.Count(); // RA2: as above
	while (index-- > Pending) { // RA2: as above
		Queue.Delete_Index(index); // RA2: as above
	}
	if (Pending == 0) { // RA2: as above
		Idle = true; // RA2: as above
	}
}


/// <summary>
/// RA2: an empty function at 0x007329D0.
/// </summary>
void Voices_Noop1(void) // RA2: an empty function at 0x007329D0
{
}


/// <summary>
/// RA2: an empty function at 0x007329E0.
/// </summary>
void Voices_Noop2(void) // RA2: an empty function at 0x007329E0
{
}


/// <summary>
/// RA2: stops the speech and forgets everything queued.
/// </summary>
void Voices::Stop(void) // RA2: stops the speech and forgets everything queued
{
	if (Queue.Count() > 0) { // RA2: as above
		Speech_Stop(true); // RA2: as above
		Queue.Clear(); // RA2: as above
	}
	Timer = 0; // RA2: as above
}


/// <summary>
/// RA2: is the speech stream busy?
/// </summary>
bool Voices::Is_Speaking(void) // RA2: is the speech stream busy?
{
	return(::Is_Speaking()); // RA2: as above
}


/// <summary>
/// RA2: a new animation that plays this player's queue.
/// </summary>
Voices::Anim * Voices::Create_Anim(void) // RA2: a new animation that plays this player's queue
{
	return(new Anim(this)); // RA2: as above
}


Voices::Anim::Anim(Voices * voices) : // RA2: a new animation that plays this player's queue
	MSAnim(0, 0, false), // RA2: the animation starts inactive
	VoicePlayer(voices) // RA2: as above
{
}


Voices::Anim::~Anim(void) // RA2: a new animation that plays this player's queue
{
}


/// <summary>
/// RA2: switching the animation off stops the voices.
/// </summary>
void Voices::Anim::Set_Active(bool active) // RA2: switching the animation off stops the voices
{
	Active = active; // RA2: as above
	if (!Active) { // RA2: as above
		VoicePlayer->Stop(); // RA2: as above
	}
}


void Voices::Anim::Pause(void) // RA2: switching the animation off stops the voices
{
	Timer.Stop(); // RA2: as above
}


void Voices::Anim::Resume(void) // RA2: switching the animation off stops the voices
{
	Timer.Start(); // RA2: as above
}


/// <summary>
/// RA2: services the queue; the animation never asks to be deleted.
/// </summary>
bool Voices::Anim::Advance(Surface * surface, Rect & rect) // RA2: services the queue; the animation never asks to be deleted
{
	VoicePlayer->Update(); // RA2: as above
	return(false); // RA2: as above
}


void Voices::Anim::Redraw(Surface * surface, Rect const * rect) // RA2: services the queue; the animation never asks to be deleted
{
	VoicePlayer->Update(); // RA2: as above
}


/// <summary>
/// RA2: an animation that is heard, not seen, takes no screen area.
/// </summary>
Rect Voices::Anim::Get_Rect(void) const // RA2: an animation that is heard, not seen, takes no screen area
{
	return(Rect()); // RA2: as above
}


/// <summary>
/// RA2: finished once nothing is being said.
/// </summary>
bool Voices::Anim::Has_Finished(void) const // RA2: finished once nothing is being said
{
	return(!::Is_Speaking()); // RA2: as above
}


void Voices::Anim::Restore(Rect const & rect) // RA2: finished once nothing is being said
{
	VoicePlayer->Update(); // RA2: as above
}
