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

#include "data.h"
#include "language\language.h"
#include "wdtgameoptions.h"
#include "wstring.h" // RA2: the includes this file needs

using namespace WorldDominationTour;


/// <summary>
/// Creates a counted conflict over the specified territory.
/// A conflict is the campaign's record of a battle still to be fought. The territory is
/// what supplies the map and the game options when the player finally commits to it.
/// </summary>
/// <param name="terr">The territory to be fought over. May be empty, which leaves the
/// conflict as a placeholder with no battle attached to it.</param>
/// <returns>Returns with the new conflict.</returns>
rc_ptr<Conflict> __fastcall Conflict::Create(rc_ptr<WDTTerritory> const & terr) // RA2: a counted conflict made over a counted territory (0x007272F0)
{
	return(rc_ptr<Conflict>(new Conflict(terr))); // RA2: as above
}


/// <summary>
/// Creates a conflict over the specified territory.
/// </summary>
/// <param name="terr">The territory to be fought over. May be empty, which leaves the
/// conflict as a placeholder with no battle attached to it.</param>
Conflict::Conflict(rc_ptr<WDTTerritory> const & terr) : // RA2: counted territory (0x00727360)
	Territory(terr)
{
	/// nothing
}


/// <summary>
/// Determines if two conflicts are the same battle.
/// Two conflicts are considered equal when they are over the same territory.
/// </summary>
/// <returns>bool; Are both conflicts over the same territory?</returns>
bool Conflict::operator==(Conflict const & that) const // RA2: 0x007273A0
{
	return(Territory == that.Territory);
}


/// <summary>
/// Destroys the conflict record.
/// The territory itself belongs to the campaign map and outlives every conflict that
/// refers to it, so there is nothing here to clean up.
/// </summary>
Conflict::~Conflict(void) // RA2: 0x007273C0
{
	/// nothing
}


/// <summary>
/// Has this conflict been given a territory to be fought over?
/// A default constructed conflict is a placeholder with nowhere to fight, and the campaign
/// uses this test to tell the two apart.
/// </summary>
/// <returns>bool; Is there a territory to fight over?</returns>
bool Conflict::Is_Territory_Set(void) const // RA2: 0x007273E0
{
	return(Territory.Get() != NULL); // RA2: counted pointer
}


/// <summary>
/// Fetches the map index of the territory being fought over.
/// </summary>
/// <returns>Returns with the territory index, or zero if no territory has been set.</returns>
int Conflict::Get_Territory_Index(void) const // RA2: 0x007273F0
{
	if (Territory.Get() != NULL) { // RA2: counted pointer
		return(Territory->Index); // RA2: the number of the territory
	}
	return(0);
}


/// <summary>
/// The shared list of game options that a territory is described by.
/// Init_Game_Options fills it on demand.
/// </summary>
WDT_GAME_OPTION_LIST WorldDominationTour::WDTGameOptionList; // RA2: 0x00B256C0, the options are counted objects


/// <summary>
/// Creates the list of game options that a territory can be described by.
/// Each option knows how to pull one rule out of a territory and phrase it for the player.
/// Describe_Options builds the list on demand, so any options surviving from an earlier
/// conflict are thrown away first.
/// </summary>
void __fastcall Conflict::Init_Game_Options(void) // RA2: 0x00727480, builds the shared list of battle options
{
	/// nothing
	Deinit_Game_Options(); // RA2: starts from an empty list
	WDTGameOptionList.Add(rc_ptr<GameOption>(ValueGameOption<unsigned char>::Create(0x10000, WDT_Get_Tech_Level, Wstring("WDT:TechLevel"), 0))); // RA2: the options are counted objects of the executable's own classes
	WDTGameOptionList.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x10, Wstring("WDT:NoBases"), 0, Wstring("WDT:Bases"), 1))); // RA2: as above
	WDTGameOptionList.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x400, Wstring("WDT:NotShortGame"), 1, Wstring("WDT:ShortGame"), 0))); // RA2: as above
	WDTGameOptionList.Add(rc_ptr<GameOption>(ValueGameOption<unsigned char>::Create(0x8000, WDT_Get_Unit_Count, Wstring("WDT:UnitCount"), 0))); // RA2: as above
	WDTGameOptionList.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x20, Wstring("WDT:NoRedeployMCV"), 1, Wstring("WDT:RedeployableMCV"), 0))); // RA2: as above
	WDTGameOptionList.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x100, Wstring("WDT:NoCrates"), 1, Wstring("WDT:Crates"), 1))); // RA2: as above
	WDTGameOptionList.Add(MapSizeGameOption::Create(0xC000000)); // RA2: as above
}


/// <summary>
/// Destroys the game option list.
/// Use this routine to release the option objects that Init_Game_Options created.
/// </summary>
void Conflict::Deinit_Game_Options(void) // RA2: 0x00727920, empties the shared list of battle options
{
	WDTGameOptionList.Clear(); // RA2: as above
}


/// <summary>
/// Builds the game option summary text for this conflict.
/// This routine is used by the World Domination Tour screens to tell the player what kind
/// of battle he is about to pick. Each option contributes one comma separated phrase, and
/// the summary is deliberately brief, so the options flagged as most worth knowing are
/// described first and the merely user modifiable ones are left out altogether.
/// </summary>
/// <returns>Returns with the text describing up to six of the conflict's options.</returns>
WideWstring Conflict::Describe_Options(void) const // RA2: 0x00727960, the text of up to six options: first those worth calling out, then the rest that are not user modifiable
{
	if (WDTGameOptionList.Count() == 0) { // RA2: as above
		Init_Game_Options();
	}
	int left = 6;
	WideWstring text; // RA2: as above
	unsigned int used = 0; // RA2: as above
	WDT_GAME_OPTION_ITER it(WDTGameOptionList); // RA2: as above
	for (; left != 0 && it; it++) { // RA2: as above
		rc_ptr<GameOption> opt = *it; // RA2: as above
		if ((opt->Bitmask & used) == 0 && opt->Get_Display_Priority(Territory) == WDT_GAME_OPT_PRIORITY) { // RA2: as above
			text = opt->Get_String(Territory, text); // RA2: as above
			left--;
			used |= opt->Bitmask; // RA2: as above
		}
	}
	it.Cursor = 0; // RA2: the second pass starts over from the first option, the vector stays
	for (; left != 0 && it; it++) { // RA2: as above
		rc_ptr<GameOption> opt = *it; // RA2: as above
		if ((opt->Bitmask & used) == 0 && opt->Get_Display_Priority(Territory) != WDT_GAME_OPT_USER_MOD) { // RA2: as above
			text = opt->Get_String(Territory, text); // RA2: as above
			left--;
			used |= opt->Bitmask; // RA2: as above
		}
	}
	return(text); // RA2: as above
}


/// <summary>
/// Fetches the number of units each player starts a battle over the territory with.
/// This is the getter that the unit count game option reads its value through.
/// </summary>
/// <param name="territory">The territory to read.</param>
/// <returns>Returns with the territory's starting unit count.</returns>
int __fastcall WDT_Get_Unit_Count(WDTTerritory const * territory) // RA2: 0x00727B70, the getter of the WDT:UnitCount option
{
	return(territory->UnitCount); // RA2: as above
}


/// <summary>
/// Fetches the tech level a battle over the territory is fought at.
/// This is the getter that the tech level game option reads its value through.
/// </summary>
/// <param name="territory">The territory to read.</param>
/// <returns>Returns with the territory's tech level.</returns>
int __fastcall WDT_Get_Tech_Level(WDTTerritory const * territory) // RA2: 0x00727B80, the getter of the WDT:TechLevel option
{
	return(territory->TechLevel); // RA2: as above
}
