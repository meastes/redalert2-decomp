/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/TAB.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TAB.CPP                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : September 20, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TabClass::AI -- Handles player I/O with the tab buttons.                                  *
 *   TabClass::Draw_It -- Displays the tab buttons as necessary.                               *
 *   TabClass::One_Time -- Performs one time initialization of tab handler class.              *
 *   TabClass::Set_Active -- Activates a "filefolder tab" button.                              *
 *   TabClass::TabClass -- Default construct for the tab button class.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "tab.h"
#include <stdio.h> // RA2: sprintf
#include <string.h> // RA2: strcmp
#include "data.h" // RA2: Load_Alloc_Data
#include "cctooltip.h" // RA2: the tool tips of the buttons
#include "_tooltip.h" // RA2: the includes this file needs
#include "_rect.h" // RA2: the includes this file needs
#include "_xmouse.h" // RA2: the includes this file needs
#include "voc.h" // RA2: the GUI sounds
#include "keyboard.h" // RA2: the includes this file needs
#include "gadget.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs

#include "_convert.h"
#include "_map.h"
#include "_mixfile.h"
#include "_rules.h"
#include "_surface.h"
#include "dialog.h"
#include "draw.h"
#include "goptions.h"
#include "language\language.h"
#include "mixfile.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"
#include "scheme.h"
#include "shapeset.h"
#include "surface.h"


ShapeButtonClass TabClass::CommandButtons[TabClass::BUTTON_COUNT]; // RA2: 0xABD918
ShapeButtonClass TabClass::ThumbOpenButton; // RA2: 0xABE400
ShapeButtonClass TabClass::ThumbClosedButton; // RA2: 0xABE390
ShapeSet const * TabClass::ButtonShapes[TabClass::BUTTON_COUNT]; // RA2: 0xABD8A0
bool TabClass::ButtonShapeAllocated[TabClass::BUTTON_COUNT]; // RA2: 0xABE32C
int TabClass::SlotButton[TabClass::BUTTON_COUNT]; // RA2: 0xABE2C8
int TabClass::ButtonWidth; // RA2: 0xABE478
int TabClass::ButtonHeight; // RA2: 0xABE2A8
int TabClass::ButtonRows; // RA2: 0xABE2AC
int TabClass::ButtonsLeft; // RA2: 0xABE37C
int TabClass::SlotTop; // RA2: 0xABE374
int TabClass::SlotPitch; // RA2: 0xABE388
int TabClass::SlotGap; // RA2: 0xABE460
int TabClass::AttackMoveCommand; // RA2: 0xABE464
int TabClass::BeaconCommand; // RA2: 0xABE294
int TabClass::CheerCommand; // RA2: 0xABD910
int TabClass::DeployCommand; // RA2: 0xABE278
int TabClass::GuardCommand; // RA2: 0xABE2B8
int TabClass::PlanningModeCommand; // RA2: 0xABE36C
int TabClass::StopCommand; // RA2: 0xABE2BC
int TabClass::Team01Command; // RA2: 0xABE370
int TabClass::Team02Command; // RA2: 0xABE378
int TabClass::Team03Command; // RA2: 0xABE468
int TabClass::TypeSelectCommand; // RA2: 0xABE290

/*
**	RA2: where the mouse was last seen over the bar (a function static of Draw_It).
*/
struct TabMouseSpot {
		TabMouseSpot(void) : X(0), Y(0) {}
		~TabMouseSpot(void) {}
		int X;
		int Y;
};

/*
**	RA2: the notices the planning mode sends to the bar.
*/
enum TabNoticeType {
	NOTICE_PLANNING_ON = 0x11C7,
	NOTICE_PLANNING_OFF = 0x11C8
};





























void TabClass::Free_Button_Shape(int index)	// RA2: frees the button shape when it was loaded rather than retrieved
{
	if (ButtonShapeAllocated[index]) {
		delete[] (char *)ButtonShapes[index];
		ButtonShapeAllocated[index] = false;
	}
	ButtonShapes[index] = NULL;
}



int TabClass::Command_From_Name(char const * name)	// RA2: the index of the name among the 11 command names
{
	for (int index = 0; index < COMMAND_COUNT; index++) {
		if (strcmp(CommandNames[index], name) == 0) {
			return(index);
		}
	}
	return(NoAdvancedCommand);
}



void TabClass::Set_Command_Position(int command, int position)	// RA2: maps the command to a button position
{
	SlotButton[command] = position;
}



int TabClass::Get_Command_Position(int command)	// RA2: the button position of a command
{
	return(SlotButton[command]);
}



ShapeButtonClass * TabClass::Get_Button(int command)	// RA2: the button a command is on, or none when its position is out of range
{
	if (SlotButton[command] >= 0 && SlotButton[command] < BUTTON_COUNT) {	// RA2: the position is read three times in the source; the extra source size is what keeps this call out of line in AI's inlined Hide_Buttons loop
		return(&CommandButtons[SlotButton[command]]);
	}
	return(NULL);
}



Rect TabClass::Get_Button_Rect(int command)	// RA2: the first slot rectangle moved by the command's position
{
	Rect rect = *ButtonSlotRect;
	return(Rect(SlotButton[command] * SlotPitch + rect.X, SlotTop, rect.Width, rect.Height));
}



void TabClass::Set_Button_Count(int count)	// RA2: sets the number of rows; the buttons' left edge is not left of the open thumb button's right edge
{
	ButtonRows = count;
	int left = TabEndRect->X - count * ButtonSlotRect->Width;
	int limit = ThumbOpenRect->X + ThumbOpenRect->Width;
	ButtonsLeft = left > limit ? left : limit;
}



int TabClass::Is_Team_Command(int command)	// RA2: whether the command lies in Team01 to Team03
{
	if (command >= Team01Command && command <= Team03Command) {
		return(1);
	}
	return(0);
}



int TabClass::Team_Number(int command)	// RA2: the team of a team command, from 1
{
	return(command - Team01Command + 1);
}



/***********************************************************************************************
 * TabClass::TabClass -- Default construct for the tab button class.                           *
 *                                                                                             *
 *    The default constructor merely sets the tab buttons to default non-selected state.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TabClass::TabClass(void) :	// RA2: looks up the command numbers by name and registers as the planning mode's notice sink
	IsThumbActive(true),
	IsToRedraw(true),
	IsDrawn(true)
{
	AttackMoveCommand = Command_From_Name("AttackMove");
	BeaconCommand = Command_From_Name("Beacon");
	CheerCommand = Command_From_Name("Cheer");
	DeployCommand = Command_From_Name("Deploy");
	GuardCommand = Command_From_Name("Guard");
	PlanningModeCommand = Command_From_Name("PlanningMode");
	StopCommand = Command_From_Name("Stop");
	Team01Command = Command_From_Name("Team01");
	Team02Command = Command_From_Name("Team02");
	Team03Command = Command_From_Name("Team03");
	TypeSelectCommand = Command_From_Name("TypeSelect");
	Planning_Set_Sink(this);
}



/***********************************************************************************************
 * TabClass::One_Time -- Performs one time initialization of tab handler class.                *
 *                                                                                             *
 *    This routine will perform any one time initializations of the tab handler class. This    *
 *    typically includes the loading of the shapes that appear on it.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void TabClass::One_Time(void)	// RA2: only the sidebar's one-time setup
{
	BASECLASS::One_Time();
}






void TabClass::DisposeOfArt(void)	// RA2: after the sidebar's, frees every command button shape
{
	BASECLASS::DisposeOfArt();
	for (int index = 0; index < BUTTON_COUNT; index++) {
		Free_Button_Shape(index);
	}
}



/// <summary>
/// Initializes the tab bar for the player's house.
/// This routine is called once the player's house has been established. It fetches the
/// tab artwork and resets the credits readout so that it will tick up from nothing.
/// </summary>
void TabClass::Init_For_House(void)	// RA2: after the sidebar's, loads Button00.SHP to Button24.SHP and derives the bar's metrics
{
	BASECLASS::Init_For_House();

	bool allocated = false;
	for (int index = 0; index < BUTTON_COUNT; index++) {
		char name[256];
		sprintf(name, "Button%02d.SHP", index);
		ButtonShapes[index] = (ShapeSet const *)Load_Alloc_Data(name, allocated);
		ButtonShapeAllocated[index] = allocated;
	}

	short const * header = (short const *)ButtonShapes[0];
	ButtonWidth = header[1];
	ButtonHeight = header[2];

	Rect slot = *ButtonSlotRect;
	ButtonsLeft = TabEndRect->X - ButtonRows * slot.Width;
	SlotTop = TabBarRect->Y;
	SlotGap = slot.Width - ButtonWidth;
	SlotPitch = SlotGap + ButtonWidth;
	Credits.Current = 0;
}





















void TabClass::Init_IO(void)	// RA2: after the sidebar's, sets up the command buttons and the two thumb buttons
{
	BASECLASS::Init_IO();
	if (!Debug_Map && Is_Sidebar_Art_Loaded()) {
		Setup_Command_Buttons();

		ThumbOpenButton.ID = BUTTON_THUMB_OPEN;
		ThumbOpenButton.ShapeDrawer = SidebarDrawer;
		ThumbOpenButton.IsOn = false;
		ThumbOpenButton.IsToggleType = 0;
		ThumbOpenButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);
		ShapeSet const * shape = TabThumbShape;
		Rect area = *ThumbOpenRect;
		ThumbOpenButton.Set_Position(area.X, area.Y);
		ThumbOpenButton.Set_Shape(shape, 0, 0);

		ThumbClosedButton.ID = BUTTON_THUMB_CLOSED;
		ThumbClosedButton.ShapeDrawer = SidebarDrawer;
		ThumbClosedButton.IsOn = false;
		ThumbClosedButton.IsToggleType = 0;
		ThumbClosedButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);
		Rect area2 = *ThumbClosedRect;
		ThumbClosedButton.Set_Position(area2.X, area2.Y);
		ThumbClosedButton.Set_Shape(shape, 0, 0);
	}
}



void TabClass::Add_Command_Buttons(void)	// RA2: adds the 25 command buttons
{
	for (int index = 0; index < BUTTON_COUNT; index++) {
		ShapeButtonClass & button = CommandButtons[index];
		button.Zap();
		Add_A_Button(button);
	}
}



void TabClass::Remove_Command_Buttons(void)	// RA2: removes the 25 command buttons
{
	for (int index = 0; index < BUTTON_COUNT; index++) {
		Remove_A_Button(CommandButtons[index]);
	}
}



bool TabClass::Activate(int control)	// RA2: the sidebar shown or hidden: updates the bar's buttons and tool tips
{
	bool changed = BASECLASS::Activate(control);
	if (changed) {
		if (IsSidebarActive) {
			if (IsThumbActive) {
				Show_Buttons();
			} else {
				Add_A_Button(ThumbClosedButton);
				Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");
			}
		} else if (IsThumbActive) {
			for (int index = 0; index < BUTTON_COUNT; index++) {
				Remove_A_Button(CommandButtons[index]);
			}
			Remove_A_Button(ThumbOpenButton);
			for (int command = 0; command < BUTTON_COUNT; command++) {
				ShapeButtonClass * button = Get_Button(command);
				if (button != NULL) {
					ToolTips->Remove(button->ID);
				}
			}
			ToolTips->Remove(BUTTON_THUMB_OPEN);
		} else {
			Remove_A_Button(ThumbClosedButton);
			ToolTips->Remove(BUTTON_THUMB_CLOSED);
		}
		Flag_To_Redraw(GS_REDRAW_ALL);
	}
	return(changed);
}



/***********************************************************************************************
 * TabClass::AI -- Handles player I/O with the tab buttons.                                    *
 *                                                                                             *
 *    This routine is called every game tick and passed whatever key the player has supplied.  *
 *    If the input selects a tab button, then the graphic gets updated accordingly.            *
 *                                                                                             *
 * INPUT:   input -- The player's input character (might be mouse click).                      *
 *                                                                                             *
 *          x,y   -- Mouse coordinates at time of input.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *   12/31/1994 JLB : Uses mouse coordinate parameters.                                        *
 *   05/31/1995 JLB : Fixed to handle mouse shape properly.                                    *
 *   08/25/1995 JLB : Handles new scrolling option.                                            *
 *=============================================================================================*/
void TabClass::AI(KeyNumType &input, Point2D const & xy)	// RA2: a pressed command or thumb button runs its command; the bar's area keeps the normal mouse. Not a match: the exe inlines Close_Thumb and Hide_Buttons here but not Get_Button in the loop
{
	if (MoneyFlashTimer == 1) {
		IsToRedraw = true;
		Flag_To_Redraw();
	}

	Rect rect(TabBarRect->X, TabBarRect->Y, TacticalRect.Width, TabBarHeight - 2);
	if (rect.Is_Point_Within(xy)) {
		Map.Override_Mouse_Shape(MOUSE_NORMAL, false);
	}

	int key = input;
	if (key >= (KN_BUTTON | BUTTON_COMMAND_FIRST) && key < (KN_BUTTON | (BUTTON_COMMAND_FIRST + BUTTON_COUNT))) {
		int id = (key & ~KN_BUTTON) - BUTTON_COMMAND_FIRST;
		if (id == AttackMoveCommand) {
			Command_Attack_Move();
		} else if (id == BeaconCommand) {
			Command_Beacon();
		} else if (id == CheerCommand) {
			Command_Cheer();
		} else if (id == DeployCommand) {
			Command_Deploy();
		} else if (id == GuardCommand) {
			Command_Guard();
		} else if (id == PlanningModeCommand) {
			if (Get_Button(PlanningModeCommand)->IsOn) {
				Command_Planning_Mode_On(true);
			} else {
				Command_Planning_Mode_Off(true);
			}
		} else if (id == StopCommand) {
			Command_Stop();
		} else if (Is_Team_Command(id)) {
			int group = Team_Number(id);
			if (Group_Count(group) == 0) {
				Group_Assign(group);
			} else if (Group_Is_All_Selected(group)) {
				Group_Reselect(group);
			} else {
				Group_Select(group);
			}
		} else if (id == TypeSelectCommand) {
			Command_Type_Select();
		}
	} else if (key == (KN_BUTTON | BUTTON_THUMB_OPEN)) {
		VocClass::PlayGlobal(Rule->GUICloseSound, 1.0f, 0x2000, NULL);
		Close_Thumb();
	} else if (key == (KN_BUTTON | BUTTON_THUMB_CLOSED)) {
		if (MuteSWLaunches == 0) {
			VocClass::PlayGlobal(Rule->GUIOpenSound, 1.0f, 0x2000, NULL);
			Open_Thumb();
		}
	} else if (key >= (WWKEY_UNK_BIT | KN_BUTTON | BUTTON_COMMAND_FIRST) && key < (WWKEY_UNK_BIT | KN_BUTTON | (BUTTON_COMMAND_FIRST + BUTTON_COUNT))) {
		int id = (key & ~(WWKEY_UNK_BIT | KN_BUTTON)) - BUTTON_COMMAND_FIRST;
		if (Is_Team_Command(id)) {
			Group_Clear(Team_Number(id));
		}
	}

	Credits.AI();
	BASECLASS::AI(input, xy);
}






/***********************************************************************************************
 * TabClass::Draw_It -- Displays the tab buttons as necessary.                                 *
 *                                                                                             *
 *    This routine is called whenever the display is being redrawn (in some fashion). The      *
 *    parameter can be used to force the tab buttons to redraw completely. The default action  *
 *    is to only redraw if the tab buttons have been explicitly flagged to be redraw. The      *
 *    result of this is the elimination of unnecessary redraws.                                *
 *                                                                                             *
 * INPUT:   complete -- bool; Force redraw of the entire tab button graphics?                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *   05/19/1995 JLB : New EVA style.                                                           *
 *=============================================================================================*/
#define	EVA_WIDTH		80
#define	TAB_HEIGHT		8
void TabClass::Add_Button_ToolTip(ControlClass * control, char const * label)	// RA2: registers a control's tool tip under its ID with a label
{
	if (control != NULL) {
		ToolTip tip;
		tip.ID = control->ID;
		tip.Text = (int)label;
		tip.Region.X = control->X;
		tip.Region.Y = control->Y;
		tip.Region.Width = control->Width;
		tip.Region.Height = control->Height;
		ToolTips->Remove(control->ID);
		ToolTips->Add(&tip);
	}
}



void TabClass::Draw_It(bool complete)	// RA2: draws the command bar when flagged, when a button changed or when the mouse moved over it
{
	static TabMouseSpot LastMouseSpot;
	static bool MouseInside;
	static Surface * DrawnOn[4];
	static int DrawnCount;

	bool redraw = complete || IsToRedraw;
	if (!redraw) {
		for (int command = 0; command < BUTTON_COUNT; command++) {
			ShapeButtonClass * button = Get_Button(command);
			if (button != NULL && button->IsDrawn) {
				redraw = true;
				break;
			}
		}
	}
	if (redraw) {
		DrawnCount = 0;
	} else {
		Point2D mouse = MouseCursor->Get_Mouse_Point();
		if (TabBarRect->Is_Point_Within(mouse)) {
			MouseInside = true;
			if (mouse.X != LastMouseSpot.X || mouse.Y != LastMouseSpot.Y) {
				redraw = true;
			}
		} else {
			if (MouseInside) {
				redraw = true;
			}
			MouseInside = false;
		}
		LastMouseSpot.X = mouse.X;
		LastMouseSpot.Y = mouse.Y;
		if (redraw) {
			DrawnCount = 0;
		} else {
			for (int index = 0; index < DrawnCount; index++) {
				if (DrawnOn[index] == LogicalSurface) {
					goto drawn;
				}
			}
			redraw = true;
		drawn:;
		}
	}

	if (!Debug_Map) {
		Draw_Credits_Tab();
		Credits.Graphic_Logic(true);
		if (redraw) {
			Rect view = LogicalSurface->Get_Rect();
			Point2D where(0, 0);
			where.X = TabBarRect->X;
			where.Y = TabBarRect->Y;
			Rect narrow = view;
			narrow.Width = TabBarRect->Width;
			Surface * surface = LogicalSurface;
			Draw_Shape(*surface, *SidebarDrawer, TabLeftShape, 0, where, narrow);
			if (IsThumbActive) {
				where.X = ThumbOpenRect->X;
				where.Y = ThumbOpenRect->Y;
			} else {
				where.X = ThumbClosedRect->X;
				where.Y = ThumbClosedRect->Y;
			}
			Draw_Shape(*surface, *SidebarDrawer, TabThumbShape, 0, where, view);
			if (IsThumbActive) {
				where.X = ButtonSlotRect->X;
				where.Y = TabBarRect->Y;
				for (int tile = 0; tile < TabTileCount; tile++) {
					Draw_Shape(*surface, *SidebarDrawer, TabTileShape, 0, where, view);
					where.X += ButtonSlotRect->Width;
				}
				for (int command = 0; command < BUTTON_COUNT; command++) {
					ShapeButtonClass * button = Get_Button(command);
					if (button != NULL && button->Get_Shape_Data() != NULL) {
						button->Draw_Me(true);
						button->IsDrawn = false;
					}
				}
			}
			where.X = TabEndRect->X;
			Draw_Shape(*surface, *SidebarDrawer, TabRightShape, 0, where, view);
			if (IsThumbActive) {
				ThumbOpenButton.Draw_Me(true);
			} else {
				ThumbClosedButton.Draw_Me(true);
			}
			IsDrawn = true;
			IsToRedraw = false;
			if (DrawnCount < sizeof(DrawnOn) / sizeof(DrawnOn[0])) {
				DrawnCount++;
			}
			for (int index = DrawnCount - 2; index >= 0; index--) {
				DrawnOn[index + 1] = DrawnOn[index];
			}
			DrawnOn[0] = surface;
		} else {
			IsDrawn = false;
		}
	}

	BASECLASS::Draw_It(complete);
}



/// <summary>
/// Draws the tab backdrop for the credits and the mission timer.
/// This routine lays down the sidebar tab imagery that the credits readout is printed
/// over, and prints the mission timer alongside it whenever a timer is running. The
/// credit display calls this before it prints the new money value.
/// </summary>
void TabClass::Draw_Credits_Tab(void)	// RA2: only the backdrop shape on the sidebar surface
{
	ShapeSet const * shape = TabBackShape;
	Draw_Shape(*SidebarSurface, *SidebarDrawer, shape, 0, Point2D(0, 0), SidebarSurface->Get_Rect());
	IsToBlitSidebar = true;
}



/// <summary>
/// Flashes the credits readout to catch the player's eye.
/// This routine is used when something has happened that the player really ought to
/// notice about the state of his funds, such as running short of cash. The tab bar is
/// flagged for redraw and the money display pulses for a moment before settling down.
/// </summary>
void TabClass::Flash_Money(void)	// RA2: flags the bar and restarts the flash timer at 7 frames
{
	IsToRedraw = true;
	Flag_To_Redraw(GS_REDRAW_DIRTY);
	MoneyFlashTimer = 7;
}













































void TabClass::Redraw_Tab(void)	// RA2: out-of-line copy of the inline
{
	IsToRedraw = true;
	Flag_To_Redraw();
}



void __cdecl Load_Button_Shapes(void)	// RA2: out-of-line copy of the loop Init_For_House has
{
	char name[256];
	bool allocated = false;
	for (int index = 0; index < TabClass::BUTTON_COUNT; index++) {
		sprintf(name, "Button%02d.SHP", index);
		TabClass::ButtonShapes[index] = (ShapeSet const *)Load_Alloc_Data(name, allocated);
		TabClass::ButtonShapeAllocated[index] = allocated;
	}
}



void __cdecl Free_Button_Shapes(void)	// RA2: out-of-line copy of the loop DisposeOfArt has
{
	for (int index = 0; index < TabClass::BUTTON_COUNT; index++) {
		TabClass::Free_Button_Shape(index);
	}
}



void Init_Shape_Button(ShapeButtonClass * button, int id, unsigned flags)	// RA2: out-of-line copy of the button set-up the thumb buttons share
{
	button->ID = id;
	button->ShapeDrawer = SidebarDrawer;
	button->IsOn = false;
	button->IsToggleType = 0;
	button->Set_Flags(flags);
}



void TabClass::Setup_Command_Buttons(void)	// RA2: gives each command button its place, shape and click flags
{
	for (int index = 0; index < BUTTON_COUNT; index++) {
		CommandButtons[index].Set_Shape(NULL, 0, 0);
	}
	Rect area = *ThumbClosedRect;
	int right = area.X + area.Width;
	for (int command = 0; command < BUTTON_COUNT; command++) {
		ShapeButtonClass * button = Get_Button(command);
		if (button) {
			Rect box = *ButtonSlotRect;
			int x = SlotButton[command] * SlotPitch + box.X;
			int y = SlotTop;
			if (box.Width + x <= right) {
				button->ID = command + BUTTON_COMMAND_FIRST;
				button->ShapeDrawer = SidebarDrawer;
				button->IsOn = false;
				button->IsToggleType = 0;
				button->Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);
				button->Set_Position(x, y);
				button->Set_Shape(ButtonShapes[command], 0, 0);
			}
		}
	}
	ShapeButtonClass * planning = Get_Button(PlanningModeCommand);
	if (planning) {
		planning->IsToggleType = 1;
		planning->ReflectButtonState = true;
	}
	for (int team = Team01Command; team <= Team03Command; team++) {
		ShapeButtonClass * button = Get_Button(team);
		if (button) {
			button->Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE | GadgetClass::RIGHTPRESS | GadgetClass::RIGHTRELEASE);
		}
	}
}



void __cdecl TabClass::Setup_Thumb_Buttons(void)	// RA2: out-of-line copy of the code Init_IO has
{
	ThumbOpenButton.ID = BUTTON_THUMB_OPEN;
	ThumbOpenButton.ShapeDrawer = SidebarDrawer;
	ThumbOpenButton.IsOn = false;
	ThumbOpenButton.IsToggleType = 0;
	ThumbOpenButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);
	ShapeSet const * shape = TabThumbShape;
	Rect area = *ThumbOpenRect;
	ThumbOpenButton.Set_Position(area.X, area.Y);
	ThumbOpenButton.Set_Shape(shape, 0, 0);

	ThumbClosedButton.ID = BUTTON_THUMB_CLOSED;
	ThumbClosedButton.ShapeDrawer = SidebarDrawer;
	ThumbClosedButton.IsOn = false;
	ThumbClosedButton.IsToggleType = 0;
	ThumbClosedButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);
	Rect area2 = *ThumbClosedRect;
	ThumbClosedButton.Set_Position(area2.X, area2.Y);
	ThumbClosedButton.Set_Shape(shape, 0, 0);
}



void TabClass::Show_Buttons(void)	// RA2: adds the open bar's buttons and their tool tips
{
	Add_Command_Buttons();
	Add_A_Button(ThumbOpenButton);
	Add_Button_ToolTip(Get_Button(AttackMoveCommand), "Tip:AttackMove");
	Add_Button_ToolTip(Get_Button(BeaconCommand), "Tip:Beacon");
	Add_Button_ToolTip(Get_Button(CheerCommand), "Tip:Cheer");
	Add_Button_ToolTip(Get_Button(DeployCommand), "Tip:Deploy");
	Add_Button_ToolTip(Get_Button(GuardCommand), "Tip:Guard");
	Add_Button_ToolTip(Get_Button(PlanningModeCommand), "Tip:PlanningMode");
	{
		ShapeButtonClass * button = Get_Button(StopCommand);
		Add_Button_ToolTip(button, "Tip:Stop");
	}
	{
		ShapeButtonClass * button = Get_Button(Team01Command);
		Add_Button_ToolTip(button, "Tip:Team01");
	}
	{
		ShapeButtonClass * button = Get_Button(Team02Command);
		Add_Button_ToolTip(button, "Tip:Team02");
	}
	{
		ShapeButtonClass * button = Get_Button(Team03Command);
		Add_Button_ToolTip(button, "Tip:Team03");
	}
	{
		ShapeButtonClass * button = Get_Button(TypeSelectCommand);
		Add_Button_ToolTip(button, "Tip:TypeSelect");
	}
	Add_Button_ToolTip(&ThumbOpenButton, "Tip:ThumbOpen");
}



void TabClass::Hide_Buttons(void)	// RA2: removes the open bar's buttons and their tool tips
{
	Remove_Command_Buttons();
	Remove_A_Button(ThumbOpenButton);
	for (int command = 0; command < BUTTON_COUNT; command++) {
		ShapeButtonClass * button = Get_Button(command);
		if (button != NULL) {
			ToolTips->Remove(button->ID);
		}
	}
	ToolTips->Remove(BUTTON_THUMB_OPEN);
}



void TabClass::Show_Thumb_Button(void)	// RA2: adds the closed bar's thumb button and its tool tip
{
	Add_A_Button(ThumbClosedButton);
	Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");
}



void TabClass::Hide_Thumb_Button(void)	// RA2: removes the closed bar's thumb button and its tool tip
{
	Remove_A_Button(ThumbClosedButton);
	ToolTips->Remove(BUTTON_THUMB_CLOSED);
}



void TabClass::Open_Thumb(void)	// RA2: swaps the closed bar's thumb button for the open bar's buttons
{
	if (!IsThumbActive) {
		Hide_Thumb_Button();
		IsThumbActive = true;
		Show_Buttons();
		IsToRedraw = true;
		Flag_To_Redraw(GS_REDRAW_DIRTY);
	}
}



void TabClass::Close_Thumb(void)	// RA2: swaps the open bar's buttons for the closed bar's thumb button
{
	if (IsThumbActive) {
		Hide_Buttons();
		IsThumbActive = false;
		Show_Thumb_Button();
		IsToRedraw = true;
		Flag_To_Redraw(GS_REDRAW_DIRTY);
	}
}



bool TabClass::Unknown_Virtual_0(int code, int)	// RA2: the planning mode switches the planning button on or off
{
	if (code == NOTICE_PLANNING_ON) {
		ShapeButtonClass * button = Get_Button(PlanningModeCommand);
		if (button != NULL && !button->IsOn) {
			button->Turn_On();
		}
		return(true);
	}
	if (code == NOTICE_PLANNING_OFF) {
		ShapeButtonClass * button = Get_Button(PlanningModeCommand);
		if (button != NULL && button->IsOn) {
			button->Turn_Off();
		}
		return(true);
	}
	return(false);
}



char const * TabClass::Help_Text(int id)	// RA2: only the sidebar's
{
	return(BASECLASS::Help_Text(id));
}


/// <summary>
/// Draws the specified tab in its highlighted state.
/// This routine is used to give the player some feedback while a tab is being pressed.
/// The tab imagery and its label are redrawn in the highlight style.
/// </summary>
/// <param name="tab">The tab to highlight; zero for the controls tab, non-zero for the
/// sidebar tab.</param>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
ShapeSet const * TabClass::TabShape = NULL; // RA2: no reference in the executable; read only by this draft, so parked with it

void TabClass::Hilite_Tab(int tab)
{
	int xpos = 0;
	int text = TXT_TAB_BUTTON_CONTROLS;
	int textx = (EVA_WIDTH/2) * 2;

	if (tab) {
		xpos = (320-EVA_WIDTH) * 2;
		//text = TXT_TAB_SIDEBAR;
		//textx = (320-(EVA_WIDTH/2)) * 2;
	} else {
		xpos = Options.IsSidebarOnRight ? 0 : LogicalSurface->Get_Rect().Width - textx*2;
	}

	Draw_Shape(*LogicalSurface, *SidebarDrawer, TabShape, 1, Point2D(xpos, 0), VisibleRect);
	Fancy_Text_Print(text, *LogicalSurface, LogicalSurface->Get_Rect(), Point2D(xpos + textx, 0), ColorSchemes[0], TBLACK, TextPrintType(TPF_METAL12 | TPF_CENTER | TPF_USE_GRAD_PAL));
}
#endif


/***********************************************************************************************
 * TabClass::Set_Active -- Activates a "filefolder tab" button.                                *
 *                                                                                             *
 *    This function is used to activate one of the file folder tab buttons that appear at the  *
 *    top edge of the screen.                                                                  *
 *                                                                                             *
 * INPUT:   select   -- The button to activate. 0 = left button, 1=next button, etc.           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
void TabClass::Set_Active(int select)
{
	switch (select) {
		case 0:
			Queue_Options();
			break;

		case 1:
			BASECLASS::Activate(-1);
			break;

		default:
			break;
	}
}
#endif


/***********************************************************************************************
 * TabClass::One_Time -- Performs one time initialization of tab handler class.                *
 *                                                                                             *
 *    This routine will perform any one time initializations of the tab handler class. This    *
 *    typically includes the loading of the shapes that appear on it.                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int const NoAdvancedCommand = -1; // RA2: 0x7F67F4; defined after its uses, so that they load it
char const * const CommandNames[TabClass::COMMAND_COUNT] = { // RA2: 0x7F67F8
	"Team01", "Team02", "Team03", "TypeSelect", "Deploy", "AttackMove", "Guard", "Beacon", "Stop", "PlanningMode", "Cheer"
};
