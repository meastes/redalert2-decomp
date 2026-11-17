/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "worlddom.h"

#include "_mixfile.h"
#include "_pk.h"
#include "_surface.h" // RA2: AlternateSurface, which the side choice dialog draws its pictures on
#include "addon.h"
#include "grphmenu.h"
#include "language\language.h"
#include "mapgen.h"
#include "mixfile.h"
#include "ownrdraw.h"
#include "pcx.h" // RA2: the side choice pictures are PCX files
#include "wdtnet.h"
#include "wonline.h"
#include "wstring.h" // RA2: the includes this file needs
#include "goptions.h" // RA2: Setup_Game switches the screen to 640x480 for the tour
#include "session.h" // RA2: as above
#include "misc.h" // RA2: VideoModeWidth
#include "netshare.h" // RA2: ODMessageBox
#include "stringtable.h" // RA2: the texts are loaded by label
#include "movie.h"
#undef LoadString // RA2: windows.h maps it to LoadStringA


using namespace WorldDominationTour;


// RA2: callees that are not in a header yet
class Surface; // RA2: as above
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, dims the screen behind the dialog
void __fastcall Dialog_On_WM_PAINT(HWND dialog); // RA2: 0x005FEE70, defined in ownrdraw.cpp
void __fastcall Dialog_On_WM_DRAWITEM(long item); // RA2: 0x005FE380, as above
void __fastcall Slide_Pending_Dialog(HWND window); // RA2: 0x005E5F20, as above
bool __fastcall Test_Display_Mode_Dialog(int width, int height); // RA2: 0x00543A70, asks the player to confirm a screen mode (not in this module)
int Start_Quick_Match(void); // RA2: 0x00759E20, starts the match (not in this module); 1 once the game is over


rc_ptr<Campaign> const & __fastcall Current_WDT_Campaign(void); // RA2: 0x00769670, defined below
rc_ptr<Campaign> const & __fastcall Previous_WDT_Campaign(void); // RA2: 0x00769680, as above


/// <summary>
/// Fetches the name a faction is called in the tour's INI files.
/// </summary>
/// <param name="faction">The faction, 1 to 3.</param>
/// <returns>Returns with the faction's name, or "xx" for any other number.</returns>
Wstring const & __fastcall Faction_Name(int faction) // RA2: 0x007685A0
{
	static Wstring dis("DIS"); // RA2: the three names and the fallback are function statics (0x007685A0); TibSun has no such routine
	static Wstring all("ALL"); // RA2: as above
	static Wstring sov("SOV"); // RA2: as above

	switch (faction) { // RA2: as above
		case 1: return dis;
		case 2: return all; // RA2: as above
		case 3: return sov; // RA2: as above
		default: break; // RA2: this label is what keeps Theater_Choice_Menu from expanding the routine (the executable calls it)
	}
	static Wstring unknown("xx"); // RA2: the fallback is a static of its own, built on first use
	return unknown; // RA2: as above
}


/// <summary>
/// Fetches the short code of a faction, the one the LastDay keys of the history file end in.
/// </summary>
/// <param name="faction">The faction, 1 to 3.</param>
/// <returns>Returns with the faction's code, or "XX" for any other number.</returns>
Wstring const & __fastcall Faction_Code(int faction) // RA2: 0x007686D0
{
	static Wstring na("NA"); // RA2: the short codes are function statics, the same shape (0x007686D0)
	static Wstring eu("EU"); // RA2: as above
	static Wstring as("AS"); // RA2: as above

	switch (faction) { // RA2: as above
		case 1: return na; // RA2: as above
		case 2: return eu; // RA2: as above
		case 3: return as; // RA2: as above
	}
	static Wstring unknown("XX"); // RA2: as above
	return unknown; // RA2: as above
}


namespace WorldDominationTour { // RA2: the side choice dialog control (vtable 0x007AF9DC; fields +0x0C..+0x1C) is a class of this file
	typedef Surface FactionImage; // RA2: a PCX picture

	/*
	 * RA2: the dialog control of the side choice (Allies or Soviets). It remembers which side the
	 * mouse is over and which of the three pictures goes with it.
	 */
	class FactionSelectDialogControl : public OwnerDraw::SimpleDialogControl { // RA2: as above
		public: // RA2: as above
			FactionSelectDialogControl(void); // RA2: 0x00769790
			virtual ~FactionSelectDialogControl(void); // RA2: 0x00768800 (scalar deleting 0x00769740)

			void Update_Hover(HWND window, Point2D const & point); // RA2: 0x00768840
			void Draw(int unused); // RA2: 0x00768940

			int Selection; // RA2: +0x0C, 0 none, 2 Allies, 3 Soviets
			FactionImage * Image1; // RA2: +0x10, neither
			FactionImage * Image2; // RA2: +0x14, Allies
			FactionImage * Image3; // RA2: +0x18, Soviets
			FactionImage * Current; // RA2: +0x1C, the picture drawn
	};
}


/// <summary>
/// Frees the side choice pictures.
/// </summary>
WorldDominationTour::FactionSelectDialogControl::~FactionSelectDialogControl(void) // RA2: 0x00768800
{
	delete Image1; // RA2: the destructor deletes the three pictures through their virtual destructors (0x00768800), then the base
	delete Image2; // RA2: as above
	delete Image3; // RA2: as above
}


/// <summary>
/// Works out which side the mouse is over and highlights its picture.
/// This routine finds the side by the clickable regions of the picture and redraws the dialog
/// when the choice changes.
/// </summary>
/// <param name="window">The dialog.</param>
/// <param name="point">The mouse position.</param>
void WorldDominationTour::FactionSelectDialogControl::Update_Hover(HWND window, Point2D const & point) // RA2: 0x00768840
{
	if (Is_Done()) { // RA2: the hover test does nothing once the dialog is done (virtual slot 2, called at 0x00768846)
		return; // RA2: as above
	}
	Current = Image1; // RA2: the neutral picture is shown first and the regions of Allies and Soviets override it
	int side = 0; // RA2: as above
	int x = point.X; // RA2: as above
	do { // RA2: as above
		if ((x >= 16 && x < 364 && point.Y >= 20 && point.Y < 136) || // RA2: as above
		(x >= 130 && x < 300 && point.Y >= 100 && point.Y < 216) || // RA2: as above
		(x >= 175 && x < 245 && point.Y >= 216 && point.Y < 262)) { // RA2: as above
			side = 2; // RA2: as above
			Current = Image2; // RA2: as above
			break;
		}
		if ((x >= 252 && x < 466 && point.Y >= 180 && point.Y < 382) || // RA2: the Allied regions are tested first and leave through a break, the Soviet regions follow (the executable lays the Allied body out after the Soviet one)
			(x >= 212 && x < 496 && point.Y >= 262 && point.Y < 422)) { // RA2: as above
			side = 3; // RA2: as above
			Current = Image3; // RA2: as above
		}
	} while (false); // RA2: as above
	if (Selection != side) { // RA2: as above
		Selection = side; // RA2: as above
		InvalidateRect(window, NULL, FALSE); // RA2: as above
	}
}


/// <summary>
/// Draws the picture of the side the mouse is over.
/// </summary>
void WorldDominationTour::FactionSelectDialogControl::Draw(int unused) // RA2: 0x00768940
{
	if (AlternateSurface != NULL && Current != NULL) { // RA2: the picture is blitted to the alternate surface over a black fill of its own rectangle (0x00768940)
		Rect rect = Current->Get_Rect(); // RA2: as above
		AlternateSurface->Fill_Rect(rect, 0); // RA2: as above
		AlternateSurface->Blit_From(rect, *Current, rect, false, true); // RA2: as above
	}
}


/// <summary>
/// Handles the dialog messages for the tour side choice.
/// This routine gives the owner draw default handler first refusal, tracks the mouse over the
/// two sides, paints the pictures and ends the dialog with the side that was clicked.
/// </summary>
/// <returns>
/// Returns with the result of the owner draw handler, or FALSE if it left the message alone.
/// </returns>
BOOL CALLBACK Faction_Select_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) // RA2: 0x007689C0
{
	if (message == WM_INITDIALOG && lparam != 0) { // RA2: the creation record (resource, control) hands the control to the dialog before the default handler runs (0x007689C0)
		SetWindowLong(window, DWL_USER, ((int *)lparam)[1]); // RA2: the dialog's parameter is the control (the second field of the creation record)
	}

	int rc = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (rc != 0) { // RA2: the default handler goes first and the answer is returned as it is
		return(rc);
	}

	FactionSelectDialogControl * control = (FactionSelectDialogControl *)GetWindowLong(window, DWL_USER); // RA2: the control stored at WM_INITDIALOG

	switch (message) {
		case WM_NCHITTEST: { // RA2: the mouse position (message 0x84, lparam split into x and y) goes to the hover test
			Point2D point(LOWORD(lparam), HIWORD(lparam)); // RA2: as above
			control->Update_Hover(window, point); // RA2: as above
			return(FALSE);
		}

		case WM_DRAWITEM: // RA2: the owner draw items are drawn by the dialog draw handler (0x005FE380)
			Dialog_On_WM_DRAWITEM(lparam); // RA2: as above
			return(TRUE); // RA2: as above

		case WM_PAINT: // RA2: as above
			Dialog_On_WM_PAINT(window); // RA2: as above
			control->Draw(0); // RA2: as above
			Slide_Pending_Dialog(window); // RA2: as above
			ValidateRect(window, NULL); // RA2: as above
			return(TRUE); // RA2: as above

		case WM_ERASEBKGND: // RA2: as above
			return(TRUE); // RA2: as above

		case WM_COMMAND: // RA2: as above
			if (LOWORD(wparam) == 0x686) { // RA2: as above
				control->Set_Window((unsigned int)window, (HWND)-1); // RA2: as above
				return(FALSE);
			}
			break;

		case 0x497: // RA2: message 0x497 is left alone
			return(FALSE);

		case 0x4E9: { // RA2: the tool tip request; the text depends on the side under the mouse
			ToolTipTextRequest * request = (ToolTipTextRequest *)lparam; // RA2: as above
			if (request->Window == window) { // RA2: as above
				wchar_t const * text; // RA2: as above
				switch (control->Selection) { // RA2: as above
					case 2: // RA2: as above
						text = StringTable::LoadString("WDT:FactionSelectAlliesToolTip", NULL, "D:\\Ra2\\WorldDom.cpp", 176); // RA2: as above
						break;
					case 3: // RA2: as above
						text = StringTable::LoadString("WDT:FactionSelectSovietToolTip", NULL, "D:\\Ra2\\WorldDom.cpp", 179); // RA2: as above
						break;
					default: // RA2: as above
						text = StringTable::LoadString("WDT:FactionSelectToolTip", NULL, "D:\\Ra2\\WorldDom.cpp", 182); // RA2: as above
						break;
				}
				WideWstring tip(text); // RA2: the text is answered as a wide string
				request->Text = tip; // RA2: as above
			}
			return(FALSE);
		}

		case WM_LBUTTONUP: // RA2: a click on a side ends the dialog with it (Set_Window with 1)
			if (control->Selection != 0) { // RA2: as above
				control->Set_Window((unsigned int)window, (HWND)1); // RA2: as above
			}
			break;
	}
	return(FALSE);
}


/// <summary>
/// Loads a side choice picture.
/// </summary>
static inline void Load_Image(WorldDominationTour::FactionImage *& image, char const * name) // RA2: as in the executable's FactionSelectDialogControl constructor
{
	CCFileClass file(name); // RA2: the constructor of the side choice control loads each picture through this helper (inlined there)
	image = Read_PCX_File(file); // RA2: as above
}


/// <summary>
/// Asks the player which side to fight for in the tour.
/// This routine darkens the screen behind it and runs the side choice dialog.
/// </summary>
/// <returns>Returns with the side the player chose: 2 for the Allies, 3 for the Soviets, or zero if the choice was declined.</returns>
int Faction_Select_Dialog(void) // RA2: 0x00768C70
{
	Darken_Surface(NULL, true); // RA2: the screen behind the dialog is darkened, and the control is a stack object run by Controlled_Dialog (0x006011A0)
	FactionSelectDialogControl control; // RA2: as above
	OwnerDraw::Controlled_Dialog(0x125, (DLGPROC)Faction_Select_Dialog_Proc, &control); // RA2: as above
	return(control.Selection); // RA2: as above
}


/// <summary>
/// Asks the player which theater of the tour to fight in.
/// This routine runs the graphic menu of the chosen side.
/// </summary>
/// <param name="faction">The side the player chose.</param>
/// <returns>Returns with the theater the player picked, or zero if the choice was declined.</returns>
int __fastcall Theater_Choice_Menu(int faction) // RA2: 0x00768DD0
{
	char buffer[128]; // RA2: the menu name is "TheaterChoice<Faction_Name>Menu" in WDTChoice.ini

	sprintf(buffer, "TheaterChoice%sMenu", Faction_Name(faction)); // RA2: as above
	GraphicMenu *gmenu = Do_Graphic_Menu("WDTChoice.ini", buffer); // RA2: as above
	if (gmenu == NULL) {
		return(0);
	}
	int rc = gmenu->Presentation();
	delete gmenu;
	switch (rc) {
		case 1: return(1); // RA2: the menu returns 1 to 3, the theater picked
		case 2: return(2); // RA2: as above
		case 3: return(3); // RA2: as above
	}
	return(0);
}


int g_WDTChosenSide;
int g_WDTChosenTheater; // RA2: the theater picked, a global next to the side (0x00B2ADA8)

enum WDTSetupState { // RA2: the steps of Setup_Game
	WDT_SETUP_DONE = 0, // RA2: as above
	WDT_SETUP_CHOOSE_SIDE = 1, // RA2: as above
	WDT_SETUP_PLAY_THEATER_MOVIE = 2, // RA2: as above
	WDT_SETUP_CHOOSE_THEATER = 3, // RA2: as above
	WDT_SETUP_SELECT = 4, // RA2: as above
	WDT_SETUP_PLAY = 5, // RA2: as above
	WDT_SETUP_FINISHED = 6 // RA2: as above
};


/// <summary>
/// Sets up a World Domination Tour game for play.
/// This routine makes sure the screen is no wider than 640 pixels, mounts the tour data files, asks the player which side to fight for,
/// negotiates a cycle with the tour server and then hands the campaign to the mission
/// selection screen. A player who abandons the selection is offered the side choice again
/// rather than being dumped back to the main menu.
/// </summary>
/// <param name="first_time">Is this the first tour game since the player logged in?</param>
/// <returns>bool; Was a tour match played?</returns>
int WDT_Setup_Game(bool first_time) // RA2: returns int, the result of the match state machine (0x00768E90)
{
	if (VideoModeWidth > 640) { // RA2: the tour needs 640x480: a wider screen asks the player to switch (WOL:WDTResolutionQuery) and saves the setting
		WideWstring query(StringTable::LoadString("WOL:WDTResolutionQuery", NULL, "D:\\Ra2\\WorldDom.cpp", 385)); // RA2: as above
		if (ODMessageBox(query.get(), 4, WOL_Wait_Callback, true) != 6) { // RA2: as above
			return(false);
		}
		if (!Test_Display_Mode_Dialog(640, 480)) { // RA2: as above
			return(false);
		}
		Options.ScreenWidth = 640; // RA2: as above
		Options.ScreenHeight = 480; // RA2: as above
		Options.Save_Settings(); // RA2: as above
	}

	MFCD *wdtmix = NULL;
	MFCD *wdtvoxmix = NULL;
	MFCD *localmix = NULL;

	if (CCFileClass("WDT.MIX").Is_Available()) {
		wdtmix = new MFCD("WDT.MIX", &FastKey);
	}

	if (CCFileClass("WDTVOX.MIX").Is_Available()) {
		wdtvoxmix = new MFCD("WDTVOX.MIX", &FastKey);
	}

	if (CCFileClass("Local.MIX").Is_Available()) {
		localmix = new MFCD("Local.MIX", &FastKey);
	}

	if (wdtmix != NULL && localmix != NULL) {
		int state = first_time ? WDT_SETUP_SELECT : WDT_SETUP_CHOOSE_SIDE; // RA2: a state machine (0x00768E90) replaces TibSun's valid/done flags
		while (state != WDT_SETUP_DONE && state != WDT_SETUP_FINISHED) { // RA2: as above
			switch (state) { // RA2: as above
				case WDT_SETUP_CHOOSE_SIDE: // RA2: as above
					g_WDTChosenSide = Faction_Select_Dialog(); // RA2: as above
					state = g_WDTChosenSide != 0 ? WDT_SETUP_SELECT : WDT_SETUP_DONE; // RA2: as above
					break;

				case WDT_SETUP_PLAY_THEATER_MOVIE: { // RA2: as above
					char name[128]; // RA2: as above
					sprintf(name, "WDTTheater%02.VQA", g_WDTChosenSide); // RA2: as above
					if (CCFileClass(name).Is_Available()) { // RA2: as above
						Play_Movie(name, THEME_NONE, false, true, false, false); // RA2: as above
					}
					state = WDT_SETUP_CHOOSE_THEATER; // RA2: as above
					break;
				}

				case WDT_SETUP_CHOOSE_THEATER: // RA2: as above
					g_WDTChosenTheater = Theater_Choice_Menu(g_WDTChosenSide); // RA2: as above
					state = g_WDTChosenTheater != 0 ? WDT_SETUP_SELECT : WDT_SETUP_CHOOSE_SIDE; // RA2: as above
					break;

				case WDT_SETUP_SELECT: { // RA2: the campaign is taken from the counted pointer the server request fills
					if (!Request_WDT_Cycle()) { // RA2: as above
						state = WDT_SETUP_DONE; // RA2: as above
						break;
					}
					rc_ptr<Campaign> campaign = Current_WDT_Campaign(); // RA2: as above
					if (campaign.Get() == NULL) { // RA2: as above
						return(false);
					}
					campaign->Set_Faction(g_WDTChosenSide);
					if (campaign->Is_Different_Cycle()) {
						rc_ptr<Campaign> previous = Previous_WDT_Campaign(); // RA2: the earlier cycle is reviewed first when it is not itself a different cycle
						if (previous.Get() != NULL && !previous->Is_Different_Cycle()) { // RA2: as above
							Selection::Show(previous, g_WDTChosenTheater); // RA2: as above
						}
					}
					Session.House = g_WDTChosenSide == 3; // RA2: the player's house is 1 for the Soviets and 0 for the Allies
					if (Selection::Choose_Conflict(campaign, g_WDTChosenTheater)) { // RA2: as above
						g_MaxPlayers = 0;
						RandomMapGen.SeedData.Fixup_WDT_Settings();
						state = WDT_SETUP_PLAY; // RA2: the mission selection now leads to the match itself instead of ending the setup
					} else {
						state = WDT_SETUP_CHOOSE_SIDE; // RA2: as above
					}
					break;
				}

				case WDT_SETUP_PLAY: // RA2: as above
					if (Start_Quick_Match() == 1) { // RA2: as above
						state = WDT_SETUP_FINISHED; // RA2: as above
					} else {
						state = WDT_SETUP_SELECT; // RA2: as above
					}
					break;
			}
		}

		delete wdtmix;
		delete localmix;
		delete wdtvoxmix;

		return(state == WDT_SETUP_FINISHED); // RA2: the result is whether the match was played
	}

	delete wdtmix;
	delete localmix;
	delete wdtvoxmix;

	return(false);
}


WDTPointer<void> g_WDTUnusedPointer1; // RA2: 0x00B2AD60
WDTPointer<void> g_WDTUnusedPointer2; // RA2: 0x00B2AD70
rc_ptr<WDTState> g_WDTNewState; // RA2: 0x00B2AD68
rc_ptr<WDTState> g_WDTResumedState; // RA2: 0x00B2AD80
rc_ptr<Campaign> g_WDTNewCampaign; // RA2: 0x00B2AD78
rc_ptr<Campaign> g_WDTResumedCampaign; // RA2: 0x00B2AD88


/// <summary>
/// Fetches the tour state the new campaign was built from.
/// </summary>
/// <returns>Returns with the holder of the new state.</returns>
rc_ptr<WDTState> const & __fastcall Current_WDT_State(void) // RA2: 0x00769510
{
	return(g_WDTNewState); // RA2: the state is a counted pointer; the getter returns the holder itself (0x00769510)
}


/// <summary>
/// Starts a fresh World Domination Tour campaign.
/// This routine adopts the state block the tour server handed back and builds the campaign
/// object that the tour screens work through for the logged in player.
/// </summary>
/// <param name="state">The tour state to start the campaign from.</param>
void __fastcall WDT_Store_Current(rc_ptr<WDTState> const & state) // RA2: 0x00769520
{
	g_WDTNewState = state;
	g_WDTNewCampaign = Campaign::Create(g_NickName, CampaignProperties::Create(state)); // RA2: the campaign is created from the state by the counted constructors (0x00769520)
}


/// <summary>
/// Fetches the tour state the resumed campaign was built from.
/// </summary>
/// <returns>Returns with the holder of the resumed state.</returns>
rc_ptr<WDTState> const & __fastcall Previous_WDT_State(void) // RA2: 0x00769590
{
	return(g_WDTResumedState); // RA2: as above (0x00769590)
}


/// <summary>
/// Resumes a World Domination Tour campaign already in progress.
/// This routine adopts the state block the tour server handed back and builds the campaign
/// object that the tour screens work through for the logged in player.
/// </summary>
/// <param name="state">The tour state to resume the campaign from.</param>
void __fastcall WDT_Store_Previous(rc_ptr<WDTState> const & state) // RA2: 0x007695A0
{
	g_WDTResumedState = state;
	g_WDTResumedCampaign = Campaign::Create(g_NickName, CampaignProperties::Create(state)); // RA2: as above (0x007695A0)
}


/// <summary>
/// Fetches the first of the tour's spare state pointers.
/// </summary>
/// <returns>Returns with the pointer held, or NULL if none was ever set.</returns>
void * WDT_Get_Unused_Pointer1(void)
{
	return(*g_WDTUnusedPointer1);
}


/// <summary>
/// Sets the first of the tour's spare state pointers.
/// </summary>
/// <param name="p">The pointer to take ownership of.</param>
void WDT_Set_Unused_Pointer1(void *p)
{
	g_WDTUnusedPointer1 = p;
}


/// <summary>
/// Fetches the second of the tour's spare state pointers.
/// </summary>
/// <returns>Returns with the pointer held, or NULL if none was ever set.</returns>
void * WDT_Get_Unused_Pointer2(void)
{
	return(*g_WDTUnusedPointer2);
}


/// <summary>
/// Sets the second of the tour's spare state pointers.
/// </summary>
/// <param name="p">The pointer to take ownership of.</param>
void WDT_Set_Unused_Pointer2(void *p)
{
	g_WDTUnusedPointer2 = p;
}


/// <summary>
/// Fetches the campaign the player has just begun.
/// </summary>
/// <returns>Returns with the holder of the new campaign.</returns>
rc_ptr<Campaign> const & __fastcall Current_WDT_Campaign(void) // RA2: 0x00769670
{
	return(g_WDTNewCampaign); // RA2: a counted pointer holder is returned (0x00769670)
}


/// <summary>
/// Fetches the campaign the player is picking up again.
/// </summary>
/// <returns>Returns with the holder of the resumed campaign.</returns>
rc_ptr<Campaign> const & __fastcall Previous_WDT_Campaign(void) // RA2: 0x00769680
{
	return(g_WDTResumedCampaign); // RA2: as above (0x00769680)
}


/// <summary>
/// Fetches the territory that one of the campaign's conflicts is fought over.
/// This routine is used to resolve a conflict slot of the new campaign into the piece of
/// the world map it disputes.
/// </summary>
/// <param name="index">Index of the conflict to look up.</param>
/// <returns>
/// Returns with the territory in dispute. Otherwise, an empty pointer is returned.
/// </returns>
rc_ptr<WDTTerritory> Find_WDT_Territory(int index) // RA2: 0x00769690
{
	CONFLICT_ITER it = g_WDTNewCampaign->Get_Conflict_Iterator(index); // RA2: the territory is returned as a counted pointer (0x00769690)
	return(it ? (*it)->Territory : rc_ptr<WDTTerritory>()); // RA2: as above
}


/// <summary>
/// Determines if the World Domination Tour is available.
/// This routine guards every entry point into the tour. RA2 has no addon check: the tour is
/// always available.
/// </summary>
/// <returns>bool; Is the tour available to the player?</returns>
bool WDT_Is_Allowed(void)
{
	return(true); // RA2: the tour is always available (0x00769730)
}


inline WorldDominationTour::FactionSelectDialogControl::FactionSelectDialogControl(void) // RA2: 0x00769790
	: Selection(0), Image1(NULL), Image2(NULL), Image3(NULL) // RA2: as above
{
	if (Image1 == NULL) { // RA2: each picture is loaded once (constructor 0x00769790)
		Load_Image(Image1, "WDTFSBSm.pcx"); // RA2: as above
	}
	if (Image2 == NULL) { // RA2: as above
		Load_Image(Image2, "WDTFSAsm.pcx"); // RA2: as above
	}
	if (Image3 == NULL) { // RA2: as above
		Load_Image(Image3, "WDTFSSsm.pcx"); // RA2: as above
	}
	Current = Image1; // RA2: as above
}
