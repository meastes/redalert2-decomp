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
struct TabMouseSpot { // RA2: as above
	TabMouseSpot(void) : X(0), Y(0) {} // RA2: as above
	~TabMouseSpot(void) {} // RA2: as above
	int X; // RA2: as above
	int Y; // RA2: as above
};

/*
**	RA2: the notices the planning mode sends to the bar.
*/
enum TabNoticeType { // RA2: as above
	NOTICE_PLANNING_ON = 0x11C7, // RA2: as above
	NOTICE_PLANNING_OFF = 0x11C8 // RA2: as above
};





























void TabClass::Free_Button_Shape(int index)	// RA2: frees the button shape when it was loaded rather than retrieved
{
	if (ButtonShapeAllocated[index]) {	// RA2: as above
		delete [] (char *)ButtonShapes[index];	// RA2: as above
		ButtonShapeAllocated[index] = false;	// RA2: as above
	}
	ButtonShapes[index] = NULL;	// RA2: as above
}



int TabClass::Command_From_Name(char const * name)	// RA2: the index of the name among the 11 command names
{
	for (int index = 0; index < COMMAND_COUNT; index++) {	// RA2: as above
		if (strcmp(CommandNames[index], name) == 0) {	// RA2: as above
			return(index);	// RA2: as above
		}
	}
	return(NoAdvancedCommand);	// RA2: as above
}



void TabClass::Set_Command_Position(int command, int position)	// RA2: maps the command to a button position
{
	SlotButton[command] = position;	// RA2: as above
}



int TabClass::Get_Command_Position(int command)	// RA2: the button position of a command
{
	return(SlotButton[command]);	// RA2: as above
}



ShapeButtonClass * TabClass::Get_Button(int command)	// RA2: the button a command is on, or none when its position is out of range
{
	if (SlotButton[command] >= 0 && SlotButton[command] < BUTTON_COUNT) {	// RA2: the position is read three times in the source; the extra source size is what keeps this call out of line in AI's inlined Hide_Buttons loop
		return(&CommandButtons[SlotButton[command]]);	// RA2: as above
	}
	return(NULL);	// RA2: as above
}



Rect TabClass::Get_Button_Rect(int command)	// RA2: the first slot rectangle moved by the command's position
{
	Rect rect = *ButtonSlotRect;	// RA2: as above
	return(Rect(SlotButton[command] * SlotPitch + rect.X, SlotTop, rect.Width, rect.Height));	// RA2: as above
}



void TabClass::Set_Button_Count(int count)	// RA2: sets the number of rows; the buttons' left edge is not left of the open thumb button's right edge
{
	ButtonRows = count;	// RA2: as above
	int left = TabEndRect->X - count * ButtonSlotRect->Width;	// RA2: as above
	int limit = ThumbOpenRect->X + ThumbOpenRect->Width;	// RA2: as above
	ButtonsLeft = left > limit ? left : limit;	// RA2: as above
}



int TabClass::Is_Team_Command(int command)	// RA2: whether the command lies in Team01 to Team03
{
	if (command >= Team01Command && command <= Team03Command) {	// RA2: as above
		return(1);	// RA2: as above
	}
	return(0);	// RA2: as above
}



int TabClass::Team_Number(int command)	// RA2: the team of a team command, from 1
{
	return(command - Team01Command + 1);	// RA2: as above
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
	IsThumbActive(true),	// RA2: as above
	IsToRedraw(true),	// RA2: as above
	IsDrawn(true)	// RA2: as above
{
	AttackMoveCommand = Command_From_Name("AttackMove");	// RA2: as above
	BeaconCommand = Command_From_Name("Beacon");	// RA2: as above
	CheerCommand = Command_From_Name("Cheer");	// RA2: as above
	DeployCommand = Command_From_Name("Deploy");	// RA2: as above
	GuardCommand = Command_From_Name("Guard");	// RA2: as above
	PlanningModeCommand = Command_From_Name("PlanningMode");	// RA2: as above
	StopCommand = Command_From_Name("Stop");	// RA2: as above
	Team01Command = Command_From_Name("Team01");	// RA2: as above
	Team02Command = Command_From_Name("Team02");	// RA2: as above
	Team03Command = Command_From_Name("Team03");	// RA2: as above
	TypeSelectCommand = Command_From_Name("TypeSelect");	// RA2: as above
	Planning_Set_Sink(this);	// RA2: as above
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
	BASECLASS::One_Time();	// RA2: as above
}






void TabClass::DisposeOfArt(void)	// RA2: after the sidebar's, frees every command button shape
{
	BASECLASS::DisposeOfArt();	// RA2: as above
	for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
		Free_Button_Shape(index);	// RA2: as above
	}
}



/// <summary>
/// Initializes the tab bar for the player's house.
/// This routine is called once the player's house has been established. It fetches the
/// tab artwork and resets the credits readout so that it will tick up from nothing.
/// </summary>
void TabClass::Init_For_House(void)	// RA2: after the sidebar's, loads Button00.SHP to Button24.SHP and derives the bar's metrics
{
	BASECLASS::Init_For_House();	// RA2: as above

	bool allocated = false;	// RA2: as above
	for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
		char name[256];	// RA2: as above
		sprintf(name, "Button%02d.SHP", index);	// RA2: as above
		ButtonShapes[index] = (ShapeSet const *)Load_Alloc_Data(name, allocated);	// RA2: as above
		ButtonShapeAllocated[index] = allocated;	// RA2: as above
	}

	short const * header = (short const *)ButtonShapes[0];	// RA2: as above
	ButtonWidth = header[1];	// RA2: as above
	ButtonHeight = header[2];	// RA2: as above

	Rect slot = *ButtonSlotRect;	// RA2: as above
	ButtonsLeft = TabEndRect->X - ButtonRows * slot.Width;	// RA2: as above
	SlotTop = TabBarRect->Y;	// RA2: as above
	SlotGap = slot.Width - ButtonWidth;	// RA2: as above
	SlotPitch = SlotGap + ButtonWidth;	// RA2: as above
	Credits.Current = 0;	// RA2: as above
}





















void TabClass::Init_IO(void)	// RA2: after the sidebar's, sets up the command buttons and the two thumb buttons
{
	BASECLASS::Init_IO();	// RA2: as above
	if (!Debug_Map && Is_Sidebar_Art_Loaded()) {	// RA2: as above
		Setup_Command_Buttons();	// RA2: as above

		ThumbOpenButton.ID = BUTTON_THUMB_OPEN;	// RA2: as above
		ThumbOpenButton.ShapeDrawer = SidebarDrawer;	// RA2: as above
		ThumbOpenButton.IsOn = false;	// RA2: as above
		ThumbOpenButton.IsToggleType = 0;	// RA2: as above
		ThumbOpenButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);	// RA2: as above
		ShapeSet const * shape = TabThumbShape;	// RA2: as above
		Rect area = *ThumbOpenRect;	// RA2: as above
		ThumbOpenButton.Set_Position(area.X, area.Y);	// RA2: as above
		ThumbOpenButton.Set_Shape(shape, 0, 0);	// RA2: as above

		ThumbClosedButton.ID = BUTTON_THUMB_CLOSED;	// RA2: as above
		ThumbClosedButton.ShapeDrawer = SidebarDrawer;	// RA2: as above
		ThumbClosedButton.IsOn = false;	// RA2: as above
		ThumbClosedButton.IsToggleType = 0;	// RA2: as above
		ThumbClosedButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);	// RA2: as above
		Rect area2 = *ThumbClosedRect;	// RA2: as above
		ThumbClosedButton.Set_Position(area2.X, area2.Y);	// RA2: as above
		ThumbClosedButton.Set_Shape(shape, 0, 0);	// RA2: as above
	}
}



void TabClass::Add_Command_Buttons(void)	// RA2: adds the 25 command buttons
{
	for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
		ShapeButtonClass & button = CommandButtons[index];	// RA2: as above
		button.Zap();	// RA2: as above
		Add_A_Button(button);	// RA2: as above
	}
}



void TabClass::Remove_Command_Buttons(void)	// RA2: removes the 25 command buttons
{
	for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
		Remove_A_Button(CommandButtons[index]);	// RA2: as above
	}
}



bool TabClass::Activate(int control)	// RA2: the sidebar shown or hidden: updates the bar's buttons and tool tips
{
	bool changed = BASECLASS::Activate(control);	// RA2: as above
	if (changed) {	// RA2: as above
		if (IsSidebarActive) {	// RA2: as above
			if (IsThumbActive) {	// RA2: as above
				Show_Buttons();	// RA2: as above
			} else {	// RA2: as above
				Add_A_Button(ThumbClosedButton);	// RA2: as above
				Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");	// RA2: as above
			}
		} else if (IsThumbActive) {	// RA2: as above
			for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
				Remove_A_Button(CommandButtons[index]);	// RA2: as above
			}
			Remove_A_Button(ThumbOpenButton);	// RA2: as above
			for (int command = 0; command < BUTTON_COUNT; command++) {	// RA2: as above
				ShapeButtonClass * button = Get_Button(command);	// RA2: as above
				if (button != NULL) {	// RA2: as above
					ToolTips->Remove(button->ID);	// RA2: as above
				}
			}
			ToolTips->Remove(BUTTON_THUMB_OPEN);	// RA2: as above
		} else {	// RA2: as above
			Remove_A_Button(ThumbClosedButton);	// RA2: as above
			ToolTips->Remove(BUTTON_THUMB_CLOSED);	// RA2: as above
		}
		Flag_To_Redraw(GS_REDRAW_ALL);	// RA2: as above
	}
	return(changed);	// RA2: as above
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
	if (MoneyFlashTimer == 1) {	// RA2: as above
		IsToRedraw = true;	// RA2: as above
		Flag_To_Redraw();	// RA2: as above
	}

	Rect rect(TabBarRect->X, TabBarRect->Y, TacticalRect.Width, TabBarHeight - 2);	// RA2: as above
	if (rect.Is_Point_Within(xy)) {	// RA2: as above
		Map.Override_Mouse_Shape(MOUSE_NORMAL, false);	// RA2: as above
	}

	int key = input;	// RA2: as above
	if (key >= (KN_BUTTON | BUTTON_COMMAND_FIRST) && key < (KN_BUTTON | (BUTTON_COMMAND_FIRST + BUTTON_COUNT))) {	// RA2: as above
		int id = (key & ~KN_BUTTON) - BUTTON_COMMAND_FIRST;	// RA2: as above
		if (id == AttackMoveCommand) {	// RA2: as above
			Command_Attack_Move();	// RA2: as above
		} else if (id == BeaconCommand) {	// RA2: as above
			Command_Beacon();	// RA2: as above
		} else if (id == CheerCommand) {	// RA2: as above
			Command_Cheer();	// RA2: as above
		} else if (id == DeployCommand) {	// RA2: as above
			Command_Deploy();	// RA2: as above
		} else if (id == GuardCommand) {	// RA2: as above
			Command_Guard();	// RA2: as above
		} else if (id == PlanningModeCommand) {	// RA2: as above
			if (Get_Button(PlanningModeCommand)->IsOn) {	// RA2: as above
				Command_Planning_Mode_On(true);	// RA2: as above
			} else {	// RA2: as above
				Command_Planning_Mode_Off(true);	// RA2: as above
			}
		} else if (id == StopCommand) {	// RA2: as above
			Command_Stop();	// RA2: as above
		} else if (Is_Team_Command(id)) {	// RA2: as above
			int group = Team_Number(id);	// RA2: as above
			if (Group_Count(group) == 0) {	// RA2: as above
				Group_Assign(group);	// RA2: as above
			} else if (Group_Is_All_Selected(group)) {	// RA2: as above
				Group_Reselect(group);	// RA2: as above
			} else {	// RA2: as above
				Group_Select(group);	// RA2: as above
			}
		} else if (id == TypeSelectCommand) {	// RA2: as above
			Command_Type_Select();	// RA2: as above
		}
	} else if (key == (KN_BUTTON | BUTTON_THUMB_OPEN)) {	// RA2: as above
		VocClass::PlayGlobal(Rule->GUICloseSound, 1.0f, 0x2000, NULL);	// RA2: as above
		Close_Thumb();	// RA2: as above
	} else if (key == (KN_BUTTON | BUTTON_THUMB_CLOSED)) {	// RA2: as above
		if (MuteSWLaunches == 0) {	// RA2: as above
			VocClass::PlayGlobal(Rule->GUIOpenSound, 1.0f, 0x2000, NULL);	// RA2: as above
			Open_Thumb();	// RA2: as above
		}
	} else if (key >= (WWKEY_UNK_BIT | KN_BUTTON | BUTTON_COMMAND_FIRST) && key < (WWKEY_UNK_BIT | KN_BUTTON | (BUTTON_COMMAND_FIRST + BUTTON_COUNT))) {	// RA2: as above
		int id = (key & ~(WWKEY_UNK_BIT | KN_BUTTON)) - BUTTON_COMMAND_FIRST;	// RA2: as above
		if (Is_Team_Command(id)) {	// RA2: as above
			Group_Clear(Team_Number(id));	// RA2: as above
		}
	}

	Credits.AI();	// RA2: as above
	BASECLASS::AI(input, xy);	// RA2: as above
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
	if (control != NULL) {	// RA2: as above
		ToolTip tip;	// RA2: as above
		tip.ID = control->ID;	// RA2: as above
		tip.Text = (int)label;	// RA2: as above
		tip.Region.X = control->X;	// RA2: as above
		tip.Region.Y = control->Y;	// RA2: as above
		tip.Region.Width = control->Width;	// RA2: as above
		tip.Region.Height = control->Height;	// RA2: as above
		ToolTips->Remove(control->ID);	// RA2: as above
		ToolTips->Add(&tip);	// RA2: as above
	}
}



void TabClass::Draw_It(bool complete)	// RA2: draws the command bar when flagged, when a button changed or when the mouse moved over it
{
	static TabMouseSpot LastMouseSpot;	// RA2: as above
	static bool MouseInside;	// RA2: as above
	static Surface * DrawnOn[4];	// RA2: as above
	static int DrawnCount;	// RA2: as above

	bool redraw = complete || IsToRedraw;	// RA2: as above
	if (!redraw) {	// RA2: as above
		for (int command = 0; command < BUTTON_COUNT; command++) {	// RA2: as above
			ShapeButtonClass * button = Get_Button(command);	// RA2: as above
			if (button != NULL && button->IsDrawn) {	// RA2: as above
				redraw = true;	// RA2: as above
				break;	// RA2: as above
			}
		}
	}
	if (redraw) {	// RA2: as above
		DrawnCount = 0;	// RA2: as above
	} else {	// RA2: as above
		Point2D mouse = MouseCursor->Get_Mouse_Point();	// RA2: as above
		if (TabBarRect->Is_Point_Within(mouse)) {	// RA2: as above
			MouseInside = true;	// RA2: as above
			if (mouse.X != LastMouseSpot.X || mouse.Y != LastMouseSpot.Y) {	// RA2: as above
				redraw = true;	// RA2: as above
			}
		} else {	// RA2: as above
			if (MouseInside) {	// RA2: as above
				redraw = true;	// RA2: as above
			}
			MouseInside = false;	// RA2: as above
		}
		LastMouseSpot.X = mouse.X;	// RA2: as above
		LastMouseSpot.Y = mouse.Y;	// RA2: as above
		if (redraw) {	// RA2: as above
			DrawnCount = 0;	// RA2: as above
		} else {	// RA2: as above
			for (int index = 0; index < DrawnCount; index++) {	// RA2: as above
				if (DrawnOn[index] == LogicalSurface) {	// RA2: as above
					goto drawn;	// RA2: as above
				}
			}
			redraw = true;	// RA2: as above
drawn:;	// RA2: as above
		}
	}

	if (!Debug_Map) {	// RA2: as above
		Draw_Credits_Tab();	// RA2: as above
		Credits.Graphic_Logic(true);	// RA2: as above
		if (redraw) {	// RA2: as above
			Rect view = LogicalSurface->Get_Rect();	// RA2: as above
			Point2D where(0, 0);	// RA2: as above
			where.X = TabBarRect->X;	// RA2: as above
			where.Y = TabBarRect->Y;	// RA2: as above
			Rect narrow = view;	// RA2: as above
			narrow.Width = TabBarRect->Width;	// RA2: as above
			Surface * surface = LogicalSurface;	// RA2: as above
			Draw_Shape(*surface, *SidebarDrawer, TabLeftShape, 0, where, narrow);	// RA2: as above
			if (IsThumbActive) {	// RA2: as above
				where.X = ThumbOpenRect->X;	// RA2: as above
				where.Y = ThumbOpenRect->Y;	// RA2: as above
			} else {	// RA2: as above
				where.X = ThumbClosedRect->X;	// RA2: as above
				where.Y = ThumbClosedRect->Y;	// RA2: as above
			}
			Draw_Shape(*surface, *SidebarDrawer, TabThumbShape, 0, where, view);	// RA2: as above
			if (IsThumbActive) {	// RA2: as above
				where.X = ButtonSlotRect->X;	// RA2: as above
				where.Y = TabBarRect->Y;	// RA2: as above
				for (int tile = 0; tile < TabTileCount; tile++) {	// RA2: as above
					Draw_Shape(*surface, *SidebarDrawer, TabTileShape, 0, where, view);	// RA2: as above
					where.X += ButtonSlotRect->Width;	// RA2: as above
				}
				for (int command = 0; command < BUTTON_COUNT; command++) {	// RA2: as above
					ShapeButtonClass * button = Get_Button(command);	// RA2: as above
					if (button != NULL && button->Get_Shape_Data() != NULL) {	// RA2: as above
						button->Draw_Me(true);	// RA2: as above
						button->IsDrawn = false;	// RA2: as above
					}
				}
			}
			where.X = TabEndRect->X;	// RA2: as above
			Draw_Shape(*surface, *SidebarDrawer, TabRightShape, 0, where, view);	// RA2: as above
			if (IsThumbActive) {	// RA2: as above
				ThumbOpenButton.Draw_Me(true);	// RA2: as above
			} else {	// RA2: as above
				ThumbClosedButton.Draw_Me(true);	// RA2: as above
			}
			IsDrawn = true;	// RA2: as above
			IsToRedraw = false;	// RA2: as above
			if (DrawnCount < sizeof(DrawnOn) / sizeof(DrawnOn[0])) {	// RA2: as above
				DrawnCount++;	// RA2: as above
			}
			for (int index = DrawnCount - 2; index >= 0; index--) {	// RA2: as above
				DrawnOn[index + 1] = DrawnOn[index];	// RA2: as above
			}
			DrawnOn[0] = surface;	// RA2: as above
		} else {	// RA2: as above
			IsDrawn = false;	// RA2: as above
		}
	}

	BASECLASS::Draw_It(complete);	// RA2: as above
}



/// <summary>
/// Draws the tab backdrop for the credits and the mission timer.
/// This routine lays down the sidebar tab imagery that the credits readout is printed
/// over, and prints the mission timer alongside it whenever a timer is running. The
/// credit display calls this before it prints the new money value.
/// </summary>
void TabClass::Draw_Credits_Tab(void)	// RA2: only the backdrop shape on the sidebar surface
{
	ShapeSet const * shape = TabBackShape;	// RA2: as above
	Draw_Shape(*SidebarSurface, *SidebarDrawer, shape, 0, Point2D(0, 0), SidebarSurface->Get_Rect());	// RA2: as above
	IsToBlitSidebar = true;	// RA2: as above
}



/// <summary>
/// Flashes the credits readout to catch the player's eye.
/// This routine is used when something has happened that the player really ought to
/// notice about the state of his funds, such as running short of cash. The tab bar is
/// flagged for redraw and the money display pulses for a moment before settling down.
/// </summary>
void TabClass::Flash_Money(void)	// RA2: flags the bar and restarts the flash timer at 7 frames
{
	IsToRedraw = true;	// RA2: as above
	Flag_To_Redraw(GS_REDRAW_DIRTY);	// RA2: as above
	MoneyFlashTimer = 7;	// RA2: as above
}













































void TabClass::Redraw_Tab(void)	// RA2: out-of-line copy of the inline
{
	IsToRedraw = true;	// RA2: as above
	Flag_To_Redraw();	// RA2: as above
}



void __cdecl Load_Button_Shapes(void)	// RA2: out-of-line copy of the loop Init_For_House has
{
	char name[256];	// RA2: as above
	bool allocated = false;	// RA2: as above
	for (int index = 0; index < TabClass::BUTTON_COUNT; index++) {	// RA2: as above
		sprintf(name, "Button%02d.SHP", index);	// RA2: as above
		TabClass::ButtonShapes[index] = (ShapeSet const *)Load_Alloc_Data(name, allocated);	// RA2: as above
		TabClass::ButtonShapeAllocated[index] = allocated;	// RA2: as above
	}
}



void __cdecl Free_Button_Shapes(void)	// RA2: out-of-line copy of the loop DisposeOfArt has
{
	for (int index = 0; index < TabClass::BUTTON_COUNT; index++) {	// RA2: as above
		TabClass::Free_Button_Shape(index);	// RA2: as above
	}
}



void Init_Shape_Button(ShapeButtonClass * button, int id, unsigned flags)	// RA2: out-of-line copy of the button set-up the thumb buttons share
{
	button->ID = id;	// RA2: as above
	button->ShapeDrawer = SidebarDrawer;	// RA2: as above
	button->IsOn = false;	// RA2: as above
	button->IsToggleType = 0;	// RA2: as above
	button->Set_Flags(flags);	// RA2: as above
}



void TabClass::Setup_Command_Buttons(void)	// RA2: gives each command button its place, shape and click flags
{
	for (int index = 0; index < BUTTON_COUNT; index++) {	// RA2: as above
		CommandButtons[index].Set_Shape(NULL, 0, 0);	// RA2: as above
	}
	Rect area = *ThumbClosedRect;	// RA2: as above
	int right = area.X + area.Width;	// RA2: as above
	for (int command = 0; command < BUTTON_COUNT; command++) {	// RA2: as above
		ShapeButtonClass * button = Get_Button(command);	// RA2: as above
		if (button) {	// RA2: as above
			Rect box = *ButtonSlotRect;	// RA2: as above
			int x = SlotButton[command] * SlotPitch + box.X;	// RA2: as above
			int y = SlotTop;	// RA2: as above
			if (box.Width + x <= right) {	// RA2: as above
				button->ID = command + BUTTON_COMMAND_FIRST;	// RA2: as above
				button->ShapeDrawer = SidebarDrawer;	// RA2: as above
				button->IsOn = false;	// RA2: as above
				button->IsToggleType = 0;	// RA2: as above
				button->Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);	// RA2: as above
				button->Set_Position(x, y);	// RA2: as above
				button->Set_Shape(ButtonShapes[command], 0, 0);	// RA2: as above
			}
		}
	}
	ShapeButtonClass * planning = Get_Button(PlanningModeCommand);	// RA2: as above
	if (planning) {	// RA2: as above
		planning->IsToggleType = 1;	// RA2: as above
		planning->ReflectButtonState = true;	// RA2: as above
	}
	for (int team = Team01Command; team <= Team03Command; team++) {	// RA2: as above
		ShapeButtonClass * button = Get_Button(team);	// RA2: as above
		if (button) {	// RA2: as above
			button->Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE | GadgetClass::RIGHTPRESS | GadgetClass::RIGHTRELEASE);	// RA2: as above
		}
	}
}



void __cdecl TabClass::Setup_Thumb_Buttons(void)	// RA2: out-of-line copy of the code Init_IO has
{
	ThumbOpenButton.ID = BUTTON_THUMB_OPEN;	// RA2: as above
	ThumbOpenButton.ShapeDrawer = SidebarDrawer;	// RA2: as above
	ThumbOpenButton.IsOn = false;	// RA2: as above
	ThumbOpenButton.IsToggleType = 0;	// RA2: as above
	ThumbOpenButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);	// RA2: as above
	ShapeSet const * shape = TabThumbShape;	// RA2: as above
	Rect area = *ThumbOpenRect;	// RA2: as above
	ThumbOpenButton.Set_Position(area.X, area.Y);	// RA2: as above
	ThumbOpenButton.Set_Shape(shape, 0, 0);	// RA2: as above

	ThumbClosedButton.ID = BUTTON_THUMB_CLOSED;	// RA2: as above
	ThumbClosedButton.ShapeDrawer = SidebarDrawer;	// RA2: as above
	ThumbClosedButton.IsOn = false;	// RA2: as above
	ThumbClosedButton.IsToggleType = 0;	// RA2: as above
	ThumbClosedButton.Set_Flags(GadgetClass::LEFTPRESS | GadgetClass::LEFTRELEASE);	// RA2: as above
	Rect area2 = *ThumbClosedRect;	// RA2: as above
	ThumbClosedButton.Set_Position(area2.X, area2.Y);	// RA2: as above
	ThumbClosedButton.Set_Shape(shape, 0, 0);	// RA2: as above
}



void TabClass::Show_Buttons(void)	// RA2: adds the open bar's buttons and their tool tips
{
	Add_Command_Buttons();	// RA2: as above
	Add_A_Button(ThumbOpenButton);	// RA2: as above
	Add_Button_ToolTip(Get_Button(AttackMoveCommand), "Tip:AttackMove");	// RA2: as above
	Add_Button_ToolTip(Get_Button(BeaconCommand), "Tip:Beacon");	// RA2: as above
	Add_Button_ToolTip(Get_Button(CheerCommand), "Tip:Cheer");	// RA2: as above
	Add_Button_ToolTip(Get_Button(DeployCommand), "Tip:Deploy");	// RA2: as above
	Add_Button_ToolTip(Get_Button(GuardCommand), "Tip:Guard");	// RA2: as above
	Add_Button_ToolTip(Get_Button(PlanningModeCommand), "Tip:PlanningMode");	// RA2: as above
	{
		ShapeButtonClass * button = Get_Button(StopCommand);	// RA2: as above
		Add_Button_ToolTip(button, "Tip:Stop");	// RA2: as above
	}
	{
		ShapeButtonClass * button = Get_Button(Team01Command);	// RA2: as above
		Add_Button_ToolTip(button, "Tip:Team01");	// RA2: as above
	}
	{
		ShapeButtonClass * button = Get_Button(Team02Command);	// RA2: as above
		Add_Button_ToolTip(button, "Tip:Team02");	// RA2: as above
	}
	{
		ShapeButtonClass * button = Get_Button(Team03Command);	// RA2: as above
		Add_Button_ToolTip(button, "Tip:Team03");	// RA2: as above
	}
	{
		ShapeButtonClass * button = Get_Button(TypeSelectCommand);	// RA2: as above
		Add_Button_ToolTip(button, "Tip:TypeSelect");	// RA2: as above
	}
	Add_Button_ToolTip(&ThumbOpenButton, "Tip:ThumbOpen");	// RA2: as above
}



void TabClass::Hide_Buttons(void)	// RA2: removes the open bar's buttons and their tool tips
{
	Remove_Command_Buttons();	// RA2: as above
	Remove_A_Button(ThumbOpenButton);	// RA2: as above
	for (int command = 0; command < BUTTON_COUNT; command++) {	// RA2: as above
		ShapeButtonClass * button = Get_Button(command);	// RA2: as above
		if (button != NULL) {	// RA2: as above
			ToolTips->Remove(button->ID);	// RA2: as above
		}
	}
	ToolTips->Remove(BUTTON_THUMB_OPEN);	// RA2: as above
}



void TabClass::Show_Thumb_Button(void)	// RA2: adds the closed bar's thumb button and its tool tip
{
	Add_A_Button(ThumbClosedButton);	// RA2: as above
	Add_Button_ToolTip(&ThumbClosedButton, "Tip:ThumbClosed");	// RA2: as above
}



void TabClass::Hide_Thumb_Button(void)	// RA2: removes the closed bar's thumb button and its tool tip
{
	Remove_A_Button(ThumbClosedButton);	// RA2: as above
	ToolTips->Remove(BUTTON_THUMB_CLOSED);	// RA2: as above
}



void TabClass::Open_Thumb(void)	// RA2: swaps the closed bar's thumb button for the open bar's buttons
{
	if (!IsThumbActive) {	// RA2: as above
		Hide_Thumb_Button();	// RA2: as above
		IsThumbActive = true;	// RA2: as above
		Show_Buttons();	// RA2: as above
		IsToRedraw = true;	// RA2: as above
		Flag_To_Redraw(GS_REDRAW_DIRTY);	// RA2: as above
	}
}



void TabClass::Close_Thumb(void)	// RA2: swaps the open bar's buttons for the closed bar's thumb button
{
	if (IsThumbActive) {	// RA2: as above
		Hide_Buttons();	// RA2: as above
		IsThumbActive = false;	// RA2: as above
		Show_Thumb_Button();	// RA2: as above
		IsToRedraw = true;	// RA2: as above
		Flag_To_Redraw(GS_REDRAW_DIRTY);	// RA2: as above
	}
}



bool TabClass::Unknown_Virtual_0(int code, int)	// RA2: the planning mode switches the planning button on or off
{
	if (code == NOTICE_PLANNING_ON) {	// RA2: as above
		ShapeButtonClass * button = Get_Button(PlanningModeCommand);	// RA2: as above
		if (button != NULL && !button->IsOn) {	// RA2: as above
			button->Turn_On();	// RA2: as above
		}
		return(true);	// RA2: as above
	}
	if (code == NOTICE_PLANNING_OFF) {	// RA2: as above
		ShapeButtonClass * button = Get_Button(PlanningModeCommand);	// RA2: as above
		if (button != NULL && button->IsOn) {	// RA2: as above
			button->Turn_Off();	// RA2: as above
		}
		return(true);	// RA2: as above
	}
	return(false);	// RA2: as above
}



char const * TabClass::Help_Text(int id)	// RA2: only the sidebar's
{
	return(BASECLASS::Help_Text(id));	// RA2: as above
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
	"Team01", "Team02", "Team03", "TypeSelect", "Deploy", "AttackMove", "Guard", "Beacon", "Stop", "PlanningMode", "Cheer" // RA2: as above
}; // RA2: as above
