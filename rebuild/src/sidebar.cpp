/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /counterstrike/SIDEBAR.CPP 2     3/17/97 1:05a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SIDEBAR.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 20, 1994                                             *
 *                                                                                             *
 *                  Last Update : October 9, 1996 [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   SidebarClass::AI -- Handles player clicking on sidebar area.                              *
 *   SidebarClass::Abandon_Production -- Stops production of the object specified.             *
 *   SidebarClass::Activate -- Controls the sidebar activation.                                *
 *   SidebarClass::Activate_Demolish -- Controls the demolish button on the sidebar.           *
 *   SidebarClass::Activate_Repair -- Controls the repair button on the sidebar.               *
 *   SidebarClass::Activate_Upgrade -- Controls the upgrade button on the sidebar.             *
 *   SidebarClass::Add -- Adds a game object to the sidebar list.                              *
 *   SidebarClass::Draw_It -- Renders the sidebar display.                                     *
 *   SidebarClass::Factory_Link -- Links a factory to a sidebar strip.                         *
 *   SidebarClass::Init_Clear -- Sets sidebar to a known (and deactivated) state               *
 *   SidebarClass::Init_IO -- Adds buttons to the button list                                  *
 *   SidebarClass::Init_Theater -- Performs theater-specific initialization                    *
 *   SidebarClass::One_Time -- Handles the one time game initializations.                      *
 *   SidebarClass::One_Time -- Handles the one time game initializations.                      *
 *   SidebarClass::Recalc -- Examines the sidebar data and updates it as necessary.            *
 *   SidebarClass::Refresh_Cells -- Intercepts the refresh, looking for sidebar controls.      *
 *   SidebarClass::SBGadgetClass::Action -- Special function that controls the mouse over the s*
 *   SidebarClass::Scroll -- Handles scrolling the sidebar object strip.                       *
 *   SidebarClass::Set_Current -- Sets a specified object that controls the sidebar display.   *
 *   SidebarClass::SidebarClass -- Default constructor for the sidebar.                        *
 *   SidebarClass::SidebarClass -- This is the no initialization constructor for the sidebar.  *
 *   SidebarClass::StripClass::AI -- Input and AI processing for the side strip.               *
 *   SidebarClass::StripClass::Abandon_Produ -- Abandons production associated with sidebar.   *
 *   SidebarClass::StripClass::Activate -- Adds the strip buttons to the input system.         *
 *   SidebarClass::StripClass::Add -- Add an object to the side strip.                         *
 *   SidebarClass::StripClass::Deactivate -- Removes the side strip buttons from the input syst*
 *   SidebarClass::StripClass::Draw_It -- Render the sidebar display.                          *
 *   SidebarClass::StripClass::Factory_Link -- Links a factory to a sidebar button.            *
 *   SidebarClass::StripClass::Flag_To_Redra -- Flags the sidebar strip to be redrawn.         *
 *   SidebarClass::StripClass::Get_Special_Cameo -- Fetches the special event cameo shape.     *
 *   SidebarClass::StripClass::Init_Clear -- Sets sidebar to a known (and deactivated) state   *
 *   SidebarClass::StripClass::Init_IO -- Adds buttons to the button list                      *
 *   SidebarClass::StripClass::Init_Theater -- Performs theater-specific initialization        *
 *   SidebarClass::StripClass::One_Time -- Performs one time actions necessary for the side str*
 *   SidebarClass::StripClass::Recalc -- Revalidates the current sidebar list of objects.      *
 *   SidebarClass::StripClass::Scroll -- Causes the side strip to scroll.                      *
 *   SidebarClass::StripClass::SelectClass:: -- Action function when buildable cameo is selecte*
 *   SidebarClass::StripClass::SelectClass:: -- Assigns special values to a buildable select bu*
 *   SidebarClass::StripClass::SelectClass::SelectClass -- Default constructor.                *
 *   SidebarClass::StripClass::StripClass -- Default constructor for the side strip class.     *
 *   SidebarClass::Which_Column -- Determines which column a given type should appear.         *
 *   SidebarClass::Zoom_Mode_Control -- Handles the zoom mode toggle operation.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "ownrdraw.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "sidebar.h"
#include "bitfont.h" // RA2: the includes this file needs

#include "_bench.h"
#include "_convert.h"
#include "_map.h"
#include "_mixfile.h"
#include "_rtti.h"
#include "_rules.h"
#include "_surface.h"
#include "_tooltip.h"
#include "bench.h"
#include "building.h"
#include "cctooltip.h"
#include "conquer.h"
#include "convert.h"
#include "data.h"
#include "dialog.h"
#include "draw.h"
#include "event.h"
#include "factory.h"
#include "font.h"
#include "globals.h"
#include "goptions.h"
#include "house.h"
#include "stringtable.h" // RA2: the includes this file needs
#include "dsurface.h" // RA2: the includes this file needs
#include "scenario.h" // RA2: the includes this file needs
#include "houstype.h" // RA2: the includes this file needs
#include "incdec.h"
#include "language\language.h"
#include "map.h"
#include "mixfile.h"
#include "movie.h"
#include "queue.h"
#include "rect.h"
#include "rules.h"
#include "scheme.h"
#include "session.h"
#include "shapeset.h"
#include "super.h"
#include "suprtype.h"
#include "surface.h"
#include "voc.h"
#include "vox.h"

#include "bench.hh"
#include "color.hh"
#include "bittext.h" // RA2: the includes this file needs
#include "ftimer.h" // RA2: the includes this file needs

inline int Bottom_Extra(void) // RA2: the strips leave this much room for the scroll buttons, inline in the strip's Draw_It (0x67F800)
{
	return Scen->IsGDI ? 26 : 18; // RA2: as above
}
inline int Max_Visible_Rows(void) // RA2: the rows that fit, inline in Draw_It (0x67CF80) and AI (0x67DC90); Get_Row_Count (0x6820A0) has the same term out of line
{
	return (SidebarRect.Y + SidebarRect.Height - SidebarClass::StripTop - (Scen->IsGDI ? 26 : 18) - 7) / 50; // RA2: as above
}
PaletteClass * Get_Game_Palette(void); // RA2: 0x6F45E0, the pointer at 0xAC11A0 (the sidebar layout's module)
void Set_Tab_Building(BuildingClass * building); // RA2: 0x6F8A00; the structure or defense tab (by the building type's build category) shows this building
void Clear_Sidebar_Tab_Object(TechnoClass * techno); // RA2: 0x6F8A20 (scenario.cpp), forgets techno in the sidebar tab slots
namespace Unsorted { extern bool ArmageddonMode; } // RA2: as above
namespace Game { extern bool IsActive; extern HWND hWnd; } // RA2: as above
namespace Unsorted { extern bool ScenarioStarted; } // RA2: as above
namespace Game { extern unsigned char RecordingFlag; } // RA2: as above
namespace Game { extern int SpecialDialog; } // RA2: as above
namespace Unsorted { extern int WSDialogCount; } // RA2: as above
Rect * Drawing_GetTextDimensions(Rect * out, wchar_t const * text, Point2D loc, unsigned short flags, int marginX, int marginY); // RA2: 0x497360 (dialog.cpp)
static inline Rect Text_Rect(wchar_t const * text, Point2D loc, unsigned short flags, int marginx, int marginy) {Rect rect; return(*Drawing_GetTextDimensions(&rect, text, loc, flags, marginx, marginy));} // RA2: the box a text needs


ShapeSet const * SidebarClass::SidebarShapes[SBSHAPE_COUNT]; // RA2: 0xABCD20; one array instead of TibSun's four shapes
bool SidebarClass::SidebarShapeAllocated[SBSHAPE_COUNT]; // RA2: 0xABCD40
int SidebarClass::TopButtonX; // RA2: 0xABCD48
int SidebarClass::TopButtonY; // RA2: 0xABCD4C
int SidebarClass::TopButtonSpacing; // RA2: 0xABCD50
int SidebarClass::TabButtonX; // RA2: 0xABCD54
int SidebarClass::TabButtonY; // RA2: 0xABCD58
int SidebarClass::TabButtonSpacing; // RA2: 0xABCD5C
int SidebarClass::StripLeft; // RA2: 0xABCD60
int SidebarClass::StripTop; // RA2: 0xABCD64
int SidebarClass::CameoWidth; // RA2: 0xABCD68
int SidebarClass::CameoHeight; // RA2: 0xABCD6C
int SidebarClass::PowerBarLeft; // RA2: 0xABCD74
int SidebarClass::SidebarBottomY; // RA2: 0xABCD78
int SidebarClass::SidebarEdgeWidth; // RA2: 0xABCD7C
int SidebarClass::SidebarEdgeStep; // RA2: 0xABCD80
bool IsCameoTextShown; // RA2: 0x8373C0; set, the cameos carry their names and the help text shows the cost only
bool SidebarClass::IsSidebarRedrawPending; // RA2: 0x8373C3
wchar_t ButtonHelpBuffer[256]; // RA2: 0xAB9274; the help text of a scroll or tab button marked as disabled
wchar_t CameoHelpBuffer[66]; // RA2: 0xAB9484; the help text of a cameo

/***************************************************************************
**	This points to the main sidebar shapes. These include the upgrade and
**	repair buttons.
*/
//TheaterType SidebarClass::StripClass::LastTheater = THEATER_NONE;

enum ButtonNumberType {
	BUTTON_RADAR = 100,
	BUTTON_REPAIR,
	BUTTON_POWER,
	BUTTON_SELL,
	BUTTON_UPGRADE,
	BUTTON_WAYPOINT
};

/*
**	Sidebar buttons
*/
SidebarClass::SBGadgetClass SidebarClass::Background; // RA2: 0xAB9718
ShapeButtonClass SidebarClass::Repair; // RA2: 0xAB96B8
ShapeButtonClass SidebarClass::Upgrade; // RA2: 0xABCC58
ShapeButtonClass SidebarClass::TabButton[STRIPS]; // RA2: 0xAB9508
ShapeButtonClass SidebarClass::ScrollDownButton; // RA2: 0xABCBE8
ShapeButtonClass SidebarClass::ScrollUpButton; // RA2: 0xABCCC0
SidebarClass::StripClass::SelectClass SidebarClass::StripClass::SelectButton[STRIPS * MAX_SLOTS_PER_STRIP]; // RA2: 0xAB9740

bool SidebarClass::IsToBlitSidebar = false;
bool SidebarClass::IsToEraseSidebarMouse = false;

/*
**	Shape data pointers
*/
ShapeSet const * SidebarClass::StripClass::DarkenShapes; // RA2: 0xAB9480

void Print_Cameo_Text(wchar_t const * string, Point2D const & point, Rect const & cliprect, int maxlinelen); // RA2: wide text; the flag is a global
/// <summary>
/// Default constructor for the sidebar background gadget.
/// This gadget exists to cover the sidebar so that the mouse reverts to its normal shape
/// whenever it strays over there. The sidebar gives the gadget its real position and size
/// when it lays itself out.
/// </summary>
SidebarClass::SBGadgetClass::SBGadgetClass(void) :
	GadgetClass(0, 0, 1, 1, LEFTUP)
{
}


/***********************************************************************************************
 * SidebarClass::SidebarClass -- Default constructor for the sidebar.                          *
 *                                                                                             *
 *    Constructor for the sidebar handler. It basically sets up the sidebar to the empty       *
 *    condition.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
SidebarClass::SidebarClass(void) :
	BASECLASS(),
	ActiveTabIndex(0), // RA2: 0x67B700
	RedrawUntilFrame(0), // RA2: as above
	IsCameoText(false),
	IsSidebarActive(false),
	IsToRedraw(true),
	IsToRedrawCredits(false),
	IsDemolishActive(false), // RA2: as above
	IsRepairActive(false) // RA2: as above
{
	/*
	**	Set up the coordinates for the sidebar strips. These coordinates are for
	**	the upper left corner.
	**	Set up the strips (RA2 has four, one per tab).
	*/
	for (int index = 0; index < STRIPS; index++) { // RA2: four strips
		new (&Column[index]) StripClass(InitClass()); // RA2: as above
	}
}


/***********************************************************************************************
 * SidebarClass::SidebarClass -- This is the no initialization constructor for the sidebar.    *
 *                                                                                             *
 *    Unlike the normal constructor, this one doesn't do any initialization. There is one      *
 *    exception to this. The stip classes can't call an explicit NoInitClass constructor       *
 *    since they are an array. Since the default constructor is called for these strips, we    *
 *    must reset the X and Y location to what we know they should be.                          *
 *                                                                                             *
 * INPUT:   flag to indicate that this is a no initialization constructor.                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
SidebarClass::SidebarClass(NoInitClass const & x) :
	BASECLASS(x)
{

}


/// <summary>
/// Turns the cameo cost text on or off.
/// This routine is called when the player changes the option. With the text enabled, every
/// build cameo is captioned with its cost, so the sidebar is flagged to redraw itself.
/// </summary>
/// <param name="on">Should the cameo text be displayed?</param>
void SidebarClass::Set_Cameo_Text(bool state) // RA2: as above
{
	// RA2: 0x67B880; the body is empty (the caption flag is a global here)
}


/***********************************************************************************************
 * SidebarClass::One_Time -- Handles the one time game initializations.                        *
 *                                                                                             *
 *    This routine is used to load the graphic data that is needed by the sidebar display. It  *
 *    should only be called ONCE.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once when the game first starts.                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/94   JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::One_Time(void)
{
	BASECLASS::One_Time();

	for (int index = 0; index < STRIPS; index++) { // RA2: as above
		Column[index].One_Time(index); // RA2: as above
	}
}


/***********************************************************************************************
 * SidebarClass::Init_Clear -- Sets sidebar to a known (and deactivated) state                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::Init_Clear(void)
{
	PowerClass::Init_Clear(); // RA2: as above
	IsToRedraw = true;
	IsRepairActive = false;
	IsDemolishActive = false;
	for (int index = 0; index < 4; index++) { // RA2: as above
		Column[index].Clear_Buildables(); // RA2: as above
	}
	ActiveTabIndex = 0; // RA2: as above
	RedrawUntilFrame = 0; // RA2: as above
	Activate(0); // RA2: as above
}


/***********************************************************************************************
 * SidebarClass::Init_IO -- Adds buttons to the button list                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::Recalc_Layout() // RA2: as above
{
	SidebarRect.Width = SidebarWidth; // RA2: as above
	SidebarRect.Y = 0x9E; // RA2: as above
	SidebarRect.Height = TacticalRect.Y + TacticalRect.Height + TabBarHeight - 0x9E; // RA2: as above
	if (Scen->IsGDI) { // RA2: as above
		TopButtonY = 0xA6; // RA2: as above
		TopButtonSpacing = 0x40; // RA2: as above
		TabButtonY = 0xC5; // RA2: as above
		TabButtonSpacing = 0x1D; // RA2: as above
		StripTop = 0xE3; // RA2: as above
		CameoWidth = 0x3F; // RA2: as above
	} else {
		TopButtonY = 0xA5; // RA2: as above
		TopButtonSpacing = 0x34; // RA2: as above
		TabButtonY = 0xC5; // RA2: as above
		TabButtonSpacing = 0x20; // RA2: as above
		StripTop = 0xE3; // RA2: as above
		CameoWidth = 0x40; // RA2: as above
	}
	CameoHeight = 0x32; // RA2: as above
	int extra; // RA2: as above
	if (Scen->IsGDI) { // RA2: as above
		extra = 26; // RA2: as above
	} else {
		extra = 18; // RA2: as above
	}
	PowerBarHeight = (SidebarRect.Height - extra - 0x4C) / 50 * 50; // RA2: as above
	if (Scen->IsGDI) { // RA2: as above
		SidebarBottomY = PowerBarHeight + 0xEA; // RA2: as above
		SidebarEdgeWidth = 0x2E; // RA2: as above
		SidebarEdgeStep = 0x32; // RA2: as above
	} else {
		SidebarBottomY = PowerBarHeight + 0xEA; // RA2: as above
		SidebarEdgeWidth = 0x2D; // RA2: as above
		SidebarEdgeStep = 0x32; // RA2: as above
	}
}


void SidebarClass::Recalc_Horizontal_Layout() // RA2: as above
{
	int x = Options.IsSidebarOnRight ? TacticalRect.X + TacticalRect.Width : 0; // RA2: as above
	SidebarRect.X = x; // RA2: as above
	StripLeft = x + 22; // RA2: as above
	if (Scen->IsGDI) { // RA2: as above
		TopButtonX = x + 20; // RA2: as above
		TabButtonX = x + 26; // RA2: as above
		PowerBarLeft = x + 39; // RA2: as above
	} else {
		TopButtonX = x + 33; // RA2: as above
		TabButtonX = x + 20; // RA2: as above
		PowerBarLeft = x + 39; // RA2: as above
	}
}


void SidebarClass::Init_IO() // RA2: as above
{
	RadarClass::Init_IO(); // RA2: as above
	Recalc_Layout(); // RA2: as above
	Recalc_Horizontal_Layout(); // RA2: as above
	if (Debug_Map) { // RA2: as above
		return;
	}

	Upgrade.IsSticky = true;
	Upgrade.ID = 101; // RA2: as above
	Upgrade.X = TopButtonX; // RA2: as above
	Upgrade.Y = TopButtonY; // RA2: as above
	Upgrade.DrawOffsetX = -480; // RA2: as above
	Upgrade.DrawOffsetY = 0; // RA2: as above
	Upgrade.DrawOnSidebar = true;
	Upgrade.ShapeDrawer = SidebarDrawer;
	Upgrade.IsOn = false; // RA2: as above
	Upgrade.IsToggleType = 1; // RA2: as above
	Upgrade.ReflectButtonState = true;
	Upgrade.Stop_Flashing(); // RA2: as above

	/*
	**	Add the sidebar's buttons only if we're not in editor mode.
	*/
	Repair.IsSticky = true;
	Repair.ID = 102; // RA2: as above
	Repair.X = Upgrade.X + TopButtonSpacing; // RA2: as above
	Repair.Y = Upgrade.Y; // RA2: as above
	Repair.DrawOffsetX = -480; // RA2: as above
	Repair.DrawOffsetY = 0; // RA2: as above
	Repair.DrawOnSidebar = true;
	Repair.ShapeDrawer = SidebarDrawer;
	Repair.IsOn = false; // RA2: as above
	Repair.IsToggleType = 1; // RA2: as above
	Repair.ReflectButtonState = true;
	Repair.Stop_Flashing(); // RA2: as above

	for (int index = 0; index < 4; index++) { // RA2: as above
		TabButton[index].IsSticky = true; // RA2: as above
		TabButton[index].ID = 203 + index; // RA2: as above
		TabButton[index].X = TabButtonSpacing * index + TabButtonX; // RA2: as above
		TabButton[index].Y = TabButtonY; // RA2: as above
		TabButton[index].DrawOffsetX = -480; // RA2: as above
		TabButton[index].DrawOffsetY = 0; // RA2: as above
		TabButton[index].DrawOnSidebar = true; // RA2: as above
		TabButton[index].ShapeDrawer = SidebarDrawer; // RA2: as above
		TabButton[index].IsOn = false; // RA2: as above
		TabButton[index].IsToggleType = 2; // RA2: as above
		TabButton[index].ReflectButtonState = true; // RA2: as above
		TabButton[index].Disable(); // RA2: as above
		TabButton[index].Stop_Flashing(); // RA2: as above
	}

	ScrollDownButton.IsSticky = true; // RA2: as above
	ScrollDownButton.ID = 201; // RA2: as above
	ScrollDownButton.DrawOnSidebar = true; // RA2: as above
	ScrollDownButton.ShapeDrawer = SidebarDrawer; // RA2: as above
	ScrollDownButton.Set_Flags(0x55); // RA2: as above
	ScrollDownButton.Stop_Flashing(); // RA2: as above

	ScrollUpButton.IsSticky = true; // RA2: as above
	ScrollUpButton.ID = 200; // RA2: as above
	ScrollUpButton.DrawOnSidebar = true; // RA2: as above
	ScrollUpButton.ShapeDrawer = SidebarDrawer; // RA2: as above
	ScrollUpButton.Set_Flags(0x55); // RA2: as above
	ScrollUpButton.Stop_Flashing(); // RA2: as above

	for (int column = 0; column < 4; column++) { // RA2: as above
		Column[column].X = StripLeft; // RA2: as above
		Column[column].Y = StripTop; // RA2: as above
		Column[column].ObjectRect = Rect(0, 0, 60, PowerBarHeight); // RA2: as above
		Column[column].Init_Buttons(column); // RA2: as above
	}
	Reposition_Sidebar();
	if (IsSidebarActive) {
		Activate(0); // RA2: as above
		Activate(1);
	}
}


void SidebarClass::Init_For_House() // RA2: as above
{
	PowerClass::Init_For_House(); // RA2: as above
	PaletteClass palette = *Get_Game_Palette(); // RA2: as above
	if (SidebarDrawer) { // RA2: as above
		delete SidebarDrawer;
		SidebarDrawer = 0; // RA2: as above
	}
	SidebarDrawer = new ConvertClass(palette, palette, *VisibleSurface, 1, false); // RA2: as above
	SidebarShapes[SBSHAPE_CLOCK] = (ShapeSet const *)Load_Alloc_Data("GCLOCK2.SHP", SidebarShapeAllocated[SBSHAPE_CLOCK]); // RA2: as above

	bool allocated = false; // RA2: as above
	Repair.Set_Shape((ShapeSet const *)Load_Alloc_Data("SELL.SHP", allocated)); // RA2: as above
	Repair.Set_Shape_Loaded(allocated); // RA2: as above
	Repair.ShapeDrawer = SidebarDrawer;

		/*
		**	If a game was loaded & the sidebar was enabled, pop it up now
		*/
	Upgrade.Set_Shape((ShapeSet const *)Load_Alloc_Data("REPAIR.SHP", allocated)); // RA2: as above
	Upgrade.Set_Shape_Loaded(allocated); // RA2: as above
	Upgrade.ShapeDrawer = SidebarDrawer;

	for (int index = 0; index < 4; index++) { // RA2: as above
		char name[256]; // RA2: as above
		sprintf(name, "TAB%02d.SHP", index); // RA2: as above
		TabButton[index].Set_Shape((ShapeSet const *)Load_Alloc_Data(name, allocated)); // RA2: as above
		TabButton[index].Set_Shape_Loaded(allocated); // RA2: as above
		TabButton[index].ShapeDrawer = SidebarDrawer; // RA2: as above
	}

	ScrollDownButton.Set_Shape((ShapeSet const *)Load_Alloc_Data("R-DN.SHP", allocated)); // RA2: as above
	ScrollDownButton.Set_Shape_Loaded(allocated); // RA2: as above
	ScrollDownButton.ShapeDrawer = SidebarDrawer; // RA2: as above

	ScrollUpButton.Set_Shape((ShapeSet const *)Load_Alloc_Data("R-UP.SHP", allocated)); // RA2: as above
	ScrollUpButton.Set_Shape_Loaded(allocated); // RA2: as above
	ScrollUpButton.ShapeDrawer = SidebarDrawer; // RA2: as above

	SidebarShapes[SBSHAPE_TOP] = (ShapeSet const *)Load_Alloc_Data("SIDE1.SHP", SidebarShapeAllocated[SBSHAPE_TOP]); // RA2: as above
	SidebarShapes[SBSHAPE_MIDDLE] = (ShapeSet const *)Load_Alloc_Data("SIDE2.SHP", SidebarShapeAllocated[SBSHAPE_MIDDLE]); // RA2: as above
	SidebarShapes[SBSHAPE_BOTTOM] = (ShapeSet const *)Load_Alloc_Data("SIDE3.SHP", SidebarShapeAllocated[SBSHAPE_BOTTOM]); // RA2: as above
	SidebarShapes[SBSHAPE_ADDON] = (ShapeSet const *)Load_Alloc_Data("ADDON.SHP", SidebarShapeAllocated[SBSHAPE_ADDON]); // RA2: as above
}


void SidebarClass::DisposeOfArt() // RA2: as above
{
	PowerClass::DisposeOfArt(); // RA2: as above
	if (SidebarShapeAllocated[SBSHAPE_TOP]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_TOP]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_TOP] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_MIDDLE]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_MIDDLE]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_MIDDLE] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_BOTTOM]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_BOTTOM]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_BOTTOM] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_ADDON]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_ADDON]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_ADDON] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_BACK]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_BACK]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_SHUTTER_BACK] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_MIDDLE]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_MIDDLE]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_SHUTTER_MIDDLE] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_FRONT]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_FRONT]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_SHUTTER_FRONT] = false; // RA2: as above
	}
	if (SidebarShapeAllocated[SBSHAPE_CLOCK]) { // RA2: as above
		operator delete((void *)SidebarShapes[SBSHAPE_CLOCK]); // RA2: as above
		SidebarShapeAllocated[SBSHAPE_CLOCK] = false; // RA2: as above
	}
	Repair.Clear_Shape(); // RA2: as above
	Upgrade.Clear_Shape(); // RA2: as above
	for (int index = 0; index < 4; index++) { // RA2: as above
		TabButton[index].Clear_Shape(); // RA2: as above
	}
	ScrollDownButton.Clear_Shape(); // RA2: as above
	ScrollUpButton.Clear_Shape(); // RA2: as above
}


void SidebarClass::Request_Redraw() // RA2: as above
{
	IsToRedraw = true;
	Column[ActiveTabIndex].IsToRedraw = true; // RA2: as above
	Map.Flag_To_Redraw();
	if (RedrawUntilFrame <= Frame) { // RA2: as above
		RedrawUntilFrame = Frame + 2; // RA2: as above
	}
}


void SidebarClass::Notice_Object_Change(ObjectClass * object) // RA2: as above
{
	if (!IsSidebarActive || !object) { // RA2: as above
		return;
	}
	TechnoTypeClass const * type = object->Techno_Type_Class(); // RA2: as above
	if (!PlayerPtr) { // RA2: as above
		return;
	}
	RTTIType kind = object->Fetch_RTTI(); // RA2: as above
	int id = object->Fetch_Heap_ID(); // RA2: as above
	if (Which_Tab(kind, id, false) == ActiveTabIndex) { // RA2: as above
		if (PlayerPtr->Is_Build_Limited(type)) { // RA2: as above
			Request_Redraw(); // RA2: as above
		}
	} else if (kind == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsHelipad) { // RA2: as above
		Request_Column_Redraw(3); // RA2: as above
	}
}


void SidebarClass::Flag_Tab_To_Redraw(int column) // RA2: as above
{
	if (column == ActiveTabIndex) { // RA2: as above
		IsToRedraw = true;
		Column[column].IsToRedraw = true; // RA2: as above
		Map.Flag_To_Redraw();
	}
}


void SidebarClass::Request_Column_Redraw(int column) // RA2: as above
{
	if (column == ActiveTabIndex) { // RA2: as above
		IsToRedraw = true;
		Column[ActiveTabIndex].IsToRedraw = true; // RA2: as above
		Map.Flag_To_Redraw();
		if (RedrawUntilFrame <= Frame) { // RA2: as above
			RedrawUntilFrame = Frame + 2; // RA2: as above
		}
	}
}


bool SidebarClass::Factory_Link(FactoryClass * factory, RTTIType type, int id)
{
	return Column[Which_Tab(type, id, false)].Factory_Link(factory, type, id); // RA2: as above
}


/***********************************************************************************************
 * SidebarClass::Activate_Repair -- Controls the repair button on the sidebar.                 *
 *                                                                                             *
 *    Use this routine to turn the repair sidebar button on and off. Typically, the button     *
 *    is enabled when the currently selected structure is friendly and damaged.                *
 *                                                                                             *
 * INPUT:   control  -- The controls how the button is to be activated or deactivated;         *
 *                      0  -- Turn button off.                                                 *
 *                      1  -- Turn button on.                                                  *
 *                      -1 -- Toggle button state.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the button previously activated?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarClass::Activate_Repair(int control)
{
	bool old = IsRepairActive;

	if (control == -1) {
		control = IsRepairActive ? 0 : 1;
	}
	switch (control) {
		case 1:
			IsRepairActive = true;
			break;

		default:
		case 0:
			IsRepairActive = false;
			break;
	}
	if (old != IsRepairActive) {
		Flag_To_Redraw();
		IsToRedraw = true;

		if (!IsRepairActive) {
//			Help_Text(TXT_NONE);
			Set_Default_Mouse(MOUSE_NORMAL, false);
		}
	}
	return(old);
}


/***********************************************************************************************
 * SidebarClass::Activate_Demolish -- Controls the demolish button on the sidebar.             *
 *                                                                                             *
 *    Use this routine to turn the demolish/dismantle sidebar button on and off. Typically,    *
 *    the button is enabled when a friendly building is selected and disabled otherwise.       *
 *                                                                                             *
 * INPUT:   control  -- The controls how the button is to be activated or deactivated;         *
 *                      0  -- Turn button off.                                                 *
 *                      1  -- Turn button on.                                                  *
 *                      -1 -- Toggle button state.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the button previously activated?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarClass::Activate_Demolish(int control)
{
	bool old = IsDemolishActive;

	if (control == -1) {
		control = IsDemolishActive ? 0 : 1;
	}
	switch (control) {
		case 1:
			IsDemolishActive = true;
			break;

		default:
		case 0:
			IsDemolishActive = false;
			break;
	}
	if (old != IsDemolishActive) {
		Flag_To_Redraw();
		IsToRedraw = true;
		if (!IsDemolishActive) {
			Set_Default_Mouse(MOUSE_NORMAL, false);
		}
	}
	return(old);
}


/***********************************************************************************************
 * SidebarClass::Add -- Adds a game object to the sidebar list.                                *
 *                                                                                             *
 *    This routine is used to add a game object to the sidebar. Call this routine when a       *
 *    factory type building is created. It handles the case of adding an item that has already *
 *    been added -- it just ignores it.                                                        *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object that is being added.                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the object added to the sidebar?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarClass::Add(RTTIType type, int id)
{
	if (!Debug_Map) {
		int first = First_Used_Column(); // RA2: as above
		int column = Which_Tab(type, id, false); // RA2: as above
		if (Column[column].Add(type, id)) {
			Enable_Column_Button(column); // RA2: as above
			Activate(1);
			if (first == -1) { // RA2: as above
				Select_Tab(First_Used_Column()); // RA2: as above
			}
			IsToRedraw = true;
			Flag_To_Redraw();
			if (column == ActiveTabIndex) { // RA2: as above
				IsToBlitSidebar = true;
			}
			return(true);
		}
	}

	return(false);
}


/***********************************************************************************************
 * SidebarClass::Scroll -- Handles scrolling the sidebar object strip.                         *
 *                                                                                             *
 *    This routine is used to scroll the sidebar strip of objects. The strip appears whenever  *
 *    a building is selected that can produce units. If the number of units to produce is      *
 *    greater than what the sidebar can hold, this routine is used to scroll the other object  *
 *    into view so they can be selected.                                                       *
 *                                                                                             *
 * INPUT:   up -- Should the scroll be upwards? Upward scrolling reveals object that are       *
 *                later in the list of objects.                                                *
 *                                                                                             *
 * OUTPUT:  bool; Did scrolling occur?                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/94   JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::Update_Build_Buttons() // RA2: as above
{
	if (Column[ActiveTabIndex].BuildableCount > Get_Doubled_Row_Count()) { // RA2: as above
		if (!ScrollDownButton.Is_Enabled()) { // RA2: as above
			ScrollDownButton.Enable(); // RA2: as above
		}
		if (!ScrollUpButton.Is_Enabled()) { // RA2: as above
			ScrollUpButton.Enable(); // RA2: as above
		}
	} else {
		if (ScrollDownButton.Is_Enabled()) { // RA2: as above
			ScrollDownButton.Disable(); // RA2: as above
		}
		if (ScrollUpButton.Is_Enabled()) { // RA2: as above
			ScrollUpButton.Disable(); // RA2: as above
		}
	}
}




void SidebarClass::Enable_Column_Button(int index) // RA2: as above
{
	if (First_Used_Column() == -1 && SidebarShapes[SBSHAPE_SHUTTER_BACK]) { // RA2: as above
		ShutterStep = 1; // RA2: as above
		ShutterFrame = 0; // RA2: as above
	}
	if (!TabButton[index].Is_Enabled()) { // RA2: as above
		TabButton[index].Enable(); // RA2: as above
	}
	if (index == ActiveTabIndex) { // RA2: as above
		Update_Build_Buttons(); // RA2: as above
	}
}


void SidebarClass::Update_Column_Availability(int index) // RA2: as above
{
	if (Column[index].BuildableCount > 0) { // RA2: as above
		Update_Build_Buttons(); // RA2: as above
	} else {
		TabButton[index].Disable(); // RA2: as above
		int first = -1; // RA2: as above
		for (int i = 0; i < 4; i++) { // RA2: as above
			if (Column[i].BuildableCount > 0) { // RA2: as above
				first = i; // RA2: as above
				break;
			}
		}
		if (first == -1) { // RA2: as above
			Update_Build_Buttons(); // RA2: as above
			if (SidebarShapes[SBSHAPE_SHUTTER_BACK]) { // RA2: as above
				ShutterStep = -1; // RA2: as above
				ShutterFrame = (SidebarShapes[SBSHAPE_SHUTTER_BACK])->Get_Count(); // RA2: as above
			}
		} else {
			Select_Tab(first); // RA2: as above
		}
	}
}


bool SidebarClass::Scroll(bool up, int column)
{
	if (_dialog_count) { // RA2: as above
		return(false);
	}

	if (column == -1) {
		column = ActiveTabIndex; // RA2: as above
	}
	if (Column[column].Step(up)) { // RA2: as above
		Flag_To_Redraw();
		return(true);
	}

	return(false);
}


/// <summary>
/// Handles paging the sidebar object strips.
/// This routine works just like Scroll, save that the strips move a whole screenful of
/// objects at a time. The player is scolded with a sound effect when there is nothing left
/// in that direction to page to.
/// </summary>
/// <param name="up">Should the paging be upwards?</param>
/// <param name="column">The strip to page, or -1 to page both of them together.</param>
/// <returns>bool; Did any paging occur?</returns>
bool SidebarClass::Page(bool up, int column)
{
	if (column == -1) {
		column = ActiveTabIndex; // RA2: as above
	}
	StripClass &strip = Column[column]; // RA2: as above
	int amount = Get_Row_Count(); // RA2: as above
	if (strip.Scroll(up, amount)) { // RA2: as above
		Flag_To_Redraw();
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * SidebarClass::Draw_It -- Renders the sidebar display.                                       *
 *                                                                                             *
 *    This routine performs the actual drawing of the sidebar display.                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the sidebar imagery changed at all?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/94   JLB : Created.                                                                 *
 *   12/31/1994 JLB : Split rendering off into the sidebar strip class.                        *
 *=============================================================================================*/
void SidebarClass::Draw_It(bool complete)
{
	DirtyRect = RECT_NONE; // RA2: as above
	Surface *old = LogicalSurface; // RA2: as above
	LogicalSurface = SidebarSurface;
	bool drawn = false; // RA2: as above
	Rect window(0, 0, SidebarSurface->Get_Width(), SidebarSurface->Get_Height());
	complete = complete || IsToRedraw || IsForceCompleteRedraw; // RA2: as above

	if (IsSidebarActive && complete && !Debug_Map) { // RA2: as above
		if (IsForceCompleteRedraw) { // RA2: as above
			drawn = true; // RA2: as above
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_TOP], 0, // RA2: as above
				Point2D(0, SidebarRect.Y), window, SHAPE_WIN_REL); // RA2: as above
			int y = StripTop; // RA2: as above
			int row = 0; // RA2: as above
			while (true) { // RA2: as above
				if (row >= Max_Visible_Rows()) { // RA2: as above
					break;
				}
				Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_MIDDLE], 0, // RA2: as above
					Point2D(0, y), window, SHAPE_WIN_REL); // RA2: as above
				y += (SidebarShapes[SBSHAPE_MIDDLE])->Get_Height(); // RA2: as above
				row++; // RA2: as above
			}
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_BOTTOM], 0, // RA2: as above
				Point2D(0, y), window, SHAPE_WIN_REL); // RA2: as above
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_ADDON], 0, // RA2: as above
				Point2D(0, y + (SidebarShapes[SBSHAPE_BOTTOM])->Get_Height()), window, SHAPE_WIN_REL); // RA2: as above
		}
		Repair.Draw_Me(drawn); // RA2: as above
		Upgrade.Draw_Me(drawn); // RA2: as above
		for (int index = 0; index < 4; index++) { // RA2: as above
			TabButton[index].Draw_Me(true); // RA2: as above
		}
		ScrollDownButton.Draw_Me(drawn); // RA2: as above
		ScrollUpButton.Draw_Me(drawn); // RA2: as above
		if (ShutterStep) { // RA2: as above
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_BACK], ShutterFrame, // RA2: as above
				Point2D(0, StripTop), window, SHAPE_WIN_REL); // RA2: as above
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_MIDDLE], ShutterFrame, // RA2: as above
				Point2D(0, StripTop), window, SHAPE_WIN_REL); // RA2: as above
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_FRONT], ShutterFrame, // RA2: as above
				Point2D(0, StripTop), window, SHAPE_WIN_REL); // RA2: as above
		}
		IsToBlitSidebar = true;
	}
	Column[ActiveTabIndex].Draw_It(IsForceCompleteRedraw); // RA2: as above
	PowerClass::Draw_It(complete || drawn); // RA2: as above
	if (Repair.IsDrawn) {
		Repair.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	if (Upgrade.IsDrawn) {
		Upgrade.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	for (int tab = 0; tab < 4; tab++) { // RA2: as above
		if (TabButton[tab].IsDrawn) { // RA2: as above
			IsToBlitSidebar = true;
			TabButton[tab].IsDrawn = false; // RA2: as above
		}
	}
	if (ScrollDownButton.IsDrawn) { // RA2: as above
		ScrollDownButton.IsDrawn = false; // RA2: as above
		IsToBlitSidebar = true;
	}
	if (ScrollUpButton.IsDrawn) { // RA2: as above
		ScrollUpButton.IsDrawn = false; // RA2: as above
		IsToBlitSidebar = true;
	}
	if (ToolTips) {
		ToolTips->Draw_Current(true);
	}
	Blit_Sidebar(IsToBlitSidebar || IsForceCompleteRedraw); // RA2: as above
	IsToRedraw = false;
	IsForceCompleteRedraw = false;
	LogicalSurface = old;
}


/// <summary>
/// Copies the sidebar onto the visible screen.
/// This routine is the last step of the sidebar's redraw. When nothing but the credits have
/// changed, only that much is copied, so an idle sidebar costs almost nothing.
/// </summary>
/// <param name="complete">Should the entire sidebar be copied rather than just the portion
/// that changed?</param>
void SidebarClass::Blit_Sidebar(bool complete)
{
	if (IsSidebarActive && GameActive && ScenarioActive) {

		RECT crect;
		GetClientRect(MainWindow, &crect);

		POINT scrnpt;
		scrnpt.x = crect.left;
		scrnpt.y = crect.top;
		ClientToScreen(MainWindow, &scrnpt);

		if (!IsToBlitSidebar && !complete) {
			IsToBlitSidebar = false;
			if (DirtyRect == RECT_NONE) { // RA2: as above
				if (IsToRedrawCredits) {
					MouseCursor->Draw_Mouse(SidebarSurface, true);
					VisibleSurface->Blit_From(
						Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, SidebarRect.Width, 16), // RA2: as above
						*SidebarSurface, Rect(0, 0, SidebarRect.Width, 16), false, true);
					IsToRedrawCredits = false;
					IsToEraseSidebarMouse = true;
				}
				IsToBlitSidebar = false;
				return;
			}
		} else {
			IsToBlitSidebar = true;
		}

		MouseCursor->Draw_Mouse(SidebarSurface, true);
		if (DirtyRect == RECT_NONE && !complete) { // RA2: as above
			if (IsToRedrawCredits) {
				VisibleSurface->Blit_From(
					Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, SidebarRect.Width, 16), // RA2: as above
					*SidebarSurface, Rect(0, 0, SidebarRect.Width, 16), false, true);
				IsToRedrawCredits = false;
			}
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y + SidebarRect.Y, // RA2: as above
					 SidebarRect.Width, SidebarSurface->Get_Height() - SidebarRect.Y), // RA2: as above
				*SidebarSurface, Rect(0, SidebarRect.Y, SidebarRect.Width, SidebarRect.Height), false, true);
		} else if (IsToBlitSidebar) { // RA2: as above
			Rect sb_rect = SidebarSurface->Get_Rect();
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, sb_rect.Width, sb_rect.Height), // RA2: as above
				*SidebarSurface, Rect(0, 0, sb_rect.Width, sb_rect.Height), false, true);
		} else {
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + DirtyRect.X + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), // RA2: as above
					 scrnpt.y + DirtyRect.Y, DirtyRect.Width, DirtyRect.Height), // RA2: as above
				*SidebarSurface, DirtyRect, false, true);
		}
		IsToEraseSidebarMouse = true;
	}
	IsToBlitSidebar = false;
}


/***********************************************************************************************
 * SidebarClass::AI -- Handles player clicking on sidebar area.                                *
 *                                                                                             *
 *    This routine handles the processing necessary when the player clicks on the sidebar.     *
 *    Typically, this is selection of the item to build.                                       *
 *                                                                                             *
 * INPUT:   input -- Reference to the keyboard input value.                                    *
 *                                                                                             *
 *          x,y   -- Mouse coordinates at time of input.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the click handled?                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/28/94   JLB : Created.                                                                 *
 *   11/11/1994 JLB : Processes input directly.                                                *
 *   12/26/1994 JLB : Uses factory manager class for construction handling.                    *
 *   12/31/1994 JLB : Simplified to use the sidebar strip class handlers.                      *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *   06/27/1995 JLB : <TAB> key toggles sidebar.                                               *
 *=============================================================================================*/
void SidebarClass::Activate_Tab(int index) // RA2: as above
{
	ActiveTabIndex = index; // RA2: as above
	TabButton[index].Turn_On(); // RA2: as above
	Column[ActiveTabIndex].Activate(); // RA2: as above
	Update_Build_Buttons(); // RA2: as above
}


void SidebarClass::Select_Tab(int index) // RA2: as above
{
	if (index == ActiveTabIndex) { // RA2: as above
		return;
	}
	ShapeButtonClass &old_button = TabButton[ActiveTabIndex]; // RA2: as above
	old_button.Turn_Off(); // RA2: as above
	StripClass &old = Column[ActiveTabIndex]; // RA2: as above
	old.Deactivate(); // RA2: as above
	ActiveTabIndex = index; // RA2: as above
	ShapeButtonClass &button = TabButton[ActiveTabIndex]; // RA2: as above
	button.Turn_On(); // RA2: as above
	StripClass &current = Column[ActiveTabIndex]; // RA2: as above
	current.Activate(); // RA2: as above
	Update_Build_Buttons(); // RA2: as above
	IsForceCompleteRedraw = true; // RA2: as above
}


void SidebarClass::Start_Tab_Flash(int index) // RA2: as above
{
	int delay = 10 - Frame % 10; // RA2: as above
	TabButton[index].Start_Flashing(10, delay, (Frame + delay) / 10 % 2 == 0); // RA2: as above
	Map.IsForceCompleteRedraw = true; // RA2: as above
	Map.Flag_To_Redraw();
}





bool SidebarClass::Stop_Tab_Flash(int index) // RA2: as above
{
	return TabButton[index].Stop_Flashing(); // RA2: as above
}


#ifdef RA2_NON_MATCHING // not exact: 480 bytes differ (adopted text)
void SidebarClass::AI(KeyNumType & input, Point2D const & xy)
{
	Point2D point = xy - Point2D(SidebarRect.X, 0); // RA2: as above
	if (!Debug_Map) {
		Activate(1);	// Force the sidebar always on in Win95 mode
		for (int index = 0; index < 4; index++) { // RA2: as above
			Column[index].AI(input, point); // RA2: as above
		}
	}

	if (IsSidebarActive) {
		if (ShutterStep == 1) { // RA2: as above
			ShutterFrame++; // RA2: as above
			if (ShutterFrame > (SidebarShapes[SBSHAPE_SHUTTER_BACK])->Get_Count()) { // RA2: as above
				ShutterStep = 0; // RA2: as above
				ShutterFrame = 0; // RA2: as above
			}
		} else if (ShutterStep == -1) { // RA2: as above
			ShutterFrame--; // RA2: as above
			if (ShutterFrame < 0) { // RA2: as above
				ShutterStep = 0; // RA2: as above
				ShutterFrame = 0; // RA2: as above
			}
		}

		Activate_Demolish(PlayerPtr->CurBuildings > 0); // RA2: as above

		if (input == 0x8066) { // RA2: as above
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, 0); // RA2: as above
			Sell_Mode_Control(-1);
		} else if (input == 0x8065) { // RA2: as above
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, 0); // RA2: as above
			Repair_Mode_Control(-1);
		} else if (input >= 0x80CB && input < 0x80CF) { // RA2: as above
			Select_Tab(input - 0x80CB); // RA2: as above
			VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, 0); // RA2: as above
		} else if ((input & ~0x4000) == (ScrollDownButton.ID | 0x8000)) { // RA2: as above
			ScrollDownButton.IsPressed = false; // RA2: as above
			if (!Column[ActiveTabIndex].Page(false)) { // RA2: as above
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, 0); // RA2: as above
			} else {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, 0); // RA2: as above
			}
		} else if ((input & ~0x4000) == (ScrollUpButton.ID | 0x8000)) { // RA2: as above
			ScrollUpButton.IsPressed = false; // RA2: as above
			if (!Column[ActiveTabIndex].Page(true)) { // RA2: as above
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, 0); // RA2: as above
			} else {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, 0); // RA2: as above
			}
		}

		bool flashed = false; // RA2: as above
		for (int tab = 0; tab < 4; tab++) { // RA2: as above
			if (TabButton[tab].AI()) { // RA2: as above
				flashed = true; // RA2: as above
			}
		}
		if (flashed) { // RA2: as above
			IsToRedraw = true;
			IsToBlitSidebar = true;
			IsSidebarRedrawPending = true; // RA2: as above
			Flag_To_Redraw();
		}
	}

	if (!IsSellMode && Repair.IsOn) { // RA2: as above
		Repair.Turn_Off();
	}
	if (!IsRepairMode && Upgrade.IsOn) { // RA2: as above
		Upgrade.Turn_Off();
	}
	PowerClass::AI(input, xy); // RA2: as above
}
#endif




/***********************************************************************************************
 * SidebarClass::Recalc -- Examines the sidebar data and updates it as necessary.              *
 *                                                                                             *
 *    Occasionally a factory gets destroyed. This routine must be called in such a case        *
 *    because it might be possible that sidebar object need to be removed. This routine will   *
 *    examine all existing objects in the sidebar class and if no possible factory can         *
 *    produce it, then it will be removed.                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine is exhaustive and thus time consuming. Only call it when really    *
 *             necessary. Such as when a factory is destroyed rather than when a non-factory   *
 *             is destroyed.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/30/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::Recalc(void)
{
	bool redraw = false;
	for (int index = 0; index < STRIPS; index++) { // RA2: as above
		if (Column[index].Recalc() && index == ActiveTabIndex) { // RA2: as above
			Column[index].IsToRedraw = true; // RA2: as above
			redraw = true;
		}
	}
	if (redraw) {
		IsToRedraw = true;
		Flag_To_Redraw();
	}
}


/***********************************************************************************************
 * SidebarClass::Activate -- Controls the sidebar activation.                                  *
 *                                                                                             *
 *    Use this routine to turn the sidebar on or off. This routine handles updating the        *
 *    necessary flags.                                                                         *
 *                                                                                             *
 * INPUT:   control  -- Tells what to do with the sidebar according to the following:          *
 *                         0 = Turn sidebar off.                                               *
 *                         1 = Turn sidebar on.                                                *
 *                         -1= Toggle sidebar on or off.                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the sidebar already on?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/09/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (0067e210 has no row); main: best 99.04, tried 11 times (frame, register, order)
bool SidebarClass::Activate(int control)
{
	bool old = IsSidebarActive;

	if (Session.Play && Session.Type != GAME_NORMAL && Session.Type != GAME_SKIRMISH)
		return(old);

	/*
	**	Determine the new state of the sidebar.
	*/
	switch (control) {
		case -1:
			IsSidebarActive = IsSidebarActive == false;
			break;

		case 1:
			IsSidebarActive = true;
			break;

		default:
		case 0:
			IsSidebarActive = false;
			break;
	}

	/*
	**	Only if there is a change in the state of the sidebar will anything
	**	be done to change it.
	*/
	bool changed = IsSidebarActive != old; // RA2: as above
	if (changed) { // RA2: as above
		Set_Buttons(IsSidebarActive); // RA2: as above
		if (IsSidebarActive) {
			Reposition_Sidebar();
			IsToRedraw = true;
			Repair.Zap();
			Add_A_Button(Repair);
			Upgrade.Zap();
			Add_A_Button(Upgrade);
			for (int tab = 0; tab < STRIPS; tab++) { // RA2: as above
				TabButton[tab].Zap(); // RA2: as above
				TabButton[tab].IsPressed = false; // RA2: the exe clears the byte at +0x2C of the button (IsPressed), not IsOn at +0x2D
				Add_A_Button(TabButton[tab]); // RA2: as above
			}
			ScrollDownButton.Zap(); // RA2: as above
			Map.Add_A_Button(ScrollDownButton); // RA2: as above
			ScrollUpButton.Zap(); // RA2: as above
			Map.Add_A_Button(ScrollUpButton); // RA2: as above
			Background.Zap();
			Add_A_Button(Background);
			RadarButton.Zap();
			Add_A_Button(RadarButton);
			for (int index = 0; index < STRIPS; index++) { // RA2: as above
				Column[index].TopIndex = 0; // RA2: as above
				if (Column[index].BuildableCount > 0) { // RA2: as above
					TabButton[index].Enable(); // RA2: as above
				}
			}
			TabButton[ActiveTabIndex].Turn_On(); // RA2: as above
			Column[ActiveTabIndex].Activate(); // RA2: as above
			Update_Build_Buttons(); // RA2: as above
		} else {
			Stop_Ingame_Movie();
			Remove_A_Button(Repair);
			Remove_A_Button(Upgrade);
			Remove_A_Button(Background);
			for (int tab = 0; tab < STRIPS; tab++) { // RA2: as above
				Remove_A_Button(TabButton[tab]); // RA2: as above
			}
			Remove_A_Button(ScrollDownButton); // RA2: as above
			Remove_A_Button(ScrollUpButton); // RA2: as above
			Column[ActiveTabIndex].Remove_Buttons(); // RA2: as above
			Remove_A_Button(RadarButton);
		}

		/*
		**	Since the sidebar status has changed, update the map so that the graphics
		**	will be rendered correctly.
		*/
		Flag_To_Redraw(GS_REDRAW_ALL);
	}
	return changed; // RA2: as above
}
#endif


/***********************************************************************************************
 * SidebarClass::StripClass::StripClass -- Default constructor for the side strip class.       *
 *                                                                                             *
 *    This constructor is used to reset the side strip to default empty state.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int __fastcall SidebarClass::Cameos_Width(int value) // RA2: as above
{
	return CameoWidth * value; // RA2: as above
}


int __fastcall SidebarClass::Cameos_Height(int value) // RA2: as above
{
	return CameoHeight * value + 1; // RA2: as above
}


SidebarClass::StripClass::StripClass(InitClass const &) :
	X(0),
	Y(0),
	ObjectRect(RECT_NONE),
	ID(0),
	IsToRedraw(true),
	IsBuilding(false),
	IsScrollingDown(false),
	IsScrolling(false),
	Flasher(-1),
	TopIndex(0),
	Scroller(0),
	Slid(0),
	LastSlid(0),
	BuildableCount(0)
{
	for (int index = 0; index < MAX_BUILDABLES; index++) {
		Buildables[index].BuildableID = 0;
		Buildables[index].BuildableType = RTTI_NONE;
		Buildables[index].IsAlt = false; // RA2: 0x67E660
		Buildables[index].Factory = NULL;
	}
}


/***********************************************************************************************
 * SidebarClass::StripClass::One_Time -- Performs one time actions necessary for the side stri *
 *                                                                                             *
 *    Call this routine ONCE at the beginning of the game. It handles retrieving pointers to   *
 *    the shape files it needs for rendering.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::One_Time(int )
{
	DarkenShapes = (ShapeSet*)MFCD::Retrieve("DARKEN.SHP");
}


/***********************************************************************************************
 * SidebarClass::StripClass::Get_Special_Cameo -- Fetches the special event cameo shape.       *
 *                                                                                             *
 *    This routine will return with a pointer to the cameo data for the special objects that   *
 *    can appear on the sidebar (e.g., nuclear bomb).                                          *
 *                                                                                             *
 * INPUT:   type  -- The special type to fetch the cameo imagery for.                          *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cameo imagery for the specified special object.      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1995 JLB : commented                                                                *
 *=============================================================================================*/
ShapeSet const * SidebarClass::StripClass::Get_Special_Cameo(SuperWeaponType type)
{
	if ((unsigned)type < (unsigned)SuperWeaponTypes.Count()) {
		return(SuperWeaponTypes[type]->CameoData);
	}
	return(NULL);
}


/***********************************************************************************************
 * SidebarClass::StripClass::Init_Clear -- Sets sidebar to a known (and deactivated) state     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::Clear_Buildables() // RA2: as above
{
	IsScrollingDown = false;
	IsScrolling = false;
	IsBuilding = false;
	Flasher = -1;
	TopIndex = 0;
	Slid = 0;
	BuildableCount = 0;
	for (int index = 0; index < 75; index++) { // RA2: as above
		Buildables[index].BuildableID = 0;
		Buildables[index].BuildableType = RTTI_NONE;
		Buildables[index].IsAlt = 0; // RA2: as above
		Buildables[index].Factory = 0; // RA2: as above
		Buildables[index].Unknown_10 = 0; // RA2: as above
		Buildables[index].Progress.Set_Rate(0); // RA2: as above
		Buildables[index].Progress.Set_Stage(0); // RA2: as above
		Buildables[index].FlashEndFrame = 0; // RA2: as above
	}
}


/***********************************************************************************************
 * SidebarClass::StripClass::Init_IO -- Initializes the strip's buttons                        *
 *                                                                                             *
 * This routine doesn't actually add any buttons to the list.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::Init_Buttons(int id) // RA2: as above
{
	ID = id;
	int rows = Map.Get_Row_Count(); // RA2: as above
	for (int row = 0; row < rows; row++) { // RA2: as above
		for (int column = 0; column < 2; column++) { // RA2: as above
			int index = row * 2 + column; // RA2: as above
			SelectClass &button = SelectButton[ID * 60 + index]; // RA2: as above
			button.ID = 202; // RA2: as above
			button.X = Map.Column[ID].X + CameoWidth * column; // RA2: as above
			button.Y = Map.Column[ID].Y + CameoHeight * row + 1; // RA2: as above
			button.Width = 60; // RA2: as above
			button.Height = 48; // RA2: as above
			button.Strip = this; // RA2: as above
			button.Index = index; // RA2: as above
		}
	}
}


/***********************************************************************************************
 * SidebarClass::StripClass::Deactivate -- Removes the side strip buttons from the input syste *
 *                                                                                             *
 *    Call this routine to remove all the buttons on the side strip from the map's input       *
 *    system.                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Never call this routine unless the Activate() function was previously called.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::Add_Buttons() // RA2: as above
{

	int max = Map.Get_Doubled_Row_Count(); // RA2: as above
	for (int index = 0; index < max; index++) { // RA2: as above
		SelectButton[ID * 60 + index].Zap(); // RA2: as above
		Map.Add_A_Button(SelectButton[ID * 60 + index]); // RA2: as above
	}
	IsSidebarRedrawPending = true; // RA2: as above
}


void SidebarClass::StripClass::Remove_Buttons() // RA2: as above
{
	int max = Map.Get_Doubled_Row_Count(); // RA2: as above
	for (int index = 0; index < max; index++) { // RA2: as above
		Map.Remove_A_Button(SelectButton[ID * 60 + index]); // RA2: as above
	}
}


/***********************************************************************************************
 * SidebarClass::StripClass::Add -- Add an object to the side strip.                           *
 *                                                                                             *
 *    Use this routine to add a buildable object to the side strip.                            *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object type that can be built and is to be added to     *
 *                      the side strip.                                                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the object successfully added to the side strip? Failure could be the    *
 *                result of running out of room in the side strip array or the object might    *
 *                already be in the list.                                                      *
 *                                                                                             *
 * WARNINGS:   none.                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarClass::StripClass::Is_Before(RTTIType type, int id, RTTIType other_type, int other_id) // RA2: as above
{
	TechnoTypeClass *techno = (TechnoTypeClass *)Fetch_Techno_Type(type, id); // RA2: as above
	TechnoTypeClass *other = (TechnoTypeClass *)Fetch_Techno_Type(other_type, other_id); // RA2: as above
	wchar_t const *name; // RA2: as above
	wchar_t const *other_name; // RA2: as above
	if (other_type == RTTI_NONE) { // RA2: as above
		return(true);
	}

	bool super = type == RTTI_SPECIAL || type == RTTI_SUPERWEAPON || type == RTTI_SUPERWEAPONTYPE; // RA2: as above
	bool other_super = other_type == RTTI_SPECIAL || other_type == RTTI_SUPERWEAPON || other_type == RTTI_SUPERWEAPONTYPE; // RA2: as above
	if (super) { // RA2: as above
		if (other_super) { // RA2: as above
			SuperWeaponTypeClass *weapon = SuperWeaponTypes[id]; // RA2: as above
			SuperWeaponTypeClass *other_weapon = SuperWeaponTypes[other_id]; // RA2: as above
			if (weapon->RechargeTime < other_weapon->RechargeTime) { // RA2: as above
				return(true);
			}
			if (weapon->RechargeTime > other_weapon->RechargeTime) { // RA2: as above
				return(false);
			}
			name = weapon->UIName; // RA2: as above
			other_name = other_weapon->UIName; // RA2: as above
			return wcscmp(name, other_name) <= 0 ? true : false; // RA2: as above
		}
	} else if (!other_super) { // RA2: as above
		int side = PlayerPtr->Class->Side; // RA2: as above
		bool own = side == techno->AIBasePlanningSide; // RA2: as above
		bool other_own = side == other->AIBasePlanningSide; // RA2: as above
		if (own && !other_own) { // RA2: as above
			return(true);
		}
		if (!own && other_own) { // RA2: as above
			return(false);
		}
	}

	bool unit = type == RTTI_UNITTYPE || type == RTTI_AIRCRAFTTYPE; // RA2: as above
	bool aircraft = unit && techno->ConsideredAircraft; // RA2: as above
	bool naval = unit && techno->Naval; // RA2: as above
	bool ground = !aircraft && !naval; // RA2: as above
	bool other_unit = other_type == RTTI_UNITTYPE || other_type == RTTI_AIRCRAFTTYPE; // RA2: as above
	bool other_aircraft = other_unit && other->ConsideredAircraft; // RA2: as above
	bool other_naval = other_unit && other->Naval; // RA2: as above
	bool other_ground = !other_aircraft && !other_naval; // RA2: as above

	if (super) { // RA2: as above
		if (other_aircraft || other_naval || other_ground) { // RA2: as above
			return(true);
		}
	} else if (ground) { // RA2: as above
		if (other_super) { // RA2: as above
			return(false);
		}
		if (other_aircraft || other_naval) { // RA2: as above
			return(true);
		}
	} else if (aircraft) { // RA2: as above
		if (other_super) { // RA2: as above
			return(false);
		}
		if (other_ground) { // RA2: as above
			return(false);
		}
		if (other_naval) { // RA2: as above
			return(true);
		}
	} else if (naval) { // RA2: as above
		if (other_super) { // RA2: as above
			return(false);
		}
		if (other_ground || other_aircraft) { // RA2: as above
			return(false);
		}
	}

	if (techno->Level < other->Level) { // RA2: as above
		return(true);
	}
	if (techno->Level > other->Level) { // RA2: as above
		return(false);
	}
	int cost = techno->Cost_Of(PlayerPtr); // RA2: as above
	int other_cost = other->Cost_Of(PlayerPtr); // RA2: as above
	if (cost < other_cost) { // RA2: as above
		return(true);
	}
	if (cost > other_cost) { // RA2: as above
		return(false);
	}
	name = techno->UIName; // RA2: as above
	other_name = other->UIName; // RA2: as above
	return wcscmp(name, other_name) <= 0 ? true : false; // RA2: as above
}


void SidebarClass::StripClass::Insert_Buildable(RTTIType type, int id) // RA2: as above
{
	BuildableCount++;
	for (int index = 0; index < BuildableCount; index++) {
		RTTIType other_type = Buildables[index].BuildableType; // RA2: as above
		int other_id = Buildables[index].BuildableID; // RA2: as above
		if (Is_Before(type, id, other_type, other_id)) { // RA2: as above
			for (int move = BuildableCount - 1; move > index; move--) { // RA2: as above
				Buildables[move] = Buildables[move - 1]; // RA2: as above
			}
			Buildables[index].BuildableType = type; // RA2: as above
			Buildables[index].BuildableID = id; // RA2: as above
			if (type == RTTI_BUILDINGTYPE) { // RA2: as above
				Buildables[index].IsAlt = HouseClass::Build_Category(type, id); // RA2: as above
			}
			Buildables[index].Factory = 0; // RA2: as above
			Buildables[index].Unknown_10 = 0; // RA2: as above
			Buildables[index].Progress.Set_Stage(0); // RA2: as above
			Buildables[index].Progress.Set_Rate(0); // RA2: as above
			Buildables[index].FlashEndFrame = 0; // RA2: as above
			return;
		}
	}
}


bool SidebarClass::StripClass::Add(RTTIType type, int id)
{
	if (BuildableCount <= 75) { // RA2: as above
		for (int index = 0; index < BuildableCount; index++) {
			if (Buildables[index].BuildableType == type && Buildables[index].BuildableID == id) {
				return(false);
			}
		}
		if (!ScenarioInit && type != RTTI_SPECIAL) {
			Queue_Speech("EVA_NewConstructionOptions", -1, -1); // RA2: as above
		}
		Insert_Buildable(type, id); // RA2: as above
		IsToRedraw = true;
		IsSidebarRedrawPending = true; // RA2: as above
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * SidebarClass::StripClass::Scroll -- Causes the side strip to scroll.                        *
 *                                                                                             *
 *    Use this routine to flag the side strip to scroll. The direction scrolled is controlled  *
 *    by the parameter. Scrolling is merely initiated by this routine. Subsequent calls to     *
 *    the AI function and the Draw_It function are required to properly give the appearance    *
 *    of scrolling.                                                                            *
 *                                                                                             *
 * INPUT:   bool; Should the side strip scroll UP? If it is to scroll down then pass false.    *
 *                                                                                             *
 * OUTPUT:  bool; Was the side strip started to scroll in the desired direction?               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *   07/29/1995 JLB : Simplified scrolling logic.                                              *
 *=============================================================================================*/
bool SidebarClass::StripClass::Scroll(bool up, int amount) // RA2: as above
{
	if (up) { // RA2: 0x67EE60
		if (!TopIndex) { // RA2: as above
			return false; // RA2: as above
		}
		Scroller -= amount; // RA2: as above
		return true; // RA2: as above
	}
	if ((TopIndex + Map.Get_Row_Count()) * 2 >= BuildableCount) { // RA2: as above
		return false; // RA2: as above
	}
	Scroller += amount; // RA2: as above
	return true; // RA2: as above
}


/// <summary>
/// Causes the side strip to scroll by a whole page.
/// Use this routine to flag the side strip to move a full screenful of cameos rather than
/// the single one that Scroll would move. As with scrolling, this routine only starts the
/// motion; the AI and Draw_It routines carry it out.
/// </summary>
/// <param name="up">Should the side strip page UP? If it is to page down then pass false.</param>
/// <returns>bool; Was the side strip started to page in the desired direction?</returns>
bool SidebarClass::StripClass::Step(bool up) // RA2: as above
{
	return Scroll(up, 1); // RA2: 0x67EEF0
}


bool SidebarClass::StripClass::Page(bool up)
{
	int amount = Map.Get_Row_Count(); // RA2: 0x67EF70
	return Scroll(up, amount); // RA2: as above
}


/***********************************************************************************************
 * SidebarClass::StripClass::Flag_To_Redraw -- Flags the sidebar strip to be redrawn.          *
 *                                                                                             *
 *    This utility routine is called when something changes on the sidebar and it must be      *
 *    reflected the next time drawing is performed.                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::Flag_To_Redraw(void)
{
	IsToRedraw = true;
	Map.Flag_To_Redraw();
}


/***********************************************************************************************
 * SidebarClass::StripClass::AI -- Input and AI processing for the side strip.                 *
 *                                                                                             *
 *    The side strip AI processing is performed by this function. This function not only       *
 *    checks for player input, but also handles any graphic logic updating necessary as a      *
 *    result of flashing or construction animation.                                            *
 *                                                                                             *
 * INPUT:   input -- The player input code.                                                    *
 *                                                                                             *
 *          x,y   -- Mouse coordinate to use.                                                  *
 *                                                                                             *
 * OUTPUT:  bool; Did the AI detect that it will need a rendering change? If this routine      *
 *                returns true, then the Draw_It function should be called at the              *
 *                earliest opportunity.                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *=============================================================================================*/
bool SidebarClass::StripClass::AI(KeyNumType & input, Point2D const & xy) // RA2: 0x67F050; the scroll buttons are handled in SidebarClass::AI
{
	bool redraw = false; // RA2: as above

	/*
	**	Reflect the scroll desired direction/value into the scroll
	**	logic handler. This might result in up or down scrolling.
	*/
	if (!IsScrolling && Scroller) { // RA2: as above
		int columns = COLUMNS; // RA2: two cameos to a row
		int rows = Map.Get_Row_Count(); // RA2: the rows are computed once and used twice
		if (BuildableCount <= rows * 2) { // RA2: two cameos to a row
			Scroller = 0;
		} else {

			/*
			**	Top of list is moving toward lower ordered entries in the object list. It looks like
			**	the "window" to the object list is moving up even though the actual object images are
			**	scrolling downward.
			*/
			if (Scroller < 0) {
				if (!TopIndex) {
					Scroller = 0;
				} else {
					Scroller++;
					IsScrollingDown = false;
					IsScrolling = true;
					TopIndex--;
					Slid = 0;
				}

			} else {
				if ((TopIndex + rows) * 2 >= BuildableCount) { // RA2: two cameos to a row
					Scroller = 0;
				} else {
					Scroller--;
					Slid = SidebarClass::CameoHeight; // RA2: the step from one row of cameos to the next
					IsScrollingDown = true;
					IsScrolling = true;
				}
			}
		}
	}

	/*
	**	Scroll logic is handled here.
	*/
	if (IsScrolling) {
		if (IsScrollingDown) {
			Slid -= SidebarClass::SidebarEdgeStep; // RA2: 0xABCD80
			if (Slid <= 0) {
				IsScrolling = false;
				Slid = 0;
				TopIndex++;
			}
		} else {
			Slid += SidebarClass::SidebarEdgeStep; // RA2: as above
			if (Slid >= SidebarClass::CameoHeight) { // RA2: as above
				IsScrolling = false;
				Slid = 0;
			}
		}
		redraw = true;
	}

	/*
	**	Handle any flashing logic. Flashing occurs when the player selects an object
	**	and provides the visual feedback of a recognized and legal selection.
	*/
	if (Flasher != -1) {
		if (Graphic_Logic()) {
			redraw = true;
			if (Fetch_Stage() >= 7) {
				Set_Rate(0);
				Set_Stage(0);
				Flasher = -1;
			}
		}
	}

	if (Map.RedrawUntilFrame > Frame) { // RA2: the sidebar was asked to redraw for a few more frames
		redraw = true; // RA2: as above
	}

	/*
	**	The tab flashes while something on its strip can be placed.
	*/
	if (ID == 0 || ID == 1) { // RA2: the structure and defense tabs
		bool ready = false; // RA2: as above
		for (int index = 0; index < BuildableCount; index++) { // RA2: as above
			if (Buildables[index].BuildableType == RTTI_SPECIAL) { // RA2: as above
				SuperClass * super = PlayerPtr->SuperWeapon[Buildables[index].BuildableID]; // RA2: as above
				if (super != NULL && super->Can_Place()) { // RA2: as above
					ready = true; // RA2: as above
					break; // RA2: as above
				}
			} else {
				FactoryClass * factory = Buildables[index].Factory; // RA2: as above
				if (factory != NULL && factory->Has_Completed()) { // RA2: as above
					TechnoClass * done = factory->Get_Object(); // RA2: as above
					if (done != NULL && done->Fetch_RTTI() == RTTI_BUILDING) { // RA2: as above
						ready = true; // RA2: as above
						break; // RA2: as above
					}
				}
			}
		}
		if (ready) { // RA2: as above
			int frame = Frame; // RA2: the frame is read once; the exe adds the delay to it as (delay + frame), unlike the out-of-line Start_Tab_Flash 0x67DC00
			int delay = 10 - frame % 10; // RA2: the tab flashes in step with the next multiple of ten frames, as Start_Tab_Flash does
			Map.TabButton[ID].Start_Flashing(10, delay, (delay + frame) / 10 % 2 == 0); // RA2: as above
			Map.IsForceCompleteRedraw = true; // RA2: as above
			Map.Flag_To_Redraw(); // RA2: as above
		} else {
			Map.Stop_Tab_Flash(ID); // RA2: as above
		}
	}

	/*
	**	Handle any building clock animation logic.
	*/
	if (IsBuilding) {
		for (int index = 0; index < BuildableCount; index++) {
			FactoryClass * factory = Buildables[index].Factory;

			if (factory && factory->Has_Changed()) {
				redraw = true;
				if (factory->Has_Completed()) {

					/*
					**	Construction has been completed. Announce this fact to the player and
					**	try to get the object to automatically leave the factory. Buildings are
					**	the main exception to the ability to leave the factory under their own
					**	power.
					*/
					TechnoClass * pending = factory->Get_Object();
					if (pending != NULL) {
						switch (pending->Fetch_RTTI()) { // RA2: as above
							case RTTI_BUILDING: { // RA2: as above
								Queue_Speech("EVA_ConstructionComplete", -1, -1); // RA2: RA2's EVA speaks through Queue_Speech
								BuildingClass * building = (pending->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)pending : NULL; // RA2: the cast the exe makes before the call
								Set_Tab_Building(building); // RA2: 0x6F8A00 makes the building the one the structure or defense tab shows
								break;
							}

							case RTTI_UNIT: // RA2: the jump table of the exe sends these three kinds to one body
							case RTTI_AIRCRAFT: // RA2: as above
							case RTTI_INFANTRY: // RA2: as above
								OutList.Add(EventClass(pending->Owner(), EventClass::PLACE, pending->Fetch_RTTI(), -1, pending->Techno_Type_Class()->Naval, CELL_NONE)); // RA2: the event carries the naval flag of the type
								Buildables[index].Unknown_10 = 0; // RA2: the entry is cleared once the placement event is queued (or dropped)
								break;
						}
					}
				}
			}
		}
	}

	/*
	**	The cameo clocks of the entries that are not producing are reset, and all of them are
	**	advanced here.
	*/
	for (int index = 0; index < BuildableCount; index++) { // RA2: as above
		if (Buildables[index].Unknown_10 == 1) { // RA2: as above
			FactoryClass * factory = Buildables[index].Factory; // RA2: as above
			if (factory != NULL && (!factory->Is_Building() || factory->Is_On_Hold())) { // RA2: as above
				Buildables[index].Progress.Set_Rate(0); // RA2: as above
				Buildables[index].Progress.Set_Stage(0); // RA2: as above
			}
			if (Buildables[index].Progress.Graphic_Logic()) { // RA2: as above
				redraw = true; // RA2: as above
				if (Buildables[index].Progress.Fetch_Stage() >= 53) { // RA2: the clock has 54 frames; at the last one it stops and keeps the stage
					Buildables[index].Progress.Set_Rate(0); // RA2: as above
				}
			}
		}
	}

	/*
	**	If any of the logic determined that this side strip needs to be redrawn, then
	**	set the redraw flag for this side strip.
	*/
	if (redraw) {
		Map.Flag_Tab_To_Redraw(Map.ActiveTabIndex); // RA2: as above
		Flag_To_Redraw();
		SidebarClass::IsSidebarRedrawPending = true; // RA2: as above
		SidebarClass::IsToBlitSidebar = true; // RA2: as above
	}

	return(redraw);
}
















/// <summary>
/// Fetches the tooltip text for one of the strip's cameos.
/// This routine composes the name and cost line that appears when the mouse lingers over a
/// build cameo. Only the cost is given when the player has cameo text turned on, since the
/// name is already printed beneath the cameo itself.
/// </summary>
/// <param name="id">The cameo slot, counted from the top of the strip as it is displayed.</param>
/// <returns>Returns with a pointer to the help text, or NULL if that slot is empty.</returns>
wchar_t const *SidebarClass::StripClass::Help_Text(int gadget_index) // RA2: as above
{
	int index = gadget_index + TopIndex * 2; // RA2: as above
	if (!GameActive || index >= BuildableCount || BuildableCount >= 75) { // RA2: as above
		return(0);
	}
	if (Buildables[index].BuildableType == RTTI_SPECIAL) { // RA2: as above
		return SuperWeaponTypes[Buildables[index].BuildableID]->UIName; // RA2: as above
	}
	TechnoTypeClass const * type = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID); // RA2: as above
	if (IsCameoTextShown) { // RA2: as above
		swprintf(CameoHelpBuffer, // RA2: as above
			StringTable::LoadString("TXT_MONEY_FORMAT_1", 0, "D:\\Ra2\\Sidebar.CPP", 2627), // RA2: as above
			type->Cost_Of(PlayerPtr)); // RA2: as above
	} else {
		swprintf(CameoHelpBuffer, // RA2: as above
			StringTable::LoadString("TXT_MONEY_FORMAT_2", 0, "D:\\Ra2\\Sidebar.CPP", 2629), // RA2: as above
			type->UIName, type->Cost_Of(PlayerPtr)); // RA2: as above
	}
	int length = wcslen(CameoHelpBuffer); // RA2: as above
	for (int i = 0; i < length; i++) { // RA2: as above
		if (CameoHelpBuffer[i] == ' ') { // RA2: as above
			CameoHelpBuffer[i] = '\n'; // RA2: as above
		}
	}
	return CameoHelpBuffer; // RA2: as above
}


void SidebarClass::StripClass::Activate() // RA2: as above
{
	AllowedToDraw = true; // RA2: as above
	Add_Buttons(); // RA2: as above
}


void SidebarClass::StripClass::Deactivate() // RA2: as above
{
	AllowedToDraw = false; // RA2: as above
	Remove_Buttons(); // RA2: as above
}


int SidebarClass::StripClass::Get_Progress(int index) // RA2: as above
{
	int progress = 0; // RA2: as above
	if (Buildables[index].Factory) { // RA2: as above
		progress = Buildables[index].Factory->Completion(); // RA2: as above
	}
	if (Buildables[index].Unknown_10 == 1 && Buildables[index].Progress.Fetch_Stage() > progress) { // RA2: as above
		progress = (progress + Buildables[index].Progress.Fetch_Stage()) / 2; // RA2: as above
	}
	return progress; // RA2: as above
}


/***********************************************************************************************
 * SidebarClass::StripClass::Draw_It -- Render the sidebar display.                            *
 *                                                                                             *
 *    Use this routine to render the sidebar display. It checks to see if it needs to be       *
 *    redrawn and only redraw if necessary. If the "complete" parameter is true, then it       *
 *    will force redraw the entire strip.                                                      *
 *                                                                                             *
 * INPUT:   complete -- Should the redraw be forced? A force redraw will ignore the redraw     *
 *                      flag.                                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/31/1994 JLB : Created.                                                                 *
 *   08/06/1995 JLB : Handles multi factory tracking in same strip.                            *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ (outline pixel channel shifts)
void SidebarClass::StripClass::Draw_It(bool complete)
{
	if (AllowedToDraw && (IsToRedraw || complete)) { // RA2: only an active strip is drawn
		IsToRedraw = false;
		IsToBlitSidebar = true;
		Rect cliprect; // RA2: the whole sidebar surface
		cliprect.X = 0; // RA2: as above
		cliprect.Y = 0; // RA2: as above
		cliprect.Width = SidebarSurface->Get_Width(); // RA2: as above
		cliprect.Height = SidebarSurface->Get_Height(); // RA2: as above

		/*
		**	RA2: while a building is being made or is on hold, no other building can be started.
		*/
		bool busy = false; // RA2: as above
		for (int i = 0; i < BuildableCount; i++) { // RA2: as above
			if (Buildables[i].Unknown_10 == 1 || Buildables[i].Unknown_10 == 2) { // RA2: as above
				busy = true; // RA2: as above
				break; // RA2: as above
			}
		}

		/*
		**	Loop through all the buildable objects that are visible in the strip and render
		**	them. Their Y offset may be adjusted if the strip is in the process of scrolling.
		*/
		int columns = COLUMNS; // RA2: two cameos to a row
		int rows = Map.Get_Row_Count() + (IsScrolling ? 1 : 0); // RA2: the rows are counted once, and two cameos make a row
		for (int row = 0; row < rows; row++) { // RA2: as above
			for (int col = 0; col < columns; col++) { // RA2: as above
				bool draw = complete; // RA2: only changed cameos are drawn unless the strip is redrawn in full
				int stage = 0; // RA2: as above
				ShapeSet const * shapefile = NULL; // RA2: as above
				FactoryClass * factory = NULL; // RA2: as above
				wchar_t const * state = NULL; // RA2: the readout is wide text
				int index = col + (TopIndex + row) * columns; // RA2: as above
				bool production = false; // RA2: as above
				bool completed = false; // RA2: as above
				bool darken = false; // RA2: as above
				bool hover = false; // RA2: the mouse is over this cameo
				int x = CameoWidth * col - SidebarRect.X + X; // RA2: the cameo sizes are the sidebar's
				int y = CameoHeight * row + 1 + Y; // RA2: as above
				SelectClass * button = &SelectButton[row * columns + ID * MAX_SLOTS_PER_STRIP + col]; // RA2: as above

				/*
				**	If the strip is scrolling, then the offset is adjusted accordingly.
				*/
				if (IsScrolling) { // RA2: as above
					y += Slid - CameoHeight; // RA2: as above
				}

				wchar_t const * name = NULL; // RA2: wide names
				TechnoTypeClass const * obj = NULL; // RA2: as above

				/*
				**	Fetch the shape number for the object type located at this current working
				**	slot. This shape pointer is used to draw the underlying graphic there.
				*/
				if (index < BuildableCount) { // RA2: as above
					hover = button->IsHovered && !IgnoreInput; // RA2: the mouse is over the button and input is on (Is_Hovered, 0x681690, is a separate function)

					if (Buildables[index].BuildableType != RTTI_SPECIAL) { // RA2: as above

						obj = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID); // RA2: as above
						if (obj != NULL) { // RA2: as above

							name = obj->UIName; // RA2: the wide UI name

							/*
							**	If there is already a factory producing this kind of object, then all
							**	objects of this type are displays in a disabled state.
							*/
							bool isbusy = false; // RA2: as above
							if (obj->RTTI == RTTI_BUILDINGTYPE && busy) { // RA2: the strip's own busy flag
								isbusy = true; // RA2: as above
							} else if (obj->Who_Can_Build_Me(true, true, true, PlayerPtr) == NULL) { // RA2: as above
								isbusy = true; // RA2: as above
							} else {
								TechnoTypeClass const * type = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID); // RA2: as above
								bool none = PlayerPtr->Can_Build(type, false, false) == -1; // RA2: as above
								bool limited = PlayerPtr->Is_Build_Limited(type); // RA2: as above
								if (none || limited) { // RA2: as above
									isbusy = true; // RA2: as above
								}
							}

							shapefile = (ShapeSet const *)obj->Get_Cameo_Data(); // RA2: as above
							factory = Buildables[index].Factory; // RA2: as above
							if (factory != NULL) { // RA2: as above
								production = true; // RA2: as above
								completed = factory->Has_Completed(); // RA2: as above
								if (completed) { // RA2: as above
									state = StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\Sidebar.CPP", 2845); // RA2: wide text
								} else {
									draw = true; // RA2: a cameo that is being built is redrawn every time
								}
								stage = Get_Progress(index); // RA2: as above
								darken = false; // RA2: as above
							} else {
								production = false; // RA2: as above

								/*
								**	Darken the imagery if a factory of a matching type is
								**	already busy.
								*/
								darken = isbusy; // RA2: as above

								/*
								**	RA2: an entry without a factory is still shown as in progress while it is
								**	queued (1), on hold (2) or waits to be placed (3).
								*/
								switch (Buildables[index].Unknown_10) { // RA2: as above
									case 1: // RA2: as above
										draw = true; // RA2: as above
										production = true; // RA2: as above
										stage = Get_Progress(index); // RA2: as above
										darken = false; // RA2: as above
										break; // RA2: as above

									case 3: // RA2: as above
										production = true; // RA2: as above
										completed = true; // RA2: as above
										darken = false; // RA2: as above
										break; // RA2: as above

									case 2: // RA2: as above
										production = true; // RA2: as above
										completed = false; // RA2: as above
										darken = true; // RA2: as above
										stage = Get_Progress(index); // RA2: as above
										break; // RA2: as above

									case 0: // RA2: as above
										if (!complete) { // RA2: nothing changed, so the cameo is left as it is
											continue; // RA2: as above
										}
										break; // RA2: as above
								}
							}
						} else {
							darken = false; // RA2: as above
						}

					} else {

						int id = Buildables[index].BuildableID; // RA2: as above
						name = SuperWeaponTypes[id]->UIName; // RA2: as above
						shapefile = Get_Special_Cameo(SuperWeaponType(id)); // RA2: as above

						production = true; // RA2: as above
						completed = PlayerPtr->SuperWeapon[id]->Is_Charging() == false; // RA2: as above
						state = PlayerPtr->SuperWeapon[id]->State_String(); // RA2: as above
						stage = PlayerPtr->SuperWeapon[id]->Anim_Stage(); // RA2: as above
						darken = false; // RA2: as above
						draw = true; // RA2: a superweapon cameo is redrawn every time
					}

				} else {
					production = false; // RA2: as above
				}

				/*
				**	Now that the shape of the object at the current working slot has been found,
				**	draw it and any graphic overlays as necessary.
				*/
				Rect window(0, 0, x + 60, y + 48); // RA2: the clipping window follows the cameo
				if (draw) { // RA2: as above
					if (shapefile != NULL) { // RA2: as above
						Draw_Shape(*SidebarSurface, *CameoDrawer, shapefile, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL)); // RA2: as above
					}

					/*
					**	RA2: the mouse is over this cameo, so it gets an outline in the tooltip colour.
					*/
					RGBClass const & color = TooltipColor; // RA2: shared by the outline and the text below.
					if (hover) { // RA2: as above
						int outline = DSurface::Build_Hicolor_Pixel(RGBClass(color)); // RA2: as above
						SidebarSurface->Draw_Rect(window, Rect(x, y, 60, 48), outline); // RA2: as above
					}

					/*
					**	Darken this object because it cannot be produced or is otherwise
					**	unavailable.
					*/
					if (darken) { // RA2: as above
						Draw_Shape(*SidebarSurface, *SidebarDrawer, DarkenShapes, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_DARKEN)); // RA2: as above
					}

					/*
					**	RA2: a cameo flashes (darkened every other half second) until its flash frame is past.
					*/
					if (Buildables[index].FlashEndFrame > Frame && Frame % 16 > 8) { // RA2: as above
						Draw_Shape(*SidebarSurface, *SidebarDrawer, DarkenShapes, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_TRANSLUCENT50)); // RA2: as above
					}

					if (name != NULL) { // RA2: as above
						Print_Cameo_Text(name, Point2D(x, y + 36), cliprect, 60); // RA2: the caption sits 36 below the cameo's top and is 60 wide
					}

					bool hasqueuecount = false; // RA2: as above
					unsigned pixel = DSurface::Build_Hicolor_Pixel(color); // RA2: the text colour is the tooltip colour
					if (obj != NULL) { // RA2: as above
						FactoryClass * queue = PlayerPtr->Fetch_Factory(obj->Fetch_RTTI(), obj->Naval, 0); // RA2: the factory is picked by type and naval flag
						if (queue != NULL) { // RA2: as above
							int total = queue->Total(obj); // RA2: as above
							if (total > 1 || total > 0 && !queue->Is_Currently_Producing(obj)) { // RA2: as above
								wchar_t buffer[256]; // RA2: the queued count, with a dimmed box behind it
								swprintf(buffer, L"%d", total); // RA2: as above
								Point2D point(x + QUEUE_COUNT_X_OFFSET, y + 1); // RA2: as above
								Rect rect = Text_Rect(buffer, point, 0x242, 2, 1); // RA2: as above
								Blend_Rect(&rect, SidebarSurface, 0, 175); // RA2: as above
								Fancy_Text_Print(L"%d", *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x242), total); // RA2: as above
								hasqueuecount = true; // RA2: as above
							}
						}
					}

					/*
					**	Draw the overlapping clock shape if this is object is being constructed.
					**	If the object is completed, then display "Ready" with no clock shape.
					*/
					if (production) { // RA2: as above

						/*
						**	Display text showing that the object is ready to place.
						*/
						if (state != NULL) { // RA2: as above
							Point2D point(x + TEXT_X_OFFSET, y + 1); // RA2: the readout sits on a dimmed box
							Rect rect = Text_Rect(state, point, 0x142, 2, 1); // RA2: as above
							Blend_Rect(&rect, SidebarSurface, 0, 175); // RA2: as above
							Fancy_Text_Print(state, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x142)); // RA2: as above
						}

						if (!completed) { // RA2: as above

							Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_CLOCK], stage + 1, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_TRANSLUCENT50)); // RA2: one clock for all

							/*
							**	Display text showing that the construction is temporarily on hold.
							*/
							if (factory && !factory->Is_Building() || Buildables[index].Unknown_10 == 2) { // RA2: also an entry on hold (2)
								wchar_t const * hold = StringTable::LoadString("TXT_HOLD", NULL, "D:\\Ra2\\Sidebar.CPP", 3070); // RA2: wide text
								if (hasqueuecount) { // RA2: as above
									Point2D point(x + 2, y + 1); // RA2: left aligned next to the count
									Rect rect = Text_Rect(hold, point, 0x42, 2, 1); // RA2: as above
									Blend_Rect(&rect, SidebarSurface, 0, 175); // RA2: as above
									Fancy_Text_Print(hold, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x42)); // RA2: as above
								} else {
									Point2D point(x + TEXT_X_OFFSET, y + 1); // RA2: as above
									Rect rect = Text_Rect(hold, point, 0x142, 2, 1); // RA2: as above
									Blend_Rect(&rect, SidebarSurface, 0, 175); // RA2: as above
									Fancy_Text_Print(hold, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x142)); // RA2: as above
								}
							}
						}
					}
				}
			}
		}

		LastSlid = Slid;

	}
}
#endif


/***********************************************************************************************
 * SidebarClass::StripClass::Recalc -- Revalidates the current sidebar list of objects.        *
 *                                                                                             *
 *    This routine will revalidate all the buildable objects in the sidebar. This routine      *
 *    comes in handy when a factory has been destroyed, and the sidebar needs to reflect any   *
 *    change that this requires. It checks every object to see if there is a factory available *
 *    that could produce it. If none can be found, then the object is removed from the         *
 *    sidebar.                                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; The sidebar has changed as a result of this call?                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   06/26/1995 JLB : Doesn't collapse sidebar when buildables removed.                        *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (006802d0 has no row); main: best 62.52, tried 9 times (inline, order, frame)
bool SidebarClass::StripClass::Recalc(void)
{
	int ok;

	if (Debug_Map || !BuildableCount) {
		return(false);
	}

	bool scroll = false;
	int max_visible = Map.Get_Row_Count(); // RA2: as above
	BuildType * unshifted = new BuildType[max_visible];

	for (int i = 0; i < max_visible; i++) {
		if (i + TopIndex < BuildableCount) {
			unshifted[i] = Buildables[i + TopIndex];
		}
	}

	/*
	**	Sweep through all objects listed in the sidebar. If any of those object can
	**	not be created -- even in theory -- then they must be removed form the sidebar and
	**	any current production must be abandoned.
	*/
	bool redraw = false;
	for (int index = 0; index < BuildableCount; index++) {
		TechnoTypeClass const * tech = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
		if (tech != NULL) {
			BuildingClass const * who = tech->Who_Can_Build_Me(true, false, false, PlayerPtr);
			ok = who != NULL && who->House->Can_Build(tech, true, true);
		} else {
			if ((unsigned)Buildables[index].BuildableID < (unsigned)PlayerPtr->SuperWeapon.Count()) {
				ok = PlayerPtr->SuperWeapon[Buildables[index].BuildableID]->Is_Present();
			} else {
				ok = false;
			}
		}

		if (!ok) {
			for (int i = 0; i < max_visible; i++) {
				if (unshifted[i] == Buildables[index]) {
					unshifted[i] = BuildType(0, RTTI_NONE);
				}
			}

			/*
			**	Removes this entry from the list.
			*/
			if (BuildableCount > 1 && index < BuildableCount-1) {
				memmove(&Buildables[index], &Buildables[index+1], sizeof(Buildables[0])*((BuildableCount-index)-1));
			}
			redraw = true;
			scroll = true;
			Buildables[BuildableCount-1].Factory = NULL;
			IsToRedraw = true;
			BuildableCount--;
			index--;
		}
	}

	if (scroll) {
		bool got_old = false;
		bool got_new = false;

		int oldpos;
		for (oldpos = 0; oldpos < max_visible; oldpos++) {
			if (unshifted[oldpos] != BuildType(0, RTTI_NONE)) {
				got_old = true;
				break;
			}
		}

		int newpos;
		if (got_old && BuildableCount != 0) {
			for (newpos = 0; newpos < BuildableCount; newpos++) {
				if (Buildables[newpos] == unshifted[oldpos]) {
					got_new = true;
					break;
				}
			}
		}

		if (got_old && got_new) {
			TopIndex = newpos - oldpos;
			TopIndex = MAX(0, MIN(TopIndex, BuildableCount - max_visible));
		} else {
			TopIndex = 0;
		}
	}

	delete[] unshifted;
	return(redraw);
}
#endif


/***********************************************************************************************
 * SidebarClass::StripClass::SelectClass::SelectClass -- Default constructor.                  *
 *                                                                                             *
 *    This is the default constructor for the button that controls the buildable cameos on     *
 *    the sidebar strip.                                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The coordinates are set to zero by this routine. They must be set to the        *
 *             correct values before this button will function.                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
SidebarClass::StripClass::SelectClass::SelectClass(void) :
	ControlClass(0, 0, 0, 60, 48, LEFTPRESS | RIGHTPRESS | LEFTUP), // RA2: the 60 by 48 cameo (ra2-decomp)
	Strip(0),
	Index(0)
{
}


/***********************************************************************************************
 * SidebarClass::StripClass::SelectClass:: -- Assigns special values to a buildable select but *
 *                                                                                             *
 *    Use this routine to set custom buildable vars for this particular select button. It      *
 *    uses this information to properly know what buildable object to start or stop production *
 *    on.                                                                                      *
 *                                                                                             *
 * INPUT:   strip    -- Reference to the strip that owns this buildable button.                *
 *                                                                                             *
 *          index    -- The index (0 .. MAX_VISIBLE-1) of this button. This is used to let     *
 *                      the owning strip know what index this button refers to.                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::StripClass::SelectClass::Set_Owner(StripClass & strip, int index)
{
	Strip = &strip;
	Index = index;
}


bool SidebarClass::StripClass::SelectClass::Is_Hovered(void) // RA2: the mouse is over an enabled cameo
{
	return IsHovered && !IgnoreInput;
}


/***********************************************************************************************
 * SidebarClass::StripClass::SelectClass:: -- Action function when buildable cameo is selected *
 *                                                                                             *
 *    This function is called when the buildable icon (cameo) is clicked on. It handles        *
 *    starting and stopping production as indicated.                                           *
 *                                                                                             *
 * INPUT:   flags -- The input event that triggered the call.                                  *
 *                                                                                             *
 *          key   -- The keyboard value at the time of the input.                              *
 *                                                                                             *
 * OUTPUT:  Returns with whether the input list should be scanned further.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *   10/09/1996 JLB : Sonar pulse converted to regular event type.                             *
 *=============================================================================================*/
int SidebarClass::StripClass::SelectClass::Action(unsigned flags, KeyNumType & key)
{
	if (Strip == NULL) {
		return(1);
	}

	TechnoTypeClass const * choice = NULL;
	int spc = SUPER_NONE;

	int index = Strip->TopIndex * 2 + Index; // RA2: two cameo columns
	RTTIType otype = Strip->Buildables[index].BuildableType;
	int oid = Strip->Buildables[index].BuildableID;
	int category = Strip->Buildables[index].IsAlt;
	FactoryClass * factory = Strip->Buildables[index].Factory;

	Map.Override_Mouse_Shape(MOUSE_NORMAL);

	if (index < Strip->BuildableCount) {
		if (otype != RTTI_SPECIAL) {
			choice = Fetch_Techno_Type(otype, oid);
		} else {
			spc = oid;
		}
	}

	if (spc != SUPER_NONE) {

		/*
		**	Display the help text if the mouse is over the button.
		*/
		if (flags & LEFTUP) {
			flags &= ~LEFTUP;
		}

		/*
		**	A right mouse button signals "cancel".  If we are in targeting
		**	mode then we don't want to be any more.
		*/
		if (flags & RIGHTPRESS) {
			Map.IsTargettingMode = SUPER_NONE;
		}

		/*
		**	A left mouse press signal "activate".  If our weapon type is
		**	available then we should activate it.
		*/
		if (flags & LEFTPRESS) {
			if ((unsigned)spc < (unsigned)PlayerPtr->SuperWeapon.Count()) {
				VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
				SuperClass * super = PlayerPtr->SuperWeapon[spc];
				if (super->Can_Place()) {
					if (PlayerPtr->SuperWeapon[spc]->Class->Action == ACTION_NONE) {
						OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::SPECIAL_PLACE, PlayerPtr->SuperWeapon[spc]->Class->HeapID, Cell(0, 0)));
					} else {
						Map.PendingObjectPtr = NULL;
						Map.PendingObject = NULL;
						Map.PendingHouse = HOUSE_NONE;
						Map.Set_Cursor_Shape(NULL);
						Map.Repair_Mode_Control(0);
						Map.Sell_Mode_Control(0);
						Map.IsPowerMode = false;
						Map.IsWaypointMode = false;
						Map.IsBeaconMode = false;
						Map.IsTargettingMode = SuperWeaponType(spc);
						Unselect_All();
						Queue_Speech("EVA_SelectTarget", -1, -1);
					}
				} else {
					super->Impatient_Click();
				}
			}
		}

	} else if (choice != NULL) {

		/*
		**	Display the help text if the mouse is over the button.
		*/
		if (flags & LEFTUP) {
			flags &= ~LEFTUP;
		}

		/*
		**	A right mouse button signals "cancel".
		*/
		if (flags & RIGHTPRESS) {

			/*
			**	If production is in progress, put it on hold. If production is already
			**	on hold, then abandon it. Money will be refunded, the factory
			**	manager deleted, and the object under construction is returned to
			**	the free pool.
			*/
			if (factory != NULL) {

				/*
				**	Cancels placement mode if the sidebar factory is abandoned or
				**	suspended.
				*/
				if (Map.PendingObjectPtr && Dynamic_Cast<TechnoClass *>(Map.PendingObjectPtr)) {
					Map.PendingObjectPtr = NULL;
					Map.PendingObject = NULL;
					Map.PendingHouse = HOUSE_NONE;
					Map.Set_Cursor_Shape(NULL);
				}

				if (!factory->Is_Building()) {
					VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
					Queue_Speech("EVA_Canceled", -1, -1);
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ABANDON, otype, oid, choice->Naval));
				} else {
					VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
					Queue_Speech("EVA_OnHold", -1, -1);
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::SUSPEND, otype, oid, choice->Naval));
					Map.Column[Strip->ID].IsToRedraw = true;
				}
			} else {

				/*
				**	A pending order goes on hold; a queued one is cancelled.
				*/
				if (Strip->Buildables[index].Unknown_10 == 1) {
					Strip->Buildables[index].Unknown_10 = 2;
					Strip->Flag_To_Redraw();
					VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
					Queue_Speech("EVA_OnHold", -1, -1);
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::SUSPEND, otype, oid, choice->Naval));
				}
				FactoryClass * fptr = PlayerPtr->Fetch_Factory(otype, choice->Naval, category);
				if (fptr != NULL && fptr->Is_Queued(choice)) {
					VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ABANDON, otype, oid, choice->Naval));
				}
			}
		}

		if (flags & LEFTPRESS) {

			if (factory != NULL && !factory->Is_Building()) {

				/*
				**	If production has completed, then attempt to have the object exit
				**	the factory or go into placement mode.
				*/
				if (factory->Has_Completed()) {

					TechnoClass * pending = factory->Get_Object();
					if (!pending) {
						if (factory->Get_Special_Item() != -1) {
							Map.IsTargettingMode = SUPER_ANY;
						}
					} else {
						VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
						BuildingClass * builder = pending->Who_Can_Build_Me(false, false);
						if (!builder) {
							OutList.Add(EventClass(pending->Owner(), EventClass::ABANDON, otype, oid, choice->Naval));
							Queue_Speech("EVA_UnableToComply", -1, -1);
						} else if (pending->RTTI == RTTI_BUILDING) {

							/*
							**	If the completed object is a building, then change the
							**	game state into building placement mode. This fact is
							**	not transmitted to any linked computers until the moment
							**	the building is actually placed down.
							*/
							PlayerPtr->Manual_Place(builder, (BuildingClass *)pending);
						} else {

							/*
							**	For objects that can leave the factory under their own
							**	power, queue this event and process through normal house
							**	production channels.
							*/
							OutList.Add(EventClass(pending->Owner(), EventClass::PLACE, otype, -1, choice->Naval, CELL_NONE));
						}
					}
				} else {

					/*
					**	The factory must have been in a suspended state. Resume construction
					**	normally.
					*/
					VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
					if (otype == RTTI_INFANTRYTYPE) {
						Queue_Speech("EVA_Training", -1, -1);
					} else {
						Queue_Speech("EVA_Building", -1, -1);
					}
					TechnoTypeClass const * type = Fetch_Techno_Type(Strip->Buildables[index].BuildableType, Strip->Buildables[index].BuildableID);
					bool naval = type && type->Naval;
					PlayerPtr->Set_Production_Pending(otype, choice->Naval, category);
					OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::PRODUCE, Strip->Buildables[index].BuildableType, Strip->Buildables[index].BuildableID, naval));
					Strip->Buildables[index].Unknown_10 = 1;
					Strip->Buildables[index].Progress.Set_Stage(Strip->Get_Progress(index));
					int rate = factory->Build_Rate();
					Strip->Buildables[index].Progress.Set_Rate(rate + 1);
					Strip->Flag_To_Redraw();
					IsSidebarRedrawPending = true;
				}

			} else {

				/*
				**	If there is already a factory attached to this strip but the player didn't click
				**	on the icon that has the attached factory, then say that the factory is busy and
				**	ignore the click.
				*/
				factory = PlayerPtr->Fetch_Factory(otype, choice->Naval, category);
				TechnoTypeClass const * type = Fetch_Techno_Type(Strip->Buildables[index].BuildableType, Strip->Buildables[index].BuildableID);
				bool naval = type && type->Naval;
				bool limited = PlayerPtr->Is_Build_Limited(type);
				bool busy;
				bool produce = false;
				if (factory != NULL && (factory->Is_Building() || factory->Has_Production_Target())) {
					busy = true;
					if (otype == RTTI_BUILDINGTYPE) {
						Queue_Speech("EVA_UnableToComply", -1, -1);
					} else {
						produce = true;
					}
				} else {

					/*
					**	If this side strip is already busy with production, then ignore the
					**	input and announce this fact.
					*/
					busy = false;
					if (otype == RTTI_INFANTRYTYPE) {
						if (!limited) {
							Queue_Speech("EVA_Training", -1, -1);
						}
					} else {
						if (!limited) {
							Queue_Speech("EVA_Building", -1, -1);
						}
					}
					produce = true;
				}
				if (produce) {
					bool queued = PlayerPtr->Is_Production_Pending(otype, choice->Naval, category);
					bool started = false;
					if (!limited) {
						VocClass::PlayGlobal(Rule->GUIBuildSound, 1.0f, 0x2000);
						if (factory == NULL && !queued) {
							PlayerPtr->Set_Production_Pending(otype, choice->Naval, category);
							started = true;
						}
						OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::PRODUCE, Strip->Buildables[index].BuildableType, Strip->Buildables[index].BuildableID, naval));
					}
					if (started || !queued) {
						if (!busy && !limited) {
							Strip->Buildables[index].Unknown_10 = 1;
							Strip->Flag_To_Redraw();

							/*
							**	The clock starts at the frame the order can arrive, in steps of 54.
							*/
							TechnoTypeClass const * tech = Fetch_Techno_Type(otype, Strip->Buildables[index].BuildableID);
							if (tech) {
								int ahead = Session.MaxAhead;
								if (PlayerPtr->Available_Money() >= tech->Cost_Of(PlayerPtr) * ahead / 54
									&& tech->Who_Can_Build_Me(true, false, false, PlayerPtr)) {
									int time = tech->Time_To_Build();
									if (tech->RTTI == RTTI_BUILDINGTYPE && ((BuildingTypeClass const *)tech)->IsWall) {
										time = time * Rule->WallBuildSpeedCoefficient;
									}
									int least = ahead + 15;
									if (time <= least) {
										time = least;
									}
									int steps = time / 54;
									if (steps * 54 < least) {
										steps++;
									}
									if (steps < 1) {
										steps = 1;
									} else if (steps > 255) {
										steps = 255;
									}
									Strip->Buildables[index].Progress.Set_Stage(0);
									Strip->Buildables[index].Progress.Set_Rate(steps);
								}
							}
						}
					}
				}
			}
		}
	} else {
		flags = 0;
	}

	ControlClass::Action(flags, key);

	return(true);
}


/***********************************************************************************************
 * SidebarClass::StripClass::SelectClass::Mouse_Enter -- The mouse moved onto a cameo button.   *
 *                                                                                             *
 *    RA2: the tooltip comes up at once (its delay is saved, then cleared), the button marks    *
 *    itself hovered and the strip and the whole sidebar are redrawn.                          *
 *=============================================================================================*/
void SidebarClass::StripClass::SelectClass::Mouse_Enter(void)
{
	ToolTips->Save_Timer_Delay();
	ToolTips->Set_Timer_Delay(0);
	IsHovered = true;
	Strip->IsToRedraw = true;
	Map.Flag_To_Redraw();
	Map.IsForceCompleteRedraw = true;
	SidebarClass::IsToBlitSidebar = true;
}


/***********************************************************************************************
 * SidebarClass::StripClass::SelectClass::Mouse_Leave -- The mouse left a cameo button.         *
 *                                                                                             *
 *    RA2: the tooltip delay is restored and the hover mark cleared; then the same redraws.    *
 *=============================================================================================*/
void SidebarClass::StripClass::SelectClass::Mouse_Leave(void)
{
	ToolTips->Restore_Timer_Delay();
	IsHovered = false;
	Strip->IsToRedraw = true;
	Map.Flag_To_Redraw();
	Map.IsForceCompleteRedraw = true;
	SidebarClass::IsToBlitSidebar = true;
}


/***********************************************************************************************
 * SidebarClass::SBGadgetClass::Action -- Special function that controls the mouse over the si *
 *                                                                                             *
 *    This routine is called whenever the mouse is over the sidebar. It makes sure that the    *
 *    mouse is always the normal shape while over the sidebar.                                 *
 *                                                                                             *
 * INPUT:   flags -- The event flags that resulted in this routine being called.               *
 *                                                                                             *
 *          key   -- Reference the keyboard code that may be present.                          *
 *                                                                                             *
 * OUTPUT:  Returns that no further keyboard processing is necessary.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/28/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int SidebarClass::SBGadgetClass::Action(unsigned , KeyNumType & )
{
	Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
	return(true);
}


/***********************************************************************************************
 * SidebarClass::StripClass::Factory_Link -- Links a factory to a sidebar button.              *
 *                                                                                             *
 *    This routine will link the specified factory to this sidebar strip. The exact button to  *
 *    link to is determined from the object type and id specified. A linked button is one that *
 *    will show appropriate construction animation (clock shape) that matches the state of     *
 *    the factory.                                                                             *
 *                                                                                             *
 * INPUT:   factory  -- The factory number to link to the sidebar.                             *
 *                                                                                             *
 *          type     -- The object type that this factory refers to.                           *
 *                                                                                             *
 *          id       -- The object sub-type that this factory refers to.                       *
 *                                                                                             *
 * OUTPUT:  Was the factory successfully attached? Failure would indicate that there is no     *
 *          object of the specified type and sub-type in the sidebar list.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool SidebarClass::StripClass::Factory_Link(FactoryClass * factory, RTTIType type, int id)
{
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].BuildableType == type && Buildables[index].BuildableID == id) {
			Buildables[index].Factory = factory;
			IsBuilding = true;
			/*
			**	Flag that all the icons on this strip need to be redrawn
			*/
			Flag_To_Redraw();
			return(true);
		}
	}
	return(false);
}


bool SidebarClass::StripClass::Has_Buildables(void) const // RA2: as above
{
	return BuildableCount > 0; // RA2: as above
}
/// <summary>
/// Abandons the production of an object kind in the strip it is built from.
/// </summary>
/// <param name="type">The object type's RTTI.</param>
/// <param name="id">The object type's index; with the kind it gives the strip.</param>
/// <param name="factory">The factory that was producing.</param>
/// <returns>Was the factory abandoned?</returns>
bool SidebarClass::Abandon_Production(RTTIType type, int id, FactoryClass * factory) // RA2: 0x681740 takes the object id too; the strip comes from Which_Tab, whose body is expanded here
{
	return(Column[Which_Tab(type, id, false)].Abandon_Production(factory)); // RA2: as above
}


/// <summary>
/// Determines which sidebar tab an object type is built from.
/// </summary>
/// <param name="type">The object type's RTTI.</param>
/// <param name="id">The object type's index.</param>
/// <param name="naval">Unused.</param>
/// <returns>The tab (0 to 3), or -1 if the type is not built from the sidebar.</returns>
int SidebarClass::Which_Tab(RTTIType type, int id, bool naval) // RA2: 0x6818D0; the calls in this file expand it
{
	if (type == RTTI_INFANTRY || type == RTTI_INFANTRYTYPE) { // RA2: as above
		return 2; // RA2: as above
	}
	if (type == RTTI_UNIT || type == RTTI_UNITTYPE || type == RTTI_AIRCRAFT || type == RTTI_AIRCRAFTTYPE) { // RA2: as above
		return 3; // RA2: as above
	}
	if (type == RTTI_BUILDING || type == RTTI_BUILDINGTYPE) { // RA2: as above
		return HouseClass::Build_Category(type, id) == 5; // RA2: as above
	}
	if (type == RTTI_SUPERWEAPON || type == RTTI_SUPERWEAPONTYPE || type == RTTI_SPECIAL) { // RA2: as above
		return 1; // RA2: as above
	}
	return -1; // RA2: as above
}




/***********************************************************************************************
 * SidebarClass::Abandon_Production -- Stops production of the object specified.               *
 *                                                                                             *
 *    This routine is used to abandon production of the object specified. The factory will     *
 *    be completely disabled by this call.                                                     *
 *                                                                                             *
 * INPUT:   type     -- The object type that is to be abandoned. The sub-type is not needed    *
 *                      since it is presumed there can be only one type in production at any   *
 *                      one time.                                                              *
 *                                                                                             *
 *          factory  -- The factory number that is doing the production.                       *
 *                                                                                             *
 * OUTPUT:  Was the factory successfully abandoned?                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool SidebarClass::Abandon_Production(RTTIType type, FactoryClass * factory)
{
	return(Column[Which_Column(type)].Abandon_Production(factory));
}
#endif


static inline bool Is_Building(TechnoClass * object) // RA2: ra2-decomp's sidebar.cpp text; expanded in Abandon_Production
{
	return(object != NULL && object->Fetch_RTTI() == RTTI_BUILDING);
}


static inline BuildingClass * As_Building(TechnoClass * object) // RA2: as in temporal.cpp (ra2-decomp's cast.h)
{
	return((object == NULL) ? NULL : ((object->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)object : NULL));
}


/***********************************************************************************************
 * SidebarClass::StripClass::Abandon_Produ -- Abandons production associated with sidebar.     *
 *                                                                                             *
 *    Production of the object associated with this sidebar is abandoned when this routine is  *
 *    called.                                                                                  *
 *                                                                                             *
 * INPUT:   factory  -- The factory index that is to be suspended.                             *
 *                                                                                             *
 * OUTPUT:  Was the production abandonment successful?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/18/1995 JLB : Created.                                                                 *
 *   08/06/1995 JLB : More intelligent abandon logic for multiple factories.                   *
 *=============================================================================================*/
bool SidebarClass::StripClass::Abandon_Production(FactoryClass const * factory)
{
	/*
	**	A building whose production is abandoned is no longer the object of a sidebar tab.
	*/
	if (factory != NULL && factory->Get_Object() != NULL && Is_Building(factory->Get_Object())) { // RA2: ra2-decomp's text (YR twin 0x6ABB60)
		Clear_Sidebar_Tab_Object(As_Building(factory->Get_Object())); // RA2: as above
	}

	bool abandon = false; // RA2: as above
	bool noprod = true; // RA2: as above
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].Factory == factory) {
			((FactoryClass *)factory)->Abandon();
			Buildables[index].Factory = NULL;
			abandon = true;
			Buildables[index].Unknown_10 = 0; // RA2: as above
		} else {
			if (Buildables[index].Factory != NULL) {
				noprod = false;
			}
		}
	}

	/*
	**	If there was a change to the strip, then flag the strip to be redrawn.
	*/
	if (abandon) {
		Flag_To_Redraw();
		IsSidebarRedrawPending = true; // RA2: as above
	}

	/*
	**	If there is no production whatsoever on this strip, then flag it so.
	*/
	if (noprod) {
		IsBuilding = false;
	}
	return(abandon);
}


/***********************************************************************************************
 * SidebarClass::Zoom_Mode_Control -- Handles the zoom mode toggle operation.                  *
 *                                                                                             *
 *    This is the function that is called when the map button is pressed. It will toggle       *
 *    between the different modes that the radar map can assume.                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/31/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void SidebarClass::Zoom_Mode_Control(void)
{
	/*
	**	RA2: the radar no longer cycles; in a multiplayer game the key brings up the special dialog.
	*/
	if (Session.Type != GAME_NORMAL) {
		SpecialDialog = SDLG_SPECIAL;
	}
}

/// <summary>
/// Determines which sidebar tab an object type is built from, when the build category is known already.
/// </summary>
/// <param name="type">The object type's RTTI (only the type classes are tabbed this way).</param>
/// <param name="category">The build category of the object (5 for the defense buildings).</param>
/// <param name="naval">Unused.</param>
/// <returns>The tab (0 to 3), or -1 if the kind is not built from the sidebar.</returns>
int Sidebar_Which_Tab_By_Category(RTTIType type, int category, bool naval) // RA2: 0x681940 (YR: SidebarClass::GetObjectTabIdxByCat)
{
	if (type == RTTI_INFANTRYTYPE) { // RA2: as above
		return 2; // RA2: as above
	}
	if (type == RTTI_UNITTYPE || type == RTTI_AIRCRAFTTYPE) { // RA2: as above
		return 3; // RA2: as above
	}
	if (type == RTTI_BUILDINGTYPE) { // RA2: as above
		return category == 5; // RA2: as above
	}
	return -1; // RA2: as above
}



/// <summary>
/// Repositions the sidebar and everything that sits on it.
/// This routine is called whenever the sidebar changes which edge of the screen it lives
/// on, or the display it must fit into changes size. The sidebar rectangle is placed first
/// and then the mode buttons, scroll arrows, cameo buttons and their tooltip regions are
/// moved to suit.
/// </summary>
int SidebarClass::First_Used_Column() // RA2: as above
{
	for (int index = 0; index < 4; index++) { // RA2: as above
		if (Column[index].BuildableCount > 0) { // RA2: as above
			return index; // RA2: as above
		}
	}
	return -1; // RA2: as above
}


#ifdef RA2_NON_MATCHING // not exact: 4 bytes differ (calibrated to an incomplete unit; flipped by sidebar Action and the trect.h split)
void SidebarClass::Reposition_Sidebar() // RA2: as above
{
	PowerClass::Reposition_Sidebar(); // RA2: as above

	/*
	 * Position the sidebar.
	 */
	if (Options.IsSidebarOnRight) {
		SidebarRect.X = TacticalRect.X + TacticalRect.Width;
	} else {
		SidebarRect.X = 0; // RA2: as above
	}
	StripLeft = SidebarRect.X + 0x16; // RA2: as above
	if (Scen->IsGDI) { // RA2: as above
		TopButtonX = SidebarRect.X + 0x14; // RA2: as above
		TabButtonX = SidebarRect.X + 0x1A; // RA2: as above
		PowerBarLeft = SidebarRect.X + 0x27; // RA2: as above
	} else {
		TopButtonX = SidebarRect.X + 0x21; // RA2: as above
		TabButtonX = SidebarRect.X + 0x14; // RA2: as above
		PowerBarLeft = SidebarRect.X + 0x27; // RA2: as above
	}

	/*
	 * Position the sidebar's buttons.
	 */
	if (SidebarShapes[SBSHAPE_TOP] == 0) { // RA2: as above
		SidebarShapes[SBSHAPE_TOP] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI1.SHP"); // RA2: as above
		SidebarShapes[SBSHAPE_MIDDLE] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI2.SHP"); // RA2: as above
		SidebarShapes[SBSHAPE_BOTTOM] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI3.SHP"); // RA2: as above
	}

	Background.Set_Position(SidebarRect.X + 0x10, TacticalRect.Y); // RA2: as above
	Background.Flag_To_Redraw();
	int top = RadarButton.Y + RadarButton.Height; // RA2: as above
	if (Options.IsSidebarOnRight) {
		Background.Set_Position(TacticalRect.X + TacticalRect.Width, top); // RA2: as above
	} else {
		Background.Set_Position(0, top); // RA2: as above
	}
	Background.Set_Size(SidebarSurface->Get_Width(), SidebarSurface->Get_Height() - top); // RA2: as above

	Upgrade.Set_Position(TopButtonX, TopButtonY); // RA2: as above
	Upgrade.Flag_To_Redraw();
	Upgrade.DrawOffsetX = -SidebarRect.X;

	Repair.Set_Position(Upgrade.X + TopButtonSpacing, Upgrade.Y); // RA2: as above
	Repair.Flag_To_Redraw();
	Repair.DrawOffsetX = -SidebarRect.X;

	for (int tab = 0; tab < 4; tab++) { // RA2: as above
		TabButton[tab].Set_Position(TabButtonSpacing * tab + TabButtonX, TabButtonY); // RA2: as above
		TabButton[tab].Flag_To_Redraw(); // RA2: as above
		TabButton[tab].DrawOffsetX = -SidebarRect.X; // RA2: as above
	}

	ScrollDownButton.Set_Position(PowerBarLeft, SidebarBottomY); // RA2: as above
	ScrollDownButton.Flag_To_Redraw(); // RA2: as above
	ScrollDownButton.DrawOffsetX = -SidebarRect.X; // RA2: as above

	ScrollUpButton.Set_Position(PowerBarLeft + SidebarEdgeWidth, SidebarBottomY); // RA2: as above
	ScrollUpButton.Flag_To_Redraw(); // RA2: as above
	ScrollUpButton.DrawOffsetX = -SidebarRect.X; // RA2: as above

	/*
	 * Create the tooltips for the sidebar.
	 */
	int strip_top = StripTop; // RA2: the strip top is read before the side's bottom piece
	int extra;
	if (Scen->IsGDI) {
		extra = 26;
	} else {
		extra = 18;
	}
	int rows = (SidebarRect.Y + SidebarRect.Height - strip_top - extra - 7) / 50;
	int count = rows * 2; // RA2: as above
	for (int column = 0; column < 4; column++) { // RA2: as above
		StripClass::SelectClass *button = &StripClass::SelectButton[column * 60]; // RA2: as above
		for (int row = 0; row < rows; row++) { // RA2: as above
			for (int side = 0; side < 2; side++, button++) { // RA2: as above
				button->Set_Position(CameoWidth * side + StripLeft, CameoHeight * row + StripTop + 1); // RA2: as above
				button->Flag_To_Redraw(); // RA2: as above
			}
		}
	}

	if (ToolTips != 0) { // RA2: as above
		ToolTip tip; // RA2: as above
		for (int tab = 0; tab < 4; tab++) { // RA2: as above
			tip.ID = tab + 0xCB; // RA2: as above
			tip.Text = 0; // RA2: as above
			tip.Region.X = TabButton[tab].X; // RA2: as above
			tip.Region.Y = TabButton[tab].Y; // RA2: as above
			tip.Region.Width = TabButton[tab].Width; // RA2: as above
			tip.Region.Height = TabButton[tab].Height; // RA2: as above
			ToolTips->Remove(tip.ID); // RA2: as above
			ToolTips->Add(&tip); // RA2: as above
		}
		for (int index = 0; index < count; index++) { // RA2: as above
			tip.ID = index + 1000; // RA2: as above
			tip.Text = 0; // RA2: as above
			tip.Region.X = StripClass::SelectButton[index].X; // RA2: as above
			tip.Region.Y = StripClass::SelectButton[index].Y; // RA2: as above
			tip.Region.Width = StripClass::SelectButton[index].Width; // RA2: as above
			tip.Region.Height = StripClass::SelectButton[index].Height; // RA2: as above
			tip.IsAnchored = true; // RA2: as above
			ToolTips->Remove(tip.ID); // RA2: as above
			ToolTips->Add(&tip); // RA2: as above
		}
		tip.IsAnchored = false; // RA2: as above
		tip.ID = 0x66; // RA2: as above
		tip.Text = (int)"TXT_SELL_MODE"; // RA2: the label of the string table
		tip.Region.X = Repair.X; // RA2: as above
		tip.Region.Y = Repair.Y; // RA2: as above
		tip.Region.Width = Repair.Width; // RA2: as above
		tip.Region.Height = Repair.Height; // RA2: as above
		ToolTips->Remove(0x66); // RA2: as above
		ToolTips->Add(&tip); // RA2: as above
		tip.ID = 0x65; // RA2: as above
		tip.Text = (int)"TXT_REPAIR_MODE"; // RA2: as above
		tip.Region.X = Upgrade.X; // RA2: as above
		tip.Region.Y = Upgrade.Y; // RA2: as above
		tip.Region.Width = Upgrade.Width; // RA2: as above
		tip.Region.Height = Upgrade.Height; // RA2: as above
		ToolTips->Remove(0x65); // RA2: as above
		ToolTips->Add(&tip); // RA2: as above
		tip.ID = 0xC8; // RA2: as above
		tip.Text = 0; // RA2: as above
		tip.Region.X = ScrollUpButton.X; // RA2: as above
		tip.Region.Y = ScrollUpButton.Y; // RA2: as above
		tip.Region.Width = ScrollUpButton.Width; // RA2: as above
		tip.Region.Height = ScrollUpButton.Height; // RA2: as above
		ToolTips->Remove(0xC8); // RA2: as above
		ToolTips->Add(&tip); // RA2: as above
		tip.ID = 0xC9; // RA2: as above
		tip.Text = 0; // RA2: as above
		tip.Region.X = ScrollDownButton.X; // RA2: as above
		tip.Region.Y = ScrollDownButton.Y; // RA2: as above
		tip.Region.Width = ScrollDownButton.Width; // RA2: as above
		tip.Region.Height = ScrollDownButton.Height; // RA2: as above
		ToolTips->Remove(0xC9); // RA2: as above
		ToolTips->Add(&tip); // RA2: as above
	}
}
#endif


/// <summary>
/// Fetches the tooltip text for a sidebar gadget.
/// This routine is called by the tooltip manager. Any gadget the map layers underneath do
/// not claim is taken to be a build cameo and handed to the strip that owns it.
/// </summary>
/// <param name="id">The gadget identifier that text is wanted for.</param>
/// <returns>Returns with a pointer to the help text, or NULL if the gadget has none.</returns>
const char * SidebarClass::Help_Text(int id) // RA2: the text is wide
{
	wchar_t const * text = (wchar_t const *)BASECLASS::Help_Text(id);
	if (text == NULL) {
		bool disabled = false;
		if (id == 200) { // RA2: the scroll and tab buttons have tips, marked when the button is off
			text = StringTable::LoadString("Tip:ScrollUp", NULL, "D:\\Ra2\\Sidebar.CPP", 4297);
			disabled = !ScrollUpButton.Is_Enabled();
		} else if (id == 201) {
			text = StringTable::LoadString("Tip:ScrollDown", NULL, "D:\\Ra2\\Sidebar.CPP", 4303);
			disabled = !ScrollDownButton.Is_Enabled();
		} else if (id >= 203 && id < 207) {
			int tab = id - 203;
			if (tab == 0) {
				text = StringTable::LoadString("Tip:Tab1", NULL, "D:\\Ra2\\Sidebar.CPP", 4311);
			} else if (tab == 1) {
				text = StringTable::LoadString("Tip:Tab2", NULL, "D:\\Ra2\\Sidebar.CPP", 4313);
			} else if (tab == 2) {
				text = StringTable::LoadString("Tip:Tab3", NULL, "D:\\Ra2\\Sidebar.CPP", 4315);
			} else if (tab == 3) {
				text = StringTable::LoadString("Tip:Tab4", NULL, "D:\\Ra2\\Sidebar.CPP", 4317);
			}
			disabled = !TabButton[tab].Is_Enabled();
		} else {
			int count = Get_Doubled_Row_Count(); // RA2: the cameos of the active strip
			if (id >= 1000 && id < 1000 + count) {
				return((char const *)Column[ActiveTabIndex].Help_Text(id - 1000));
			}
		}
		if (disabled) {
			swprintf(ButtonHelpBuffer, L"%s\n(%s)", text, StringTable::LoadString("Tip:Disabled", NULL, "D:\\Ra2\\Sidebar.CPP", 4336));
			return((char const *)ButtonHelpBuffer);
		}
	}
	return((char const *)text);
}


/// <summary>
/// Determines how many cameos a sidebar strip can show.
/// The answer follows from the height of the sidebar, so it varies with the screen
/// resolution. It is used to lay the strips out and to decide when scrolling is called for.
/// </summary>
/// <returns>Returns with the number of build cameos that will fit in one strip.</returns>
int SidebarClass::Get_Row_Count(void) // RA2: as above
{
	int extra; // RA2: as above
	if (Scen->IsGDI) { // RA2: 0x6820A0
		extra = 26; // RA2: as above
	} else {
		extra = 18; // RA2: as above
	}
	return (SidebarRect.Y + SidebarRect.Height - StripTop - extra - 7) / 50; // RA2: as above
}


int SidebarClass::Get_Doubled_Row_Count() // RA2: as above
{
	int const rows = Get_Row_Count(); // RA2: as above
	int const count = rows + rows; // RA2: as above
	return count; // RA2: as above
}


/// <summary>
/// Prints the descriptive text that goes with a sidebar cameo.
/// This routine is used by the strip renderer to caption a cameo with its name and cost.
/// Text too wide for the space is broken at a space or a hyphen and carried onto as many
/// lines as it takes.
/// </summary>
/// <param name="point">Where to print the last line. Earlier lines stack upward from
/// there.</param>
/// <param name="cliprect">The clipping rectangle to print within.</param>
/// <param name="maxlinelen">The width, in pixels, a line may reach before it is broken.</param>
/// <remarks>Nothing at all is printed unless the player has cameo text turned on.</remarks>
void Print_Cameo_Text(wchar_t const * string, Point2D const & point, Rect const & cliprect, int maxlinelen) // RA2: as above
{
	if (IsCameoTextShown && string != NULL) { // RA2: 0x682140
		Point2D drawpoint = point;
		BitFont * font = BitFont::Instance; // RA2: as above
		ColorScheme * scheme = Fetch_Scheme_By_Name("DarkRed", 1); // RA2: as above
		RGBClass rgb = scheme->HSV; // RA2: as above
		unsigned color = ((rgb.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((rgb.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((rgb.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight); // RA2: as above
		LTRBStruct box; // RA2: as above
		box.Left = cliprect.X; // RA2: as above
		box.Top = cliprect.Y; // RA2: as above
		box.Right = cliprect.X + cliprect.Width; // RA2: as above
		box.Bottom = cliprect.Y + cliprect.Height; // RA2: as above
		font->Set_Clip(true); // RA2: as above
		font->Set_Bounds(&box); // RA2: as above
		font->Set_Color(color); // RA2: as above
		int textwidth; // RA2: as above
		int textheight; // RA2: as above
		font->Text_Dimensions(string, &textwidth, &textheight, maxlinelen); // RA2: as above
		drawpoint.Y += font->LineHeight - textheight; // RA2: as above
		BitText::Instance->Draw_Text(font, SidebarSurface, string, drawpoint.X, drawpoint.Y, maxlinelen, 0, 0, 0, 0); // RA2: as above
	}
}


/// <summary>
/// Loads the sidebar from the save game stream.
/// There is nothing of the sidebar's own to read back, so the stream is simply passed down
/// to the map layers underneath it.
/// </summary>
/// <returns>Returns with S_OK if the load succeeded.</returns>
HRESULT SidebarClass::Load(IStream * stream)
{
	return BASECLASS::Load(stream); // RA2: as above
}


/// <summary>
/// Saves the sidebar to the save game stream.
/// The sidebar keeps nothing of its own that must survive a save, so the real work falls to
/// the map layers underneath it.
/// </summary>
/// <returns>Returns with S_OK if the save succeeded.</returns>
HRESULT SidebarClass::Save(IStream * stream)
{
	return BASECLASS::Save(stream); // RA2: as above
}


void SidebarClass::StripClass::Set_Flash(int id, int delay) // RA2: as above
{
	for (int index = 0; index < 75; index++) { // RA2: as above
		if (Buildables[index].BuildableID == id) { // RA2: as above
			Buildables[index].FlashEndFrame = Frame + delay; // RA2: as above
			return;
		}
	}
}
/***********************************************************************************************
 * SidebarClass::Reload_Sidebar -- Loads appropriate sidebar shapes depending on house         *
 *                                                                                             *
 * INPUT:  none                                                                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   9/18/1996 BWG : Created.                                                                  *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void SidebarClass::Reload_Sidebar(void)
{
	Column[0].X = COLUMN_ONE_X;
	Column[0].Y = COLUMN_ONE_Y;
	Column[0].ObjectRect = Rect(Column[0].X, Column[0].Y, StripClass::OBJECT_WIDTH, (StripClass::OBJECT_HEIGHT) * Get_Row_Count()); // RA2: as above

	Column[1].X = COLUMN_TWO_X;
	Column[1].Y = COLUMN_ONE_Y;
	Column[1].ObjectRect = Rect(Column[1].X, Column[1].Y, StripClass::OBJECT_WIDTH, (StripClass::OBJECT_HEIGHT) * Get_Row_Count()); // RA2: as above
}
#endif


/***********************************************************************************************
 * SidebarClass::Which_Column -- Determines which column a given type should appear.           *
 *                                                                                             *
 *    Use this function to resolve what column the specified object type should be placed      *
 *    into.                                                                                    *
 *                                                                                             *
 * INPUT:   otype -- Pointer to the object type class of the object in question.               *
 *                                                                                             *
 * OUTPUT:  Returns with the column number that the object should be placed in.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
int SidebarClass::Which_Column(RTTIType type)
{
	if (type == RTTI_BUILDINGTYPE || type == RTTI_BUILDING) {
		return(0);
	}
	return(1);
}
#endif


/***********************************************************************************************
 * SidebarClass::Activate_Upgrade -- Controls the upgrade button on the sidebar.               *
 *                                                                                             *
 *    Use this routine to turn the upgrade sidebar button on and off. Typically, the button    *
 *    is enabled when the currently selected structure can be upgraded and disabled otherwise. *
 *                                                                                             *
 * INPUT:   control  -- The controls how the button is to be activated or deactivated;         *
 *                      0  -- Turn button off.                                                 *
 *                      1  -- Turn button on.                                                  *
 *                      -1 -- Toggle button state.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the button previously activated?                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/24/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
bool SidebarClass::Activate_Upgrade(int control)
{
	return(false); // RA2: RA2 has no upgrade button
}
#endif


// RA2: flashes a techno type's cameo and keeps the sidebar updating until it ends.
void SidebarClass::Flash_Cameo(TechnoTypeClass const * type, int frames)
{
	int index;
	for (index = 0; index < InfantryTypes.Count(); index++) {
		if ((TechnoTypeClass *)InfantryTypes[index] == type) {
			goto found;
		}
	}
	for (index = 0; index < UnitTypes.Count(); index++) {
		if (type == (TechnoTypeClass *)UnitTypes[index]) {
			goto found;
		}
	}
	for (index = 0; index < AircraftTypes.Count(); index++) {
		if (type == (TechnoTypeClass *)AircraftTypes[index]) {
			goto found;
		}
	}
	for (index = 0; index < BuildingTypes.Count(); index++) {
		if (type == (TechnoTypeClass *)BuildingTypes[index]) {
			goto found;
		}
	}
	return;

found:
	RTTIType kind = type->Fetch_RTTI();
	int column;
	if (kind == RTTI_INFANTRY || kind == RTTI_INFANTRYTYPE) {
		column = 2;
	} else if (kind == RTTI_UNIT || kind == RTTI_UNITTYPE || kind == RTTI_AIRCRAFT || kind == RTTI_AIRCRAFTTYPE) {
		column = 3;
	} else if (kind == RTTI_BUILDING || kind == RTTI_BUILDINGTYPE) {
		column = (HouseClass::Build_Category(kind, index) == 5);
	} else if (kind == RTTI_SUPERWEAPON || kind == RTTI_SUPERWEAPONTYPE || kind == RTTI_SPECIAL) {
		column = 1;
	} else {
		return;
	}
	Column[column].Set_Flash(index, frames);
	RedrawUntilFrame = Frame + 1 + frames;
}
