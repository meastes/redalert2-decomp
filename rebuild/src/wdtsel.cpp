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
#include "geometry.h"
#include "language\language.h"
#include "bitfont.h"

#include "_keyboar.h"
#include "_surface.h"
#include "ccfile.h"
#include "convert.h"
#include "data.h"
#include "dsurface.h"
#include "globals.h"
#include "ini.h"
#include "keyboard.h"
#include "mschoice.h"
#include "msfont.h"
#include "newmenu.h"
#include "ownrdraw.h"
#include "pcx.h"
#include "session.h"
#include "stringtable.h"
#include "theme.h"
#include "vector.h"
#include "wdtnet.h"
#include "wonline.h"
#include "worlddom.h"
#include "wstring.h"

using namespace WorldDominationTour;



inline TERRITORY_ITER Walk_Territories(TERRITORY_LIST & list) { return(TERRITORY_ITER(list, 0)); }

static inline WideWstring Load_Selection_Text(char const * label, int source_line) // RA2: ra2-decomp's file-local helper (its wdtsel.cpp, used by its Select_Territory at the four tooltips; `static` there)
{
	return(WideWstring(StringTable::LoadString(label, NULL, "D:\\Ra2\\WDTSel.cpp", source_line)));
}


/// <summary>
/// Creates the world domination territory selection screen.
/// This routine assembles the whole screen out of the map database -- the voice overs, the
/// sound effects, the layout, the artwork and the region currently being fought over -- and
/// then sets it running.
/// </summary>
/// <param name="campaign">The campaign whose current state the screen presents.</param>
/// <param name="faction">The faction whose progress the screen shows.</param>
Selection::Selection(rc_ptr<Campaign> const & campaign, int faction) :
	TourCampaign(campaign),
	VoicePlayer(campaign->Get_Player_Faction()),
	Brokeout(false),
	LastPoint(-1, -1),
	StartTick(campaign->Properties->Get_Current_Tick()),
	LastHoverZone(-1),
	WorldMap(Map::Create(campaign->Properties->Get_Map_ID(), faction, campaign->Get_Player_Faction())),
	HoveredTerritoryID(-1),
	MousedTerritory(),
	TargetDrawer(NULL),
	ZoomingTarget(NULL),
	ThrobbingTarget(NULL),
	ThrobbingTargetDividingFrame(32),
	TextAnim(NULL),
	TooltipAnim(NULL),
	Font(new MSFont(false)),
	CurrentState(State::Create()),
	ThemeName(NULL),
	SfxEntries(0, 0)
{
	int color = Text_Color;
	int pixel = DSurface::Build_Hicolor_Pixel((unsigned char)color, (unsigned char)((unsigned short)color >> 8), (unsigned char)(color >> 16));
	TextColor = BitFont::Instance->Set_Color(pixel) & 0xFFFF;

	Add_Animation(VoicePlayer.Create_Anim());

	CCFileClass file("WDTMAP.INI");
	INIClass ini;
	ini.Load(file);

	Init_Voices(ini, "VoiceOvers");
	Init_Sounds(ini, "SOUNDS");

	Wstring layout;
	ini.Get_String("LAYOUTS", Wstring_FromInt(campaign->Properties->Get_Layout()), "", layout);
	Init_Target(ini, layout);

	WorldMap->Load(ini, this, campaign->Is_Different(faction));

	WideWstring play_text(Load_Selection_Text("GUI:PlayWDT", 157)); // RA2: the unit's loader (no twin text of this constructor: a judgment call)
	MSBitPrintAnim * play = new MSBitPrintAnim(play_text, WorldMap->PlayRect, true);
	play->Set_Flag(true);
	Add_Animation(play);

	WideWstring back_text(Load_Selection_Text("TXT_BACK", 164)); // RA2: as above
	MSBitPrintAnim * back = new MSBitPrintAnim(back_text, WorldMap->BackRect, true);
	back->Set_Flag(true);
	Add_Animation(back);

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

	for (SFX_ITER sfx(SfxEntries, 0); sfx; sfx++) {
		delete *sfx;
	}

	delete TargetDrawer;
	delete ZoomingTarget;
	delete ThrobbingTarget;
	delete Font;

	BitFont::Instance->Set_Color((unsigned short)TextColor);
}


/// <summary>
/// Fetches the voice over settings for the selection screen.
/// Along with the voice list itself, this routine picks up the score thresholds that
/// decide how enthusiastic the announcer gets about the campaign so far.
/// </summary>
/// <param name="ini">The map database to fetch the voice overs from.</param>
/// <param name="section">The section that describes the voice overs.</param>
void Selection::Init_Voices(INIClass & ini, Wstring const & section)
{
	VoicePlayer.Load(ini, section);
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

void Selection::Init_Sounds(INIClass & ini, Wstring const & section)
{
	Wstring value;
	int count = ini.Entry_Count(section);
	for (int i = 0; i < count; i++) {
		Wstring entry = ini.Get_Entry(section, i);
		if (entry != 0 && ini.Get_String(section, entry, NULL, value) > 0) {
			if (entry == "Theme") {
				ThemeName = value;
				break;
			}
			MSSfxEntry * sfx = new MSSfxEntry(entry, value);
			if (sfx->Get_Name() == 0) {
				delete sfx;
			} else if (sfx != NULL) {
				SfxEntries.Add(sfx);
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
void Selection::Init_Target(INIClass & ini, Wstring const & section)
{
	Wstring value;
	Wstring faction(Faction_Name(TourCampaign->Get_Player_Faction()));
	if (ini.Get_String(section, faction + "ZoomingTarget", NULL, value) > 0) {
		ZoomingTarget = new MSAnimEntry(value);
	}
	ini.Get_String(section, faction + "TargetPalette", NULL, value);
	TargetDrawer = Create_Drawer(value);
	ini.Get_String(section, faction + "Target", NULL, value);
	ThrobbingTarget = new MSAnimEntry(value);
	ThrobbingTargetDividingFrame = ini.Get_Int(section, faction + "TargetDividingFrame", 32);
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
#ifdef RA2_NON_MATCHING // not exact: 912 bytes differ (adopted text)
rc_ptr<Territory> Selection::Pick_Territory(void)
{
	Set_Long_Description();
	bool canceled = false;
	bool selected = false;

	Keyboard->Clear();

	do {
		WOL_Wait_Callback();
		Wait_For_Focus();
		Point2D point(MouseCursor->Get_Mouse_X(), MouseCursor->Get_Mouse_Y());

		if (Keyboard->Check()) {
			int key = Keyboard->Get();
			if (key == KN_LMOUSE) {
				Handle_Click(point, selected);
			} else if (key == (KN_LMOUSE | WWKEY_RLS_BIT)) {
				WorldMap->Set_Back_Hot(this, false);
				if (Region_At(point) == 2) {
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
			int zone = Region_At(point);

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
	} while (!canceled && !selected);

	return(selected ? MousedTerritory : rc_ptr<Territory>());
}
#endif



/// <summary>
/// Handles a mouse click on the territory selection screen.
/// This routine routes the click either to the cancel button or to the campaign map,
/// where it becomes an attempt to pick a territory.
/// </summary>
/// <param name="point">The screen position that was clicked.</param>
/// <param name="selected">Set to true if the click landed on a selectable territory.</param>
#ifdef RA2_NON_MATCHING // not exact: 505 bytes differ (adopted text)
void Selection::Handle_Click(Point2D const & point, bool & selected)
{
	switch (Region_At(point)) {
		case 2:
			WorldMap->Set_Back_Hot(this, true);
			On_Cancel_Enter();
			break;

		case 1:
			WorldMap->Set_Back_Hot(this, false);
			selected = Hover_At(point);
			if (selected) {
				Play_SFX("ConflictSelected");
			}
			break;
	}
}
#endif


#ifdef RA2_NON_MATCHING // not exact: 175 bytes differ (adopted text)
bool Selection::Select_Territory_At(Point2D const & point)
{
	Update_Hovered_Territory(point);
	rc_ptr<Territory> territory = WorldMap->Get_Territory(HoveredTerritoryID);
	return(Select_Territory(territory));
}
#endif


void Selection::Update_Hovered_Territory(Point2D const & point)
{
	int id = WorldMap->Territory_At(point);
	if (id != HoveredTerritoryID) {
		HoveredTerritoryID = id;
		Select_Territory(WorldMap->Get_Territory(id));
	}
}


/// <summary>
/// Selects whichever territory lies beneath the point.
/// </summary>
/// <param name="point">The point to look up, relative to the campaign map.</param>
/// <returns>bool; Is there a conflict waiting on the territory selected?</returns>
#ifdef RA2_NON_MATCHING // not exact: 841 bytes differ (adopted text)
void Selection::Do_Target_Anim(rc_ptr<State> const & state, bool show_targets, bool silent)
{
	Remove_Target_Anims();

	MS_ANIM_LIST zoom_anims;
	MS_ANIM_LIST throb_anims;

	/*
	 * Walk each territory and advance any whose state has changed, playing the
	 * matching occupation sound effect. When requested, queue the zoom/throb
	 * target animations for newly disputed territories.
	 */
	for (TERRITORY_ITER it = Walk_Territories(WorldMap->Territories); it; it++) {
		int id = (*it)->Get_ID();
		int old_state = CurrentState->Owner_Of(id);
		int new_state = state->Owner_Of(id);

		if (new_state != old_state) {
			Check_For_Breakout();
			(*it)->Set_Owner_State(new_state, this, !Brokeout || silent);
		}

		if (show_targets && new_state == 1) {
			Point2D target = WorldMap->Origin + (*it)->Get_Target();
			int x = target.X;
			int y = target.Y;

			if (ZoomingTarget != NULL) {
				CCFileClass file(ZoomingTarget->Get_Filename());
				if (file.Is_Available()) {
					MSFadeAnim * fade = new MSFadeAnim(ZoomingTarget->Get_Filename(), x, y, TargetDrawer, 3, SHAPE_CENTER, &Anims);
					zoom_anims.Add(fade);
				}
			}

			CCFileClass file(ThrobbingTarget->Get_Filename());
			if (file.Is_Available()) {
				MSShapeAnim * shape = new MSShapeAnim(ThrobbingTarget->Get_Filename(), x, y, TargetDrawer, 5, true, SHAPE_CENTER);
				shape->Set_Stop_Frame(ThrobbingTargetDividingFrame - 1);
				shape->Set_Start_Frame(0);
				shape->Set_Frame(0);
				(*it)->Set_Target_Anim(shape);
				throb_anims.Add(shape);
			}
		}
	}

	/*
	 * Play the queued target animations. On abort, discard the zoom animations;
	 * otherwise play the zoom SFX and wait for each to complete.
	 */
	MS_ANIM_ITER zoom_iter(zoom_anims, 0);
	for (MS_ANIM_ITER throb_iter(throb_anims, 0); throb_iter; throb_iter++) {
		Check_For_Breakout();

		if (zoom_iter) {
			if (Brokeout) {
				MSAnim * anim = *zoom_iter;
				delete anim;
			} else {
				Play_SFX("TargetZoom");
				Add_Animation(*zoom_iter);
				Wait_For_Anim(*zoom_iter, 300000);
			}
			zoom_iter++;
		}

		Add_Animation(*throb_iter);
	}

	CurrentState = state;
}
#endif



/// <summary>
/// Makes a territory the one under the player's attention.
/// This routine takes the highlight and the pending conflict away from whichever
/// territory held them, highlights the new one, and replaces the screen caption with
/// its name, description, and game options. Passing NULL merely clears the selection.
/// </summary>
/// <param name="territory">The territory to select, or NULL to select none at all.</param>
/// <returns>bool; Is there a conflict waiting on the selected territory?</returns>
bool Selection::Select_Territory(rc_ptr<Territory> const & territory)
{
	bool was_set = SelectedConflict.Get() != NULL;

	if (territory.Get() == MousedTerritory.Get()) {
		return(was_set);
	}

	if (MousedTerritory.Get() != NULL) {
		if (SelectedConflict.Get() != NULL) {
			SelectedConflict = NULL;
		}
		MousedTerritory->Set_Highlight(false, this);
		MSShapeAnim * anim = (MSShapeAnim *)MousedTerritory->Get_Target_Anim();
		if (anim != NULL) {
			anim->Set_Stop_Frame(ThrobbingTargetDividingFrame - 1);
			anim->Set_Start_Frame(0);
			anim->Set_Frame(0);
		}
	}

	MousedTerritory = territory;

	if (MousedTerritory.Get() == NULL) {
		Set_Long_Description();
		VoicePlayer.Say_List(1, false, 30);
		return(was_set);
	}

	bool found = false;
	WideWstring description;
	WideWstring options;

	int id = MousedTerritory->Get_ID();
	int territory_state = CurrentState->Owner_Of(id);
	int outcome = OUTCOME_DRAW;
	if (territory_state != 1 && territory_state != 0) {
		outcome = TourCampaign->Get_Player_Faction() != territory_state;
	}
	VoicePlayer.Say(Voices::VOICECAT_TERRITORY, outcome, false, false, 30);

	switch (territory_state) {
		case 1:
			Set_Tooltip(Load_Selection_Text("WDT:TerritorySelDispToolTip", 0x2b5));
			break;

		case 2:
			Set_Tooltip(Load_Selection_Text("WDT:AlliedHeldTooltip", 0x2ad));
			break;

		case 3:
			Set_Tooltip(Load_Selection_Text("WDT:SovietHeldTooltip", 0x2b1));
			break;

		default:
			Set_Tooltip(Load_Selection_Text("WDT:TerritorySelectToolTip", 0x2b9));
			break;
	}

	MousedTerritory->Set_Highlight(true, this);

	if (TourCampaign->Properties->Get_Current_Tick() == StartTick) {
		CONFLICT_ITER it(TourCampaign->Conflicts, 0);
		while (it) {
			rc_ptr<Conflict> const & conflict = *it;
			Conflict const * current = conflict.operator->();
			int id = MousedTerritory->Get_ID();
			if (current->Is_Territory(id)) {
				break;
			}
			it++;
		}

		if (it) {
			found = true;
			SelectedConflict = *it;
			description = (*it)->Describe_Options();
			options = description;
			MSShapeAnim * anim = (MSShapeAnim *)territory->Get_Target_Anim();
			if (anim != NULL) {
				anim->Set_Stop_Frame(-1);
				anim->Set_Start_Frame(ThrobbingTargetDividingFrame);
				anim->Set_Frame(ThrobbingTargetDividingFrame);
			}
		}
	}

	WideWstring text = StringTable::LoadString("WDT:TerritoryPrefix", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2d7) + MousedTerritory->Name + StringTable::LoadString("WDT:TerritorySuffix", NULL, "D:\\Ra2\\WDTSel.cpp", 0x2d7);
	if (options.length() > 0) {
		text += description;
	}
	Set_Text(text, 0);
	return(found);
}



/// <summary>
/// Removes the target marker animation from every territory on the map.
/// </summary>
void Selection::Remove_Target_Anims(void)
{
	for (TERRITORY_ITER it(WorldMap->Territories, 0); it; ++it) {
		MSAnim * anim = (*it)->Remove_Target_Anim();
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
	for (TERRITORY_ITER it(WorldMap.Get()->Territories, 0); it; ++it) {
		MSAnim * anim = (*it)->Remove_Target_Anim();
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
	rc_ptr<Territory> none;
	Select_Territory(none);
}


/// <summary>
/// Handles the mouse entering the cancel button.
/// This routine swaps the button over to its highlighted animation, repaints it, and
/// plays the accompanying sound effect.
/// </summary>
void Selection::On_Cancel_Enter(void)
{
	if (!WorldMap->BackButton.IsDown) {
		WorldMap->Set_Back_Down(this, true);
		Play_SFX("MouseCancel");
		Set_Tooltip(StringTable::LoadString("WOL:WDTTerritoryBack", NULL, "D:\\Ra2\\WDTSel.cpp", 894));
	}
}


/// <summary>
/// Handles the mouse leaving the cancel button.
/// This routine puts the button back to its idle animation and repaints it.
/// </summary>
void Selection::On_Cancel_Leave(void)
{
	if (WorldMap->BackButton.IsDown) {
		WorldMap->Set_Back_Down(this, false);
		Set_Tooltip(StringTable::LoadString("WDT:TerritorySelectToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 916));
	}
}

/// <summary>
/// Brings the campaign map up to date and marks the available targets.
/// This routine runs before the player is allowed to choose. It replays whatever has
/// happened since the last visit, plants the target markers on the territories that are
/// up for grabs, and writes the campaign back out again.
/// </summary>

void Selection::Do_Target_Selection(int faction)
{
	Set_Text(NULL, 0);

	int tick = TourCampaign->Properties->Get_Current_Tick();
	bool won = true;
	int last_day = TourCampaign->Get_Last_Day(faction);
	if (tick - last_day > 8) {
		last_day = tick - 8;
	}

	if (last_day != tick) {
		won = !Present_History(last_day, tick);
	} else {
		bool mega = false;
		int outcome = Get_Outcome(tick, mega);
		VoicePlayer.Say(Voices::VOICECAT_STATUS, outcome, mega, true, 0);
	}

	Set_Text(NULL, 0);
	Check_For_Breakout();
	Do_Target_Anim(TourCampaign->CycleHistory->Get_State(tick), true, won);
	TourCampaign->Save_Progress(faction);
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
int Selection::Get_Outcome(int tick, bool & mega)
{
	rc_ptr<State> state = TourCampaign->CycleHistory->Get_State(tick);
	if (state.Get() != NULL) {
		int owned = state->Count_Owned(TourCampaign->PlayerFaction);
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
bool Selection::Present_Ticks(int tick_from, int tick_to)
{
	bool first = true;
	bool first_state = true;
	unsigned int length = 0;
	for (int tick = tick_from; tick <= tick_to; tick++) {
		Check_For_Breakout();
		if (Brokeout) {
			break;
		}

		WideWstring text(StringTable::LoadString("WDT:ReviewingHistory", NULL, "D:\\Ra2\\WDTSel.cpp", 995));
		text += L"\n";
		if (first) {
			length = text.length();
		}
		text += WideWstring(StringTable::LoadString("WDT:ReviewingHistoryDay", NULL, "D:\\Ra2\\WDTSel.cpp", 1003)) + WideWstring(L" ") + Int_To_WideString_Width(tick + 1, 2);
		Set_Text(text, first ? 0 : length);
		Do_Target_Anim(TourCampaign->CycleHistory->Get_State(tick), false, first_state);
		first = false;
		first_state = false;
		Wait_For_Anim(TextAnim, 300000);
		int count = 5;
		while (!Brokeout && count--) {
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

void Selection::Presentation(int faction)
{
	Set_Text(L"", 0);
	int tick = TourCampaign->Properties->Get_Current_Tick();
	int last_day = TourCampaign->Get_Last_Day(faction);
	if (tick - last_day > 8) {
		last_day = tick - 8;
	}
	rc_ptr<State> state = TourCampaign->CycleHistory->Get_State(last_day);
	if (state.Get() != NULL) {
		int owned = state->Count_Owned(TourCampaign->Get_Player_Faction());
		if (owned >= MegaWinningThreshold) {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_WIN, owned >= 30, true, 0);
		} else if (owned <= MegaLosingThreshold) {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_LOSE, owned <= 0, true, 0);
		} else {
			VoicePlayer.Say(Voices::VOICECAT_OLD_CYCLE, OUTCOME_DRAW, true, true, 0);
		}
		Present_Ticks(last_day, tick);
		while (VoicePlayer.Is_Speaking() && !Brokeout) {
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

bool Selection::Present_History(int tick_from, int tick_to)
{
	rc_ptr<State> before = TourCampaign->CycleHistory->Get_State(tick_from);
	int old_count = before->Count_Owned(TourCampaign->Get_Player_Faction());
	rc_ptr<State> after = TourCampaign->CycleHistory->Get_State(tick_to);
	int new_count = after->Count_Owned(TourCampaign->Get_Player_Faction());
	int change = new_count - old_count;
	int outcome;
	bool mega;
	if (change < -1) {
		outcome = OUTCOME_LOSE;
		mega = change < -3 || new_count <= MegaLosingThreshold;
	} else if (change > 1) {
		outcome = OUTCOME_WIN;
		mega = change > 3 || new_count >= MegaWinningThreshold;
	} else {
		outcome = OUTCOME_DRAW;
		mega = false;
		mega = Get_Outcome(tick_to, mega) == OUTCOME_LOSE && mega;
	}
	VoicePlayer.Say(Voices::VOICECAT_HISTORY, outcome, mega, true, 0);
	return(!Present_Ticks(tick_from, tick_to));
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
		if (Keyboard->Get() == KN_ESC) {
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
void Selection::Set_Text(WideWstring const & text, int start_delay)
{
	if (TextAnim != NULL) {
		Remove_Anim(TextAnim);
		HiddenSurface->Blit_From(WorldMap->DescriptionRect, *AlternateSurface, WorldMap->DescriptionRect, false, true);
		Add_Update_Rect(WorldMap->DescriptionRect);
		TextAnim = NULL;
	}
	if (text.length() > 0) {
		WideWstring copy(text);
		TextAnim = new MSBitPrintAnim(copy, WorldMap->DescriptionRect, true);
		TextAnim->Set_Flag(true);
		Add_Animation(TextAnim);
	}
}


/// <summary>
/// Plays one of the screen's named sound effects.
/// The effect is looked up by the name it was given in the INI. An unknown name is
/// quietly ignored.
/// </summary>
/// <param name="name">The name of the sound effect to play.</param>
void Selection::Set_Tooltip(WideWstring const & text)
{
	Rect rect = WorldMap->TooltipRect;
	if (TooltipAnim != NULL) {
		Remove_Anim(TooltipAnim);
		HiddenSurface->Blit_From(rect, *AlternateSurface, rect, false, true);
		Add_Update_Rect(rect);
		TooltipAnim = NULL;
	}
	if (text.length() > 0) {
		WideWstring copy(text);
		TooltipAnim = new MSBitPrintAnim(copy, rect, false);
		TooltipAnim->Set_Flag(true);
		Add_Animation(TooltipAnim);
	}
}


void Selection::Play_SFX(Wstring const & name)
{
	if (name != 0) {
		SFX_ITER it(SfxEntries, 0);
		while (it && name == (*it)->Get_Name()) {
			++it;
		}
		if (it) {
			(*it)->Play();
		}
	}
}


/// <summary>
/// Puts the campaign's own description up as the screen caption.
/// This is the caption the screen wears whenever no territory is under the mouse.
/// </summary>
void Selection::Set_Long_Description(void)
{
	Set_Text(TourCampaign->Properties->LongDescription, 0);
	WideWstring tip(StringTable::LoadString("WDT:TerritorySelectToolTip", NULL, "D:\\Ra2\\WDTSel.cpp", 1230));
	Set_Tooltip(tip);
}


/// <summary>
/// Replays the recent history of a world domination campaign.
/// This routine brings the selection screen up purely as a spectacle -- the player
/// watches the territories change hands and nothing is picked.
/// </summary>
/// <param name="campaign">The campaign whose history is to be replayed.</param>
void __fastcall Selection::Show(rc_ptr<Campaign> const & campaign, int faction)
{
	Keyboard->Clear();
	Selection selection(campaign, faction);
	selection.Presentation(faction);
}


/// <summary>
/// Handles the world domination territory selection screen.
/// This routine runs the selection screen for the campaign supplied and then copies the
/// chosen territory's game options into the session so that the mission may start.
/// </summary>
/// <param name="campaign">The campaign whose map and conflicts are to be presented.</param>
/// <param name="vq_anim">Should the animated backdrop be used?</param>
/// <returns>bool; Was a playable territory chosen?</returns>
bool __fastcall Selection::Choose_Conflict(rc_ptr<Campaign> const & campaign, int faction)
{
	if (campaign.Get() == NULL) {
		return(false);
	}

	Selection selection(campaign, faction);
	selection.Set_Long_Description();
	selection.Do_Target_Selection(faction);
	rc_ptr<Territory> territory = selection.Pick_Territory();
	bool chosen = territory.Get() != NULL;
	if (chosen) {
		Session.TournamentTerritory = territory->ID;
		rc_ptr<WDTTerritory> settings = Find_WDT_Territory(Session.TournamentTerritory);
		if (settings.Get() != NULL) {
			Session.Options.UnitCount = settings->UnitCount;
			BuildLevel = settings->TechLevel;
			Session.Options.Credits = settings->StartingCredits;
			Session.Options.AlliesAllowed = (settings->Booleans & 0x4) != 0;
			Session.Options.HarvTruce = false;
			Session.Options.Bases = (settings->Booleans & 0x10) != 0;
			Session.Options.MCVRedeploy = (settings->Booleans & 0x20) != 0;
			Session.Options.FogOfWar = false;
			Session.Options.BridgeDestruction = true;
			Session.Options.Goodies = (settings->Booleans & 0x100) != 0;
			Session.Options.ShortGame = (settings->Booleans & 0x400) != 0;
			Session.Options.SWAllowed = (settings->Booleans & 0x200) != 0;
			Session.Options.BuildOffAlly = false;
			Session.Options.AIPlayers = 0;
			Session.Options.AIDifficulty = (DiffType)DIFF_NORMAL;
		} else {
			chosen = false;
		}
	}
	return(chosen);
}


/// <summary>
/// Brings the territory selection screen up.
/// This routine takes the mouse, starts the screen's theme, clears the surfaces, and
/// then waits out the map's introductory animation before the player is given control.
/// </summary>
void Selection::Start(void)
{
	IgnoreInput = false;
	if (ThemeName != 0) {
		Theme.Play_Song(Theme.From_Name(ThemeName.get()));
		Theme.Set_Repeat(true);
	}
	MouseCursor->Hide_Mouse();
	AlternateSurface->Fill(0);
	HiddenSurface->Fill(0);
	VisibleSurface->Blit_From(*HiddenSurface, false, true);
	Keyboard->Clear();
	WorldMap->Start_Intro(this, &VoicePlayer);
	while (MouseCursor->Get_Mouse_State() < 0) {
		MouseCursor->Show_Mouse();
	}
}


/// <summary>
/// Shuts the territory selection screen down.
/// This routine fades the music out, clears the screen back to the menu backdrop, and
/// hands the mouse back to the owner draw system.
/// </summary>
void Selection::End(void)
{
	MouseCursor->Hide_Mouse();
	Theme.Queue_Song(THEME_QUIET);
	AlternateSurface->Fill(0);
	HiddenSurface->Fill(0);
	while (MouseCursor->Get_Mouse_State() < 0) {
		MouseCursor->Show_Mouse();
	}
}
