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
**	RA2: the campaign map selection (the choice of the next mission after a mission is won). Tiberian Sun's
**	mapsel.cpp had an interactive map; RA2's keeps only the bookkeeping: the next scenario is the first
**	selection of the current stage, and the screen itself is never shown. The code is the executable's from
**	0x0058F320 (the static initializers of the geometry constants) to 0x0058FDE0, apart from the compiler-made
**	copies and three empty functions (0x0058FCF0, 0x0058FD70, 0x0058FD80) whose source is not known.
*/

#define GEOMETRY_H // RA2: geometry.h follows the includes, as its constants take their initializers after those of the other headers
#include "always.h" // RA2: as above
#include "mapsel.h" // RA2: as above
#include "_surface.h" // RA2: as above
#include "convert.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "houstype.h" // RA2: as above
#include "side.h" // RA2: the includes this file needs
#include "msanim.h" // RA2: as above
#include "mschoice.h" // RA2: as above
#include "msengine.h" // RA2: as above
#include "msfont.h" // RA2: as above
#include "msgbox.h" // RA2: as above
#include "scenario.h" // RA2: as above
#include "stimer.h" // RA2: as above
#include "stringtable.h" // RA2: the includes this file needs
#include "surface.h" // RA2: as above
#include "theme.h" // RA2: as above
#include "timer.h" // RA2: as above
#include "geometry.h" // RA2: this module holds its own copy of the geometry constants (initialized after MapSelectRect, from 0x0058F340)

#define RA2_SOURCE "D:\\Ra2\\MapSel.CPP" // RA2: the path the executable passes with each lookup



/// The binary confirms this was in the cpp as the vtable is inside this module
/// Moving it into a header would make the first thing that includes the header construct the vtable in that module

class MapSelect : public MSEngine { // RA2: as above
	public: // RA2: as above
		bool Set_Next_Scenario(ScenarioClass * scenario); // RA2: named after YR's SetNextScenario; TibSun's Presentation showed the map
		bool Advance_Progression(ScenarioClass * scenario, Wstring const & map_name); // RA2: the name arrives as a Wstring

	private: // RA2: as above
		bool Init(ScenarioClass * scenario); // RA2: as above
		void Deinit(void); // RA2: as above

		void Play_Sound(Wstring const & name); // RA2: the name is a Wstring
		void Queue_Voice(Wstring const & name, int delay); // RA2: the voice name is a Wstring (0x0058FD30 assigns it with Wstring::operator=)

		virtual void Process_Idle(void) override; // RA2: the third vtable slot of MapSelect is at 0x0058FCC0

	private: // RA2: as above
		int XOffset; // RA2: as above
		int YOffset; // RA2: as above

		MapChoice Choices; // RA2: as above

		ConvertClass * AnimDrawer; // RA2: as above
		ConvertClass * OverlayDrawer; // RA2: as above

		MSFont * Font; // RA2: as above
		Rect TextRect; // RA2: as above

		Wstring VoiceName; // RA2: a Wstring (0x0058FD30 assigns it with Wstring::operator=)
		CDTimerClass<SystemTimerClass> VoiceTimer; // RA2: as above
		int VoiceHandle; // RA2: as above
};


/// <summary>
/// Starts the whole process of selecting the next map to go to.
/// </summary>
/// <param name="scen">The scenario to advance. It is updated with the map chosen.</param>
/// <returns>Returns with the name of the next scenario, or NULL if there is none.</returns>
const char * Map_Selection(ScenarioClass * scen) // RA2: as above
{
	if (MapSelect().Set_Next_Scenario(scen) == false) { // RA2: the temporary is built and destroyed inline; the message box below is reached on failure
		WWMessageBox(L" ").Process(StringTable::LoadString("MSG:UnableMapSelect", NULL, RA2_SOURCE, 134), StringTable::LoadString("TXT_OK", NULL, RA2_SOURCE, 134), NULL, NULL, false); // RA2: the message box takes wide text
		return(NULL); // RA2: as above
	}

	DebugString("MapSelect: Scenario - %s, Stage - %ld\n", scen->ScenarioName, scen->Stage); // RA2: as above

	return((strlen(scen->ScenarioName) == 0) ? NULL : scen->ScenarioName); // RA2: as above
}


/// <summary>
/// Advances the campaign to a specific map.
/// This is the way into the map selection system when the next map is already known, such
/// as when a mission hands directly off to another. An error box is displayed if the map
/// is not one the campaign can reach from here.
/// </summary>
/// <param name="scenario">The scenario to advance. It is updated with the map named.</param>
/// <param name="map_name">Name of the scenario to advance to.</param>
/// <returns>Returns with the map name if the advance succeeded, otherwise NULL.</returns>
const char * Map_Select_Advance(ScenarioClass * scenario, const char * map_name) // RA2: as above
{
	if (MapSelect().Advance_Progression(scenario, map_name) == false) { // RA2: as above
		WWMessageBox(L" ").Process(StringTable::LoadString("MSG:UnableMapSelect", NULL, RA2_SOURCE, 152), StringTable::LoadString("TXT_OK", NULL, RA2_SOURCE, 152), NULL, NULL, false); // RA2: the same message box as Map_Selection, with the line number 152 of the source
		return(NULL); // RA2: as above
	}

	return(map_name); // RA2: as above
}


/// <summary>
/// Advances the campaign to a map without showing the selection screen.
/// This routine is used when the destination is already decided and only the bookkeeping
/// is wanted. The map must be one of the choices offered by the scenario's current stage.
/// </summary>
/// <param name="scen">The scenario to advance. It is updated with the map named.</param>
/// <param name="map_name">Name of the scenario to advance to.</param>
/// <returns>bool; Was the map found among the available choices?</returns>
bool MapSelect::Advance_Progression(ScenarioClass * scen, Wstring const & map_name) // RA2: the name arrives as a Wstring (the call from Map_Select_Advance converts the char pointer)
{
	if ((scen == NULL) || (map_name == NULL)) { // RA2: as above
		return(false); // RA2: as above
	}

	SideClass * side = Sides[HouseTypes[scen->PlayerHouse]->Side]; // RA2: the choices belong to the side, not the house
	if (side == NULL) { // RA2: the executable checks the side (string at 0x007E1C44), TibSun checked the house
		DebugString("MapSelect: Invalid side!\n"); // RA2: the string is the executable's
		return(false); // RA2: as above
	}

	DebugString("MapSelect: Side %s\n", (const char*)side->IniName); // RA2: the string is the executable's, the side's INI name is printed

	bool failed; // RA2: the result of the initialization is kept until the Wstring temporary is gone
	{
		Wstring side_name((const char*)side->IniName); // RA2: the temporary is destroyed before the failure test
		failed = (Choices.Initialize(side_name) == false); // RA2: Initialize takes the side's name as a Wstring (0x005B0030)
	}
	if (failed) { // RA2: the failure test follows the end of the temporary's scope
		Choices.Deinit(); // RA2: as above
		DebugString("MapSelect: Unable to initialize choices!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	MapStage * stage = Choices.Find_Stage_By_ID(scen->Stage); // RA2: as above

	if (stage == NULL) { // RA2: as above
		Choices.Deinit(); // RA2: as above
		DebugString("MapSelect: Invalid stage!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	scen->Set_Scenario_Name(""); // RA2: as above

	for (int index = 0; index < stage->Get_Selection_Count(); index++) { // RA2: as above
		MapSelection * selection = stage->Get_Selection(index); // RA2: as above

		if (selection != NULL) { // RA2: as above
			MapStage * sel_stage = Choices.Find_Stage_By_Name(selection->Get_Stage_Label()); // RA2: as above

			Wstring candidate(sel_stage->Get_Scenario_Name()); // RA2: the names are compared in lower case, and sel_stage is not tested before it is read
			candidate.toLower(); // RA2: the executable lower-cases both names and compares them with Wstring::operator== instead of stricmp
			Wstring requested(map_name); // RA2: a copy of the requested name, lower-cased below
			requested.toLower(); // RA2: the requested name is lower-cased in place as well
			if ((sel_stage != NULL) && candidate == requested) { // RA2: the comparison is Wstring::operator== on the lower-cased copies, not stricmp
				scen->Set_Scenario_Name(sel_stage->Get_Scenario_Name().text()); // RA2: the name is passed as a char pointer from the Wstring
				scen->Stage = (unsigned short)Choices.Get_Stage_ID(sel_stage); // RA2: the stage number is stored as a short (+0x2304)
				break; // RA2: as above
			}
		}
	}

	Choices.Deinit(); // RA2: as above
	return(strlen(scen->ScenarioName) > 0); // RA2: as above
}


/// <summary>
/// Picks the next map of the campaign without showing the selection screen.
/// RA2 plays the map movie's theme and takes the first selection of the stage the scenario
/// is at, rather than letting the player choose.
/// </summary>
/// <param name="scenario">The scenario to advance. It is updated with the map chosen.</param>
/// <returns>bool; Could the map choices be set up?</returns>
bool MapSelect::Set_Next_Scenario(ScenarioClass * scenario) // RA2: 0x0058F900
{
	if (Init(scenario) == false) { // RA2: as above
		return(false); // RA2: as above
	}

	ScenarioSaveNameSet = false; // RA2: 0x00A6EBD0; the save name is made again for the next map

	MapStage * stage; // RA2: the stage of the scenario's current map
	{
		Wstring name(scenario->ScenarioName); // RA2: the lookup by scenario name takes a Wstring temporary (0x005B08A0)
		stage = Choices.Find_Stage_By_Scenario(name); // RA2: the stage is found by the scenario's file name
	}

	DebugString("MapSelect: Stage: %s\n", *(char const * const *)&stage->Get_Stage_Label()); // RA2: the executable reads the label's string pointer in place without calling text(); Wstring's pointer is private and a new accessor in wstring.h unmatches wonline Draw_Player_List

	if (stage->Get_Map_VQ_Name() != NULL) { // RA2: only a stage with a map movie picks the next map
		Theme.Play_Song(Theme.From_Name("LOADING")); // RA2: the loading theme is played while the choices are worked out

		char const * selected = stage->Get_Selection(0)->Get_Stage_Label().text(); // RA2: the label of the stage's first selection is a char pointer into its Wstring (0x0076E730 text)
		if (selected != NULL) { // RA2: as above
			DebugString("Selected %s\n", selected); // RA2: as above

			{
				Wstring name(selected); // RA2: the label is looked up through a Wstring temporary (0x005B07F0)
				stage = Choices.Find_Stage_By_Name(name); // RA2: the stage the first selection names
			}

			Wstring scenario_name(stage->Get_Scenario_Name()); // RA2: a copy of the stage's scenario name
			if (scenario_name.length() != 0) { // RA2: a stage without a scenario leaves the scenario unchanged
				scenario->Set_Scenario_Name(scenario_name.get()); // RA2: the next map is the first selection's
				scenario->Stage = (unsigned short)Choices.Get_Stage_ID(stage); // RA2: as in Advance_Progression
			}
		}
	}

	Deinit(); // RA2: as above
	return(true); // RA2: as above
}


/// <summary>
/// Sets up the map choices and the font of the selection.
/// </summary>
/// <param name="scenario">The scenario whose side and stage select the choices.</param>
/// <returns>bool; Was everything created?</returns>
bool MapSelect::Init(ScenarioClass * scenario) // RA2: as above
{
	AnimDrawer = NULL; // RA2: as above
	OverlayDrawer = NULL; // RA2: as above
	Font = NULL; // RA2: as above

	if (scenario == NULL) { // RA2: as above
		DebugString("MapSelect: Invalid Scenario pointer!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	SideClass * side = Sides[HouseTypes[scenario->PlayerHouse]->Side]; // RA2: the choices belong to the side, not the house
	if (side == NULL) { // RA2: the side is checked, as in Advance_Progression
		DebugString("MapSelect: Invalid side!\n"); // RA2: the string is the executable's
		return(false); // RA2: as above
	}

	DebugString("MapSelect: Side %s\n", (const char*)side->IniName); // RA2: the side's name, as in Advance_Progression

	if (Choices.Initialize((const char*)side->IniName) == false) { // RA2: the side's name converts to a Wstring temporary that lives for the call
		Choices.Deinit(); // RA2: as above
		DebugString("MapSelect: Unable to initialize choices!\n"); // RA2: as above
		return(false); // RA2: as above
	}

	MapStage * map_stage = Choices.Find_Stage_By_Scenario(scenario->ScenarioName); // RA2: the stage is found by the scenario's name, not its number

	if (map_stage == NULL) { // RA2: as above
		DebugString("MapSelect: Invalid stage!\n"); // RA2: as above
		Deinit(); // RA2: as above
		return(false); // RA2: as above
	}

	Font = new MSFont(true); // RA2: no animation or overlay drawers and no click map are made; only the font (0x0058FA20)

	if (Font == NULL) { // RA2: as above
		DebugString("MapSelect: Unable to create font!\n"); // RA2: as above
		Deinit(); // RA2: as above
		return(false); // RA2: as above
	}

	XOffset = ((HiddenSurface->Get_Width() - 640) / 2); // RA2: as above
	YOffset = ((HiddenSurface->Get_Height() - 400) / 2); // RA2: as above

	TextRect = *Choices.Get_Text_Rect(); // RA2: as above
	TextRect.X += XOffset; // RA2: as above
	TextRect.Y += YOffset; // RA2: as above

	int i = 0; // RA2: as above

	do { // RA2: as above
		WideWstring description(map_stage->Get_Description()); // RA2: the wrap works on a copy
		MSPrintAnim::Word_Wrap(description, Font, TextRect.Width); // RA2: the wrapped result is not used; the call pre-wraps a copy of each stage's text
		i++; // RA2: as above
		map_stage = Choices.Find_Stage_By_ID((unsigned short)i); // RA2: as above
	} while (map_stage != NULL); // RA2: a do-while: the first stage is known to exist

	VoiceName = (char const *)NULL; // RA2: Wstring::operator=(char const *) with zero (0x0076DBC0)
	VoiceTimer = 0; // RA2: as above
	VoiceHandle = -1; // RA2: as above

	return(true); // RA2: as above
}


/// <summary>
/// Frees the resources the map selection allocated.
/// This routine may be called from a half finished initialization -- whatever was never
/// created is simply skipped over.
/// </summary>
void MapSelect::Deinit(void) // RA2: as above
{
	if (Font != NULL) { // RA2: as above
		delete Font; // RA2: as above
		Font = NULL; // RA2: as above
	}

	if (OverlayDrawer != NULL) { // RA2: as above
		delete OverlayDrawer; // RA2: as above
		OverlayDrawer = NULL; // RA2: as above
	}

	if (AnimDrawer != NULL) { // RA2: as above
		delete AnimDrawer; // RA2: as above
		AnimDrawer = NULL; // RA2: as above
	}

	Choices.Deinit(); // RA2: as above
}


/// <summary>
/// Performs the idle processing for the map selection.
/// RA2 no longer starts the queued voice over; only the timer is read while one is queued.
/// </summary>
void MapSelect::Process_Idle(void) // RA2: as above
{
	if (VoiceName != NULL) { // RA2: a queued voice only has its timer read (its Value test of the start tick is inlined); nothing is started
		VoiceTimer.Value(); // RA2: the result is not used
	}
}


/// <summary>
/// Plays one of the map selection sound effects.
/// </summary>
/// <param name="name">Name of the sound effect entry to play.</param>
void MapSelect::Play_Sound(Wstring const & name) // RA2: the name arrives as a Wstring (0x0058FD00)
{
	if (name != NULL) { // RA2: as above
		MSSfxEntry * sfx = Choices.Find_Sound(name); // RA2: as above

		if (sfx != NULL) { // RA2: as above
			sfx->Play(); // RA2: as above
		}
	}
}


/// <summary>
/// Queues a voice over to start after a delay.
/// </summary>
/// <param name="name">Name of the voice over sample.</param>
/// <param name="delay">The delay, in timer ticks.</param>
void MapSelect::Queue_Voice(Wstring const & name, int delay) // RA2: the name is kept as a Wstring; TibSun stopped a playing voice first, RA2 does not (0x0058FD30)
{
	VoiceName = name; // RA2: as above
	VoiceTimer = delay; // RA2: as above
}
