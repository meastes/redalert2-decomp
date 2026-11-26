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

/* $Header: /CounterStrike/TAB.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : TAB.H                                                        *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 12/15/94                                                     *
 *                                                                                             *
 *                  Last Update : December 15, 1994 [JLB]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "credits.h"
#include "abstract.h" // RA2: INoticeSink
#include "sidebar.h"
#include "shapebtn.h" // RA2: the command bar's buttons

extern int const NoAdvancedCommand; // RA2: 0x7F67F4, -1

class TabClass : public SidebarClass, INoticeSink // RA2: INoticeSink added (private, per its RTTI), vtable at +0x4EE8
{
		typedef SidebarClass BASECLASS;

	public:
		TabClass(void);
		TabClass(NoInitClass const & x) : BASECLASS(x), Credits(x), FlasherTimer(x), MoneyFlashTimer(x) {};

		virtual void AI(KeyNumType &input, Point2D const & xy) override;
		virtual void Draw_It(bool complete=false) override;
		void Draw_Credits_Tab(void); // RA2: not static
		static void Hilite_Tab(int tab);
		static int Command_From_Name(char const * name); // RA2: 0x6A0580
		static void Set_Command_Position(int command, int position); // RA2: 0x6A05E0
		static void Set_Button_Count(int count); // RA2: 0x6A0670
		static void Free_Button_Shape(int index); // RA2: 0x6A0540; frees a command button shape that was loaded
		static int Get_Command_Position(int command); // RA2: 0x6A05F0
		static ShapeButtonClass * Get_Button(int command); // RA2: 0x6A0600; the button a command is on, if any
		static Rect Get_Button_Rect(int command); // RA2: 0x6A0620
		static int Is_Team_Command(int command); // RA2: 0x6A06B0; Team01 to Team03
		static int Team_Number(int command); // RA2: 0x6A06D0; 1 to 3
		static void Add_Button_ToolTip(ControlClass * control, char const * label); // RA2: 0x6A1270
		void Add_Command_Buttons(void); // RA2: 0x6A0D50
		void Remove_Command_Buttons(void); // RA2: 0x6A0D80
		void Setup_Command_Buttons(void); // RA2: 0x6A1880
		static void __cdecl Setup_Thumb_Buttons(void); // RA2: 0x6A19E0; Init_IO has its own copy of this
		void Show_Buttons(void); // RA2: 0x6A1AB0; the open bar's buttons and tool tips
		void Hide_Buttons(void); // RA2: 0x6A1DA0
		void Show_Thumb_Button(void); // RA2: 0x6A1E20; the closed bar's button
		void Hide_Thumb_Button(void); // RA2: 0x6A1EA0
		void Open_Thumb(void); // RA2: 0x6A1EC0
		void Close_Thumb(void); // RA2: 0x6A1F10

		enum {
			BUTTON_COUNT = 25, // RA2: command buttons
			COMMAND_COUNT = 11, // RA2: command names
			BUTTON_COMMAND_FIRST = 0xD6, // RA2: gadget ID of the first command button
			BUTTON_THUMB_OPEN = 0xF0, // RA2: gadget ID of the thumb button of the open bar
			BUTTON_THUMB_CLOSED = 0xF1 // RA2: gadget ID of the thumb button of the closed bar
		};

		static ShapeButtonClass CommandButtons[BUTTON_COUNT]; // RA2: 0xABD918
		static ShapeButtonClass ThumbOpenButton; // RA2: 0xABE400; ID 0xF0, tip "Tip:ThumbOpen"
		static ShapeButtonClass ThumbClosedButton; // RA2: 0xABE390; ID 0xF1, tip "Tip:ThumbClosed"
		static ShapeSet const * ButtonShapes[BUTTON_COUNT]; // RA2: 0xABD8A0; Button00.SHP to Button24.SHP
		static bool ButtonShapeAllocated[BUTTON_COUNT]; // RA2: 0xABE32C
		static int SlotButton[BUTTON_COUNT]; // RA2: 0xABE2C8; the button position of each command

		static int ButtonWidth; // RA2: 0xABE478; of the button shapes
		static int ButtonHeight; // RA2: 0xABE2A8
		static int ButtonRows; // RA2: 0xABE2AC
		static int ButtonsLeft; // RA2: 0xABE37C; left edge of the bar's buttons
		static int SlotTop; // RA2: 0xABE374
		static int SlotPitch; // RA2: 0xABE388
		static int SlotGap; // RA2: 0xABE460; the slot width less the button width

		static int AttackMoveCommand; // RA2: 0xABE464; the commands' numbers, from their names
		static int BeaconCommand; // RA2: 0xABE294
		static int CheerCommand; // RA2: 0xABD910
		static int DeployCommand; // RA2: 0xABE278
		static int GuardCommand; // RA2: 0xABE2B8
		static int PlanningModeCommand; // RA2: 0xABE36C
		static int StopCommand; // RA2: 0xABE2BC
		static int Team01Command; // RA2: 0xABE370
		static int Team02Command; // RA2: 0xABE378
		static int Team03Command; // RA2: 0xABE468
		static int TypeSelectCommand; // RA2: 0xABE290
		void Flash_Money(void);

		virtual void One_Time(void) override;							// One-time inits
		void Redraw_Tab(void); // RA2: 0x6A17B0; out of line

		virtual void Init_For_House(void) override;
		virtual void Init_IO(void) override; // RA2: 0x6A0C50; sets up the command buttons and the two thumb buttons
		virtual void DisposeOfArt(void) override; // RA2: 0x6A0B30; frees the command button shapes
		virtual bool Activate(int control) override; // RA2: 0x6A0DA0 (vtable slot 54); the sidebar shown or hidden: updates the bar's buttons and tool tips
		virtual char const * Help_Text(int id) override; // RA2: 0x6A20B0; only the sidebar's

		virtual bool Unknown_Virtual_0(int, int) override; // RA2: TabClass's INoticeSink virtual

		CreditClass Credits;

		CDTimerClass<FrameTimerClass> FlasherTimer;

	private:
		void Set_Active(int select);

		CDTimerClass<FrameTimerClass> MoneyFlashTimer;

	public: // RA2: HouseClass::MPlayer_Defeated reads and closes the thumb
		bool IsThumbActive; // RA2: +0x4F14; picks which of two buttons the tab bar draws
	protected:

		/*
		**	If the tab graphic is to be redrawn, then this flag is true.
		*/
		bool IsToRedraw; // RA2: after MoneyFlashTimer, at +0x4F15

	public:
		bool IsDrawn; // RA2: +0x4F16; set once the tab bar has been drawn; public, Update_Visible_Surface reads it

	private:

		static ShapeSet const * TabShape;
};

/*
**	RA2: the shapes and areas of the command bar, which the sidebar's layout sets up (none of it is in this file's module).
*/
extern Rect * ThumbOpenRect; // RA2: 0xAC1204; where the thumb button of the open bar is
extern Rect * ThumbClosedRect; // RA2: 0xAC1208; where the thumb button of the closed bar is
extern Rect * ButtonSlotRect; // RA2: 0xAC120C; the first button slot
extern Rect * TabEndRect; // RA2: 0xAC1210; the bar's right end
extern char const * const CommandNames[TabClass::COMMAND_COUNT]; // RA2: 0x7F67F8; the commands' names, in the order of their numbers
extern ShapeSet const * TabLeftShape; // RA2: 0xAC1048
extern ShapeSet const * TabThumbShape; // RA2: 0xAC1098; the thumb buttons' shape
extern ShapeSet const * TabTileShape; // RA2: 0xAC10A4; repeated TabTileCount times between the thumb button and the right end
extern ShapeSet const * TabRightShape; // RA2: 0xAC10B0
extern ShapeSet const * TabBackShape; // RA2: 0xAC10F8; the bar's backdrop on the sidebar surface
extern int TabTileCount; // RA2: 0xAC0FF4
extern int TabBarHeight; // RA2: 0x7ADD84; the mouse region of the bar is this less 2 high
bool Is_Sidebar_Art_Loaded(void); // RA2: 0x6F4590 (the sidebar layout's module); the flag at 0xAC11B0
void Planning_Set_Sink(INoticeSink * sink); // RA2: 0x613E00 (the planning mode's module); stores the sink at 0xA76680

/*
**	RA2: the commands of the bar's buttons (the module of the user interface commands, D:\Ra2\UICmnds.cpp).
*/
void __cdecl Command_Attack_Move(void); // RA2: 0x6F6570; refused with nothing selected or a selected object that cannot
void __cdecl Command_Beacon(void); // RA2: 0x6F64B0
void __cdecl Command_Cheer(void); // RA2: 0x6F5C50; queues a cheer event once MaximumCheerRate frames have passed
void __cdecl Command_Deploy(void); // RA2: 0x6F5940
void Command_Guard(void); // RA2: 0x6F5B10
void Command_Planning_Mode_On(bool sound); // RA2: 0x6F64D0
void Command_Planning_Mode_Off(bool sound); // RA2: 0x6F64F0
void __cdecl Command_Stop(void); // RA2: 0x6F5BC0
void __fastcall Command_Type_Select(void); // RA2: 0x6F7360
int Group_Count(int group); // RA2: 0x6F5870; the player's technos in a team
void Group_Assign(int group); // RA2: 0x6F5D90; the selection becomes the team
bool Group_Is_All_Selected(int group); // RA2: 0x6F5800
void Group_Reselect(int group); // RA2: 0x6F60D0
void Group_Select(int group); // RA2: 0x6F5F00
void Group_Clear(int group); // RA2: 0x6F5E10
