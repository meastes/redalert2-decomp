/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the two player cooperative campaign: the campaigns CoopCamp.ini offers, and the state of
**	the one being played (the maps each player drew, the scores so far), which is kept in
**	coopsave.ini and sent to the partner with the game options. TibSun has no such file; the code
**	is the executable's from 0x0048DE60.
*/

#define RA2_GEOMETRY_H // RA2: this module has no copy of the geometry constants (the executable has no initializer before its first function)
#include "always.h" // RA2: the includes this file needs
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"
#include <string.h> // RA2: as above
#include <stdio.h> // RA2: as above
#include <ctype.h> // RA2: as above
#include <wchar.h> // RA2: as above
#include "ccfile.h" // RA2: as above
#include "ccini.h" // RA2: as above
#include "vector.h" // RA2: as above
#include "house.h" // RA2: as above
#include "houstype.h" // RA2: as above
#include "mplayerscore.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "scenario.h" // RA2: as above
#include "session.h" // RA2: as above
#include "netshare.h" // RA2: as above
#include "ccrand.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#undef LoadString // RA2: as above
#include "stringtable.h" // RA2: as above
#include "coopcamp.h" // RA2: as above

char EncodedBuffer[1024]; // RA2: 0x00851170, 1024 characters, the result of Quoted_Printable_Encode
char DecodedBuffer[1024]; // RA2: 0x00851570, 1024 characters, the result of Quoted_Printable_Decode
char CoopCampBuffer[128]; // RA2: 0x008519C8, the line of CoopCamp.ini being split up
char * Wide_To_Narrow(wchar_t const * text); // RA2: 0x006FA120, static buffer

/// <summary>
/// The hexadecimal digit for a value from 0 to 15, or 0 for any other value.
/// </summary>
char __fastcall Hex_Digit(int value) // RA2: 0x0048DE60
{
	if (value < 0 || value > 15) return(0); // RA2: a value outside 0 to 15 has no digit and gives 0
	if (value < 10) return(value + '0'); // RA2: 0 to 9 are '0' to '9'
	return(value + '7'); // RA2: 10 to 15 are 'A' to 'F' ('A' is '7' + 10)
}

/// <summary>
/// The value of a hexadecimal digit, or 0 when the character is none.
/// </summary>
int __fastcall Hex_Value(char value) // RA2: 0x0048DE80
{
	if (value <= '9' && value >= '0') return(value - '0'); // RA2: a digit; the three ranges are tested high end first, as written here
	if (value <= 'f' && value >= 'a') return(value - 'a' + 10); // RA2: lower case letters a to f are 10 to 15
	if (value <= 'F' && value >= 'A') return(value - 'A' + 10); // RA2: upper case letters A to F are 10 to 15
	return(0); // RA2: as above
}

/// <summary>
/// Turns a name into letters and digits only (everything else becomes =XX), so that it can be part of an
/// INI section name. The result is in a static buffer.
/// </summary>
char * __fastcall Quoted_Printable_Encode(char const * input) // RA2: 0x0048DEC0
{
	char const * cursor = input; // RA2: as above
	int length; // RA2: as above
	for (length = 0; *cursor && length < 1021; ++length, ++cursor) { // RA2: stops at 1021 so that the three characters of a =XX still fit the 1024 character buffer at 0x00851170
		if (!isalnum(*cursor)) { // RA2: everything but letters and digits is written as = and two hexadecimal digits
			EncodedBuffer[length] = '='; // RA2: the escape mark
			++length; // RA2: as above
			EncodedBuffer[length] = Hex_Digit(*cursor >> 4); // RA2: the high nibble of the (signed) character
			++length; // RA2: as above
			EncodedBuffer[length] = Hex_Digit(*cursor & 15); // RA2: the low nibble
		} else {
			EncodedBuffer[length] = *cursor; // RA2: letters and digits are copied
		}
	}
	EncodedBuffer[length] = 0; // RA2: as above
	return(EncodedBuffer); // RA2: as above
}

/// <summary>
/// Undoes Quoted_Printable_Encode. The result is in a static buffer.
/// </summary>
char * __fastcall Quoted_Printable_Decode(char const * input) // RA2: 0x0048DF70
{
	char * output = DecodedBuffer; // RA2: the decoded text is made in the buffer at 0x00851570
	while (*input) { // RA2: as above
		if (*input == '=') { // RA2: = starts a hexadecimal pair
			if (!input[1]) break; // RA2: a lone = at the end ends the text
			*output = Hex_Value(input[1]); // RA2: as above
			++input; // RA2: as above
			if (input[1]) { // RA2: as above
				*output <<= 4; // RA2: the first digit becomes the high nibble when a second digit follows
				*output |= Hex_Value(input[1]); // RA2: the second digit is the low nibble
				++input; // RA2: as above
			}
		} else {
			*output = *input; // RA2: as above
		}
		++input; // RA2: as above
		++output; // RA2: as above
	}
	*output = 0; // RA2: as above
	return(DecodedBuffer); // RA2: as above
}

/// <summary>
/// Starts an empty campaign without names.
/// </summary>
CoopCampaignClass::CoopCampaignClass(void) // RA2: 0x0048E040
{
	FirstName[0] = 0; // RA2: as above
	SecondName[0] = 0; // RA2: as above
	FirstHouse = -2; // RA2: -2 means no house chosen yet (Find_Country_Index gives it for <random>)
	SecondHouse = -2; // RA2: as above
	FirstColor = -2; // RA2: -2 means no color chosen yet
	SecondColor = -2; // RA2: as above

	MapNames = 0; // RA2: as above
	CampaignType = -1; // RA2: -1 means no campaign chosen yet
	Initialized = false; // RA2: as above
	MapCount = 0; // RA2: as above
	CurrentMap = -1; // RA2: -1 before the first map
	Kills[0] = 0; // RA2: as above
	Kills[1] = 0; // RA2: as above
	Built[0] = 0; // RA2: as above
	Built[1] = 0; // RA2: as above
	Lost[0] = 0; // RA2: as above
	Lost[1] = 0; // RA2: as above
	Time = 0; // RA2: as above
	Score[0] = 0; // RA2: as above
	Score[1] = 0; // RA2: as above
}

/// <summary>
/// Starts an empty campaign for two players.
/// </summary>
CoopCampaignClass::CoopCampaignClass(wchar_t const * first, wchar_t const * second) // RA2: 0x0048E090
{
	Set_Names(first, second); // RA2: the names come first; the other members follow in this order
	FirstHouse = -2; // RA2: as above
	SecondHouse = -2; // RA2: as above
	FirstColor = -2; // RA2: as above
	SecondColor = -2; // RA2: as above
	MapNames = 0; // RA2: as above
	CampaignType = -1; // RA2: as above
	CurrentMap = -1; // RA2: as above
	Initialized = false; // RA2: as above
	MapCount = 0; // RA2: as above
	Kills[0] = 0; // RA2: as above
	Kills[1] = 0; // RA2: as above
	Built[0] = 0; // RA2: as above
	Built[1] = 0; // RA2: as above
	Lost[0] = 0; // RA2: as above
	Lost[1] = 0; // RA2: as above
	Time = 0; // RA2: as above
	Score[0] = 0; // RA2: as above
	Score[1] = 0; // RA2: as above
}

/// <summary>
/// Frees the map names.
/// </summary>
CoopCampaignClass::~CoopCampaignClass(void) // RA2: 0x0048E150
{
	if (MapNames) { // RA2: as above
		for (int i = 0; i < MapCount; ++i) { // RA2: as above
			if (MapNames[i]) delete [] MapNames[i]; // RA2: as above
		}
		delete [] MapNames; // RA2: as above
	}
}

/// <summary>
/// Sets the names of the two players (as narrow strings), or clears the one given as NULL.
/// </summary>
void CoopCampaignClass::Set_Names(wchar_t const * first, wchar_t const * second) // RA2: 0x0048E190
{
	if (first) strcpy(FirstName, Wide_To_Narrow(first)); // RA2: the names are kept as narrow strings
	else FirstName[0] = 0; // RA2: as above
	if (second) strcpy(SecondName, Wide_To_Narrow(second)); // RA2: as above
	else SecondName[0] = 0; // RA2: as above
}

/// <summary>
/// Fills in the score record of the first (or, with skip_first, of the second) human player of the game that was
/// just played: wins, kills, buildings and units made, losses and points.
/// </summary>
void __fastcall Fill_Player_Score(MPlayerScoreType & output, bool skip_first) // RA2: 0x0048E210
{
	int count = Houses.Count(); // RA2: the houses of the game; the count also bounds the loop over BuildingsKilled below
	for (int index = 0; index < count; index++) { // RA2: as above
		HouseClass * house = Houses[index]; // RA2: as above
		if (house != 0 && house->Class->IsMultiplayPassive != true && house->IsHuman) { // RA2: only the human players of the game count; the compare with true is written out as the executable shows
			if (!skip_first) { // RA2: the first human player is taken unless skip_first asks for the second
				output.Wins = 0; // RA2: wins and the other counts start at 0 before they are filled in
				wcscpy(output.Name, house->UIName); // RA2: copied without a length limit
				output.Lost[0] = 0; // RA2: as above
				output.Kills[0] = 0; // RA2: as above
				output.Built[0] = 0; // RA2: as above
				output.Score[0] = 0; // RA2: as above
				output.Scheme = house->Scheme; // RA2: as above
				if (!house->IsDefeated) { // RA2: as above
					output.Wins++; // RA2: a player that is not defeated has won
				}
				int kills = 0; // RA2: as above
				int i; // RA2: as above
				for (i = 0; i < 20; i++) { // RA2: the 20 object counts of UnitsKilled
					kills += house->UnitsKilled[i]; // RA2: as above
				}
				for (i = 0; i < count; i++) { // RA2: the buildings count is added over the number of houses, as the executable loops
					kills += house->BuildingsKilled[i]; // RA2: as above
				}
				output.Kills[0] = kills; // RA2: as above
				output.Lost[0] = house->UnitsLost + house->BuildingsLost; // RA2: lost units and buildings
				int built = house->ABQuantity.Total(); // RA2: as above
				built += house->AUQuantity.Total(); // RA2: as above
				built += house->AIQuantity.Total(); // RA2: the four production counters: buildings, units, infantry, aircraft
				built += house->AAQuantity.Total(); // RA2: as above
				output.Built[0] = house->UnitsLost + house->BuildingsLost + built; // RA2: the executable adds the lost units and buildings to the total made
				if (house->PointTotal > 0) { // RA2: as above
					output.Score[0] += house->PointTotal; // RA2: as above
				}
				char const * result = output.Wins > 0 ? "Winner" : "Loser"; // RA2: the debug text names the result
				char * name = Wide_To_Narrow(output.Name); // RA2: as above
				DebugString("@@@@ %s: %s\n Scheme: %d\n Lost = %d\n Kills = %d\n Built = %d\n Score = %d\n", // RA2: as above
							name, result, output.Scheme, // RA2: as above
							output.Lost[0], output.Kills[0], output.Built[0], output.Score[0]); // RA2: as above
				return; // RA2: as above
			}
			skip_first = false; // RA2: the first human player was skipped, so the next one is the one wanted
		}
	}
}

/// <summary>
/// The time as hours, minutes and seconds (99 hours at most).
/// </summary>
inline void Format_Time(int seconds, char * buffer) // RA2: as above
{
	if (buffer) { // RA2: as above
		if (seconds / 3600 > 99) seconds = 359999; // RA2: at most 99:59:59
		int hours = seconds / 3600; // RA2: as above
		seconds -= hours * 3600; // RA2: as above
		int minutes = seconds / 60; // RA2: as above
		seconds -= minutes * 60; // RA2: as above
		sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds); // RA2: hours, minutes and seconds with two digits each
	}
}

/// <summary>
/// Adds the scores of the map that was just played to the campaign's totals and moves on to the next map.
/// </summary>
bool CoopCampaignClass::Next_Map(void) // RA2: 0x0048E360
{
	if (!Initialized) return(false); // RA2: as above
	DebugString("@@@@ NextMap\n"); // RA2: as above
	MPlayerScoreType first; // RA2: as above
	Fill_Player_Score(first, false); // RA2: as above
	MPlayerScoreType second; // RA2: as above
	Fill_Player_Score(second, true); // RA2: as above
	if (strcmp(FirstName, Wide_To_Narrow(first.Name)) == 0) { // RA2: the first human player is the local one when the names agree
		DebugString("@@@@ %s is local\n", Wide_To_Narrow(first.Name)); // RA2: as above
		Kills[0] += first.Kills[0]; Kills[1] += second.Kills[0]; // RA2: as above
		Built[0] += first.Built[0]; Built[1] += second.Built[0]; // RA2: as above
		Lost[0] += first.Lost[0]; Lost[1] += second.Lost[0]; // RA2: as above
		Score[0] += first.Score[0]; Score[1] += second.Score[0]; // RA2: as above
	} else {
		DebugString("@@@@ Assuming %s is local\n", Wide_To_Narrow(second.Name)); // RA2: as above
		Kills[1] += first.Kills[0]; Kills[0] += second.Kills[0]; // RA2: the first record is the partner's when the names differ: the sums swap sides
		Built[1] += first.Built[0]; Built[0] += second.Built[0]; // RA2: as above
		Lost[1] += first.Lost[0]; Lost[0] += second.Lost[0]; // RA2: as above
		Score[1] += first.Score[0]; Score[0] += second.Score[0]; // RA2: as above
	}
	char game_time[256]; // RA2: as above
	Format_Time(Scen->ElapsedTimer / 60, game_time); // RA2: the executable divides the elapsed timer by 60 for the displayed time
	Time += Scen->ElapsedTimer; // RA2: the total keeps the unconverted ticks
	char total_time[256]; // RA2: as above
	Format_Time(Time, total_time); // RA2: as above
	DebugString("@@@@ Game Time: %s, Total Time: %s\n", game_time, total_time); // RA2: as above
	++CurrentMap; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// The index of the campaign in CoopCamp.ini; false when no campaign is set up.
/// </summary>
bool CoopCampaignClass::Get_Campaign_Type(int * value) // RA2: 0x0048E6B0
{
	if (!Initialized) return(false); // RA2: as above
	*value = CampaignType; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// How many maps the campaign has; false when no campaign is set up.
/// </summary>
bool CoopCampaignClass::Get_Map_Count(int * value) // RA2: 0x0048E6D0
{
	if (!Initialized) return(false); // RA2: as above
	*value = MapCount; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// The number of the map being played; false when no campaign is set up.
/// </summary>
bool CoopCampaignClass::Get_Current_Map(int * value) // RA2: 0x0048E6F0
{
	if (!Initialized) return(false); // RA2: as above
	*value = CurrentMap; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// Replaces the list with the score records of the two players (the local player first) with the campaign's
/// totals, and gives the time the campaign has taken.
/// </summary>
bool CoopCampaignClass::Get_Player_Scores(DynamicVectorClass<MPlayerScoreType *> & scores, int * time) // RA2: 0x0048E710
{
	if (!Initialized) return(false); // RA2: as above
	while (scores.Count() != 0) { // RA2: the old records are deleted one at a time from the front
		delete scores[0]; // RA2: as above
		scores.Delete_Index(0); // RA2: as above
	}
	MPlayerScoreType * first = new MPlayerScoreType; // RA2: 0x70 bytes each: the record of RA2 (20 character name, scheme, wins, four counts of four)
	Fill_Player_Score(*first, false); // RA2: as above
	MPlayerScoreType * second = new MPlayerScoreType; // RA2: as above
	Fill_Player_Score(*second, true); // RA2: as above
	if (strcmp(FirstName, Wide_To_Narrow(first->Name)) != 0) { // RA2: the local player's record goes first
		MPlayerScoreType * temp = first; // RA2: as above
		first = second; // RA2: as above
		second = temp; // RA2: as above
	}
	for (int i = 0; i < 2; ++i) { // RA2: as above
		MPlayerScoreType * score = i == 0 ? first : second; // RA2: as above
		score->Kills[0] = Kills[i]; // RA2: as above
		score->Built[0] = Built[i]; // RA2: as above
		score->Lost[0] = Lost[i]; // RA2: as above
		score->Wins = CurrentMap; // RA2: the number of wins takes the number of the map reached
		score->Score[0] = Score[i]; // RA2: as above
		scores.Add(score); // RA2: as above
	}
	*time = Time; // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// Opens coopsave.ini for writing.
/// </summary>
inline void Prepare_Campaign_Save_File(FileClass & file) // RA2: as above
{
	file.Set_Name("coopsave.ini"); // RA2: as above
	file.Open(FileClass::WRITE); // RA2: opened for writing
}

/// <summary>
/// Removes this pair of players' section from coopsave.ini.
/// </summary>
bool CoopCampaignClass::Clear_Saved_Campaign(void) // RA2: 0x0048E870
{
	if (FirstName[0] && SecondName[0]) { // RA2: as above
		CCFileClass saved("coopsave.ini"); // RA2: as above
		CCINIClass ini; // RA2: as above
		ini.Load(saved, false, false); // RA2: as above
		saved.Close(); // RA2: as above
		char section[100]; // RA2: as above
		strcpy(section, Quoted_Printable_Encode(FirstName)); // RA2: as above
		strcat(section, Quoted_Printable_Encode(SecondName)); // RA2: as above
		CCFileClass output; // RA2: as above
		Prepare_Campaign_Save_File(output); // RA2: as above
		ini.Clear(section, 0); // RA2: as above
		ini.Save(output, false); // RA2: as above
		output.Close(); // RA2: as above
	}
	return(true); // RA2: as above
}

/// <summary>
/// Reads this pair of players' campaign back from coopsave.ini.
/// </summary>
bool CoopCampaignClass::Load_Saved_Campaign(void) // RA2: 0x0048EB30
{
	if (!FirstName[0] || !SecondName[0]) return(false); // RA2: as above
	char section[100]; // RA2: as above
	strcpy(section, Quoted_Printable_Encode(FirstName)); // RA2: as above
	strcat(section, Quoted_Printable_Encode(SecondName)); // RA2: as above
	CCFileClass saved("coopsave.ini"); // RA2: as above
	CCINIClass ini; // RA2: as above
	if (!ini.Load(saved, false, false)) return(false); // RA2: as above
	CurrentMap = ini.Get_Int(section, "CurrentMap", 0); // RA2: as above
	ini.Get_Int(section, "Score", 0); // RA2: the executable reads the key and does not keep the value
	CampaignType = ini.Get_Int(section, "CampaignType", -1); // RA2: -1 when none was saved
	FirstHouse = ini.Get_HousesType(section, "House1", (HousesType)-2); // RA2: -2 as the constructors set when nothing was saved
	FirstColor = ini.Get_Int(section, "Color1", -2); // RA2: as above
	SecondHouse = ini.Get_HousesType(section, "House2", (HousesType)-2); // RA2: as above for the partner
	SecondColor = ini.Get_Int(section, "Color2", -2); // RA2: as above
	Kills[0] = ini.Get_Int(section, "Kills1", 0); // RA2: as above
	Kills[1] = ini.Get_Int(section, "Kills2", 0); // RA2: as above
	Built[0] = ini.Get_Int(section, "Built1", 0); // RA2: as above
	Built[1] = ini.Get_Int(section, "Built2", 0); // RA2: as above
	Lost[0] = ini.Get_Int(section, "Lost1", 0); // RA2: as above
	Lost[1] = ini.Get_Int(section, "Lost2", 0); // RA2: as above
	Score[0] = ini.Get_Int(section, "Score1", 0); // RA2: as above
	Score[1] = ini.Get_Int(section, "Score2", 0); // RA2: as above
	Time = ini.Get_Int(section, "Time", 0); // RA2: as above
	CoopCampListClass * campaigns = new CoopCampListClass; // RA2: CoopCamp.ini is read again for the map count of the saved campaign
	if (CampaignType >= 0 && CampaignType < campaigns->Count) { // RA2: as above
		CoopCampEntry const & record = campaigns->Entries[CampaignType]; // RA2: as above
		MapCount = (signed char)record.MapCount; // RA2: as above
		MapNames = new char *[(signed char)record.MapCount]; // RA2: the map count is a signed char in the record
		for (int i = 0; i < MapCount; ++i) { // RA2: as above
			char key[32]; // RA2: as above
			sprintf(key, "Map%d", i + 1); // RA2: as above
			MapNames[i] = new char[260]; // RA2: 260 characters per map name
			ini.Get_String(section, key, "undefined", MapNames[i], 260); // RA2: a map missing from the save reads as undefined
		}
		delete campaigns; // RA2: as above
		Initialized = true; // RA2: as above
		return(true); // RA2: as above
	}
	MapCount = 0; // RA2: as above
	delete campaigns; // RA2: as above
	return(false); // RA2: as above
}

/// <summary>
/// Writes the campaign to coopsave.ini: the maps, the two players' houses and colors and the totals.
/// </summary>
bool CoopCampaignClass::Save_Campaign(void) // RA2: 0x0048F040
{
	if (Initialized && FirstName[0] && SecondName[0]) { // RA2: as above
		CoopCampListClass * campaigns = new CoopCampListClass; // RA2: as above
		CoopCampEntry * record; // RA2: as above
		if (!campaigns->Get_Record(CampaignType, &record)) { // RA2: the campaign must still be in CoopCamp.ini
			delete campaigns; // RA2: as above
			return(false); // RA2: as above
		}
		{
			CCFileClass saved("coopsave.ini"); // RA2: as above
			CCINIClass ini; // RA2: as above
			ini.Load(saved, false, false); // RA2: as above
			saved.Close(); // RA2: as above
			char section[100]; // RA2: as above
			strcpy(section, Quoted_Printable_Encode(FirstName)); // RA2: as above
			strcat(section, Quoted_Printable_Encode(SecondName)); // RA2: as above
			CCFileClass output; // RA2: as above
			Prepare_Campaign_Save_File(output); // RA2: as above
			ini.Clear(section, 0); // RA2: as above
			ini.Put_Int(section, "CurrentMap", CurrentMap); // RA2: as above
			ini.Put_Int(section, "CampaignType", CampaignType); // RA2: as above
			for (int i = 0; i < (signed char)record->MapCount; ++i) { // RA2: as above
				char key[32]; // RA2: as above
				sprintf(key, "Map%d", i + 1); // RA2: as above
				ini.Put_String(section, key, MapNames[i]); // RA2: as above
			}
			if (Session.Players.Count()) { // RA2: as above
				ini.Put_HousesType(section, "House1", (HousesType)Session.Players[0]->Get_Country()); // RA2: the local player is the first of the session's players
				ini.Put_Int(section, "Color1", Session.Players[0]->Get_Color()); // RA2: as above
			}
			for (int player = 1; player < Session.Players.Count(); ++player) { // RA2: as above
				if (Session.Options.AISlots.Countries[player] == -1) { // RA2: the partner is the first player after the local one whose slot has no computer country (-1)
					ini.Put_HousesType(section, "House2", (HousesType)Session.Players[player]->Get_Country()); // RA2: the partner
					ini.Put_Int(section, "Color2", Session.Players[player]->Get_Color()); // RA2: as above
					break; // RA2: as above
				}
			}
			ini.Put_Int(section, "Time", Time); // RA2: as above
			ini.Put_Int(section, "Kills1", Kills[0]); // RA2: as above
			ini.Put_Int(section, "Kills2", Kills[1]); // RA2: as above
			ini.Put_Int(section, "Built1", Built[0]); // RA2: as above
			ini.Put_Int(section, "Built2", Built[1]); // RA2: as above
			ini.Put_Int(section, "Lost1", Lost[0]); // RA2: as above
			ini.Put_Int(section, "Lost2", Lost[1]); // RA2: as above
			ini.Put_Int(section, "Score1", Score[0]); // RA2: as above
			ini.Put_Int(section, "Score2", Score[1]); // RA2: as above
			ini.Save(output, false); // RA2: as above
			output.Close(); // RA2: as above
		}
		delete campaigns; // RA2: as above
		Initialized = true; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Starts the campaign at this index of CoopCamp.ini: each map's variant is drawn at random.
/// </summary>
bool CoopCampaignClass::Load_Campaign(int index) // RA2: 0x0048F590
{
	CoopCampListClass * campaigns = new CoopCampListClass; // RA2: as above
	if (index >= 0 && index < campaigns->Count) { // RA2: as above
		CoopCampEntry * record = &campaigns->Entries[index]; // RA2: as above
		Initialized = true; // RA2: as above
		CurrentMap = -1; // RA2: as above
		Kills[0] = 0; // RA2: as above
		Kills[1] = 0; // RA2: as above
		Built[0] = 0; // RA2: as above
		Built[1] = 0; // RA2: as above
		Score[0] = 0; // RA2: as above
		Score[1] = 0; // RA2: as above
		Lost[0] = 0; // RA2: as above
		Lost[1] = 0; // RA2: as above
		Time = 0; // RA2: as above
		if (CampaignType != index) { // RA2: as above
			if (MapNames) { // RA2: as above
				for (int i = 0; i < MapCount; ++i) { // RA2: as above
					if (MapNames[i]) delete [] MapNames[i]; // RA2: as above
				}
				delete [] MapNames; // RA2: as above
			}
			MapNames = 0; // RA2: as above
			MapCount = 0; // RA2: as above
			MapNames = new char *[(signed char)record->MapCount]; // RA2: as above
			for (int m = 0; m < (signed char)record->MapCount; ++m) { // RA2: as above
				int choice = Scen->RandomNumber(0, 2); // RA2: each map's variant is drawn out of three
				MapNames[m] = new char[260]; // RA2: as above
				strncpy(MapNames[m], record->Maps + (m * 3 + choice) * 260, 259); // RA2: a map has three variants of 260 characters; copies at most 259 and no terminator is added
			}
			MapCount = (signed char)record->MapCount; // RA2: as above
			CurrentMap = 0; // RA2: as above
		}
		CampaignType = index; // RA2: as above
		delete campaigns; // RA2: as above
		return(true); // RA2: as above
	}
	delete campaigns; // RA2: as above
	return(false); // RA2: as above
}

/// <summary>
/// Adds the scores of one map to the totals and makes that map the current one.
/// </summary>
bool CoopCampaignClass::Add_Score(int index, int time, MPlayerScoreType first, MPlayerScoreType second) // RA2: 0x0048F730
{
	if (index >= 0 && index < MapCount) { // RA2: as above
		Kills[0] += first.Kills[0]; // RA2: as above
		Kills[1] += second.Kills[0]; // RA2: as above
		Built[0] += first.Built[0]; // RA2: as above
		Built[1] += second.Built[0]; // RA2: as above
		Lost[0] += first.Lost[0]; // RA2: as above
		Lost[1] += second.Lost[0]; // RA2: as above
		Score[0] += first.Score[0]; // RA2: as above
		Score[1] += second.Score[0]; // RA2: as above
		Time = time; // RA2: as above
		CurrentMap = index; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Sets up the campaign the host sent: the campaign, the variant of each map and the players' (encoded) names.
/// </summary>
bool CoopCampaignClass::Set_Campaign_Info(CoopCampListClass const & campaigns, int index, DynamicVectorClass<int> choices, char const * second_name, char const * first_name) // RA2: 0x0048F7D0
{
	if (index >= 0 && index < campaigns.Count) { // RA2: as above
		CoopCampEntry const * record = &campaigns.Entries[index]; // RA2: as above
		CurrentMap = -1; // RA2: as above
		Kills[0] = 0; // RA2: as above
		Kills[1] = 0; // RA2: as above
		Built[0] = 0; // RA2: as above
		Built[1] = 0; // RA2: as above
		Lost[0] = 0; // RA2: as above
		Lost[1] = 0; // RA2: as above
		Score[0] = 0; // RA2: as above
		Score[1] = 0; // RA2: as above
		Time = 0; // RA2: as above
		if ((signed char)record->MapCount != choices.Count()) return(false); // RA2: the host must have chosen one variant per map
		CampaignType = index; // RA2: as above
		Initialized = true; // RA2: as above
		if (MapNames) { // RA2: as above
			for (int i = 0; i < MapCount; ++i) { // RA2: as above
				if (MapNames[i]) delete [] MapNames[i]; // RA2: as above
			}
			delete [] MapNames; // RA2: as above
		}
		MapNames = 0; // RA2: as above
		MapCount = 0; // RA2: as above
		MapNames = new char *[choices.Count()]; // RA2: as above
		for (int m = 0; m < choices.Count(); ++m) { // RA2: as above
			MapNames[m] = new char[260]; // RA2: as above
			strncpy(MapNames[m], record->Maps + (m * 3 + choices[m]) * 260, 259); // RA2: the variant the host chose for map m (three names of 260 characters per map)
		}
		MapCount = choices.Count(); // RA2: as above
		CurrentMap = 0; // RA2: as above
		if (first_name) strcpy(FirstName, Quoted_Printable_Decode(first_name)); // RA2: the names arrive encoded (see Send_Campaign_Options)
		else FirstName[0] = 0; // RA2: as above
		if (second_name) strcpy(SecondName, Quoted_Printable_Decode(second_name)); // RA2: the second name is read after the first
		else SecondName[0] = 0; // RA2: as above
		DebugString("@@@@ SetCampaignInfo saw campaign_type %d\n", CampaignType); // RA2: as above
	}
	return(Initialized); // RA2: as above
}

/// <summary>
/// The name of a map of the campaign (the current one for -1).
/// </summary>
bool CoopCampaignClass::Get_Map_Name(char * * name, int index) // RA2: 0x0048FB40
{
	bool result = false; // RA2: as above
	*name = 0; // RA2: as above
	if (index == -1) index = CurrentMap; // RA2: -1 asks for the current map
	if (index < 0 || index >= MapCount) index = -1; // RA2: an index outside the maps gives no name
	if (Initialized) { // RA2: as above
		if (index != -1) *name = MapNames[index]; // RA2: as above
		if (*name != 0) result = true; // RA2: as above
	}
	return(result); // RA2: as above
}

/// <summary>
/// The house and color of the local player.
/// </summary>
bool CoopCampaignClass::Get_First_Player_Settings(int * house, int * color) // RA2: 0x0048FB90
{
	if (Initialized) { // RA2: as above
		*house = FirstHouse; // RA2: as above
		*color = FirstColor; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// The house and color of the partner.
/// </summary>
bool CoopCampaignClass::Get_Second_Player_Settings(int * house, int * color) // RA2: 0x0048FBC0
{
	if (Initialized) { // RA2: as above
		*house = SecondHouse; // RA2: as above
		*color = SecondColor; // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}

/// <summary>
/// Whether coopsave.ini has a section for this pair of players.
/// </summary>
bool CoopCampaignClass::Has_Saved_Campaign(void) // RA2: 0x0048FBF0
{
	if (FirstName[0] && SecondName[0]) { // RA2: as above
		CCFileClass saved("coopsave.ini"); // RA2: as above
		CCINIClass ini; // RA2: as above
		if (!ini.Load(saved, false, false)) { // RA2: as above
			saved.Close(); // RA2: as above
		} else {
			saved.Close(); // RA2: as above
			char section[100]; // RA2: as above
			strcpy(section, Quoted_Printable_Encode(FirstName)); // RA2: as above
			strcat(section, Quoted_Printable_Encode(SecondName)); // RA2: as above
			return(ini.Find_Section(section) != 0); // RA2: as above
		}
	}
	return(false); // RA2: as above
}

/// <summary>
/// Whether the saved campaign of this pair of players has no name for the map it stands at; the name is put in the buffer.
/// </summary>
bool CoopCampaignClass::Saved_Map_Is_Undefined(char * map) // RA2: 0x0048FE70
{
	if (FirstName[0] && SecondName[0]) { // RA2: as above
		CCFileClass saved("coopsave.ini"); // RA2: as above
		CCINIClass ini; // RA2: as above
		if (!ini.Load(saved, false, false)) { // RA2: as above
			saved.Close(); // RA2: as above
			return(false); // RA2: leaves at once when the file cannot be read; without this return the compiler expands Quoted_Printable_Encode and Hex_Digit differently from the executable (32 bytes longer)
		} else {
			saved.Close(); // RA2: as above
			char section[100]; // RA2: as above
			strcpy(section, Quoted_Printable_Encode(FirstName)); // RA2: as above
			strcat(section, Quoted_Printable_Encode(SecondName)); // RA2: as above
			int current = ini.Get_Int(section, "CurrentMap", 0); // RA2: as above
			char key[32]; // RA2: as above
			sprintf(key, "Map%d", current + 1); // RA2: as above
			ini.Get_String(section, key, "undefined", map, 260); // RA2: as above
			if (strcmp(map, "undefined") == 0) return(true); // RA2: as above
		}
	}
	return(false); // RA2: as above
}

/// <summary>
/// Sends the campaign (type, current map, totals, each map's variant and the players' names) to the partner with
/// the game options.
/// </summary>
bool CoopCampaignClass::Send_Campaign_Options(CoopCampListClass & campaigns) // RA2: 0x00490150
{
	if (!Initialized) return(false); // RA2: as above
	CoopCampEntry * record; // RA2: as above
	if (!campaigns.Get_Record(CampaignType, &record)) return(false); // RA2: as above
	char message[400]; // RA2: as above
	sprintf(message, "C0,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", CampaignType, CurrentMap, Time, // RA2: the campaign line the partner decodes in Net2DecodeGameopt: type, current map, time, then kills, built, lost and score of the local player, then of the partner
			Kills[0], Built[0], Lost[0], Score[0], Kills[1], Built[1], Lost[1], Score[1]); // RA2: the line continues as above
	for (int map = 0; map < MapCount; ++map) { // RA2: as above
		int choice = 0; // RA2: as above
		for (int option = 0; option < 3; ++option) { // RA2: as above
			if (stricmp(record->Maps + map * 0x30C + option * 260, MapNames[map]) == 0) { // RA2: the variant of this map is found by comparing with the three names (0x30C bytes per map)
				choice = option; // RA2: as above
				break; // RA2: as above
			}
		}
		char choice_text[16]; // RA2: as above
		sprintf(choice_text, ",%d", choice); // RA2: as above
		strcat(message, choice_text); // RA2: as above
	}
	strcat(message, ",-1,"); // RA2: -1 ends the variant list
	strcat(message, Quoted_Printable_Encode(FirstName)); // RA2: the names are sent encoded, separated by a comma
	strcat(message, ","); // RA2: as above
	strcat(message, Quoted_Printable_Encode(SecondName)); // RA2: as above
	DebugString("@@@@ Sending campaign game option string: %s\n", message); // RA2: as above
	Send_Host_Option(message); // RA2: sent to the partner with the game options
	DebugString("@@@@ Sent\n"); // RA2: as above
	return(true); // RA2: as above
}

/// <summary>
/// The index of the house type with this given name or ID, -2 for "&lt;random&gt;" and -1 when there is none.
/// </summary>
int __fastcall Find_Country_Index(char const * name) // RA2: 0x004904A0
{
	if (stricmp(name, "<random>") == 0) return(-2); // RA2: -2 stands for <random>
	if (name) { // RA2: as above
		for (int index = 0; index < HouseTypes.Count(); ++index) { // RA2: as above
			HouseTypeClass * country = HouseTypes[index]; // RA2: as above
			if (stricmp(country->Full_Name(), name) == 0 || stricmp(country->Name(), name) == 0) // RA2: the given name or the ID name matches
				return(index); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}

/// <summary>
/// Reads CoopCamp.ini: for each campaign its title, difficulty name, maps with their variants and the
/// house names of the players and enemies.
/// </summary>
CoopCampListClass::CoopCampListClass(void) // RA2: 0x00490510
{
	CCFileClass file("CoopCamp.ini"); // RA2: the list is read from CoopCamp.ini when the object is made
	if (!Ini.Load(file, false, false)) { // RA2: an unreadable file leaves an empty list that is marked not initialized
		Count = 0; // RA2: as above
		Entries = 0; // RA2: as above
		Initialized = false; // RA2: as above
		return; // RA2: as above
	}
	Count = Ini.Entry_Count("Campaigns"); // RA2: one entry per campaign in the Campaigns section
	if (Count <= 0) { // RA2: as above
		Count = 0; // RA2: as above
		Entries = 0; // RA2: as above
	} else {
		Entries = new CoopCampEntry[Count]; // RA2: the array of 0x44 byte entries (the compiler makes its vector deleting destructor, 0x00490EF0)
		if (Entries) { // RA2: as above
			for (int i = 0; i < Count; ++i) { // RA2: as above
				char const * entry = Ini.Get_Entry("Campaigns", i); // RA2: as above
				if (!entry) continue; // RA2: as above
				char section[128]; // RA2: as above
				Ini.Get_String("Campaigns", entry, 0, section, 128); // RA2: the value of the entry names the section of the campaign
				int map_count = Ini.Get_Int(section, "NumberOfCampaignMaps", 0); // RA2: the number of maps of the campaign
				if (Ini.Get_String(section, "CampaignName", 0, CoopCampBuffer, 128)) { // RA2: the title is a string table label
#line 1032 "D:\\Ra2\\coopcamp.cpp"
					wchar_t const * name = StringTable::LoadString(CoopCampBuffer, 0, __FILE__, __LINE__); // RA2: the source file name and line (0x408) the executable passes are those of its own build; the #line above gives them
					Entries[i].Name = new wchar_t[129]; // RA2: 128 characters and the terminator
					if (Entries[i].Name) wcscpy(Entries[i].Name, name); // RA2: as above
				}
				Ini.Get_String(section, "CampaignAI", "Easy", CoopCampBuffer, 128); // RA2: the difficulty name of the computer players, Easy when missing
				Entries[i].AIName = new char[strlen(CoopCampBuffer) + 1]; // RA2: as above
				strcpy(Entries[i].AIName, CoopCampBuffer); // RA2: as above
				Entries[i].Index = i; // RA2: as above
				Entries[i].MapCount = (char)map_count; // RA2: stored in a signed char
				Entries[i].Maps = new char[map_count * 0x30C]; // RA2: three names of 260 characters (0x104) per map, so 0x30C bytes a map
				if (Ini.Get_String(section, "CampaignPlayer", 0, CoopCampBuffer, 128)) { // RA2: the house names of the players, separated by commas
					char * token = strtok(CoopCampBuffer, ","); // RA2: strtok splits the line at the commas; the first token is taken apart from the loop
					if (token) { // RA2: as above
						char * copy = new char[strlen(token) + 1]; // RA2: as above
						strcpy(copy, token); // RA2: as above
						Entries[i].Players.Add(copy); // RA2: as above
					}
					token = strtok(0, ","); // RA2: as above
					while (token) { // RA2: as above
						char * copy = new char[strlen(token) + 1]; // RA2: as above
						strcpy(copy, token); // RA2: as above
						Entries[i].Players.Add(copy); // RA2: as above
						token = strtok(0, ","); // RA2: as above
					}
				}
				if (Ini.Get_String(section, "CampaignEnemy", 0, CoopCampBuffer, 128)) { // RA2: the house names of the enemies, separated by commas
					char * token = strtok(CoopCampBuffer, ","); // RA2: strtok splits the line at the commas; the first token is taken apart from the loop
					if (token) { // RA2: as above
						char * copy = new char[strlen(token) + 1]; // RA2: as above
						strcpy(copy, token); // RA2: as above
						Entries[i].Enemies.Add(copy); // RA2: as above
					}
					token = strtok(0, ","); // RA2: as above
					while (token) { // RA2: as above
						char * copy = new char[strlen(token) + 1]; // RA2: as above
						strcpy(copy, token); // RA2: as above
						Entries[i].Enemies.Add(copy); // RA2: as above
						token = strtok(0, ","); // RA2: as above
					}
				}
				for (int m = 1; m < map_count + 1; ++m) { // RA2: as above
					char key[32]; // RA2: as above
					sprintf(key, "Map%d", m); // RA2: the maps are Map1, Map2 and so on
					if (Ini.Get_String(section, key, 0, CoopCampBuffer, 128)) { // RA2: as above
						char * token = strtok(CoopCampBuffer, ","); // RA2: strtok splits the line at the commas; the first token is taken apart from the loop
						if (token) strcpy(Entries[i].Maps + (m - 1) * 0x30C, token); // RA2: the first variant of the map
						token = strtok(0, ","); // RA2: as above
						if (token) strcpy(Entries[i].Maps + (m - 1) * 0x30C + 0x104, token); // RA2: the second variant
						token = strtok(0, ","); // RA2: as above
						if (token) strcpy(Entries[i].Maps + (m - 1) * 0x30C + 0x208, token); // RA2: the third variant
					}
				}
			}
		}
	}
	Initialized = true; // RA2: as above
}

/// <summary>
/// The campaign at this index.
/// </summary>
bool CoopCampListClass::Get_Record(int index, CoopCampEntry * * record) // RA2: 0x00490C50
{
	if (index >= 0 && index < Count) { // RA2: as above
		*record = &Entries[index]; // RA2: as above
		return(true); // RA2: as above
	}
	*record = 0; // RA2: as above
	return(false); // RA2: as above
}

/// <summary>
/// How many campaigns there are; false when CoopCamp.ini could not be read.
/// </summary>
bool CoopCampListClass::Get_Count(int * count) const // RA2: 0x00490C90
{
	*count = Count; // RA2: as above
	return(Initialized); // RA2: as above
}

/// <summary>
/// Frees the campaigns.
/// </summary>
CoopCampListClass::~CoopCampListClass(void) // RA2: 0x00490CA0
{
	if (Entries) { // RA2: as above
		for (int i = 0; i < Count; ++i) { // RA2: as above
			if (Entries[i].Maps) { // RA2: an entry that was never filled in (no Maps) has nothing to free
				delete [] Entries[i].Name; // RA2: as above
				delete [] Entries[i].AIName; // RA2: as above
				delete [] Entries[i].Maps; // RA2: as above
			}
			for (int j = 0; j < Entries[i].Enemies.Count(); ++j) { // RA2: as above
				delete [] Entries[i].Enemies[j]; // RA2: the enemy names go first, then the player names
			}
			Entries[i].Enemies.Clear(); // RA2: empties the list
			for (j = 0; j < Entries[i].Players.Count(); ++j) { // RA2: the j of the loop above is reused (the executable shows one counter)
				delete [] Entries[i].Players[j]; // RA2: as above
			}
			Entries[i].Players.Clear(); // RA2: as above
		}
		delete [] Entries; // RA2: the entries are destroyed through the compiler's vector deleting destructor (0x00490EF0)
		Entries = 0; // RA2: as above
		Initialized = false; // RA2: as above
	}
}

/// <summary>
/// Makes a copy of each campaign's title for a list box.
/// </summary>
bool CoopCampListClass::Copy_Names(DynamicVectorClass<wchar_t *> & names) // RA2: 0x00490E00
{
	if (Initialized) { // RA2: as above
		names.Resize(Count, 0); // RA2: as above
		for (int i = 0; i < Count; ++i) { // RA2: as above
			names[i] = new wchar_t[129]; // RA2: as above
			if (names[i]) wcscpy(names[i], Entries[i].Name); // RA2: as above
		}
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}
