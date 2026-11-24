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
**	copies. The unused map input and voice methods keep their published no-op bodies (0x0058FCF0, 0x0058FD70,
**	0x0058FD80; ra2-decomp's mapsel.cpp).
*/

#include "always.h" // RA2: geometry.h follows the includes, as its constants take their initializers after those of the other headers
#include "mapsel.h"
#include "_surface.h"
#include "convert.h"
#include "dbgprint.h"
#include "globals.h"
#include "houstype.h"
#include "side.h" // RA2: the includes this file needs
#include "msanim.h"
#include "mschoice.h"
#include "msengine.h"
#include "msfont.h"
#include "msgbox.h"
#include "scenario.h"
#include "stimer.h"
#include "stringtable.h" // RA2: the includes this file needs
#include "surface.h"
#include "theme.h"
#include "timer.h"
#include "geometry.h" // RA2: this module holds its own copy of the geometry constants (initialized after MapSelectRect, from 0x0058F340)

#define RA2_SOURCE "D:\\Ra2\\MapSel.CPP" // RA2: the path the executable passes with each lookup



/// The binary confirms this was in the cpp as the vtable is inside this module
/// Moving it into a header would make the first thing that includes the header construct the vtable in that module

class MapSelect : public MSEngine {
	public:
		bool Set_Next_Scenario(ScenarioClass * scenario); // RA2: named after YR's SetNextScenario; TibSun's Presentation showed the map
		bool Advance_Progression(ScenarioClass * scenario, Wstring const & map_name); // RA2: the name arrives as a Wstring

	private:
		bool Init(ScenarioClass * scenario);
		void Deinit(void);

		void Play_Sound(Wstring const & name); // RA2: the name is a Wstring
		void Queue_Voice(Wstring const & name, int delay); // RA2: the voice name is a Wstring (0x0058FD30 assigns it with Wstring::operator=)
		void Start_Voice(Wstring const & name); // RA2: 0x0058FD70, empty
		void Stop_Voice(bool fade); // RA2: 0x0058FD80, empty
		const char * Process_Input(MapStage * stage); // RA2: 0x0058FCF0, no map input in RA2

		virtual void Process_Idle(void) override; // RA2: the third vtable slot of MapSelect is at 0x0058FCC0

	private:
		int XOffset;
		int YOffset;

		MapChoice Choices;

		ConvertClass * AnimDrawer;
		ConvertClass * OverlayDrawer;

		MSFont * Font;
		Rect TextRect;

		Wstring VoiceName; // RA2: a Wstring (0x0058FD30 assigns it with Wstring::operator=)
		CDTimerClass<SystemTimerClass> VoiceTimer;
		int VoiceHandle;
};


/// <summary>
/// Starts the whole process of selecting the next map to go to.
/// </summary>
/// <param name="scen">The scenario to advance. It is updated with the map chosen.</param>
/// <returns>Returns with the name of the next scenario, or NULL if there is none.</returns>
const char * Map_Selection(ScenarioClass * scen)
{
	if (MapSelect().Set_Next_Scenario(scen) == false) { // RA2: the temporary is built and destroyed inline; the message box below is reached on failure
		WWMessageBox(L" ").Process(StringTable::LoadString("MSG:UnableMapSelect", NULL, RA2_SOURCE, 134), StringTable::LoadString("TXT_OK", NULL, RA2_SOURCE, 134), NULL, NULL, false); // RA2: the message box takes wide text
		return(NULL);
	}

	DebugString("MapSelect: Scenario - %s, Stage - %ld\n", scen->ScenarioName, scen->Stage);

	return((strlen(scen->ScenarioName) == 0) ? NULL : scen->ScenarioName);
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
const char * Map_Select_Advance(ScenarioClass * scenario, const char * map_name)
{
	if (MapSelect().Advance_Progression(scenario, map_name) == false) {
		WWMessageBox(L" ").Process(StringTable::LoadString("MSG:UnableMapSelect", NULL, RA2_SOURCE, 152), StringTable::LoadString("TXT_OK", NULL, RA2_SOURCE, 152), NULL, NULL, false); // RA2: the same message box as Map_Selection, with the line number 152 of the source
		return(NULL);
	}

	return(map_name);
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
	if ((scen == NULL) || (map_name == NULL)) {
		return(false);
	}

	SideClass * side = Sides[HouseTypes[scen->PlayerHouse]->Side]; // RA2: the choices belong to the side, not the house
	if (side == NULL) { // RA2: the executable checks the side (string at 0x007E1C44), TibSun checked the house
		DebugString("MapSelect: Invalid side!\n"); // RA2: the string is the executable's
		return(false);
	}

	DebugString("MapSelect: Side %s\n", (const char*)side->IniName); // RA2: the string is the executable's, the side's INI name is printed

	bool failed; // RA2: the result of the initialization is kept until the Wstring temporary is gone
	{
		Wstring side_name((const char*)side->IniName); // RA2: the temporary is destroyed before the failure test
		failed = (Choices.Initialize(side_name) == false); // RA2: Initialize takes the side's name as a Wstring (0x005B0030)
	}
	if (failed) { // RA2: the failure test follows the end of the temporary's scope
		Choices.Deinit();
		DebugString("MapSelect: Unable to initialize choices!\n");
		return(false);
	}

	MapStage * stage = Choices.Find_Stage_By_ID(scen->Stage);

	if (stage == NULL) {
		Choices.Deinit();
		DebugString("MapSelect: Invalid stage!\n");
		return(false);
	}

	scen->Set_Scenario_Name("");

	for (int index = 0; index < stage->Get_Selection_Count(); index++) {
		MapSelection * selection = stage->Get_Selection(index);

		if (selection != NULL) {
			MapStage * sel_stage = Choices.Find_Stage_By_Name(selection->Get_Stage_Label());

			Wstring candidate(sel_stage->Get_Scenario_Name()); // RA2: the names are compared in lower case, and sel_stage is not tested before it is read
			candidate.toLower(); // RA2: the executable lower-cases both names and compares them with Wstring::operator== instead of stricmp
			Wstring requested(map_name); // RA2: a copy of the requested name, lower-cased below
			requested.toLower(); // RA2: the requested name is lower-cased in place as well
			if ((sel_stage != NULL) && candidate == requested) { // RA2: the comparison is Wstring::operator== on the lower-cased copies, not stricmp
				scen->Set_Scenario_Name(sel_stage->Get_Scenario_Name().text()); // RA2: the name is passed as a char pointer from the Wstring
				scen->Stage = (unsigned short)Choices.Get_Stage_ID(sel_stage); // RA2: the stage number is stored as a short (+0x2304)
				break;
			}
		}
	}

	Choices.Deinit();
	return(strlen(scen->ScenarioName) > 0);
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
	if (Init(scenario) == false) {
		return(false);
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
		if (selected != NULL) {
			DebugString("Selected %s\n", selected);

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

	Deinit();
	return(true);
}


/// <summary>
/// Sets up the map choices and the font of the selection.
/// </summary>
/// <param name="scenario">The scenario whose side and stage select the choices.</param>
/// <returns>bool; Was everything created?</returns>
bool MapSelect::Init(ScenarioClass * scenario)
{
	AnimDrawer = NULL;
	OverlayDrawer = NULL;
	Font = NULL;

	if (scenario == NULL) {
		DebugString("MapSelect: Invalid Scenario pointer!\n");
		return(false);
	}

	SideClass * side = Sides[HouseTypes[scenario->PlayerHouse]->Side]; // RA2: the choices belong to the side, not the house
	if (side == NULL) { // RA2: the side is checked, as in Advance_Progression
		DebugString("MapSelect: Invalid side!\n"); // RA2: the string is the executable's
		return(false);
	}

	DebugString("MapSelect: Side %s\n", (const char*)side->IniName); // RA2: the side's name, as in Advance_Progression

	if (Choices.Initialize((const char*)side->IniName) == false) { // RA2: the side's name converts to a Wstring temporary that lives for the call
		Choices.Deinit();
		DebugString("MapSelect: Unable to initialize choices!\n");
		return(false);
	}

	MapStage * map_stage = Choices.Find_Stage_By_Scenario(scenario->ScenarioName); // RA2: the stage is found by the scenario's name, not its number

	if (map_stage == NULL) {
		DebugString("MapSelect: Invalid stage!\n");
		Deinit();
		return(false);
	}

	Font = new MSFont(true); // RA2: no animation or overlay drawers and no click map are made; only the font (0x0058FA20)

	if (Font == NULL) {
		DebugString("MapSelect: Unable to create font!\n");
		Deinit();
		return(false);
	}

	XOffset = ((HiddenSurface->Get_Width() - 640) / 2);
	YOffset = ((HiddenSurface->Get_Height() - 400) / 2);

	TextRect = *Choices.Get_Text_Rect();
	TextRect.X += XOffset;
	TextRect.Y += YOffset;

	int i = 0;

	do {
		WideWstring description(map_stage->Get_Description()); // RA2: the wrap works on a copy
		MSPrintAnim::Word_Wrap(description, Font, TextRect.Width); // RA2: the wrapped result is not used; the call pre-wraps a copy of each stage's text
		i++;
		map_stage = Choices.Find_Stage_By_ID((unsigned short)i);
	} while (map_stage != NULL); // RA2: a do-while: the first stage is known to exist

	VoiceName = (char const *)NULL; // RA2: Wstring::operator=(char const *) with zero (0x0076DBC0)
	VoiceTimer = 0;
	VoiceHandle = -1;

	return(true);
}


/// <summary>
/// Frees the resources the map selection allocated.
/// This routine may be called from a half finished initialization -- whatever was never
/// created is simply skipped over.
/// </summary>
void MapSelect::Deinit(void)
{
	if (Font != NULL) {
		delete Font;
		Font = NULL;
	}

	if (OverlayDrawer != NULL) {
		delete OverlayDrawer;
		OverlayDrawer = NULL;
	}

	if (AnimDrawer != NULL) {
		delete AnimDrawer;
		AnimDrawer = NULL;
	}

	Choices.Deinit();
}


/// <summary>
/// Performs the idle processing for the map selection.
/// RA2 no longer starts the queued voice over; only the timer is read while one is queued.
/// </summary>
void MapSelect::Process_Idle(void)
{
	if (VoiceName != NULL) { // RA2: a queued voice only has its timer read (its Value test of the start tick is inlined); nothing is started
		VoiceTimer.Value(); // RA2: the result is not used
	}
}


/// <summary>
/// No interactive map input is processed in RA2.
/// </summary>
const char * MapSelect::Process_Input(MapStage * stage)
{
	return(NULL);
}


/// <summary>
/// Plays one of the map selection sound effects.
/// </summary>
/// <param name="name">Name of the sound effect entry to play.</param>
void MapSelect::Play_Sound(Wstring const & name) // RA2: the name arrives as a Wstring (0x0058FD00)
{
	if (name != NULL) {
		MSSfxEntry * sfx = Choices.Find_Sound(name);

		if (sfx != NULL) {
			sfx->Play();
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
	VoiceName = name;
	VoiceTimer = delay;
}


void MapSelect::Start_Voice(Wstring const & name)
{
}


void MapSelect::Stop_Voice(bool fade)
{
}
