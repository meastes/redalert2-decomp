/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "rect.h"
#include "geoconst1.h" // RA2: this unit holds its own copy of the geometry constants (its first initializers)

#include "data.h"
#include "globals.h" // RA2: the includes this file needs
#include "language\language.h"
#include "wdtnet.h"
#include "wstring.h" // RA2: as above
#include "geoconst2.h" // RA2: the slope and bridge constants (after globals.h)

using namespace WorldDominationTour;

Wstring __fastcall operator+(Wstring const & left, Wstring const & right); // RA2: 0x0076F7D0, the label and the band number joined
WideWstring __fastcall operator+(WideWstring const & left, WideWstring const & right); // RA2: 0x00771460, the text so far and the separator joined


/// <summary>
/// Destructor for game option objects.
/// The options are counted descriptors that own nothing of their own; this routine exists
/// so that the derived options can be destroyed through a base pointer.
/// </summary>
GameOption::~GameOption(void)
{
	//nothing
}


/// <summary>
/// Can the player change this option for the territory?
/// A territory locks down some of the game options and leaves the rest for the player to
/// set as he pleases before the battle begins.
/// </summary>
/// <returns>bool; Is this option left to the player's discretion?</returns>
bool GameOption::Is_User_Modifiable(rc_ptr<WDTTerritory> const & territory) const // RA2: the territory is a counted pointer and no option is ever left to the player (0x00728680)
{
	return(false); // RA2: as above
}


/// <summary>
/// Appends the wording for this option's band to the text so far.
/// The option owns a label whose text is completed by the number of the band, so the
/// description reads as low or high rather than as a raw number.
/// </summary>
/// <param name="territory">The territory the option is described for.</param>
/// <param name="prefix">The text built up so far.</param>
/// <returns>Returns with the text so far and the wording of the band.</returns>
WideWstring RangedGameOption::Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const // RA2: returns the text so far with the label of the band number appended (0x00728690); the label is completed by the number, loaded as a counted wide text
{
	int range = Get_Range(territory); // RA2: as above
	Wstring label = operator+(Label, Wstring_FromInt(range)); // RA2: as above
	if (prefix.length() != 0) { // RA2: as above
		WideWstring separator(L", "); // RA2: as above
		return(operator+(prefix, separator) + StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 22)); // RA2: as above
	}
	return(WideWstring(StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 25))); // RA2: as above
}


/// <summary>
/// Determines which band of this option's range the value falls into.
/// The bands run from very low to very high and are what the descriptive wording and the
/// display priority are both chosen from.
/// </summary>
/// <param name="value">The setting to classify.</param>
/// <returns>Returns with the band the value belongs to.</returns>
int RangedGameOption::In_Range_Of(unsigned int value) const // RA2: const (0x00728770)
{
	if (value <= Range0) {
		return(WDT_GAME_OPT_RANGE_VERY_LOW);
	}
	if (value <= Range1) {
		return(WDT_GAME_OPT_RANGE_LOW);
	}
	if (value >= Range3) {
		return(WDT_GAME_OPT_RANGE_VERY_HIGH);
	}
	if (value >= Range2) {
		return(WDT_GAME_OPT_RANGE_HIGH);
	}
	return(WDT_GAME_OPT_RANGE_MIDDLE);
}


/// <summary>
/// Determines how prominently this ranged option should be listed for the territory.
/// A setting sitting in the middle of its range is unremarkable and is left out of the
/// first pass.
/// </summary>
/// <returns>Returns with the display priority this option has earned.</returns>
int RangedGameOption::Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const // RA2: no option is left to the player, so only the middle band is worth skipping (0x007287B0)
{
	if (Get_Range(territory) == WDT_GAME_OPT_RANGE_MIDDLE) { // RA2: the band comes from the option's own Get_Range
		return(WDT_GAME_OPT_USER_MOD);
	}
	return(WDT_GAME_OPT_PRIORITY);
}


/// <summary>
/// Creates a counted flag option.
/// The option names the bit of the territory's flags it reads, the wording and the display
/// priority of the flag being off, and the wording and the display priority of it being on.
/// </summary>
/// <returns>Returns with the new option.</returns>
rc_ptr<FlagGameOption> __fastcall FlagGameOption::Create(unsigned int bitmask, Wstring off_string, int off_value, Wstring on_string, int on_value) // RA2: a counted option made from the bit, the two labels and the two priorities (0x007287D0)
{
	return(rc_ptr<FlagGameOption>(new FlagGameOption(bitmask, off_string, off_value, on_string, on_value))); // RA2: as above
}


/// <summary>
/// Determines how prominently this flag should be listed for the territory.
/// The option carries its own priority for each of its two states, so a rule that is
/// switched on can be shown more urgently than one that is switched off.
/// </summary>
/// <returns>Returns with the display priority this option has earned.</returns>
int FlagGameOption::Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const // RA2: no option is left to the player, so the flag's state alone decides (0x00728850)
{
	if (territory->Booleans & Bitmask) {
		return(OnValue);
	}
	return(OffValue);
}


/// <summary>
/// Appends the on or off wording of this flag to the text so far.
/// A flag whose current state has no wording assigned to it leaves the text as it is.
/// </summary>
/// <param name="territory">The territory the option is described for.</param>
/// <param name="prefix">The text built up so far.</param>
/// <returns>Returns with the text so far and the wording of the flag's state.</returns>
WideWstring FlagGameOption::Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const // RA2: returns the text so far with the label of the flag's state appended (0x00728870); a state without a label leaves the text as it is
{
	Wstring label = (territory->Booleans & Bitmask) ? OnString : OffString; // RA2: as above
	if (label.length() == 0) { // RA2: as above
		return(prefix); // RA2: as above
	}
	if (prefix.length() > 0) { // RA2: as above
		WideWstring separator(L", "); // RA2: as above
		return(operator+(prefix, separator) + StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 78)); // RA2: as above
	}
	return(WideWstring(StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 81))); // RA2: as above
}


/// <summary>
/// Determines how prominently the player count should be listed for this territory.
/// </summary>
/// <returns>Returns with the display priority this option has earned.</returns>
int NumberOfPlayersGameOption::Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const // RA2: no option is left to the player (0x00728970)
{
	return(WDT_GAME_OPT_PRIORITY);
}


/// <summary>
/// Appends the player count restriction of the territory to the text so far.
/// </summary>
/// <param name="territory">The territory the option is described for.</param>
/// <param name="prefix">The text built up so far.</param>
/// <returns>Returns with the text so far and the wording of the restriction.</returns>
WideWstring NumberOfPlayersGameOption::Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const // RA2: returns the text so far with the label of the two or four player restriction appended (0x00728980)
{
	Wstring label(territory->NumPlayers == 4 ? "WDT:FourPlayerOnly" : "WDT:TwoPlayerOnly"); // RA2: the labels of the string table
	if (prefix.length() > 0) { // RA2: as above
		WideWstring separator(L", "); // RA2: as above
		return(operator+(prefix, separator) + StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 104)); // RA2: as above
	}
	return(WideWstring(StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTGameOptions.cpp", 107))); // RA2: as above
}


/// <summary>
/// Creates a counted map size option.
/// </summary>
/// <returns>Returns with the new option.</returns>
rc_ptr<MapSizeGameOption> __fastcall MapSizeGameOption::Create(unsigned int bitmask) // RA2: a counted option made from the bit (0x00728A60)
{
	return(rc_ptr<MapSizeGameOption>(new MapSizeGameOption(bitmask))); // RA2: as above
}


/// <summary>
/// Appends a description of the territory's map size to the text so far.
/// This routine is used while building the summary of a territory so that the player can
/// see at a glance what shape of battlefield is being fought over.
/// </summary>
/// <param name="territory">The territory the option is described for.</param>
/// <param name="prefix">The text built up so far.</param>
/// <returns>Returns with the text so far and the wording of the map size.</returns>
WideWstring MapSizeGameOption::Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const // RA2: returns the text so far with the label of the map size appended (0x00728AB0); one label per size step, read for the width after a comma and for the height otherwise
{
	static char const * const _labels[4] = { "WDT:SmallMap", "WDT:MediumMap", "WDT:LargeMap", "WDT:VeryLargeMap" }; // RA2: the labels of the string table (0x007AF140)
	if (prefix.length() > 0) { // RA2: as above
		return(operator+(prefix, L", ") + StringTable::LoadString(_labels[territory->Width], NULL, "D:\\Ra2\\WDTGameOptions.cpp", 129)); // RA2: the separator is made from the text where the executable joins the two
	}
	return(WideWstring(StringTable::LoadString(_labels[territory->Height], NULL, "D:\\Ra2\\WDTGameOptions.cpp", 131))); // RA2: as above
}


/// <summary>
/// Determines how prominently the map size should be listed for this territory.
/// An unusually shaped battlefield is worth calling out, while an ordinary one can be
/// left further down the summary.
/// </summary>
/// <returns>Returns with the display priority this option has earned.</returns>
int MapSizeGameOption::Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const // RA2: no option is left to the player, so the size alone decides (0x00728B70)
{
	static int _table[4][4] = {
		{ WDT_GAME_OPT_PRIORITY, WDT_GAME_OPT_PRIORITY,	 WDT_GAME_OPT_PRIORITY,	 WDT_GAME_OPT_PRIORITY },
		{ WDT_GAME_OPT_PRIORITY, WDT_GAME_OPT_NORMAL,	 WDT_GAME_OPT_USER_MOD,	 WDT_GAME_OPT_PRIORITY },
		{ WDT_GAME_OPT_PRIORITY, WDT_GAME_OPT_USER_MOD,	 WDT_GAME_OPT_NORMAL,	 WDT_GAME_OPT_PRIORITY },
		{ WDT_GAME_OPT_PRIORITY, WDT_GAME_OPT_PRIORITY,	 WDT_GAME_OPT_PRIORITY,	 WDT_GAME_OPT_PRIORITY }
	};

	return(_table[territory->Width][territory->Height]);
}


/// <summary>
/// Creates a flag option.
/// </summary>
/// <param name="bitmask">The bit of the territory's flags the option reads.</param>
/// <param name="off_string">The wording of the flag being off.</param>
/// <param name="off_value">The display priority of the flag being off.</param>
/// <param name="on_string">The wording of the flag being on.</param>
/// <param name="on_value">The display priority of the flag being on.</param>
FlagGameOption::FlagGameOption(unsigned int bitmask, Wstring off_string, int off_value, Wstring on_string, int on_value) : // RA2: the option keeps the bit, the two labels and the two priorities, out of line for Create (0x00728B90)
	GameOption(bitmask), // RA2: as above
	OffValue(off_value), // RA2: as above
	OnValue(on_value), // RA2: as above
	OnString(on_string), // RA2: as above
	OffString(off_string) // RA2: as above
{
}
