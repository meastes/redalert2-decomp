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
#include "language\language.h"
#include "bitfont.h" // RA2: the includes this file needs

#include "_keyboar.h"
#include "_surface.h"
#include "ccfile.h"
#include "convert.h"
#include "data.h"
#include "dsurface.h" // RA2: the font color is a hicolor pixel
#include "globals.h"
#include "ini.h"
#include "keyboard.h"
#include "mschoice.h"
#include "msfont.h"
#include "newmenu.h"
#include "ownrdraw.h"
#include "pcx.h"
#include "session.h"
#include "stringtable.h" // RA2: the texts come from the string table
#include "theme.h"
#include "vector.h"
#include "wdtnet.h"
#include "wonline.h" // RA2: WOL_Wait_Callback
#include "worlddom.h"
#include "wstring.h" // RA2: the includes this file needs

using namespace WorldDominationTour;

rc_ptr<WDTTerritory> Find_WDT_Territory(int index); // RA2: 0x00769690, the tour's territory with this index (not in this module)
WideWstring __fastcall Int_To_WideString_Width(int value, int width); // RA2: 0x00771130, the number as text, padded to the width
WideWstring __fastcall operator+(wchar_t const * left, WideWstring const & right); // RA2: the text and the territory's name joined


inline TERRITORY_ITER Walk_Territories(TERRITORY_LIST & list) { return(TERRITORY_ITER(list, 0)); } // RA2: the cursor over the map's territories is made one inline level down


/// <summary>
/// Creates the world domination territory selection screen.
/// This routine assembles the whole screen out of the map database -- the voice overs, the
/// sound effects, the layout, the artwork and the region currently being fought over -- and
/// then sets it running.
/// </summary>
/// <param name="campaign">The campaign whose current state the screen presents.</param>
/// <param name="faction">The faction whose progress the screen shows.</param>
Selection::Selection(rc_ptr<Campaign> const & campaign, int faction) : // RA2: counted campaign, the player's faction picks the voices and the map is made by Create
	TourCampaign(campaign), // RA2: as above
	VoicePlayer(campaign->Get_Player_Faction()), // RA2: as above
	Brokeout(false), // RA2: as above
	LastPoint(-1, -1), // RA2: as above
	StartTick(campaign->Properties->Get_Current_Tick()), // RA2: as above
	LastHoverZone(-1), // RA2: as above
	WorldMap(Map::Create(campaign->Properties->Get_Map_ID(), faction, campaign->Get_Player_Faction())), // RA2: as above
	HoveredTerritoryID(-1), // RA2: as above
	MousedTerritory(), // RA2: the empty counted pointer is made by its constructor between the territory id and the target drawer
	TargetDrawer(NULL), // RA2: as above
	ZoomingTarget(NULL), // RA2: as above
	ThrobbingTarget(NULL), // RA2: as above
	ThrobbingTargetDividingFrame(32), // RA2: as above
	TextAnim(NULL), // RA2: as above
	TooltipAnim(NULL), // RA2: as above
	Font(new MSFont(false)), // RA2: as above
	CurrentState(State::Create()), // RA2: the empty state
	ThemeName(NULL), // RA2: as above
	SfxEntries(0, 0) // RA2: the sound list is made empty by its constructor arguments
{
	int color = Text_Color; // RA2: the font color is put back by the destructor
	int pixel = DSurface::Build_Hicolor_Pixel((unsigned char)color, (unsigned char)((unsigned short)color >> 8), (unsigned char)(color >> 16)); // RA2: as above
	TextColor = BitFont::Instance->Set_Color(pixel) & 0xFFFF; // RA2: as above

	Add_Animation(VoicePlayer.Create_Anim()); // RA2: the voice player is run by the engine

	CCFileClass file("WDTMAP.INI");
	INIClass ini;
	ini.Load(file);

	Init_Voices(ini, "VoiceOvers");
	Init_Sounds(ini, "SOUNDS");

	Wstring layout; // RA2: the layout section name is a Wstring
	ini.Get_String("LAYOUTS", Wstring_FromInt(campaign->Properties->Get_Layout()), "", layout); // RA2: as above
	Init_Target(ini, layout); // RA2: as above

	WorldMap->Load(ini, this, campaign->Is_Different(faction)); // RA2: the map reads the rest of the layout

	WideWstring play_text(StringTable::LoadString("GUI:PlayWDT", NULL, "D:\\Ra2\\WDTSel.cpp", 157)); // RA2: the play button's text
	MSBitPrintAnim * play = new MSBitPrintAnim(play_text, WorldMap->PlayRect, true); // RA2: as above
	play->Set_Flag(true); // RA2: as above
	Add_Animation(play); // RA2: as above

	WideWstring back_text(StringTable::LoadString("TXT_BACK", NULL, "D:\\Ra2\\WDTSel.cpp", 164)); // RA2: the back button's text
	MSBitPrintAnim * back = new MSBitPrintAnim(back_text, WorldMap->BackRect, true); // RA2: as above
	back->Set_Flag(true); // RA2: as above
	Add_Animation(back); // RA2: as above

	Start();
}


/// <summary>
/// Shuts the selection screen down and releases everything it owns.
/// </summary>
Selection::~Selection(void)
{
	End();

	MousedTerritory = NULL;

	Remove_Target_Anims();
	Remove_Target_Anims2();

	for (SFX_ITER sfx(SfxEntries, 0); sfx; sfx++) { // RA2: the cursor over the sound entries is made with an explicit start index
		delete *sfx; // RA2: as above
	}

	delete TargetDrawer;
	delete ZoomingTarget;
	delete ThrobbingTarget;
	delete Font;

	BitFont::Instance->Set_Color((unsigned short)TextColor); // RA2: the font color saved by the constructor is put back
}


/// <summary>
/// Fetches the voice over settings for the selection screen.
/// Along with the voice list itself, this routine picks up the score thresholds that
/// decide how enthusiastic the announcer gets about the campaign so far.
/// </summary>
/// <param name="ini">The map database to fetch the voice overs from.</param>
/// <param name="section">The section that describes the voice overs.</param>
void Selection::Init_Voices(INIClass & ini, Wstring const & section) // RA2: the INI and the section are the screen's own string types
{
	VoicePlayer.Load(ini, section); // RA2: RA2 voice player loads its lists from the section
	WinningThreshold = ini.Get_Int(section, "WinningThreshold", WinningThreshold);
	MegaWinningThreshold = ini.Get_Int(section, "MegaWinningThreshold", MegaWinningThreshold);
	LosingThreshold = ini.Get_Int(section, "LosingThreshold", LosingThreshold);
	MegaLosingThreshold = ini.Get_Int(section, "MegaLosingThreshold", MegaLosingThreshold);
}

/// <summary>
/// Creates the sound effect table for the selection screen.
/// Every entry in the section becomes a named sound effect that the screen can ask for by
/// name later on. The music theme for the screen is chosen here as well.
/// </summary>
/// <param name="ini">The map database to fetch the sound list from.</param>
/// <param name="section">The section that lists the sound effects.</param>

void Selection::Init_Sounds(INIClass & ini, Wstring const & section) // RA2: takes the INI and the section as RA2 string types
{
	Wstring value; // RA2: RA2 reads the entries as Wstring values; a "Theme" entry names the music
	int count = ini.Entry_Count(section);
	for (int i = 0; i < count; i++) {
		Wstring entry = ini.Get_Entry(section, i); // RA2: RA2 reads the entries as Wstring values; a "Theme" entry names the music
		if (entry != 0 && ini.Get_String(section, entry, NULL, value) > 0) { // RA2: as above
			if (entry == "Theme") { // RA2: the "Theme" entry names the music instead of a sound
				ThemeName = value; // RA2: as above
				break;
			}
			MSSfxEntry * sfx = new MSSfxEntry(entry, value); // RA2: RA2 reads the entries as Wstring values; a "Theme" entry names the music
			if (sfx->Get_Name() == 0) { // RA2: as above
				delete sfx;
			} else if (sfx != NULL) { // RA2: RA2 reads the entries as Wstring values; a "Theme" entry names the music
				SfxEntries.Add(sfx); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Fetches the screen layout for the selection screen.
/// The screen is centered within the display and the map, the text panel and the cancel
/// button are then positioned relative to it.
/// </summary>
/// <param name="ini">The map database to fetch the layout from.</param>
/// <param name="section">The section that describes this screen layout.</param>
void Selection::Init_Target(INIClass & ini, Wstring const & section) // RA2: the target artwork of the player's faction
{
	Wstring value; // RA2: RA2 reads the entries as Wstring values; a "Theme" entry names the music
	Wstring faction(Faction_Name(TourCampaign->Get_Player_Faction())); // RA2: as above
	if (ini.Get_String(section, faction + "ZoomingTarget", NULL, value) > 0) { // RA2: as above
		ZoomingTarget = new MSAnimEntry(value); // RA2: as above
	}
	ini.Get_String(section, faction + "TargetPalette", NULL, value); // RA2: RA2 reads the palette name into the same Wstring
	TargetDrawer = Create_Drawer(value); // RA2: as above
	ini.Get_String(section, faction + "Target", NULL, value); // RA2: as above
	ThrobbingTarget = new MSAnimEntry(value); // RA2: as above
	ThrobbingTargetDividingFrame = ini.Get_Int(section, faction + "TargetDividingFrame", 32); // RA2: as above
}


/// <summary>
/// Creates the campaign map and everything that is drawn over it.
/// This routine brings in the region backdrop, the click map that turns a mouse position
/// into a territory, the territories themselves and the cancel button artwork -- all of it
/// chosen to suit the player's faction.
/// </summary>
/// <param name="ini">The map database to fetch the region artwork from.</param>
/// <param name="section">The section that describes this region.</param>
/// <param name="vq_anim">Should the backdrop play as a movie rather than a still image?</param>
rc_ptr<Territory> Selection::Pick_Territory(void) // RA2: the screen returns a counted pointer, empty when the player backs out
{
	Set_Long_Description(); // RA2: the long description is set by the inline Set_Long_Description
	bool canceled = false;
	bool selected = false;

	Keyboard->Clear();

	do { // RA2: RA2 runs the loop until the player picks a territory or backs out
		WOL_Wait_Callback(); // RA2: as above
		Wait_For_Focus();
		Point2D point(MouseCursor->Get_Mouse_X(), MouseCursor->Get_Mouse_Y());

		if (Keyboard->Check()) { // RA2: any key waiting
			int key = Keyboard->Get();
			if (key == KN_LMOUSE) { // RA2: the press
				Handle_Click(point, selected);
			} else if (key == (KN_LMOUSE | WWKEY_RLS_BIT)) { // RA2: the release
				WorldMap->Set_Back_Hot(this, false); // RA2: as above
				if (Region_At(point) == 2) { // RA2: as above
					Play_SFX("Canceled");
					canceled = true;
				}
			}

		/*
		 * The hover update only runs on frames with no pending keyboard event:
		 * every path through the event handling above jumps past this block.
		 */
		} else if (point != LastPoint) {
			LastPoint = point;
			int zone = Region_At(point); // RA2: the back button region and state of the map (an exact copy of the executable)

			if (LastHoverZone != zone) {
				switch (LastHoverZone) {
					case 1:
						On_Map_Leave();
						break;

					case 2:
						On_Cancel_Leave();
						break;
				}
			}

			switch (zone) {
				case 1:
					if (LastHoverZone != 1) {
						On_Map_Enter();
					}
					Update_Hovered_Territory(point);
					break;

				case 2:
					if (LastHoverZone != 2) {
						On_Cancel_Enter();
					}
					break;
			}

			LastHoverZone = zone;
		}

		Wait_Delay(1);
	} while (!canceled && !selected); // RA2: RA2 runs the loop until the player picks a territory or backs out

	return(selected ? MousedTerritory : rc_ptr<Territory>()); // RA2: as above
}


/// <summary>
/// Handles a mouse click on the territory selection screen.
/// This routine routes the click either to the cancel button or to the campaign map,
/// where it becomes an attempt to pick a territory.
/// </summary>
/// <param name="point">The screen position that was clicked.</param>
/// <param name="selected">Set to true if the click landed on a selectable territory.</param>
#pragma auto_inline(off) // RA2: Pick_Territory calls it out of line
void Selection::Handle_Click(Point2D const & point, bool & selected) // RA2: a press on the back button or on the map
{
	switch (Region_At(point)) { // RA2: the back button region and state of the map (an exact copy of the executable)
		case 2:
			WorldMap->Set_Back_Hot(this, true); // RA2: the back button region and state of the map (an exact copy of the executable)
			On_Cancel_Enter();
			break;

		case 1:
			WorldMap->Set_Back_Hot(this, false); // RA2: the back button region and state of the map (an exact copy of the executable)
			selected = Hover_At(point); // RA2: as above
			if (selected) {
				Play_SFX("ConflictSelected");
			}
			break;
	}
}
#pragma auto_inline(on) // RA2: as above


bool Selection::Select_Territory_At(Point2D const & point) // RA2: refreshes the hovered territory, then selects it
{
	Update_Hovered_Territory(point);
	rc_ptr<Territory> territory = true ? WorldMap->Get_Territory(HoveredTerritoryID) : rc_ptr<Territory>(); // RA2: the dead condition is in the executable: a zeroed flag for the temporary and the copy through the default constructor and Set
	return(Select_Territory(territory));
}


void Selection::Update_Hovered_Territory(Point2D const & point) // RA2: the click map holds territory numbers
{
	int id = WorldMap->Territory_At(point); // RA2: the map keeps its territories as counted pointers
	if (id != HoveredTerritoryID) { // RA2: as above
		HoveredTerritoryID = id; // RA2: as above
		Select_Territory(WorldMap->Get_Territory(id)); // RA2: as above
	}
}


/// <summary>
/// Selects whichever territory lies beneath the point.
/// </summary>
/// <param name="point">The point to look up, relative to the campaign map.</param>
/// <returns>bool; Is there a conflict waiting on the territory selected?</returns>
void Selection::Do_Target_Anim(rc_ptr<State> const & state, bool show_targets, bool silent) // RA2: counted state; no sound effects, the owner changes are shown unless Escape was pressed (and the player has not asked for silence)
{
	Remove_Target_Anims();

	MS_ANIM_LIST zoom_anims;
	MS_ANIM_LIST throb_anims;

	/*
	 * Walk each territory and advance any whose state has changed, playing the
	 * matching occupation sound effect. When requested, queue the zoom/throb
	 * target animations for newly disputed territories.
	 */
	for (TERRITORY_ITER it = Walk_Territories(WorldMap->Territories); it; it++) { // RA2: the map keeps its territories as counted pointers
		int id = (*it)->Get_ID(); // RA2: as above
		int old_state = CurrentState->Owner_Of(id); // RA2: as above
		int new_state = state->Owner_Of(id); // RA2: as above

		if (new_state != old_state) {
			Check_For_Breakout();
			(*it)->Set_Owner_State(new_state, this, !Brokeout || silent); // RA2: the map keeps its territories as counted pointers
		}

		if (show_targets && new_state == 1) { // RA2: the zoom and throb animations are made only when their files exist
			Point2D target = WorldMap->Origin + (*it)->Get_Target(); // RA2: as above
			int x = target.X; // RA2: as above
			int y = target.Y; // RA2: as above

			if (ZoomingTarget != NULL) {
				CCFileClass file(ZoomingTarget->Get_Filename()); // RA2: the zoom and throb animations are made only when their files exist
				if (file.Is_Available()) { // RA2: as above
					MSFadeAnim * fade = new MSFadeAnim(ZoomingTarget->Get_Filename(), x, y, TargetDrawer, 3, SHAPE_CENTER, &Anims); // RA2: as above
					zoom_anims.Add(fade);
				}
			}

			CCFileClass file(ThrobbingTarget->Get_Filename()); // RA2: the zoom and throb animations are made only when their files exist
			if (file.Is_Available()) { // RA2: as above
				MSShapeAnim * shape = new MSShapeAnim(ThrobbingTarget->Get_Filename(), x, y, TargetDrawer, 5, true, SHAPE_CENTER); // RA2: as above
				shape->Set_Stop_Frame(ThrobbingTargetDividingFrame - 1);
				shape->Set_Start_Frame(0);
				shape->Set_Frame(0);
				(*it)->Set_Target_Anim(shape); // RA2: the map keeps its territories as counted pointers
				throb_anims.Add(shape);
			}
		}
	}

	/*
	 * Play the queued target animations. On abort, discard the zoom animations;
	 * otherwise play the zoom SFX and wait for each to complete.
	 */
	MS_ANIM_ITER zoom_iter(zoom_anims, 0); // RA2: the zoom and throb animations are made only when their files exist
	for (MS_ANIM_ITER throb_iter(throb_anims, 0); throb_iter; throb_iter++) { // RA2: as above
		Check_For_Breakout();

		if (zoom_iter) {
			if (Brokeout) {
				MSAnim * anim = *zoom_iter; // RA2: the zoom and throb animations are made only when their files exist
				delete anim;
			} else {
				Play_SFX("TargetZoom");
				Add_Animation(*zoom_iter); // RA2: the zoom and throb animations are made only when their files exist
				Wait_For_Anim(*zoom_iter, 300000); // RA2: as above
			}
			zoom_iter++;
		}

		Add_Animation(*throb_iter); // RA2: the zoom and throb animations are made only when their files exist
	}

	CurrentState = state;
}


/// <summary>
/// Makes a territory the one under the player's attention.
/// This routine takes the highlight and the pending conflict away from whichever
/// territory held them, highlights the new one, and replaces the screen caption with
/// its name, description, and game options. Passing NULL merely clears the selection.
/// </summary>
/// <param name="territory">The territory to select, or NULL to select none at all.</param>
/// <returns>bool; Is there a conflict waiting on the selected territory?</returns>
bool Selection::Select_Territory(rc_ptr<Territory> const & territory) // RA2: counted pointers; the conflict is a counted pointer too
{
	bool was_set = SelectedConflict.Get() != NULL; // RA2: the conflict and the territory are counted pointers

	if (territory.Get() == MousedTerritory.Get()) { // RA2: as above
		return(was_set);
	}

	if (MousedTerritory.Get() != NULL) { // RA2: the conflict and the territory are counted pointers
		if (SelectedConflict.Get() != NULL) { // RA2: as above
			SelectedConflict = NULL; // RA2: as above
		}
		MousedTerritory->Set_Highlight(false, this); // RA2: the conflict and the territory are counted pointers
		MSShapeAnim * anim = (MSShapeAnim *)MousedTerritory->Get_Target_Anim(); // RA2: as above
		if (anim != NULL) {
			anim->Set_Stop_Frame(ThrobbingTargetDividingFrame - 1);
			anim->Set_Start_Frame(0);
			anim->Set_Frame(0);
		}
	}

	MousedTerritory = territory;

	if (MousedTerritory.Get() == NULL) { // RA2: the conflict and the territory are counted pointers
		Set_Long_Description(); // RA2: as above
		VoicePlayer.Say_List(1, false, 30); // RA2: as above
		return(was_set);
	}

	bool found = false;
	WideWstring description; // RA2: the texts are wide strings
	WideWstring options; // RA2: as above

	int id = MousedTerritory->Get_ID(); // RA2: as above
	int territory_state = CurrentState->Owner_Of(id); // RA2: as above
	int outcome = OUTCOME_DRAW; // RA2: as above
	if (territory_state != 1 && territory_state != 0) { // RA2: as above
		outcome = TourCampaign->Get_Player_Faction() != territory_state; // RA2: as above
	}
	VoicePlayer.Say(Voices::VOICECAT_TERRITORY, outcome, false, false, 30); // RA2: RA2 voice player: group, result, emphasis, counted, delay

	switch (territory_state) { // RA2: as above
		case 1:
			Set_Tooltip(StringTable::LoadString("WDT:TerritorySelDispToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2b5)); // RA2: RA2 string table text (label, source path and line as the executable passes them)
			break;

		case 2:
			Set_Tooltip(StringTable::LoadString("WDT:AlliedHeldTooltip", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2ad)); // RA2: RA2 string table text (label, source path and line as the executable passes them)
			break;

		case 3:
			Set_Tooltip(StringTable::LoadString("WDT:SovietHeldTooltip", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2b1)); // RA2: RA2 string table text (label, source path and line as the executable passes them)
			break;

		default: // RA2: the tooltip when the territory is held by nobody
			Set_Tooltip(StringTable::LoadString("WDT:TerritorySelectToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2b9)); // RA2: as above
			break;
	}

	MousedTerritory->Set_Highlight(true, this); // RA2: the conflict and the territory are counted pointers

	if (TourCampaign->Properties->Get_Current_Tick() == StartTick) { // RA2: as above
		CONFLICT_ITER it(TourCampaign->Conflicts, 0); // RA2: as above
		while (it) { // RA2: as above
			rc_ptr<Conflict> const & conflict = *it; // RA2: as above
			Conflict const * current = conflict.operator->(); // RA2: as above
			int id = MousedTerritory->Get_ID(); // RA2: as above
			if (current->Is_Territory(id)) { // RA2: as above
				break;
			}
			it++; // RA2: the cursor over the counted conflicts steps on
		}

		if (it) {
			found = true;
			SelectedConflict = *it; // RA2: the conflict and the territory are counted pointers
			description = (*it)->Describe_Options(); // RA2: as above
			options = description; // RA2: as above
			MSShapeAnim * anim = (MSShapeAnim *)territory->Get_Target_Anim(); // RA2: as above
			if (anim != NULL) {
				anim->Set_Stop_Frame(-1);
				anim->Set_Start_Frame(ThrobbingTargetDividingFrame);
				anim->Set_Frame(ThrobbingTargetDividingFrame);
			}
		}
	}

	WideWstring text = StringTable::LoadString("WDT:TerritoryPrefix", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2d7) + MousedTerritory->Name + StringTable::LoadString("WDT:TerritorySuffix", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2d7); // RA2: the conflict and the territory are counted pointers
	if (options.length() > 0) { // RA2: as above
		text += description; // RA2: as above
	}
	Set_Text(text, 0); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	return(found);
}


/// <summary>
/// Removes the target marker animation from every territory on the map.
/// </summary>
void Selection::Remove_Target_Anims(void)
{
	for (TERRITORY_ITER it(WorldMap->Territories, 0); it; ++it) { // RA2: the map keeps its territories as counted pointers
		MSAnim * anim = (*it)->Remove_Target_Anim(); // RA2: as above
		if (anim != NULL) {
			Remove_Anim(anim);
		}
	}
}


/// An exact duplicate of Remove_Target_Anims. Both are called by the destructor.

/// <summary>
/// Removes the target marker animation from every territory on the map.
/// </summary>
void Selection::Remove_Target_Anims2(void)
{
	for (TERRITORY_ITER it(WorldMap.Get()->Territories, 0); it; ++it) { // RA2: the map is reached through the plain getter here: its operator-> expanded inline in the destructor only so (the executable has all of the destructor's calls expanded)
		MSAnim * anim = (*it)->Remove_Target_Anim(); // RA2: as above
		if (anim != NULL) {
			Remove_Anim(anim);
		}
	}
}


/// <summary>
/// Handles the mouse entering the campaign map.
/// </summary>
void Selection::On_Map_Enter(void)
{
	Play_SFX("MapEnter");
}


/// <summary>
/// Handles the mouse leaving the campaign map.
/// This routine drops the territory highlight along with the caption that went with it.
/// </summary>
void Selection::On_Map_Leave(void)
{
	Play_SFX("MapLeave");
	HoveredTerritoryID = -1;
	rc_ptr<Territory> none; // RA2: an empty counted pointer
	Select_Territory(none); // RA2: as above
}


/// <summary>
/// Handles the mouse entering the cancel button.
/// This routine swaps the button over to its highlighted animation, repaints it, and
/// plays the accompanying sound effect.
/// </summary>
void Selection::On_Cancel_Enter(void) // RA2: the back button of the map
{
	if (!WorldMap->BackButton.IsDown) { // RA2: the back button of the map is down
		WorldMap->Set_Back_Down(this, true); // RA2: as above
		Play_SFX("MouseCancel");
		Set_Tooltip(StringTable::LoadString("WOL:WDTTerritoryBack", NULL, "D:\\Ra2\\WDTSel.cpp", 894)); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	}
}


/// <summary>
/// Handles the mouse leaving the cancel button.
/// This routine puts the button back to its idle animation and repaints it.
/// </summary>
void Selection::On_Cancel_Leave(void)
{
	if (WorldMap->BackButton.IsDown) { // RA2: the back button of the map is down
		WorldMap->Set_Back_Down(this, false); // RA2: as above
		Set_Tooltip(StringTable::LoadString("WDT:TerritorySelectToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 916)); // RA2: as above
	}
}

/// <summary>
/// Brings the campaign map up to date and marks the available targets.
/// This routine runs before the player is allowed to choose. It replays whatever has
/// happened since the last visit, plants the target markers on the territories that are
/// up for grabs, and writes the campaign back out again.
/// </summary>

void Selection::Do_Target_Selection(int faction) // RA2: replays the days since the player's last visit, then marks the targets and saves the progress
{
	Set_Text(NULL, 0); // RA2: RA2 string table text (label, source path and line as the executable passes them)

	int tick = TourCampaign->Properties->Get_Current_Tick(); // RA2: as above
	bool won = true; // RA2: as above
	int last_day = TourCampaign->Get_Last_Day(faction); // RA2: as above
	if (tick - last_day > 8) { // RA2: as above
		last_day = tick - 8; // RA2: as above
	}

	if (last_day != tick) { // RA2: the campaign history is read tick by tick (RA2 form)
		won = !Present_History(last_day, tick); // RA2: as above
	} else {
		bool mega = false; // RA2: the campaign history is read tick by tick (RA2 form)
		int outcome = Get_Outcome(tick, mega); // RA2: as above
		VoicePlayer.Say(Voices::VOICECAT_STATUS, outcome, mega, true, 0); // RA2: as above
	}

	Set_Text(NULL, 0); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	Check_For_Breakout();
	Do_Target_Anim(TourCampaign->CycleHistory->Get_State(tick), true, won); // RA2: the campaign history is read tick by tick (RA2 form)
	TourCampaign->Save_Progress(faction); // RA2: as above
}


/// <summary>
/// Determines how well the player stands at a given campaign cycle.
/// This routine is used to pick the commentary and the caption that match the player's
/// share of the map.
/// </summary>
/// <param name="tick">The campaign cycle to judge.</param>
/// <param name="mega">Set to true when the standing is lopsided enough to deserve the
/// emphatic commentary rather than the ordinary one.</param>
/// <returns>Returns with the outcome -- win, lose, or draw.</returns>
int Selection::Get_Outcome(int tick, bool & mega) // RA2: the outcome of a tick, from the territories the player's side owns
{
	rc_ptr<State> state = TourCampaign->CycleHistory->Get_State(tick); // RA2: the campaign history is read tick by tick (RA2 form)
	if (state.Get() != NULL) { // RA2: as above
		int owned = state->Count_Owned(TourCampaign->PlayerFaction); // RA2: as above
		if (owned <= LosingThreshold) {
			mega = owned <= MegaLosingThreshold;
			return(OUTCOME_LOSE);
		}
		if (owned >= WinningThreshold) {
			mega = owned >= MegaWinningThreshold;
			return(OUTCOME_WIN);
		}
	}
	mega = false;
	return(OUTCOME_DRAW);
}


/// <summary>
/// Animates the campaign map across a range of cycles.
/// This routine steps the map from one cycle to the next, captioning each with the day
/// it stands for and pausing between them. The player may cut the sequence short at any
/// point.
/// </summary>
/// <param name="tick_from">The campaign cycle to start the animation from.</param>
/// <param name="tick_to">The campaign cycle to run the animation up to.</param>
/// <returns>bool; Did the sequence end before a single cycle could be presented?</returns>
bool Selection::Present_Ticks(int tick_from, int tick_to) // RA2: shows each day in turn; true when nothing was shown
{
	bool first = true;
	bool first_state = true; // RA2: the campaign history is read tick by tick (RA2 form)
	unsigned int length = 0; // RA2: as above
	for (int tick = tick_from; tick <= tick_to; tick++) { // RA2: as above
		Check_For_Breakout();
		if (Brokeout) {
			break;
		}

		WideWstring text(StringTable::LoadString("WDT:ReviewingHistory", NULL, "D:\\Ra2\\WDTSel.cpp", 995)); // RA2: RA2 string table text (label, source path and line as the executable passes them)
		text += L"\n"; // RA2: as above
		if (first) {
			length = text.length(); // RA2: the text animation covers the map rectangle (RA2 form)
		}
		text += WideWstring(StringTable::LoadString("WDT:ReviewingHistoryDay", NULL, "D:\\Ra2\\WDTSel.cpp", 1003)) + WideWstring(L" ") + Int_To_WideString_Width(tick + 1, 2); // RA2: RA2 string table text (label, source path and line as the executable passes them)
		Set_Text(text, first ? 0 : length); // RA2: as above
		Do_Target_Anim(TourCampaign->CycleHistory->Get_State(tick), false, first_state); // RA2: as above
		first = false;
		first_state = false; // RA2: the campaign history is read tick by tick (RA2 form)
		Wait_For_Anim(TextAnim, 300000); // RA2: as above
		int count = 5; // RA2: as above
		while (!Brokeout && count--) { // RA2: as above
			Wait_Delay(5);
			Check_For_Breakout();
		}
		if (Brokeout) {
			break;
		}
	}

	return(first);
}

/// <summary>
/// Handles the review presentation of the campaign map.
/// This routine queues the commentary that suits the player's standing, replays the
/// cycles that have passed since the last visit, and then waits for the voices to
/// finish before it returns.
/// </summary>

void Selection::Presentation(int faction) // RA2: the voice-over for the campaign so far, then the days in review
{
	Set_Text(L"", 0); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	int tick = TourCampaign->Properties->Get_Current_Tick(); // RA2: as above
	int last_day = TourCampaign->Get_Last_Day(faction); // RA2: as above
	if (tick - last_day > 8) { // RA2: as above
		last_day = tick - 8; // RA2: as above
	}
	rc_ptr<State> state = TourCampaign->CycleHistory->Get_State(last_day); // RA2: the campaign history is read tick by tick (RA2 form)
	if (state.Get() != NULL) { // RA2: as above
		int owned = state->Count_Owned(TourCampaign->Get_Player_Faction()); // RA2: as above
		if (owned >= MegaWinningThreshold) {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_WIN, owned >= 30, true, 0); // RA2: RA2 voice player: group, result, emphasis, counted, delay
		} else if (owned <= MegaLosingThreshold) {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_LOSE, owned <= 0, true, 0); // RA2: RA2 voice player: group, result, emphasis, counted, delay
		} else {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_DRAW, true, true, 0); // RA2: RA2 voice player: group, result, emphasis, counted, delay
		}
		Present_Ticks(last_day, tick); // RA2: the campaign history is read tick by tick (RA2 form)
		while (VoicePlayer.Is_Speaking() && !Brokeout) { // RA2: as above
			Wait_Delay(5);
			Check_For_Breakout();
		}
	}
}

/// <summary>
/// Presents the change in the player's fortunes over a span of the campaign.
/// This routine weighs how much territory the player held at either end of the span,
/// queues the commentary that suits the result, and then plays the map animation
/// through the intervening cycles.
/// </summary>
/// <param name="tick_from">The campaign cycle the presentation starts from.</param>
/// <param name="tick_to">The campaign cycle the presentation runs up to.</param>
/// <returns>bool; Did the player cut the presentation short?</returns>

bool Selection::Present_History(int tick_from, int tick_to) // RA2: the voice-over for the change between two ticks, then the days
{
	rc_ptr<State> before = TourCampaign->CycleHistory->Get_State(tick_from); // RA2: the campaign history is read tick by tick (RA2 form)
	int old_count = before->Count_Owned(TourCampaign->Get_Player_Faction()); // RA2: as above
	rc_ptr<State> after = TourCampaign->CycleHistory->Get_State(tick_to); // RA2: as above
	int new_count = after->Count_Owned(TourCampaign->Get_Player_Faction()); // RA2: as above
	int change = new_count - old_count; // RA2: as above
	int outcome; // RA2: as above
	bool mega;
	if (change < -1) { // RA2: a loss of more than one territory is a defeat, a gain of more than one a win (more than three, or the mega thresholds, is emphatic)
		outcome = OUTCOME_LOSE;
		mega = change < -3 || new_count <= MegaLosingThreshold; // RA2: the campaign history is read tick by tick (RA2 form)
	} else if (change > 1) { // RA2: as above
		outcome = OUTCOME_WIN;
		mega = change > 3 || new_count >= MegaWinningThreshold; // RA2: the campaign history is read tick by tick (RA2 form)
	} else {
		outcome = OUTCOME_DRAW;
		mega = false;
		mega = Get_Outcome(tick_to, mega) == OUTCOME_LOSE && mega; // RA2: the campaign history is read tick by tick (RA2 form)
	}
	VoicePlayer.Say(Voices::VOICECAT_HISTORY, outcome, mega, true, 0); // RA2: RA2 voice player: group, result, emphasis, counted, delay
	return(!Present_Ticks(tick_from, tick_to)); // RA2: as above
}


/// <summary>
/// Checks whether the player has asked to skip the presentation.
/// This routine drains the keyboard and latches the breakout flag if ESC was pressed.
/// The presentation routines poll it between animation steps.
/// </summary>
void Selection::Check_For_Breakout(void)
{
	Wait_For_Focus();
	while (Keyboard->Check()) {
		if (Keyboard->Get() == KN_ESC) { // RA2: the Escape key sets Brokeout
			Brokeout = true;
		}
	}
}


/// <summary>
/// Sets the caption shown in the screen's text panel.
/// Whatever caption was showing is taken down and the panel restored, then the new text
/// is word wrapped and handed to a fresh print animation.
/// </summary>
/// <param name="text">The text to display, or NULL to leave the panel blank.</param>
/// <param name="start_delay">The delay before the text begins to print itself out.</param>
void Selection::Set_Text(WideWstring const & text, int start_delay) // RA2: the description is a bit print animation of the map's rectangle; start_delay is the length of the heading that is not typed again
{
	if (TextAnim != NULL) {
		Remove_Anim(TextAnim);
		HiddenSurface->Blit_From(WorldMap->DescriptionRect, *AlternateSurface, WorldMap->DescriptionRect, false, true); // RA2: the text animation covers the map rectangle (RA2 form)
		Add_Update_Rect(WorldMap->DescriptionRect); // RA2: as above
		TextAnim = NULL;
	}
	if (text.length() > 0) { // RA2: the text animation covers the map rectangle (RA2 form)
		WideWstring copy(text); // RA2: as above
		TextAnim = new MSBitPrintAnim(copy, WorldMap->DescriptionRect, true); // RA2: as above
		TextAnim->Set_Flag(true); // RA2: as above
		Add_Animation(TextAnim);
	}
}


/// <summary>
/// Plays one of the screen's named sound effects.
/// The effect is looked up by the name it was given in the INI. An unknown name is
/// quietly ignored.
/// </summary>
/// <param name="name">The name of the sound effect to play.</param>
void Selection::Set_Tooltip(WideWstring const & text) // RA2: RA2 string table text (label, source path and line as the executable passes them)
{
	Rect rect = WorldMap->TooltipRect; // RA2: the text animation covers the map rectangle (RA2 form)
	if (TooltipAnim != NULL) { // RA2: as above
		Remove_Anim(TooltipAnim); // RA2: as above
		HiddenSurface->Blit_From(rect, *AlternateSurface, rect, false, true); // RA2: as above
		Add_Update_Rect(rect); // RA2: as above
		TooltipAnim = NULL; // RA2: as above
	}
	if (text.length() > 0) { // RA2: the text animation covers the map rectangle (RA2 form)
		WideWstring copy(text); // RA2: as above
		TooltipAnim = new MSBitPrintAnim(copy, rect, false); // RA2: as above
		TooltipAnim->Set_Flag(true); // RA2: as above
		Add_Animation(TooltipAnim); // RA2: as above
	}
}


void Selection::Play_SFX(Wstring const & name) // RA2: RA2 looks the sound up by Wstring name
{
	if (name != 0) { // RA2: RA2 looks the sound up by Wstring name
		SFX_ITER it(SfxEntries, 0);
		while (it && name == (*it)->Get_Name()) { // RA2: RA2 looks the sound up by Wstring name
			++it; // RA2: as above
		}
		if (it) {
			(*it)->Play(); // RA2: RA2 looks the sound up by Wstring name
		}
	}
}


/// <summary>
/// Puts the campaign's own description up as the screen caption.
/// This is the caption the screen wears whenever no territory is under the mouse.
/// </summary>
void Selection::Set_Long_Description(void)
{
	Set_Text(TourCampaign->Properties->LongDescription, 0); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	WideWstring tip(StringTable::LoadString("WDT:TerritorySelectToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 1230)); // RA2: as above
	Set_Tooltip(tip); // RA2: as above
}


/// <summary>
/// Replays the recent history of a world domination campaign.
/// This routine brings the selection screen up purely as a spectacle -- the player
/// watches the territories change hands and nothing is picked.
/// </summary>
/// <param name="campaign">The campaign whose history is to be replayed.</param>
void __fastcall Selection::Show(rc_ptr<Campaign> const & campaign, int faction) // RA2: the review of the campaign so far
{
	Keyboard->Clear();
	Selection selection(campaign, faction); // RA2: the screen is a stack object made from the campaign
	selection.Presentation(faction); // RA2: as above
}


/// <summary>
/// Handles the world domination territory selection screen.
/// This routine runs the selection screen for the campaign supplied and then copies the
/// chosen territory's game options into the session so that the mission may start.
/// </summary>
/// <param name="campaign">The campaign whose map and conflicts are to be presented.</param>
/// <param name="vq_anim">Should the animated backdrop be used?</param>
/// <returns>bool; Was a playable territory chosen?</returns>
bool __fastcall Selection::Choose_Conflict(rc_ptr<Campaign> const & campaign, int faction) // RA2: runs the screen for the player's next conflict and hands the territory's battle settings to the session
{
	if (campaign.Get() == NULL) { // RA2: the screen is a stack object made from the campaign
		return(false); // RA2: as above
	}

	Selection selection(campaign, faction); // RA2: the screen is a stack object made from the campaign
	selection.Set_Long_Description(); // RA2: as above
	selection.Do_Target_Selection(faction); // RA2: as above
	rc_ptr<Territory> territory = selection.Pick_Territory(); // RA2: as above
	bool chosen = territory.Get() != NULL; // RA2: as above
	if (chosen) { // RA2: as above
		Session.TournamentTerritory = territory->ID; // RA2: as above
		rc_ptr<WDTTerritory> settings = Find_WDT_Territory(Session.TournamentTerritory); // RA2: as above
		if (settings.Get() != NULL) { // RA2: as above
			Session.Options.UnitCount = settings->UnitCount; // RA2: as above
			BuildLevel = settings->TechLevel; // RA2: as above
			Session.Options.Credits = settings->StartingCredits; // RA2: as above
			Session.Options.AlliesAllowed = (settings->Booleans & 0x4) != 0; // RA2: as above
			Session.Options.HarvTruce = false; // RA2: as above
			Session.Options.Bases = (settings->Booleans & 0x10) != 0; // RA2: as above
			Session.Options.MCVRedeploy = (settings->Booleans & 0x20) != 0; // RA2: as above
			Session.Options.FogOfWar = false; // RA2: as above
			Session.Options.BridgeDestruction = true; // RA2: as above
			Session.Options.Goodies = (settings->Booleans & 0x100) != 0; // RA2: as above
			Session.Options.ShortGame = (settings->Booleans & 0x400) != 0; // RA2: as above
			Session.Options.SWAllowed = (settings->Booleans & 0x200) != 0; // RA2: as above
			Session.Options.BuildOffAlly = false; // RA2: as above
			Session.Options.AIPlayers = 0;
			Session.Options.AIDifficulty = (DiffType)1; // RA2: the chosen territory
		} else {
			chosen = false; // RA2: the chosen territory
		}
	}
	return(chosen); // RA2: the chosen territory
}


/// <summary>
/// Brings the territory selection screen up.
/// This routine takes the mouse, starts the screen's theme, clears the surfaces, and
/// then waits out the map's introductory animation before the player is given control.
/// </summary>
void Selection::Start(void) // RA2: starts the theme, blanks the surfaces and runs the map's introduction
{
	IgnoreInput = false; // RA2: RA2 start and end of the screen: input, mouse and theme
	if (ThemeName != 0) { // RA2: as above
		Theme.Play_Song(Theme.From_Name(ThemeName.get())); // RA2: as above
		Theme.Set_Repeat(true);
	}
	MouseCursor->Hide_Mouse(); // RA2: RA2 start and end of the screen: input, mouse and theme
	AlternateSurface->Fill(0);
	HiddenSurface->Fill(0);
	VisibleSurface->Blit_From(*HiddenSurface, false, true); // RA2: the text animation covers the map rectangle (RA2 form)
	Keyboard->Clear();
	WorldMap->Start_Intro(this, &VoicePlayer); // RA2: RA2 start and end of the screen: input, mouse and theme
	while (MouseCursor->Get_Mouse_State() < 0) { // RA2: as above
		MouseCursor->Show_Mouse(); // RA2: as above
	}
}


/// <summary>
/// Shuts the territory selection screen down.
/// This routine fades the music out, clears the screen back to the menu backdrop, and
/// hands the mouse back to the owner draw system.
/// </summary>
void Selection::End(void)
{
	MouseCursor->Hide_Mouse(); // RA2: RA2 start and end of the screen: input, mouse and theme
	Theme.Queue_Song((ThemeType)-3); // RA2: as above
	AlternateSurface->Fill(0);
	HiddenSurface->Fill(0);
	while (MouseCursor->Get_Mouse_State() < 0) { // RA2: RA2 start and end of the screen: input, mouse and theme
		MouseCursor->Show_Mouse(); // RA2: as above
	}
}
