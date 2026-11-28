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
	return Scen->IsGDI ? 26 : 18;
}
inline int Max_Visible_Rows(void) // RA2: the rows that fit, inline in Draw_It (0x67CF80) and AI (0x67DC90); Max_Visible (0x6820A0) has the same term out of line
{
	return (SidebarRect.Y + SidebarRect.Height - SidebarClass::StripTop - (Scen->IsGDI ? 26 : 18) - 7) / 50;
}
PaletteClass * Get_Game_Palette(void); // RA2: 0x6F45E0, the pointer at 0xAC11A0 (the sidebar layout's module)
void Set_Tab_Building(BuildingClass * building); // RA2: 0x6F8A00; the structure or defense tab (by the building type's build category) shows this building
void Clear_Sidebar_Tab_Object(TechnoClass * techno); // RA2: 0x6F8A20 (scenario.cpp), forgets techno in the sidebar tab slots
namespace Unsorted { extern bool ArmageddonMode; }
namespace Game { extern bool IsActive; extern HWND hWnd; }
namespace Unsorted { extern bool ScenarioStarted; }
namespace Game { extern unsigned char RecordingFlag; }
namespace Game { extern int SpecialDialog; }
namespace Unsorted { extern int WSDialogCount; }
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
	RedrawUntilFrame(0),
	IsCameoText(false),
	IsSidebarActive(false),
	IsToRedraw(true),
	IsToRedrawCredits(false),
	IsDemolishActive(false),
	IsRepairActive(false)
{
	/*
	**	Set up the coordinates for the sidebar strips. These coordinates are for
	**	the upper left corner.
	**	Set up the strips (RA2 has four, one per tab).
	*/
	for (int index = 0; index < STRIPS; index++) { // RA2: four strips
		new (&Column[index]) StripClass(InitClass());
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
void SidebarClass::Set_Cameo_Text(bool state)
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

	for (int index = 0; index < STRIPS; index++) {
		Column[index].One_Time(index);
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
	PowerClass::Init_Clear();
	IsToRedraw = true;
	IsRepairActive = false;
	IsDemolishActive = false;
	for (int index = 0; index < 4; index++) {
		Column[index].Clear_Buildables();
	}
	ActiveTabIndex = 0;
	RedrawUntilFrame = 0;
	Activate(0);
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
void SidebarClass::Recalc_Layout()
{
	SidebarRect.Width = SidebarWidth;
	SidebarRect.Y = 0x9E;
	SidebarRect.Height = TacticalRect.Y + TacticalRect.Height + TabBarHeight - 0x9E;
	if (Scen->IsGDI) {
		TopButtonY = 0xA6;
		TopButtonSpacing = 0x40;
		TabButtonY = 0xC5;
		TabButtonSpacing = 0x1D;
		StripTop = 0xE3;
		CameoWidth = 0x3F;
	} else {
		TopButtonY = 0xA5;
		TopButtonSpacing = 0x34;
		TabButtonY = 0xC5;
		TabButtonSpacing = 0x20;
		StripTop = 0xE3;
		CameoWidth = 0x40;
	}
	CameoHeight = 0x32;
	int extra;
	if (Scen->IsGDI) {
		extra = 26;
	} else {
		extra = 18;
	}
	PowerBarHeight = (SidebarRect.Height - extra - 0x4C) / 50 * 50;
	if (Scen->IsGDI) {
		SidebarBottomY = PowerBarHeight + 0xEA;
		SidebarEdgeWidth = 0x2E;
		SidebarEdgeStep = 0x32;
	} else {
		SidebarBottomY = PowerBarHeight + 0xEA;
		SidebarEdgeWidth = 0x2D;
		SidebarEdgeStep = 0x32;
	}
}


void SidebarClass::Recalc_Horizontal_Layout()
{
	if (Options.IsSidebarOnRight) {
		SidebarRect.X = TacticalRect.X + TacticalRect.Width;
	} else {
		SidebarRect.X = 0;
	}
	StripLeft = SidebarRect.X + 22;
	if (Scen->IsGDI) {
		TopButtonX = SidebarRect.X + 20;
		TabButtonX = SidebarRect.X + 26;
		PowerBarLeft = SidebarRect.X + 39;
	} else {
		TopButtonX = SidebarRect.X + 33;
		TabButtonX = SidebarRect.X + 20;
		PowerBarLeft = SidebarRect.X + 39;
	}
}


void SidebarClass::Init_IO()
{
	RadarClass::Init_IO();
	Recalc_Layout();
	Recalc_Horizontal_Layout();
	if (Debug_Map) {
		return;
	}

	Upgrade.IsSticky = true;
	Upgrade.ID = 101;
	Upgrade.X = TopButtonX;
	Upgrade.Y = TopButtonY;
	Upgrade.DrawOffsetX = -480;
	Upgrade.DrawOffsetY = 0;
	Upgrade.DrawOnSidebar = true;
	Upgrade.ShapeDrawer = SidebarDrawer;
	Upgrade.IsOn = false;
	Upgrade.IsToggleType = 1;
	Upgrade.ReflectButtonState = true;
	Upgrade.Stop_Flashing();

	/*
	**	Add the sidebar's buttons only if we're not in editor mode.
	*/
	Repair.IsSticky = true;
	Repair.ID = 102;
	Repair.X = Upgrade.X + TopButtonSpacing;
	Repair.Y = Upgrade.Y;
	Repair.DrawOffsetX = -480;
	Repair.DrawOffsetY = 0;
	Repair.DrawOnSidebar = true;
	Repair.ShapeDrawer = SidebarDrawer;
	Repair.IsOn = false;
	Repair.IsToggleType = 1;
	Repair.ReflectButtonState = true;
	Repair.Stop_Flashing();

	for (int index = 0; index < 4; index++) {
		TabButton[index].IsSticky = true;
		TabButton[index].ID = 203 + index;
		TabButton[index].X = TabButtonSpacing * index + TabButtonX;
		TabButton[index].Y = TabButtonY;
		TabButton[index].DrawOffsetX = -480;
		TabButton[index].DrawOffsetY = 0;
		TabButton[index].DrawOnSidebar = true;
		TabButton[index].ShapeDrawer = SidebarDrawer;
		TabButton[index].IsOn = false;
		TabButton[index].IsToggleType = 2;
		TabButton[index].ReflectButtonState = true;
		TabButton[index].Disable();
		TabButton[index].Stop_Flashing();
	}

	ScrollDownButton.IsSticky = true;
	ScrollDownButton.ID = 201;
	ScrollDownButton.DrawOnSidebar = true;
	ScrollDownButton.ShapeDrawer = SidebarDrawer;
	ScrollDownButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE | GadgetClass::RIGHTPRESS | GadgetClass::RIGHTRELEASE);
	ScrollDownButton.Stop_Flashing();

	ScrollUpButton.IsSticky = true;
	ScrollUpButton.ID = 200;
	ScrollUpButton.DrawOnSidebar = true;
	ScrollUpButton.ShapeDrawer = SidebarDrawer;
	ScrollUpButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE | GadgetClass::RIGHTPRESS | GadgetClass::RIGHTRELEASE);
	ScrollUpButton.Stop_Flashing();

	for (int column = 0; column < 4; column++) {
		Column[column].X = StripLeft;
		Column[column].Y = StripTop;
		Column[column].ObjectRect = Rect(0, 0, 60, PowerBarHeight);
		Column[column].Init_Buttons(column);
	}
	Reposition_Sidebar();
	if (IsSidebarActive) {
		Activate(0);
		Activate(1);
	}
}


void SidebarClass::Init_For_House()
{
	PowerClass::Init_For_House();
	PaletteClass palette = *Get_Game_Palette();
	if (SidebarDrawer) {
		delete SidebarDrawer;
		SidebarDrawer = 0;
	}
	SidebarDrawer = new ConvertClass(palette, palette, *VisibleSurface, 1, false);
	SidebarShapes[SBSHAPE_CLOCK] = (ShapeSet const *)Load_Alloc_Data("GCLOCK2.SHP", SidebarShapeAllocated[SBSHAPE_CLOCK]);

	bool allocated = false;
	Repair.Set_Shape((ShapeSet const *)Load_Alloc_Data("SELL.SHP", allocated));
	Repair.Set_Shape_Loaded(allocated);
	Repair.ShapeDrawer = SidebarDrawer;

		/*
		**	If a game was loaded & the sidebar was enabled, pop it up now
		*/
	Upgrade.Set_Shape((ShapeSet const *)Load_Alloc_Data("REPAIR.SHP", allocated));
	Upgrade.Set_Shape_Loaded(allocated);
	Upgrade.ShapeDrawer = SidebarDrawer;

	for (int index = 0; index < 4; index++) {
		char name[256];
		sprintf(name, "TAB%02d.SHP", index);
		TabButton[index].Set_Shape((ShapeSet const *)Load_Alloc_Data(name, allocated));
		TabButton[index].Set_Shape_Loaded(allocated);
		TabButton[index].ShapeDrawer = SidebarDrawer;
	}

	ScrollDownButton.Set_Shape((ShapeSet const *)Load_Alloc_Data("R-DN.SHP", allocated));
	ScrollDownButton.Set_Shape_Loaded(allocated);
	ScrollDownButton.ShapeDrawer = SidebarDrawer;

	ScrollUpButton.Set_Shape((ShapeSet const *)Load_Alloc_Data("R-UP.SHP", allocated));
	ScrollUpButton.Set_Shape_Loaded(allocated);
	ScrollUpButton.ShapeDrawer = SidebarDrawer;

	SidebarShapes[SBSHAPE_TOP] = (ShapeSet const *)Load_Alloc_Data("SIDE1.SHP", SidebarShapeAllocated[SBSHAPE_TOP]);
	SidebarShapes[SBSHAPE_MIDDLE] = (ShapeSet const *)Load_Alloc_Data("SIDE2.SHP", SidebarShapeAllocated[SBSHAPE_MIDDLE]);
	SidebarShapes[SBSHAPE_BOTTOM] = (ShapeSet const *)Load_Alloc_Data("SIDE3.SHP", SidebarShapeAllocated[SBSHAPE_BOTTOM]);
	SidebarShapes[SBSHAPE_ADDON] = (ShapeSet const *)Load_Alloc_Data("ADDON.SHP", SidebarShapeAllocated[SBSHAPE_ADDON]);
}


void SidebarClass::DisposeOfArt()
{
	PowerClass::DisposeOfArt();
	if (SidebarShapeAllocated[SBSHAPE_TOP]) {
		operator delete((void *)SidebarShapes[SBSHAPE_TOP]);
		SidebarShapeAllocated[SBSHAPE_TOP] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_MIDDLE]) {
		operator delete((void *)SidebarShapes[SBSHAPE_MIDDLE]);
		SidebarShapeAllocated[SBSHAPE_MIDDLE] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_BOTTOM]) {
		operator delete((void *)SidebarShapes[SBSHAPE_BOTTOM]);
		SidebarShapeAllocated[SBSHAPE_BOTTOM] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_ADDON]) {
		operator delete((void *)SidebarShapes[SBSHAPE_ADDON]);
		SidebarShapeAllocated[SBSHAPE_ADDON] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_BACK]) {
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_BACK]);
		SidebarShapeAllocated[SBSHAPE_SHUTTER_BACK] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_MIDDLE]) {
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_MIDDLE]);
		SidebarShapeAllocated[SBSHAPE_SHUTTER_MIDDLE] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_SHUTTER_FRONT]) {
		operator delete((void *)SidebarShapes[SBSHAPE_SHUTTER_FRONT]);
		SidebarShapeAllocated[SBSHAPE_SHUTTER_FRONT] = false;
	}
	if (SidebarShapeAllocated[SBSHAPE_CLOCK]) {
		operator delete((void *)SidebarShapes[SBSHAPE_CLOCK]);
		SidebarShapeAllocated[SBSHAPE_CLOCK] = false;
	}
	Repair.Clear_Shape();
	Upgrade.Clear_Shape();
	for (int index = 0; index < 4; index++) {
		TabButton[index].Clear_Shape();
	}
	ScrollDownButton.Clear_Shape();
	ScrollUpButton.Clear_Shape();
}


void SidebarClass::Request_Redraw()
{
	IsToRedraw = true;
	Column[ActiveTabIndex].IsToRedraw = true;
	Map.Flag_To_Redraw();
	if (RedrawUntilFrame <= Frame) {
		RedrawUntilFrame = Frame + 2;
	}
}


void SidebarClass::Notice_Object_Change(ObjectClass * object)
{
	if (!IsSidebarActive || !object) {
		return;
	}
	TechnoTypeClass const * type = object->Techno_Type_Class();
	if (!PlayerPtr) {
		return;
	}
	RTTIType kind = object->Fetch_RTTI();
	int id = object->Fetch_Heap_ID();
	if (Which_Tab(kind, id, false) == ActiveTabIndex) {
		if (PlayerPtr->Is_Build_Limited(type)) {
			Request_Redraw();
		}
	} else if (kind == RTTI_BUILDING && ((BuildingClass *)object)->Class->IsHelipad) {
		Request_Column_Redraw(3);
	}
}


void SidebarClass::Flag_Tab_To_Redraw(int column)
{
	if (column == ActiveTabIndex) {
		IsToRedraw = true;
		Column[column].IsToRedraw = true;
		Map.Flag_To_Redraw();
	}
}


void SidebarClass::Request_Column_Redraw(int column)
{
	if (column == ActiveTabIndex) {
		IsToRedraw = true;
		Column[ActiveTabIndex].IsToRedraw = true;
		Map.Flag_To_Redraw();
		if (RedrawUntilFrame <= Frame) {
			RedrawUntilFrame = Frame + 2;
		}
	}
}


bool SidebarClass::Factory_Link(FactoryClass * factory, RTTIType type, int id)
{
	return Column[Which_Tab(type, id, false)].Factory_Link(factory, type, id);
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
		int first = First_Used_Column();
		int column = Which_Tab(type, id, false);
		if (Column[column].Add(type, id)) {
			Enable_Column_Button(column);
			Activate(1);
			if (first == -1) {
				Select_Tab(First_Used_Column());
			}
			IsToRedraw = true;
			Flag_To_Redraw();
			if (column == ActiveTabIndex) {
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
void SidebarClass::Update_Build_Buttons()
{
	if (Column[ActiveTabIndex].BuildableCount > Get_Doubled_Row_Count()) {
		if (!ScrollDownButton.Is_Enabled()) {
			ScrollDownButton.Enable();
		}
		if (!ScrollUpButton.Is_Enabled()) {
			ScrollUpButton.Enable();
		}
	} else {
		if (ScrollDownButton.Is_Enabled()) {
			ScrollDownButton.Disable();
		}
		if (ScrollUpButton.Is_Enabled()) {
			ScrollUpButton.Disable();
		}
	}
}




void SidebarClass::Enable_Column_Button(int index)
{
	if (First_Used_Column() == -1 && SidebarShapes[SBSHAPE_SHUTTER_BACK]) {
		ShutterStep = 1;
		ShutterFrame = 0;
	}
	if (!TabButton[index].Is_Enabled()) {
		TabButton[index].Enable();
	}
	if (index == ActiveTabIndex) {
		Update_Build_Buttons();
	}
}


void SidebarClass::Update_Column_Availability(int index)
{
	if (Column[index].BuildableCount > 0) {
		Update_Build_Buttons();
	} else {
		TabButton[index].Disable();
		int first = -1;
		for (int i = 0; i < 4; i++) {
			if (Column[i].BuildableCount > 0) {
				first = i;
				break;
			}
		}
		if (first == -1) {
			Update_Build_Buttons();
			if (SidebarShapes[SBSHAPE_SHUTTER_BACK]) {
				ShutterStep = -1;
				ShutterFrame = (SidebarShapes[SBSHAPE_SHUTTER_BACK])->Get_Count();
			}
		} else {
			Select_Tab(first);
		}
	}
}


bool SidebarClass::Scroll(bool up, int column)
{
	if (_dialog_count) {
		return(false);
	}

	if (column == -1) {
		column = ActiveTabIndex;
	}
	if (Column[column].Step(up)) {
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
		column = ActiveTabIndex;
	}
	StripClass & strip = Column[column];
	int amount = Max_Visible();
	if (strip.Scroll(up, amount)) {
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
	DirtyRect = RECT_NONE;
	Surface * old = LogicalSurface;
	LogicalSurface = SidebarSurface;
	bool drawn = false;
	Rect window(0, 0, SidebarSurface->Get_Width(), SidebarSurface->Get_Height());
	complete = complete || IsToRedraw || IsForceCompleteRedraw;

	if (IsSidebarActive && complete && !Debug_Map) {
		if (IsForceCompleteRedraw) {
			drawn = true;
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_TOP], 0,
				Point2D(0, SidebarRect.Y), window, SHAPE_WIN_REL);
			int y = StripTop;
			int row = 0;
			while (true) {
				if (row >= Max_Visible_Rows()) {
					break;
				}
				Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_MIDDLE], 0,
					Point2D(0, y), window, SHAPE_WIN_REL);
				y += (SidebarShapes[SBSHAPE_MIDDLE])->Get_Height();
				row++;
			}
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_BOTTOM], 0,
				Point2D(0, y), window, SHAPE_WIN_REL);
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_ADDON], 0,
				Point2D(0, y + (SidebarShapes[SBSHAPE_BOTTOM])->Get_Height()), window, SHAPE_WIN_REL);
		}
		Repair.Draw_Me(drawn);
		Upgrade.Draw_Me(drawn);
		for (int index = 0; index < 4; index++) {
			TabButton[index].Draw_Me(true);
		}
		ScrollDownButton.Draw_Me(drawn);
		ScrollUpButton.Draw_Me(drawn);
		if (ShutterStep) {
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_BACK], ShutterFrame,
				Point2D(0, StripTop), window, SHAPE_WIN_REL);
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_MIDDLE], ShutterFrame,
				Point2D(0, StripTop), window, SHAPE_WIN_REL);
			Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_SHUTTER_FRONT], ShutterFrame,
				Point2D(0, StripTop), window, SHAPE_WIN_REL);
		}
		IsToBlitSidebar = true;
	}
	Column[ActiveTabIndex].Draw_It(IsForceCompleteRedraw);
	PowerClass::Draw_It(complete || drawn);
	if (Repair.IsDrawn) {
		Repair.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	if (Upgrade.IsDrawn) {
		Upgrade.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	for (int tab = 0; tab < 4; tab++) {
		if (TabButton[tab].IsDrawn) {
			IsToBlitSidebar = true;
			TabButton[tab].IsDrawn = false;
		}
	}
	if (ScrollDownButton.IsDrawn) {
		ScrollDownButton.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	if (ScrollUpButton.IsDrawn) {
		ScrollUpButton.IsDrawn = false;
		IsToBlitSidebar = true;
	}
	if (ToolTips) {
		ToolTips->Draw_Current(true);
	}
	Blit_Sidebar(IsToBlitSidebar || IsForceCompleteRedraw);
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
			if (DirtyRect == RECT_NONE) {
				if (IsToRedrawCredits) {
					MouseCursor->Draw_Mouse(SidebarSurface, true);
					VisibleSurface->Blit_From(
						Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, SidebarRect.Width, 16),
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
		if (DirtyRect == RECT_NONE && !complete) {
			if (IsToRedrawCredits) {
				VisibleSurface->Blit_From(
					Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, SidebarRect.Width, 16),
					*SidebarSurface, Rect(0, 0, SidebarRect.Width, 16), false, true);
				IsToRedrawCredits = false;
			}
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y + SidebarRect.Y,
					SidebarRect.Width, SidebarSurface->Get_Height() - SidebarRect.Y),
				*SidebarSurface, Rect(0, SidebarRect.Y, SidebarRect.Width, SidebarRect.Height), false, true);
		} else if (IsToBlitSidebar) {
			Rect sb_rect = SidebarSurface->Get_Rect();
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + (Options.IsSidebarOnRight ? TacticalRect.Width : 0), scrnpt.y, sb_rect.Width, sb_rect.Height),
				*SidebarSurface, Rect(0, 0, sb_rect.Width, sb_rect.Height), false, true);
		} else {
			VisibleSurface->Blit_From(
				Rect(scrnpt.x + DirtyRect.X + (Options.IsSidebarOnRight ? TacticalRect.Width : 0),
					scrnpt.y + DirtyRect.Y, DirtyRect.Width, DirtyRect.Height),
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
void SidebarClass::Activate_Tab(int index)
{
	ActiveTabIndex = index;
	TabButton[index].Turn_On();
	Column[ActiveTabIndex].Activate();
	Update_Build_Buttons();
}


void SidebarClass::Select_Tab(int index)
{
	if (index == ActiveTabIndex) {
		return;
	}
	ShapeButtonClass & old_button = TabButton[ActiveTabIndex];
	old_button.Turn_Off();
	StripClass & old = Column[ActiveTabIndex];
	old.Deactivate();
	ActiveTabIndex = index;
	ShapeButtonClass & button = TabButton[ActiveTabIndex];
	button.Turn_On();
	StripClass & current = Column[ActiveTabIndex];
	current.Activate();
	Update_Build_Buttons();
	IsForceCompleteRedraw = true;
}


void SidebarClass::Start_Tab_Flash(int index)
{
	int delay = 10 - Frame % 10;
	TabButton[index].Start_Flashing(10, delay, (Frame + delay) / 10 % 2 == 0);
	Map.IsForceCompleteRedraw = true;
	Map.Flag_To_Redraw();
}





bool SidebarClass::Stop_Tab_Flash(int index)
{
	return TabButton[index].Stop_Flashing();
}


void SidebarClass::AI(KeyNumType & input, Point2D const & xy)
{
	Point2D const sidebar_origin(SidebarRect.X, SidebarRect.Y); // RA2: ra2-decomp's form (4fa48b2): the mouse position is taken relative to the sidebar's origin
	Point2D point = xy - Point2D(sidebar_origin.X, 0);
	if (!Debug_Map) {
		Activate(1);	// Force the sidebar always on in Win95 mode
		for (int index = 0; index < 4; index++) {
			Column[index].AI(input, point);
		}
	}

	if (IsSidebarActive) {
		if (ShutterStep == 1) {
			ShutterFrame++;
			if (ShutterFrame > (SidebarShapes[SBSHAPE_SHUTTER_BACK])->Get_Count()) {
				ShutterStep = 0;
				ShutterFrame = 0;
			}
		} else if (ShutterStep == -1) {
			ShutterFrame--;
			if (ShutterFrame < 0) {
				ShutterStep = 0;
				ShutterFrame = 0;
			}
		}

		Activate_Demolish(PlayerPtr->CurBuildings > 0);

		if (input == (102 | KN_BUTTON)) {
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, NULL);
			Sell_Mode_Control(-1);
		} else if (input == (101 | KN_BUTTON)) {
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000, NULL);
			Repair_Mode_Control(-1);
		} else if (input >= (203 | KN_BUTTON) && input < (207 | KN_BUTTON)) {
			Select_Tab(input - (203 | KN_BUTTON));
			VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, NULL);
		} else if ((input & ~KN_UNK) == (ScrollDownButton.ID | KN_BUTTON)) {
			ScrollDownButton.IsPressed = false;
			if (!Column[ActiveTabIndex].Page(false)) {
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, NULL);
			} else {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, NULL);
			}
		} else if ((input & ~KN_UNK) == (ScrollUpButton.ID | KN_BUTTON)) {
			ScrollUpButton.IsPressed = false;
			if (!Column[ActiveTabIndex].Page(true)) {
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000, NULL);
			} else {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000, NULL);
			}
		}

		bool flashed = false;
		for (int tab = 0; tab < 4; tab++) {
			if (TabButton[tab].AI()) {
				flashed = true;
			}
		}
		if (flashed) {
			IsToRedraw = true;
			IsToBlitSidebar = true;
			IsSidebarRedrawPending = true;
			Flag_To_Redraw();
		}
	}

	if (!IsSellMode && Repair.IsOn) {
		Repair.Turn_Off();
	}
	if (!IsRepairMode && Upgrade.IsOn) {
		Upgrade.Turn_Off();
	}
	PowerClass::AI(input, xy);
}




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
	for (int index = 0; index < STRIPS; index++) {
		if (Column[index].Recalc() && index == ActiveTabIndex) {
			Column[index].IsToRedraw = true;
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
#ifdef RA2_NON_MATCHING // not exact: 58 bytes differ by position, 6 instruction lines in 5 hunks (stack offsets blind), ours 1,067 B of 1,067, no unbound references (lane O107 prep, 2026-11-28: the draft compiles with this guard open); before: 58 bytes differ against the image-derived bound 0067e210 (1067 B): three instructions in the else arm's expanded Remove_Buttons (the side's extra subtracted before StripTop, StripTop in a scratch register). The order follows the number of locals the function holds, its expanded members' locals included: exact with seven to ten more referenced locals in the function now that the uncalled Activate_Tab is called here (its parameter is one unit; lane F72), and seven is the only count that leaves SelectClass::Action exact; or three to six more in Max_Visible (probes, never a landing form); ra2-decomp's written-out loops with `Max_Visible() * 2` move the window the other way (lane F48, 2026-11-23); main: best 99.04
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
	bool changed = IsSidebarActive != old;
	if (changed) {
		Set_Buttons(IsSidebarActive);
		if (IsSidebarActive) {
			Reposition_Sidebar();
			IsToRedraw = true;
			Repair.Zap();
			Add_A_Button(Repair);
			Upgrade.Zap();
			Add_A_Button(Upgrade);
			for (int tab = 0; tab < STRIPS; tab++) {
				TabButton[tab].Zap();
				TabButton[tab].IsPressed = false; // RA2: the exe clears the byte at +0x2C of the button (IsPressed), not IsOn at +0x2D
				Add_A_Button(TabButton[tab]);
			}
			ScrollDownButton.Zap();
			Map.Add_A_Button(ScrollDownButton);
			ScrollUpButton.Zap();
			Map.Add_A_Button(ScrollUpButton);
			Background.Zap();
			Add_A_Button(Background);
			RadarButton.Zap();
			Add_A_Button(RadarButton);
			for (int index = 0; index < STRIPS; index++) {
				Column[index].TopIndex = 0;
				if (Column[index].BuildableCount > 0) {
					TabButton[index].Enable();
				}
			}
			Activate_Tab(ActiveTabIndex); // RA2: Activate_Tab 0x67D7E0 has no caller in the image; its three statements stand here and the store of ActiveTabIndex to itself folds away
		} else {
			Stop_Ingame_Movie();
			Remove_A_Button(Repair);
			Remove_A_Button(Upgrade);
			Remove_A_Button(Background);
			for (int tab = 0; tab < STRIPS; tab++) {
				Remove_A_Button(TabButton[tab]);
			}
			Remove_A_Button(ScrollDownButton);
			Remove_A_Button(ScrollUpButton);
			Column[ActiveTabIndex].Remove_Buttons();
			Remove_A_Button(RadarButton);
		}

		/*
		**	Since the sidebar status has changed, update the map so that the graphics
		**	will be rendered correctly.
		*/
		Flag_To_Redraw(GS_REDRAW_ALL);
	}
	return changed;
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
int __fastcall SidebarClass::Cameos_Width(int value)
{
	return CameoWidth * value;
}


int __fastcall SidebarClass::Cameos_Height(int value)
{
	return CameoHeight * value + 1;
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
void SidebarClass::StripClass::Clear_Buildables()
{
	IsScrollingDown = false;
	IsScrolling = false;
	IsBuilding = false;
	Flasher = -1;
	TopIndex = 0;
	Slid = 0;
	BuildableCount = 0;
	for (int index = 0; index < 75; index++) {
		Buildables[index].BuildableID = 0;
		Buildables[index].BuildableType = RTTI_NONE;
		Buildables[index].IsAlt = 0;
		Buildables[index].Factory = 0;
		Buildables[index].Unknown_10 = 0;
		Buildables[index].Progress.Set_Rate(0);
		Buildables[index].Progress.Set_Stage(0);
		Buildables[index].FlashEndFrame = 0;
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
void SidebarClass::StripClass::Init_Buttons(int id)
{
	ID = id;
	int rows = Map.Max_Visible();
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < 2; column++) {
			int index = row * 2 + column;
			SelectClass & button = SelectButton[ID * 60 + index];
			button.ID = 202;
			button.X = Map.Column[ID].X + CameoWidth * column;
			button.Y = Map.Column[ID].Y + CameoHeight * row + 1;
			button.Width = 60;
			button.Height = 48;
			button.Strip = this;
			button.Index = index;
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
void SidebarClass::StripClass::Add_Buttons()
{

	int max = Map.Get_Doubled_Row_Count();
	for (int index = 0; index < max; index++) {
		SelectButton[ID * 60 + index].Zap();
		Map.Add_A_Button(SelectButton[ID * 60 + index]);
	}
	IsSidebarRedrawPending = true;
}


void SidebarClass::StripClass::Remove_Buttons()
{
	int max = Map.Get_Doubled_Row_Count();
	for (int index = 0; index < max; index++) {
		Map.Remove_A_Button(SelectButton[ID * 60 + index]);
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
bool SidebarClass::StripClass::Is_Before(RTTIType type, int id, RTTIType other_type, int other_id)
{
	TechnoTypeClass * techno = (TechnoTypeClass *)Fetch_Techno_Type(type, id);
	TechnoTypeClass * other = (TechnoTypeClass *)Fetch_Techno_Type(other_type, other_id);
	wchar_t const * name;
	wchar_t const * other_name;
	if (other_type == RTTI_NONE) {
		return(true);
	}

	bool super = type == RTTI_SPECIAL || type == RTTI_SUPERWEAPON || type == RTTI_SUPERWEAPONTYPE;
	bool other_super = other_type == RTTI_SPECIAL || other_type == RTTI_SUPERWEAPON || other_type == RTTI_SUPERWEAPONTYPE;
	if (super) {
		if (other_super) {
			SuperWeaponTypeClass * weapon = SuperWeaponTypes[id];
			SuperWeaponTypeClass * other_weapon = SuperWeaponTypes[other_id];
			if (weapon->RechargeTime < other_weapon->RechargeTime) {
				return(true);
			}
			if (weapon->RechargeTime > other_weapon->RechargeTime) {
				return(false);
			}
			name = weapon->UIName;
			other_name = other_weapon->UIName;
			return wcscmp(name, other_name) <= 0 ? true : false;
		}
	} else if (!other_super) {
		int side = PlayerPtr->Class->Side;
		bool own = side == techno->AIBasePlanningSide;
		bool other_own = side == other->AIBasePlanningSide;
		if (own && !other_own) {
			return(true);
		}
		if (!own && other_own) {
			return(false);
		}
	}

	bool unit = type == RTTI_UNITTYPE || type == RTTI_AIRCRAFTTYPE;
	bool aircraft = unit && techno->ConsideredAircraft;
	bool naval = unit && techno->Naval;
	bool ground = !aircraft && !naval;
	bool other_unit = other_type == RTTI_UNITTYPE || other_type == RTTI_AIRCRAFTTYPE;
	bool other_aircraft = other_unit && other->ConsideredAircraft;
	bool other_naval = other_unit && other->Naval;
	bool other_ground = !other_aircraft && !other_naval;

	if (super) {
		if (other_aircraft || other_naval || other_ground) {
			return(true);
		}
	} else if (ground) {
		if (other_super) {
			return(false);
		}
		if (other_aircraft || other_naval) {
			return(true);
		}
	} else if (aircraft) {
		if (other_super) {
			return(false);
		}
		if (other_ground) {
			return(false);
		}
		if (other_naval) {
			return(true);
		}
	} else if (naval) {
		if (other_super) {
			return(false);
		}
		if (other_ground || other_aircraft) {
			return(false);
		}
	}

	if (techno->Level < other->Level) {
		return(true);
	}
	if (techno->Level > other->Level) {
		return(false);
	}
	int cost = techno->Cost_Of(PlayerPtr);
	int other_cost = other->Cost_Of(PlayerPtr);
	if (cost < other_cost) {
		return(true);
	}
	if (cost > other_cost) {
		return(false);
	}
	name = techno->UIName;
	other_name = other->UIName;
	return wcscmp(name, other_name) <= 0 ? true : false;
}


void SidebarClass::StripClass::Insert_Buildable(RTTIType type, int id)
{
	BuildableCount++;
	for (int index = 0; index < BuildableCount; index++) {
		RTTIType other_type = Buildables[index].BuildableType;
		int other_id = Buildables[index].BuildableID;
		if (Is_Before(type, id, other_type, other_id)) {
			for (int move = BuildableCount - 1; move > index; move--) {
				Buildables[move] = Buildables[move - 1];
			}
			Buildables[index].BuildableType = type;
			Buildables[index].BuildableID = id;
			if (type == RTTI_BUILDINGTYPE) {
				Buildables[index].IsAlt = HouseClass::Build_Category(type, id);
			}
			Buildables[index].Factory = 0;
			Buildables[index].Unknown_10 = 0;
			Buildables[index].Progress.Set_Stage(0);
			Buildables[index].Progress.Set_Rate(0);
			Buildables[index].FlashEndFrame = 0;
			return;
		}
	}
}


bool SidebarClass::StripClass::Add(RTTIType type, int id)
{
	if (BuildableCount <= 75) {
		for (int index = 0; index < BuildableCount; index++) {
			if (Buildables[index].BuildableType == type && Buildables[index].BuildableID == id) {
				return(false);
			}
		}
		if (!ScenarioInit && type != RTTI_SPECIAL) {
			Queue_Speech("EVA_NewConstructionOptions", -1, -1);
		}
		Insert_Buildable(type, id);
		IsToRedraw = true;
		IsSidebarRedrawPending = true;
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
bool SidebarClass::StripClass::Scroll(bool up, int amount)
{
	if (up) { // RA2: 0x67EE60
		if (!TopIndex) {
			return false;
		}
		Scroller -= amount;
		return true;
	}
	if ((TopIndex + Map.Max_Visible()) * 2 >= BuildableCount) {
		return false;
	}
	Scroller += amount;
	return true;
}


/// <summary>
/// Causes the side strip to scroll by a whole page.
/// Use this routine to flag the side strip to move a full screenful of cameos rather than
/// the single one that Scroll would move. As with scrolling, this routine only starts the
/// motion; the AI and Draw_It routines carry it out.
/// </summary>
/// <param name="up">Should the side strip page UP? If it is to page down then pass false.</param>
/// <returns>bool; Was the side strip started to page in the desired direction?</returns>
bool SidebarClass::StripClass::Step(bool up)
{
	return Scroll(up, 1); // RA2: 0x67EEF0
}


bool SidebarClass::StripClass::Page(bool up)
{
	int amount = Map.Max_Visible(); // RA2: 0x67EF70
	return Scroll(up, amount);
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
	bool redraw = false;

	/*
	**	Reflect the scroll desired direction/value into the scroll
	**	logic handler. This might result in up or down scrolling.
	*/
	if (!IsScrolling && Scroller) {
		int columns = COLUMNS; // RA2: two cameos to a row
		int rows = Map.Max_Visible(); // RA2: the rows are computed once and used twice
		int count = BuildableCount;
		int visible = rows * 2; // RA2: two cameos to a row
		if (count <= visible) {
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
				if ((TopIndex + rows) * columns >= count) {
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
			Slid += SidebarClass::SidebarEdgeStep;
			if (Slid >= SidebarClass::CameoHeight) {
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

	bool flash_active = Map.RedrawUntilFrame > Frame; // RA2: the sidebar was asked to redraw for a few more frames
	if (flash_active) {
		redraw = true;
	}

	/*
	**	The tab flashes while something on its strip can be placed.
	*/
	bool building_tab = ID == 0 || ID == 1; // RA2: the structure and defense tabs
	if (building_tab) {
		bool ready = false;
		for (int index = 0; index < BuildableCount; index++) {
			if (Buildables[index].BuildableType == RTTI_SPECIAL) {
				SuperClass * super = PlayerPtr->SuperWeapon[Buildables[index].BuildableID];
				if (super != NULL && super->Can_Place()) {
					ready = true;
					break;
				}
			} else {
				FactoryClass * factory = Buildables[index].Factory;
				if (factory != NULL && factory->Has_Completed()) {
					TechnoClass * done = factory->Get_Object();
					if (done != NULL && done->Fetch_RTTI() == RTTI_BUILDING) {
						ready = true;
						break;
					}
				}
			}
		}
		if (ready) {
			int frame = Frame; // RA2: the frame is read once; the exe adds the delay to it as (delay + frame), unlike the out-of-line Start_Tab_Flash 0x67DC00
			int delay = 10 - frame % 10; // RA2: the tab flashes in step with the next multiple of ten frames, as Start_Tab_Flash does
			Map.TabButton[ID].Start_Flashing(10, delay, (delay + frame) / 10 % 2 == 0);
			Map.IsForceCompleteRedraw = true;
			Map.Flag_To_Redraw();
		} else {
			Map.Stop_Tab_Flash(ID);
		}
	}

	/*
	**	Handle any building clock animation logic.
	*/
	if (IsBuilding) {
		for (int index = 0; index < BuildableCount; index++) {
			FactoryClass * factory = Buildables[index].Factory;

			bool changed = factory && factory->Has_Changed();
			if (changed) {
				redraw = true;
				bool completed = factory->Has_Completed();
				if (completed) {

					/*
					**	Construction has been completed. Announce this fact to the player and
					**	try to get the object to automatically leave the factory. Buildings are
					**	the main exception to the ability to leave the factory under their own
					**	power.
					*/
					TechnoClass * pending = factory->Get_Object();
					if (pending != NULL) {
						switch (pending->Fetch_RTTI()) {
							case RTTI_BUILDING: {
								Queue_Speech("EVA_ConstructionComplete", -1, -1); // RA2: RA2's EVA speaks through Queue_Speech
								BuildingClass * building = (pending->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)pending : NULL; // RA2: the cast the exe makes before the call
								Set_Tab_Building(building); // RA2: 0x6F8A00 makes the building the one the structure or defense tab shows
								break;
							}

							case RTTI_UNIT: // RA2: the jump table of the exe sends these three kinds to one body
							case RTTI_AIRCRAFT:
							case RTTI_INFANTRY:
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
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].Unknown_10 == 1) {
			FactoryClass * factory = Buildables[index].Factory;
			if (factory != NULL && (!factory->Is_Building() || factory->Is_On_Hold())) {
				Buildables[index].Progress.Set_Rate(0);
				Buildables[index].Progress.Set_Stage(0);
			}
			bool advanced = Buildables[index].Progress.Graphic_Logic();
			if (advanced) {
				redraw = true;
				if (Buildables[index].Progress.Fetch_Stage() >= 53) { // RA2: the clock has 54 frames; at the last one it stops and keeps the stage
					Buildables[index].Progress.Set_Rate(0);
				}
			}
		}
	}

	/*
	**	If any of the logic determined that this side strip needs to be redrawn, then
	**	set the redraw flag for this side strip.
	*/
	if (redraw) {
		Map.Flag_Tab_To_Redraw(Map.ActiveTabIndex);
		Flag_To_Redraw();
		SidebarClass::IsSidebarRedrawPending = true;
		SidebarClass::IsToBlitSidebar = true;
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
wchar_t const * SidebarClass::StripClass::Help_Text(int gadget_index)
{
	int index = gadget_index + TopIndex * 2;
	if (!GameActive || index >= BuildableCount || BuildableCount >= 75) {
		return(0);
	}
	if (Buildables[index].BuildableType == RTTI_SPECIAL) {
		return SuperWeaponTypes[Buildables[index].BuildableID]->UIName;
	}
	TechnoTypeClass const * type = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
	if (IsCameoTextShown) {
		swprintf(CameoHelpBuffer,
			StringTable::LoadString("TXT_MONEY_FORMAT_1", 0, "D:\\Ra2\\Sidebar.CPP", 2627),
			type->Cost_Of(PlayerPtr));
	} else {
		swprintf(CameoHelpBuffer,
			StringTable::LoadString("TXT_MONEY_FORMAT_2", 0, "D:\\Ra2\\Sidebar.CPP", 2629),
			type->UIName, type->Cost_Of(PlayerPtr));
	}
	int length = wcslen(CameoHelpBuffer);
	for (int i = 0; i < length; i++) {
		if (CameoHelpBuffer[i] == ' ') {
			CameoHelpBuffer[i] = '\n';
		}
	}
	return CameoHelpBuffer;
}


void SidebarClass::StripClass::Activate()
{
	AllowedToDraw = true;
	Add_Buttons();
}


void SidebarClass::StripClass::Deactivate()
{
	AllowedToDraw = false;
	Remove_Buttons();
}


int SidebarClass::StripClass::Get_Progress(int index)
{
	int progress = 0;
	if (Buildables[index].Factory) {
		progress = Buildables[index].Factory->Completion();
	}
	if (Buildables[index].Unknown_10 == 1) {
		int stage = Buildables[index].Progress.Fetch_Stage();
		if (stage > progress) {
			progress = (progress + stage) / 2;
		}
	}
	return progress;
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
#ifdef RA2_NON_MATCHING // not exact: 6 bytes differ by position, 9 instruction lines in 3 hunks (stack offsets blind), ours 2,760 B of 2,760, no unbound references (lane O107 prep, 2026-11-28: the draft compiles with this guard open); before: 6 bytes differ (outline pixel channel shifts: the image packs Blue, Red, Green from the by-value copy, ours Blue, Green, Red; the text colour below is the image's. ra2-decomp's matched twin (2e5b2c1) measured, lane F62, 2026-11-25: its named print styles change nothing here (in Yuri's Revenge they meet a count for the observer rows, which RA2 lacks), its colour copy with the three-value call turns the text colour instead (48); neither pixel follows the local count, and distinct live globals turn them only to orders the image does not have) (lane F100, 2026-11-27: the outline's order is a count of this function's own referenced locals, but only while the pixel is written as Draw_Rect's argument beside the Rect temporary; with a named pixel or a named Rect it is the draft's order at every count. With the pixel as the argument, ra2-decomp's four named print styles (two units each) and two more locals (any two simple conditions held in bools) the function is exact, and Reposition_Sidebar 0x6819A0 then turns by 6 B (its six named ints are a window of the unit's count, and this function's locals move it; exact again only with ten more of its own). With Activate's draft compiled ahead all rows of the unit are exact with that form, so the three parked functions and the two count rows are one set: not landed. Patch: private/dossiers/fable/f100_tools/sidebar_draw_it_candidate.patch)
void SidebarClass::StripClass::Draw_It(bool complete)
{
	if (AllowedToDraw && (IsToRedraw || complete)) { // RA2: only an active strip is drawn
		IsToRedraw = false;
		IsToBlitSidebar = true;
		Rect cliprect; // RA2: the whole sidebar surface
		cliprect.X = 0;
		cliprect.Y = 0;
		cliprect.Width = SidebarSurface->Get_Width();
		cliprect.Height = SidebarSurface->Get_Height();

		/*
		**	RA2: while a building is being made or is on hold, no other building can be started.
		*/
		bool busy = false;
		for (int i = 0; i < BuildableCount; i++) {
			if (Buildables[i].Unknown_10 == 1 || Buildables[i].Unknown_10 == 2) {
				busy = true;
				break;
			}
		}

		/*
		**	Loop through all the buildable objects that are visible in the strip and render
		**	them. Their Y offset may be adjusted if the strip is in the process of scrolling.
		*/
		int columns = COLUMNS; // RA2: two cameos to a row
		int rows = Map.Max_Visible() + (IsScrolling ? 1 : 0); // RA2: the rows are counted once, and two cameos make a row
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < columns; col++) {
				bool draw = complete; // RA2: only changed cameos are drawn unless the strip is redrawn in full
				int stage = 0;
				ShapeSet const * shapefile = NULL;
				FactoryClass * factory = NULL;
				wchar_t const * state = NULL; // RA2: the readout is wide text
				int index = col + (TopIndex + row) * columns;
				bool production = false;
				bool completed = false;
				bool darken = false;
				bool hover = false; // RA2: the mouse is over this cameo
				int x = CameoWidth * col - SidebarRect.X + X; // RA2: the cameo sizes are the sidebar's
				int y = CameoHeight * row + 1 + Y;
				SelectClass * button = &SelectButton[row * columns + ID * MAX_SLOTS_PER_STRIP + col];

				/*
				**	If the strip is scrolling, then the offset is adjusted accordingly.
				*/
				if (IsScrolling) {
					y += Slid - CameoHeight;
				}

				wchar_t const * name = NULL; // RA2: wide names
				TechnoTypeClass const * obj = NULL;

				/*
				**	Fetch the shape number for the object type located at this current working
				**	slot. This shape pointer is used to draw the underlying graphic there.
				*/
				if (index < BuildableCount) {
					hover = button->Is_Hovered(); // RA2: the mouse is over the button and input is on (Is_Hovered 0x681690: nothing in the image calls it, so it is expanded here; same code)

					if (Buildables[index].BuildableType != RTTI_SPECIAL) {

						obj = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
						if (obj != NULL) {

							name = obj->UIName; // RA2: the wide UI name

							/*
							**	If there is already a factory producing this kind of object, then all
							**	objects of this type are displays in a disabled state.
							*/
							bool isbusy = false;
							if (obj->RTTI == RTTI_BUILDINGTYPE && busy) { // RA2: the strip's own busy flag
								isbusy = true;
							} else if (obj->Who_Can_Build_Me(true, true, true, PlayerPtr) == NULL) {
								isbusy = true;
							} else {
								TechnoTypeClass const * type = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
								bool none = PlayerPtr->Can_Build(type, false, false) == -1;
								bool limited = PlayerPtr->Is_Build_Limited(type);
								if (none || limited) {
									isbusy = true;
								}
							}

							shapefile = (ShapeSet const *)obj->Get_Cameo_Data();
							factory = Buildables[index].Factory;
							if (factory != NULL) {
								production = true;
								completed = factory->Has_Completed();
								if (completed) {
									state = StringTable::LoadString("TXT_READY", NULL, "D:\\Ra2\\Sidebar.CPP", 2845); // RA2: wide text
								} else {
									draw = true; // RA2: a cameo that is being built is redrawn every time
								}
								stage = Get_Progress(index);
								darken = false;
							} else {
								production = false;

								/*
								**	Darken the imagery if a factory of a matching type is
								**	already busy.
								*/
								darken = isbusy;

								/*
								**	RA2: an entry without a factory is still shown as in progress while it is
								**	queued (1), on hold (2) or waits to be placed (3).
								*/
								switch (Buildables[index].Unknown_10) {
									case 1:
										draw = true;
										production = true;
										stage = Get_Progress(index);
										darken = false;
										break;

									case 3:
										production = true;
										completed = true;
										darken = false;
										break;

									case 2:
										production = true;
										completed = false;
										darken = true;
										stage = Get_Progress(index);
										break;

									case 0:
										if (!complete) { // RA2: nothing changed, so the cameo is left as it is
											continue;
										}
										break;
								}
							}
						} else {
							darken = false;
						}

					} else {

						int id = Buildables[index].BuildableID;
						name = SuperWeaponTypes[id]->UIName;
						shapefile = Get_Special_Cameo(SuperWeaponType(id));

						production = true;
						completed = PlayerPtr->SuperWeapon[id]->Is_Charging() == false;
						state = PlayerPtr->SuperWeapon[id]->State_String();
						stage = PlayerPtr->SuperWeapon[id]->Anim_Stage();
						darken = false;
						draw = true; // RA2: a superweapon cameo is redrawn every time
					}

				} else {
					production = false;
				}

				/*
				**	Now that the shape of the object at the current working slot has been found,
				**	draw it and any graphic overlays as necessary.
				*/
				Rect window(0, 0, x + 60, y + 48); // RA2: the clipping window follows the cameo
				if (draw) {
					if (shapefile != NULL) {
						Draw_Shape(*SidebarSurface, *CameoDrawer, shapefile, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL));
					}

					/*
					**	RA2: the mouse is over this cameo, so it gets an outline in the tooltip colour.
					*/
					RGBClass const & color = TooltipColor; // RA2: shared by the outline and the text below.
					if (hover) {
						int outline = DSurface::Build_Hicolor_Pixel(RGBClass(color));
						SidebarSurface->Draw_Rect(window, Rect(x, y, 60, 48), outline);
					}

					/*
					**	Darken this object because it cannot be produced or is otherwise
					**	unavailable.
					*/
					if (darken) {
						Draw_Shape(*SidebarSurface, *SidebarDrawer, DarkenShapes, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL | SHAPE_DARKEN));
					}

					/*
					**	RA2: a cameo flashes (darkened every other half second) until its flash frame is past.
					*/
					if (Buildables[index].FlashEndFrame > Frame && Frame % 16 > 8) {
						Draw_Shape(*SidebarSurface, *SidebarDrawer, DarkenShapes, 0, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL | SHAPE_TRANSLUCENT50));
					}

					if (name != NULL) {
						Print_Cameo_Text(name, Point2D(x, y + 36), cliprect, 60); // RA2: the caption sits 36 below the cameo's top and is 60 wide
					}

					bool hasqueuecount = false;
					unsigned pixel = DSurface::Build_Hicolor_Pixel(color); // RA2: the text colour is the tooltip colour
					if (obj != NULL) {
						FactoryClass * queue = PlayerPtr->Fetch_Factory(obj->Fetch_RTTI(), obj->Naval, 0); // RA2: the factory is picked by type and naval flag
						if (queue != NULL) {
							int total = queue->Total(obj);
							if (total > 1 || total > 0 && !queue->Is_Currently_Producing(obj)) {
								wchar_t buffer[256]; // RA2: the queued count, with a dimmed box behind it
								swprintf(buffer, L"%d", total);
								Point2D point(x + QUEUE_COUNT_X_OFFSET, y + 1);
								Rect rect = Text_Rect(buffer, point, 0x242, 2, 1);
								Blend_Rect(&rect, SidebarSurface, 0, 175);
								Fancy_Text_Print(L"%d", *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x242), total);
								hasqueuecount = true;
							}
						}
					}

					/*
					**	Draw the overlapping clock shape if this is object is being constructed.
					**	If the object is completed, then display "Ready" with no clock shape.
					*/
					if (production) {

						/*
						**	Display text showing that the object is ready to place.
						*/
						if (state != NULL) {
							Point2D point(x + TEXT_X_OFFSET, y + 1); // RA2: the readout sits on a dimmed box
							Rect rect = Text_Rect(state, point, 0x142, 2, 1);
							Blend_Rect(&rect, SidebarSurface, 0, 175);
							Fancy_Text_Print(state, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x142));
						}

						if (!completed) {

							Draw_Shape(*SidebarSurface, *SidebarDrawer, SidebarShapes[SBSHAPE_CLOCK], stage + 1, Point2D(x, y), window, ShapeFlags_Type(SHAPE_WIN_REL|SHAPE_TRANSLUCENT50)); // RA2: one clock for all

							/*
							**	Display text showing that the construction is temporarily on hold.
							*/
							if (factory && !factory->Is_Building() || Buildables[index].Unknown_10 == 2) { // RA2: also an entry on hold (2)
								wchar_t const * hold = StringTable::LoadString("TXT_HOLD", NULL, "D:\\Ra2\\Sidebar.CPP", 3070); // RA2: wide text
								if (hasqueuecount) {
									Point2D point(x + 2, y + 1); // RA2: left aligned next to the count
									Rect rect = Text_Rect(hold, point, 0x42, 2, 1);
									Blend_Rect(&rect, SidebarSurface, 0, 175);
									Fancy_Text_Print(hold, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x42));
								} else {
									Point2D point(x + TEXT_X_OFFSET, y + 1);
									Rect rect = Text_Rect(hold, point, 0x142, 2, 1);
									Blend_Rect(&rect, SidebarSurface, 0, 175);
									Fancy_Text_Print(hold, *SidebarSurface, cliprect, point, pixel, 0, TextPrintType(0x142));
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
#ifdef RA2_NON_MATCHING // not exact: 1660 bytes differ by position, 301 instruction lines in 96 hunks (stack offsets blind), ours 1,472 B of 1,732, no unbound references (lane O107 prep, 2026-11-28: the draft compiles with this guard open); before: 1660 bytes differ by position against the image-derived bound 006802d0 (1732 B; ours 1472 B, the frame the image's): ra2-decomp's twin statements (the named type, id and last entry; BuildType's constructor clears IsAlt, sidebar.h). The image expands the plain member Update_Column_Availability here with its loop, first Update_Build_Buttons and the tab lookup, and calls the second Update_Build_Buttons and Select_Tab inside it; ours calls the member: it is over the plain-function limit by its shutter block (without that block, or declared inline, it expands and 10 to 16 instructions are left: the scratch registers of the old-position search); written with First_Used_Column() it expands but First_Used_Column is then called (327 bytes) and the member's own exact row is lost (lane F33, 2026-11-23)
bool SidebarClass::StripClass::Recalc(void)
{
	int ok;

	if (Debug_Map || !BuildableCount) {
		return(false);
	}

	bool scroll = false;
	int max_visible = Map.Max_Visible() * 2;
	SmartPtr<BuildType> unshifted(new BuildType[max_visible]); // RA2: ra2-decomp's form

	for (int i = 0; i < max_visible; i++) {
		if (i + TopIndex * 2 < BuildableCount) {
			unshifted[i] = Buildables[i + TopIndex * 2];
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
			ok = who != NULL && who->House->Can_Build(tech, false, true);
		} else {
			if ((unsigned)Buildables[index].BuildableID < (unsigned)PlayerPtr->SuperWeapon.Count()) {
				ok = PlayerPtr->SuperWeapon[Buildables[index].BuildableID]->Is_Present();
			} else {
				ok = false;
			}
		}

		if (!ok) {
			if (Buildables[index].Factory != NULL) {
				RTTIType object_type = Buildables[index].BuildableType;
				int object_id = Buildables[index].BuildableID;
				OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ABANDON, object_type, object_id, tech->Naval));
				if (Buildables[index].BuildableType == RTTI_BUILDINGTYPE || Buildables[index].BuildableType == RTTI_BUILDING) {
					Map.PendingObjectPtr = NULL;
					Map.PendingObject = NULL;
					Map.PendingHouse = HOUSE_NONE;
					Map.Set_Cursor_Shape(NULL);
				}
			}

			if (tech) {
				FactoryClass * factory = PlayerPtr->Fetch_Factory(tech->Fetch_RTTI(), tech->Naval, 0);
				if (factory) {
					for (int q = factory->QueuedObjects.Count() - 1; q >= 0; q--) {
						if (factory->QueuedObjects[q] == tech) {
							OutList.Add(EventClass(PlayerPtr->HeapID, EventClass::ABANDON, Buildables[index].BuildableType, Buildables[index].BuildableID, tech->Naval));
						}
					}
				}
			}

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
			int last = BuildableCount - 1;
			Buildables[last].Factory = NULL;
			Buildables[last].BuildableID = 0;
			Buildables[last].BuildableType = RTTI_NONE;
			Buildables[last].IsAlt = 0;
			Buildables[last].Unknown_10 = 0;
			Buildables[last].Progress.Set_Stage(0);
			Buildables[last].Progress.Set_Rate(0);
			Buildables[last].FlashEndFrame = 0;
			IsToRedraw = true;
			BuildableCount--;
			index--;
		}
	}

	Map.Update_Column_Availability(ID);
	if (scroll) {
		BuildType const * previous = unshifted;
		bool got_old = false;
		bool got_new = false;

		int oldpos;
		for (oldpos = 0; oldpos < max_visible; oldpos++) {
			if (previous[oldpos] != BuildType(0, RTTI_NONE)) {
				got_old = true;
				break;
			}
		}

		int newpos;
		if (got_old && BuildableCount != 0) {
			for (newpos = 0; newpos < BuildableCount; newpos++) {
				if (Buildables[newpos] == previous[oldpos]) {
					got_new = true;
					break;
				}
			}
		}

		if (got_old && got_new) {
			TopIndex -= (oldpos - newpos) / 2;
			TopIndex = MAX(0, MIN(TopIndex, (BuildableCount - max_visible) / 2));
		} else {
			TopIndex = 0;
		}
		Map.IsForceCompleteRedraw = true;
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


bool SidebarClass::StripClass::Has_Buildables(void) const
{
	return BuildableCount > 0;
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
	return(Column[Which_Tab(type, id, false)].Abandon_Production(factory));
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
	if (type == RTTI_INFANTRY || type == RTTI_INFANTRYTYPE) {
		return 2;
	}
	if (type == RTTI_UNIT || type == RTTI_UNITTYPE || type == RTTI_AIRCRAFT || type == RTTI_AIRCRAFTTYPE) {
		return 3;
	}
	if (type == RTTI_BUILDING || type == RTTI_BUILDINGTYPE) {
		return HouseClass::Build_Category(type, id) == 5;
	}
	if (type == RTTI_SUPERWEAPON || type == RTTI_SUPERWEAPONTYPE || type == RTTI_SPECIAL) {
		return 1;
	}
	return -1;
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
		Clear_Sidebar_Tab_Object(As_Building(factory->Get_Object()));
	}

	bool abandon = false;
	bool noprod = true;
	for (int index = 0; index < BuildableCount; index++) {
		if (Buildables[index].Factory == factory) {
			((FactoryClass *)factory)->Abandon();
			Buildables[index].Factory = NULL;
			abandon = true;
			Buildables[index].Unknown_10 = 0;
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
		IsSidebarRedrawPending = true;
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
	if (type == RTTI_INFANTRYTYPE) {
		return 2;
	}
	if (type == RTTI_UNITTYPE || type == RTTI_AIRCRAFTTYPE) {
		return 3;
	}
	if (type == RTTI_BUILDINGTYPE) {
		return category == 5;
	}
	return -1;
}



/// <summary>
/// Repositions the sidebar and everything that sits on it.
/// This routine is called whenever the sidebar changes which edge of the screen it lives
/// on, or the display it must fit into changes size. The sidebar rectangle is placed first
/// and then the mode buttons, scroll arrows, cameo buttons and their tooltip regions are
/// moved to suit.
/// </summary>
int SidebarClass::First_Used_Column()
{
	for (int index = 0; index < 4; index++) {
		if (Column[index].BuildableCount > 0) {
			return index;
		}
	}
	return -1;
}


void SidebarClass::Reposition_Sidebar()
{
	PowerClass::Reposition_Sidebar();

	/*
	 * Position the sidebar.
	 */
	if (Options.IsSidebarOnRight) {
		int right_edge = TacticalRect.X + TacticalRect.Width;
		SidebarRect.X = right_edge;
	} else {
		SidebarRect.X = 0;
	}
	StripLeft = SidebarRect.X + 0x16;
	if (Scen->IsGDI) {
		TopButtonX = SidebarRect.X + 0x14;
		TabButtonX = SidebarRect.X + 0x1A;
		PowerBarLeft = SidebarRect.X + 0x27;
	} else {
		TopButtonX = SidebarRect.X + 0x21;
		TabButtonX = SidebarRect.X + 0x14;
		PowerBarLeft = SidebarRect.X + 0x27;
	}

	/*
	 * Position the sidebar's buttons.
	 */
	if (SidebarShapes[SBSHAPE_TOP] == 0) {
		SidebarShapes[SBSHAPE_TOP] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI1.SHP");
		SidebarShapes[SBSHAPE_MIDDLE] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI2.SHP");
		SidebarShapes[SBSHAPE_BOTTOM] = (ShapeSet const *)MixFileClass::Retrieve("SIDEGDI3.SHP");
	}

	int background_x = SidebarRect.X + 0x10;
	Background.Set_Position(background_x, TacticalRect.Y);
	Background.Flag_To_Redraw();
	int x = 0;
	int y = RadarButton.Y + RadarButton.Height;
	if (Options.IsSidebarOnRight) {
		Background.Set_Position(TacticalRect.X + TacticalRect.Width, y);
	} else {
		Background.Set_Position(x, y);
	}
	Background.Set_Size(SidebarSurface->Get_Width(), SidebarSurface->Get_Height() - y);

	Upgrade.Set_Position(TopButtonX, TopButtonY);
	Upgrade.Flag_To_Redraw();
	Upgrade.DrawOffsetX = -SidebarRect.X;

	int power_x = Upgrade.X + TopButtonSpacing;
	Repair.Set_Position(power_x, Upgrade.Y);
	Repair.Flag_To_Redraw();
	Repair.DrawOffsetX = -SidebarRect.X;

	for (int tab = 0; tab < 4; tab++) {
		int tab_x = TabButtonX + TabButtonSpacing * tab;
		TabButton[tab].Set_Position(tab_x, TabButtonY);
		TabButton[tab].Flag_To_Redraw();
		TabButton[tab].DrawOffsetX = -SidebarRect.X;
	}

	ScrollDownButton.Set_Position(PowerBarLeft, SidebarBottomY);
	ScrollDownButton.Flag_To_Redraw();
	ScrollDownButton.DrawOffsetX = -SidebarRect.X;

	int waypoint_x = PowerBarLeft + SidebarEdgeWidth;
	ScrollUpButton.Set_Position(waypoint_x, SidebarBottomY);
	ScrollUpButton.Flag_To_Redraw();
	ScrollUpButton.DrawOffsetX = -SidebarRect.X;

	/*
	 * Create the tooltips for the sidebar.
	 */
	int rows = Max_Visible();
	int count = rows * 2;
	for (int column = 0; column < 4; column++) {
		int first = column * 60;
		StripClass::SelectClass * button = &StripClass::SelectButton[first];
		for (int row = 0; row < rows; row++) {
			for (int side = 0; side < 2; side++, button++) {
				int row_y = CameoHeight * row;
				int cameo_y = StripTop + row_y + 1;
				int cameo_x = StripLeft + CameoWidth * side;
				button->Set_Position(cameo_x, cameo_y);
				button->Flag_To_Redraw();
			}
		}
	}

	if (ToolTips != 0) {
		ToolTip tip;
		for (int tab = 0; tab < 4; tab++) {
			int tab_id = tab + 0xCB;
			tip.ID = tab_id;
			tip.Text = 0;
			ShapeButtonClass const & tab_button = TabButton[tab];
			tip.Region.Set(tab_button.X, tab_button.Y, tab_button.Width, tab_button.Height);
			ToolTips->Remove(tab_id);
			ToolTips->Add(&tip);
		}
		for (int index = 0; index < count; index++) {
			int cameo_id = index + 1000;
			tip.ID = cameo_id;
			tip.Text = 0;
			StripClass::SelectClass const & cameo_button = StripClass::SelectButton[index];
			tip.Region.Set(cameo_button.X, cameo_button.Y, cameo_button.Width, cameo_button.Height);
			tip.IsAnchored = true;
			ToolTips->Remove(cameo_id);
			ToolTips->Add(&tip);
		}
		tip.IsAnchored = false;
		tip.ID = 0x66;
		tip.Text = (int)"TXT_SELL_MODE"; // RA2: the label of the string table
		tip.Region.Set(Repair.X, Repair.Y, Repair.Width, Repair.Height);
		ToolTips->Remove(0x66);
		ToolTips->Add(&tip);
		tip.ID = 0x65;
		tip.Text = (int)"TXT_REPAIR_MODE";
		tip.Region.Set(Upgrade.X, Upgrade.Y, Upgrade.Width, Upgrade.Height);
		ToolTips->Remove(0x65);
		ToolTips->Add(&tip);
		tip.ID = 0xC8;
		tip.Text = 0;
		tip.Region.Set(ScrollUpButton.X, ScrollUpButton.Y, ScrollUpButton.Width, ScrollUpButton.Height);
		ToolTips->Remove(0xC8);
		ToolTips->Add(&tip);
		tip.ID = 0xC9;
		tip.Text = 0;
		tip.Region.Set(ScrollDownButton.X, ScrollDownButton.Y, ScrollDownButton.Width, ScrollDownButton.Height);
		ToolTips->Remove(0xC9);
		ToolTips->Add(&tip);
	}
}


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
int SidebarClass::Max_Visible(void)
{
	int extra;
	if (Scen->IsGDI) { // RA2: 0x6820A0
		extra = 26;
	} else {
		extra = 18;
	}
	return (SidebarRect.Y + SidebarRect.Height - StripTop - extra - 7) / 50;
}


int SidebarClass::Get_Doubled_Row_Count()
{
	int const rows = Max_Visible();
	int const count = rows + rows;
	return count;
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
void Print_Cameo_Text(wchar_t const * string, Point2D const & point, Rect const & cliprect, int maxlinelen)
{
	if (IsCameoTextShown && string != NULL) { // RA2: 0x682140
		Point2D drawpoint = point;
		BitFont * font = BitFont::Instance;
		ColorScheme * scheme = Fetch_Scheme_By_Name("DarkRed", 1);
		RGBClass rgb = scheme->HSV;
		unsigned color = ((rgb.Get_Red() >> DSurface::RedLeft) << DSurface::RedRight) | ((rgb.Get_Green() >> DSurface::GreenLeft) << DSurface::GreenRight) | ((rgb.Get_Blue() >> DSurface::BlueLeft) << DSurface::BlueRight);
		LTRBStruct box;
		box.Left = cliprect.X;
		box.Top = cliprect.Y;
		box.Right = cliprect.X + cliprect.Width;
		box.Bottom = cliprect.Y + cliprect.Height;
		font->Set_Clip(true);
		font->Set_Bounds(&box);
		font->Set_Color(color);
		int textwidth;
		int textheight;
		font->Text_Dimensions(string, &textwidth, &textheight, maxlinelen);
		drawpoint.Y += font->LineHeight - textheight;
		BitText::Instance->Draw_Text(font, SidebarSurface, string, drawpoint.X, drawpoint.Y, maxlinelen, 0, 0, 0, 0);
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
	return BASECLASS::Load(stream);
}


/// <summary>
/// Saves the sidebar to the save game stream.
/// The sidebar keeps nothing of its own that must survive a save, so the real work falls to
/// the map layers underneath it.
/// </summary>
/// <returns>Returns with S_OK if the save succeeded.</returns>
HRESULT SidebarClass::Save(IStream * stream)
{
	return BASECLASS::Save(stream);
}


void SidebarClass::StripClass::Set_Flash(int id, int delay)
{
	for (int index = 0; index < 75; index++) {
		if (Buildables[index].BuildableID == id) {
			Buildables[index].FlashEndFrame = Frame + delay;
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
	Column[0].ObjectRect = Rect(Column[0].X, Column[0].Y, StripClass::OBJECT_WIDTH, (StripClass::OBJECT_HEIGHT)*Max_Visible());

	Column[1].X = COLUMN_TWO_X;
	Column[1].Y = COLUMN_ONE_Y;
	Column[1].ObjectRect = Rect(Column[1].X, Column[1].Y, StripClass::OBJECT_WIDTH, (StripClass::OBJECT_HEIGHT)*Max_Visible());
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
